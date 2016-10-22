#include "ibdcs.h"
#include "tmcibtms.h"
#include "dbstruct.h"
#include "../db/inc/glbdb.h"
#include "CMDI.h"
#include "bc.h"
#include "iso8583.h"
#include <string.h>
#include <stdlib.h>

#define MIN(a,b) a>b?b:a

int gs_myFid    = -1;
static void appSrvExit(int signo);
char caMRID[30][20];
static int g_iSum=0;
static int g_iPos=0;

static int OpenLog(char *IDENT);
static int CreateMyFolder();
static int DoLoop();
ti_bc_ivr_list_inf_def     g_tiBcIvrListInf;
/*
 *******************************
   main  
********************************/
int main(int argc, char *argv[])
{

    char configfile[500];
    catch_all_signals(appSrvExit);
    //signal(17, SIG_IGN);

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

    if ( (g_iSum = get_ivr_routename()) < 0)
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d> get_ivr_routename() failed:%s\n",__FILE__,__LINE__);
        BC_SendExeception("04","get_ivr_routename初始化失败，appSrv进程启动失败，退出");
        appSrvExit(0);
    }

    g_iPos = 0;
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

    if ( u_fabricatefile("log/ivrappsvr.log",logfile,sizeof(logfile)) < 0 )
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

    gs_myFid = fold_create_folder("IVRAPP");
    if ( gs_myFid < 0 )
        gs_myFid = fold_locate_folder("IVRAPP");
    if ( gs_myFid < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>cannot create folder '%s':%d\n",__FILE__,__LINE__, "IVRAPP", (errno) );
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
        //iRead = fold_read( gs_myFid, &fromFid, caBuffer, sizeof(caBuffer), 1);
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
        dcs_log(0,0, "<FILE:%s,LINE:%d>srcfolder[%d]!",__FILE__,__LINE__,g_iSrcFoldId);

        //foldnm
        //LocateNameByFdid(g_iSrcFoldId,g_iSrcFoldName);
        fold_get_name(g_iSrcFoldId, g_iSrcFoldName, 30);
        g_iSrcFoldName[30] = 0;

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
    char caCommand[1024];
    char Rsp[1024],destBuf[1024],caTmp[1024],caTmpBuf[1024],packkeyid[1024];
    int  idestLen;
    int  iRet, i;
    char *cmd_type = srcBuf+4;
    char *cmd_data = srcBuf+8;
    int  sqlCode; 
    ti_bc_ivr_list_inf_def *p_tiBcIvrListInf= &g_tiBcIvrListInf;

    memset( destBuf, 0, sizeof(destBuf));
    memset( packkeyid, 0, sizeof(packkeyid));
    memset( Rsp, 0, sizeof(Rsp));
    if ( 0 == memcmp(cmd_type, "0005", 4) )
    {
        dcs_debug(cmd_data,iLen-8,"gygy");
	iRet = BC_ApplConvToIvr( srcBuf,  iLen, destBuf, &idestLen, packkeyid);
	if (iRet == -2)
	{
              	memset( caTmp, 0, sizeof(caTmp));
               	memcpy( caTmp, srcBuf+13, 50);
		if (!memcmp(srcBuf+8, "OP01", 4))
               		sprintf( destBuf, "CMDI0005OP031%s<~~RSPCODE~~>NOBACK<~~/~~>", caTmp);
		if (!memcmp(srcBuf+8, "OP02", 4))
               		sprintf( destBuf, "CMDI0005OP041%s<~~RSPCODE~~>NOBACK<~~/~~>", caTmp);
		business_prepare_return(destBuf,idestLen);
	}
	else if (iRet == 0)
	{
    		memset(g_folder_msg.data,0 ,sizeof(g_folder_msg.data));
		memcpy( g_folder_msg.data, destBuf, idestLen);
    		memset(g_folder_msg.OutFoldName,0 ,sizeof(g_folder_msg.OutFoldName));
        	memset(&g_tiBcIvrListInf, 0, sizeof(ti_bc_ivr_list_inf_def));
        	strcpy(g_tiBcIvrListInf.packkeyid, packkeyid);
        	dcs_debug(packkeyid,strlen(packkeyid),"gygy222");
        	tiBcIvrListInfOpr(GLB_DB_SELECT1, p_tiBcIvrListInf, &sqlCode);
		if (sqlCode == 0)
		{
        		rtrim(p_tiBcIvrListInf->foldername);
			strcpy(g_folder_msg.OutFoldName, p_tiBcIvrListInf->foldername);
        		rtrim(g_folder_msg.OutFoldName);
		}
		else 
		{
			for (i=0; i < g_iSum; i++)
			{
				if (g_iPos < g_iSum)
				{	
					strcpy(g_folder_msg.OutFoldName, caMRID[g_iPos]);
				}
				else
				{
					g_iPos = 0;
        				strcpy(g_folder_msg.OutFoldName, caMRID[g_iPos]);
				}
  				rtrim(g_folder_msg.OutFoldName);
    				if ( IsReady( g_folder_msg.OutFoldName )<0 )
    				{
					g_iPos ++;
    				}
				else
				{
					dcs_log(g_folder_msg.OutFoldName,strlen(g_folder_msg.OutFoldName),"<FILE:%s,LINE:%d>使用路由偏移数[%d]\n",__FILE__,__LINE__,g_iPos);
					memset(&g_tiBcIvrListInf, 0, sizeof(ti_bc_ivr_list_inf_def));
					strcpy(g_tiBcIvrListInf.packkeyid, packkeyid);
					strcpy(g_tiBcIvrListInf.foldername, g_folder_msg.OutFoldName);
					tiBcIvrListInfOpr(GLB_DB_INSERT1, p_tiBcIvrListInf, &sqlCode);
					if ((sqlCode != 0) && (sqlCode != SQL_ERD_UNIKEY))
					{
						return -1;
					}
					g_iPos ++;
					break;
				}
			}
			if ( i == g_iSum )
			{
                		memset( destBuf, 0, sizeof(destBuf));
                		memset( caTmp, 0, sizeof(caTmp));
                		memcpy( caTmp, srcBuf+13, 50);
				if (!memcmp(srcBuf+12, "0", 1))
				{
					if (!memcmp(srcBuf+8, "OP01", 4))
                				sprintf( destBuf, "CMDI0005OP031%s<~~RSPCODE~~>NOBACK<~~/~~>", caTmp);
					if (!memcmp(srcBuf+8, "OP02", 4))
                				sprintf( destBuf, "CMDI0005OP041%s<~~RSPCODE~~>NOBACK<~~/~~>", caTmp);
					business_prepare_return(destBuf,strlen(destBuf));
                			dcs_log(destBuf,strlen(destBuf),"<FILE:%s,LINE:%d>IVR通讯不可使用，代IVR发请求给APPL，NOBACK .",__FILE__,__LINE__);
                			return -2;
				}
				else
				{
                			dcs_log(destBuf,strlen(destBuf),"<FILE:%s,LINE:%d>IVR通讯不可使用，APPL回应给不到IVR,NOSEND .",__FILE__,__LINE__);
                			return -2;
				}
			}
		}
		g_folder_msg.datalen= idestLen;
		g_folder_msg.OutFoldType = 1;
		BC_SendMessageToFolder(Rsp);
	}
    }
    else if ( 0 == memcmp(cmd_type, "0004", 4) )
    {
        idestLen = 0;
        memcpy( destBuf+idestLen, "30", 2);
        idestLen = idestLen + 2;
        memcpy( destBuf+idestLen, INS_IVR, 8);
        idestLen = idestLen + 8;
        memcpy( destBuf+idestLen, "SIGN12345678901234567890123456789012345", 39);
        idestLen = idestLen + 39;
        memcpy( destBuf+idestLen, "000000FFFFFFFF", 14);
        idestLen = idestLen + 14;
    	memset(g_folder_msg.data,0 ,sizeof(g_folder_msg.data));
	memcpy( g_folder_msg.data, destBuf, idestLen);
    	memset(g_folder_msg.OutFoldName,0 ,sizeof(g_folder_msg.OutFoldName));
	for (i=0; i < g_iSum; i++)
	{
		if (g_iPos < g_iSum)
		{
       			strcpy(g_folder_msg.OutFoldName, caMRID[g_iPos]);
		}
		else
		{
			g_iPos = 0;
       			strcpy(g_folder_msg.OutFoldName, caMRID[g_iPos]);
		}
       		rtrim(g_folder_msg.OutFoldName);
               	if ( IsReady( g_folder_msg.OutFoldName )<0 )
               	{
			g_iPos ++;
               	}
		else
		{
			g_iPos ++;
			break;
		}
	}
	if ( i == g_iSum )
	{
               	dcs_log(destBuf,strlen(destBuf),"<FILE:%s,LINE:%d>IVR通讯不可使用，APPL回应给不到IVR,NOSEND .",__FILE__,__LINE__);
               	return -2;
	}
	g_folder_msg.datalen= idestLen;
	g_folder_msg.OutFoldType = 1;
	BC_SendMessageToFolder(Rsp);
    }
	
    return 0;
}

/*处理终端渠道上送交易报文格式为8583终端报文*/
int process( char* srcBuf,int  iFid, int iLen )
{
    char buff[100];
    char Rsp[1024],msgtype[5],process[7],processno[7],SerCode[3],mchnt_tp[5],packet_type[5];
    int iRc;
    char destBuf[1024];
    char pinkey[33],mackey[33],datakey[33],oprdata[100],caItem[1024];
    ti_bc_ins_inf_def tiBcInsInf;
    int  sqlCode,iOffset=0,i,idestLen, oprLen;

    memset(msgtype,0,sizeof(msgtype));
    memset(process,0,sizeof(process));
    memset(processno,0,sizeof(processno));
    memset(SerCode,0,sizeof(SerCode));
    memset(mchnt_tp,0,sizeof(mchnt_tp));
    memset( Rsp, 0, sizeof(Rsp));

    memset(packet_type,0,sizeof(packet_type));
    memcpy(packet_type, srcBuf, 4);

    if (!memcmp( srcBuf, "31", 2))
    {
    	memset(pinkey,0,sizeof(pinkey));
    	memset(mackey,0,sizeof(mackey));

    	memset(oprdata,0,sizeof(oprdata));
	memcpy( oprdata, srcBuf+52, 3);
	oprLen = atoi(oprdata);
	if (oprLen == 0)
	{
        	dcs_debug(srcBuf,iLen,"回应报文没有指令!");
		return -1;
	}
       	dcs_debug(srcBuf,iLen,"[%d]!",oprLen);
	for (i=0; i<(oprLen/2); i++)
	{
    		memset(oprdata,0,sizeof(oprdata));
		memcpy(oprdata, srcBuf+52+3+i*2, 2);
		if (!memcmp(oprdata, "20", 2))
		{
    			memset(caItem,0,sizeof(caItem));
			memcpy(caItem, srcBuf+52+3+oprLen+3+iOffset, 6);
			if (memcmp(caItem, "N00000", 6))
			{
        			dcs_debug(srcBuf,iLen,"回应报文是失败回应!");
				return -1;				
			}
			else
			{
				iOffset = iOffset+6;
			}
		}
		else if (!memcmp(oprdata, "26", 2))
		{
    			memset(caItem,0,sizeof(caItem));
			memcpy(caItem, srcBuf+52+3+oprLen+3+iOffset, 8);
        		dcs_debug(caItem,8,"mackey!\n");
                	bcd_to_asc((unsigned char*)mackey,(unsigned char*)caItem,16,0);
			iOffset = iOffset+8;
		}
		else if (!memcmp(oprdata, "27", 2))
		{
    			memset(caItem,0,sizeof(caItem));
			memcpy(caItem, srcBuf+52+3+oprLen+3+iOffset, 8);
        		dcs_debug(caItem,8,"pinkey!\n");
                	bcd_to_asc((unsigned char*)pinkey,(unsigned char*)caItem,16,0);
			iOffset = iOffset+8;
		}
		else if (!memcmp(oprdata, "29", 2))
		{
    			memset(caItem,0,sizeof(caItem));
			memcpy(caItem, srcBuf+52+3+oprLen+3+iOffset, 8);
        		dcs_debug(caItem,8,"datakey!\n");
                	bcd_to_asc((unsigned char*)datakey,(unsigned char*)caItem,16,0);
			iOffset = iOffset+8;
		}
	}
	memset((char *)&tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
        memcpy(tiBcInsInf.ins_id_cd, INS_IVR, 8);
        memcpy(tiBcInsInf.key_1, mackey, 16);
        memcpy(tiBcInsInf.key_2, datakey, 16);
        memcpy(tiBcInsInf.key_3, pinkey, 16);
        tiBcInsInfOpr(GLB_DB_UPDATE2, &tiBcInsInf, &sqlCode);
    	if ( (-3114 == sqlCode) || (-3135 == sqlCode))
    	{
        	BC_SendExeception("03","数据库连接异常，自动重连数据库。");
        	dcs_debug(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);
   	 
        	if ( DasConectDB()==0 )
        	{
        		tiBcInsInfOpr(GLB_DB_UPDATE2, &tiBcInsInf, &sqlCode);
        	}
    	}
        if ( 0!= sqlCode )
        {
            dcs_debug(0,0,"更新密钥信息失败[返回mackey[%s],pinkey[%s]].\n", tiBcInsInf.key_1,tiBcInsInf.key_2);
            return -1;
        }	
        dcs_debug(0,0,"向IVR签到成功\n");
    }
    else
    {
    	memset( destBuf, 0, sizeof(destBuf));
    	iRc = BC_IvrConvToAppl( srcBuf,  iLen, destBuf, &idestLen);
    	if (iRc == -2)
    	{
		business_prepare_return(destBuf,idestLen);
    	}
    	else if (iRc == 0)
    	{
    		memset(g_folder_msg.data,0 ,sizeof(g_folder_msg.data));
		memcpy( g_folder_msg.data, destBuf, idestLen);
    		memset(g_folder_msg.OutFoldName,0 ,sizeof(g_folder_msg.OutFoldName));
       		strcpy(g_folder_msg.OutFoldName, "APPL");
       		rtrim(g_folder_msg.OutFoldName);
		g_folder_msg.datalen= idestLen;
		g_folder_msg.OutFoldType = 1;
		BC_SendMessageToFolder(Rsp); 
    	}
    }

    return 0;
}

int business_prepare_return(char *buff, int ilen)
{
    dcs_log(0,0,"<FILE:%s,LINE:%d><business_prepare_return>交易没有成功，准备返回。\n",__FILE__,__LINE__);
    BC_SendMessageToFolder_direct( buff, ilen);
    return 0;
}

int get_ivr_routename()
{
   FILE *fp;
   char  caFileName[128];
   char  caFileLine[1024];
   int   iLoop;

   memset( caFileName, 0, sizeof( caFileName ) );
   sprintf( caFileName, "%s/config/ivrroutename.conf", getenv("FEP_RUNPATH") );
   fp = NULL;
   if ( ( fp = fopen( caFileName, "r" ) ) == NULL )
   {
    	dcs_log(0,0,"<FILE:%s,LINE:%d><get_ivr_routename> 文件不存在，准备返回。\n",__FILE__,__LINE__);
    	return -1;
   }
   memset( caFileLine, 0, sizeof( caFileLine ) );
   fgets( caFileLine, sizeof( caFileLine )-1, fp );
   fclose( fp );

   for ( iLoop = 0; iLoop < 30; iLoop++ )
       memset( caMRID[iLoop], 0, sizeof( caMRID[iLoop] ) );
   sscanf( caFileLine, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", 
           caMRID[0], caMRID[1], caMRID[2], caMRID[3], caMRID[4], caMRID[5], 
           caMRID[6], caMRID[7], caMRID[8], caMRID[9], caMRID[10], caMRID[11], 
           caMRID[12], caMRID[13], caMRID[14], caMRID[15], caMRID[16], 
           caMRID[17], caMRID[18], caMRID[19], caMRID[20], caMRID[21], 
           caMRID[22], caMRID[23], caMRID[24], caMRID[25], caMRID[26], 
           caMRID[27], caMRID[28], caMRID[29] );

   for (iLoop=0; iLoop<30; iLoop++)
   {
	if (strlen(caMRID[iLoop]) == 0)
	{
		if (iLoop == 0)
		{
			return 0;
		}
		else
		{
			return iLoop;	
		}
	}
   }
   if (iLoop == 30)
   {
	return iLoop;
   }

   return 0;
}

void    tiIvrSignIn()
{
        char destBuf[1024],Rsp[1024];
        int  idestLen,i;

        memset( Rsp, 0, sizeof(Rsp));
        idestLen = 0;
        memcpy( destBuf+idestLen, "30", 2);
        idestLen = idestLen + 2;
        memcpy( destBuf+idestLen, INS_IVR, 8);
        idestLen = idestLen + 8;
        memcpy( destBuf+idestLen, "SIGN12345678901234567890123456789012345", 39);
        idestLen = idestLen + 39;
        memcpy( destBuf+idestLen, "000000FFFFFFFF", 14);
        idestLen = idestLen + 14;
        memset(g_folder_msg.data,0 ,sizeof(g_folder_msg.data));
        memcpy( g_folder_msg.data, destBuf, idestLen);
        memset(g_folder_msg.OutFoldName,0 ,sizeof(g_folder_msg.OutFoldName));
	for (i=0; i < g_iSum; i++)
	{
		if (g_iPos < g_iSum)
		{
       			strcpy(g_folder_msg.OutFoldName, caMRID[g_iPos]);
		}
		else
		{
			g_iPos = 0;
       			strcpy(g_folder_msg.OutFoldName, caMRID[g_iPos]);
		}
       		rtrim(g_folder_msg.OutFoldName);
               	if ( IsReady( g_folder_msg.OutFoldName )<0 )
               	{
			g_iPos ++;
               	}
		else
		{
			break;
		}
	}
        g_folder_msg.datalen= idestLen;
        g_folder_msg.OutFoldType = 1;
        BC_SendMessageToFolder(Rsp);
}    
