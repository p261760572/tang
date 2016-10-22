#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

//�յ�������Ӧ����
int BusGZ_Z22_ev4(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso, char* flag)
{
	int succeed = 1 ;
    int iRet = -1   ;
    int sqlCode     ;
    char buff[256] ;

    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //�����̻���Ϣ
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog; // from db
    tl_bc_trans_log_def *p_BcTransLog1 = &g_TransMsg.BcTransLog;  // new resp
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;

    strcpy(p_BcTransLog->flag_lock, "0");
    BC_SendMonitor_trans("1","2",p_BcTransLog1, p_tiBcTransInf);
    memset(p_BcTransLog->resp_cd_rcv, 0, sizeof(p_BcTransLog->resp_cd_rcv));
    memcpy(p_BcTransLog->resp_cd_rcv, p_BcTransLog1->resp_cd, 2);
    strcpy(p_BcTransLog->fld_28, "N000");
    strcpy_safe(p_BcTransLog->fld_28+4, p_BcTransLog1->resp_cd, 2);
    strcpy(p_BcTransLog->flag_1, "1");
    	
    // ��ȡ�����̻���Ϣ
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->center_term_id);
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->center_mchnt_cd);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, g_rtIns_id_cd);
    iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
	    sprintf(buff, "Ӧ���׻�ȡ�����̻���Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]", p_BcTransLog->center_sys_tra_no, p_BcTransLog->center_term_id, p_BcTransLog->center_mchnt_cd);	    
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		BusGZ_Z22_ev4_T();
		BusIVR_Z22_RespIvr("N99999", p_BcTransLog);
	    return SendPayResultToRecive("N00096");
    }
    if ( 0 != BC_TermCheckMac(p_tiBcQsMchntInf->mkey_index,p_tiBcQsMchntInf->mkey_1, g_Rcv_Buffer + 4 + 11, g_RcvBufferSize - 4 - 11) )
    {
	    sprintf(buff, "Ӧ����У����ջ����̻���Կʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]", p_BcTransLog->center_sys_tra_no, p_BcTransLog->center_term_id, p_BcTransLog->center_mchnt_cd);	    
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N000A0", buff, 1);
		BusGZ_Z22_ev4_T();
		BusIVR_Z22_RespIvr("N99999", p_BcTransLog);
	    return SendPayResultToRecive("N00096");
    }
    if ( 0 != BC_CheckIsoDataField(iso, "3,4,11,12,13,25,37,39,41,42,49") )
    {
	    sprintf(buff, "�������֧��Ӧ����ʧ��.");	    
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		BusGZ_Z22_ev4_T();
		BusIVR_Z22_RespIvr("N99999", p_BcTransLog);
	    return SendPayResultToRecive("N00096");
    }

    //�жϽ����Ƿ�ɹ�
    memcpy(p_BcTransLog->resp_cd, p_BcTransLog1->resp_cd, 2);
    p_BcTransLog->resp_cd[2] = 0;
    strcpy(p_BcTransLog->flag_rcv, "2" );
    if ( 0 == memcmp(p_BcTransLog1->resp_cd,"00", 2) )
    {
        //���׳ɹ�
        strcpy(p_BcTransLog->trans_flag, "Z" );
        strcpy(p_BcTransLog->bc_settle_in, "1");
		strcpy(p_BcTransLog->is_over, "1");
		BusIVR_Z22_RespIvr("N00000", p_BcTransLog);
        chkCardnoList();
        SMS_Send(1);
    }
   	else if( 0 == memcmp(p_BcTransLog1->resp_cd, "55", 2)) //�������
   	{
    	switch(p_BcTransLog->trans_flag[0])
    	{
    		case 'E':
			    strcpy(p_BcTransLog->is_over, "0");
     			strcpy(p_BcTransLog->trans_flag,"F"); //��������������
     			BusIVR_Z22_RespIvr("N0PSE2", p_BcTransLog);
    			break;
    		case 'G':
			    strcpy(p_BcTransLog->is_over, "1");
     			strcpy(p_BcTransLog->trans_flag,"Y"); //֧��ʧ��
		        strcpy(p_BcTransLog->bc_settle_in, "0");
     			BusIVR_Z22_RespIvr("N00055", p_BcTransLog);
    			break;
    		case 'C':
			    strcpy(p_BcTransLog->is_over, "0");
     			strcpy(p_BcTransLog->trans_flag,"D"); //�ڶ�����������
    			BusIVR_Z22_RespIvr("N0PSE1", p_BcTransLog);
    			break;
    	  default:
    	  	//�ظ����ģ�������
    	  	break;
    	}   	
   	}
    else
    {    
    	strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->trans_flag, "Y" );
        strcpy(p_BcTransLog->bc_settle_in, "0");
		BusIVR_Z22_RespIvr(p_BcTransLog->fld_28, p_BcTransLog);
    } 

	strcpy(p_BcTransLog->settle_dt, p_BcTransLog1->settle_dt);
	strcpy(p_BcTransLog->retri_ref_no, p_BcTransLog1->retri_ref_no);

    //�������ݿ�
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
	    sprintf(buff, "���棬�����˵��������ݿ��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
    }

    //����bill��
    BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
    iRet = tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
    if ( 0 != iRet )
    {
	    sprintf(buff, "���棬�˵�BILL���ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
    }             
    if ( p_BcTransLog->is_over[0] == '1' )
    {
        SendPayResultToRecive(p_BcTransLog->fld_28);
    }
    return 0;
}

// �ǰ�֧������ʱ�Ĵ��� -- �������
int BusGZ_Z22_ev4_T()
{
	int iRet, sqlCode;
	char buff[255];
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //����ԭʼ���׽��д���

	if(p_BcTransLog_o->is_over[0] != '1')
	{
	    strcpy(p_BcTransLog_o->is_over, "1");
	    strcpy(p_BcTransLog_o->resp_cd,"98");
	    strcpy(p_BcTransLog_o->fld_28 ,"N00098");
	}
    strcpy(p_BcTransLog_o->bc_settle_in, "0");
    strcpy(p_BcTransLog_o->flag_reversal, "1");        
    strcpy(p_BcTransLog_o->flag_4, "1");
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
    if ( 0 != iRet )
    {
	    sprintf(buff, "tlBcTransLogOpr ���¿�ʧ��!");
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
    } 
    
	SendPayResultToRecive("N00098");

    //��IVRӦ��
    BusIVR_Z22_RespIvr( "N99999", p_BcTransLog_o);
    
    return SendRollBack(1);
}

//�����ĳ�ʱ����
int BusGZ_Z22_ev4_RT()
{
	struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
    //���²��볬ʱ��
    if( pTimeOutRec->iFlags > 0 )
    {
		--pTimeOutRec->iFlags ;
		InsTimeOutTBL(pTimeOutRec);
    }
    return SendRollBack(0);
}


// ������������Ӧ����
int BusGZ_Z22_ev4_R(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso, char* flag)
{
	int sqlCode = 0;
	char resp_code[7];
	char buff[255];
	struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //�����̻���Ϣ
	
    BC_SendMonitor_trans("1","2",p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
    // ��ȡ�����̻���Ϣ
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->center_term_id);
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->center_mchnt_cd);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, g_rtIns_id_cd);
    tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
	    sprintf(buff, "Ӧ���׻�ȡ�����̻���Ϣʧ��.(mchnt_cd:%s,sqlcode=%d) ", p_BcTransLog->center_mchnt_cd, sqlCode );
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
	}
	else
    {
        if ( 0 != BC_TermCheckMac(p_tiBcQsMchntInf->mkey_index,p_tiBcQsMchntInf->mkey_1, g_Rcv_Buffer + 4 + 11, g_RcvBufferSize - 4 - 11) )
        {
		    sprintf(buff, "Ӧ����MACУ��ʧ��..��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]", p_BcTransLog->center_sys_tra_no, p_BcTransLog->center_term_id, p_BcTransLog->center_mchnt_cd);
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N000A0", buff, 0);
        }
        else
        {
			if( 0 == memcmp("00", p_BcTransLog->resp_cd, 2)
				|| 0 == memcmp("25", p_BcTransLog->resp_cd, 2)
				|| 0 == memcmp("12", p_BcTransLog->resp_cd, 2)){
				strcpy( p_BcTransLog_o->flag_reversal , "2");
				strcpy( p_BcTransLog_o->bc_settle_in , "0");
				strcpy( p_BcTransLog_o->is_over , "1");
				memcpy( p_BcTransLog->resp_cd_reversal, p_BcTransLog->resp_cd, 2);
				tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
				if( 0 != sqlCode )
				{
				    sprintf(buff, "����ɹ�������Ӧ��ʱ���ǿ�ʧ��! ��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s].",  p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
					ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
				}
				return 0;
			}        
		}
    }
	//���²��볬ʱ��
    if( pTimeOutRec->iFlags > 0 )
    {
    	-- pTimeOutRec->iFlags ;
      InsTimeOutTBL(pTimeOutRec);
    }
    else
    {
		strcpy( p_BcTransLog_o->flag_reversal , "3");
		strcpy( p_BcTransLog_o->bc_settle_in , "0");
		strcpy( p_BcTransLog_o->is_over , "1");
		memcpy( p_BcTransLog->resp_cd_reversal, p_BcTransLog->resp_cd, 2);
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if( 0 != sqlCode )
		{
		    sprintf(buff, "���������Ӧ��ʱ���ǿ�ʧ��! ��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]", p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd);
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
		}
	    sprintf(buff, "�Ѿ�����%d�γ�������δ���سɹ������ٴ���ñʽ���! ��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s].", I_TIMEOUT_TIMES, p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
    }
    BC_UnLockTrans(p_BcTransLog_o);
	return 0;
}

int ExpendApp_GZ(ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcIvrInsInf, char *password)
{
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog;
	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf = &g_tiBcQsMchntInf ; //�����̻���Ϣ
	ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf; //����������Ϣ
    ti_bc_ins_inf_def tiBcInsInf, *p_tiBcInsInf = &tiBcInsInf ; // ���ջ����Ļ�����Ϣ
	char buff[255], resp_code[6+1];
	int sqlCode, iRet;

    if( 0!= BC_Search_ins_inf(resp_code, p_tiBcInsInf, g_rtIns_id_cd))
    {
    	// ��ѯ���ջ�����·��Ϣʧ��
		sprintf(buff, "��ѯ������·��Ϣʧ��,����id:[%s].", g_rtIns_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00091", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		SendPayResultToRecive("N00091");
		return -1;
    }
    memset(p_iso, 0, sizeof(ISO_data));
    clearbit(p_iso);        
    //���ð�ģʽ
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
 		sprintf(buff, "���ð�ģʽ![%s]", p_tiBcInsInf->packet_type);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
        //����ʧ��
		SendPayResultToRecive("N00096");
		return -1;
    }

	//ת��������Ϣ
	memset(buff, 0, sizeof(buff) );  
	asc_to_bcd(buff, password, 16, 0);  
	memset(password, 0, sizeof(password));
	memset(resp_code, 0, sizeof(resp_code));
	iRet = DESPINTR(resp_code, password, p_tiBcIvrInsInf->key_index, p_tiBcIvrInsInf->key_3,
	p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_2, buff, "0000000000000000", "0000000000000000");
	if( 0 != iRet)
	{
 		sprintf(buff, "ת��ivr����ʧ��(Ӧ����:%s).\n", resp_code);
		ErrInfoInBcTransLog(__FILE__, __LINE__, RESP_CODE_CUS019, buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		SendPayResultToRecive("N00096");
		return -1;
	}

    //�齨����
    rtrim(p_BcTransLog->pri_acct_no);
    set_msghead(p_iso, "\x60\x00\x05\x00\x00\x60\x21\x00\x00\x00\x00", 11); 
    setbit_t(p_iso, 0 , "0200", 4); 
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "190000", 6);
    sprintf(buff, "%.12d", atoi(p_BcTransLog->trans_at) );
    setbit_t(p_iso, 4 , (unsigned char *)buff, 12);
    setbit_t(p_iso, 11, (unsigned char *)p_BcTransLog->center_sys_tra_no, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
    setbit_t(p_iso, 22, (unsigned char *)"011", 3);
    setbit_t(p_iso, 25, (unsigned char *)"81", 2);
    setbit_t(p_iso, 26, (unsigned char *)"06", 2);
    setbit_t(p_iso, 41, (unsigned char *)p_BcTransLog->center_term_id, 8);
    setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog->center_mchnt_cd, 15);
    memset(buff, ' ', 30);
    memcpy(buff, "PAGA", 4);
    memcpy(buff+4, p_BcTransLog->center_mchnt_cd + 7, 4);
    memcpy(buff+24, "000000", 6);
    buff[30] = '#';
    buff[31] = 0;
    setbit_t(p_iso, 48, (unsigned char *)buff, 31);
    setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,3));
    setbit_t(p_iso, 52, (unsigned char *)password, 8);
    setbit_t(p_iso, 53, "1000000000000000", 16); 
    memset(buff, 0, sizeof(buff));
    memset(buff, ' ', 22);
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
    memcpy(buff+2, p_BcTransLog->fld_61, MIN(20, strlen(p_BcTransLog->fld_61)));
    setbit_t(p_iso, 61, buff, 22); 
    setbit_t(p_iso, 64, "00000000", 8); 

    
    //���͸��㶫�����ǰ�֧������
    if ( '0' == *tiBcInsInf.key_type )
    {
        BC_SetMacKey(0,0);
    } else
    {
		dcs_debug(0, 0,"mk[%s],k[%s]",p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_1);
		BC_SetMacKey(p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_1);
    }
    
    //���ͼ����Ϣ
    BC_SendMonitor_iso("0","0", tiBcInsInf.line_nm, p_iso, &g_tiBcTransInf) ;
    
    iRet = BC_InsertTimeOut1(0, gs_myFid, g_iSrcFoldId,I_TIMEOUT, p_iso, &g_TransMsg_o);
    if ( 0 != iRet )
    {
    	dcs_log(0, 0,"<FILE:%s,LINE:%d>BC_InsertBillNotify failure.",__FILE__,__LINE__ );
    }
	setbitoff(p_iso, 13);
	return BC_SendMessage(p_iso, tiBcInsInf.line_nm);
}

int SendRollBack(int flag)
{
	int iRet;
	int sqlCode;
	char buff[2048];
	
	ISO_data iso;
	ISO_data *p_iso = &iso ;
	ti_bc_ins_inf_def tiBcInsInf; // ���ջ����Ļ�����Ϣ
	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //�����̻���Ϣ
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
	
	dcs_debug(0,0, "<FILE:%s,LINE:%d>enter BusGZ_Z22_ev4_T ",__FILE__,__LINE__);
	  
    dcs_debug(0,0,"<FILE:%s,LINE:%d>start to call tiBcInsInfOpr for select.",__FILE__,__LINE__,RPT_TO_LOG,0,0);
    memset( &tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy( tiBcInsInf.ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode  || '0' == *tiBcInsInf.openflag )
    {
	    sprintf(buff, "���ջ���[%s]������", tiBcInsInf.ins_id_cd);	    
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
        return -1;
    }
    
    // ��ȡ�����̻���Ϣ
    strcpy( p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog_o->center_mchnt_cd );
    strcpy( p_tiBcQsMchntInf->term_id, p_BcTransLog_o->center_term_id );
    strcpy( p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd );
    iRet = tiBcQsMchntInfOpr( GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode );
    if ( 0 != sqlCode )
    {
        //û���ҵ������̻���Ϣ
        dcs_log(0, 0,"<FILE:%s,LINE:%d>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__, p_tiBcQsMchntInf->mchnt_cd, sqlCode );
        BC_SendExeception("03","cant not find qs mchnt.(mchnt_cd:%s,sqlcode=%d) ",p_tiBcQsMchntInf->mchnt_cd,sqlCode );
        return -1;
    }    
    
    memset(p_iso, 0, sizeof(ISO_data));
    clearbit(p_iso);
    
    if ( 0 != BC_SetIsoMode(p_iso, tiBcInsInf.packet_type) )
    {
	    sprintf(buff, "BC_SetIsoMode failure(ins_id_cd:%s,packet_type:%s)��", tiBcInsInf.ins_id_cd, tiBcInsInf.packet_type);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
        return -1;
    }    

    rtrim(p_BcTransLog_o->pri_acct_no);
    set_msghead(p_iso, "\x60\x00\x05\x00\x00\x60\x21\x00\x00\x00\x00", 11); 
    setbit_t(p_iso, 0,  (unsigned char *)"0400", 1);
    setbit_t(p_iso, 3,  (unsigned char *)"190000", 6);
    sprintf(buff, "%.12d", atoi(p_BcTransLog_o->trans_at));   
    setbit_t(p_iso, 4,  (unsigned char *)buff, 12);
    setbit_t(p_iso, 11, (unsigned char *)p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog_o->loc_trans_dt, 4);
    setbit_t(p_iso, 22, (unsigned char *)"011", 3);
    setbit_t(p_iso, 25, (unsigned char *)"81", 2);
    setbit_t(p_iso, 39, (unsigned char *)"98", 2);
    setbit_t(p_iso, 41, (unsigned char *)p_BcTransLog_o->center_term_id, 8);
    setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog_o->center_mchnt_cd, 15);
    memset(buff, ' ', 30);
    memcpy(buff, "PAGA", 4);
    memcpy(buff+4, p_BcTransLog_o->center_mchnt_cd + 7, 4);
    memcpy(buff+24, "000000", 6);
    buff[30] = '#';
    buff[31] = 0;
    setbit_t(p_iso, 48, (unsigned char *)buff, 31);
    setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog_o->trans_curr_cd, min_strlen(p_BcTransLog_o->trans_curr_cd,30));
    memset(buff, '0', 29);
    memcpy(buff+6, (unsigned char *)p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(p_iso, 61, (unsigned char *)buff, 29);
    setbit_t(p_iso, 64, "00000000", 8); 
    
    BC_SendMonitor_iso("0", "0", tiBcInsInf.line_nm, p_iso, 0);
	if(flag)
	{
	    iRet = BC_InsertTimeOut1(I_TIMEOUT_TIMES, gs_myFid, g_iSrcFoldId, I_TIMEOUT, p_iso, &g_TransMsg_o);
	    if ( 0 != iRet )
	    {
		    sprintf(buff, "BC_InsertTimeOut1 failure");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
	    }
	}
    //���͸��㶫��������
    if ( '0' == *tiBcInsInf.key_type )
    {
    	BC_SetMacKey(0, 0);
    }
    else
    {
    	BC_SetMacKey(p_tiBcQsMchntInf->mkey_index, p_tiBcQsMchntInf->mkey_1);
    }    
    setbitoff(p_iso, 13);
    return BC_SendMessage(p_iso, tiBcInsInf.line_nm );    
}
