#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

/*
char *CardNo
char *ValidDate
char *CVV2
char *IdType
char *IdNo
char *Name
*/

int GetRouteGateway(char *gateWay, char *rcv_ins_id_cd, char *iss_ins_id_cd)
{
	int sqlCode;
	ti_bc_route_gateway_inf_def tiBcRouteGatewayInf;
	if(memcmp(rcv_ins_id_cd, INS_YBZF, 8)==0)
	{
		strcpy(gateWay, "00");
		return 0;
	}
	memset(&tiBcRouteGatewayInf, 0, sizeof(tiBcRouteGatewayInf));
	strcpy_safe(tiBcRouteGatewayInf.rcv_ins_id_cd, rcv_ins_id_cd, sizeof(tiBcRouteGatewayInf.rcv_ins_id_cd) );
	strcpy_safe(tiBcRouteGatewayInf.iss_ins_id_cd, iss_ins_id_cd, sizeof(tiBcRouteGatewayInf.iss_ins_id_cd) );
	tiBcRouteGatewayInfOpr(GLB_DB_SELECT1, &tiBcRouteGatewayInf, &sqlCode);
	if(0 == sqlCode )
	{
		strcpy(gateWay, tiBcRouteGatewayInf.gate_id);
		return 0;
	}
	return -1;
}

//收到请求确认报文
int BusHF_Z22_RESP1(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso, char* flag)
{
	int iRet = -1   ;
	int sqlCode;
	char buff[256];
	char *msg;
	ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; // from db
	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;  // new resp
	ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
	ti_bc_err_inf_def tiBcErrInf;
	strcpy(p_BcTransLog_o->flag_lock, "0");
	BC_SendMonitor_trans("1","2",p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_rcv, "4");
	memset(&tiBcInsInf.ins_id_cd, 0, sizeof(tiBcInsInf.ins_id_cd) );
	strcpy(tiBcInsInf.ins_id_cd, p_BcTransLog->fwd_ins_id_cd ) ;
	dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(JAVA系统)的机构信息 !",__FILE__,__LINE__, tiBcInsInf.ins_id_cd);
	iRet = tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
	if ( 0 != sqlCode )
	{
		sprintf(buff, "查询机构线路信息失败,机构id:[%s].",tiBcInsInf.ins_id_cd); 
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		SendPayResultToRecive("N00096");
		BC_UnLockTrans(p_BcTransLog_o);
		return -1;
	}
	//check field
	if ( 0 != BC_CheckIsoDataField(iso, memcmp(p_BcTransLog_o->rcv_ins_id_cd, INS_YBZF, 8)==0 ? "3,4,11,12,13,28" : "3,4,11,12,13,39") )
	{
		sprintf(buff, "校验接收报文0830失败！"); 
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		SendPayResultToRecive("N00096");
		BC_UnLockTrans(p_BcTransLog_o);
		return -1;
	}
  //校验接收机构密钥
	if ( 0 != BC_TermCheckMac(tiBcInsInf.key_index, tiBcInsInf.key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) )
	{
		sprintf(buff, "接收机构MAC校验未通过.流水号:[%s],终端号:[%s],商户号:[%s] ",p_BcTransLog->sys_tra_no,p_BcTransLog->center_term_id,p_BcTransLog->center_mchnt_cd );
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		SendPayResultToRecive("N00096");
		BC_UnLockTrans(p_BcTransLog_o);
		return -1;
	}
  //判断提交交易是否成功
	if(memcmp(p_BcTransLog_o->rcv_ins_id_cd, INS_YBZF, 8)==0)
	{
		memcpy(p_BcTransLog->resp_cd, p_BcTransLog->fld_28 + 4, 2);
		p_BcTransLog->resp_cd[2] = 0;
	}
    strcpy(p_BcTransLog_o->resp_cd_rcv, p_BcTransLog->resp_cd);  
	if ( 0 == memcmp(p_BcTransLog->resp_cd,"00",2))	//交易成功
	{
	    strcpy(p_BcTransLog_o->fld_28, "N00009");
	    strcpy(p_BcTransLog_o->resp_cd, "09");
		strcpy(p_BcTransLog_o->trans_flag, "I");
		strcpy(p_BcTransLog_o->bc_settle_in, "0");
		//更新数据库
		iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if ( 0 != sqlCode )
		{
			sprintf(buff, "更新BcTransLog失败！");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
        	tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		}
		return 0; 
	}
  //交易不成功
	strcpy(p_BcTransLog_o->is_over, "0");
  	if( 0 == memcmp(p_BcTransLog->resp_cd,"65",2) || 0 == memcmp(p_BcTransLog->resp_cd,"53",2))
  	{
		if(p_BcTransLog_o->flag_6[0] == '1')
		{
			switch(p_BcTransLog_o->trans_flag[0])
			{
				case 'E':
					strcpy(p_BcTransLog_o->trans_flag,"F"); //第三次请求关键信息
					strcpy(p_BcTransLog_o->fld_28, "N10053"); 
					break;
				case 'G':
					strcpy(p_BcTransLog_o->trans_flag,"X"); //支付失败
					strcpy(p_BcTransLog_o->bc_settle_in, "0");
					strcpy(p_BcTransLog_o->is_over, "1");
					break;
    			case 'C':
					strcpy(p_BcTransLog_o->trans_flag,"D"); //第二次请求关键信息
					strcpy(p_BcTransLog_o->fld_28, "N00053"); 
					break;
				default:
    	  			//重复报文，不处理
					break;
			}   	
		}
		else
		{
			strcpy(p_BcTransLog_o->trans_flag,"X"); //支付失败
			strcpy(p_BcTransLog_o->is_over, "1");
			strcpy(p_BcTransLog_o->bc_settle_in, "0");
		}
	}
	else
	{
		strcpy(p_BcTransLog_o->trans_flag, "X" );
		strcpy(p_BcTransLog_o->is_over, "1");
		strcpy(p_BcTransLog_o->bc_settle_in, "0");
	}
	strcpy(p_BcTransLog_o->bc_settle_in, "0");
	if(p_BcTransLog_o->is_over[0] == '1')
	{
		if(memcmp(p_BcTransLog_o->rcv_ins_id_cd, INS_HFTX, 8)==0)
		{
			strcpy(tiBcErrInf.fwd_ins_id_cd, p_BcTransLog->fwd_ins_id_cd);
			strcpy(tiBcErrInf.rcv_ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd);
			strcpy(tiBcErrInf.resp_code, p_BcTransLog->resp_cd);
			tiBcErrInfOpr(GLB_DB_SELECT1, &tiBcErrInf, &sqlCode);
			if(sqlCode == 0)
			{
				strcpy(p_BcTransLog_o->fld_28, tiBcErrInf.fld_28);
				strcpy(p_BcTransLog_o->resp_cd, tiBcErrInf.resp_cd);
				msg = tiBcErrInf.msg;
			}
			else
			{
				strcpy(p_BcTransLog_o->fld_28, "N00096");
				strcpy(p_BcTransLog_o->resp_cd, "96");
				msg = NULL;
			}
		}
	}		
	if(p_BcTransLog_o->flag_6[0] == '1')//发送IVR支付失败信息
	{
		iRet =BusIVR_Z22_RespIvr(p_BcTransLog_o->fld_28, p_BcTransLog_o);
		if ( 0 != iRet  )//发送给IVR失败
		{
			sprintf(buff, "发送IVR支付失败信息出错！");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			SendPayResultToRecive("N00096");
			return -1;
		}else
		{
			strcpy(p_BcTransLog_o->resp_cd_verifi, "N00000");
  		}	  
  	}
	//更新BcTransLog
	iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
	if ( 0 != iRet )
	{
		sprintf(buff, "tlBcTransLogOpr 更新库失败！.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
	}
  	//发送给受理方失败信息
  	if(p_BcTransLog_o->is_over[0] == '1')return SendPayResultToRecive_Msg(p_BcTransLog_o->fld_28, msg);
}

//支付结果通知
int BusHF_Z22_RESP2(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso, char* flag)
{
	int succeed = 1 ;
    int iRet = -1   ;
    int sqlCode, reIvrFlag;
    char buff[256], *msg;

    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; // from db
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;  // new resp
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
	ti_bc_err_inf_def tiBcErrInf;
    
    strcpy(p_BcTransLog_o->flag_lock, "0");
    BC_SendMonitor_trans("0","2",p_BcTransLog, p_tiBcTransInf);
    strcpy(p_BcTransLog_o->flag_1, "1");
    	
    //获取#33接收机构信息
    memset(p_tiBcInsInf->ins_id_cd, 0, sizeof(p_tiBcInsInf->ins_id_cd) );
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog->fwd_ins_id_cd ) ;
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(JAVA系统)的机构信息 !",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
	    sprintf(buff, "查询机构线路信息失败,机构id:[%s].",p_tiBcInsInf->ins_id_cd); 
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		return -1;
    }    
    //校验接收机构密钥
    if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) )
    {
		sprintf(buff, "校验接收机构密钥失败,机构id:[%s].",p_tiBcInsInf->ins_id_cd); 
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		return -1;
	}
    //判断交易是否成功
	if(memcmp(p_BcTransLog_o->rcv_ins_id_cd, INS_YBZF, 8)==0)
	{
		memcpy(p_BcTransLog->resp_cd, p_BcTransLog->fld_28 + 4, 2);
		p_BcTransLog->resp_cd[2] = 0;
	}
    if(p_BcTransLog_o->is_over[0] != '1')
    	strcpy(p_BcTransLog_o->resp_cd_rcv, p_BcTransLog->resp_cd);    
    strcpy(p_BcTransLog_o->flag_rcv, "2" );
    strcpy(p_BcTransLog_o->bill_id_rcv, p_BcTransLog->fld_45);
    //check field
    if ( 0 != BC_CheckIsoDataField(iso, memcmp(p_BcTransLog_o->rcv_ins_id_cd, INS_YBZF, 8)==0 ? "3,4,11,12,13,28" : "3,4,11,12,13,39"))
    {
		sprintf(buff, "检测0870域信息出错！");
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
    }
    reIvrFlag = 0;
	msg = NULL;
	if ( 0 == memcmp(p_BcTransLog->resp_cd,"00",2) )
    {
        //交易成功
	    strcpy(p_BcTransLog_o->fld_28, "N00000");
	    strcpy(p_BcTransLog_o->resp_cd, "00");
		strcpy(p_BcTransLog_o->resp_cd_rcv, "00");
        strcpy(p_BcTransLog_o->trans_flag, "Z");
        strcpy(p_BcTransLog_o->bc_settle_in, "1");
		strcpy(p_BcTransLog_o->settle_dt, p_BcTransLog->settle_dt);
		strcpy(p_BcTransLog_o->retri_ref_no, p_BcTransLog->retri_ref_no);
        chkCardnoList();
        SMS_Send(1);
        reIvrFlag = 1;
	}
	else
	{
        strcpy(p_BcTransLog_o->bc_settle_in, "0");
		strcpy(tiBcErrInf.fwd_ins_id_cd, p_BcTransLog->fwd_ins_id_cd);
		strcpy(tiBcErrInf.rcv_ins_id_cd, p_BcTransLog_o->fwd_ins_id_cd);
	    if(p_BcTransLog_o->is_over[0] != '1')
	    {
			strcpy(p_BcTransLog_o->resp_cd_rcv, p_BcTransLog->resp_cd);
			reIvrFlag = 1;
		}
		strcpy(tiBcErrInf.resp_code, p_BcTransLog_o->resp_cd_rcv);
		tiBcErrInfOpr(GLB_DB_SELECT1, &tiBcErrInf, &sqlCode);
		if(sqlCode == 0)
		{
			strcpy(p_BcTransLog_o->fld_28, tiBcErrInf.fld_28);
			strcpy(p_BcTransLog_o->resp_cd, tiBcErrInf.resp_cd);
			msg = tiBcErrInf.msg;
		}
		else
		{
			strcpy(p_BcTransLog_o->fld_28, "N00096");
			strcpy(p_BcTransLog_o->resp_cd, "96");
		}
	}
	strcpy(p_BcTransLog_o->is_over, "1");
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
    if ( 0 != sqlCode )
    {
		sprintf(buff, "tlBcTransLogOpr 更新库记录失败!.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
    }
    //更新bill库
    BusZ_BillCopy(p_BcTransLog_o, p_tlBcBillLog);
    iRet = tlBcBillLogOpr(GLB_DB_UPDATE1, p_tlBcBillLog, &sqlCode);
    if ( 0 != sqlCode )
    {
	    sprintf(buff, "账单BILL入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
    } else
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d>tlBcBillLogOpr 更新库成功",__FILE__,__LINE__); 
    }     
    if(p_BcTransLog_o->flag_6[0] == '1' && reIvrFlag == 1)
    {
	    //给IVR应答
		iRet =BusIVR_Z22_RespIvr(p_BcTransLog_o->fld_28 , p_BcTransLog_o);
		if ( 0 != iRet  )
		{
			sprintf(buff, "发送给IVR失败");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		}else
		{
			strcpy(p_BcTransLog_o->resp_cd_verifi, "1");
		}
	}
	SendPayResultToRecive_Msg(p_BcTransLog_o->fld_28, msg);
	ReturnPayResultToJAVA("00", iso, p_tiBcTransInf, p_BcTransLog_o);
}

// 超时处理
int BusHF_Z22_RT()
{
	char vdate[17], e_vdate[17], cvv[17], e_cvv[17], gate_id[16];
	char tmpkeyindex[6], temp[1024], buff[1024], resp_code[6+1];
	ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
	ti_bc_ins_inf_def tiBcIvrInsInf; // IVR机构的机构信息
	ISO_data iso;
	ISO_data *p_iso = &iso;
	int sqlCode = 0, iRet = 0, len;
	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
	  
	dcs_debug(0,0, "<FILE:%s,LINE:%d>enter JAVA支付请求超时的处理 ",__FILE__,__LINE__);
	//查询接收机构信息
	memset(&tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
	memcpy(tiBcInsInf.ins_id_cd, p_BcTransLog->rcv_ins_id_cd, sizeof(tiBcInsInf.ins_id_cd));
	tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
	if( 0 != sqlCode || '0' == *tiBcInsInf.openflag)	
	{
		dcs_log(0, 0,"<FILE:%s,LINE:%d>call BC_CheckIsoDataField  接收机构不存在.",__FILE__,__LINE__);
		BC_SendExeception("02","接收机构不存在.");
		//原交易解锁
		BC_UnLockTrans(p_BcTransLog_o);
		return -1;
	}
	decryptInfo(cvv, vdate, p_BcTransLog->encrypt_info, 0, "00005500", 1); 
	return SendPayMessageToJAVA(p_iso, &tiBcInsInf, p_BcTransLog, vdate, cvv, 1); 
}

//发送支付报文0820到JAVA前置
int SendPayMessageToJAVA(ISO_data *p_iso, ti_bc_ins_inf_def *tiBcInsInf, tl_bc_trans_log_def *p_BcTransLog, char *vdate, char *cvv, int flag)
{
	char temp[1024],buff[1024],tempKeyIndex[10],resp_code[6+1], gate_id[17], vMon[3], vYear[3];
	int len, sqlCode;
  //获取网关
	if(0 != GetRouteGateway(gate_id, p_BcTransLog->rcv_ins_id_cd, p_BcTransLog->iss_ins_id_cd))
	{
		//没有找到网关信息
		sprintf(buff,"查找网关信息失败.(rcv_ins_id_cd:%s,iss_ins_id_cd:%s) ", p_BcTransLog->rcv_ins_id_cd, p_BcTransLog->iss_ins_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		if(flag)SendPayResultToRecive("N00096");
		return -1;	  		
	}  
	memset(p_iso, 0, sizeof(ISO_data));
	clearbit(p_iso);
	if( 0 != BC_SetIsoMode(p_iso, tiBcInsInf->packet_type) )
	{
		sprintf(buff, "设置报文类型[%s]失败", tiBcInsInf->packet_type);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		if(flag)SendPayResultToRecive("N00096");
		return -1;	  		
	}
	rtrim(p_BcTransLog->pri_acct_no);
	setbit_t(p_iso, 0 , "0820", 4); 
	if(memcmp(p_BcTransLog->rcv_ins_id_cd, INS_HFTX, 8)==0)
		setbit_t(p_iso, 3, "810001", 6);
	else
		setbit_t(p_iso, 3, "810002", 6);
	sprintf(buff, "%.12d", atoi(p_BcTransLog->trans_at) );
	setbit_t(p_iso, 4 , (unsigned char *)buff, 12);
	setbit_t(p_iso, 5 , (unsigned char *)p_BcTransLog->fld_5, 12);
	setbit_t(p_iso, 8 , (unsigned char *)p_BcTransLog->fld_8, 8);
	setbit_t(p_iso, 11, (unsigned char *)p_BcTransLog->center_sys_tra_no, 6);
	setbit_t(p_iso, 12, (unsigned char *)p_BcTransLog->loc_trans_tm, 6);
	setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
	setbit_t(p_iso, 33, (unsigned char *)p_BcTransLog->rcv_ins_id_cd, 8);
	setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog->center_mchnt_cd, 20);
	setbit_t(p_iso, 45, (unsigned char *)p_BcTransLog->fld_45, 19);
	//CardNo + “|” + ValidDate +  “|”+CVV2+ “|” +IdType+ “|” +IdNo+ “|” +Name    
	memset(buff, 0, sizeof(buff));
	if(memcmp(p_BcTransLog->rcv_ins_id_cd, INS_HFTX, 8)==0)
		sprintf(buff, "%s|%s|%s|%s|%s|%s",p_BcTransLog->pri_acct_no, vdate, cvv,
		p_BcTransLog->id_type, rtrim(p_BcTransLog->fld_61), rtrim(p_BcTransLog->pname));
	else
	{
		memset(vMon, 0, sizeof(vMon));
		memset(vYear, 0, sizeof(vYear));
		memcpy(vMon, vdate, 2);
		memcpy(vYear,vdate+2, 2);
		sprintf(buff, "%s|%s|%s|%s|%s|%s|%s",p_BcTransLog->pri_acct_no, vMon, vYear, cvv,
		p_BcTransLog->id_type, rtrim(p_BcTransLog->fld_61), rtrim(p_BcTransLog->pname));
	}
	memset(tempKeyIndex, 0 ,sizeof(tempKeyIndex));
	strcpy(tempKeyIndex, "S0");
	memcpy(tempKeyIndex+2, tiBcInsInf->key_index, 3);
	memset(resp_code, 0, sizeof(resp_code));
	len = strlen(buff);
	if((len % 8) !=0)  	len  += 8 - (len % 8);
	if( 1 != EncTrackPrg(resp_code, tempKeyIndex, tiBcInsInf->key_2, len, buff, &len, temp))
	{
		sprintf(buff, "加密第#48域失败(应答码:%s).", resp_code);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		if(flag)SendPayResultToRecive("N00096");
		return -1;        	
	}
	*(temp + len ) = 0;  
	setbit_t(p_iso, 48, (unsigned char *)temp, len);     
	setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,30));
	setbit_t(p_iso, 63, (unsigned char *)gate_id, 2);
	setbit_t(p_iso, 64, "00000000", 8); 
	BC_SetMacKey(tiBcInsInf->key_index, tiBcInsInf->key_1);
	BC_SendMonitor_iso("0","0", tiBcInsInf->line_nm, p_iso, &g_tiBcTransInf) ;
	if(0 != BC_InsertTimeOut1(0, gs_myFid, g_iSrcFoldId, I_TIMEOUT, p_iso, &g_TransMsg_o))
	{
		dcs_log(0, 0,"<FILE:%s,LINE:%d>BC_InsertBillNotify failure.",__FILE__,__LINE__ );
	}
	return BC_SendMessage(p_iso, tiBcInsInf->line_nm ) ;
}

//返回支付结果报文0880到JAVA前置
int ReturnPayResultToJAVA(char *resp_code, ISO_data *iso, ti_bc_trans_inf_def *p_tiBcTransInf, tl_bc_trans_log_def *p_BcTransLog)
{
	char rc[6 + 1];
	setbit_t(iso, 0, "0880", 4);
	if(memcmp(p_BcTransLog->rcv_ins_id_cd, INS_HFTX, 8)==0)
		setbit_t(iso, 39, resp_code, 2);
	else
	{
		strcpy(rc, "N000");
		memcpy(rc + 4, resp_code, 2);
		rc[6] = 0;
		setbit_t(iso, 28, rc, 6); 
	}
	setbit_t(iso,64, "00000000", 8);
	setbitoff(iso, 48);
	BC_SetRcvMacKey(&g_tiBcInsInf);
	BC_SendMonitor_iso("1", "0", "HFQZ", iso, p_tiBcTransInf);
	BC_SendMessageToSrc(iso);
	return 0;
}

