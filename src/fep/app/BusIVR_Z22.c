#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

//
int BusIVR_SendIvr(tl_bc_trans_log_def *p_BcTransLog, char *mchnt_cn_abbr)
{
	return BusIVR_SendIvr_flag(p_BcTransLog, mchnt_cn_abbr, 1);
}

//���ͻ�ȡ�ؼ�����Ϣ
int BusIVR_SendIvr_flag(tl_bc_trans_log_def *p_BcTransLog, char *mchnt_cn_abbr, int flag)
{
	int iRet;
	char info [1024];
	char buff[1024];
	IVR_STRUCT ivr ;
  
	TimeOut_Key *tk = (TimeOut_Key *)ivr.key;
	memset( &ivr, ' ', sizeof(IVR_STRUCT)-1 );
	memcpy( ivr.head, "CMDI0005" , 8);
	if(IsCreditCard(p_BcTransLog->card_attr))//���ÿ�
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>start to ��IVR���Ͳ�ѯCVN����.", __FILE__, __LINE__); 
		memcpy( ivr.opcode, "OP02", 4);
	}
	else	
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>start to ��IVR���Ͳ�ѯ��������.", __FILE__, __LINE__); 
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
		dcs_log(0, 0, "48������תUTF8ת��ʧ�ܣ�");
		return -1;
	}
	//�̻����
	rtrim(mchnt_cn_abbr);
	iRet = GBKToUTF8(mchnt_cn_abbr, strlen(mchnt_cn_abbr), info, sizeof(info));
	if( 0 < iRet )
	{
		*(info + iRet) = 0;
		IVR_AddFieldValue(ivr.data, "MERNAME", info);
	}
	else
	{
		dcs_log(0, 0, "�̻��������תUTF8ת��ʧ�ܣ�");
		return -1;
	}	
	if(0 > IVR_SendIVR(gs_myFid, &ivr))
	{
		return  -1;
	}
	if(flag)
	{
	 	//�峬ʱ��
		if(0 != BC_InsertIVRTimeOut(3, gs_myFid, g_iSrcFoldId, 10, &g_TransMsg_o))
		{
			dcs_log(0, 0,"<FILE:%s,LINE:%d>BC_InsertIVRNotify .",__FILE__,__LINE__ );
		}
	}
	return 0;
}

//�յ�IVR������Ϣ
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
	
	//��ȡһЩȫ�ֵı���, ����
	ti_bc_mt_flow_id_def tiBcMTFlowId ;
	ti_bc_ins_inf_def tiBcInsInf; // ���ջ����Ļ�����Ϣ
	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog;
	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //�����̻���Ϣ
	ti_bc_ins_inf_def tiBcIvrInsInf; // IVR�����Ļ�����Ϣ
	
	//init 
	strcpy(p_BcTransLog->flag_lock, "0");
	if('1' == *p_BcTransLog->is_over)
	{
		if('1' == *p_BcTransLog->bc_settle_in)
		{
			dcs_debug(0, 0,"<FILE:%s,LINE:%d>�յ�ivrӦ�𣬵��ǽ����Ѿ��ɹ����������·�����.\n",__FILE__,__LINE__);
			return ReturnIvrMsg("N00000", pIVR);
		}
		else
		{
			dcs_debug(0, 0,"<FILE:%s,LINE:%d>�յ�ivrӦ�𣬵��ǽ����Ѿ�ʧ�ܣ��������·�����.\n",__FILE__,__LINE__);
			return ReturnIvrMsg("N99999", pIVR);
		}
	}
	strcpy(p_BcTransLog->flag_verifi,  "2"  );
	memset(buff, 0, sizeof(buff) );    
	if( 0 >= IVR_Get_Field(buff, pIVR, "RSPCODE"))
	{
		sprintf(buff, "��ȡivrӦ����Ӧ����ʧ��");
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
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>ivr����Ӧ���ķ���ʧ��(Ӧ����:%s)", __FILE__,__LINE__, buff);
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
	//��ѯIVR������Ϣ
	if( 0!= BC_Search_ins_inf(resp_code, &tiBcIvrInsInf, INS_IVR))
	{
		sprintf(buff, "��ѯIVR������·��Ϣʧ��,����id:[%s].", INS_IVR);
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
			sprintf(buff, "��ȡivr����Ӧ����������Ϣʧ��");
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
			sprintf(buff, "��ȡivr�ؼ���Ӧ������Ϣʧ��");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			ReturnIvrMsg("N99999", pIVR);
			return SendPayResultToRecive("N00096");
		}
		// CVN2	 
		if( 0 >= IVR_Get_Field(buff + 16, pIVR, "CVN2"))
		{
			sprintf(buff, "��ȡivr�ؼ���Ӧ������Ϣʧ��");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			ReturnIvrMsg("N99999", pIVR);
			return SendPayResultToRecive("N00096");
		}
		if(0>decryptInfo(cvn, vdate, buff, &tiBcIvrInsInf, 0, 1))
		{
			sprintf(buff, "�������ÿ��ؼ���Ϣʧ��");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			ReturnIvrMsg("N99999", pIVR);
			return SendPayResultToRecive("N00096");
		}
		EncryptInfo(cvn, vdate, p_BcTransLog->encrypt_info);
	}
	//��ѯ���ջ�����Ϣ
	if( 0!= BC_Search_ins_inf(resp_code, &tiBcInsInf, g_rtIns_id_cd))
	{
		sprintf(buff, "��ѯ������·��Ϣʧ��,����id:[%s].", g_rtIns_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00096");
	}
	//��ȡת����ˮ��
	strcpy(tiBcMTFlowId.term_id   ,  p_BcTransLog->center_term_id);
	strcpy(tiBcMTFlowId.mchnt_cd  ,  p_BcTransLog->center_mchnt_cd);
	if(0 != tiBcMTFlowIdOpr(GLB_DB_SELECT1, &tiBcMTFlowId, &sqlCode))
	{
		sprintf(buff, "��ȡת����ˮ��ʧ��,�����̻���:[%s].�����ն˺�[%s].", p_BcTransLog->center_mchnt_cd, p_BcTransLog->center_term_id);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00096");
	}
	strcpy(p_BcTransLog->center_sys_tra_no,  tiBcMTFlowId.sys_tra_no);
	//��ȡ�����̻���Ϣ
	dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcQsMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__, p_BcTransLog->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->center_term_id);
	strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog->rcv_ins_id_cd);
	iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
	if ( 0 != sqlCode )
	{
		sprintf(buff, "��ȡ�����̻���Ϣʧ��,�����̻���:[%s].�����ն˺�[%s].", p_BcTransLog->center_mchnt_cd, p_BcTransLog->center_term_id);
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
	  		strcpy(p_BcTransLog->trans_flag,"E"); //�ڶ����ύ֧��
	  		break;
	  	case 'F':
	  		strcpy(p_BcTransLog->trans_flag,"G"); //�������ύ֧��
	  		break;
	  	case 'B':
	  		strcpy(p_BcTransLog->trans_flag,"C"); //�ύ֧��
	  		break;
	    default:
			dcs_debug(0, 0, "�ظ�����IVR��Ϣ[%s]", p_BcTransLog->trans_flag);
	    	return 0;
	}
	//���浽���ݿ�
	iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
	if ( 0 != iRet )
	{
		sprintf(buff, "����tlBcTransLog ʧ��");
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00003");
	} 
	else
		dcs_debug(0,0, "<FILE:%s,LINE:%d>tlBcTransLogOpr ���¿�ɹ�",__FILE__,__LINE__);
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
		sprintf(buff, "���ջ�������![%s]", g_rtIns_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);	
		//���͸��ն�ʧ����Ϣ
		ReturnIvrMsg("N99999", pIVR);
		return SendPayResultToRecive("N00091");    	
    }
    if(0 > iRet)
    {
		ReturnIvrMsg("N99999", pIVR);
	}
	return 0;
}

//�ظ�IVR������� 
int BusIVR_Z22_RespIvr(char *resp_code, tl_bc_trans_log_def *p_BcTransLog)
{
	int iRet;
	char info [1024];
	char buff[1024];
	IVR_STRUCT ivr ;
	TimeOut_Key *tk = (TimeOut_Key *)ivr.key;
	
	memset( &ivr, ' ', sizeof(IVR_STRUCT)-1 );
	memcpy( ivr.head, "CMDI0005" , 8);
	if(IsCreditCard(p_BcTransLog->card_attr))//���ÿ�
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

//IVR��ʱ����
int BusIVR_Z22_T()
{
	struct TimeOut_REC *p_TimeOutRec = &g_TimeOutRec;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; // from db
	ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
	int sqlCode, iRet;
	char buff[256];
	
	if(--p_TimeOutRec->iFlags>0)
	{
		//��ѯ�̻���Ϣ    
		strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog_o->mchnt_cd);
		tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
		if ( 0 != sqlCode ) //û���ҵ��̻���Ϣ
		{
			sprintf(buff, "�̻��Ų�����!(mchnt_cd:%s,sqlcode=%d)", p_tiBcMchntInf->mchnt_cd, sqlCode);
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			return SendPayResultToRecive("N00003"); 
	    }
		//���͸�IVRS
		iRet =BusIVR_SendIvr_flag( p_BcTransLog_o, p_tiBcMchntInf->mchnt_cn_abbr, 0);
		if ( iRet < 0 )
		{
			sprintf(buff, "���͸�IVRʧ��!");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			return SendPayResultToRecive("N00096"); 
		}
		//�峬ʱ��
		if(0 != BC_InsertIVRTimeOut(p_TimeOutRec->iFlags, gs_myFid, g_iSrcFoldId, 10, &g_TransMsg_o))
		{
			dcs_log(0, 0,"<FILE:%s,LINE:%d>BC_InsertIVRNotify failure.",__FILE__,__LINE__ );
		}
	}
	else
	{
		sprintf(buff, "���͸�IVR��Ϣ3��δӦ��!");
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

