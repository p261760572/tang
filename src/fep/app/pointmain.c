#include "ibdcs.h"
#include "tmcibtms.h"
#include "dbstruct.h"
#include "../db/inc/glbdb.h"
#include "iso8583.h"
#include <string.h>
#include <stdlib.h>

#define MIN(a,b) a>b?b:a

int gs_myFid    = -1;
char   *g_pcBcdaShmPtr;
static void appSrvExit(int signo);
struct ISO_8583 iso8583_POINTconf[128];
int g_iSrcFoldId = -1;
char g_iSrcFoldName[30+1] ;
char g_srcPacketType[5];
char g_Rcv_Buffer[4096] ;
int g_RcvBufferSize ;
int g_FromFolderId = -1;

// 终端信息表
//ti_bc_term_inf_def g_tiBcTermInf;
//char g_center_ins_id_cd[8+1] = "09005500";

//超时表结构
struct TimeOut_REC g_TimeOutRec;
MONI_STRUCT g_Moni_Data;

static int OpenLog(char *IDENT);
static int CreateMyFolder();
static int DoLoop();
/*
 *******************************
   main  
********************************/
int main(int argc, char *argv[])
{

    char configfile[500];
    catch_all_signals(appSrvExit);
    signal(17, SIG_IGN);

    // glb_SetErrLog_FuncPtr((int)&BC_ErrLog);

    // strcpy( g_center_ins_id_cd, "09005500");

    //prepare the logging stuff
    if ( 0 > OpenLog(argv[0]) )
    {
        BC_SendExeception("04","<FILE:%s,LINE:%d>打开日志失败，appSrv进程启动失败，退出");
        exit(1);
    }

    dcs_log(0,0, "<FILE:%s,LINE:%d>appSrv Servers is starting up...\n",__FILE__,__LINE__);
    u_daemonize(NULL);

    //attach to SHM of IBDCS
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",strerror(errno),__FILE__,__LINE__);
        BC_SendExeception("04","连接SHM失败，appSrv进程启动失败，退出");
        exit(1);
    }
    g_pcBcdaShmPtr = (char *)shm_connect("BCDA",NULL);
    if ( g_pcBcdaShmPtr == NULL )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><appSrv> cannot connect shared memory 'BCDA'!",__FILE__,__LINE__);
        BC_SendExeception("04","连接共享内存BCDA失败，appSrv进程启动失败，退出");
        return(-1);
    }

    if ( DasConectDB()!=0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>Can not open oracle DB !",__FILE__,__LINE__);
        BC_SendExeception("04","连接oracle DB失败，appSrv进程启动失败，退出");
        appSrvExit(0);
    }

    /*
    if ( IsoLoad8583config(&iso8583_TERMconf[0],"iso8583_TERM.conf") < 0 )
    {

        dcs_log(0,0,"<FILE:%s,LINE:%d>Load term system interface iso8583config() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        BC_SendExeception("04","装载iso8583_TERM.conf失败，appSrv进程启动失败，退出");
        appSrvExit(0);
    }

    if ( IsoLoad8583config(&iso8583_MCHNconf[0],"iso8583_MCHN.conf") < 0 )
    {
        BC_SendExeception("04","装载iso8583_MCHN.conf失败，appSrv进程启动失败，退出");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load mchn system interface iso8583config() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }

    if ( IsoLoad8583config(&iso8583_POSBconf[0],"iso8583_POSB.conf") < 0 )
    {
        BC_SendExeception("04","装载iso8583_POSB.conf失败，appSrv进程启动失败，退出");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load POSB system interface iso8583config() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }*/

    if ( IsoLoad8583config(&iso8583_POINTconf[0],"iso8583_POINT.conf") < 0 )
    {
        BC_SendExeception("04","iso8583_AJXT.conf失败，appSrv进程启动失败，退出");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load POSB system interface iso8583_AJXTconf() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }

    if ( CreateMyFolder() < 0 )
    {
        BC_SendExeception("04","创建folder文件夹失败，appSrv进程启动失败，退出");
        appSrvExit(0);
    }


    //do my main logic in a infinite loop    

    dcs_log(0,0,"*************************************************\n"
            "!!        Application Servers startup completed.        !!\n"
            "*************************************************\n");

    if ( 1 != DESInit() )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d> DESInit() failed:%s\n",__FILE__,__LINE__);
        BC_SendExeception("04","Des初始化失败，appSrv进程启动失败，退出");
        appSrvExit(0);
    }

    DoLoop();

    appSrvExit(0);
}//main()

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

    //assuming the log file is "$FEP_RUNPATH/log/appsvr1.log"
    if ( u_fabricatefile("log/pointsvr.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

static int CreateMyFolder()
{
    if ( fold_initsys() < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d>cannot attach to folder system!",__FILE__,__LINE__);
        return -1;
    }

    gs_myFid = fold_create_folder("APPL");
    if ( gs_myFid < 0 )
        gs_myFid = fold_locate_folder("APPL");
    if ( gs_myFid < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>cannot create folder '%s':%d\n",__FILE__,__LINE__, "APPL", (errno) );
        return -1;
    }

    dcs_log(0,0, "<FILE:%s,LINE:%d>folder myFid=%d\n",__FILE__,__LINE__, gs_myFid);

    if ( fold_get_maxmsg(gs_myFid) <2 )
        fold_set_maxmsg(gs_myFid, 20) ;

    return 0;
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

        g_RcvBufferSize = iRead;

        //save --- 
        //folder id
        g_iSrcFoldId = fromFid;

        //foldnm
        //LocateNameByFdid(g_iSrcFoldId,g_iSrcFoldName);
        //fold_get_name(g_iSrcFoldId, g_iSrcFoldName, 30);
        //g_iSrcFoldName[30] = 0;

        //folder msg
        memset(&g_folder_msg,0,sizeof(g_folder_msg));
        memcpy(g_folder_msg.packet_type,caBuffer,4);
        g_folder_msg.srcFoldid = g_iSrcFoldId;
        g_folder_msg.OutFoldType = 0;

        memset(g_srcPacketType,0, sizeof(g_srcPacketType) );
        memcpy(g_srcPacketType, caBuffer, 4);

        dcs_debug(caBuffer,iRead,"recv data len=%d(from foldid:[%d],foldName[%s],packet_type:[%s])",iRead,g_iSrcFoldId, g_iSrcFoldName, g_srcPacketType);

        //process
        if ( 0 == memcmp(caBuffer,"CMDI",4) )
        {
            //命令行
            process_cmdi(caBuffer,fromFid,iRead);
        } else
        {
            //交易信息
            process(caBuffer,fromFid,iRead);
        }

    }//for(;;)

    return -1;
}

int process_cmdi( char* srcBuf,int  iFid, int iLen )
{
    char *cmd_type = srcBuf+4;
    char *cmd_data = srcBuf+8;

    if ( 0 == memcmp(cmd_type, "0001", 4) )
    {
        //超时命令
        //process_cmdi_timeout(cmd_data, iFid, iLen-8);
    } else if ( 0 == memcmp(cmd_type, "0002", 4) )
    {
        //process_cmdi_timeout(cmd_data, iFid, iLen-8);
    } else if ( 0 == memcmp(cmd_type, "0003", 4) )
    {
        //机构签到
        //账单商户代签到
    }
    return 0;
}

//srcBuf 前4位为报文类型，解包后的数据存放在srcBuf+4的位置
int PT_UnPack( ISO_data *iso, char *srcBuf,  int iLen)
{
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>into BC_UnPack",__FILE__,__LINE__);
    memset(iso,0, sizeof(ISO_data));
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>start to call BC_SetIsoMode2!",__FILE__,__LINE__);
    if ( 0!= BC_SetIsoMode2(iso, srcBuf) )
    {
        dcs_log(srcBuf, iLen, "<FILE:%s,LINE:%d><BC_UnPack>BC_SetIsoMode failure",__FILE__,__LINE__);
        return -1;
    }
    if ( iLen < 20 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BC_UnPack>public system Msg error",__FILE__,__LINE__);
        return -1;
    }
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>start to call strtoiso!",__FILE__,__LINE__);
    if ( strtoiso((unsigned char *)srcBuf+4,iso,iLen-4)<0 )
    {
        dcs_log(srcBuf, iLen,"<FILE:%s,LINE:%d><BC_UnPack>cant not analyse got message(%d bytes) from ter-->",__FILE__,__LINE__, iLen);
        return -1;            
    }
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>BC_UnPack complated!",__FILE__,__LINE__);
    return 1;
}


/*处理终端渠道上送交易报文格式为8583终端报文*/
int process( char* srcBuf,int  iFid, int iLen )
{
    char buff[100];
    int iRc;
    ISO_data iso;

    /*解包*/
    if ( 0 > BC_UnPack( &iso, srcBuf, iLen) )
    {
        dcs_log(srcBuf,iLen,"<FILE:%s,LINE:%d>Can not unpacket Terminal Msg!datalen=%d",__FILE__,__LINE__,iLen);
        return;
    }
    PrintISO( &iso,"上送报文",0);

    //读取商户，终端信息

    //check mac

    //重新计算mac

    //打包

    return 0;
}


