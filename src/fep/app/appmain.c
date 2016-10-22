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
        BC_SendExeception("04","<FILE:%s,LINE:%d>����־ʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        exit(1);
    }

    dcs_log(0,0, "<FILE:%s,LINE:%d>appSrv Servers is starting up...\n",__FILE__,__LINE__);
    u_daemonize(NULL);

    //attach to SHM of IBDCS
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",strerror(errno),__FILE__,__LINE__);
        BC_SendExeception("04","����SHMʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        exit(1);
    }
    g_pcBcdaShmPtr = (char *)shm_connect("BCDA",NULL);
    if ( g_pcBcdaShmPtr == NULL )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><appSrv> cannot connect shared memory 'BCDA'!",__FILE__,__LINE__);
        BC_SendExeception("04","���ӹ����ڴ�BCDAʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        return(-1);
    }

    if ( DasConectDB()!=0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>Can not open oracle DB !",__FILE__,__LINE__);
        BC_SendExeception("04","����oracle DBʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        appSrvExit(0);
    }
    
    if ( IsoLoad8583config(&iso8583_ZDXTconf[0],"iso8583_ZDXT.conf") < 0 )
    {

        dcs_log(0,0,"<FILE:%s,LINE:%d>Load term system interface iso8583config() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        BC_SendExeception("04","װ��iso8583_TERM.confʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        appSrvExit(0);
    }
     
    if ( IsoLoad8583config(&iso8583_GDYLconf[0],"iso8583_GDYL.conf") < 0 )
    {
        BC_SendExeception("04","װ��iso8583_GDYL.confʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load mchn system interface iso8583config() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }

    if ( IsoLoad8583config(&iso8583_AJXTconf[0],"iso8583_AJXT.conf") < 0 )
    {
        BC_SendExeception("04","iso8583_AJXT.confʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load POSB system interface iso8583_AJXTconf() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }
    
    if ( IsoLoad8583config(&iso8583_JAVAconf[0],"iso8583_JAVA.conf") < 0 )
    {
        BC_SendExeception("04","iso8583_JAVA.confʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load JAVA system interface iso8583_JAVAconf() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }

    if ( IsoLoad8583config(&iso8583_CUPSconf[0],"iso8583_CUPS.conf") < 0 )
    {
        BC_SendExeception("04","iso8583_CUPS.confʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load CUPS system interface iso8583_CUPSconf() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }
    
    if ( CreateMyFolder() < 0 )
    {
        BC_SendExeception("04","����folder�ļ���ʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
        appSrvExit(0);
    }
    
    //do my main logic in a infinite loop    

    dcs_log(0,0,"*************************************************\n"
            "!!        Application Servers startup completed.        !!\n"
            "*************************************************\n");

    if ( 1 != DESInit() )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d> DESInit() failed:%s\n",__FILE__,__LINE__);
        BC_SendExeception("04","Des��ʼ��ʧ�ܣ�appSrv��������ʧ�ܣ��˳�");
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
            //������
            process_cmdi(caBuffer,fromFid,iRead);
        } else 
        {
            //������Ϣ
            process(caBuffer,fromFid,iRead);
        }

    }//for(;;)
    
    return -1;
}


int process_SignIn(ti_bc_trans_inf_def *p_tiBcTransInf, tl_bc_trans_log_def *p_BcTransLog,ISO_data *iso)
{
	int iRet = 0;
	int sqlCode = 0;
	char procedure[5 + 1]; //�������
	ti_bc_ins_inf_def tiBcInsInf; // ���ջ����Ļ�����Ϣ
	
	//init variant
	memset( &tiBcInsInf, 0, sizeof(tiBcInsInf) );
	strcpy( tiBcInsInf.line_nm,  p_BcTransLog->trans_chnl );
	strcpy( tiBcInsInf.packet_type,  p_BcTransLog->packet_type );
	tiBcInsInfOpr(GLB_DB_SELECT2, &tiBcInsInf, &sqlCode);
	if( 0 != sqlCode ){
		dcs_log(0, 0,"<FILE:%s,LINE:%d>������·���Ʋ�ѯ������Ϣʧ��(line_nm:%s, packet_type:%s).\n ",__FILE__,__LINE__, tiBcInsInf.line_nm, tiBcInsInf.packet_type );
		return -1;
	}
	
	// ����ԭʼ���״�����
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
	    BC_SendExeception("02","<FILE:%s,LINE:%d>δ�ҵ��ý��׵ĺ�������ʽ���������ñ�ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);
	
	    //û���ҵ��ý��׵ĺ�������ʽ�����ݿ����ô���
	    //û���ҵ��ý��׵�ת����Ϣ
	    dcs_log(0,0,"<FILE:%s,LINE:%d>δ�ڱ�ti_bc_trans_det���ҵ��ý��׵Ĵ���ʽ��trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    //����
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
  
  dcs_log(0,0,"<FILE:%s,LINE:%d>���õĴ�����(%s)������.\n",__FILE__,__LINE__,procedure);
	return -1;
}

//IVR ������������Ϣ
int IVR_entry_on_password(char *ivrdata, int ifoldid, int ivrdatasize)
{
	//variant
	int iRet = -1;
	int sqlCode = 0;
	char procedure[5 + 1]; //�������
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
          
    dcs_log(0,0,"<FILE:%s,LINE:%d>start to call tlBcTransLogOpr ����ԭʼ������Ϣ��key:%s.\n",__FILE__,__LINE__,  pIVR->key );
    tlBcTransLogOpr(GLB_DB_SELECT11, p_BcTransLog_o, &sqlCode);
	  if ( 0 != sqlCode )
	  {
	      BC_SendExeception("02","��ѯ������־��Ϣʧ�ܣ�δ�ҵ�Ӧ���׵���������Ϣ. key:%s\n",  pIVR->key );
	      BC_SendMonitor_trans("0","3", p_BcTransLog_o, 0); //&g_tiBcTransInf);
	      //δ��ѯ�����ݿ��¼��������Ч��Ӧ��
	      dcs_log(&g_TransMsg, sizeof(BcTransMsg),"<FILE:%s,LINE:%d>�յ�ivr�������ݿ���δ�ҵ��ý��ף�key:%s.\n ",__FILE__,__LINE__,  pIVR->key );
	      return -1;
	  }
    *g_TransLock = '1'; 
    memset( g_rtIns_id_cd, 0, sizeof(g_rtIns_id_cd) );
    strcpy(g_rtIns_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    g_iSrcFoldId = fold_locate_folder( p_BcTransLog_o->trans_chnl );
        
    // ��ѯ����������Ϣ
		memset(&g_tiBcTransInf, 0, sizeof(ti_bc_trans_inf_def) );
		strcpy(g_tiBcTransInf.trans_chnl, p_BcTransLog_o->trans_chnl);
		strcpy(g_tiBcTransInf.msg_tp,p_BcTransLog_o->msg_tp);
		strcpy(g_tiBcTransInf.proc_cd,p_BcTransLog_o->proc_cd);
		strcpy(g_tiBcTransInf.pos_cond_cd,p_BcTransLog_o->pos_cond_cd);
		strcpy(g_tiBcTransInf.mchnt_tp,p_BcTransLog_o->mchnt_tp);
		
		iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
		if ( -3114 == sqlCode || -3135 == sqlCode )
		{
		    BC_SendExeception("03","���ݿ������쳣���Զ��������ݿ⡣");
		    dcs_log(0,0,"<FILE:%s,LINE:%d>���ݿ������쳣���Զ��������ݿ⡣\n",__FILE__,__LINE__);
		
		    if ( DasConectDB()==0 )
		    {
		        iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
		    }
		}
		
	 if ( 0 != sqlCode )
   {
   	 BC_SendExeception("02","��ѯ����������Ϣʧ��.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",p_BcTransLog_o->trans_chnl,p_BcTransLog_o->msg_tp,p_BcTransLog_o->proc_cd,p_BcTransLog_o->pos_cond_cd,p_BcTransLog_o->mchnt_tp);
   	 BC_SendMonitor_trans("0","3", p_BcTransLog_o, 0);
   	 dcs_log(0,0,"��ѯ����������Ϣʧ��.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",__FILE__,__LINE__,p_BcTransLog_o->trans_chnl,p_BcTransLog_o->msg_tp,p_BcTransLog_o->proc_cd,p_BcTransLog_o->pos_cond_cd,p_BcTransLog_o->mchnt_tp);
   	 BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
   	 return -1;
   }
	
	  // ����ԭʼ���״�����
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
	    BC_SendExeception("02","<FILE:%s,LINE:%d>δ�ҵ��ý��׵ĺ�������ʽ���������ñ�ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_SendMonitor_trans("0","3", p_BcTransLog_o,&g_tiBcTransInf);
	
	    //û���ҵ��ý��׵ĺ�������ʽ�����ݿ����ô���
	    //û���ҵ��ý��׵�ת����Ϣ
	    dcs_log(0,0,"<FILE:%s,LINE:%d>δ�ڱ�ti_bc_trans_det���ҵ��ý��׵Ĵ���ʽ��trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_UnLockTrans(p_BcTransLog_o);  
	    
	    //����
	    return -1;
	  }
	   
	  memset(procedure, 0, sizeof(procedure) );
	  strcpy(procedure, g_tiBcTransDet.procedure); 
    rtrim(procedure);
   
	  //������Ӧ�ĺ���
	  if( 0 == strcmp("Z0400", procedure) ){
	  	return BusIVR_Z22_ev1(&g_tiBcTransInf, &g_tiBcTransDet, ivrdata);
	  }else {
	  	BC_UnLockTrans(p_BcTransLog_o); 
	  	dcs_log(0,0,"<FILE:%s,LINE:%d>�ڱ�ti_bc_trans_det�����õĴ�����procedure������IVR PASSW0RD������ȷ��trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,  procedure, g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new); 
	  	return -1;
	  }
    
	return 0;	
}


//IVR �����˲�ѯ�ؼ������Ϣ
int IVR_entry_on_primaryField(char *ivrdata, int ifoldid, int ivrdatasize)
{
	//variant
	int iRet = -1;
	int sqlCode = 0;
	char procedure[5 + 1]; //�������
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
          
    dcs_log(0,0,"<FILE:%s,LINE:%d>start to call tlBcTransLogOpr ����ԭʼ������Ϣ��key:%s.\n",__FILE__,__LINE__,  pIVR->key );
    tlBcTransLogOpr(GLB_DB_SELECT11, p_BcTransLog_o, &sqlCode);
	  if ( 0 != sqlCode )
	  {
	      BC_SendExeception("02","��ѯ������־��Ϣʧ�ܣ�δ�ҵ�Ӧ���׵���������Ϣ. key:%s\n",  pIVR->key );
	      BC_SendMonitor_trans("0","3", p_BcTransLog_o, 0); //&g_tiBcTransInf);
	      //δ��ѯ�����ݿ��¼��������Ч��Ӧ��
	      dcs_log(&g_TransMsg, sizeof(BcTransMsg),"<FILE:%s,LINE:%d>�յ�ivr�������ݿ���δ�ҵ��ý��ף�key:%s.\n ",__FILE__,__LINE__,  pIVR->key );
	      return -1;
	  }
    *g_TransLock = '1'; 
    memset( g_rtIns_id_cd, 0, sizeof(g_rtIns_id_cd) );
    strcpy(g_rtIns_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    g_iSrcFoldId = fold_locate_folder( p_BcTransLog_o->trans_chnl );
        
    // ��ѯ����������Ϣ
	memset(&g_tiBcTransInf, 0, sizeof(ti_bc_trans_inf_def) );
	strcpy(g_tiBcTransInf.trans_chnl, p_BcTransLog_o->trans_chnl);
	strcpy(g_tiBcTransInf.msg_tp,p_BcTransLog_o->msg_tp);
	strcpy(g_tiBcTransInf.proc_cd,p_BcTransLog_o->proc_cd);
	strcpy(g_tiBcTransInf.pos_cond_cd,p_BcTransLog_o->pos_cond_cd);
	strcpy(g_tiBcTransInf.mchnt_tp,p_BcTransLog_o->mchnt_tp);
	iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
	if ( -3114 == sqlCode || -3135 == sqlCode)
	{
	    BC_SendExeception("03","���ݿ������쳣���Զ��������ݿ⡣");
	    dcs_log(0,0,"<FILE:%s,LINE:%d>���ݿ������쳣���Զ��������ݿ⡣\n",__FILE__,__LINE__);
	    if ( DasConectDB()==0 )
	    {
	        iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
	    }
	}
	if ( 0 != sqlCode )
	{
		BC_SendExeception("02","��ѯ����������Ϣʧ��.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",p_BcTransLog_o->trans_chnl,p_BcTransLog_o->msg_tp,p_BcTransLog_o->proc_cd,p_BcTransLog_o->pos_cond_cd,p_BcTransLog_o->mchnt_tp);
		BC_SendMonitor_trans("0","3", p_BcTransLog_o, 0);
		BC_UnLockTrans(&g_TransMsg_o.BcTransLog);
		dcs_log(0,0,"<FILE:%s,LINE:%d>��ѯ����������Ϣʧ��.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",__FILE__,__LINE__,p_BcTransLog_o->trans_chnl,p_BcTransLog_o->msg_tp,p_BcTransLog_o->proc_cd,p_BcTransLog_o->pos_cond_cd,p_BcTransLog_o->mchnt_tp);
		return -1;
	}
  	// ����ԭʼ���״�����
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
	    BC_SendExeception("02","<FILE:%s,LINE:%d>δ�ҵ��ý��׵ĺ�������ʽ���������ñ�ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_SendMonitor_trans("0","3", p_BcTransLog_o,&g_tiBcTransInf);
	
	    //û���ҵ��ý��׵ĺ�������ʽ�����ݿ����ô���
	    //û���ҵ��ý��׵�ת����Ϣ
	    dcs_log(0,0,"<FILE:%s,LINE:%d>δ�ڱ�ti_bc_trans_det���ҵ��ý��׵Ĵ���ʽ��trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_UnLockTrans(p_BcTransLog_o);  
	    
	    //����
	    return -1;
	  }
	   
	  memset(procedure, 0, sizeof(procedure) );
	  strcpy(procedure, g_tiBcTransDet.procedure); 
    rtrim(procedure);
   
	  //������Ӧ�ĺ���
	  if( 0 == strcmp("Z0400", procedure) ){
	  	return BusIVR_Z22_ev1(&g_tiBcTransInf, &g_tiBcTransDet, ivrdata);
	  }else {
	  	BC_UnLockTrans(p_BcTransLog_o); 
	  	dcs_log(0,0,"<FILE:%s,LINE:%d>�ڱ�ti_bc_trans_det�����õĴ�����procedure������IVR ��ȡ�ؼ��򣩲���ȷ��trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,  procedure, g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new); 
	  	return -1;
	  }
    
	return 0;	
}

int IVR_entry(char *ivrdata, int ifoldid, int ivrdatasize)
{
	IVR_STRUCT *pIVR = (IVR_STRUCT *)ivrdata;	
	struct TimeOut_REC *p_TimeOutRec = &g_TimeOutRec;
	dcs_debug(ivrdata, ivrdatasize,"<FILE:%s,LINE:%d>�յ�IVR��Ϣ(%s).\n",__FILE__,__LINE__,ivrdata);
    TimeOut_Key *tk = (TimeOut_Key *)p_TimeOutRec->caKey;
    memset(p_TimeOutRec, 0, sizeof(struct TimeOut_REC));
    memcpy_safe((char*)tk, pIVR->key, sizeof(tk->sys_tra_no) + sizeof(tk->term_id) + sizeof(tk->mchnt_cd));
	BC_DeleteTimeOut1(&g_TimeOutRec);    
	if( 0 == memcmp("OP01", pIVR->opcode, 4) )//����������Ӧ��
	{
		dcs_debug(0, 0,"<FILE:%s,LINE:%d>�յ�OP01Ӧ��\n",__FILE__,__LINE__);
		return 0;
	}
	else if( 0 == memcmp("OP02", pIVR->opcode, 4) )//��CVV����Ӧ��
	{
		dcs_debug(0, 0,"<FILE:%s,LINE:%d>�յ�OP02Ӧ��\n",__FILE__,__LINE__);
		return 0;
	}
	else if( 0 == memcmp("OP03", pIVR->opcode, 4) )
	{
		return IVR_entry_on_password(ivrdata, ifoldid, ivrdatasize);
	}else if( 0 == memcmp("OP04", pIVR->opcode, 4) )
	{
		dcs_debug(0, 0,"<FILE:%s,LINE:%d>�յ�Ӧ��OP04Ӧ��\n",__FILE__,__LINE__);
		return IVR_entry_on_primaryField(ivrdata, ifoldid, ivrdatasize);
	}else
	{
		dcs_log(ivrdata, ivrdatasize,"<FILE:%s,LINE:%d>IVR_entry: opcode �޷�ʶ��",__FILE__,__LINE__);
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
        //��ʱ����
        process_cmdi_timeout(cmd_data, iFid, iLen-8);
    } 
    else if ( 0 == memcmp(cmd_type, "0002", 4) )
    {
        process_cmdi_timeout(cmd_data, iFid, iLen-8);
    } 
    else if ( 0 == memcmp(cmd_type, "0003", 4) )
    {
        //����ǩ��
        //�̻���ǩ��
	      mchntSignIn(cmd_data);
    }else if ( 0 == memcmp(cmd_type, "0005", 4) )
    {
    	//IVR process
    	IVR_entry(srcBuf, iFid, iLen );
    }
    return 0;
}

/*�����ն��������ͽ��ױ��ĸ�ʽΪ8583�ն˱���*/
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
    /*���*/
    if ( 0 > BC_UnPack( &iso, srcBuf, iLen) )
    {
        dcs_log(srcBuf,iLen,"<FILE:%s,LINE:%d>Can not unpacket Terminal Msg!datalen=%d",__FILE__,__LINE__,iLen);
        return;
    }
    dcs_log(0,0, "<FILE:%s,LINE:%d>g_iSrcFoldName[%s]",__FILE__,__LINE__,g_iSrcFoldName);

    memset(packet_type, 0, sizeof(packet_type));
    memcpy(packet_type, srcBuf, 4);

    PrintISO( &iso,"�յ�����",0);

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

    /*���ݽ������ͣ��ж���һ������*/
    /*�����ݿ��ж�ȡ��һ�����̣��ؼ���Ϊ�����ã���Ϣ���ͣ������룬ȡ����һ�����̱�ʶ*/
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
    dcs_log(0,0,"<FILE:%s,LINE:%d><business_prepare_return>opFlag<%c>����û�гɹ���׼�����ء�\n",__FILE__,__LINE__,*p_tiBcTransInf->opFlag);
    
    // ���� N00000
    if ( '0' == *p_tiBcTransInf->opFlag )
    {
        //����Ӧ��Ľ���
    } else if ( '1' == *p_tiBcTransInf->opFlag )
    {
        //��ҪӦ��#39�Ľ���
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
        //��ҪӦ��#28�Ľ���
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
        //��Ҫֱ��Ӧ��6λӦ����Ľ���
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
		dcs_debug(0,0,"<FILE:%s,LINE:%d>�ڿ�bin����δ�ҵ���bin(%s)��Ϣ��sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__,g_tiBcCardBin.card_bin, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
		strcpy(OutRespCode, RESP_CODE_CUS008);
		return -1;
	}
	return 0;
}

// ���������
int CardBinBlCheck(char *OutRespCode,  tl_bc_trans_log_def *p_BcTransLog)
{
	int sqlCode = -1;
	char tmpbuf[1024];
	memset( &g_tiBcBlackList, 0, sizeof(ti_bc_black_list_def) );
	
  strcpy_safe(g_tiBcBlackList.card_idno_inf, p_BcTransLog->pri_acct_no, sizeof(g_tiBcBlackList.card_idno_inf)-1);
  tiBcBlackListOpr(GLB_DB_SELECT1, &g_tiBcBlackList, &sqlCode);
  
  if( 0 == sqlCode)
  	{
			sprintf(tmpbuf, "����(%s)�ں������У����ױ��ܾ���",p_BcTransLog->pri_acct_no);
  	  strcpy(OutRespCode, RESP_CODE_CUS005);
			BC_SendExeception("03", "����, %s", tmpbuf);
  		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
  		{
    		BC_SendExeception("03","����, ��¼�쳣��ʧ��.");
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>,��¼�쳣��ʧ��.",__FILE__,__LINE__);
  		}
      // ��鲻ͨ��     
      dcs_log(0,0,"<FILE:%s,LINE:%d>��bin(%s)�ں������У����ױ��ܾ���sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__,g_tiBcBlackList.card_idno_inf, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
    	 return -1;
    }else if(SQL_ERD_NONE_RECORD == sqlCode){
    	strcpy(OutRespCode, "N00000");
    	return 0;
    }else{
    	dcs_log(0,0,"<FILE:%s,LINE:%d>��ѯ�̻���bin(%s)��������Ϣʱ�����쳣[sqlCode:%d]��sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__,  g_tiBcBlackList.card_idno_inf, sqlCode, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
    	strcpy(OutRespCode, "N00096");
    	return -1;
    }
}

// ���������
int CardBinWlCheck(char *OutRespCode,  tl_bc_trans_log_def *p_BcTransLog)
{
	int sqlCode = -1;
	memset( &g_tiBcWhiteList, 0, sizeof(ti_bc_white_list_def) );
	
  strcpy_safe(g_tiBcWhiteList.card_idno_inf, p_BcTransLog->pri_acct_no, sizeof(g_tiBcWhiteList.card_idno_inf)-1);
  strcpy_safe(g_tiBcWhiteList.mchnt_cd, p_BcTransLog->mchnt_cd, sizeof(g_tiBcWhiteList.mchnt_cd)-1);
  
  tiBcWhiteListOpr(GLB_DB_SELECT1, &g_tiBcWhiteList, &sqlCode);
  
  if( 0 == sqlCode){
      // �ڰ�������
    	 strcpy(OutRespCode, "N00000");
    	 return 0;
    }else if(SQL_ERD_NONE_RECORD == sqlCode){
    	 strcpy(OutRespCode, "N00000");
    	 return 1;
    }else{
    	dcs_log(0,0,"<FILE:%s,LINE:%d>��ѯ�̻���bin(%s)��������Ϣʱ�����쳣[sqlCode:%d]��sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__,  g_tiBcWhiteList.card_idno_inf, sqlCode, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
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
	
	//��������
	if(IsCreditCard(p_BcTransLog->card_attr)){
		max_at =   atof ( p_tiBcMchntInf->credit_limit_times );			
		dcs_debug(0,0,"<FILE:%s,LINE:%d>����̻�(%s)���ʽ��(%f)����(max:%f).\n",__FILE__,__LINE__,p_BcTransLog->mchnt_cd, trans_at, max_at );	
		if( 0 != max_at && trans_at > max_at )
		{
			sprintf(tmpbuf, "���׽��[%.2fԪ]����[%s(%s)]�̻����ÿ����ʽ������[%.2fԪ]�����ױ��ܾ���",trans_at/100, rtrim(p_tiBcMchntInf->mchnt_cn_abbr), p_BcTransLog->mchnt_cd, max_at/100);
    	strcpy(OutRespCode, RESP_CODE_CUS012 );
			BC_SendExeception("03", "����, %s", tmpbuf);
  		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
  		{
    		BC_SendExeception("03","����, ��¼�쳣��ʧ��.");
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>,��¼�쳣��ʧ��.",__FILE__,__LINE__);
  		}
			dcs_log(0,0,"<FILE:%s,LINE:%d>�ﵽ�̻�(%s)���ʽ������(%f)���ޣ����ױ��ܾ���sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__, p_BcTransLog->mchnt_cd, max_at, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
    	return -1;
		}
	}else{
		max_at =  atof ( p_tiBcMchntInf->debit_limit_times );	
		dcs_debug(0,0,"<FILE:%s,LINE:%d>����̻�(%s)���ʽ��(%f)����(max:%f).\n",__FILE__,__LINE__,p_BcTransLog->mchnt_cd, trans_at, max_at );	
		if( 0 != max_at && trans_at > max_at )
		{
			sprintf(tmpbuf, "���׽��[%.2fԪ]����[%s(%s)]�̻���ǿ����ʽ������[%.2fԪ]�����ױ��ܾ���",trans_at/100, rtrim(p_tiBcMchntInf->mchnt_cn_abbr), p_BcTransLog->mchnt_cd, max_at/100);
    	strcpy(OutRespCode, RESP_CODE_CUS012 );
			BC_SendExeception("03", "����, %s", tmpbuf);
  		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
  		{
    		BC_SendExeception("03","����, ��¼�쳣��ʧ��.");
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>,��¼�쳣��ʧ��.",__FILE__,__LINE__);
  		}
			dcs_log(0,0,"<FILE:%s,LINE:%d>�ﵽ�̻�(%s)���ʽ������(%f)���ޣ����ױ��ܾ���sys_tra_no:[%s],term_id:[%s],mchnt_cd:[%s].\n",__FILE__,__LINE__, p_BcTransLog->mchnt_cd, max_at, p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd );	
    	return -1;
		}
	}
	
	//�̻����޶���
	max_at =   atof ( p_tiBcMchntInf->limit_day );	
	if( 0!= max_at ){
		//��ȡ��ǰ����,���
		iRet = tlBcTransLogOpr2(&allAt, &allNum, settle_dt, 0 /*p_BcTransLog->pri_acct_no*/, p_BcTransLog->mchnt_cd, &sqlCode);
		if( 0!= iRet ){
			strcpy(OutRespCode, "N00096");
			return -1;
		}
		dcs_debug(0,0,"<FILE:%s,LINE:%d>�̻����޶�(%f)��飬��ǰ���%f, ��ǰ�����㽻���ܱ���%f, �ܽ��%f.\n",__FILE__,__LINE__, max_at, trans_at, allNum, allAt );
		if( max_at < allAt + trans_at )
		{
			sprintf(tmpbuf, "���׽��[%.2fԪ],�ѽ��ɹ����׽��[%.2fԪ]���ܽ���[%s(%s)]�̻��ܽ��׽������[%.2fԪ]�����ױ��ܾ���",trans_at/100, allAt/100, rtrim(p_tiBcMchntInf->mchnt_cn_abbr), p_BcTransLog->mchnt_cd, max_at/100);
			strcpy(OutRespCode, RESP_CODE_CUS002);
			BC_SendExeception("03", "����, %s", tmpbuf);
  		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
  		{
    		BC_SendExeception("03","����, ��¼�쳣��ʧ��.");
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>,��¼�쳣��ʧ��.",__FILE__,__LINE__);
  		}
			return -1;
		}
	}	
	strcpy(OutRespCode, "N00000" );
	return 0;
}

//ȫ�ֳֿ�������������
int TimesLimitCheck( char *OutRespCode,  tl_bc_trans_log_def *p_BcTransLog )
{
	int iRet ;
	int sqlCode;
	double maxNum = 0;
	double allNum = 0;
	char settle_dt[8]; 
	char tmpbuf[1024];
	
	//��ȡ����������
	strcpy(g_tiBcMidDayLimitTims.fwd_ins_id_cd, g_TransMsg.BcTransLog.fwd_ins_id_cd);
	iRet = tiBcMidDayLimitTimsOpr(GLB_DB_SELECT1, &g_tiBcMidDayLimitTims, &sqlCode);
	if( SQL_ERD_NONE_RECORD ==  sqlCode )
	{
		dcs_debug(0,0,"<FILE:%s,LINE:%d><business_prepare>������б�����������ơ�\n",__FILE__,__LINE__);		
		return 0;
	}else if(0 == sqlCode )
	{
		if(IsCreditCard(p_BcTransLog->card_attr)){
			maxNum = (double)g_tiBcMidDayLimitTims.credit_times_day;
		}else{
			maxNum = (double)g_tiBcMidDayLimitTims.debit_times_day;
		}
		if( 0 == maxNum){
			//������гֿ�������������
			strcpy(OutRespCode, "N00000");
			return 0;
		}
	}else
	{
		//���ݿ��쳣
		strcpy(OutRespCode, "N00096");
		return -1;
	}
	
	memset(settle_dt, 0, sizeof(settle_dt));
	glb_GetLocalTimeString(settle_dt, sizeof(settle_dt), "%m%d");
	//��ȡ��ǰ����,���
	iRet = tlBcTransLogOpr2(0, &allNum, settle_dt, p_BcTransLog->pri_acct_no, 0, &sqlCode);
	if( 0!= iRet ){
		strcpy(OutRespCode, "N00096");
		return -1;
	}
	
	dcs_debug(0,0,"<FILE:%s,LINE:%d><business_prepare>�������Ƽ��(maxNum:%f, allNum:%f).��\n",__FILE__,__LINE__, maxNum, allNum);		
	
	if( allNum >= maxNum )
	{
		strcpy(OutRespCode, RESP_CODE_CUS014);
		sprintf(tmpbuf, "��[%s],�ѽ��ɹ�����[%.0f����]���ﵽ��������������[%.0f]�����ױ��ܾ���",p_BcTransLog->pri_acct_no, allNum, maxNum);
		BC_SendExeception("03", "����, %s", tmpbuf);
		if( 0 > BC_exp_log(OutRespCode+2, tmpbuf, g_Rcv_Buffer, g_RcvBufferSize))
		{
	  		BC_SendExeception("03","����, ��¼�쳣��ʧ��.");
	  		dcs_log(0, 0, "<FILE:%s,LINE:%d>,��¼�쳣��ʧ��.",__FILE__,__LINE__);
		}
		dcs_log(0,0,"<FILE:%s,LINE:%d><business_prepare>������������(maxNum:%f, allNum:%f).��\n",__FILE__,__LINE__, maxNum, allNum);
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
    int iRet;          // ���ش���
    int sqlCode;       // sql ���ش���
    char trans_flag[4+1];
    char procedure[5 + 1]; //�������
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

    // ��ѯ����������Ϣ
    memset(&g_tiBcTransInf, 0, sizeof(ti_bc_trans_inf_def) );
    strcpy(g_tiBcTransInf.trans_chnl,trans_chnl);
    strcpy(g_tiBcTransInf.msg_tp,msg_tp);
    strcpy(g_tiBcTransInf.proc_cd,proc_cd);
    strcpy(g_tiBcTransInf.pos_cond_cd,pos_cond_cd);
    strcpy(g_tiBcTransInf.mchnt_tp,mchnt_tp);

    iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
    if ( -3114 == sqlCode || -3135 == sqlCode )
    {
        BC_SendExeception("03","���ݿ������쳣���Զ��������ݿ⡣");
        dcs_debug(0,0,"<FILE:%s,LINE:%d>���ݿ������쳣���Զ��������ݿ⡣\n",__FILE__,__LINE__);

        if ( DasConectDB()==0 )
        {
            iRet = tiBcTransInfOpr(GLB_DB_SELECT1, &g_tiBcTransInf, &sqlCode);
        }
    }
    if ( 0 != sqlCode )
    {
        BC_SendExeception("02","��ѯ����������Ϣʧ��.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
        BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);

        //��⣬ʧ�ܵĴ�������
        strcpy(g_TransMsg.BcTransLog.is_over, "1");
        if ( sqlCode == SQL_ERD_NONE_RECORD )
        {
            strcpy(g_TransMsg.BcTransLog.resp_cd,"40");
            //û�����øý�����Ϣ
            dcs_log(0,0,"<FILE:%s,LINE:%d>û����ti_bc_trans_inf�������øý�����Ϣ��trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s\n",__FILE__,__LINE__,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
            BC_SendExeception("03","�������ʧ��.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
        } else
        {
            strcpy(g_TransMsg.BcTransLog.resp_cd,"96");
            BC_SendExeception("04","���棬���ݿ���ܴ����쳣���뾡����ϵ������Ա����.");
        }
        iRet = BC_InsertIntoDB(&g_TransMsg.BcTransLog, &sqlCode);
        if ( 0!=sqlCode )
        {
            BC_SendExeception("03","�������ʧ��.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
        }

        //����
        BC_exp_log(EXP_CODE_CUS001, "δ��ti_bc_trans_inf�������øý�����Ϣ", g_Rcv_Buffer,g_RcvBufferSize);
        return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,"N00040");
    }

    if( 0 == strcmp("Z00", rtrim(g_tiBcTransInf.TransType) ))
    {
    	//ǩ��Ӧ��
    	iRet = process_SignIn(&g_tiBcTransInf, &g_TransMsg.BcTransLog, iso);
    	if( 0 == iRet ) return 0;
    }
    
    //�Ƿ���Ҫɾ����ʱ��
    //tiBcTransInf.respFlag: 0:��Ӧ���� 1��Ӧ����Ҫɾ����ʱ��
    memset(&g_TimeOutRec, 0, sizeof(struct TimeOut_REC));
    if ( '0' == *g_tiBcTransInf.respFlag )
    {
        //��Ӧ����
    } else if ( '1' == *g_tiBcTransInf.respFlag )
    {
    	  TimeOut_ReMark *trm = (TimeOut_ReMark *)g_TimeOutRec.caRemark;
        
        BC_PacketTimeOut(&g_TimeOutRec, iso, 1);
        //ɾ����ʱ��
        if ( 0 == BC_DeleteTimeOut1(&g_TimeOutRec) )
        {
            *flag = '1' ;
            memset(p_BcTransLog_o->sys_tra_no, 0, sizeof(p_BcTransLog_o->sys_tra_no));
            memcpy(p_BcTransLog_o->sys_tra_no, trm->sys_tra_no, 6);
            memset(p_BcTransLog_o->term_id, 0, sizeof(p_BcTransLog_o->term_id));
            memcpy(p_BcTransLog_o->term_id, trm->term_id, 8);
            memset(p_BcTransLog_o->mchnt_cd, 0, sizeof(p_BcTransLog_o->mchnt_cd));
            memcpy(p_BcTransLog_o->mchnt_cd, trm->mchnt_cd, 20);
            dcs_debug(0,0,"<FILE:%s,LINE:%d>ɾ����ʱ��ɹ���sys_tra_no:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,trm->sys_tra_no,trm->term_id,trm->mchnt_cd );
        } else
        {
            //ɾ��ʧ�ܣ�����Ϊ�ٵ���Ӧ��һ��ɲ����������˽�����ȷ�ϴ���            
            dcs_log(0,0,"<FILE:%s,LINE:%d>ɾ����ʱ��ʧ�ܣ�sys_tra_no:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,trm->sys_tra_no,trm->term_id,trm->mchnt_cd );
            return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,RESP_CODE_CUS007 );            
        }
    } else if ( '2' == *g_tiBcTransInf.respFlag || '3' == *g_tiBcTransInf.respFlag)
    {
        BC_PacketTimeOut(&g_TimeOutRec, iso, '2' == *g_tiBcTransInf.respFlag ? 1 : 0);
        //ɾ���˵���ʱ��
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
            dcs_debug(0,0,"<FILE:%s,LINE:%d>ɾ����ʱ��ɹ���sys_tra_no:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,trm->sys_tra_no,trm->term_id,trm->mchnt_cd );
            
        } else
        {
            //ɾ��ʧ�ܣ�����Ϊ�ٵ���Ӧ��һ��ɲ����������˽�����ȷ�ϴ���
            dcs_log(0,0,"<FILE:%s,LINE:%d>ɾ���˵���ʱ��ʧ�ܣ�sys_tra_no:%s,term_id:%s,mchnt_cd:%s.\n",__FILE__,__LINE__,g_TransMsg_o.BcTransLog.sys_tra_no,g_TransMsg_o.BcTransLog.term_id,g_TransMsg_o.BcTransLog.mchnt_cd);
            return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,RESP_CODE_CUS006);            
        }
    }

    //��ʼ����״̬�������Ϊ*
    if( *flag != '1') strcpy(g_tiBcTransDet.trans_flag,"*");

    //�Ƿ���Ҫ��ѯ���ݿ⣬����ѯ��ʽ(key value)
    //һ����Ҫɾ��ʱ������Ҫ��ѯ���ݿ�
    //tiBcTransInf.DbFlag: 0:����������ݿ⣬1,2,3,...�����ݿ��ѯ��ʽ
    //dcs_log(&g_tiBcTransInf, sizeof(g_tiBcTransInf), "<FILE:%s,LINE:%d>show transinf",__FILE__,__LINE__);
    if ( '0' != *g_tiBcTransInf.DbFlag ) //��ѯ���ݿ� 1: ���ύ��Ϣ��ѯ��2����������Ϣ��ѯ
    {
        //��ѯ���ݿ� GLB_DB_SELECT1
        //���ڲ�ѯ�ظ����ף�����Ҫͨ����ѯ���ݿ�õ�trans_flag����״̬���롣
        TimeOut_ReMark *trm = (TimeOut_ReMark *)g_TimeOutRec.caRemark;
        if ( *flag == '1' && *trm->loc_trans_dt !=0 )
        {
        	   memset(p_BcTransLog_o->loc_trans_dt, 0, sizeof(p_BcTransLog_o->loc_trans_dt));
             memcpy(p_BcTransLog_o->loc_trans_dt, trm->loc_trans_dt, 4);
             dcs_log(0,0,"<FILE:%s,LINE:%d>�ӳ�ʱ���л�ȡ�������ڣ�loc_trans_dt:%s.\n",__FILE__,__LINE__,trm->loc_trans_dt);
        }
        dcs_debug(0,0,"<FILE:%s,LINE:%d>start to call tlBcTransLogOpr ����ԭʼ������Ϣ��mchnt_cd:[%s],term_id:[%s],sys_tra_no:[%s],loc_trans_dt:[%s]",__FILE__,__LINE__,p_BcTransLog_o->mchnt_cd,p_BcTransLog_o->term_id,p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->loc_trans_dt);
        iRet = tlBcTransLogOpr('1' == g_tiBcTransInf.DbFlag[0] ?  GLB_DB_SELECT11 : GLB_DB_SELECT13, &g_TransMsg_o.BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("02","��ѯ������־��Ϣʧ�ܣ�δ�ҵ�Ӧ���׵���������Ϣ.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
            BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);
            //δ��ѯ�����ݿ��¼��������Ч��Ӧ��
            //û���ҵ����ն���Ϣ
            dcs_log(&g_TransMsg,sizeof(BcTransMsg),"<FILE:%s,LINE:%d>�����ݿ���δ�ҵ��ý��ף�trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s\n",__FILE__,__LINE__,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
            //����
            return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,"N00022");
        }
        *g_TransLock = '1';

        //ͨ����ѯ��ʷ���ף���Ҫ�õ��Ľ���Ҫ��
        strcpy(g_tiBcTransDet.trans_flag,g_TransMsg_o.BcTransLog.trans_flag);
        strcpy(g_rtIns_id_cd, g_TransMsg_o.BcTransLog.rcv_ins_id_cd );
        dcs_log(0,0,"<FILE:%s,LINE:%d>�����ݿ��в���ԭʼ���׳ɹ���trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s\n",__FILE__,__LINE__,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);
    }    
   	else
   	{
   		if(0 != *p_BcTransLog_o->mchnt_cd) //���̻��ţ����̻���Ϣ
   		{
		    strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog_o->mchnt_cd);
		    tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
		    if ( 0 != sqlCode ) //û���ҵ��̻���Ϣ
		    {
				dcs_log(0,0, "�̻��Ų�����!(mchnt_cd:%s,sqlcode=%d)", p_tiBcMchntInf->mchnt_cd, sqlCode);
		    	return business_prepare_return(trans_chnl, msg_tp, iso, &g_tiBcTransInf, "N00003");
		    }
		}
   	}
    // ��ѯ��bin ��Ϣ
    if( '1' == *g_tiBcTransInf.CardBinDetFlag )
    {
    	if( 0 != SearchCardBinInfo(RespCode,  p_BcTransLog))//�鿨BIN·��
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
	    //����·�ɽ��ܻ������뵽���ݿ��ֶ�
       strcpy(g_TransMsg_o.BcTransLog.rcv_ins_id_cd ,g_rtIns_id_cd);
       strcpy(p_BcTransLog->rcv_ins_id_cd, g_rtIns_id_cd );
       dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ2210>�̻���bin·��ȷ��Ϊ[%s].\n ",__FILE__,__LINE__, g_rtIns_id_cd );
    }
    //��Ҫ�����̻�����غڰ��������޶���
    if( '1' == *g_tiBcTransInf.Mid_chk_flg) 
    {
		dcs_log(0, 0,"<FILE:%s,LINE:%d>�����̻��������޶���.(mchnt_cd:%s) ",__FILE__,__LINE__,g_tiBcMchntInf.mchnt_cd );
   	  	//���������
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
	   	  		else if( 0 == iRet )//�ڰ�������
	   	  		{
	   	  			dcs_debug(0, 0,"<FILE:%s,LINE:%d>����(%s)���̻���������(mchnt_cd:%s) ",__FILE__,__LINE__, p_BcTransLog->pri_acct_no, g_tiBcMchntInf.mchnt_cd );	
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
    //ʹ��ԭʼ���׼�״̬��ѯ  
    //   --  ����Ӧ�����޷�ͨ��ԭʼ���״������֣���Ҫ�ж�Ӧ���׵ı�־
    //   --  ��ʱ���׵Ĵ�����ڲ��ڴ˴����ɳ�ʱ���������ڴ���


//    strcpy(g_tiBcTransDet.trans_chnl,p_BcTransLog_o->trans_chnl);
    strcpy(g_tiBcTransDet.trans_chnl,trans_chnl);
    strcpy(g_tiBcTransDet.msg_tp,p_BcTransLog_o->msg_tp);
    strcpy(g_tiBcTransDet.proc_cd,p_BcTransLog_o->proc_cd);
    strcpy(g_tiBcTransDet.pos_cond_cd,p_BcTransLog_o->pos_cond_cd);
    strcpy(g_tiBcTransDet.mchnt_tp,p_BcTransLog_o->mchnt_tp);
    strcpy(g_tiBcTransDet.msg_tp_new,p_BcTransLog->msg_tp); //new msgtp
    strcpy(g_tiBcTransDet.trans_flag,p_BcTransLog_o->trans_flag);
    strcpy(g_tiBcTransDet.rcv_ins_id_cd, g_rtIns_id_cd);
    
    //������Ϣ���ͣ������룬����������룬�����ױ�ʶ��ȷ�����״������    
    dcs_debug(0,0,"<FILE:%s,LINE:%d>start to call tiBcTransDetOpr()ȷ�����״�����̣�trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s],rcv_ins_id_cd[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new,g_tiBcTransDet.rcv_ins_id_cd);
    iRet = tiBcTransDetOpr(GLB_DB_SELECT1, &g_tiBcTransDet, &sqlCode);
    if ( 0 != sqlCode )
    {
        BC_SendExeception("02","<FILE:%s,LINE:%d>δ�ҵ����׵ĺ�������ʽ���������ñ�ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
        BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);

        //û���ҵ��ý��׵ĺ�������ʽ�����ݿ����ô���
        //û���ҵ��ý��׵�ת����Ϣ
        dcs_log(0,0,"<FILE:%s,LINE:%d>δ�ڱ�ti_bc_trans_det���ҵ��ý��׵Ĵ���ʽ��trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);

        //��⣬ʧ�ܵĴ�������
        if ( '0' == *g_TransLock )
        {
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->resp_cd,"40");
            iRet = BC_InsertIntoDB(p_BcTransLog, &sqlCode);            
        }
        BC_UnLockTrans(p_BcTransLog_o);  

        //����
        return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,"N00040");
    }

    //�ҵ���Ӧ�Ĵ���������
    strcpy(procedure, g_tiBcTransDet.procedure); 
    rtrim(procedure);
   
    dcs_debug(0,0,"<FILE:%s,LINE:%d>ȷ�����׵Ĵ������Ϊ%s" ,__FILE__,__LINE__,procedure);

    //�˴�Ϊ���еĽ��׵Ĵ������    
    if ( 0 );

    //�ն�ǩ������
    ON_BC_FUNC("T0010",Bus_SignInReq_term)

    //T22�ɷѽ��׵����д������
    /*
    ON_BC_FUNC("T2210",BusT2210) //�ɷ�����
    ON_BC_FUNC("T2220",BusT2220) 
    ON_BC_FUNC("T2230",BusT2230)
    ON_BC_FUNC("T2240",BusT2240)
    ON_BC_FUNC("T2221",BusT2221)
    ON_BC_FUNC("T2231",BusT2231)
    ON_BC_FUNC("T2241",BusT2241)
		*/
		
    //�˵�ϵͳ����---------------
    ON_BC_FUNC("Z0000",BusZ0000) //ǩ��
    ON_BC_FUNC("Z0100",BusZ0100) //������ѯ
    ON_BC_FUNC("Z0200",BusZ0200) //���׽����ѯ
    ON_BC_FUNC("Z0400",BusZ00_ev1) //��������
    ON_BC_FUNC("Z2210",BusZ2210) //0850�ǰ��û�֧��
    ON_BC_FUNC("Z2211",BusZ2211) //0830�ǰ��û�֧��Ӧ��
    ON_BC_FUNC("Z2212",BusZ2212) //0880�ǰ��û�֧��Ӧ��֪ͨ
    ON_BC_FUNC("Z2220",BusZ2220) //���û�֧��
    ON_BC_FUNC("Z2221",BusZ2221) //���û�֧��Ӧ��
    
    //�������˵����ѽ���
//    ON_BC_FUNC("Z22G1",  BusGZ_Z22_ev1)     //0200�������ǰ��û�֧��
    ON_BC_FUNC("Z22G4",  BusGZ_Z22_ev4)     //0210�������ǰ��û�֧��Ӧ��
    ON_BC_FUNC("Z22G4_R",BusGZ_Z22_ev4_R) 	//�������ǰ��û�֧������Ӧ��
    ON_BC_FUNC("Z22G5",  BusZ2212)          //�������ǰ��û�֧��--����Ӧ��
    
    //�㸶�����˵����ѽ���
//    ON_BC_FUNC("Z22H1",   BusHF_Z22)         //�㸶�ǰ��û�֧��
    ON_BC_FUNC("Z22H2",   BusHF_Z22_RESP1)   //�㸶�ǰ��û�֧��֪ͨ
    ON_BC_FUNC("Z22H3",   BusHF_Z22_RESP2)   //�㸶�ǰ��û�֧��Ӧ��
    ON_BC_FUNC("Z22H5",   BusZ2212)          //�㸶�ǰ��û�֧��--����Ӧ��
    
    //POSPǰ�ý���
    ON_BC_FUNC("Z22P1", POSP_ExpendApp )         					//POSPǰ����������
    ON_BC_FUNC("Z22P2", POSP_ExpendUndo )        					//POSPǰ�����ѳ���
    ON_BC_FUNC("Z22P3", POSP_BalanceQuery )      					//POSPǰ������ѯ
    ON_BC_FUNC("Z22P4", POSP_ExpendRollBack )    					//POSPǰ�����ѳ���
    ON_BC_FUNC("Z22P5", POSP_ExpendUndoRollBack )					//POSPǰ�����ѳ�������
    ON_BC_FUNC("SIGN_POS1", POSP_KeysResetApp )						//POSPǰ����Կ��������
    ON_BC_FUNC("SIGN_POS2", POSP_KeysResetReply )					//POSPǰ����Կ����Ӧ��
    
    //CUPS���н���
    ON_BC_FUNC("Z22P1_R", CUPS_ExpendAppReply  )       			//CUPS��������Ӧ��
    ON_BC_FUNC("Z22P2_R", CUPS_ExpendUndoReply  )       		//CUPS���ѳ���Ӧ��
    ON_BC_FUNC("Z22P3_R", CUPS_BalanceQueryReply  )     		//CUPS����ѯӦ��
    ON_BC_FUNC("Z22P1_R_R", CUPS_ExpendRollBackReply  )   		//CUPS���ѳ���Ӧ��
    ON_BC_FUNC("Z22P2_R_R", CUPS_ExpendUndoRoolBackReply  )		//CUPS���ѳ�������Ӧ��
    //����ǰ��
    ON_BC_FUNC("Z22YL2", BusHF_Z22_ev2  )       				//��������Ӧ��(ͬ��)
    ON_BC_FUNC("Z22YL3", BusHF_Z22_ev3  )       				//��������Ӧ��(�첽)   
    //SMS����
    ON_BC_FUNC("Z22S1_R", SMS_SendReply )						//���Ͷ���ϢӦ��
    //δ�ҵ��ý��׵Ĵ���ʽ
    ELSE
    {
    	//����
    	  BC_UnLockTrans(p_BcTransLog_o);
    	
    	//�����쳣��
      BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);
      BC_exp_log(EXP_CODE_CUS001, "�ҵ��Ĵ����������ڣ��������ô���", g_Rcv_Buffer,g_RcvBufferSize);  
        
        //û��ƥ�䵽
        dcs_log(0,0,"<FILE:%s,LINE:%d>û�����øý��׵Ĵ���ʽ��trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s,procedure:%s\n",__FILE__,__LINE__,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp,procedure);
        BC_SendExeception("02","���׵ĺ�������ʽ������[%s]���������ñ�ti_bc_trans_det.trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s]",procedure,trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp);

        
        //����
        return business_prepare_return(trans_chnl,msg_tp,iso,&g_tiBcTransInf,"N00040");
    }

    return 0;
}

int appbusiness(char *msgtype,char *process,char *SerCode, char *mchnt_tp, ISO_data *iso)
{
    return 0;
}

//���͸�����
int SendMessageToRcvMcht(ISO_data *iso, char *pRecvData, int rcvSize, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    int iRet;
    int iConnSock ;
    char buff[2048];
    int buffLen;

    buffLen = isotostr(buff, iso);
    if ( buffLen < 0 )
    {
        printf("ISO���ʧ��.\n");
        return -1;
    }

    //��������
    iConnSock = tcp_connet_server(p_tiBcInsInf->ip_addr, atoi(p_tiBcInsInf->port),0);
    if ( iConnSock < 0 )
    {
        printf("����ʧ��[ip:%s,port:%s].\n", p_tiBcInsInf->ip_addr, p_tiBcInsInf->port);
        return -1;
    }

    //������Ϣ
    iRet = write_msg_to_net(iConnSock, buff, buffLen, 30*1000);
    if ( iRet < 0 )
    {
        printf("����ǩ����Ϣʧ��.\n");
        tcp_close_socket(iConnSock);
        return iRet;
    }

    dcs_debug(buff, buffLen, "������־\n");

    //������Ϣ
    iRet = read_msg_from_net(iConnSock, pRecvData,rcvSize,5*1000);
    if ( iRet < 0 )
    {
        printf("����ǩ��Ӧ����Ϣʧ��[%d].\n", iRet);
        dcs_debug(0,0,"����ǩ��Ӧ����Ϣʧ��[%d].\n", iRet);
        tcp_close_socket(iConnSock);
        return iRet;         
    }

    tcp_close_socket(iConnSock);
    return iRet;
}

int sendSignIN(char *mchnt_cd, char *term_id, char *rcv_ins_id_cd)
{
}

