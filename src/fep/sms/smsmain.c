#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "ibdcs.h"
#include "tmcibtms.h"
#include "dbstruct.h"
#include "../db/inc/glbdb.h"
#include "CMDI.h"
#include "iso8583.h"
#include "bc.h"

extern gs_myFid, gs_appSvrFid;
char g_iSrcFoldName[30+1] ;

static void appSrvExit(int signo)
{
    if ( signo > 0 )
        dcs_log(0,0,"<FILE:%s,LINE:%d>catch a signal %d\n",__FILE__,__LINE__,signo);
    if ( signo !=0 && signo != SIGTERM && signo != SIGSEGV )
        return;

    dcs_log(0,0,"<FILE:%s,LINE:%d>AppServer terminated.\n",__FILE__,__LINE__);
    DasEnd( COMMIT );
    shm_detach((char *)g_pcBcdaShmPtr);
    exit(signo);
}

static int OpenLog(char *IDENT)
{
    char logfile[256];

    if ( u_fabricatefile("log/smsapp.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

static int CreateMyFolder()
{
    if ( fold_initsys() < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d>cannot attach to folder system!", __FILE__, __LINE__);
        return -1;
    }
    
    gs_myFid = fold_create_folder("SMSS");
    if ( gs_myFid < 0 )
        gs_myFid = fold_locate_folder("SMSS");
    if ( gs_myFid < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>cannot create folder '%s':%d\n", __FILE__, __LINE__, "SMSS", (errno) );
        return -1;
    }

    dcs_log(0,0, "<FILE:%s,LINE:%d>folder myFid=%d\n",__FILE__,__LINE__, gs_myFid);

    if ( fold_get_maxmsg(gs_myFid) <2 )
        fold_set_maxmsg(gs_myFid, 20) ;

    return 0;
}

/*处理终端渠道上送交易报文*/
int process( char* srcBuf,int  iFid, int iLen )
{
	ISO_data iso;
	char buff[1024];
	char msgtype[5],process[7];
	int len;
    /*解包*/
    memset(&iso, 0, sizeof(iso));
    if ( 0!= BC_SetIsoMode(&iso, "JAVA") )
    {
        dcs_log(srcBuf, iLen, "<FILE:%s,LINE:%d><BC_UnPack>BC_SetIsoMode failure",__FILE__,__LINE__);
        return -1;
    }    
    if ( 0 > BC_UnPack_Data( &iso, srcBuf, iLen) )
    {
        dcs_log(srcBuf,iLen,"<FILE:%s,LINE:%d>Can not unpacket Terminal Msg!datalen=%d",__FILE__,__LINE__,iLen);
        return;
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d>g_iSrcFoldName[%s]",__FILE__,__LINE__,g_iSrcFoldName);

    memset(msgtype,0,sizeof(msgtype));
    memset(process,0,sizeof(process));
    PrintISO( &iso,"收到报文",0);
   	if ( (len = getbit(&iso,  0, buff)) > 0 )
    {
        memcpy(msgtype, buff, MIN(len,  4));
    }
    if ( (len = getbit(&iso,  3, buff)) > 0 )
    {
        memcpy(process, buff, MIN(len,  6));
    }
    if(memcmp(process, "510000", 6) == 0 && memcmp(msgtype, "0520", 4) == 0)//签到交易
    {
    	Sign(&iso);
    }
	else if(memcmp(process, "820001", 6) == 0) 
	{
		if(memcmp(msgtype, "0720", 4) == 0)          //交易申请
		{
			SendMsg(&iso);	
		}
		else
	    	dcs_log(0,0, "<FILE:%s,LINE:%d>Msgtype[%s] error!",__FILE__,__LINE__,msgtype);
	}
	else
	{
    	dcs_log(0,0, "<FILE:%s,LINE:%d>Process[%s] error!",__FILE__,__LINE__,process);
	}	
}


static int DoLoop()
{
    //char   caBuffer[4096];
    char *caBuffer = (char *)g_Rcv_Buffer;
    int iRead, fromFid;
    for ( ;; )
    {
        g_ForkFlag = 0;
        //read from my folder in blocking mode
        memset(caBuffer, 0, sizeof(caBuffer));
        iRead = fold_read( gs_myFid, &fromFid, g_Rcv_Buffer, sizeof(g_Rcv_Buffer)-8, 1);
        if ( iRead < 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d>fold_read() failed:%d\n",__FILE__,__LINE__,(errno));
            break;
        }
//		if(memcmp(g_Rcv_Buffer, "HTTP", 4) == 0)//http异步返回结果
//		{
//			ExpendAppReply(g_Rcv_Buffer+4, iRead-4, 1, NULL);
//		}
		else
		{
	        g_RcvBufferSize = iRead;
	        g_iSrcFoldId = fromFid;
	        dcs_debug(0,0, "<FILE:%s,LINE:%d>srcfolder[%d]!",__FILE__,__LINE__,g_iSrcFoldId);	
	        fold_get_name(g_iSrcFoldId, g_iSrcFoldName, 30);
	        g_iSrcFoldName[30] = 0;
	
	        //folder msg
	        memset(&g_folder_msg,0,sizeof(g_folder_msg));
	        memcpy(g_folder_msg.packet_type, "JAVA", 4);
	        g_folder_msg.srcFoldid = g_iSrcFoldId;
	        g_folder_msg.OutFoldType = 0;
	        memset(g_srcPacketType,0, sizeof(g_srcPacketType) );
	        memcpy(g_srcPacketType, "JAVA", 4);
	        dcs_debug(caBuffer, iRead, "recv data len=%d(from foldid:[%d],foldName[%s],packet_type:[%s])", iRead,g_iSrcFoldId, g_iSrcFoldName, g_srcPacketType);
	
	        //交易信息
	        process(caBuffer,fromFid,iRead);
	    }
    }//for(;;)

    return -1;
}

int main(int argc, char *argv[])
{
    catch_all_signals(appSrvExit);
    if ( 0 > OpenLog(argv[0]))
    {
        BC_SendExeception("04","<FILE:%s,LINE:%d>打开日志失败，smsapp进程启动失败，退出");
        appSrvExit(0);
    }
    dcs_log(0,0, "<FILE:%s,LINE:%d>smsapp Servers is starting up...\n",__FILE__,__LINE__);
    
    u_daemonize(NULL);
    
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",__FILE__,__LINE__ ,strerror(errno));
        appSrvExit(0);
    }
    
    g_pcBcdaShmPtr = (char *)shm_connect("BCDA",NULL);
    if ( g_pcBcdaShmPtr == NULL )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><appSrv> cannot connect shared memory 'BCDA'!",__FILE__,__LINE__);
        appSrvExit(0);
    }

    if ( IsoLoad8583config(&iso8583_JAVAconf[0],"iso8583_JAVA.conf") < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load JAVA system interface iso8583_JAVAconf() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }
    
    if ( DasConectDB()!=0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>Can not open oracle DB !",__FILE__,__LINE__);
        BC_SendExeception("04","连接oracle DB失败，appSrv进程启动失败，退出");
        appSrvExit(0);
    }


    if ( CreateMyFolder() < 0 )
    {
        dcs_log(0, 0, "创建folder文件夹失败，smsapp进程启动失败，退出");
        appSrvExit(0);
    }
    
    gs_appSvrFid = fold_locate_folder("APPL");
    if ( gs_appSvrFid < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>cannot create folder '%s':%d\n",__FILE__,__LINE__, "APPL", (errno) );
        appSrvExit(0);
    }
   
    if ( 1 != DESInit() )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d> DESInit() failed\n",__FILE__,__LINE__);
        appSrvExit(0);
    }
	
	if ( 0 > LoadConfig())
	{
        dcs_log(0,0,"<FILE:%s,LINE:%d> LoadConfig() failed\n",__FILE__,__LINE__);
        appSrvExit(0);
	}
    //do my main logic in a infinite loop    

    dcs_log(0,0,"*************************************************\n"
            "!!      SMS Application Servers startup completed. !!\n"
            "*************************************************\n");

    
    DoLoop();
    appSrvExit(0);
}
