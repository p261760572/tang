#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

//
int BusIVR_SendIvr(tl_bc_trans_log_def *p_BcTransLog, char *mchnt_cn_abbr)
{
	return BusIVR_SendIvr_flag(p_BcTransLog, mchnt_cn_abbr, 1);
}

//发送获取关键域信息
int BusIVR_SendIvr_flag(tl_bc_trans_log_def *p_BcTransLog, char *mchnt_cn_abbr, int flag)
{
	int iRet;
	char info [1024];
	char buff[1024];
	IVR_STRUCT ivr ;
  
	TimeOut_Key *tk = (TimeOut_Key *)ivr.key;
	memset( &ivr, ' ', sizeof(IVR_STRUCT)-1 );
	memcpy( ivr.head, "CMDI0005" , 8);
	if(IsCreditCard(p_BcTransLog->card_attr))//信用卡
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>start to 向IVR发送查询CVN请求.", __FILE__, __LINE__); 
		memcpy( ivr.opcode, "OP02", 4);
	}
	else	
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>start to 向IVR发送查询密码请求.", __FILE__, __LINE__); 
		memcpy( ivr.opcode, "OP01", 4);
	}
	memcpy( ivr.flag, "0", 1);
	memcpy( ivr.fwd_ins_id_cd, g_center_ins_id_cd, strlen(g_center_ins_id_cd) );
	memcpy_safe(tk->sys_tra_no,  p_BcTransLog->sys_tra_no, sizeof(tk->sys_tra_no));
	memcpy_safe(tk->term_id,  p_BcTransLog->term_id, sizeof(tk->term_id) );
	memcpy_safe(tk->mchnt_cd,  p_BcTransLog->mchnt_cd, sizeof(tk->mchnt_cd));	
	memcpy_safe(&tk->end,  p_BcTransLog->loc_trans_dt, 4);	
	memset(ivr.data, 0, sizeof(ivr.data) );
	IVR_AddFieldValue(ivr.data, "TEL",  rtrim(p_BcTransLog->fld_5));
	IVR_AddFieldValue(ivr.data, "AMT", rtrim(p_BcTransLog->trans_at));
	//field 48
	sprintf(buff, "%s", rtrim(p_BcTransLog->fld_48) );
	iRet = GBKToUTF8(buff, strlen(buff), info, sizeof(info));
	if( 0 < iRet )
	{
		*(info + iRet) = 0;
		IVR_AddFieldValue(ivr.data, "INFO", info);
	}
	else
	{
		dcs_log(0, 0, "48域数据转UTF8转码失败！");
		return -1;
	}
	//商户简称
	rtrim(mchnt_cn_abbr);
	iRet = GBKToUTF8(mchnt_cn_abbr, strlen(mchnt_cn_abbr), info, sizeof(info));
	if( 0 < iRet )
	{
		*(info + iRet) = 0;
		IVR_AddFieldValue(ivr.data, "MERNAME", info);
	}
	else
	{
		dcs_log(0, 0, "商户简称数据转UTF8转码失败！");
		return -1;
	}	
	if(0 > IVR_SendIVR(gs_myFid, &ivr))
	{
		return  -1;
	}
	if(flag)
	{
	 	//插超时表
		if(0 != BC_InsertIVRTimeOut(3, gs_myFid, g_iSrcFoldId, 10, &g_TransMsg_o))
		{
			dcs_log(0, 0,"<FILE:%s,LINE:%d>BC_InsertIVRNotify .",__FILE__,__LINE__ );
		}
	}
	return 0;
}

//收到IVR数据信息
int BusIVR_Z22_ev1(ti_bc_trans_inf_def *p_tiBcTransInf, ti_bc_trans_det_def *p_tiBcTransDet, char *ivrdata)
{
	int iRet = 0;
	int sqlCode = 0 ;
	char resp_code[6+1]  ;
	char buff[1024] ;
	char temp[1024];
	int isExitFlag=0 ;
	int len;
	char cvn[17], vdate[17], password[128];
	ISO_data iso; 
	ISO_data *p_iso=&iso; 
	IVR_STRUCT *pIVR = (IVR_STRUCT *)ivrdata;
	
	//获取一些全局的变量, 待用
	ti_bc_mt_flow_id_def tiBcMTFlowId ;
	ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog;
	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
	ti_bc_ins_inf_def tiBcIvrInsInf; // IVR机构的机构信息
	
	//init 
	strcpy(p_BcTransLog->flag_lock, "0");
	if('1' == *p_BcTransLog->is_over)
	{
		if('1' == *p_BcTransLog->bc_settle_in)
		{
			dcs_debug(0, 0,"<FILE:%s,LINE:%d>收到ivr应答，但是交易已经成功，无需重新发起交易.\n",__FILE__,__LINE__);
			return ReturnIvrMsg("N00000", pIVR);
		}
		else
		{
			dcs_debug(0, 0,"<FILE:%s,LINE:%d>收到ivr应答，但是交易已经失败，无需重新发起交易.\n",__FILE__,__LINE__);
			return ReturnIvrMsg("N99999", pIVR);
		}
	}
	strcpy(p_BcTransLog->flag_verifi,  "2"  );
	memset(buff, 0, sizeof(buff) );    
	if( 0 >= IVR_Get_Field(buff, pIVR, "RSPCODE"))
	{
		sprintf(buff, "获取ivr应答报文应答码失败");
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00096");
	}
	*(buff + 6) = 0;
	strcpy_safe(p_BcTransLog->resp_cd_verifi,  buff+4, 2 );
	if( 0!= memcmp("N00000", buff, 6) )
	{
		if(memcmp(buff, "NOBACK", 6)) 
			ReturnIvrMsg("N99999", pIVR);
		memset(resp_code, 0, sizeof(resp_code));
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>ivr密码应答报文返回失败(应答码:%s)", __FILE__,__LINE__, buff);
		if(memcmp(buff, "N41209", 6)==0 || memcmp(buff, "N000A2", 6)==0)
			memcpy(resp_code, buff, 6);
		else
			memcpy(resp_code, "N00096", 6);
	    strcpy(p_BcTransLog->fld_28, resp_code);
	    strcpy(p_BcTransLog->resp_cd, resp_code+4);
    	strcpy(p_BcTransLog->resp_cd_rcv, buff);
        strcpy(p_BcTransLog->trans_flag, "Y" );
        strcpy(p_BcTransLog->bc_settle_in, "0");
    	strcpy(p_BcTransLog->is_over, "1");
		return SendPayResultToRecive(resp_code);
	}
	//查询IVR机构信息
	if( 0!= BC_Search_ins_inf(resp_code, &tiBcIvrInsInf, INS_IVR))
	{
		sprintf(buff, "查询IVR机构线路信息失败,机构id:[%s].", INS_IVR);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00096");
	}  
	if(0 == memcmp("OP03", pIVR->opcode, 4))
	{
		// password	 
		memset(password, 0, sizeof(password));   
		if( 0 >= IVR_Get_Field(password, pIVR, "PASSWORD"))
		{
			sprintf(buff, "获取ivr密码应答报文密码信息失败");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			ReturnIvrMsg("N99999", pIVR);
			return SendPayResultToRecive("N00096");
		}
	}
	if(0 == memcmp("OP04", pIVR->opcode, 4))
	{
		// VALIDDATA	 
		memset(buff, 0, sizeof(buff) );    
		if( 0 >= IVR_Get_Field(buff, pIVR, "VALIDDATA"))
		{
			sprintf(buff, "获取ivr关键域应答报文信息失败");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			ReturnIvrMsg("N99999", pIVR);
			return SendPayResultToRecive("N00096");
		}
		// CVN2	 
		if( 0 >= IVR_Get_Field(buff + 16, pIVR, "CVN2"))
		{
			sprintf(buff, "获取ivr关键域应答报文信息失败");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			ReturnIvrMsg("N99999", pIVR);
			return SendPayResultToRecive("N00096");
		}
		if(0>decryptInfo(cvn, vdate, buff, &tiBcIvrInsInf, 0, 1))
		{
			sprintf(buff, "解密信用卡关键信息失败");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			ReturnIvrMsg("N99999", pIVR);
			return SendPayResultToRecive("N00096");
		}
		EncryptInfo(cvn, vdate, p_BcTransLog->encrypt_info);
	}
	//查询接收机构信息
	if( 0!= BC_Search_ins_inf(resp_code, &tiBcInsInf, g_rtIns_id_cd))
	{
		sprintf(buff, "查询机构线路信息失败,机构id:[%s].", g_rtIns_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00096");
	}
	//获取转换流水号
	strcpy(tiBcMTFlowId.term_id   ,  p_BcTransLog->center_term_id);
	strcpy(tiBcMTFlowId.mchnt_cd  ,  p_BcTransLog->center_mchnt_cd);
	if(0 != tiBcMTFlowIdOpr(GLB_DB_SELECT1, &tiBcMTFlowId, &sqlCode))
	{
		sprintf(buff, "获取转换流水号失败,中心商户号:[%s].中心终端号[%s].", p_BcTransLog->center_mchnt_cd, p_BcTransLog->center_term_id);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00096");
	}
	strcpy(p_BcTransLog->center_sys_tra_no,  tiBcMTFlowId.sys_tra_no);
	//获取清算商户信息
	dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcQsMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__, p_BcTransLog->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->center_term_id);
	strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog->rcv_ins_id_cd);
	iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
	if ( 0 != sqlCode )
	{
		sprintf(buff, "获取清算商户信息失败,中心商户号:[%s].中心终端号[%s].", p_BcTransLog->center_mchnt_cd, p_BcTransLog->center_term_id);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00003", buff, 1);
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00003");
	}      
    strcpy(p_BcTransLog->flag_rcv,  "1"  );
    strcpy(p_BcTransLog->is_over ,  "0"  );
    strcpy(p_BcTransLog->flag_6, "1" );
	switch(p_BcTransLog->trans_flag[0])
	{
	  	case 'D':   
	  		strcpy(p_BcTransLog->trans_flag,"E"); //第二次提交支付
	  		break;
	  	case 'F':
	  		strcpy(p_BcTransLog->trans_flag,"G"); //第三次提交支付
	  		break;
	  	case 'B':
	  		strcpy(p_BcTransLog->trans_flag,"C"); //提交支付
	  		break;
	    default:
			dcs_debug(0, 0, "重复接收IVR信息[%s]", p_BcTransLog->trans_flag);
	    	return 0;
	}
	//保存到数据库
	iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
	if ( 0 != iRet )
	{
		sprintf(buff, "更新tlBcTransLog 失败");
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00003");
	} 
	else
		dcs_debug(0,0, "<FILE:%s,LINE:%d>tlBcTransLogOpr 更新库成功",__FILE__,__LINE__);
    if(0 == memcmp(p_BcTransLog->rcv_ins_id_cd, INS_GDYL, 6))
    {
    	iRet = ExpendApp_GZ(p_iso, &tiBcIvrInsInf, password);
    }
    else if(0 == memcmp(p_BcTransLog->rcv_ins_id_cd, INS_YL, 6))
    {
		if(0 == memcmp("OP03", pIVR->opcode, 4))
    		iRet = ExpendApp_YL_Debit(p_iso, p_BcTransLog, &tiBcIvrInsInf, password);
		if(0 == memcmp("OP04", pIVR->opcode, 4))
			iRet = ExpendApp_YL_Credit(p_iso, p_BcTransLog, 1);

    }
    else if(0 == memcmp(p_BcTransLog->rcv_ins_id_cd, INS_HFTX, 6) || 0 == memcmp(p_BcTransLog->rcv_ins_id_cd, INS_YBZF, 6))
    {
		iRet = SendPayMessageToJAVA(p_iso, &tiBcInsInf, p_BcTransLog, vdate, cvn, 1);
    }
    else
    {
		sprintf(buff, "接收机构出错![%s]", g_rtIns_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);	
		//发送给终端失败信息
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00091");    	
    }
    if(0 > iRet)
    {
		ReturnIvrMsg("N99999", pIVR);
	}
	return 0;
}

//回复IVR结果报文 
int BusIVR_Z22_RespIvr(char *resp_code, tl_bc_trans_log_def *p_BcTransLog)
{
	int iRet;
	char info [1024];
	char buff[1024];
	IVR_STRUCT ivr ;
	TimeOut_Key *tk = (TimeOut_Key *)ivr.key;
	
	memset( &ivr, ' ', sizeof(IVR_STRUCT)-1 );
	memcpy( ivr.head, "CMDI0005" , 8);
	if(IsCreditCard(p_BcTransLog->card_attr))//信用卡
		memcpy( ivr.opcode, "OP04", 4);
	else
		memcpy( ivr.opcode, "OP03", 4);
	memcpy( ivr.flag, "1", 1);
	memcpy( ivr.fwd_ins_id_cd, g_center_ins_id_cd, strlen(g_center_ins_id_cd) );
	memcpy_safe(tk->sys_tra_no,  p_BcTransLog->sys_tra_no, sizeof(tk->sys_tra_no));
	memcpy_safe(tk->term_id,  p_BcTransLog->term_id, sizeof(tk->term_id) );
	memcpy_safe(tk->mchnt_cd,  p_BcTransLog->mchnt_cd, sizeof(tk->mchnt_cd));	
	memcpy_safe(&tk->end,  p_BcTransLog->loc_trans_dt, 4);	
	memset(ivr.data, 0, sizeof(ivr.data) );
  	IVR_AddFieldValue(ivr.data, "RSPCODE", resp_code);
  	return IVR_SendIVR(gs_myFid, &ivr);
}

//IVR超时处理
int BusIVR_Z22_T()
{
	struct TimeOut_REC *p_TimeOutRec = &g_TimeOutRec;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; // from db
	ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
	int sqlCode, iRet;
	char buff[256];
	
	if(--p_TimeOutRec->iFlags>0)
	{
		//查询商户信息    
		strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog_o->mchnt_cd);
		tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
		if ( 0 != sqlCode ) //没有找到商户信息
		{
			sprintf(buff, "商户号不存在!(mchnt_cd:%s,sqlcode=%d)", p_tiBcMchntInf->mchnt_cd, sqlCode);
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			return SendPayResultToRecive("N00003"); 
	    }
		//发送给IVRS
		iRet =BusIVR_SendIvr_flag( p_BcTransLog_o, p_tiBcMchntInf->mchnt_cn_abbr, 0);
		if ( iRet < 0 )
		{
			sprintf(buff, "发送给IVR失败!");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			return SendPayResultToRecive("N00096"); 
		}
		//插超时表
		if(0 != BC_InsertIVRTimeOut(p_TimeOutRec->iFlags, gs_myFid, g_iSrcFoldId, 10, &g_TransMsg_o))
		{
			dcs_log(0, 0,"<FILE:%s,LINE:%d>BC_InsertIVRNotify failure.",__FILE__,__LINE__ );
		}
	}
	else
	{
		sprintf(buff, "发送给IVR信息3次未应答!");
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		return SendPayResultToRecive("N00096"); 
	}
	return 0;
}


int ReturnIvrMsg(char *respCode, IVR_STRUCT *pIVR)
{
	*pIVR->flag = '1';
	IVR_Set_Field(pIVR, "RSPCODE", respCode);
	return IVR_SendIVR(gs_myFid, pIVR);	
}

