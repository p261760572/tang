#include "ibdcs.h"
#include "tmcibtms.h"
#include "dbstruct.h"
#include "../db/inc/glbdb.h"
#include "BusT22.h"
#include "BusT00.h"
#include "BusZ00.h"
#include "BusZ22.h"
#include "CMDI.h"
#include "bc.h"
#include "iso8583.h"
#include <string.h>
#include <stdlib.h>

//#define MIN(a,b) a>b?b:a

extern int gs_myFid;
static void appSrvExit(int signo);
char g_center_ins_id_cd[8+1] = "09005500";

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
    //signal(17, SIG_IGN);

    glb_SetLog_FuncPtr((int)&BC_ErrLog_Func);
    glb_SetDebug_FuncPtr((int)&BC_Debug_Func);

    strcpy( g_center_ins_id_cd, "09005500");

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
    
    if ( IsoLoad8583config(&iso8583_ZDXTconf[0],"iso8583_ZDXT.conf") < 0 )
    {

        dcs_log(0,0,"<FILE:%s,LINE:%d>Load term system interface iso8583config() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        BC_SendExeception("04","装载iso8583_TERM.conf失败，appSrv进程启动失败，退出");
        appSrvExit(0);
    }
     
    if ( IsoLoad8583config(&iso8583_GDYLconf[0],"iso8583_GDYL.conf") < 0 )
    {
        BC_SendExeception("04","装载iso8583_GDYL.conf失败，appSrv进程启动失败，退出");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load mchn system interface iso8583config() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }

    if ( IsoLoad8583config(&iso8583_AJXTconf[0],"iso8583_AJXT.conf") < 0 )
    {
        BC_SendExeception("04","iso8583_AJXT.conf失败，appSrv进程启动失败，退出");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load POSB system interface iso8583_AJXTconf() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }
    
    if ( IsoLoad8583config(&iso8583_JAVAconf[0],"iso8583_JAVA.conf") < 0 )
    {
        BC_SendExeception("04","iso8583_JAVA.conf失败，appSrv进程启动失败，退出");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load JAVA system interface iso8583_JAVAconf() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }

    if ( IsoLoad8583config(&iso8583_CUPSconf[0],"iso8583_CUPS.conf") < 0 )
    {
        BC_SendExeception("04","iso8583_CUPS.conf失败，appSrv进程启动失败，退出");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load CUPS system interface iso8583_CUPSconf() failed:%s\n",__FILE__,__LINE__,strerror(errno));
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
    if( 0 != IVR_Init())
    {
    		appSrvExit(0);
    }
	aj_mchnt_cd = getenv("AJ_MCHNT_CD");
	if(aj_mchnt_cd == NULL) aj_mchnt_cd = "302430145119998";
	dcs_debug(0, 0, "aj_mchnt_cd=%s", aj_mchnt_cd);
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
    if ( u_fabricatefile("log/appsvr.log",logfile,sizeof(logfile)) < 0 )
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
        memset(g_Rcv_Buffer, 0, sizeof(g_Rcv_Buffer));
        //iRead = fold_read( gs_myFid, &fromFid, caBuffer, sizeof(caBuffer), 1);
        fromFid = -1000;
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
        memset(g_iSrcFoldName, 0, sizeof(g_iSrcFoldName));
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


int process_SignIn(ti_bc_trans_inf_def *p_tiBcTransInf, tl_bc_trans_log_def *p_BcTransLog,ISO_data *iso)
{
	int iRet = 0;
	int sqlCode = 0;
	char procedure[5 + 1]; //处理过程
	ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
	
	//init variant
	memset( &tiBcInsInf, 0, sizeof(tiBcInsInf) );
	strcpy( tiBcInsInf.line_nm,  p_BcTransLog->trans_chnl );
	strcpy( tiBcInsInf.packet_type,  p_BcTransLog->packet_type );
	tiBcInsInfOpr(GLB_DB_SELECT2, &tiBcInsInf, &sqlCode);
	if( 0 != sqlCode ){
		dcs_log(0, 0,"<FILE:%s,LINE:%d>依据线路名称查询机构信息失败(line_nm:%s, packet_type:%s).\n ",__FILE__,__LINE__, tiBcInsInf.line_nm, tiBcInsInf.packet_type );
		return -1;
	}
	
	// 搜索原始交易处理函数
	iRet = BC_Search_Trans_det(&g_tiBcTransDet,
	   p_BcTransLog->trans_chnl   ,    
	   p_BcTransLog->msg_tp       ,        
	   p_BcTransLog->proc_cd      ,       
	   p_BcTransLog->pos_cond_cd  ,
	   p_BcTransLog->mchnt_tp     , 
	   "*" ,
	   p_BcTransLog->trans_flag   ,
	   tiBcInsInf.ins_id_cd
	  );
	  
  if( 0 != iRet )
	{
	    BC_SendExeception("02","<FILE:%s,LINE:%d>未找到该交易的后续处理方式，请检查配置表ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);
	
	    //没有找到该交易的后续处理方式，数据库配置错误
	    //没有找到该交易的转换信息
	    dcs_log(0,0,"<FILE:%s,LINE:%d>未在表ti_bc_trans_det中找到该交易的处理方式：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    //返回
	    return -1;
	}
	memset(procedure, 0, sizeof(procedure) );
  	strcpy(procedure, g_tiBcTransDet.procedure); 
  	rtrim(procedure);
  
  if( 0 == strcmp("SIGN_GZ", procedure)){
  	return BusZ00_sign_gz(p_tiBcTransInf, &tiBcInsInf, p_BcTransLog, iso);
  }else if( 0 == strcmp("SIGN_JV", procedure)){
  	return BusZ00_sign_jv(p_tiBcTransInf, &tiBcInsInf, p_BcTransLog, iso);
  }else if( 0 == strcmp("SIGN_HFQZ", procedure)){
  	return BusZ00_sign_ylqz(p_tiBcTransInf, &tiBcInsInf, p_BcTransLog, iso);
  }
  
  dcs_log(0,0,"<FILE:%s,LINE:%d>配置的处理函数(%s)不存在.\n",__FILE__,__LINE__,procedure);
	return -1;
}

//IVR 发送了密码信息
int IVR_entry_on_password(char *ivrdata, int ifoldid, int ivrdatasize)
{
	//variant
	int iRet = -1;
	int sqlCode = 0;
	char procedure[5 + 1]; //处理过程
	IVR_STRUCT *pIVR = (IVR_STRUCT *)ivrdata;
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
	
	//init
	memset ( &g_TransMsg_o, 0, sizeof(g_TransMsg_o) );
	
	//password result
	BC_PacketIVRTimeOut( &g_TimeOutRec,  pIVR );
	
  //get key           
    TimeOut_Key *tk = (TimeOut_Key *)pIVR->key;
    strcpy_safe(p_BcTransLog_o->sys_tra_no, tk->sys_tra_no, sizeof(tk->sys_tra_no) );
    strcpy_safe(p_BcTransLog_o->term_id, tk->term_id, sizeof(tk->term_id) );
    strcpy_safe(p_BcTransLog_o->mchnt_cd, tk->mchnt_cd, sizeof(tk->mchnt_cd) );
    strcpy_safe(p_BcTransLog_o->loc_trans_dt, &tk->end, 4 );
          
    dcs_log(0,0,"<FILE:%s,LINE:%d>start to call tlBcTransLogOpr 搜索原始交易信息：key:%s.\n",__FILE__,__LINE__,  pIVR->key );
    tlBcTransLogOpr(GLB_DB_SELECT11, p_BcTransLog_o, &sqlCode);
	  if ( 0 != sqlCode )
	  {
	      BC_SendExeception("02","查询交易日志信息失败，未找到应答交易的请求交易信息. key:%s\n",  pIVR->key );
	      BC_SendMonitor_trans("0","3", p_BcTransLog_o, 0); //&g_tiBcTransInf);
	      //未查询到数据库记录，丢弃无效的应答。
	      dcs_log(&g_TransMsg, sizeof(BcTransMsg),"<FILE:%s,LINE:%d>收到ivr，在数据库中未找到该交易：key:%s.\n ",__FILE__,__LINE__,  pIVR->key );
	      return -1;
	  }
    *g_TransLock = '1'; 
    memset( g_rtIns_id_cd, 0, sizeof(g_rtIns_id_cd) );
    strcpy(g_rtIns_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    g_iSrcFoldId = fold_locate_folder( p_BcTransLog_o->trans_chnl );
        
    // 查询交易类型信息
		memset(&g_tiBcTransInf, 0, sizeof(ti_bc_trans_inf_def) );
		strcpy(g_tiBcTransInf.trans_chnl, p_BcTransLog_o->trans_chnl);
		strcpy(g_tiBcTransInf.msg_tp,p_BcTransLog_o->msg_tp);
		strcpy(g_tiBcTransInf.proc_cd,p_BcTransLog_o->proc_cd);
		strcpy(g_tiBcTransInf.pos_cond_cd,p_BcTransLog_o->pos_cond_cd);
		strcpy(g_tiBcTransInf.mchnt_tp,p_BcTransLog_o->mchnt_tp);
		
		iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
		if ( -3114 == sqlCode || -3135 == sqlCode )
		{
		    BC_SendExeception("03","数据库连接异常，自动重连数据库。");
		    dcs_log(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);
		
		    if ( DasConectDB()==0 )
		    {
		        iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
		    }
		}
		
	 if ( 0 != sqlCode )
   {
   	 BC_SendExeception("02","查询交易类型信息失败.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",p_BcTransLog_o->trans_chnl,p_BcTransLog_o->msg_tp,p_BcTransLog_o->proc_cd,p_BcTransLog_o->pos_cond_cd,p_BcTransLog_o->mchnt_tp);
   	 BC_SendMonitor_trans("0","3", p_BcTransLog_o, 0);
   	 dcs_log(0,0,"查询交易类型信息失败.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",__FILE__,__LINE__,p_BcTransLog_o->trans_chnl,p_BcTransLog_o->msg_tp,p_BcTransLog_o->proc_cd,p_BcTransLog_o->pos_cond_cd,p_BcTransLog_o->mchnt_tp);
   	 BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
   	 return -1;
   }
	
	  // 搜索原始交易处理函数
	   iRet = BC_Search_Trans_det(&g_tiBcTransDet,
	   p_BcTransLog_o->trans_chnl   ,    
	   p_BcTransLog_o->msg_tp       ,        
	   p_BcTransLog_o->proc_cd      ,       
	   p_BcTransLog_o->pos_cond_cd  ,
	   p_BcTransLog_o->mchnt_tp     , 
	   p_BcTransLog_o->msg_tp ,
	   p_BcTransLog_o->trans_flag   ,
	   g_rtIns_id_cd
	  );
	      
	  if( 0 != iRet )
	  {
	    BC_SendExeception("02","<FILE:%s,LINE:%d>未找到该交易的后续处理方式，请检查配置表ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_SendMonitor_trans("0","3", p_BcTransLog_o,&g_tiBcTransInf);
	
	    //没有找到该交易的后续处理方式，数据库配置错误
	    //没有找到该交易的转换信息
	    dcs_log(0,0,"<FILE:%s,LINE:%d>未在表ti_bc_trans_det中找到该交易的处理方式：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_UnLockTrans(p_BcTransLog_o);  
	    
	    //返回
	    return -1;
	  }
	   
	  memset(procedure, 0, sizeof(procedure) );
	  strcpy(procedure, g_tiBcTransDet.procedure); 
    rtrim(procedure);
   
	  //调用相应的函数
	  if( 0 == strcmp("Z0400", procedure) ){
	  	return BusIVR_Z22_ev1(&g_tiBcTransInf, &g_tiBcTransDet, ivrdata);
	  }else {
	  	BC_UnLockTrans(p_BcTransLog_o); 
	  	dcs_log(0,0,"<FILE:%s,LINE:%d>在表ti_bc_trans_det中配置的处理函数procedure（用于IVR PASSW0RD）不正确：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,  procedure, g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new); 
	  	return -1;
	  }
    
	return 0;	
}


//IVR 发送了查询关键域的信息
int IVR_entry_on_primaryField(char *ivrdata, int ifoldid, int ivrdatasize)
{
	//variant
	int iRet = -1;
	int sqlCode = 0;
	char procedure[5 + 1]; //处理过程
	IVR_STRUCT *pIVR = (IVR_STRUCT *)ivrdata;
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
	
	//init
	memset ( &g_TransMsg_o, 0, sizeof(g_TransMsg_o) );
	
	//password result
	BC_PacketIVRTimeOut( &g_TimeOutRec,  pIVR );
	
  //get key           
    TimeOut_Key *tk = (TimeOut_Key *)pIVR->key;
    strcpy_safe(p_BcTransLog_o->sys_tra_no, tk->sys_tra_no, sizeof(tk->sys_tra_no) );
    strcpy_safe(p_BcTransLog_o->term_id, tk->term_id, sizeof(tk->term_id) );
    strcpy_safe(p_BcTransLog_o->mchnt_cd, tk->mchnt_cd, sizeof(p_BcTransLog_o->mchnt_cd) );
    strcpy_safe(p_BcTransLog_o->loc_trans_dt, &tk->end, 4 );
          
    dcs_log(0,0,"<FILE:%s,LINE:%d>start to call tlBcTransLogOpr 搜索原始交易信息：key:%s.\n",__FILE__,__LINE__,  pIVR->key );
    tlBcTransLogOpr(GLB_DB_SELECT11, p_BcTransLog_o, &sqlCode);
	  if ( 0 != sqlCode )
	  {
	      BC_SendExeception("02","查询交易日志信息失败，未找到应答交易的请求交易信息. key:%s\n",  pIVR->key );
	      BC_SendMonitor_trans("0","3", p_BcTransLog_o, 0); //&g_tiBcTransInf);
	      //未查询到数据库记录，丢弃无效的应答。
	      dcs_log(&g_TransMsg, sizeof(BcTransMsg),"<FILE:%s,LINE:%d>收到ivr，在数据库中未找到该交易：key:%s.\n ",__FILE__,__LINE__,  pIVR->key );
	      return -1;
	  }
    *g_TransLock = '1'; 
    memset( g_rtIns_id_cd, 0, sizeof(g_rtIns_id_cd) );
    strcpy(g_rtIns_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    g_iSrcFoldId = fold_locate_folder( p_BcTransLog_o->trans_chnl );
        
    // 查询交易类型信息
	memset(&g_tiBcTransInf, 0, sizeof(ti_bc_trans_inf_def) );
	strcpy(g_tiBcTransInf.trans_chnl, p_BcTransLog_o->trans_chnl);
	strcpy(g_tiBcTransInf.msg_tp,p_BcTransLog_o->msg_tp);
	strcpy(g_tiBcTransInf.proc_cd,p_BcTransLog_o->proc_cd);
	strcpy(g_tiBcTransInf.pos_cond_cd,p_BcTransLog_o->pos_cond_cd);
	strcpy(g_tiBcTransInf.mchnt_tp,p_BcTransLog_o->mchnt_tp);
	iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
	if ( -3114 == sqlCode || -3135 == sqlCode)
	{
	    BC_SendExeception("03","数据库连接异常，自动重连数据库。");
	    dcs_log(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);
	    if ( DasConectDB()==0 )
	    {
	        iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
	    }
	}
	if ( 0 != sqlCode )
	{
		BC_SendExeception("02","查询交易类型信息失败.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",p_BcTransLog_o->trans_chnl,p_BcTransLog_o->msg_tp,p_BcTransLog_o->proc_cd,p_BcTransLog_o->pos_cond_cd,p_BcTransLog_o->mchnt_tp);
		BC_SendMonitor_trans("0","3", p_BcTransLog_o, 0);
		BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
		dcs_log(0,0,"<FILE:%s,LINE:%d>查询交易类型信息失败.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",__FILE__,__LINE__,p_BcTransLog_o->trans_chnl,p_BcTransLog_o->msg_tp,p_BcTransLog_o->proc_cd,p_BcTransLog_o->pos_cond_cd,p_BcTransLog_o->mchnt_tp);
		return -1;
	}
  	// 搜索原始交易处理函数
	iRet = BC_Search_Trans_det(&g_tiBcTransDet,
	p_BcTransLog_o->trans_chnl   ,    
	p_BcTransLog_o->msg_tp       ,        
	p_BcTransLog_o->proc_cd      ,       
	p_BcTransLog_o->pos_cond_cd  ,
	p_BcTransLog_o->mchnt_tp     , 
	p_BcTransLog_o->msg_tp ,
	p_BcTransLog_o->trans_flag   ,
	g_rtIns_id_cd
	);
	      
	  if( 0 != iRet )
	  {
	    BC_SendExeception("02","<FILE:%s,LINE:%d>未找到该交易的后续处理方式，请检查配置表ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_SendMonitor_trans("0","3", p_BcTransLog_o,&g_tiBcTransInf);
	
	    //没有找到该交易的后续处理方式，数据库配置错误
	    //没有找到该交易的转换信息
	    dcs_log(0,0,"<FILE:%s,LINE:%d>未在表ti_bc_trans_det中找到该交易的处理方式：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_UnLockTrans(p_BcTransLog_o);  
	    
	    //返回
	    return -1;
	  }
	   
	  memset(procedure, 0, sizeof(procedure) );
	  strcpy(procedure, g_tiBcTransDet.procedure); 
    rtrim(procedure);
   
	  //调用相应的函数
	  if( 0 == strcmp("Z0400", procedure) ){
	  	return BusIVR_Z22_ev1(&g_tiBcTransInf, &g_tiBcTransDet, ivrdata);
	  }else {
	  	BC_UnLockTrans(p_BcTransLog_o); 
	  	dcs_log(0,0,"<FILE:%s,LINE:%d>在表ti_bc_trans_det中配置的处理函数procedure（用于IVR 获取关键域）不正确：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,  procedure, g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new); 
	  	return -1;
	  }
    
	return 0;	
}

int IVR_entry(char *ivrdata, int ifoldid, int ivrdatasize)
{
	IVR_STRUCT *pIVR = (IVR_STRUCT *)ivrdata;	
	struct TimeOut_REC *p_TimeOutRec = &g_TimeOutRec;
	dcs_debug(ivrdata, ivrdatasize,"<FILE:%s,LINE:%d>收到IVR消息(%s).\n",__FILE__,__LINE__,ivrdata);
    TimeOut_Key *tk = (TimeOut_Key *)p_TimeOutRec->caKey;
    memset(p_TimeOutRec, 0, sizeof(struct TimeOut_REC));
    memcpy_safe((char*)tk, pIVR->key, sizeof(tk->sys_tra_no) + sizeof(tk->term_id) + sizeof(tk->mchnt_cd));
	BC_DeleteTimeOut1(&g_TimeOutRec);    
	if( 0 == memcmp("OP01", pIVR->opcode, 4) )//输密码申请应答
	{
		dcs_debug(0, 0,"<FILE:%s,LINE:%d>收到OP01应答。\n",__FILE__,__LINE__);
		return 0;
	}
	else if( 0 == memcmp("OP02", pIVR->opcode, 4) )//输CVV申请应答
	{
		dcs_debug(0, 0,"<FILE:%s,LINE:%d>收到OP02应答。\n",__FILE__,__LINE__);
		return 0;
	}
	else if( 0 == memcmp("OP03", pIVR->opcode, 4) )
	{
		return IVR_entry_on_password(ivrdata, ifoldid, ivrdatasize);
	}else if( 0 == memcmp("OP04", pIVR->opcode, 4) )
	{
		dcs_debug(0, 0,"<FILE:%s,LINE:%d>收到应答OP04应答。\n",__FILE__,__LINE__);
		return IVR_entry_on_primaryField(ivrdata, ifoldid, ivrdatasize);
	}else
	{
		dcs_log(ivrdata, ivrdatasize,"<FILE:%s,LINE:%d>IVR_entry: opcode 无法识别",__FILE__,__LINE__);
		return -1;
	}
}

int process_cmdi( char* srcBuf,int  iFid, int iLen )
{
    char caCommand[1024];
    char *cmd_type = srcBuf+4;
    char *cmd_data = srcBuf+8;
    if ( 0 == memcmp(cmd_type, "0001", 4) )
    {
        //超时命令
        process_cmdi_timeout(cmd_data, iFid, iLen-8);
    } 
    else if ( 0 == memcmp(cmd_type, "0002", 4) )
    {
        process_cmdi_timeout(cmd_data, iFid, iLen-8);
    } 
    else if ( 0 == memcmp(cmd_type, "0003", 4) )
    {
        //机构签到
        //商户代签到
	      mchntSignIn(cmd_data);
    }else if ( 0 == memcmp(cmd_type, "0005", 4) )
    {
    	//IVR process
    	IVR_entry(srcBuf, iFid, iLen );
    }
    return 0;
}

/*处理终端渠道上送交易报文格式为8583终端报文*/
int process( char* srcBuf,int  iFid, int iLen )
{
    char buff[100];
    char msgtype[5],process[7],processno[7],SerCode[3],mchnt_tp[5],packet_type[5];
    int iRc;
    ISO_data iso;

    memset(msgtype,0,sizeof(msgtype));
    memset(process,0,sizeof(process));
    memset(processno,0,sizeof(processno));
    memset(SerCode,0,sizeof(SerCode));
    memset(mchnt_tp,0,sizeof(mchnt_tp));
    /*解包*/
    if ( 0 > BC_UnPack( &iso, srcBuf, iLen) )
    {
        dcs_log(srcBuf,iLen,"<FILE:%s,LINE:%d>Can not unpacket Terminal Msg!datalen=%d",__FILE__,__LINE__,iLen);
        return;
    }
    dcs_log(0,0, "<FILE:%s,LINE:%d>g_iSrcFoldName[%s]",__FILE__,__LINE__,g_iSrcFoldName);

    memset(packet_type, 0, sizeof(packet_type));
    memcpy(packet_type, srcBuf, 4);

    PrintISO( &iso,"收到报文",0);

    if ( (iRc = getbit(&iso, 0, buff)) > 0 )
    {
        memcpy(msgtype, buff, MIN(iRc,  4));
    }
    if ( (iRc = getbit(&iso, 3, buff)) > 0 )
    {
        memcpy(process, buff, MIN(iRc,  6));
    }
    if ( (iRc = getbit(&iso, 22, buff)) > 0 )
    {
        memcpy(processno, buff, MIN(iRc,  3));
    }
    if ( (iRc = getbit(&iso, 25, buff)) > 0 )
    {
        memcpy(SerCode, buff, MIN(iRc,  2));
    }
    if ( (iRc = getbit(&iso, 18, buff)) > 0 )
    {
        memcpy(mchnt_tp, buff, MIN(iRc,  4));
    }
    dcs_log(0,0, "<FILE:%s,LINE:%d>g_iSrcFoldName[%s]",__FILE__,__LINE__,g_iSrcFoldName);

    /*根据交易类型，判断下一步流程*/
    /*从数据库中读取下一步流程，关键字为渠道好，消息类型，处理码，取到下一步流程标识*/
    if ( business_prepare(packet_type, msgtype,process,SerCode,mchnt_tp,&iso)<0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>business_prepare failed:msgtype[%s],process[%s]\n",__FILE__,__LINE__,msgtype,process);
        return -1;
    }

    return 0;
}

int business_prepare_return(char *trans_chnl, char *msg_tp, ISO_data *iso, ti_bc_trans_inf_def *p_tiBcTransInf, char *resp_cd)
{
    char buff[256];
	int sqlCode;
    dcs_log(0,0,"<FILE:%s,LINE:%d><business_prepare_return>opFlag<%c>交易没有成功，准备返回。\n",__FILE__,__LINE__,*p_tiBcTransInf->opFlag);
    
    // 返回 N00000
    if ( '0' == *p_tiBcTransInf->opFlag )
    {
        //无需应答的交易
    } else if ( '1' == *p_tiBcTransInf->opFlag )
    {
        //需要应答#39的交易
        BC_StringAdder(msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,resp_cd+4,2 );
        glb_GetLocalTimeString(buff,5,"%m%d");
        setbit_t(iso,15,buff, 4);
        setbit_t(iso,100, "00005500", 8 );
        strcpy(g_tiBcInsInf.ins_id_cd, g_TransMsg.BcTransLog.fwd_ins_id_cd);
		tiBcInsInfOpr(GLB_DB_SELECT1, &g_tiBcInsInf, &sqlCode);
		if ( 0 != sqlCode )
		{
			BC_SetMacKey(0,0);
		}
		else
		{
        	BC_SetRcvMacKey(&g_tiBcInsInf);
		}
        BC_SendMessageToSrc(iso);
        BC_SendMonitor_iso("1","0",trans_chnl,iso,p_tiBcTransInf);
    } else if ( '2' == *p_tiBcTransInf->opFlag )
    {
        //需要应答#28的交易
        BC_StringAdder(msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        memcpy(buff,resp_cd,6);
        setbit_t(iso,28,buff, 6 );
        strcpy(g_tiBcInsInf.ins_id_cd, g_TransMsg.BcTransLog.fwd_ins_id_cd);
		tiBcInsInfOpr(GLB_DB_SELECT1, &g_tiBcInsInf, &sqlCode);
		if ( 0 != sqlCode )
		{
			BC_SetMacKey(0,0);
		}
		else
		{
        	BC_SetRcvMacKey(&g_tiBcInsInf);
		}
        BC_SendMessageToSrc(iso);
        BC_SendMonitor_iso("1","0",trans_chnl,iso,p_tiBcTransInf);
    } else if ( '3' == *p_tiBcTransInf->opFlag )
    {
        //需要直接应答6位应答码的交易
        memcpy(buff,resp_cd,6);
        BC_SendMessageToFolder_direct( buff, 6);
    }
    return 0;
}

int SearchCardBinInfo(char *OutRespCode,  tl_bc_trans_log_def *p_BcTransLog)
{	
	int sqlCode = -1;
	memset( &g_tiBcCardBin, 0, sizeof(ti_bc_card_bin_def) );
	sprintf(g_tiBcCardBin.card_bin, "%.2d", strlen(p_BcTransLog->pri_acct_no));
	strcpy_safe(g_tiBcCardBin.card_bin+2, p_BcTransLog->pri_acct_no, sizeof(g_tiBcCardBin.card_bin)-1-2);
	*g_tiBcCardBin.pos_entry_md_cd2 = *(p_BcTransLog->pos_entry_md_cd+1);
	*g_tiBcCardBin.pos_entry_md_cd3 = *(p_BcTransLog->pos_entry_md_cd+2);
	tiBcCardBinOpr(GLB_DB_SELECT1, &g_tiBcCardBin, &sqlCode);
	if( 0!= sqlCode)
	{
		dcs_debug(0,0,"<FILE:%s,LINE:%d>在卡bin表中未找到卡bin(%s)信息：sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__,g_tiBcCardBin.card_bin, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
		strcpy(OutRespCode, RESP_CODE_CUS008);
		return -1;
	}
	return 0;
}

// 黑名单检查
int CardBinBlCheck(char *OutRespCode,  tl_bc_trans_log_def *p_BcTransLog)
{
	int sqlCode = -1;
	char tmpbuf[1024];
	memset( &g_tiBcBlackList, 0, sizeof(ti_bc_black_list_def) );
	
  strcpy_safe(g_tiBcBlackList.card_idno_inf, p_BcTransLog->pri_acct_no, sizeof(g_tiBcBlackList.card_idno_inf)-1);
  tiBcBlackListOpr(GLB_DB_SELECT1, &g_tiBcBlackList, &sqlCode);
  
  if( 0 == sqlCode)
  	{
			sprintf(tmpbuf, "卡号(%s)在黑名单中，交易被拒绝！",p_BcTransLog->pri_acct_no);
  	  strcpy(OutRespCode, RESP_CODE_CUS005);
			BC_SendExeception("03", "警告, %s", tmpbuf);
  		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
  		{
    		BC_SendExeception("03","警告, 记录异常表失败.");
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>,记录异常表失败.",__FILE__,__LINE__);
  		}
      // 检查不通过     
      dcs_log(0,0,"<FILE:%s,LINE:%d>卡bin(%s)在黑名单中，交易被拒绝：sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__,g_tiBcBlackList.card_idno_inf, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
    	 return -1;
    }else if(SQL_ERD_NONE_RECORD == sqlCode){
    	strcpy(OutRespCode, "N00000");
    	return 0;
    }else{
    	dcs_log(0,0,"<FILE:%s,LINE:%d>查询商户卡bin(%s)黑名单信息时发生异常[sqlCode:%d]：sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__,  g_tiBcBlackList.card_idno_inf, sqlCode, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
    	strcpy(OutRespCode, "N00096");
    	return -1;
    }
}

// 白名单检查
int CardBinWlCheck(char *OutRespCode,  tl_bc_trans_log_def *p_BcTransLog)
{
	int sqlCode = -1;
	memset( &g_tiBcWhiteList, 0, sizeof(ti_bc_white_list_def) );
	
  strcpy_safe(g_tiBcWhiteList.card_idno_inf, p_BcTransLog->pri_acct_no, sizeof(g_tiBcWhiteList.card_idno_inf)-1);
  strcpy_safe(g_tiBcWhiteList.mchnt_cd, p_BcTransLog->mchnt_cd, sizeof(g_tiBcWhiteList.mchnt_cd)-1);
  
  tiBcWhiteListOpr(GLB_DB_SELECT1, &g_tiBcWhiteList, &sqlCode);
  
  if( 0 == sqlCode){
      // 在白名单中
    	 strcpy(OutRespCode, "N00000");
    	 return 0;
    }else if(SQL_ERD_NONE_RECORD == sqlCode){
    	 strcpy(OutRespCode, "N00000");
    	 return 1;
    }else{
    	dcs_log(0,0,"<FILE:%s,LINE:%d>查询商户卡bin(%s)白名单信息时发生异常[sqlCode:%d]：sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__,  g_tiBcWhiteList.card_idno_inf, sqlCode, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
    	strcpy(OutRespCode, "N00096");
    	return -1;
    }
}

int MchntCardBinLimitCheck(char *OutRespCode,  tl_bc_trans_log_def *p_BcTransLog, ti_bc_mchnt_inf_def *p_tiBcMchntInf)
{
	int iRet,sqlCode;
	double max_at = 0;
	double trans_at = 0;
	double allNum =0;
	double allAt =0;
	char settle_dt[8];
	char tmpbuf[1024];
	
	trans_at = (double) atof ( p_BcTransLog->trans_at );
	
	memset(settle_dt, 0, sizeof(settle_dt));
	glb_GetLocalTimeString(settle_dt, sizeof(settle_dt), "%m%d");
	
	//单笔限制
	if(IsCreditCard(p_BcTransLog->card_attr)){
		max_at =   atof ( p_tiBcMchntInf->credit_limit_times );			
		dcs_debug(0,0,"<FILE:%s,LINE:%d>检查商户(%s)单笔金额(%f)限制(max:%f).\n",__FILE__,__LINE__,p_BcTransLog->mchnt_cd, trans_at, max_at );	
		if( 0 != max_at && trans_at > max_at )
		{
			sprintf(tmpbuf, "交易金额[%.2f元]超过[%s(%s)]商户信用卡单笔金额限制[%.2f元]，交易被拒绝！",trans_at/100, rtrim(p_tiBcMchntInf->mchnt_cn_abbr), p_BcTransLog->mchnt_cd, max_at/100);
    	strcpy(OutRespCode, RESP_CODE_CUS012 );
			BC_SendExeception("03", "警告, %s", tmpbuf);
  		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
  		{
    		BC_SendExeception("03","警告, 记录异常表失败.");
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>,记录异常表失败.",__FILE__,__LINE__);
  		}
			dcs_log(0,0,"<FILE:%s,LINE:%d>达到商户(%s)单笔金额限制(%f)上限，交易被拒绝：sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__, p_BcTransLog->mchnt_cd, max_at, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
    	return -1;
		}
	}else{
		max_at =  atof ( p_tiBcMchntInf->debit_limit_times );	
		dcs_debug(0,0,"<FILE:%s,LINE:%d>检查商户(%s)单笔金额(%f)限制(max:%f).\n",__FILE__,__LINE__,p_BcTransLog->mchnt_cd, trans_at, max_at );	
		if( 0 != max_at && trans_at > max_at )
		{
			sprintf(tmpbuf, "交易金额[%.2f元]超过[%s(%s)]商户借记卡单笔金额限制[%.2f元]，交易被拒绝！",trans_at/100, rtrim(p_tiBcMchntInf->mchnt_cn_abbr), p_BcTransLog->mchnt_cd, max_at/100);
    	strcpy(OutRespCode, RESP_CODE_CUS012 );
			BC_SendExeception("03", "警告, %s", tmpbuf);
  		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
  		{
    		BC_SendExeception("03","警告, 记录异常表失败.");
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>,记录异常表失败.",__FILE__,__LINE__);
  		}
			dcs_log(0,0,"<FILE:%s,LINE:%d>达到商户(%s)单笔金额限制(%f)上限，交易被拒绝：sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__, p_BcTransLog->mchnt_cd, max_at, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
    	return -1;
		}
	}
	
	//商户日限额检查
	max_at =   atof ( p_tiBcMchntInf->limit_day );	
	if( 0!= max_at ){
		//获取当前笔数,金额
		iRet = tlBcTransLogOpr2(&allAt, &allNum, settle_dt, 0 /*p_BcTransLog->pri_acct_no*/, p_BcTransLog->mchnt_cd, &sqlCode);
		if( 0!= iRet ){
			strcpy(OutRespCode, "N00096");
			return -1;
		}
		dcs_debug(0,0,"<FILE:%s,LINE:%d>商户日限额(%f)检查，当前金额%f, 当前已清算交易总笔数%f, 总金额%f.\n",__FILE__,__LINE__, max_at, trans_at, allNum, allAt );
		if( max_at < allAt + trans_at )
		{
			sprintf(tmpbuf, "交易金额[%.2f元],已交成功交易金额[%.2f元]，总金额超过[%s(%s)]商户总交易金额限制[%.2f元]，交易被拒绝！",trans_at/100, allAt/100, rtrim(p_tiBcMchntInf->mchnt_cn_abbr), p_BcTransLog->mchnt_cd, max_at/100);
			strcpy(OutRespCode, RESP_CODE_CUS002);
			BC_SendExeception("03", "警告, %s", tmpbuf);
  		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
  		{
    		BC_SendExeception("03","警告, 记录异常表失败.");
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>,记录异常表失败.",__FILE__,__LINE__);
  		}
			return -1;
		}
	}	
	strcpy(OutRespCode, "N00000" );
	return 0;
}

//全局持卡人最大笔数限制
int TimesLimitCheck( char *OutRespCode,  tl_bc_trans_log_def *p_BcTransLog )
{
	int iRet ;
	int sqlCode;
	double maxNum = 0;
	double allNum = 0;
	char settle_dt[8]; 
	char tmpbuf[1024];
	
	//获取最大笔数限制
	strcpy(g_tiBcMidDayLimitTims.fwd_ins_id_cd, g_TransMsg.BcTransLog.fwd_ins_id_cd);
	iRet = tiBcMidDayLimitTimsOpr(GLB_DB_SELECT1, &g_tiBcMidDayLimitTims, &sqlCode);
	if( SQL_ERD_NONE_RECORD ==  sqlCode )
	{
		dcs_debug(0,0,"<FILE:%s,LINE:%d><business_prepare>无需进行笔数金额检查限制。\n",__FILE__,__LINE__);		
		return 0;
	}else if(0 == sqlCode )
	{
		if(IsCreditCard(p_BcTransLog->card_attr)){
			maxNum = (double)g_tiBcMidDayLimitTims.credit_times_day;
		}else{
			maxNum = (double)g_tiBcMidDayLimitTims.debit_times_day;
		}
		if( 0 == maxNum){
			//无需进行持卡人最大笔数限制
			strcpy(OutRespCode, "N00000");
			return 0;
		}
	}else
	{
		//数据库异常
		strcpy(OutRespCode, "N00096");
		return -1;
	}
	
	memset(settle_dt, 0, sizeof(settle_dt));
	glb_GetLocalTimeString(settle_dt, sizeof(settle_dt), "%m%d");
	//获取当前笔数,金额
	iRet = tlBcTransLogOpr2(0, &allNum, settle_dt, p_BcTransLog->pri_acct_no, 0, &sqlCode);
	if( 0!= iRet ){
		strcpy(OutRespCode, "N00096");
		return -1;
	}
	
	dcs_debug(0,0,"<FILE:%s,LINE:%d><business_prepare>笔数限制检查(maxNum:%f, allNum:%f).。\n",__FILE__,__LINE__, maxNum, allNum);		
	
	if( allNum >= maxNum )
	{
		strcpy(OutRespCode, RESP_CODE_CUS014);
		sprintf(tmpbuf, "卡[%s],已交成功交易[%.0f笔数]，达到卡笔数交易限制[%.0f]，交易被拒绝！",p_BcTransLog->pri_acct_no, allNum, maxNum);
		BC_SendExeception("03", "警告, %s", tmpbuf);
		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
		{
	  		BC_SendExeception("03","警告, 记录异常表失败.");
	  		dcs_log(0, 0, "<FILE:%s,LINE:%d>,记录异常表失败.",__FILE__,__LINE__);
		}
		dcs_log(0,0,"<FILE:%s,LINE:%d><business_prepare>超出笔数限制(maxNum:%f, allNum:%f).。\n",__FILE__,__LINE__, maxNum, allNum);
		return -1;
	}
	strcpy(OutRespCode, "N00000");
	return 0;
}

int business_prepare(char *packet_type, char *msg_tp,char *proc_cd,char *pos_cond_cd, char *mchnt_tp, ISO_data *iso)
{
    //variant
    char buff[256];
    char processno[10];
    int iRet;          // 返回代码
    int sqlCode;       // sql 返回代码
    char trans_flag[4+1];
    char procedure[5 + 1]; //处理过程
    char resp[3];
    char flag[10]={0};
    char g_TransLock[2] ={'0','\0'};
    char trans_chnl[30+1];
    char inwritelist = '0';
    char RespCode[6+1];
    int inWhiteList;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;

    dcs_debug(0,0,"<FILE:%s,LINE:%d>enter.\n",__FILE__,__LINE__);

    //variant init
    strcpy(trans_flag,"*");
    BC_CopyISOdata( iso, &g_TransMsg);
    strcpy(trans_chnl, g_iSrcFoldName);
    strcpy(g_TransMsg.BcTransLog.trans_chnl, g_iSrcFoldName);
    strcpy(g_TransMsg.BcTransLog.packet_type, packet_type);
    memset(g_rtIns_id_cd, 0, sizeof(g_rtIns_id_cd) );
    memcpy(&g_TransMsg_o,&g_TransMsg, sizeof(BcTransMsg)); //copy to old

    // 查询交易类型信息
    memset(&g_tiBcTransInf, 0, sizeof(ti_bc_trans_inf_def) );
    strcpy(g_tiBcTransInf.trans_chnl,trans_chnl);
    strcpy(g_tiBcTransInf.msg_tp,msg_tp);
    strcpy(g_tiBcTransInf.proc_cd,proc_cd);
    strcpy(g_tiBcTransInf.pos_cond_cd,pos_cond_cd);
    strcpy(g_tiBcTransInf.mchnt_tp,mchnt_tp);

    iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
    if ( -3114 == sqlCode || -3135 == sqlCode )
    {
        BC_SendExeception("03","数据库连接异常，自动重连数据库。");
        dcs_debug(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);

        if ( DasConectDB()==0 )
        {
            iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
        }
    }
    if ( 0 != sqlCode )
    {
        BC_SendExeception("02","查询交易类型信息失败.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
        BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);

        //入库，失败的处理。。。
        strcpy(g_TransMsg.BcTransLog.is_over, "1");
        if ( sqlCode == SQL_ERD_NONE_RECORD )
        {
            strcpy(g_TransMsg.BcTransLog.resp_cd,"40");
            //没有配置该交易信息
            dcs_log(0,0,"<FILE:%s,LINE:%d>没有在ti_bc_trans_inf表中配置该交易信息，trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s\n",__FILE__,__LINE__,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
            BC_SendExeception("03","交易入库失败.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
        } else
        {
            strcpy(g_TransMsg.BcTransLog.resp_cd,"96");
            BC_SendExeception("04","警告，数据库可能存在异常，请尽快联系技术人员处理.");
        }
        iRet = BC_InsertIntoDB(&g_TransMsg.BcTransLog, &sqlCode);
        if ( 0!=sqlCode )
        {
            BC_SendExeception("03","交易入库失败.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
        }

        //返回
        BC_exp_log(EXP_CODE_CUS001, "未在ti_bc_trans_inf表中配置该交易信息", g_Rcv_Buffer,g_RcvBufferSize);
        return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,"N00040");
    }

    if( 0 == strcmp("Z00", rtrim(g_tiBcTransInf.TransType) ))
    {
    	//签到应答
    	iRet = process_SignIn(&g_tiBcTransInf, &g_TransMsg.BcTransLog, iso);
    	if( 0 == iRet ) return 0;
    }
    
    //是否需要删除超时表
    //tiBcTransInf.respFlag: 0:非应答标记 1：应答，需要删除超时表
    memset(&g_TimeOutRec, 0, sizeof(struct TimeOut_REC));
    if ( '0' == *g_tiBcTransInf.respFlag )
    {
        //非应答标记
    } else if ( '1' == *g_tiBcTransInf.respFlag )
    {
    	  TimeOut_ReMark *trm = (TimeOut_ReMark *)g_TimeOutRec.caRemark;
        
        BC_PacketTimeOut(&g_TimeOutRec, iso, 1);
        //删除超时表
        if ( 0 == BC_DeleteTimeOut1(&g_TimeOutRec) )
        {
            *flag = '1' ;
            memset(p_BcTransLog_o->sys_tra_no, 0, sizeof(p_BcTransLog_o->sys_tra_no));
            memcpy(p_BcTransLog_o->sys_tra_no, trm->sys_tra_no, 6);
            memset(p_BcTransLog_o->term_id, 0, sizeof(p_BcTransLog_o->term_id));
            memcpy(p_BcTransLog_o->term_id, trm->term_id, 8);
            memset(p_BcTransLog_o->mchnt_cd, 0, sizeof(p_BcTransLog_o->mchnt_cd));
            memcpy(p_BcTransLog_o->mchnt_cd, trm->mchnt_cd, 20);
            dcs_debug(0,0,"<FILE:%s,LINE:%d>删除超时表成功：sys_tra_no:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,trm->sys_tra_no,trm->term_id,trm->mchnt_cd );
        } else
        {
            //删除失败，可能为迟到的应答。一般可不做处理，上账交易作确认处理            
            dcs_log(0,0,"<FILE:%s,LINE:%d>删除超时表失败：sys_tra_no:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,trm->sys_tra_no,trm->term_id,trm->mchnt_cd );
            return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,RESP_CODE_CUS007 );            
        }
    } else if ( '2' == *g_tiBcTransInf.respFlag || '3' == *g_tiBcTransInf.respFlag)
    {
        BC_PacketTimeOut(&g_TimeOutRec, iso, '2' == *g_tiBcTransInf.respFlag ? 1 : 0);
        //删除账单超时表
        if ( 0 == BC_DeleteBillNotify(&g_TimeOutRec) )
        {
        	  TimeOut_ReMark *trm = (TimeOut_ReMark *)g_TimeOutRec.caRemark;
            
            *flag = '1' ;            
            memset(p_BcTransLog_o->sys_tra_no, 0, sizeof(p_BcTransLog_o->sys_tra_no));
            memcpy(p_BcTransLog_o->sys_tra_no, trm->sys_tra_no, 6);
            memset(p_BcTransLog_o->term_id, 0, sizeof(p_BcTransLog_o->term_id));
            memcpy(p_BcTransLog_o->term_id, trm->term_id, 8);
            memset(p_BcTransLog_o->mchnt_cd, 0, sizeof(p_BcTransLog_o->mchnt_cd));
            memcpy(p_BcTransLog_o->mchnt_cd, trm->mchnt_cd, 20);
            dcs_debug(0,0,"<FILE:%s,LINE:%d>删除超时表成功：sys_tra_no:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,trm->sys_tra_no,trm->term_id,trm->mchnt_cd );
            
        } else
        {
            //删除失败，可能为迟到的应答。一般可不做处理，上账交易作确认处理
            dcs_log(0,0,"<FILE:%s,LINE:%d>删除账单超时表失败：sys_tra_no:%s,term_id:%s,mchnt_cd:%s.\n",__FILE__,__LINE__,g_TransMsg_o.BcTransLog.sys_tra_no,g_TransMsg_o.BcTransLog.term_id,g_TransMsg_o.BcTransLog.mchnt_cd);
            return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,RESP_CODE_CUS006);            
        }
    }

    //初始交易状态标记设置为*
    if( *flag != '1') strcpy(g_tiBcTransDet.trans_flag,"*");

    //是否需要查询数据库，及查询方式(key value)
    //一般需要删超时表，就需要查询数据库
    //tiBcTransInf.DbFlag: 0:无需操作数据库，1,2,3,...：数据库查询方式
    //dcs_log(&g_tiBcTransInf, sizeof(g_tiBcTransInf), "<FILE:%s,LINE:%d>show transinf",__FILE__,__LINE__);
    if ( '0' != *g_tiBcTransInf.DbFlag ) //查询数据库 1: 按提交信息查询，2：按交易信息查询
    {
        //查询数据库 GLB_DB_SELECT1
        //对于查询回复交易，仅需要通过查询数据库得到trans_flag交易状态代码。
        TimeOut_ReMark *trm = (TimeOut_ReMark *)g_TimeOutRec.caRemark;
        if ( *flag == '1' && *trm->loc_trans_dt !=0 )
        {
        	   memset(p_BcTransLog_o->loc_trans_dt, 0, sizeof(p_BcTransLog_o->loc_trans_dt));
             memcpy(p_BcTransLog_o->loc_trans_dt, trm->loc_trans_dt, 4);
             dcs_log(0,0,"<FILE:%s,LINE:%d>从超时表中获取交易日期：loc_trans_dt:%s.\n",__FILE__,__LINE__,trm->loc_trans_dt);
        }
        dcs_debug(0,0,"<FILE:%s,LINE:%d>start to call tlBcTransLogOpr 搜索原始交易信息：mchnt_cd:[%s],term_id:[%s],sys_tra_no:[%s],loc_trans_dt:[%s]",__FILE__,__LINE__,p_BcTransLog_o->mchnt_cd,p_BcTransLog_o->term_id,p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->loc_trans_dt);
        iRet = tlBcTransLogOpr('1' == g_tiBcTransInf.DbFlag[0] ?  GLB_DB_SELECT11 : GLB_DB_SELECT13, &g_TransMsg_o.BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("02","查询交易日志信息失败，未找到应答交易的请求交易信息.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
            BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);
            //未查询到数据库记录，丢弃无效的应答。
            //没有找到该终端信息
            dcs_log(&g_TransMsg,sizeof(BcTransMsg),"<FILE:%s,LINE:%d>在数据库中未找到该交易：trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s\n",__FILE__,__LINE__,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
            //返回
            return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,"N00022");
        }
        *g_TransLock = '1';

        //通过查询历史交易，需要得到的交易要素
        strcpy(g_tiBcTransDet.trans_flag,g_TransMsg_o.BcTransLog.trans_flag);
        strcpy(g_rtIns_id_cd, g_TransMsg_o.BcTransLog.rcv_ins_id_cd );
        dcs_log(0,0,"<FILE:%s,LINE:%d>在数据库中查找原始交易成功：trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s\n",__FILE__,__LINE__,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
    }    
   	else
   	{
   		if(0 != *p_BcTransLog_o->mchnt_cd) //有商户号，查商户信息
   		{
		    strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog_o->mchnt_cd);
		    tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
		    if ( 0 != sqlCode ) //没有找到商户信息
		    {
				dcs_log(0,0, "商户号不存在!(mchnt_cd:%s,sqlcode=%d)", p_tiBcMchntInf->mchnt_cd, sqlCode);
		    	return business_prepare_return(trans_chnl, msg_tp, iso, &g_tiBcTransInf, "N00003");
		    }
		}
   	}
    // 查询卡bin 信息
    if( '1' == *g_tiBcTransInf.CardBinDetFlag )
    {
    	if( 0 != SearchCardBinInfo(RespCode,  p_BcTransLog))//查卡BIN路由
    	{
    		BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
    		return business_prepare_return(trans_chnl, msg_tp, iso, &g_tiBcTransInf, RespCode);
    	} 
    	strcpy(p_BcTransLog->card_attr, g_tiBcCardBin.card_attr );
    	strcpy(p_BcTransLog->iss_ins_id_cd, g_tiBcCardBin.iss_ins_id_cd );
    	strcpy(p_BcTransLog_o->card_attr, g_tiBcCardBin.card_attr );
	    if( 0!= BC_Find_Route_Ins(RespCode, g_rtIns_id_cd,0 != *p_BcTransLog_o->mchnt_cd ? &g_tiBcMchntInf : 0, &g_tiBcCardBin, p_BcTransLog->pri_acct_no) )
	    {
	    	BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
	    	return business_prepare_return(trans_chnl, msg_tp, iso, &g_tiBcTransInf, RespCode);
	    }
	    //保存路由接受机构代码到数据库字段
       strcpy(g_TransMsg_o.BcTransLog.rcv_ins_id_cd ,g_rtIns_id_cd);
       strcpy(p_BcTransLog->rcv_ins_id_cd, g_rtIns_id_cd );
       dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ2210>商户卡bin路由确定为[%s].\n ",__FILE__,__LINE__, g_rtIns_id_cd );
    }
    //需要进行商户的相关黑白名单及限额检查
    if( '1' == *g_tiBcTransInf.Mid_chk_flg) 
    {
		dcs_log(0, 0,"<FILE:%s,LINE:%d>进行商户黑名单限额检查.(mchnt_cd:%s) ",__FILE__,__LINE__,g_tiBcMchntInf.mchnt_cd );
   	  	//黑名单检查
   	  	if(0 > CardBinBlCheck(RespCode, p_BcTransLog) )
   	  	{
   	  		BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
   	  		return business_prepare_return(trans_chnl, msg_tp, iso, &g_tiBcTransInf, RespCode);
   	  	}   	
   	  	inWhiteList = 0 ; 		
    	if( 0 != *p_BcTransLog->mchnt_cd )
    	{
			if('1' == *g_tiBcMchntInf.using_whitelist) 
	   		{
	   	  		iRet = CardBinWlCheck(RespCode, p_BcTransLog);
	   	  		if(0 > iRet )
		  		{
		  			BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
		  			return business_prepare_return(trans_chnl, msg_tp, iso, &g_tiBcTransInf, RespCode);
		  		} 
	   	  		else if( 0 == iRet )//在白名单中
	   	  		{
	   	  			dcs_debug(0, 0,"<FILE:%s,LINE:%d>卡号(%s)在商户白名单中(mchnt_cd:%s) ",__FILE__,__LINE__, p_BcTransLog->pri_acct_no, g_tiBcMchntInf.mchnt_cd );	
	   	  			inWhiteList = 1;
	   	  		}
	   	  	}
	   	  	if(!inWhiteList)
	   	  	{
	  			if( 0 != MchntCardBinLimitCheck(RespCode,  p_BcTransLog, &g_tiBcMchntInf) )
	  			{
	  				BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
	  				return business_prepare_return(trans_chnl, msg_tp, iso, &g_tiBcTransInf, RespCode);
	  			}
	  		}
	  	}
	  	if(!inWhiteList)
	  	{
	    	if( 0 != TimesLimitCheck(RespCode,  p_BcTransLog) )
	    	{
	    		BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
	    	 	return business_prepare_return(trans_chnl, msg_tp, iso, &g_tiBcTransInf, RespCode); 
			}
		}
	}
    //----------------------------
    //使用原始交易及状态查询  
    //   --  冲正应答交易无法通过原始交易代码区分，需要判断应答交易的标志
    //   --  超时交易的处理入口不在此处，由超时处理程序入口处理


//    strcpy(g_tiBcTransDet.trans_chnl,p_BcTransLog_o->trans_chnl);
    strcpy(g_tiBcTransDet.trans_chnl,trans_chnl);
    strcpy(g_tiBcTransDet.msg_tp,p_BcTransLog_o->msg_tp);
    strcpy(g_tiBcTransDet.proc_cd,p_BcTransLog_o->proc_cd);
    strcpy(g_tiBcTransDet.pos_cond_cd,p_BcTransLog_o->pos_cond_cd);
    strcpy(g_tiBcTransDet.mchnt_tp,p_BcTransLog_o->mchnt_tp);
    strcpy(g_tiBcTransDet.msg_tp_new,p_BcTransLog->msg_tp); //new msgtp
    strcpy(g_tiBcTransDet.trans_flag,p_BcTransLog_o->trans_flag);
    strcpy(g_tiBcTransDet.rcv_ins_id_cd, g_rtIns_id_cd);
    
    //根据消息类型，处理码，服务点条件码，及交易标识码确定交易处理过程    
    dcs_debug(0,0,"<FILE:%s,LINE:%d>start to call tiBcTransDetOpr()确定交易处理过程：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s],rcv_ins_id_cd[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new,g_tiBcTransDet.rcv_ins_id_cd);
    iRet = tiBcTransDetOpr(GLB_DB_SELECT1, &g_tiBcTransDet, &sqlCode);
    if ( 0 != sqlCode )
    {
        BC_SendExeception("02","<FILE:%s,LINE:%d>未找到交易的后续处理方式，请检查配置表ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
        BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);

        //没有找到该交易的后续处理方式，数据库配置错误
        //没有找到该交易的转换信息
        dcs_log(0,0,"<FILE:%s,LINE:%d>未在表ti_bc_trans_det中找到该交易的处理方式：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);

        //入库，失败的处理。。。
        if ( '0' == *g_TransLock )
        {
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->resp_cd,"40");
            iRet = BC_InsertIntoDB(p_BcTransLog, &sqlCode);            
        }
        BC_UnLockTrans(p_BcTransLog_o);  

        //返回
        return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,"N00040");
    }

    //找到相应的处理函数处理
    strcpy(procedure, g_tiBcTransDet.procedure); 
    rtrim(procedure);
   
    dcs_debug(0,0,"<FILE:%s,LINE:%d>确定交易的处理过程为%s" ,__FILE__,__LINE__,procedure);

    //此处为所有的交易的处理入口    
    if ( 0 );

    //终端签到交易
    ON_BC_FUNC("T0010",Bus_SignInReq_term)

    //T22缴费交易的所有处理过程
    /*
    ON_BC_FUNC("T2210",BusT2210) //缴费请求
    ON_BC_FUNC("T2220",BusT2220) 
    ON_BC_FUNC("T2230",BusT2230)
    ON_BC_FUNC("T2240",BusT2240)
    ON_BC_FUNC("T2221",BusT2221)
    ON_BC_FUNC("T2231",BusT2231)
    ON_BC_FUNC("T2241",BusT2241)
		*/
		
    //账单系统交易---------------
    ON_BC_FUNC("Z0000",BusZ0000) //签到
    ON_BC_FUNC("Z0100",BusZ0100) //订单查询
    ON_BC_FUNC("Z0200",BusZ0200) //交易结果查询
    ON_BC_FUNC("Z0400",BusZ00_ev1) //交易申请
    ON_BC_FUNC("Z2210",BusZ2210) //0850非绑定用户支付
    ON_BC_FUNC("Z2211",BusZ2211) //0830非绑定用户支付应答
    ON_BC_FUNC("Z2212",BusZ2212) //0880非绑定用户支付应答通知
    ON_BC_FUNC("Z2220",BusZ2220) //绑定用户支付
    ON_BC_FUNC("Z2221",BusZ2221) //绑定用户支付应答
    
    //广银联账单消费交易
//    ON_BC_FUNC("Z22G1",  BusGZ_Z22_ev1)     //0200广银联非绑定用户支付
    ON_BC_FUNC("Z22G4",  BusGZ_Z22_ev4)     //0210广银联非绑定用户支付应答
    ON_BC_FUNC("Z22G4_R",BusGZ_Z22_ev4_R) 	//广银联非绑定用户支付冲正应答
    ON_BC_FUNC("Z22G5",  BusZ2212)          //广银联非绑定用户支付--受理方应答
    
    //汇付天下账单消费交易
//    ON_BC_FUNC("Z22H1",   BusHF_Z22)         //汇付非绑定用户支付
    ON_BC_FUNC("Z22H2",   BusHF_Z22_RESP1)   //汇付非绑定用户支付通知
    ON_BC_FUNC("Z22H3",   BusHF_Z22_RESP2)   //汇付非绑定用户支付应答
    ON_BC_FUNC("Z22H5",   BusZ2212)          //汇付非绑定用户支付--受理方应答
    
    //POSP前置交易
    ON_BC_FUNC("Z22P1", POSP_ExpendApp )         					//POSP前置消费申请
    ON_BC_FUNC("Z22P2", POSP_ExpendUndo )        					//POSP前置消费撤消
    ON_BC_FUNC("Z22P3", POSP_BalanceQuery )      					//POSP前置余额查询
    ON_BC_FUNC("Z22P4", POSP_ExpendRollBack )    					//POSP前置消费冲正
    ON_BC_FUNC("Z22P5", POSP_ExpendUndoRollBack )					//POSP前置消费撤消冲正
    ON_BC_FUNC("SIGN_POS1", POSP_KeysResetApp )						//POSP前置密钥重置申请
    ON_BC_FUNC("SIGN_POS2", POSP_KeysResetReply )					//POSP前置密钥重置应答
    
    //CUPS银行交易
    ON_BC_FUNC("Z22P1_R", CUPS_ExpendAppReply  )       			//CUPS消费申请应答
    ON_BC_FUNC("Z22P2_R", CUPS_ExpendUndoReply  )       		//CUPS消费撤消应答
    ON_BC_FUNC("Z22P3_R", CUPS_BalanceQueryReply  )     		//CUPS余额查询应答
    ON_BC_FUNC("Z22P1_R_R", CUPS_ExpendRollBackReply  )   		//CUPS消费冲正应答
    ON_BC_FUNC("Z22P2_R_R", CUPS_ExpendUndoRoolBackReply  )		//CUPS消费撤消冲正应答
    //银联前置
    ON_BC_FUNC("Z22YL2", BusHF_Z22_ev2  )       				//银联消费应答(同步)
    ON_BC_FUNC("Z22YL3", BusHF_Z22_ev3  )       				//银联消费应答(异步)   
    //SMS处理
    ON_BC_FUNC("Z22S1_R", SMS_SendReply )						//发送短消息应答
    //未找到该交易的处理方式
    ELSE
    {
    	//解锁
    	  BC_UnLockTrans(p_BcTransLog_o);
    	
    	//计入异常库
      BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);
      BC_exp_log(EXP_CODE_CUS001, "找到的处理函数不存在，可能配置错误。", g_Rcv_Buffer,g_RcvBufferSize);  
        
        //没有匹配到
        dcs_log(0,0,"<FILE:%s,LINE:%d>没有配置该交易的处理方式，trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s,procedure:%s\n",__FILE__,__LINE__,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp,procedure);
        BC_SendExeception("02","交易的后续处理方式不存在[%s]，请检查配置表ti_bc_trans_det.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",procedure,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);

        
        //返回
        return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,"N00040");
    }

    return 0;
}

int appbusiness(char *msgtype,char *process,char *SerCode, char *mchnt_tp, ISO_data *iso)
{
    return 0;
}

//发送给安捷
int SendMessageToRcvMcht(ISO_data *iso, char *pRecvData, int rcvSize, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    int iRet;
    int iConnSock ;
    char buff[2048];
    int buffLen;

    buffLen = isotostr(buff, iso);
    if ( buffLen < 0 )
    {
        printf("ISO打包失败.\n");
        return -1;
    }

    //建立连接
    iConnSock = tcp_connet_server(p_tiBcInsInf->ip_addr, atoi(p_tiBcInsInf->port),0);
    if ( iConnSock < 0 )
    {
        printf("连接失败[ip:%s,port:%s].\n", p_tiBcInsInf->ip_addr, p_tiBcInsInf->port);
        return -1;
    }

    //发送信息
    iRet = write_msg_to_net(iConnSock, buff, buffLen, 30*1000);
    if ( iRet < 0 )
    {
        printf("发送签到信息失败.\n");
        tcp_close_socket(iConnSock);
        return iRet;
    }

    dcs_debug(buff, buffLen, "发送日志\n");

    //接收信息
    iRet = read_msg_from_net(iConnSock, pRecvData,rcvSize,5*1000);
    if ( iRet < 0 )
    {
        printf("接收签到应答信息失败[%d].\n", iRet);
        dcs_debug(0,0,"接收签到应答信息失败[%d].\n", iRet);
        tcp_close_socket(iConnSock);
        return iRet;         
    }

    tcp_close_socket(iConnSock);
    return iRet;
}

int sendSignIN(char *mchnt_cd, char *term_id, char *rcv_ins_id_cd)
{
}

