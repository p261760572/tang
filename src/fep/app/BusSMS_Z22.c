#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

//�յ�����ȷ�ϱ���
int SMS_SendReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso, char* flag)
{
	char errInfo[255],resp_code[7];
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
	int sqlCode;
	if( 0 != FWD_Institution_CheckMac(p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd, resp_code, errInfo, 1))
	{
		ProcessErrInfo(resp_code, errInfo, __FILE__, __LINE__, 0);
		return -1;
	}
	if ( 0 != BC_CheckIsoDataField(iso, "3,11,12,13,28") )
	{
		sprintf(errInfo, "У����ձ���0730ʧ�ܣ�"); 
		ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
    	return -1;
	}
	//�ж϶����Ƿ�ɹ�
	if ( 0 == memcmp(p_BcTransLog->fld_28,"N00000",6) )
	{
		strcpy(p_BcTransLog_o->flag_7, "1");
		//�������ݿ�
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if ( 0 != sqlCode )
		{
			sprintf(errInfo, "����BcTransLogʧ�ܣ�");
			ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
		}
		return 0;
	}
	InsTimeOutTBL(pTimeOutRec);
	return 0;
}

// ��ʱ����
int SMS_SendTimeOut()
{
    struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    int sqlCode;
    char errInfo[255];
	SMS_Send(0);
	if( --pTimeOutRec->iFlags <= 0 )
	{
		strcpy(p_BcTransLog_o->flag_7, "0");
		//�������ݿ�
		tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog_o, &sqlCode);
		if ( 0 != sqlCode )
		{
			sprintf(errInfo, "����BcTransLogʧ�ܣ�");
			ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
		}
    	sprintf(errInfo, "�Ѿ�����(%d)�ζ��ţ���δ���سɹ������ٴ���ñʽ���! ��ˮ��:[%s],�̻���:[%s].", SEND_SMS_TIMEOUT_TIMES, p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->mchnt_cd);
    	ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
		return 0;
	}
	InsTimeOutTBL(pTimeOutRec);
	return 0;
}

//���Ͷ���Ϣ
int SMS_Send(int flag)
{
	ti_bc_ins_inf_def tiBcInsInf, *p_tiBcInsInf;
	tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; // from db
	ti_bc_mchnt_inf_def *p_tiBcMchntInf, tiBcMchntInf;
	ISO_data *p_iso, iso;
	char errInfo[255];
	char tmpBuf[1024],tmpBuf1[1024];
	int sqlCode, len;
	p_iso = &iso;
	p_tiBcInsInf = &tiBcInsInf;
	p_tiBcMchntInf = &tiBcMchntInf;
  	memset(p_iso, 0, sizeof(ISO_data));
  	memset(p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
  	memset(p_tiBcMchntInf, 0, sizeof(ti_bc_mchnt_inf_def));
  	clearbit(p_iso);
	strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog_o->mchnt_cd);
    tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
    if ( 0 != sqlCode ) //û���ҵ��̻���Ϣ
    {
		sprintf(errInfo, "�̻��Ų�����!(mchnt_cd:%s,sqlcode=%d)", p_tiBcMchntInf->mchnt_cd, sqlCode);
		ProcessErrInfo("N00003", errInfo, __FILE__, __LINE__, 0);
    }
    if(p_tiBcMchntInf->send_msg_flag[0] != '1') return 0;
	strcpy(p_tiBcInsInf->ins_id_cd, "10000010");
	tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
	if ( 0 != sqlCode )// ��ѯ������Ϣʧ��
	{
		sprintf(errInfo, "��ѯ����(ins_id_cd:%s)��Ϣʧ��", p_tiBcInsInf->ins_id_cd); 
		ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 0);
		return 1;
	}
 	if( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
  	{
  		sprintf(errInfo, "���ñ�������[%s]ʧ��", p_tiBcInsInf->packet_type);
		ProcessErrInfo("N000D1", errInfo, __FILE__, __LINE__, 0);
  	}
	setbit_t(p_iso, 0 , "0720", 4); 
	setbit_t(p_iso, 3 , "820001", 6);
	setbit_t(p_iso, 5 , (unsigned char *)p_BcTransLog_o->fld_5, 12);
	setbit_t(p_iso, 11, (unsigned char *)p_BcTransLog_o->sys_tra_no, 6);
	setbit_t(p_iso, 12, (unsigned char *)p_BcTransLog_o->loc_trans_tm, 6);
	setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog_o->loc_trans_dt, 4);
	setbit_t(p_iso, 33, "10000010", 8);
	setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog_o->mchnt_cd, 20);
	sprintf(tmpBuf, "����[%s]%s,������%s", rtrim(p_tiBcMchntInf->mchnt_cn_abbr), rtrim(p_BcTransLog_o->fld_48), rtrim(p_BcTransLog_o->fld_45)+15);
	sprintf(tmpBuf1, ".���[%.2f]Ԫ,�ۿ�ɹ�", atoi(p_BcTransLog_o->trans_at)/100.0);
	len = MyStrLen(tmpBuf) + MyStrLen(tmpBuf1);
	if(len>70)
	{
		len = MyLeftPos(tmpBuf, 63 - MyStrLen(tmpBuf1));
		memcpy(tmpBuf + len, "..", 2);
		len += 2;
		memcpy(tmpBuf + len, tmpBuf1, strlen(tmpBuf1));
		len = len + strlen(tmpBuf1);
	}
	else
	{
		memcpy(tmpBuf + strlen(tmpBuf), tmpBuf1, strlen(tmpBuf1));
		len = strlen(tmpBuf) + strlen(tmpBuf1);
	}
  	setbit_t(p_iso, 48, (unsigned char *)tmpBuf, len);     
  	setbit_t(p_iso, 61, "10001", 5);     
	setbit_t(p_iso, 64, "00000000", 8); 
	if(flag)
	{
		if(0 != BC_InsertTimeOut1(SEND_SMS_TIMEOUT_TIMES, gs_myFid, g_iSrcFoldId, 180, p_iso, &g_TransMsg_o))
		{
			sprintf(errInfo, "�峬ʱ��ʧ��!");
			ProcessErrInfo("N00096", errInfo, __FILE__, __LINE__, 0);
		}
	}
	BC_SetMacKey(p_tiBcInsInf->key_index, p_tiBcInsInf->key_1);
	BC_SendMonitor_iso("0","0", p_tiBcInsInf->line_nm, p_iso, &g_tiBcTransInf) ;
	return BC_SendMessage(p_iso, p_tiBcInsInf->line_nm) ;
}

int MyStrLen(char *buff)
{
	int i,j;
	for(i=0, j=0; buff[j] != '\0'; i++)
	{
		if(buff[j++] & 0x80) 
			j++;
	}
	return i;
}

int MyLeftPos(char *buff, int size)
{
	int i,j;
	for(i=0,j=0; i<size && buff[j] !='\0'; i++)
	{
		if(buff[j++] & 0x80) j++;
	}	
	return j;
}
