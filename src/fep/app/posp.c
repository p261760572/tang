#include "ibdcs.h"
#include "tmcibtms.h"
#include "dbstruct.h"
#include "bc.h"

int POSP_SendKeysReset(int keysFlag, ISO_data *p_iso);   //����POSP��������
int POSP_RespondKeyResetApp(char *resp_code, ISO_data *iso);  //�ظ�POSP������������

int BillInDB(char *resp_code, char *errInfo); //�˵����
int FWD_Institution_CheckMac(ti_bc_ins_inf_def *p_tiBcInsInf, char *ins_id_cd, char *resp_code, char *errInfo, int flag); //��ѯ���ͻ�����У��MAC
int TradeMerchant(char *resp_code, char *errInfo, int flag); //��ѯ�����̻���Ϣ
int RCV_Institution(ti_bc_ins_inf_def *p_tiBcInsInf, char *ins_id_cd, char *resp_code, char *errInfo); //��ѯ���ջ�����Ϣ
int QueryOriginaTransLog(char *resp_code, char *errInfo); //��ѯԭ������Ϣ

//POSP��������
int POSP_ExpendApp(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    ti_bc_ins_inf_def tiBcInsInf; //���ջ�����Ϣ 
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    char resp_code[10], errInfo[200];
    int iRet, sqlCode;
    BC_SendMonitor_trans("0", "3", p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
	if( 0 != FWD_Institution_CheckMac(p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd, resp_code, errInfo, 1))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondExpendApp(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != (iRet = BillInDB(resp_code, errInfo)))
	{
		if(iRet > 0) ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondExpendApp(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != RCV_Institution(&tiBcInsInf, p_BcTransLog->rcv_ins_id_cd, resp_code, errInfo))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondExpendApp(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != TradeMerchant(resp_code, errInfo, 1))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondExpendApp(resp_code, iso, p_tiBcInsInf);
	}
    strcpy(p_BcTransLog_o->area_cd, p_tiBcMchntInf->area_cd);
	strcpy(p_BcTransLog_o->trans_flag, "C");
  	strcpy(p_BcTransLog_o->flag_rcv,  "1"  );
  	strcpy(p_BcTransLog_o->is_over ,  "0"  );
  	if ( 0 != BC_InsertIntoDB(p_BcTransLog_o, &sqlCode))
  	{
  		if(sqlCode == SQL_ERD_UNIKEY)
		{
			strcpy(resp_code, "N00094");
			sprintf(errInfo, "������ˮ���ظ���");
		}
  		else
		{
			strcpy(resp_code, "N00096");
			sprintf(errInfo, "�彻����ˮ��ʧ�ܣ�");
		}
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
  		return POSP_RespondExpendApp(resp_code, iso, p_tiBcInsInf);
	}
	if(0 != CUPS_ExpendApp(resp_code, errInfo, &tiBcInsInf))
	{
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
  		return POSP_RespondExpendApp(resp_code, iso, p_tiBcInsInf);
	} 	
	return 0;
}

//POSP���ѳ���
int POSP_ExpendUndo(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    ti_bc_ins_inf_def tiBcInsInf; //���ջ�����Ϣ 
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    char resp_code[10], errInfo[200];
    int iRet, sqlCode, bitLen;
    BC_SendMonitor_trans("0", "3", p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
	if( 0 != FWD_Institution_CheckMac(p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd, resp_code, errInfo, 1))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondExpendUndo(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != (iRet = BillInDB(resp_code, errInfo)))
	{
		if(iRet > 0) ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondExpendUndo(resp_code, iso, p_tiBcInsInf);
	}	
	if( 0 != QueryOriginaTransLog(resp_code, errInfo))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondExpendUndo(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != RCV_Institution(&tiBcInsInf, p_BcTransLog->rcv_ins_id_cd, resp_code, errInfo))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondExpendUndo(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != TradeMerchant(resp_code, errInfo, 0))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondExpendUndo(resp_code, iso, p_tiBcInsInf);
	}
	strcpy(p_BcTransLog_o->trans_flag, "C");
  	strcpy(p_BcTransLog_o->flag_rcv,  "1"  );
  	strcpy(p_BcTransLog_o->is_over ,  "0"  );
  	if ( 0 != BC_InsertIntoDB(p_BcTransLog_o, &sqlCode))
  	{
  		if(sqlCode == SQL_ERD_UNIKEY)
		{
			strcpy(resp_code, "N00094");
			sprintf(errInfo, "������ˮ���ظ���");
		}
  		else
		{
			strcpy(resp_code, "N00096");
			sprintf(errInfo, "�彻����ˮ��ʧ�ܣ�");
		}
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
  		return POSP_RespondExpendUndo(resp_code, iso, p_tiBcInsInf);
	}
	if(0 != CUPS_ExpendUndo(resp_code, errInfo, &tiBcInsInf))
	{
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
  		return POSP_RespondExpendUndo(resp_code, iso, p_tiBcInsInf);
	} 	
	return 0;
}

//POSP����ѯ
int POSP_BalanceQuery(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    ti_bc_ins_inf_def tiBcInsInf; //���ջ�����Ϣ 
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    char resp_code[10], errInfo[200];
    int iRet, sqlCode, bitLen;
    BC_SendMonitor_trans("0", "3", p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
	if( 0 != FWD_Institution_CheckMac(p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd, resp_code, errInfo, 1))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondBalanceQuery(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != (iRet = BillInDB(resp_code, errInfo)))
	{
		if(iRet > 0) ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondBalanceQuery(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != RCV_Institution(&tiBcInsInf, p_BcTransLog->rcv_ins_id_cd, resp_code, errInfo))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondBalanceQuery(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != TradeMerchant(resp_code, errInfo, 1))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondBalanceQuery(resp_code, iso, p_tiBcInsInf);
	}
    strcpy(p_BcTransLog_o->area_cd, p_tiBcMchntInf->area_cd);
	strcpy(p_BcTransLog_o->trans_flag, "C");
  	strcpy(p_BcTransLog_o->flag_rcv,  "1"  );
  	strcpy(p_BcTransLog_o->is_over ,  "0"  );
  	if ( 0 != BC_InsertIntoDB(p_BcTransLog_o, &sqlCode))
  	{
  		if(sqlCode == SQL_ERD_UNIKEY)
		{
			strcpy(resp_code, "N00094");
			sprintf(errInfo, "������ˮ���ظ���");
		}
  		else
		{
			strcpy(resp_code, "N00096");
			sprintf(errInfo, "�彻����ˮ��ʧ�ܣ�");
		}
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
  		return POSP_RespondBalanceQuery(resp_code, iso, p_tiBcInsInf);
	}
	if(0 != CUPS_BalanceQuery(resp_code, errInfo, &tiBcInsInf))
	{
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
  		return POSP_RespondBalanceQuery(resp_code, iso, p_tiBcInsInf);
	} 
	return 0;	
}

//POSP���ѳ���
int POSP_ExpendRollBack(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    ti_bc_ins_inf_def tiBcInsInf; //���ջ�����Ϣ 
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    char resp_code[10], errInfo[200];
    int iRet, sqlCode, bitLen;
    BC_SendMonitor_trans("0", "3", p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
    glb_GetLocalTimeString(p_BcTransLog->settle_dt, 5,"%m%d");
	if( 0 != FWD_Institution_CheckMac(p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd, resp_code, errInfo, 1))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondExpendRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != (iRet = BillInDB(resp_code, errInfo)))
	{
		if(iRet > 0) ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondExpendRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != (iRet = QueryOriginaTransLog(resp_code, errInfo)))
	{
		if(iRet > 0) 
			ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		else
			strcpy(resp_code, "N00000");
		return POSP_RespondExpendRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != RCV_Institution(&tiBcInsInf, p_BcTransLog->rcv_ins_id_cd, resp_code, errInfo))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondExpendRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != TradeMerchant(resp_code, errInfo, 0))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondExpendRollBack(resp_code, iso, p_tiBcInsInf);
	}
	strcpy(p_BcTransLog_o->trans_flag, "L");
  	strcpy(p_BcTransLog_o->flag_rcv,  "1"  );
  	strcpy(p_BcTransLog_o->is_over ,  "1"  );
    strcpy(p_BcTransLog_o->resp_cd, "98");
    strcpy(p_BcTransLog_o->fld_28 ,"N00098");
    strcpy(p_BcTransLog_o->bc_settle_in, "0");
    strcpy(p_BcTransLog_o->flag_reversal, "1");        
    strcpy(p_BcTransLog_o->flag_4, "1");
    memcpy(p_BcTransLog_o->center_sys_tra_no, p_BcTransLog->originaDataElements + 4, 6);
  	if ( 0 != BC_InsertIntoDB(p_BcTransLog_o, &sqlCode))
  	{
  		if(sqlCode == SQL_ERD_UNIKEY)
		{
			strcpy(resp_code, "N00094");
			sprintf(errInfo, "������ˮ���ظ���");
		}
  		else
		{
			strcpy(resp_code, "N00096");
			sprintf(errInfo, "�彻����ˮ��ʧ�ܣ�");
		}
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
  		return POSP_RespondExpendRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if(0 != CUPS_ExpendRollBack(resp_code, errInfo, &tiBcInsInf))
	{
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
  		return POSP_RespondExpendRollBack(resp_code, iso, p_tiBcInsInf);
	} 	
	return 0;
}

//POSP���ѳ�������
int POSP_ExpendUndoRollBack(ti_bc_trans_inf_def *p_tiBcTransInf, ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso, char* flag)
{
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    ti_bc_ins_inf_def tiBcInsInf; //���ջ�����Ϣ 
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    char resp_code[10], errInfo[200];
    int iRet, sqlCode, bitLen;
    BC_SendMonitor_trans("0", "3", p_BcTransLog, p_tiBcTransInf);
	strcpy(p_BcTransLog_o->flag_lock, "0");
    glb_GetLocalTimeString(p_BcTransLog->settle_dt, 5,"%m%d");
	if( 0 != FWD_Institution_CheckMac(p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd, resp_code, errInfo, 1))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondExpendUndoRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != (iRet = BillInDB(resp_code, errInfo)))
	{
		if(iRet > 0) ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return POSP_RespondExpendUndoRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != (iRet = QueryOriginaTransLog(resp_code, errInfo)))
	{
		if(iRet > 0) 
			ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		else
			strcpy(resp_code, "N00000");
		return POSP_RespondExpendUndoRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != RCV_Institution(&tiBcInsInf, p_BcTransLog->rcv_ins_id_cd, resp_code, errInfo))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondExpendUndoRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if( 0 != TradeMerchant(resp_code, errInfo, 0))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 2);
		return POSP_RespondExpendUndoRollBack(resp_code, iso, p_tiBcInsInf);
	}
	strcpy(p_BcTransLog_o->trans_flag, "L");
  	strcpy(p_BcTransLog_o->flag_rcv,  "1"  );
  	strcpy(p_BcTransLog_o->is_over ,  "1"  );
    strcpy(p_BcTransLog_o->resp_cd, "98");
    strcpy(p_BcTransLog_o->fld_28 ,"N00098");
    strcpy(p_BcTransLog_o->bc_settle_in, "0");
    strcpy(p_BcTransLog_o->flag_reversal, "1");        
    strcpy(p_BcTransLog_o->flag_4, "1");
    memcpy(p_BcTransLog_o->center_sys_tra_no, p_BcTransLog->originaDataElements + 4, 6);
  	if ( 0 != BC_InsertIntoDB(p_BcTransLog_o, &sqlCode))
  	{
  		if(sqlCode == SQL_ERD_UNIKEY)
		{
			strcpy(resp_code, "N00094");
			sprintf(errInfo, "������ˮ���ظ���");
		}
  		else
		{
			strcpy(resp_code, "N00096");
			sprintf(errInfo, "�彻����ˮ��ʧ�ܣ�");
		}
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
  		return POSP_RespondExpendUndoRollBack(resp_code, iso, p_tiBcInsInf);
	}
	if(0 != CUPS_ExpendUndoRollBack(resp_code, errInfo, &tiBcInsInf))
	{
  		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 1);
  		return POSP_RespondExpendUndoRollBack(resp_code, iso, p_tiBcInsInf);
	} 		
	return 0;
}

//POSP��Կ��������
int POSP_KeysResetApp(ti_bc_trans_inf_def *p_tiBcTransInf, ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso, char* flag)
{
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    char resp_code[10], errInfo[200], return_code[3], sek_index[6], tek_index[6], tek_tmk_data[33];
    char sec_relatd_ctrl_info[17]; //��ȫ������Ϣ
    int bitLen, exitFlag=1, keysFlag;
    BC_SendMonitor_trans("0", "3", p_BcTransLog, p_tiBcTransInf);
	if( 0 != FWD_Institution_CheckMac(p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd, resp_code, errInfo, 0))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		POSP_RespondKeyResetApp(resp_code, iso);
		return 1;
	}
	memcpy(resp_code, "N00006", 6);
	bitLen = getbit(iso, 53, sec_relatd_ctrl_info);
	if(bitLen == 16) 
	{
		memset(sek_index, 0, sizeof(sek_index));
		memset(tek_index, 0, sizeof(tek_index));
		memset(return_code, 0, sizeof(return_code));
		memcpy(sek_index, "S0", 2);
		memcpy(tek_index, "T0", 2);
		memcpy(sek_index + 2, p_tiBcInsInf->key_index, 3);
		memcpy(tek_index + 2, p_tiBcInsInf->key_index, 3);
		if( memcmp(sec_relatd_ctrl_info, "1600000000000000", 16) == 0) //PIK(˫����Կ)
		{
			if(0 > DESGETTMK2(return_code, sek_index, tek_index, p_tiBcInsInf->key_2, tek_tmk_data))
			{
				sprintf(errInfo, "����PIK��Կʧ�ܣ�return_code[%s]", return_code);
				memcpy(resp_code, "N000A7", 6);
			}
			else
			{
				memcpy(resp_code, "N00000", 6);
				keysFlag = 1;
				exitFlag = 0;
			}
		}else if( memcmp(sec_relatd_ctrl_info, "2000000000000000", 16) == 0) //MAK(������Կ)
		{
			if(0 > DESGETTMK(return_code, sek_index, tek_index, p_tiBcInsInf->key_1, tek_tmk_data))
			{
				sprintf(errInfo, "����PIK��Կʧ�ܣ�return_code[%s]", return_code);
				memcpy(resp_code, "N000A7", 6);
			}
			else
			{
				memcpy(resp_code, "N00000", 6);
				keysFlag = 0;
				exitFlag = 0;
			}
		}
	}
	POSP_RespondKeyResetApp(resp_code, iso);
	if(exitFlag) return 2;
	POSP_SendKeysReset(keysFlag, iso);
	return 0;
}

//POSP��Կ����Ӧ��
int POSP_KeysResetReply(ti_bc_trans_inf_def *p_tiBcTransInf, ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    char resp_code[10], errInfo[200];
    BC_SendMonitor_trans("1", "3", p_BcTransLog, p_tiBcTransInf);
//	if( 0 != FWD_Institution_CheckMac(p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd, resp_code, errInfo, 0))
//	{
//		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
//	}
	return 0;
}

//POSP��Կ����
int POSP_SendKeysReset(int keysFlag, ISO_data *p_iso)
{
	char errInfo[200];
	char keysB[19];
	int sqlCode;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf;
    tiBcInsInfOpr(GLB_DB_UPDATE2, p_tiBcInsInf, &sqlCode);
    if( 0 != sqlCode )
    {
	    // ��ѯ������Ϣʧ��
        sprintf(errInfo, "д����(ins_id_cd:%s)��Ϣʧ��", p_tiBcInsInf->ins_id_cd); 
		ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 0);
        return 1;
    }
    BC_SendMonitor_trans("0", "3", p_BcTransLog, p_tiBcTransInf);
    if	( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
  	{
		sprintf(errInfo, "����ISO8583������[%s]ʧ��!", p_tiBcInsInf->packet_type);
		ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
    	return 1;
  	}
  	glb_GetLocalTimeString(p_BcTransLog->transmsn_dt_tm, 11, "%m%d%H%M%S");
    setbit_t(p_iso, 0, "0800", 4);
    setbitoff(p_iso, 33);
    setbit_t(p_iso, 100, "00005500", 8);
    if(keysFlag == 0) //MAK
    {
    	asc_to_bcd(keysB, p_tiBcInsInf->key_1, 16, 0);
    	setbit_t(p_iso, 96, keysB, 8); 
    }
    if(keysFlag == 1) //PIK
    {
    	memcpy(keysB, "NK", 2);
    	asc_to_bcd(keysB, p_tiBcInsInf->key_2, 32, 0);
    	setbit_t(p_iso, 48, keysB, 16);
    }
	setbit_t(p_iso, 128, "00000000", 8);
    BC_SetMacKey(p_tiBcInsInf->key_index, p_tiBcInsInf->key_1);
  	BC_SendMonitor_iso("1", "0", p_BcTransLog->trans_chnl, p_iso, p_tiBcTransInf);
    BC_SendMessageToSrc(p_iso);
    return 0;
}

//POSP�ظ���Կ��������
int POSP_RespondKeyResetApp(char *resp_code, ISO_data *iso)
{
    ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
	setbit_t(iso, 39, resp_code+4, 2);
	BC_SetMacKey(0, 0);
  	BC_SendMonitor_iso("1", "0", p_BcTransLog->trans_chnl, iso, p_tiBcTransInf);
	BC_SendMessageToSrc(iso);
	return 0;
}

//POSP�ظ���������
int POSP_RespondExpendApp(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf)
{
	char errInfo[200];
	char tmpBuf[255];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //from DB;
    ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf;
    if(p_iso == 0) 
    {
    	ISO_data iso;
    	p_iso = &iso;
		memset(p_iso, 0, sizeof(ISO_data));
		if	( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
		{
			sprintf(errInfo, "����ISO8583������[%s]ʧ��!", p_tiBcInsInf->packet_type);
			ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
			return 1;
		}
		rtrim(p_BcTransLog_o->pri_acct_no);
		setbit_t(p_iso, 2, p_BcTransLog_o->pri_acct_no, 19);
		setbit_t(p_iso, 3, "000000", 6);
	  	sprintf(tmpBuf, "%.12d", atoi(p_BcTransLog->trans_at) );
	    setbit_t(p_iso, 4 , tmpBuf, 12);
		setbit_t(p_iso, 7, p_BcTransLog_o->transmsn_dt_tm, 10);
		setbit_t(p_iso, 11, p_BcTransLog_o->sys_tra_no, 6);
		setbit_t(p_iso, 12, p_BcTransLog_o->loc_trans_tm, 6);
		setbit_t(p_iso, 13, p_BcTransLog_o->loc_trans_dt, 4);
		setbit_t(p_iso, 14, "4912", 4);
		if(memcmp(p_BcTransLog->resp_cd, "00", 2) == 0)
		{
			setbit_t(p_iso, 15, p_BcTransLog->settle_dt, 4);
			rtrim(p_BcTransLog->auth_id_resp_cd);
			if(strlen(p_BcTransLog->auth_id_resp_cd)>0)setbit_t(p_iso, 38, p_BcTransLog->auth_id_resp_cd, 6);
		}
		else
		{
	        glb_GetLocalTimeString(tmpBuf,5,"%m%d");
	        setbit_t(p_iso, 15, tmpBuf, 4);
	    }
		setbit_t(p_iso, 18, p_BcTransLog_o->mchnt_tp, 4);
		setbit_t(p_iso, 25, "00", 4);
		setbit_t(p_iso, 32, p_BcTransLog_o->fld32_ins_id_cd, 8);
		setbit_t(p_iso, 33, p_BcTransLog_o->fwd_ins_id_cd, 8);
		setbit_t(p_iso, 37, p_BcTransLog_o->retri_ref_no, 12);
		setbit_t(p_iso, 41, p_BcTransLog_o->term_id, 8);
		setbit_t(p_iso, 42, p_BcTransLog_o->mchnt_cd, 15);
    	setbit_t(p_iso, 49, p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd, 3));
        setbit_t(p_iso, 60, p_BcTransLog_o->encrypt_info, 14);
    	setbit_t(p_iso, 128, "00000000", 8);
    }
    else
    {
		setbit_t(p_iso, 14, "4912", 4);
        glb_GetLocalTimeString(tmpBuf,5,"%m%d");
        setbit_t(p_iso, 15, tmpBuf, 4);
		setbit_t(p_iso, 25, "00", 4);
		setbitoff(p_iso, 22);
		setbitoff(p_iso, 26);
		setbitoff(p_iso, 35);
		setbitoff(p_iso, 36);
		setbitoff(p_iso, 43);
		setbitoff(p_iso, 52);
		setbitoff(p_iso, 53);
    }
    setbit_t(p_iso, 0, "0210", 4);
    setbit_t(p_iso, 39, resp_code+4, 2); 
    setbit_t(p_iso, 100, "00005500", 8);
    BC_SetMacKey(p_tiBcInsInf->key_index, p_tiBcInsInf->key_1);
  	BC_SendMonitor_iso("1", "0", p_tiBcInsInf->line_nm, p_iso, p_tiBcTransInf);
    BC_SendMessage(p_iso, p_tiBcInsInf->line_nm);
    return 0;
}

//POSP�ظ����ѳ���
int POSP_RespondExpendUndo(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf)
{
	char errInfo[200];
	char tmpBuf[255];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //from DB;
    ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf;
    if(p_iso == 0) 
    {
    	ISO_data iso;
    	p_iso = &iso;
		memset(p_iso, 0, sizeof(ISO_data));
		if	( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
		{
			sprintf(errInfo, "����ISO8583������[%s]ʧ��!", p_tiBcInsInf->packet_type);
			ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
			return 1;
		}
		rtrim(p_BcTransLog_o->pri_acct_no);
		setbit_t(p_iso, 2, p_BcTransLog_o->pri_acct_no, 19);
		setbit_t(p_iso, 3, "200000", 6);
	  	sprintf(tmpBuf, "%.12d", atoi(p_BcTransLog->trans_at) );
	    setbit_t(p_iso, 4 , tmpBuf, 12);
		setbit_t(p_iso, 7, p_BcTransLog_o->transmsn_dt_tm, 10);
		setbit_t(p_iso, 11, p_BcTransLog_o->sys_tra_no, 6);
		setbit_t(p_iso, 12, p_BcTransLog_o->loc_trans_tm, 6);
		setbit_t(p_iso, 13, p_BcTransLog_o->loc_trans_dt, 4);
		if(memcmp(p_BcTransLog->resp_cd, "00", 2) == 0)
		{
			setbit_t(p_iso, 15, p_BcTransLog->settle_dt, 4);
			rtrim(p_BcTransLog->auth_id_resp_cd);
			if(strlen(p_BcTransLog->auth_id_resp_cd)>0)setbit_t(p_iso, 38, p_BcTransLog->auth_id_resp_cd, 6);
		}
		else
		{
	        glb_GetLocalTimeString(tmpBuf,5,"%m%d");
	        setbit_t(p_iso, 15, tmpBuf, 4);
		}
		setbit_t(p_iso, 18, p_BcTransLog_o->mchnt_tp, 4);
		setbit_t(p_iso, 25, "00", 4);
		setbit_t(p_iso, 32, p_BcTransLog_o->fld32_ins_id_cd, 8);
		setbit_t(p_iso, 33, p_BcTransLog_o->fwd_ins_id_cd, 8);
		setbit_t(p_iso, 37, p_BcTransLog_o->retri_ref_no, 12);
		setbit_t(p_iso, 41, p_BcTransLog_o->term_id, 8);
		setbit_t(p_iso, 42, p_BcTransLog_o->mchnt_cd, 15);
    	setbit_t(p_iso, 49, p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd, 3));
        setbit_t(p_iso, 60, p_BcTransLog_o->encrypt_info, 14);
    	setbit_t(p_iso, 128, "00000000", 8);
    }
    else
    {
        glb_GetLocalTimeString(tmpBuf,5,"%m%d");
        setbit_t(p_iso, 15, tmpBuf, 4);
		setbitoff(p_iso, 22);
		setbitoff(p_iso, 26);
		setbitoff(p_iso, 35);
		setbitoff(p_iso, 36);
		setbitoff(p_iso, 43);
		setbitoff(p_iso, 52);
		setbitoff(p_iso, 53);
    	setbitoff(p_iso, 90);
        glb_GetLocalTimeString(tmpBuf,5,"%m%d");
        setbit_t(p_iso, 15, tmpBuf, 4);
    }
    setbit_t(p_iso, 0, "0210", 4);
    setbit_t(p_iso, 39, resp_code+4, 2); 
    setbit_t(p_iso, 100, "00005500", 8);
    BC_SetMacKey(p_tiBcInsInf->key_index, p_tiBcInsInf->key_1);
  	BC_SendMonitor_iso("1", "0", p_tiBcInsInf->line_nm, p_iso, p_tiBcTransInf);
    BC_SendMessage(p_iso, p_tiBcInsInf->line_nm);
    return 0;
}

//POSP�ظ�����ѯ
int POSP_RespondBalanceQuery(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf)
{
	char errInfo[200];
	char tmpBuf[255];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //from DB;
    ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf;
    if(p_iso == 0) 
    {
    	ISO_data iso;
    	p_iso = &iso;
		memset(p_iso, 0, sizeof(ISO_data));
		if	( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
		{
			sprintf(errInfo, "����ISO8583������[%s]ʧ��!", p_tiBcInsInf->packet_type);
			ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
			return 1;
		}
		rtrim(p_BcTransLog_o->pri_acct_no);
		setbit_t(p_iso, 2, p_BcTransLog_o->pri_acct_no, 19);
		setbit_t(p_iso, 3, "300000", 6);
		setbit_t(p_iso, 7, p_BcTransLog_o->transmsn_dt_tm, 10);
		setbit_t(p_iso, 11, p_BcTransLog_o->sys_tra_no, 6);
		setbit_t(p_iso, 12, p_BcTransLog_o->loc_trans_tm, 6);
		setbit_t(p_iso, 13, p_BcTransLog_o->loc_trans_dt, 4);
		setbit_t(p_iso, 14, "4912", 4);
		if(memcmp(p_BcTransLog->resp_cd, "00", 2) == 0)
		{
			setbit_t(p_iso, 15, p_BcTransLog->settle_dt, 4);
    		setbit_t(p_iso, 54, p_BcTransLog->addtnl_amt, 40);
			rtrim(p_BcTransLog->auth_id_resp_cd);
			if(strlen(p_BcTransLog->auth_id_resp_cd)>0)setbit_t(p_iso, 38, p_BcTransLog->auth_id_resp_cd, 6);
		}
		else
		{
	        glb_GetLocalTimeString(tmpBuf,5,"%m%d");
	        setbit_t(p_iso, 15, tmpBuf, 4);
		}
		setbit_t(p_iso, 18, p_BcTransLog_o->mchnt_tp, 4);
		setbit_t(p_iso, 25, "00", 4);
		setbit_t(p_iso, 32, p_BcTransLog_o->fld32_ins_id_cd, 8);
		setbit_t(p_iso, 33, p_BcTransLog_o->fwd_ins_id_cd, 8);
		setbit_t(p_iso, 37, p_BcTransLog_o->retri_ref_no, 12);
		setbit_t(p_iso, 41, p_BcTransLog_o->term_id, 8);
		setbit_t(p_iso, 42, p_BcTransLog_o->mchnt_cd, 15);
    	setbit_t(p_iso, 49, p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd, 3));
        setbit_t(p_iso, 60, p_BcTransLog_o->encrypt_info, 14);
    	setbit_t(p_iso, 128, "00000000", 8);
    }
    else
    {
		setbitoff(p_iso, 22);
		setbitoff(p_iso, 26);
		setbitoff(p_iso, 35);
		setbitoff(p_iso, 36);
		setbitoff(p_iso, 43);
		setbitoff(p_iso, 52);
		setbitoff(p_iso, 53);
        glb_GetLocalTimeString(tmpBuf,5,"%m%d");
        setbit_t(p_iso, 15, tmpBuf, 4);
    }
    setbit_t(p_iso, 0, "0210", 4);
    setbit_t(p_iso, 39, resp_code+4, 2); 
    setbit_t(p_iso, 100, "00005500", 8);
    BC_SetMacKey(p_tiBcInsInf->key_index, p_tiBcInsInf->key_1);
  	BC_SendMonitor_iso("1", "0", p_tiBcInsInf->line_nm, p_iso, p_tiBcTransInf);
    BC_SendMessage(p_iso, p_tiBcInsInf->line_nm);
    return 0;
}

//POSP�ظ����ѳ���
int POSP_RespondExpendRollBack(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf)
{
	char errInfo[200];
	char tmpBuf[255];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //from DB;
    ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf;
    if	( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
  	{
		sprintf(errInfo, "����ISO8583������[%s]ʧ��!", p_tiBcInsInf->packet_type);
		ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
    	return 1;
  	}
    setbit_t(p_iso, 0, "0430", 4);
    setbitoff(p_iso, 22);
	setbit_t(p_iso, 15, p_BcTransLog->settle_dt, 4); 
    setbitoff(p_iso, 38);
    setbitoff(p_iso, 43);
    setbitoff(p_iso, 90);
    if(resp_code != 0)setbit_t(p_iso, 39, resp_code+4, 2); 
    setbit_t(p_iso, 100, "00005500", 8);
	setbit_t(p_iso, 128, "00000000", 8);
    BC_SetMacKey(p_tiBcInsInf->key_index, p_tiBcInsInf->key_1);
  	BC_SendMonitor_iso("1", "0", p_tiBcInsInf->line_nm, p_iso, p_tiBcTransInf);
    BC_SendMessage(p_iso, p_tiBcInsInf->line_nm);
    return 0;
}

//POSP�ظ����ѳ�������
int POSP_RespondExpendUndoRollBack(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf)
{
	char errInfo[200];
	char tmpBuf[255];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //from DB;
    ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf;
    if	( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
  	{
		sprintf(errInfo, "����ISO8583������[%s]ʧ��!", p_tiBcInsInf->packet_type);
		ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
    	return 1;
  	}
    setbit_t(p_iso, 0, "0430", 4);
    setbitoff(p_iso, 22);
	setbit_t(p_iso, 15, p_BcTransLog->settle_dt, 4); 
    setbitoff(p_iso, 38);
    setbitoff(p_iso, 43);
    setbitoff(p_iso, 90);
    if(resp_code != 0)setbit_t(p_iso, 39, resp_code+4, 2); 
    setbit_t(p_iso, 100, "00005500", 8);
	setbit_t(p_iso, 128, "00000000", 8);
    BC_SetMacKey(p_tiBcInsInf->key_index, p_tiBcInsInf->key_1);
  	BC_SendMonitor_iso("1", "0", p_tiBcInsInf->line_nm, p_iso, p_tiBcTransInf);
    BC_SendMessage(p_iso, p_tiBcInsInf->line_nm);
    return 0;
}

//ȡ���ջ�����Ϣ
int RCV_Institution(ti_bc_ins_inf_def *p_tiBcInsInf, char *ins_id_cd, char *resp_code, char *errInfo)
{
  	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
	if( 0!= BC_Search_ins_inf(resp_code, p_tiBcInsInf, ins_id_cd))
	{
		strcpy(resp_code, "N00096");
		sprintf(errInfo, "��ѯ���ջ���(ins_id_cd:%s)��Ϣʧ��.");
		return 1;
	}
	if(p_tiBcInsInf->link_tp[0] == '1') //������
	{
		rtrim(p_tiBcInsInf->line_nm);
		if( IsReady( p_tiBcInsInf->line_nm )<0 ) //��·������
		{
			strcpy(resp_code, "N00091");
			sprintf(errInfo, "��·[%s]�����á�", p_tiBcInsInf->line_nm);
			return 2;
		}
  	}
	return 0;
} 

//��ȡ�����̻���Ϣ
int TradeMerchant(char *resp_code, char *errInfo, int flag)
{
  	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
  	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
  	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf = &g_tiBcQsMchntInf; 
  	ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf = &g_tiBcMchntTermChargeInf;

  	int sqlCode;
	ti_bc_mt_flow_id_def tiBcMTFlowId ;
	if(flag)
	{
		//���̻�ת����Ϣ��
		strcpy(p_tiBcMchntTermChargeInf->mchnt_tp, p_BcTransLog->mchnt_tp);
		strcpy(p_tiBcMchntTermChargeInf->term_id , p_BcTransLog->term_id);
		strcpy(p_tiBcMchntTermChargeInf->mchnt_cd, p_BcTransLog->mchnt_cd);
		strcpy(p_tiBcMchntTermChargeInf->rcv_ins_id_cd, p_BcTransLog->rcv_ins_id_cd);
		tiBcMchntTermChargeInfOpr(GLB_DB_SELECT1,  p_tiBcMchntTermChargeInf, &sqlCode);
		if( SQL_ERD_NONE_RECORD == sqlCode )
		{
			sprintf(errInfo, "��ѯת���̻���Ϣ������.mchnt_tp[%s],term_id[%s],mchnt_cd[%s],rcv_ins_id_cd[%s]", p_BcTransLog->mchnt_tp, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd, p_BcTransLog->rcv_ins_id_cd);
			strcpy(resp_code, "N00096");
			return 1;
		}else if (0 != sqlCode )
		{
			sprintf(errInfo, "��ѯת���̻���Ϣʧ�ܣ�");
			strcpy(resp_code, "N00096");
			return 2;
		}
		strcpy(p_BcTransLog_o->center_term_id, p_tiBcMchntTermChargeInf->term_id_c);
		strcpy(p_BcTransLog_o->center_mchnt_cd, p_tiBcMchntTermChargeInf->mchnt_cd_c);
		strcpy(p_BcTransLog->center_term_id, p_BcTransLog_o->center_term_id);
		strcpy(p_BcTransLog->center_mchnt_cd, p_BcTransLog_o->center_mchnt_cd);
	}
    
	//��ȡת����ˮ��
	memset(&tiBcMTFlowId, 0, sizeof(ti_bc_mt_flow_id_def));
	strcpy(tiBcMTFlowId.term_id   ,  p_BcTransLog->center_term_id);
	strcpy(tiBcMTFlowId.mchnt_cd  ,  p_BcTransLog->center_mchnt_cd);
	if(0 != tiBcMTFlowIdOpr(GLB_DB_SELECT1, &tiBcMTFlowId, &sqlCode))
	{
		sprintf(errInfo, "��ȡת����ˮ��ʧ��,�����̻���:[%s].�����ն˺�[%s],rcv_ins_id_cd[%s].", p_BcTransLog->center_mchnt_cd, p_BcTransLog->center_term_id, p_BcTransLog->rcv_ins_id_cd);
		strcpy(resp_code, "N00096");
		return 3;
	}
	strcpy(p_BcTransLog->center_sys_tra_no, tiBcMTFlowId.sys_tra_no);
	strcpy(p_BcTransLog_o->center_sys_tra_no, tiBcMTFlowId.sys_tra_no);
	
	//��ѯ�����̻���Ϣ
	strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->center_mchnt_cd);
	strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->center_term_id);
	strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog->rcv_ins_id_cd);
	tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
	if ( 0 != sqlCode )
	{
		sprintf(errInfo, "��ȡ�����̻���Ϣʧ��,�����̻���:[%s].�����ն˺�[%s],rcv_ins_id_cd[%s].", p_BcTransLog->center_mchnt_cd, p_BcTransLog->center_term_id, p_BcTransLog->rcv_ins_id_cd);
		strcpy(resp_code, "N00096");
		return 4;
	}	
    return 0;
}

//ȡ���ͻ�����Ϣ��У��Mac
int FWD_Institution_CheckMac(ti_bc_ins_inf_def *p_tiBcInsInf, char *ins_id_cd, char *resp_code, char *errInfo, int flag)
{
	int sqlCode;
	strcpy(p_tiBcInsInf->ins_id_cd, ins_id_cd);
	dcs_debug(0, 0, "<FILE:%s,LINE:%d><BusZ2210>��ѯ����(ins_id_cd:%s)��Ϣ !", __FILE__, __LINE__, p_tiBcInsInf->ins_id_cd);
	tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
	if ( 0 != sqlCode )
    {
	    // ��ѯ������Ϣʧ��
        strcpy(resp_code, "N000D1");
        sprintf(errInfo, "��ѯ����(ins_id_cd:%s)��Ϣʧ��", p_tiBcInsInf->ins_id_cd); 
        return 1;
    }
    if(flag == 0) return 0;
    //У�������Կ
    if ( *p_tiBcInsInf->key_type != '0' )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>У�����(ins_id_cd:%s)��Կ��Ϣ!",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
        if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index, p_tiBcInsInf->key_1, g_Rcv_Buffer+4, g_RcvBufferSize-4))
        {
	     	strcpy(resp_code, "N000A0");
	     	sprintf(errInfo, "У�����(ins_id_cd:%s)��Կ��Ϣ���� !", p_tiBcInsInf->ins_id_cd);
            return 2;
        }
    }
	return 0;    
}

int ProcessErrInfo(char *resp_code, char *errInfo, char *fileName, int lineNo, int flag)
{
	int iRet, sqlCode;
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
	strcpy(p_BcTransLog_o->trans_flag, "Y");
	strcpy(p_BcTransLog_o->is_over, "1");
	strcpy(p_BcTransLog_o->resp_cd,resp_code+4);
	strcpy(p_BcTransLog_o->fld_28 ,resp_code);
	strcpy(p_BcTransLog_o->bc_settle_in, "0");
	//��¼��־
	dcs_log(0, 0,"<FILE:%s,LINE:%d>-%s", fileName, lineNo, errInfo);
	//���ͼ����Ϣ
	BC_SendExeception("03", "����, %s", errInfo);
  	//�쳣��¼���
  	if( 0 > BC_exp_log(resp_code+2, errInfo, g_Rcv_Buffer, g_RcvBufferSize) )
  	{
    	BC_SendExeception("03","����, ��¼�쳣��ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]", p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
  	}
 	if(flag == 0)return;
  	//��¼���
  	if(flag == 1) 
  	{
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if(0 != sqlCode)
		{
			sprintf(errInfo, "���½�����ˮ��ʧ�ܣ���ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]", p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
			strcpy(resp_code, "N00096");
			ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		}
	}
	else
	{
	  	if ( 0 != BC_InsertIntoDB(p_BcTransLog_o, &sqlCode))
	  	{
	  		if(sqlCode == SQL_ERD_UNIKEY)
			{
				sprintf(errInfo, "������ˮ���ظ�����ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]", p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
				strcpy(resp_code, "N00094");
			}
	  		else
			{
				sprintf(errInfo, "�彻����ˮ��ʧ�ܣ���ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]", p_BcTransLog_o->sys_tra_no, p_BcTransLog_o->term_id, p_BcTransLog_o->mchnt_cd);
				strcpy(resp_code, "N00096");
			}
			ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		}
	} 
} 

//�˵����
int BillInDB(char *resp_code, char *errInfo)
{
  	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
  	tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
  	int sqlCode;
  	BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
  	tlBcBillLogOpr(GLB_DB_INSERT1, p_tlBcBillLog, &sqlCode);
  	if(SQL_ERD_UNIKEY == sqlCode ) //�˵��Ѵ���
  	{
    	tlBcBillLogOpr(GLB_DB_SELECT2, p_tlBcBillLog, &sqlCode);
    	if ( 0 == sqlCode )
    	{   
		  	//�������˸��˵����гɹ����׻�������� NC0001
      		if ( '1'==*p_tlBcBillLog->bc_settle_in )
      		{
      			strcpy(resp_code, "N00094");
		  		sprintf(errInfo, "�ظ�����(fwd_ins_id_cd:%s, bill_id:%s)", p_BcTransLog->fwd_ins_id_cd, p_tlBcBillLog->bill_id);
      			return 1;
    		} 
      		else if ( '0'==*p_tlBcBillLog->bc_settle_in )
      		{
	      		BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
	      		strcpy(p_tlBcBillLog->bc_settle_in, " ");
	      		tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
        		if ( 0!= sqlCode )
        		{
	      			strcpy(resp_code, "N00096");
			  		sprintf(errInfo, "�����˵���ʧ��(fwd_ins_id_cd:%s, bill_id:%s)", p_BcTransLog->fwd_ins_id_cd, p_tlBcBillLog->bill_id);
	      			return 2;
        		}
        		return 0;
      		} 
      		else
      		{
      			strcpy(resp_code, "N00009");
      			return -1;
      		}            
		} 
		else
    	{
  			strcpy(resp_code, "N00096");
	  		sprintf(errInfo, "��ѯ�˵���ʧ��(fwd_ins_id_cd:%s, bill_id:%s)", p_BcTransLog->fwd_ins_id_cd, p_tlBcBillLog->bill_id);
	  		return 3;
    	}
  	}
  	else if ( 0 != sqlCode )
 	{
		strcpy(resp_code, "N00096");
  		sprintf(errInfo, "�˵����ʧ��(fwd_ins_id_cd:%s, bill_id:%s)", p_BcTransLog->fwd_ins_id_cd, p_tlBcBillLog->bill_id);
  		return 4;
  	} 
	return 0;
}

//��Ը������Ϣ
int QueryOriginaTransLog(char *resp_code, char *errInfo)
{
	tl_bc_trans_log_def BcTransLog_o;
  	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
  	tl_bc_trans_log_def *p_BcTransLog_db = &g_TransMsg_o.BcTransLog;
	tl_bc_trans_log_def *p_BcTransLog_o = &BcTransLog_o;
	int sqlCode;
	strcpy(p_BcTransLog_o->mchnt_cd, p_BcTransLog->mchnt_cd);      
	strcpy(p_BcTransLog_o->term_id, p_BcTransLog->term_id);       
	memcpy(p_BcTransLog_o->sys_tra_no, p_BcTransLog->originaDataElements + 4, 6);
	p_BcTransLog_o->sys_tra_no[6] = 0;    
	memcpy(p_BcTransLog_o->loc_trans_dt, p_BcTransLog->originaDataElements + 10, 4); 
	p_BcTransLog_o->loc_trans_dt[4] = 0;
	tlBcTransLogOpr(GLB_DB_SELECT11, p_BcTransLog_o, &sqlCode);
	if ( 0 != sqlCode )
	{
		strcpy(resp_code, "N00025");
  		sprintf(errInfo, "�Ҳ���ԭ����(mchnt_cd:[%s], term_id:[%s], originaDataElements[%s])", p_BcTransLog_o->mchnt_cd, p_BcTransLog_o->term_id, p_BcTransLog->originaDataElements);
  		return 1;
	}
	if (memcmp(p_BcTransLog_o->transmsn_dt_tm, p_BcTransLog->originaDataElements + 10, 10) != 0)
	{
		strcpy(resp_code, "N00025");
  		sprintf(errInfo, "ԭ���ײ�ƥ��(transmsn_dt_tm:[%s], originaDataElements[%s])", p_BcTransLog_o->transmsn_dt_tm, p_BcTransLog->originaDataElements);
  		return 1;
	}
   	strcpy(p_BcTransLog->area_cd, p_BcTransLog_o->area_cd);
 	memcpy(p_BcTransLog->retri_ref_no, p_BcTransLog_o->retri_ref_no, 12);
	memcpy(p_BcTransLog->auth_id_resp_cd, p_BcTransLog_o->auth_id_resp_cd, 6);
	strcpy(p_BcTransLog->center_mchnt_cd, p_BcTransLog_o->center_mchnt_cd);
	strcpy(p_BcTransLog->center_term_id, p_BcTransLog_o->center_term_id);
	strcpy(p_BcTransLog->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
	strcpy(p_BcTransLog_db->center_mchnt_cd, p_BcTransLog_o->center_mchnt_cd);
	strcpy(p_BcTransLog_db->center_term_id, p_BcTransLog_o->center_term_id);
	strcpy(p_BcTransLog_db->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);	
	memcpy(p_BcTransLog->originaDataElements+4, p_BcTransLog_o->center_sys_tra_no, 6);
	memcpy(p_BcTransLog->originaDataElements+10, p_BcTransLog_o->transmsn_dt_tm, 10);
	memset(p_BcTransLog->originaDataElements+20, '0', 22);
	if (p_BcTransLog_o->is_over[0] == '1') //���������
	{
		strcpy(p_BcTransLog->settle_dt, p_BcTransLog_o->settle_dt); 
		strcpy(p_BcTransLog_o->settle_dt, p_BcTransLog_o->settle_dt); 
		if ( p_BcTransLog_o->bc_settle_in[0] == '1' ) //���׳ɹ�
		{
			if( memcmp(p_BcTransLog_o->settle_dt, p_BcTransLog->loc_trans_dt, 4) == 0 || memcmp(p_BcTransLog_o->loc_trans_dt, p_BcTransLog->loc_trans_dt, 4) == 0) //���콻��
			{
				return 0;
			}
			else
			{
				strcpy(resp_code, "N00025");
		  		sprintf(errInfo, "�ǵ��콻�ף�(mchnt_cd:[%s],term_id:[%s],settle_dt[%s],loc_trans_dt[%s],originaDataElements[%s])", p_BcTransLog_o->mchnt_cd, p_BcTransLog_o->term_id, p_BcTransLog_o->settle_dt, p_BcTransLog->loc_trans_dt, p_BcTransLog->originaDataElements);
		  		return 2;
			}
		}
		else //����ʧ��,���ѳɹ�����
		{
			strcpy(resp_code, "N00022");
	  		sprintf(errInfo, "����ʧ��,���ѳɹ�����(mchnt_cd:[%s], term_id:[%s], originaDataElements[%s])", p_BcTransLog_o->mchnt_cd, p_BcTransLog_o->term_id, p_BcTransLog->originaDataElements);
			return -1;
		}
	}
	else
	{
		strcpy(resp_code, "N00022");
  		sprintf(errInfo, "ԭ����δ�������(mchnt_cd:[%s], term_id:[%s], originaDataElements[%s])", p_BcTransLog_o->mchnt_cd, p_BcTransLog_o->term_id, p_BcTransLog->originaDataElements);
  		return 2;
	}
}
