#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"
#include  "CMDI.h"

/*超时的处理*/
int process_cmdi_timeout( char* srcBuf,int  iFid, int iLen )
{
    int iRet;
    int sqlCode;
    char msg_tp[4+1];
    char proc_cd[6+1];
    char procedure[30];
    char msg_tp_new[4+1];
    char buff[256];

    struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;        
    TimeOut_Key *tk = (TimeOut_Key *)pTimeOutRec->caKey;
    TimeOut_ReMark *trm = (TimeOut_ReMark *)pTimeOutRec->caRemark;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;

    memcpy(&g_TimeOutRec, srcBuf, sizeof(g_TimeOutRec));    

    //key
    memset(&g_TransMsg_o, 0, sizeof(BcTransMsg));
    memcpy(p_BcTransLog_o->sys_tra_no,  trm->sys_tra_no, 6);
    memcpy(p_BcTransLog_o->loc_trans_dt,  trm->loc_trans_dt, 4);
    memcpy(p_BcTransLog_o->term_id,  trm->term_id, 8);
    memcpy(p_BcTransLog_o->mchnt_cd,  trm->mchnt_cd, 15);

    dcs_log(0,0,"<FILE:%s,LINE:%d><process_cmdi_timeout>start to call tlBcTransLogOpr 搜索原始交易信息：sys_tra_no:%s,loc_trans_dt:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->loc_trans_dt,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
    tlBcTransLogOpr(GLB_DB_SELECT11, p_BcTransLog_o, &sqlCode);
    if ( 0 != sqlCode )
    {
        BC_SendExeception("02","查询交易日志信息失败，未找到应答交易的请求交易信息.sys_tra_no:%s,loc_trans_dt:%s,term_id:%s,mchnt_cd:%s\n",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->loc_trans_dt,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);

        //未查询到数据库记录，或记录已锁定（正在被处理）。
        dcs_log(p_BcTransLog_o,sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d><process_cmdi_timeout>在数据库中未找到该交易：sys_tra_no:%s,loc_trans_dt:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->loc_trans_dt,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
        return -1;
    }

    //记录被锁定
    *g_TransLock = '1';

    //copy trans
    memcpy(&g_TransMsg, &g_TransMsg_o, sizeof(BcTransMsg));
  
    //得到本应该得到的应答交易的消息类型
    memset(msg_tp_new, 0, sizeof(msg_tp_new) );
    memset(buff, 0, sizeof(buff));
    memcpy(buff, trm->msg_tp, 4);
    BC_StringAdder(buff, 10, msg_tp_new, 4);
    
     // 搜索原始交易处理函数
	   iRet = BC_Search_Trans_det(&g_tiBcTransDet,
	     "TIMEOUT"   ,    
	     p_BcTransLog_o->msg_tp       ,        
	     p_BcTransLog_o->proc_cd      ,       
	     p_BcTransLog_o->pos_cond_cd  ,
	     p_BcTransLog_o->mchnt_tp     , 
	     msg_tp_new                   , //new msg_tp
	     p_BcTransLog_o->trans_flag   ,
	     p_BcTransLog_o->rcv_ins_id_cd
	  );
	      
	  if( 0 != iRet )
	  {
	    BC_SendExeception("02","<FILE:%s,LINE:%d>未找到该交易的后续处理方式，请检查配置表ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    dcs_log(0,0,"<FILE:%s,LINE:%d>未在表ti_bc_trans_det中找到该交易的处理方式（超时处理）：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_UnLockTrans(p_BcTransLog_o);
	    	    
	    //返回
	    return -1;
	  }
	  
	  dcs_debug(0,0,"msg_tp_new:%s.\n", msg_tp_new);
	  
	  memset(procedure, 0, sizeof(procedure) );
	  strcpy(procedure, g_tiBcTransDet.procedure); 
    rtrim(procedure);
    dcs_log(0,0,"<FILE:%s,LINE:%d>超时交易查找到处理方式[%s]trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s,msg_tp_new:%s, procedure:%s.\n",__FILE__,__LINE__,g_tiBcTransDet.procedure,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new, procedure);
    
    //此处为所有交易超时后的处理入口
    if ( 0 );
    ON_BC_TIMEOUT("Z2211"  , BusZ2210_T)       //非绑定用户支付
    //IVR
    ON_BC_TIMEOUT("Z22IVR_T", BusIVR_Z22_T) //非绑定用户支付冲正应答超时处理
    //广银联
    ON_BC_TIMEOUT("Z22G4"  , BusGZ_Z22_ev4_T)  //非绑定用户支付广银联应答超时处理
    ON_BC_TIMEOUT("Z22G4_R", BusGZ_Z22_ev4_RT) //非绑定用户支付冲正应答超时处理
//	ON_BC_TIMEOUT("Z22G_I_T",BusGZ_Z22_I_T)   
    //汇付
    ON_BC_TIMEOUT("Z22H4"  , BusHF_Z22_RT)  //非绑定用户支付汇付应答超时处理
//    ON_BC_TIMEOUT("Z22J_I_T", BusJV_Z22_I_T)
    //CUPS
    ON_BC_TIMEOUT("Z22P1_R_T"  , CUPS_ExpendAppTimeOut)  //POSP消费广银联应答超时处理
    ON_BC_TIMEOUT("Z22P2_R_T"  , CUPS_ExpendUndoTimeOut)  //POSP消费撤消广银联应答超时处理
    ON_BC_TIMEOUT("Z22P1_R_RT" , CUPS_ExpendAppRoolBackTimeOut)  //POSP消费冲正超时广银联应答超时处理
    ON_BC_TIMEOUT("Z22P2_R_RT" , CUPS_ExpendUndoRollBackTimeOut)  //POSP消费撤消冲正超时广银联应答超时处理
    ON_BC_TIMEOUT("Z22P3_R_T"  , CUPS_BalanceQueryTimeOut)  //POSP消费撤消冲正超时广银联应答超时处理
    //SMS
    ON_BC_TIMEOUT("Z22S1_R_T"  , SMS_SendTimeOut)  //发送短消息应答超时处理
    //银联总部
    ON_BC_TIMEOUT("Z22Y3_R_T1"  , BusYL_Z22_ev3_T1)  //同步应答超时
    ON_BC_TIMEOUT("Z22Y3_R_T2"  , BusYL_Z22_ev3_T2)  //异步应答超时
    
    //未找到该交易的处理方式
    ELSE
    {
    	  //没有匹配到
        dcs_log(0,0,"<FILE:%s,LINE:%d><process_cmdi_timeout>该交易后续处理方式[%s]不存在，trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s,msg_tp_new:%s.\n",__FILE__,__LINE__,procedure,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
        BC_SendExeception("02","超时处理交易的后续处理方式[%s]不存在，请检查配置表。trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s].",g_tiBcTransDet.procedure,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp);
        BC_UnLockTrans(p_BcTransLog_o);
        
        //返回
        return -1;
    }
    return 0;
}

int SIGNIN001( ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf  )
{
	int iRet, sqlCode, iLen;
	char buff[1024];
	ISO_data iso ;
	ISO_data *p_iso = &iso ;
	char resp_code [6+1];
	
	ti_bc_ins_inf_def *p_tiBcInsInf= &g_tiBcInsInf;
	
	//search ins
	memset( p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcQsMchntInf->rcv_ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
    	return -1;
    }
    
  //packet
	memset(p_iso,0,sizeof(ISO_data));
  clearbit(p_iso);
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        return -1;
    }
    
    setbit_t(p_iso, 0 , "0520", 4); 
    setbit_t(p_iso, 3 , "510000", 6);
    setbit_t(p_iso, 42, (unsigned char *)p_tiBcQsMchntInf->mchnt_cd, strlen(p_tiBcQsMchntInf->mchnt_cd) );
    
    BC_SetMacKey(0,0);
    memset(buff, 0, sizeof(buff) );
    PrintISO( p_iso,"发送安捷商户签到请求报文",0);
    
    //发送监控信息
    BC_SendMonitor_iso("0","0",p_tiBcInsInf->line_nm, p_iso, 0);
    
    iLen = BC_SendMessageToAJ(p_iso, buff, p_tiBcInsInf);
    if ( iLen <= 0 )
    {
        //发送给安捷失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><SIGNIN001>send to rcv failure.",__FILE__,__LINE__);
        return -1;
    }

    //解包
    //to iso
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0!= BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        dcs_log(0, 0,"<FILE:%s,LINE:%d><SIGNIN001>BC_SetIsoMode failure.",__FILE__,__LINE__);
        return -1;
    }
   
    iRet = BC_UnPack_Data( p_iso, buff,  iLen);
    PrintISO( p_iso,"收到安捷商户签到应答报文",0 );

    //发送给安捷失败
    dcs_debug(0, 0,"<FILE:%s,LINE:%d><SIGNIN001>已经收到安捷商户签到回复报文.",__FILE__,__LINE__);
    if ( iRet >= 0 )
    {
        BC_SendMonitor_iso("1","2",p_tiBcInsInf->line_nm,p_iso, 0);
        
        //解包成功
        if ( (iRet = getbit(p_iso, 0, buff)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #0 failure。",__FILE__,__LINE__);
            return -1;
        }
        if ( (iRet = getbit(p_iso, 3, buff+4)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #3 failure。",__FILE__,__LINE__);
            return -1;
        }
        if ( (iRet = getbit(p_iso, 42, buff+20)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #42 failure。",__FILE__,__LINE__);
            return -1;
        }

        //check
        if ( 0 != memcmp(buff, "0530510000", 10) 
             || 0 != memcmp(buff+20, p_tiBcQsMchntInf->mchnt_cd,15)
           )
        {
            dcs_log(buff,50,"<FILE:%s,LINE:%d><SIGNIN001>check #0 #3 #42(%s) failure。",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd);
            return -1;
        }

        //return code
        if ( (iRet = getbit(p_iso, 28, buff)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #28 failure。",__FILE__,__LINE__);
            return -1;
        }

        memcpy(resp_code, buff, 6);
        resp_code[6] = 0;
        dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>return #28[%s]。",__FILE__,__LINE__, resp_code);
        if ( 0 != memcmp(resp_code, "N00000", 6) )
        {
        	return -1;
        }

        //mac
        if ( (iRet = getbit(p_iso, 48, buff)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #48 failure。",__FILE__,__LINE__);
            return -1;
        }

        //update key
        dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>start to call tiBcMchntInfOpr update key .",__FILE__,__LINE__);
        memcpy(p_tiBcQsMchntInf->mkey_1, buff, 16);
        *(p_tiBcQsMchntInf->mkey_1 + 16) = 0;
        iRet = tiBcQsMchntInfOpr(GLB_DB_UPDATE2, p_tiBcQsMchntInf, &sqlCode);
        if ( 0!= sqlCode )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>call tiBcMchntInfOpr update key failure。",__FILE__,__LINE__);
            //更新密钥失败
            return -1;
        }
        return 0;
    }
    
	return -1;
}

int SIGNIN002( ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf  )
{
	int iRet, sqlCode, iLen;
	ISO_data iso ;
	ISO_data *p_iso = &iso ;
	char resp_code [6+1];
	
	ti_bc_ins_inf_def *p_tiBcInsInf= &g_tiBcInsInf;
	
	//search ins
	memset( p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcQsMchntInf->rcv_ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
    	return -1;
    }
    
  //packet
	memset(p_iso,0,sizeof(ISO_data));
  clearbit(p_iso);
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        return -1;
    }
    
    setbit_t(p_iso, 0 , "0800", 4); 
    setbit_t(p_iso, 3 , "510000", 6);
    setbit_t(p_iso, 42, (unsigned char *)p_tiBcQsMchntInf->mchnt_cd, strlen(p_tiBcQsMchntInf->mchnt_cd) );
    
    BC_SetRcvMacKey(p_tiBcInsInf);
    
    PrintISO( p_iso,"发送商户签到请求报文",0);
    
    //发送监控信息
    BC_SendMonitor_iso("0","0",p_tiBcInsInf->line_nm, p_iso, 0);
    
    return BC_SendMessage(p_iso, p_tiBcInsInf->line_nm );
}

int mchntSignIn(char *param)
{
	int sqlCode;
	char procedure[16];
	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ;
	ti_bc_trans_det_def *p_tiBcTransDet = &g_tiBcTransDet;
	
	memset(p_tiBcQsMchntInf, 0, sizeof(ti_bc_qs_mchnt_inf_def));
	strcpy_safe(p_tiBcQsMchntInf->mchnt_cd, param, 20 );
	strcpy_safe(p_tiBcQsMchntInf->term_id, param+20, 8 );
	strcpy_safe(p_tiBcQsMchntInf->rcv_ins_id_cd, param+28, 8 );
	
	tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
  if ( 0 != sqlCode )
  {
  	//没有找到商户信息
  	dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210_T>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd,sqlCode );
    
    //交易失败
    return -1;
  }
  
    strcpy(p_tiBcTransDet->trans_chnl    , "*");
    strcpy(p_tiBcTransDet->msg_tp        , "0520");
    strcpy(p_tiBcTransDet->proc_cd       , "*");
    strcpy(p_tiBcTransDet->pos_cond_cd   , "*");
    strcpy(p_tiBcTransDet->mchnt_tp      , "*");
    strcpy(p_tiBcTransDet->msg_tp_new    , "*"); //new msgtp
    strcpy(p_tiBcTransDet->trans_flag    , "*" );
    strcpy(p_tiBcTransDet->rcv_ins_id_cd , p_tiBcQsMchntInf->rcv_ins_id_cd );
    
    //根据消息类型，处理码，服务点条件码，及交易标识码确定交易处理过程    
    tiBcTransDetOpr(GLB_DB_SELECT1, p_tiBcTransDet, &sqlCode);
    if ( 0 != sqlCode ){
    	dcs_debug(0,0,"<FILE:%s,LINE:%d>调用tiBcTransDetOpr查询处理函数失败(msg_tp:0520, rcv_ins_id_cd:%s).\n",__FILE__,__LINE__,p_tiBcTransDet->rcv_ins_id_cd);
    	return -1;
    }
    
    strcpy(procedure, p_tiBcTransDet->procedure); 
    rtrim(procedure);
    
    if( 0 == strcmp(procedure, "SIGNIN001") ){
    	//安捷签到
    	return SIGNIN001( p_tiBcQsMchntInf );
    }else if( 0 == strcmp(procedure, "SIGNIN002") ){
    	//广东银联
    	return SIGNIN002(p_tiBcQsMchntInf);
    } 
    
	return -1;	
}

