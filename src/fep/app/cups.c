#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include "bc.h"

int TradeMerchantCheckMac(char *resp_code, char *errInfo); //校验交易返回信息MAC
int CUPS_PinChange(char *resp_code, char *errInfo); //PINDATA转换
int UpdateOriginaTransLog(); 

//CUPS消费申请应答
int CUPS_ExpendAppReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_ins_inf_def tiBcInsInf;
    ti_bc_ins_inf_def *p_tiBcInsInf = &tiBcInsInf;
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
	char resp_code[10], errInfo[200];
	int sqlCode;
	strcpy(p_BcTransLog_o->flag_lock, "0");
    BC_SendMonitor_trans("1", "2",p_BcTransLog, p_tiBcTransInf);
	if(0 != TradeMerchantCheckMac(resp_code, errInfo))
	{
		return ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
	}
    //判断交易是否成功
    strcpy(resp_code, "N000");
    strcpy_safe(resp_code+4, p_BcTransLog->resp_cd, 2);
    strcpy(p_BcTransLog_o->is_over, "1");
    strcpy(p_BcTransLog_o->fld_28, resp_code );
    strcpy(p_BcTransLog_o->resp_cd, resp_code+4);
    strcpy(p_BcTransLog_o->flag_rcv, "2" );
    dcs_log(0,0,"<FILE:%s,LINE:%d>xnxn,auth_id_resp_cd[%s]", __FILE__, __LINE__, g_TransMsg.BcTransLog.auth_id_resp_cd);
    dcs_log(0,0,"<FILE:%s,LINE:%d>xnxn,auth_id_resp_cd_o[%s]", __FILE__, __LINE__, g_TransMsg_o.BcTransLog.auth_id_resp_cd);
    if ( 0 == memcmp(resp_code,"N00000",6) )
    {
        //交易成功
	    strcpy(p_BcTransLog_o->settle_dt, p_BcTransLog->settle_dt);
	    strcpy(p_BcTransLog_o->retri_ref_no, p_BcTransLog->retri_ref_no);
	    strcpy(p_BcTransLog_o->auth_id_resp_cd, p_BcTransLog->auth_id_resp_cd);
        strcpy(p_BcTransLog_o->bc_settle_in, "1");
    	strcpy(p_BcTransLog_o->trans_flag, "Z" );
    }
    else
    {
    	strcpy(p_BcTransLog_o->bc_settle_in, "0");
    	strcpy(p_BcTransLog_o->trans_flag, "Y" );
    }
    if ( 0 != BC_CheckIsoDataField(iso, "3,4,11,12,13,25,37,41,42,48,49") )
    {
    	sprintf(errInfo, "查广银联支付应答报文失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
    	strcpy(resp_code, "N00096");
		return ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
    }
    //获取受理机构信息
    memset(p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd ) ;
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(账单系统)的机构信息 !",__FILE__,__LINE__, tiBcInsInf.ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode)
    {
    	sprintf(errInfo, "查询机构[%s]的机构信息.流水号:[%s],终端号:[%s],商户号:[%s]", p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
    	strcpy(resp_code, "N00096");
		return ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
    }
    //更新数据库
    dcs_log(0,0,"<FILE:%s,LINE:%d>xnxn,auth_id_resp_cd[%s]", __FILE__, __LINE__, g_TransMsg.BcTransLog.auth_id_resp_cd);
    dcs_log(0,0,"<FILE:%s,LINE:%d>xnxn,auth_id_resp_cd_o[%s]", __FILE__, __LINE__, g_TransMsg_o.BcTransLog.auth_id_resp_cd);
    tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
    if ( 0 != sqlCode)
    {
    	sprintf(errInfo, "更新账单请求数据库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
    }
    //更新BILL
    BusZ_BillCopy(p_BcTransLog_o, p_tlBcBillLog);
    tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
    if ( 0 != sqlCode)
    {
    	sprintf(errInfo, "账单BILL入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 1);
    }
    POSP_RespondExpendApp(resp_code, 0, p_tiBcInsInf);
	return 0;
}

//CUPS消费撤消应答
int CUPS_ExpendUndoReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_ins_inf_def tiBcInsInf;
    ti_bc_ins_inf_def *p_tiBcInsInf = &tiBcInsInf;
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
	char resp_code[10], errInfo[200];
	int sqlCode;
    BC_SendMonitor_trans("1","2",p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
	if(0 != TradeMerchantCheckMac(resp_code, errInfo))
	{
		return ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
	}
    //判断交易是否成功
    strcpy(resp_code, "N000");
    strcpy_safe(resp_code+4, p_BcTransLog->resp_cd, 2);
    strcpy(p_BcTransLog_o->is_over, "1");
    strcpy(p_BcTransLog_o->fld_28, resp_code );
    strcpy(p_BcTransLog_o->resp_cd, resp_code+4);
    strcpy(p_BcTransLog_o->flag_rcv, "2" );
    if ( 0 == memcmp(resp_code,"N00000",6) )
    {
        //交易成功
	    strcpy(p_BcTransLog_o->settle_dt, p_BcTransLog->settle_dt);
	    strcpy(p_BcTransLog_o->retri_ref_no, p_BcTransLog->retri_ref_no);
	    strcpy(p_BcTransLog_o->auth_id_resp_cd, p_BcTransLog->auth_id_resp_cd);
        strcpy(p_BcTransLog_o->bc_settle_in, "1");
    	strcpy(p_BcTransLog_o->trans_flag, "Z" );
    	UpdateOriginaTransLog();
    }
    else
    {
    	strcpy(p_BcTransLog_o->bc_settle_in, "0");
    	strcpy(p_BcTransLog_o->trans_flag, "Y" );
    }
    if ( 0 != BC_CheckIsoDataField(iso, "3,4,11,12,13,25,37,41,42,49") )
    {
    	sprintf(errInfo, "查广银联支付应答报文失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
    	strcpy(resp_code, "N00096");
		return ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
    }
    //获取受理机构信息
    memset(p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd ) ;
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(账单系统)的机构信息 !",__FILE__,__LINE__, tiBcInsInf.ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode)
    {
    	sprintf(errInfo, "查询机构[%s]的机构信息.流水号:[%s],终端号:[%s],商户号:[%s]", p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
    	strcpy(resp_code, "N00096");
		return ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
    }
    //更新数据库
    tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
    if ( 0 != sqlCode)
    {
    	sprintf(errInfo, "更新账单请求数据库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
    }
    //更新BILL
    BusZ_BillCopy(p_BcTransLog_o, p_tlBcBillLog);
    tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
    if ( 0 != sqlCode)
    {
    	sprintf(errInfo, "账单BILL入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 1);
    }
    POSP_RespondExpendUndo(resp_code, 0, p_tiBcInsInf);
	return 0;
}

//CUPS余额查询应答
int CUPS_BalanceQueryReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_ins_inf_def tiBcInsInf;
    ti_bc_ins_inf_def *p_tiBcInsInf = &tiBcInsInf;
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
	char resp_code[10], errInfo[200];
	int sqlCode, bitLen;
    BC_SendMonitor_trans("1","2",p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
	if(0 != TradeMerchantCheckMac(resp_code, errInfo))
	{
		return ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
	}
    //判断交易是否成功
    strcpy(resp_code, "N000");
    strcpy_safe(resp_code+4, p_BcTransLog->resp_cd, 2);
    strcpy(p_BcTransLog_o->is_over, "1");
    strcpy(p_BcTransLog_o->fld_28, resp_code );
    strcpy(p_BcTransLog_o->resp_cd, resp_code+4);
    strcpy(p_BcTransLog_o->flag_rcv, "2" );
    if ( 0 == memcmp(resp_code,"N00000",6) )
    {
        //交易成功
	    strcpy(p_BcTransLog_o->settle_dt, p_BcTransLog->settle_dt);
	    strcpy(p_BcTransLog_o->retri_ref_no, p_BcTransLog->retri_ref_no);
	    strcpy(p_BcTransLog_o->auth_id_resp_cd, p_BcTransLog->auth_id_resp_cd);
        strcpy(p_BcTransLog_o->bc_settle_in, "0");
    	strcpy(p_BcTransLog_o->trans_flag, "Z" );
    }
    else
    {
    	strcpy(p_BcTransLog_o->bc_settle_in, "0");
    	strcpy(p_BcTransLog_o->trans_flag, "Y" );
    }
    if ( 0 != BC_CheckIsoDataField(iso, "3,11,12,13,25,41,42,49") )
    {
    	sprintf(errInfo, "查广银联支付应答报文失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
    	strcpy(resp_code, "N00096");
		return ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
    }
    //获取受理机构信息
    memset(p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd ) ;
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(账单系统)的机构信息 !",__FILE__,__LINE__, tiBcInsInf.ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode)
    {
    	sprintf(errInfo, "查询机构[%s]的机构信息.流水号:[%s],终端号:[%s],商户号:[%s]", p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
    	strcpy(resp_code, "N00096");
		return ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
    }
    //更新数据库
    tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
    if ( 0 != sqlCode)
    {
    	sprintf(errInfo, "更新账单请求数据库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
    }
    //更新BILL
    BusZ_BillCopy(p_BcTransLog_o, p_tlBcBillLog);
    tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
    if ( 0 != sqlCode)
    {
    	sprintf(errInfo, "账单BILL入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 1);
    }
    POSP_RespondBalanceQuery(resp_code, 0, p_tiBcInsInf);
	return 0;
}

//CUPS消费冲正应答
int CUPS_ExpendRollBackReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
    int sqlCode;
    char errInfo[255];
    BC_SendMonitor_trans("1","2",p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
	if( 0 == memcmp("00", p_BcTransLog->resp_cd, 2)
		|| 0 == memcmp("25", p_BcTransLog->resp_cd, 2)
		|| 0 == memcmp("12", p_BcTransLog->resp_cd, 2))
	{
		strcpy( p_BcTransLog_o->flag_reversal , "2");
		strcpy( p_BcTransLog_o->bc_settle_in , "0");
		strcpy( p_BcTransLog_o->is_over , "1");
		strcpy(p_BcTransLog_o->flag_lock, "0");
		strcpy( p_BcTransLog_o->trans_flag, "W");
		memcpy( p_BcTransLog->resp_cd_reversal, p_BcTransLog->resp_cd, 2);
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if( 0 != sqlCode )
		{
	    	sprintf(errInfo, "处理成功冲正的应答时，记库失败! 流水号:[%s],终端号:[%s],商户号:[%s].", p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
	    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
      	}
      	return 0;
	}
	if( --pTimeOutRec->iFlags <= 0 )
	{
		strcpy( p_BcTransLog_o->flag_reversal , "3");
		strcpy( p_BcTransLog_o->bc_settle_in , "0");
		strcpy( p_BcTransLog_o->is_over , "1");
		strcpy(p_BcTransLog_o->flag_lock, "0");
		strcpy( p_BcTransLog_o->trans_flag, "X");
		memcpy( p_BcTransLog->resp_cd_reversal, p_BcTransLog->resp_cd, 2);
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if( 0 != sqlCode )
		{
	    	sprintf(errInfo, "处理成功冲正的应答时，记库失败! 流水号:[%s],终端号:[%s],商户号:[%s].", p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
	    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
		}
    	sprintf(errInfo, "已经发送(%d)次冲正，均未返回成功，不再处理该笔交易! 流水号:[%s],终端号:[%s],商户号:[%s].", I_TIMEOUT_TIMES, p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
	    return -1;
	}
  	InsTimeOutTBL(pTimeOutRec);
    BC_UnLockTrans(p_BcTransLog_o);
	return 1;
}

//CUPS消费撤消冲正应答
int CUPS_ExpendUndoRoolBackReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
    int sqlCode;
    char errInfo[255];
    BC_SendMonitor_trans("1","2",p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
	if( 0 == memcmp("00", p_BcTransLog->resp_cd, 2)
		|| 0 == memcmp("25", p_BcTransLog->resp_cd, 2)
		|| 0 == memcmp("12", p_BcTransLog->resp_cd, 2))
	{
		strcpy( p_BcTransLog_o->flag_reversal , "2");
		strcpy( p_BcTransLog_o->bc_settle_in , "0");
		strcpy( p_BcTransLog_o->is_over , "1");
		strcpy(p_BcTransLog_o->flag_lock, "0");
		strcpy( p_BcTransLog_o->trans_flag, "W");
		memcpy( p_BcTransLog->resp_cd_reversal, p_BcTransLog->resp_cd, 2);
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if( 0 != sqlCode )
		{
	    	sprintf(errInfo, "处理成功冲正的应答时，记库失败! 流水号:[%s],终端号:[%s],商户号:[%s].", p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
	    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
      	}
      	return 0;
	}
	if( --pTimeOutRec->iFlags <= 0 )
	{
		strcpy( p_BcTransLog_o->flag_reversal , "3");
		strcpy( p_BcTransLog_o->bc_settle_in , "0");
		strcpy( p_BcTransLog_o->is_over , "1");
		strcpy(p_BcTransLog_o->flag_lock, "0");
		strcpy( p_BcTransLog_o->trans_flag, "Z");
		memcpy( p_BcTransLog->resp_cd_reversal, p_BcTransLog->resp_cd, 2);
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if( 0 != sqlCode )
		{
	    	sprintf(errInfo, "处理成功冲正的应答时，记库失败! 流水号:[%s],终端号:[%s],商户号:[%s].", p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
	    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
		}
    	sprintf(errInfo, "已经发送(%d)次冲正，均未返回成功，不再处理该笔交易! 流水号:[%s],终端号:[%s],商户号:[%s].", I_TIMEOUT_TIMES, p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
	    return -1;
	}
  	InsTimeOutTBL(pTimeOutRec);
    BC_UnLockTrans(p_BcTransLog_o);
    return 1;
}

//CUPS消费申请
int CUPS_ExpendApp(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
  	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf = &g_tiBcQsMchntInf; 
	ISO_data iso, *p_iso;	
	char tmpBuf[255];
	p_iso = &iso;
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {    	
		sprintf(errInfo, "设置ISO8583包类型[%s]失败!", p_tiBcInsInf->packet_type);
		strcpy(resp_code, "N00096");
		return 1;
    }
	memset(p_iso, 0, sizeof(ISO_data));
    set_msghead(p_iso, "\x60\x00\x05\x00\x00\x60\x21\x00\x00\x00\x00", 11); 
    setbit_t(p_iso, 0 , "0200", 4); 
    rtrim(p_BcTransLog->pri_acct_no);
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "190000", 6);
    sprintf(tmpBuf, "%.12d", atoi(p_BcTransLog->trans_at) );
    setbit_t(p_iso, 4 , tmpBuf, 12);
    setbit_t(p_iso, 11, p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
    setbit_t(p_iso, 22, "021", 3);  //磁条输入
    setbit_t(p_iso, 25, "00", 2);   //正常输入
    setbit_t(p_iso, 26, "06", 2);   //六位密码
    setbit_t(p_iso, 35, p_BcTransLog->track_2_data, 37);
    setbit_t(p_iso, 36, p_BcTransLog->track_3_data, 104);
    setbit_t(p_iso, 41, p_BcTransLog_o->center_term_id, 8);
    setbit_t(p_iso, 42, p_BcTransLog_o->center_mchnt_cd, 15);
    memset(tmpBuf, ' ', 30);
    memcpy(tmpBuf, "PAGA", 4);
    memcpy(tmpBuf+4, p_BcTransLog->center_mchnt_cd + 7, 4);
    memcpy(tmpBuf+24, "000000", 6);
    tmpBuf[30] = '#';
    tmpBuf[31] = 0;
    setbit_t(p_iso, 48, tmpBuf, 31);
    setbit_t(p_iso, 49, p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,3));
	if( 0 != CUPS_PinChange(resp_code, errInfo)) return 2;	
    setbit_t(p_iso, 52, p_BcTransLog->pin_data, 8);
    setbit_t(p_iso, 53, "1000000000000000", 16); 
    setbit_t(p_iso, 64, "00000000", 8); 
	BC_SetMacKey(p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_1);
	BC_SendMonitor_iso("0","0", p_tiBcInsInf->line_nm, p_iso, &g_tiBcTransInf);
	if(0 != BC_InsertTimeOut1(0, gs_myFid, g_iSrcFoldId,I_TIMEOUT_TIMES, p_iso, &g_TransMsg_o))
	{
		sprintf(errInfo, "插超时表失败!");
		strcpy(resp_code, "N00096");
    	return 1;
	}
	setbitoff(p_iso, 13);
	if(0 != BC_SendMessage(p_iso, p_tiBcInsInf->line_nm))
	{
		sprintf(errInfo, "发送结算渠道失败!");
		strcpy(resp_code, "N00091");
    	return 2;
	}
	return 0;
}

//CUPS消费撤消
int CUPS_ExpendUndo(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
  	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf = &g_tiBcQsMchntInf; 
	ISO_data iso, *p_iso;
	char tmpBuf[255];
	p_iso = &iso;
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {    	
		sprintf(errInfo, "设置ISO8583包类型[%s]失败!", p_tiBcInsInf->packet_type);
		strcpy(resp_code, "N00096");
    	return 1;
    }
	memset(p_iso, 0, sizeof(ISO_data));
    set_msghead(p_iso, "\x60\x00\x05\x00\x00\x60\x21\x00\x00\x00\x00", 11); 
    setbit_t(p_iso, 0 , "0200", 4); 
    rtrim(p_BcTransLog->pri_acct_no);
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "280000", 6);
    sprintf(tmpBuf, "%.12d", atoi(p_BcTransLog->trans_at) );
    setbit_t(p_iso, 4 , tmpBuf, 12);
    setbit_t(p_iso, 11, p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
    setbit_t(p_iso, 22, "012", 3);  //磁条输入
    setbit_t(p_iso, 25, "00", 2);   //正常输入
    setbit_t(p_iso, 37, p_BcTransLog->retri_ref_no, 12);
    dcs_log(0,0,"<FILE:%s,LINE:%d>xnxn,auth_id_resp_cd[%s]", __FILE__, __LINE__, p_BcTransLog->auth_id_resp_cd);
    dcs_log(0,0,"<FILE:%s,LINE:%d>xnxn,auth_id_resp_cd[%s]", __FILE__, __LINE__, p_BcTransLog_o->auth_id_resp_cd);
    setbit_t(p_iso, 38, p_BcTransLog->auth_id_resp_cd, 6);
    setbit_t(p_iso, 41, p_BcTransLog_o->center_term_id, 8);
    setbit_t(p_iso, 42, p_BcTransLog_o->center_mchnt_cd, 15);
    memset(tmpBuf, ' ', 30);
    memcpy(tmpBuf, "PAGA", 4);
    memcpy(tmpBuf+4, p_BcTransLog->center_mchnt_cd + 7, 4);
    memcpy(tmpBuf+24, "000000", 6);
    tmpBuf[30] = '#';
    tmpBuf[31] = 0;
    setbit_t(p_iso, 48, tmpBuf, 31);
    setbit_t(p_iso, 49, p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,3));
    memset(tmpBuf, '0', 29);
    memcpy(tmpBuf+6, p_BcTransLog->originaDataElements+4, 6);
    memcpy(tmpBuf+10, p_BcTransLog->originaDataElements+12, 4);
    setbit_t(p_iso, 61, tmpBuf, 29);
    setbit_t(p_iso, 64, "00000000", 8); 
	BC_SetMacKey(p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_1);
	BC_SendMonitor_iso("0","0", p_tiBcInsInf->line_nm, p_iso, &g_tiBcTransInf);
	if(0 != BC_InsertTimeOut1(0, gs_myFid, g_iSrcFoldId,I_TIMEOUT_TIMES,p_iso, &g_TransMsg_o))
	{
		sprintf(errInfo, "插超时表失败!", p_tiBcInsInf->packet_type);
		strcpy(resp_code, "N00096");
    	return 1;
	}
	setbitoff(p_iso, 13);
	if(0 != BC_SendMessage(p_iso, p_tiBcInsInf->line_nm))
	{
		sprintf(errInfo, "发送结算渠道失败!");
		strcpy(resp_code, "N00091");
    	return 2;
	}
	return 0;
}

//CUPS余额查询
int CUPS_BalanceQuery(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_ins_inf_def *p_fwd_tiBcInsInf = &g_tiBcInsInf ; //机构信息
  	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf = &g_tiBcQsMchntInf; 
	ISO_data iso, *p_iso;
	char tmpBuf[255];
	p_iso = &iso;
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {    	
		sprintf(errInfo, "设置ISO8583包类型[%s]失败!", p_tiBcInsInf->packet_type);
		strcpy(resp_code, "N00096");
    	return 1;
    }
	memset(p_iso, 0, sizeof(ISO_data));
    set_msghead(p_iso, "\x60\x00\x05\x00\x00\x60\x21\x00\x00\x00\x00", 11); 
    setbit_t(p_iso, 0 , "0200", 4); 
    rtrim(p_BcTransLog->pri_acct_no);
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "300000", 6);
    setbit_t(p_iso, 11, p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
    setbit_t(p_iso, 22, "021", 3);  //磁条输入
    setbit_t(p_iso, 25, "00", 2);   //正常输入
    setbit_t(p_iso, 26, "06", 2);   //六位密码
    setbit_t(p_iso, 35, p_BcTransLog->track_2_data, 37);
    setbit_t(p_iso, 36, p_BcTransLog->track_3_data, 104);
    setbit_t(p_iso, 41, p_BcTransLog_o->center_term_id, 8);
    setbit_t(p_iso, 42, p_BcTransLog_o->center_mchnt_cd, 15);
    setbit_t(p_iso, 49, p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,3));
	if( 0 != CUPS_PinChange(resp_code, errInfo)) return 2;	
    setbit_t(p_iso, 52, p_BcTransLog->pin_data, 8);
    setbit_t(p_iso, 53, "1000000000000000", 16); 
    setbit_t(p_iso, 60, "01000000", 8);
    setbit_t(p_iso, 64, "00000000", 8); 
	BC_SetMacKey(p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_1);
	BC_SendMonitor_iso("0","0", p_tiBcInsInf->line_nm, p_iso, &g_tiBcTransInf);
	if(0 != BC_InsertTimeOut1(0, gs_myFid, g_iSrcFoldId,I_TIMEOUT_TIMES, p_iso, &g_TransMsg_o))
	{
		sprintf(errInfo, "插超时表失败!", p_tiBcInsInf->packet_type);
		strcpy(resp_code, "N00096");
    	return 1;
	}
	setbitoff(p_iso, 13);
	if(0 != BC_SendMessage(p_iso, p_tiBcInsInf->line_nm))
	{
		sprintf(errInfo, "发送结算渠道失败!");
		strcpy(resp_code, "N00091");
    	return 2;
	}
	return 0;
}

//CUPS消费冲正
int CUPS_ExpendRollBack(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
  	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf = &g_tiBcQsMchntInf; 
	ISO_data iso, *p_iso;
	char tmpBuf[255];
	p_iso = &iso;
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {    	
		sprintf(errInfo, "设置ISO8583包类型[%s]失败!", p_tiBcInsInf->packet_type);
		strcpy(resp_code, "N00096");
    	return 1;
    }
	memset(p_iso, 0, sizeof(ISO_data));
    set_msghead(p_iso, "\x60\x00\x05\x00\x00\x60\x21\x00\x00\x00\x00", 11); 
    setbit_t(p_iso, 0 , "0400", 4); 
    rtrim(p_BcTransLog->pri_acct_no);
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "190000", 6);
    sprintf(tmpBuf, "%.12d", atoi(p_BcTransLog->trans_at) );
    setbit_t(p_iso, 4 , tmpBuf, 12);
    setbit_t(p_iso, 11, p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
    setbit_t(p_iso, 22, "021", 3);  //磁条输入
    setbit_t(p_iso, 25, "00", 2);   //正常输入
    setbit_t(p_iso, 39, "98", 2);   
    setbit_t(p_iso, 41, p_BcTransLog_o->center_term_id, 8);
    setbit_t(p_iso, 42, p_BcTransLog_o->center_mchnt_cd, 15);
    memset(tmpBuf, ' ', 30);
    memcpy(tmpBuf, "PAGA", 4);
    memcpy(tmpBuf+4, p_BcTransLog->center_mchnt_cd + 7, 4);
    memcpy(tmpBuf+24, "000000", 6);
    tmpBuf[30] = '#';
    tmpBuf[31] = 0;
    setbit_t(p_iso, 48, tmpBuf, 31);
    setbit_t(p_iso, 49, p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,3));
    memset(tmpBuf, '0', 29);
    memcpy(tmpBuf+6, p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(p_iso, 61, (unsigned char *)tmpBuf, 29);
    setbit_t(p_iso, 64, "00000000", 8); 
	BC_SetMacKey(p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_1);
	BC_SendMonitor_iso("0","0", p_tiBcInsInf->line_nm, p_iso, &g_tiBcTransInf);
	if(0 != BC_InsertTimeOut1(0, gs_myFid, g_iSrcFoldId,I_TIMEOUT_TIMES,p_iso, &g_TransMsg_o))
	{
		sprintf(errInfo, "插超时表失败!", p_tiBcInsInf->packet_type);
		strcpy(resp_code, "N00096");
    	return 1;
	}
	setbitoff(p_iso, 13);
	if(0 != BC_SendMessage(p_iso, p_tiBcInsInf->line_nm))
	{
		sprintf(errInfo, "发送结算渠道失败!");
		strcpy(resp_code, "N00091");
    	return 2;
	}
	return 0;
}

//CUPS消费撤消冲正
int CUPS_ExpendUndoRollBack(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
  	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf = &g_tiBcQsMchntInf; 
	ISO_data iso, *p_iso;
	char tmpBuf[255];
	p_iso = &iso;
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {    	
		sprintf(errInfo, "设置ISO8583包类型[%s]失败!", p_tiBcInsInf->packet_type);
		strcpy(resp_code, "N00096");
    	return 1;
    }
	memset(p_iso, 0, sizeof(ISO_data));
    set_msghead(p_iso, "\x60\x00\x05\x00\x00\x60\x21\x00\x00\x00\x00", 11); 
    setbit_t(p_iso, 0 , "0400", 4); 
    rtrim(p_BcTransLog->pri_acct_no);
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "280000", 6);
    sprintf(tmpBuf, "%.12d", atoi(p_BcTransLog->trans_at) );
    setbit_t(p_iso, 4 , tmpBuf, 12);
    setbit_t(p_iso, 11, p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
    setbit_t(p_iso, 22, "021", 3);  //磁条输入
    setbit_t(p_iso, 25, "00", 2);   //正常输入
    setbit_t(p_iso, 39, "98", 2);   
    setbit_t(p_iso, 41, p_BcTransLog_o->center_term_id, 8);
    setbit_t(p_iso, 42, p_BcTransLog_o->center_mchnt_cd, 15);
    memset(tmpBuf, ' ', 30);
    memcpy(tmpBuf, "PAGA", 4);
    memcpy(tmpBuf+4, p_BcTransLog->center_mchnt_cd + 7, 4);
    memcpy(tmpBuf+24, "000000", 6);
    tmpBuf[30] = '#';
    tmpBuf[31] = 0;
    setbit_t(p_iso, 48, tmpBuf, 31);
    setbit_t(p_iso, 49, p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,3));
    memset(tmpBuf, '0', 29);
    memcpy(tmpBuf+6, p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(p_iso, 61, (unsigned char *)tmpBuf, 29);
    setbit_t(p_iso, 64, "00000000", 8); 
	BC_SetMacKey(p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_1);
	BC_SendMonitor_iso("0","0", p_tiBcInsInf->line_nm, p_iso, &g_tiBcTransInf);
	if(0 != BC_InsertTimeOut1(0, gs_myFid, g_iSrcFoldId,I_TIMEOUT_TIMES,p_iso, &g_TransMsg_o))
	{
		sprintf(errInfo, "插超时表失败!", p_tiBcInsInf->packet_type);
		strcpy(resp_code, "N00096");
    	return 1;
	}
	setbitoff(p_iso, 13);
	if(0 != BC_SendMessage(p_iso, p_tiBcInsInf->line_nm))
	{
		sprintf(errInfo, "发送结算渠道失败!");
		strcpy(resp_code, "N00091");
    	return 2;
	}
	return 0;
}

int TradeMerchantCheckMac(char *resp_code, char *errInfo)
{
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    int sqlCode;
	//查询交易商户信息
	strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->center_term_id);
	strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
	tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
	if ( 0 != sqlCode )
	{
		sprintf(errInfo, "获取清算商户信息失败,中心商户号:[%s].中心终端号[%s],rcv_ins_id_cd[%s].", p_BcTransLog->center_mchnt_cd, p_BcTransLog->center_term_id, p_BcTransLog->rcv_ins_id_cd);
		strcpy(resp_code, "N00096");
		return 1;
	}	
	if ( 0 != BC_TermCheckMac(p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_1, g_Rcv_Buffer + 4 + 11, g_RcvBufferSize - 4 - 11) )
    {
		sprintf(errInfo, "商户MAC校验未通过,中心商户号:[%s].中心终端号[%s],rcv_ins_id_cd[%s].", p_BcTransLog->center_mchnt_cd, p_BcTransLog->center_term_id, p_BcTransLog->rcv_ins_id_cd);
        strcpy(resp_code,"N000A0");
        return 2;
    }
    return 0;
}

int CUPS_ExpendAppTimeOut()
{
	ti_bc_ins_inf_def tiBcInsInf; //接收机构的机构信息
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
	int sqlCode;
	char errInfo[255], resp_code[7];
    memset( &tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy( tiBcInsInf.ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    strcpy(p_BcTransLog_o->flag_lock, "0");
	if ( 0 != sqlCode )
    {
	    // 查询机构信息失败
        sprintf(errInfo, "查询机构(ins_id_cd:%s)信息失败", tiBcInsInf.ins_id_cd); 
    	ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 1);
        return 1;
    }
	strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog_o->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog_o->center_term_id);
	strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
	tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
	if ( 0 != sqlCode )
	{
		sprintf(errInfo, "获取清算商户信息失败,中心商户号:[%s].中心终端号[%s],rcv_ins_id_cd[%s].", p_BcTransLog_o->center_mchnt_cd, p_BcTransLog_o->center_term_id, p_BcTransLog_o->rcv_ins_id_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 1);
		return 2;
	}	
	strcpy(p_BcTransLog_o->is_over, "1");
	strcpy(p_BcTransLog_o->resp_cd,"98");
	strcpy(p_BcTransLog_o->fld_28 ,"N00098");
	strcpy(p_BcTransLog_o->bc_settle_in, "0");
	strcpy(p_BcTransLog_o->flag_reversal, "1");        
	strcpy(p_BcTransLog_o->trans_flag, "K");
	strcpy(p_BcTransLog_o->flag_4, "1");
	tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
	if(0 != sqlCode)
	{
		sprintf(errInfo, "更新交易流水库失败,中心商户号:[%s].中心终端号[%s],rcv_ins_id_cd[%s].", p_BcTransLog_o->center_mchnt_cd, p_BcTransLog_o->center_term_id, p_BcTransLog_o->rcv_ins_id_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
	}	
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd );
	tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
	if ( 0 != sqlCode )
    {
	    // 查询机构信息失败
        sprintf(errInfo, "查询机构(ins_id_cd:%s)信息失败", p_tiBcInsInf->ins_id_cd); 
		return ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 0);
    }
	if(0 != CUPS_ExpendRollBack(resp_code, errInfo, &tiBcInsInf))
	{
    	ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
	}
    return POSP_RespondExpendApp("N00098", 0, p_tiBcInsInf);
}

int CUPS_ExpendUndoTimeOut()
{
	ti_bc_ins_inf_def tiBcInsInf; //接收机构的机构信息
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
	int sqlCode;
	char errInfo[255], resp_code[7];
    memset( &tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy( tiBcInsInf.ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    strcpy(p_BcTransLog_o->flag_lock, "0");
	if ( 0 != sqlCode )
    {
	    // 查询机构信息失败
        sprintf(errInfo, "查询机构(ins_id_cd:%s)信息失败", tiBcInsInf.ins_id_cd); 
    	ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 1);
        return 1;
    }
	strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog_o->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog_o->center_term_id);
	strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
	tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
	if ( 0 != sqlCode )
	{
		sprintf(errInfo, "获取清算商户信息失败,中心商户号:[%s].中心终端号[%s],rcv_ins_id_cd[%s].", p_BcTransLog_o->center_mchnt_cd, p_BcTransLog_o->center_term_id, p_BcTransLog_o->rcv_ins_id_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 1);
		return 2;
	}	
	strcpy(p_BcTransLog_o->is_over, "1");
	strcpy(p_BcTransLog_o->resp_cd,"98");
	strcpy(p_BcTransLog_o->fld_28 ,"N00098");
	strcpy(p_BcTransLog_o->bc_settle_in, "0");
	strcpy(p_BcTransLog_o->flag_reversal, "1");        
	strcpy(p_BcTransLog_o->flag_4, "1");
	strcpy(p_BcTransLog_o->trans_flag, "K");
	tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
	if(0 != sqlCode)
	{
		sprintf(errInfo, "更新交易流水库失败,中心商户号:[%s].中心终端号[%s],rcv_ins_id_cd[%s].", p_BcTransLog_o->center_mchnt_cd, p_BcTransLog_o->center_term_id, p_BcTransLog_o->rcv_ins_id_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
	}	
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd );
	tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
	if ( 0 != sqlCode )
    {
	    // 查询机构信息失败
        sprintf(errInfo, "查询机构(ins_id_cd:%s)信息失败", p_tiBcInsInf->ins_id_cd); 
		return ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 0);
    }
	if(0 != CUPS_ExpendUndoRollBack(resp_code, errInfo, &tiBcInsInf))
	{
    	ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
	}
    POSP_RespondExpendUndo("N00098", 0, p_tiBcInsInf);
}

int CUPS_BalanceQueryTimeOut()
{
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
	int sqlCode;
	char errInfo[255], resp_code[7];
    strcpy(p_BcTransLog_o->flag_lock, "0");
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd );
	tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
	if ( 0 != sqlCode )
    {
	    // 查询机构信息失败
        sprintf(errInfo, "查询机构(ins_id_cd:%s)信息失败", p_tiBcInsInf->ins_id_cd); 
		return ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 0);
    }
	strcpy(p_BcTransLog_o->is_over, "1");
	strcpy(p_BcTransLog_o->resp_cd,"68");
	strcpy(p_BcTransLog_o->fld_28 ,"N00068");
	strcpy(p_BcTransLog_o->bc_settle_in, "0");
	strcpy(p_BcTransLog_o->trans_flag, "X");
	tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
	if(0 != sqlCode)
	{
		sprintf(errInfo, "更新交易流水库失败,中心商户号:[%s].中心终端号[%s],rcv_ins_id_cd[%s].", p_BcTransLog_o->center_mchnt_cd, p_BcTransLog_o->center_term_id, p_BcTransLog_o->rcv_ins_id_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
	}	
    POSP_RespondExpendUndo("N00068", 0, p_tiBcInsInf);
}

int CUPS_ExpendAppRoolBackTimeOut()
{
	ti_bc_ins_inf_def tiBcInsInf; //接收机构的机构信息
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
    struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
	int sqlCode;
	char errInfo[255], resp_code[7];
    memset( &tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy( tiBcInsInf.ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    strcpy(p_BcTransLog_o->flag_lock, "0");
	if ( 0 != sqlCode )
    {
	    // 查询机构信息失败
        sprintf(errInfo, "查询机构(ins_id_cd:%s)信息失败", tiBcInsInf.ins_id_cd); 
    	ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 1);
        return 1;
    }
	strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog_o->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog_o->center_term_id);
	strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
	tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
	if ( 0 != sqlCode )
	{
		sprintf(errInfo, "获取清算商户信息失败,中心商户号:[%s].中心终端号[%s],rcv_ins_id_cd[%s].", p_BcTransLog_o->center_mchnt_cd, p_BcTransLog_o->center_term_id, p_BcTransLog_o->rcv_ins_id_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 1);
		return 2;
	}	
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd );
	tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
	if ( 0 != sqlCode )
    {
	    // 查询机构信息失败
        sprintf(errInfo, "查询机构(ins_id_cd:%s)信息失败", p_tiBcInsInf->ins_id_cd); 
		return ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 0);
    }
	if(0 != CUPS_ExpendRollBack(resp_code, errInfo, &tiBcInsInf))
	{
    	ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
	}
	if( --pTimeOutRec->iFlags <= 0 )
	{
		strcpy( p_BcTransLog_o->flag_reversal , "3");
		strcpy( p_BcTransLog_o->bc_settle_in , "0");
		strcpy( p_BcTransLog_o->is_over , "1");
		strcpy(p_BcTransLog_o->flag_lock, "0");
		strcpy(p_BcTransLog_o->trans_flag, "X");
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if( 0 != sqlCode )
		{
	    	sprintf(errInfo, "处理成功冲正的应答时，记库失败! 流水号:[%s],终端号:[%s],商户号:[%s].", p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
	    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
		}
    	sprintf(errInfo, "已经发送(%d)次冲正，均未返回成功，不再处理该笔交易! 流水号:[%s],终端号:[%s],商户号:[%s].", I_TIMEOUT_TIMES, p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
	    return -1;
	}
  	InsTimeOutTBL(pTimeOutRec);
    BC_UnLockTrans(p_BcTransLog_o);
}

int CUPS_ExpendUndoRollBackTimeOut()
{
	ti_bc_ins_inf_def tiBcInsInf; //接收机构的机构信息
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
    struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
	int sqlCode;
	char errInfo[255], resp_code[7];
    memset( &tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy( tiBcInsInf.ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    strcpy(p_BcTransLog_o->flag_lock, "0");
	if ( 0 != sqlCode )
    {
	    // 查询机构信息失败
        sprintf(errInfo, "查询机构(ins_id_cd:%s)信息失败", tiBcInsInf.ins_id_cd); 
    	ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 1);
        return 1;
    }
	strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog_o->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog_o->center_term_id);
	strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
	tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
	if ( 0 != sqlCode )
	{
		sprintf(errInfo, "获取清算商户信息失败,中心商户号:[%s].中心终端号[%s],rcv_ins_id_cd[%s].", p_BcTransLog_o->center_mchnt_cd, p_BcTransLog_o->center_term_id, p_BcTransLog_o->rcv_ins_id_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 1);
		return 2;
	}	
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd );
	tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
	if ( 0 != sqlCode )
    {
	    // 查询机构信息失败
        sprintf(errInfo, "查询机构(ins_id_cd:%s)信息失败", p_tiBcInsInf->ins_id_cd); 
		return ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 0);
    }
	if(0 != CUPS_ExpendUndoRollBack(resp_code, errInfo, &tiBcInsInf))
	{
    	ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
	}
	if( --pTimeOutRec->iFlags <= 0 )
	{
		strcpy( p_BcTransLog_o->flag_reversal , "3");
		strcpy( p_BcTransLog_o->bc_settle_in , "0");
		strcpy( p_BcTransLog_o->is_over , "1");
		strcpy(p_BcTransLog_o->flag_lock, "0");
		strcpy(p_BcTransLog_o->trans_flag, "X");
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if( 0 != sqlCode )
		{
	    	sprintf(errInfo, "处理成功冲正的应答时，记库失败! 流水号:[%s],终端号:[%s],商户号:[%s].", p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
	    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
		}
    	sprintf(errInfo, "已经发送(%d)次冲正，均未返回成功，不再处理该笔交易! 流水号:[%s],终端号:[%s],商户号:[%s].", I_TIMEOUT_TIMES, p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
	    return -1;
	}
  	InsTimeOutTBL(pTimeOutRec);
    BC_UnLockTrans(p_BcTransLog_o);
}

int CUPS_PinChange(char *resp_code, char *errInfo)
{
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
  	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf = &g_tiBcQsMchntInf; 
	char return_code[3], sek_index1[6], sek_index2[6], in_pan_data[17], in_pin_data[17], out_pin_data[17], out_pik[33];
	int acct_len;
    memset(sek_index1, 0, sizeof(sek_index1));
    memset(sek_index2, 0, sizeof(sek_index2));
    memset(in_pan_data, 0, sizeof(in_pan_data));
    memset(in_pin_data, 0, sizeof(in_pin_data));
    memset(out_pin_data, 0, sizeof(out_pin_data));
    memset(out_pik, 0, sizeof(out_pik));
    memset(in_pan_data, '0', 4);
    acct_len = strlen(p_BcTransLog->pri_acct_no);
    memcpy(in_pan_data+4, p_BcTransLog->pri_acct_no + acct_len - 13, 12);
    strcpy(sek_index1, "S0");
    strcpy(sek_index2, "S0");
    memcpy(sek_index1+2, p_tiBcInsInf->key_index, 3);
    memcpy(sek_index2+2, p_tiBcQsMchntInf->mkey_index, 3);
    bcd_to_asc(in_pin_data, p_BcTransLog->pin_data, 16, 0);
   	memcpy(out_pik, p_tiBcQsMchntInf->mkey_2, 16);
   	memcpy(out_pik+16, p_tiBcQsMchntInf->mkey_2, 16);
 	if(0 > DESTRANSPIN2(return_code, sek_index1, sek_index2, p_tiBcInsInf->key_2, out_pik, in_pin_data, in_pan_data, out_pin_data, "0000000000000000"))
 	{
 		sprintf(errInfo, "PIN转换失败，retrun_code[%s]", return_code);
 		strcpy(resp_code, "N000A7");
 		return 1;
 	}
 	asc_to_bcd(p_BcTransLog->pin_data, out_pin_data, 16, 0);
 	return 0;
}

int UpdateOriginaTransLog()
{
	tl_bc_trans_log_def BcTransLog;
  	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
  	tl_bc_trans_log_def *p_BcTransLog = &BcTransLog;
	int sqlCode;
	char errInfo[255];
	strcpy(p_BcTransLog->mchnt_cd, p_BcTransLog_o->mchnt_cd);      
	strcpy(p_BcTransLog->term_id, p_BcTransLog_o->term_id);       
	memcpy(p_BcTransLog->sys_tra_no, p_BcTransLog_o->originaDataElements + 4, 6);
	p_BcTransLog->sys_tra_no[6] = 0;    
	memcpy(p_BcTransLog->loc_trans_dt, p_BcTransLog_o->originaDataElements + 10, 4); 
	p_BcTransLog->loc_trans_dt[4] = 0;
	tlBcTransLogOpr(GLB_DB_SELECT11, p_BcTransLog, &sqlCode);
	if ( 0 != sqlCode )
	{
  		sprintf(errInfo, "找不到原交易(mchnt_cd:[%s], term_id:[%s], originaDataElements[%s])", p_BcTransLog->mchnt_cd, p_BcTransLog->term_id, p_BcTransLog_o->originaDataElements);
    	ProcessErrInfo("N00025", errInfo, __FILE__, __LINE__, 0);
  		return 1;
	}
	strcpy(p_BcTransLog->flag_lock, "0");
	strcpy(p_BcTransLog->flag_2,"1");
    //更新数据库
    tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode)
    {
  		sprintf(errInfo, "更新账单请求数据库记录失败(mchnt_cd:[%s], term_id:[%s], originaDataElements[%s])", p_BcTransLog->mchnt_cd, p_BcTransLog->term_id, p_BcTransLog_o->originaDataElements);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
    	return 1;
    }
    return 0;
}
