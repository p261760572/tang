#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

//收到异步应答报文 0840
int BusHF_Z22_ev3(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso, char* flag)
{
	int iRet = -1   ;
	int sqlCode     ;
	char buff[256] ;
	char *msg=NULL;
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; // from db
	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;  // new resp
	ti_bc_err_inf_def tiBcErrInf;
	strcpy(p_BcTransLog_o->flag_lock, "0");
	BC_SendMonitor_trans("1","2",p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_rcv, "4");
	//check field
	if ( 0 != BC_CheckIsoDataField(iso, "3,4,11,12,13,39") )
	{
		sprintf(buff, "校验接收报文0840失败！"); 
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		SendPayResultToRecive("N00096");
		BC_UnLockTrans(p_BcTransLog_o);
		return -1;
	}
    //判断交易是否成功
    strcpy(p_BcTransLog_o->is_over, "1");
    strcpy(p_BcTransLog_o->resp_cd_rcv, p_BcTransLog->resp_cd);
    strcpy(p_BcTransLog_o->flag_rcv, "2" );
    strcpy(p_BcTransLog_o->bill_id_rcv, p_BcTransLog->fld_45);
	if ( 0 == memcmp(p_BcTransLog->resp_cd, "00", 2) )
	{
		//交易成功
	    strcpy(p_BcTransLog_o->fld_28, "N00000");
	    strcpy(p_BcTransLog_o->resp_cd, "00");
		strcpy(p_BcTransLog_o->settle_dt, p_BcTransLog->settle_dt);
		strcpy(p_BcTransLog_o->retri_ref_no, p_BcTransLog->retri_ref_no);
        strcpy(p_BcTransLog_o->trans_flag, p_tiBcTransDet->trans_flag_s );
        strcpy(p_BcTransLog_o->bc_settle_in, "1");
        chkCardnoList();
        SMS_Send(1);
	}
	else if(memcmp(p_BcTransLog->resp_cd, "15", 2) == 0 && IsCreditCard(p_BcTransLog_o->card_attr) == 0) //密码错误
	{
    	switch(p_BcTransLog_o->trans_flag[0])
    	{
    		case 'E':
			    strcpy(p_BcTransLog_o->is_over, "0");
     			strcpy(p_BcTransLog_o->trans_flag,"F"); //第三次请求密码
    			strcpy(p_BcTransLog_o->fld_28,"N0PSE2");
    			break;
    		case 'G':
     			strcpy(p_BcTransLog_o->trans_flag,"Y"); //支付失败
		        strcpy(p_BcTransLog_o->bc_settle_in, "0");
    			break;
    		case 'C':
			    strcpy(p_BcTransLog_o->is_over, "0");
     			strcpy(p_BcTransLog_o->trans_flag,"D"); //第二次请求密码
    			strcpy(p_BcTransLog_o->fld_28,"N0PSE1");
    			break;
    	  	default:
    	  		dcs_log(0, 0, "重复报文或状态不正确！trans_flag[%c]", p_BcTransLog_o->trans_flag[0]);
    	  		//重复报文，不处理		
    	  		break;
		}
	}
	if(p_BcTransLog_o->is_over[0] == '1')
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
		}
	    strcpy(p_BcTransLog_o->trans_flag, p_tiBcTransDet->trans_flag_e );
	}
	if(p_BcTransLog_o->flag_6[0] == '1')//发送IVR支付结果信息
	{
		iRet =BusIVR_Z22_RespIvr(p_BcTransLog_o->fld_28, p_BcTransLog_o);
		if ( 0 != iRet  )//发送给IVR失败
		{
			sprintf(buff, "发送IVR支付结果信息出错！");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		}
		else
		{
			strcpy(p_BcTransLog_o->resp_cd_verifi, "1");
  		}	  
  	}
	iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
	if ( 0 != iRet )
	{
		sprintf(buff, "tlBcTransLogOpr 更新库失败！.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
	}
  	if(p_BcTransLog_o->is_over[0] == '1') return SendPayResultToRecive_Msg(p_BcTransLog_o->fld_28, msg);
}

//收到请求确认报文 0830
int BusHF_Z22_ev2(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso, char* flag)
{
	int iRet = -1   ;
	int sqlCode     ;
	char resp_code[6+1];
	char buff[256] ;
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; // from db
	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;  // new resp
	strcpy(p_BcTransLog_o->flag_lock, "0");
	BC_SendMonitor_trans("1","2",p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_rcv, "4");
	memset(resp_code, 0, sizeof(resp_code));
	memcpy(resp_code, "N000", 4);
	memcpy(resp_code + 4, p_BcTransLog->resp_cd, 2);
	//check field
	if ( 0 != BC_CheckIsoDataField(iso, "3,4,11,12,13,39") )
	{
		sprintf(buff, "校验接收报文0830失败！"); 
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		SendPayResultToRecive("N00096");
		BC_UnLockTrans(p_BcTransLog_o);
		return -1;
	}
    //判断交易是否成功
    strcpy(p_BcTransLog_o->fld_28, resp_code);
    strcpy(p_BcTransLog_o->resp_cd, resp_code+4);
    strcpy(p_BcTransLog_o->resp_cd_rcv, resp_code);
    strcpy(p_BcTransLog_o->flag_rcv, "2" );
    strcpy(p_BcTransLog_o->bill_id_rcv, p_BcTransLog->fld_45);
	if ( 0 == memcmp(resp_code,"N00000",6) )
	{
		//交易成功
		strcpy(p_BcTransLog_o->bc_settle_in, "0");
		//更新数据库
		iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if ( 0 != sqlCode )
		{
			sprintf(buff, "更新BcTransLog失败！");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
		}
    	tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);     
	    iRet = BC_InsertTimeOut1(5, gs_myFid, g_iSrcFoldId, 10, iso, &g_TransMsg_o);
	    if ( 0 != iRet )
	    {
	    	dcs_log(0, 0,"<FILE:%s,LINE:%d>BC_InsertBillNotify failure.",__FILE__,__LINE__ );
	    }
		return 0;
	}
	strcpy(p_BcTransLog_o->bc_settle_in, "0");
	if(p_BcTransLog_o->flag_6[0] == '1')//发送IVR支付失败信息
	{
		iRet =BusIVR_Z22_RespIvr( "N99999", p_BcTransLog_o);
		if ( 0 != iRet  )//发送给IVR失败
		{
			sprintf(buff, "发送IVR支付失败信息出错！");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		}
		else
		{
			strcpy(p_BcTransLog_o->resp_cd_verifi, "1");
  		}	  
  	}
	iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
	if ( 0 != iRet )
	{
		sprintf(buff, "tlBcTransLogOpr 更新库失败！.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
	}
  	return SendPayResultToRecive("N00096");
}

// 同步应答超时
int BusYL_Z22_ev3_T1()
{
	char buff[256];
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; // from db
	sprintf(buff, "同步应答超时！流水号:[%s],终端号:[%s],商户号:[%s].",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
	ErrInfoInBcTransLog(__FILE__, __LINE__, "N00098", buff, 0);
  	return SendPayResultToRecive("N00098");
}

// 异步应答超时 -- 发起查询
int BusYL_Z22_ev3_T2()
{
	int iLen;
	int iRet;
	int sqlCode;
	char buff[4096];
	char packet_type[5];
	TimeOut_Key tk;
	ISO_data iso;
	struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
	ti_bc_ins_inf_def tiBcInsInf, *p_tiBcInsInf = &tiBcInsInf; // 接收机构的机构信息
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //依据原始交易进行处理

    //组建报文
    memset(&iso,0,sizeof(ISO_data));
    clearbit(&iso);

  //搜索机构信息	 
	if( 0!= BC_Search_ins_inf(buff, p_tiBcInsInf, p_BcTransLog_o->rcv_ins_id_cd))
	{
		sprintf(buff, "搜索机构信息[%s]失败!", p_BcTransLog_o->rcv_ins_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		return -1;
	}
	
    if ( 0 != BC_SetIsoMode(&iso, p_tiBcInsInf->packet_type) )
    {
		sprintf(buff, "设置机构报文类型失败[%s].", p_tiBcInsInf->packet_type);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
        dcs_log(0,0,"<FILE:%s,LINE:%d>设置机构报文类型失败。",__FILE__,__LINE__);
        return -1;
    }
    
    setbit_t(&iso, 0 , "0620", 4); 
    setbit_t(&iso, 3 , "810001", 6);
    setbit_t(&iso, 11, (unsigned char *)p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(&iso, 12, (unsigned char *)p_BcTransLog_o->loc_trans_tm, 6);
    setbit_t(&iso, 13, (unsigned char *)p_BcTransLog_o->loc_trans_dt, 4);
 	setbit_t(&iso, 33, INS_CENT, 8);
    setbit_t(&iso, 42, (unsigned char *)p_BcTransLog_o->center_mchnt_cd, 15);
    setbit_t(&iso, 45, (unsigned char *)p_BcTransLog_o->fld_45, 19);
    setbit_t(&iso, 64, "00000000", 8);

    //发送监控信息
    memset(buff, 0, sizeof(buff) );    
    if ( '0' == *p_tiBcInsInf->key_type )
    {
        BC_SetMacKey(0,0);
    } 
    else
    {
		dcs_debug(0, 0,"mk[%s],k[%s]",p_tiBcInsInf->key_index, p_tiBcInsInf->key_1);
		BC_SetMacKey(p_tiBcInsInf->key_index, p_tiBcInsInf->key_1);
    }
   	tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);     
	//重新插入超时表
    if( pTimeOutRec->iFlags > 0 )
    {
    	-- pTimeOutRec->iFlags ;
    	g_TimeOutRec.iMaxTimeout *= 2;
      	InsTimeOutTBL(pTimeOutRec);
    }
    else
    {
		sprintf(buff, "查询超次！流水号:[%s],终端号:[%s],商户号:[%s].",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00098", buff, 0);
	  	return SendPayResultToRecive("N00098");
    }
    BC_UnLockTrans(p_BcTransLog_o);
	return BC_SendMessage(&iso, p_tiBcInsInf->line_nm);
}

//发送交易请求
int ExpendApp_YL_Debit(ISO_data *p_iso, tl_bc_trans_log_def *p_BcTransLog, ti_bc_ins_inf_def *p_tiBcIvrInsInf, char *password)
{
	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
	ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf; //交易类型信息
    ti_bc_ins_inf_def tiBcInsInf, *p_tiBcInsInf = &tiBcInsInf ; // 接收机构的机构信息
	char buff[511], pwdbuff[256 + 1], resp_code[6+1];
	int sqlCode, iRet;

    rtrim(p_BcTransLog->pri_acct_no);
    memset(p_iso, 0, sizeof(ISO_data));
    if( 0!= BC_Search_ins_inf(resp_code, p_tiBcInsInf, g_rtIns_id_cd))
    {
    	// 查询接收机构线路信息失败
		sprintf(buff, "查询机构线路信息失败,机构id:[%s].", g_rtIns_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00091", buff, 1);
		SendPayResultToRecive("N00091");
		return -1;
    }
    //设置包模式
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
 		sprintf(buff, "设置包模式![%s]", p_tiBcInsInf->packet_type);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
        //交易失败
		SendPayResultToRecive("N00096");
		return -1;
    }
    //组建报文
    setbit_t(p_iso, 0 , "0820", 4); 
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "810001", 6);
    sprintf(buff, "%d", atoi(p_BcTransLog->trans_at) );
    setbit_t(p_iso, 4 , (unsigned char *)buff, 12);
	setbit_t(p_iso, 5 , (unsigned char *)p_BcTransLog->fld_5, min_strlen(p_BcTransLog->fld_5,12));    
	setbit_t(p_iso, 8 , (unsigned char *)p_BcTransLog->fld_8, min_strlen(p_BcTransLog->fld_8,8));
	setbit_t(p_iso, 11, (unsigned char *)p_BcTransLog->center_sys_tra_no, 6);
    setbit_t(p_iso, 12, (unsigned char *)p_BcTransLog->loc_trans_tm, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
 	setbit_t(p_iso, 33, INS_CENT, 8);
	setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog->center_mchnt_cd, 15);
	if(GBKToUTF8(p_tiBcQsMchntInf->mchnt_cn_abbr, strlen(p_tiBcQsMchntInf->mchnt_cn_abbr), buff, 511)>0)
	{
		rtrim(buff);
		setbit_t(p_iso, 43, (unsigned char *)buff, min_strlen(buff, 40));		
	}
	else
	{
		sprintf(buff, "商户简称GBKtoUTF8转码失败![%s]", p_tiBcQsMchntInf->mchnt_cn_abbr);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		SendPayResultToRecive("N00096");			
		return -1;
	}
	setbit_t(p_iso, 45, (unsigned char *)p_BcTransLog->fld_45, 19);
	if(GBKToUTF8(p_BcTransLog->fld_48, strlen(p_BcTransLog->fld_48), buff, 511)>0)
	{
		rtrim(buff);
	    setbit_t(p_iso, 48, (unsigned char *)buff, min_strlen(buff, 130));
	}
	else
	{
		sprintf(buff, "附加信息GBKtoUTF8转码失败![%s]", p_BcTransLog->fld_48);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		SendPayResultToRecive("N00096");			
		return -1;
	}	
    setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,3));
    memset(buff, 0, sizeof(buff));
    memcpy(buff, "00", sizeof(buff));
	//转换密码信息
    if(0 > PinConversion(p_BcTransLog->center_mchnt_cd, p_tiBcIvrInsInf->key_index, p_tiBcIvrInsInf->key_3, p_BcTransLog->pri_acct_no, password, buff+2))
    {
		sprintf(buff, "转换ivr密码失败!");
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		SendPayResultToRecive("N00096");			
		return -1;
    }
    //*****************/
//    memcpy(buff + 2, "4D9451B0B408866B688ECA231C23CBFF4BF12D4EB354EEE4F3FB4A33BEF93226C34C07DBE5255211EF08C98E3958A8654133FCEFA8A350C62B700C9F98FD30A59078AAFE69FBF10A1C52560B9FB3158F2D571F2F2B16AA6A845E896F517C8034B0B66512E17CD503F4D84E59C0B9EA005C8AAD87CD18055245DBABD7D4298799", 256);
//    memcpy(buff + 2, "B9DB18E9C8ED40E4B3070D0355AA089340681006F56E50C27E8EB94A0151F14ABDBCD08F99778AABB3AA58E3034B2125FA792C3D0BC51A4478DF641B6729585663E80448941241AFFA5F94B2AC0B7F784C6C4B402D47B8DE133D0D3347807A75D7E2BBB60EF39F82C842603E628FAF62D35DA44DE68F57FBC187E8DB0646AA2A", 256);
    /******************/
    setbit_t(p_iso, 60, buff, strlen(buff));
    setbit_t(p_iso, 61, p_BcTransLog->fld_61, min_strlen(p_BcTransLog->fld_61, 20));
    memset(buff, 0, sizeof(buff));
	if(GBKToUTF8(p_BcTransLog->pname, strlen(p_BcTransLog->pname), buff, 511)>0)
	{
		rtrim(buff);
	    setbit_t(p_iso, 62, (unsigned char *)buff, min_strlen(buff, 20));
	}
	else
	{
		sprintf(buff, "姓名GBKtoUTF8转码失败![%s]", p_BcTransLog->pname);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		SendPayResultToRecive("N00096");			
		return -1;
	}	
    memset(buff, 0, sizeof(buff));
    switch(p_BcTransLog->id_type[1])
    {
    	case '0':
    		memcpy(buff, "01", 2);
    		break;
    	case '1':
    		memcpy(buff, "03", 2);
    		break;
    	case '2':
    		memcpy(buff, "02", 2);
    		break;
    	case '3':
    		memcpy(buff, "07", 2);
    		break;
    	case '4':
    		memcpy(buff, "04", 2);
    		break;
    	case '5':
    		memcpy(buff, "99", 2);
    		break;
    	case '6':
    		memcpy(buff, "03", 2);
    		break;
    	default:
    		memcpy(buff, "99", 2);
    		break;
    }
    setbit_t(p_iso, 63, buff, 2);
    setbit_t(p_iso, 64, "00000000", 8); 
    
    //发送给银联前置支付请求
    if ( '0' == *tiBcInsInf.key_type )
    {
        BC_SetMacKey(0,0);
    } 
    else
    {
		dcs_debug(0, 0,"mk[%s],k[%s]",tiBcInsInf.key_index, tiBcInsInf.key_1);
		BC_SetMacKey(tiBcInsInf.key_index, tiBcInsInf.key_1);
    }
    //发送监控信息
    BC_SendMonitor_iso("0","0", tiBcInsInf.line_nm, p_iso, &g_tiBcTransInf) ;
    
    iRet = BC_InsertTimeOut1(0, gs_myFid, g_iSrcFoldId, 600, p_iso, &g_TransMsg_o);
    if ( 0 != iRet )
    {
    	dcs_log(0, 0,"<FILE:%s,LINE:%d>BC_InsertBillNotify failure.",__FILE__,__LINE__ );
    }
	return BC_SendMessage(p_iso, tiBcInsInf.line_nm );
}


int ExpendApp_YL_Credit(ISO_data *p_iso, tl_bc_trans_log_def *p_BcTransLog, int flag)
{
	char caBuffer[180]; 	
 	int cardPasswd_len;
	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
	ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf; //交易类型信息
    ti_bc_ins_inf_def tiBcInsInf, *p_tiBcInsInf = &tiBcInsInf ; // 接收机构的机构信息
	char buff[511], resp_code[6+1];
	int sqlCode, iRet;

    rtrim(p_BcTransLog->pri_acct_no);
    memset(p_iso, 0, sizeof(ISO_data));
    if( 0!= BC_Search_ins_inf(resp_code, p_tiBcInsInf, g_rtIns_id_cd))
    {
    	// 查询接收机构线路信息失败
		sprintf(buff, "查询机构线路信息失败,机构id:[%s].", g_rtIns_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00091", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		if(flag)SendPayResultToRecive("N00091");
		return -1;
    }
    //设置包模式
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
 		sprintf(buff, "设置包模式![%s]", p_tiBcInsInf->packet_type);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
        //交易失败
		if(flag)SendPayResultToRecive("N00096");
		return -1;
    }
    //组建报文
    setbit_t(p_iso, 0 , "0820", 4); 
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "810001", 6);
    sprintf(buff, "%d", atoi(p_BcTransLog->trans_at) );
    setbit_t(p_iso, 4 , (unsigned char *)buff, 12);
	setbit_t(p_iso, 5 , (unsigned char *)p_BcTransLog->fld_5, min_strlen(p_BcTransLog->fld_5,12));    
	setbit_t(p_iso, 8 , (unsigned char *)p_BcTransLog->fld_8, min_strlen(p_BcTransLog->fld_8,8));
	setbit_t(p_iso, 11, (unsigned char *)p_BcTransLog->center_sys_tra_no, 6);
    setbit_t(p_iso, 12, (unsigned char *)p_BcTransLog->loc_trans_tm, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
 	setbit_t(p_iso, 33, INS_CENT, 8);
	setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog->center_mchnt_cd, 15);
	if(GBKToUTF8(p_tiBcQsMchntInf->mchnt_cn_abbr, strlen(p_tiBcQsMchntInf->mchnt_cn_abbr), buff, 511)>0)
	{
		rtrim(buff);
		setbit_t(p_iso, 43, (unsigned char *)buff, min_strlen(buff, 40));		
	}
	else
	{
		sprintf(buff, "商户简称GBKtoUTF8转码失败![%s]", p_tiBcQsMchntInf->mchnt_cn_abbr);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		if(flag)SendPayResultToRecive("N00096");			
		return -1;
	}
	setbit_t(p_iso, 45, (unsigned char *)p_BcTransLog->fld_45, 19);
	if(GBKToUTF8(p_BcTransLog->fld_48, strlen(p_BcTransLog->fld_48), buff, 511)>0)
	{
		rtrim(buff);
	    setbit_t(p_iso, 48, (unsigned char *)buff, min_strlen(buff, 130));
	}
	else
	{
		sprintf(buff, "附加信息GBKtoUTF8转码失败![%s]", p_BcTransLog->fld_48);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		if(flag)SendPayResultToRecive("N00096");			
		return -1;
	}	
    setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,3));
    memset(buff, 0, sizeof(buff));
    memcpy(buff, "01", sizeof(buff));
    memcpy(buff + 2, p_BcTransLog->encrypt_info, 48);
    setbit_t(p_iso, 60, buff, 50);
    setbit_t(p_iso, 61, p_BcTransLog->fld_61, min_strlen(p_BcTransLog->fld_61, 20));
    memset(buff, 0, sizeof(buff));
	if(GBKToUTF8(p_BcTransLog->pname, strlen(p_BcTransLog->pname), buff, 511)>0)
	{
		rtrim(buff);
	    setbit_t(p_iso, 62, (unsigned char *)buff, min_strlen(buff, 20));
	}
	else
	{
		sprintf(buff, "姓名GBKtoUTF8转码失败![%s]", p_BcTransLog->pname);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		if(flag)SendPayResultToRecive("N00096");			
		return -1;
	}	
    memset(buff, 0, sizeof(buff));
    switch(p_BcTransLog->id_type[1])
    {
    	case '0':
    		memcpy(buff, "01", 2);
    		break;
    	case '1':
    		memcpy(buff, "03", 2);
    		break;
    	case '2':
    		memcpy(buff, "02", 2);
    		break;
    	case '3':
    		memcpy(buff, "07", 2);
    		break;
    	case '4':
    		memcpy(buff, "04", 2);
    		break;
    	case '5':
    		memcpy(buff, "99", 2);
    		break;
    	case '6':
    		memcpy(buff, "03", 2);
    		break;
    	default:
    		memcpy(buff, "99", 2);
    		break;
    }
    setbit_t(p_iso, 63, buff, 2);
    setbit_t(p_iso, 64, "00000000", 8); 
    
    //发送给银联前置支付请求
    if ( '0' == *tiBcInsInf.key_type )
    {
        BC_SetMacKey(0,0);
    } 
    else
    {
		dcs_debug(0, 0,"mk[%s],k[%s]", tiBcInsInf.key_index, tiBcInsInf.key_1);
		BC_SetMacKey(tiBcInsInf.key_index, tiBcInsInf.key_1);
    }
    //发送监控信息
    BC_SendMonitor_iso("0","0", tiBcInsInf.line_nm, p_iso, &g_tiBcTransInf) ;
    
    iRet = BC_InsertTimeOut1(0, gs_myFid, g_iSrcFoldId, 600, p_iso, &g_TransMsg_o);
    if ( 0 != iRet )
    {
    	dcs_log(0, 0,"<FILE:%s,LINE:%d>BC_InsertBillNotify failure.",__FILE__,__LINE__ );
    }
	return BC_SendMessage(p_iso, tiBcInsInf.line_nm );
}

int PinConversion(char *mchnt_cd, char *key_index, char *key, char *pri_acct_no, char *des_pin_data, char *rsa_pin_data)
{
	char rsa_key_str[280 + 1], rsa_key_bin[140 + 1], rsa_pan[16 + 1], return_code[6 + 1], rsa_pin_data_bin[256+1];
	int ret, rsa_datalen;
	char cfgfile[256], caBuffer[1024];
	FILE *fd;
	sprintf(caBuffer, "key/PuK%s.key", rtrim(mchnt_cd));
	if(u_fabricatefile(caBuffer, cfgfile, sizeof(cfgfile)) < 0)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, caBuffer);
		return -1;
	}
	fd = fopen(cfgfile, "r");	
	if(fd == NULL)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, cfgfile);
    	return -1;
	}  
	if(fread(rsa_key_str, 280, 1, fd)!=1)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Read file[%s] error!", __FILE__, __LINE__, cfgfile);
		fclose(fd);
    	return -1;
	}
    fclose(fd);
    
    asc_to_bcd(rsa_key_bin, rsa_key_str, 280, 0);
    
	memset(rsa_pan, '0', sizeof(rsa_pan));
	rsa_pan[16] = 0;	
	memcpy(rsa_pan + 4, pri_acct_no + strlen(pri_acct_no) - 13, 12);	
	
	ret = DESTORAS_PIN(return_code, key, 16, rsa_key_bin, 140, key_index, rsa_pan, "0000000000000000", des_pin_data, rsa_pin_data_bin, &rsa_datalen);
	if(0 > ret)
	{
		dcs_log(rsa_key_bin, 140, "<FILE:%s,LINE:%d>DESTORAS_PIN Fail! return_code=[%s], rsa_key_str[%s]", __FILE__, __LINE__, return_code, rsa_key_str);
		return ret;
	}	 
	if(rsa_datalen !=128)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>rsa_datalen[%d] Err", __FILE__, __LINE__, rsa_datalen);
		return -1;
	}
	rsa_datalen = EncodeBase64(rsa_pin_data_bin, rsa_pin_data, 128);         
	return 1;
}

