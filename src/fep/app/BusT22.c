#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

/*�ɷ�����*/
#define SETBIT_T(ISO,INDEX,DATA,DATA_O,LEN) if( 0 != *(DATA))setbit_t(ISO, INDEX, (unsigned char *)DATA, LEN); else setbit_t(ISO, INDEX, (unsigned char *)DATA_O, LEN);
#define SETBIT_N(ISO,INDEX,DATA,LEN) if( 0 != *(DATA))setbit_t(ISO, INDEX, (unsigned char *)DATA, LEN);

//���ͽɷ�Ӧ����ն�
int BusT22_send_term_resp(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog )
{
    ISO_data iso;
    ISO_data *p_iso = &iso;

    //���͸��ն�Ӧ��
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(&iso);

    setbit_t(p_iso, 0, "0210", 4); 
    SETBIT_N(p_iso, 3, p_BcTransLog->proc_cd, 6);
    SETBIT_N(p_iso, 4, (unsigned char *)p_BcTransLog->trans_at, 12);

    SETBIT_N(p_iso, 11, p_BcTransLog->sys_tra_no, 6);
    SETBIT_N(p_iso, 12, p_BcTransLog->loc_trans_tm, 6);
    SETBIT_N(p_iso, 13, p_BcTransLog->loc_trans_dt, 4);
    SETBIT_N(p_iso, 22, p_BcTransLog->pos_entry_md_cd, 3);
    SETBIT_N(p_iso, 25, p_BcTransLog->pos_cond_cd, 2);
    SETBIT_N(p_iso, 32, (unsigned char *)p_BcTransLog->fld32_ins_id_cd, 8);
    SETBIT_N(p_iso, 33, (unsigned char *)p_BcTransLog->fwd_ins_id_cd, 8);
    //35
    //36
    //setbit_t(p_iso, 37, (unsigned char *)p_BcTransLog->retri_ref_no, 12);
    setbit_t(p_iso, 39, (unsigned char *)resp_cd, 2);
    SETBIT_N(p_iso, 41, (unsigned char *)p_BcTransLog->term_id, 8);
    SETBIT_N(p_iso, 42, (unsigned char *)p_BcTransLog->mchnt_cd, 15);
    SETBIT_N(p_iso, 43, (unsigned char *)p_BcTransLog->card_accptr_nm_loc, 40);
    setbit_t(p_iso, 49, (unsigned char *)"156", 3);

    //charge mk 
    setbit_t(p_iso,64,"00000000", 8 );

    //�����ն���Կ
    BC_SetTermMacKey2(p_BcTransLog->term_id);

    //���ͼ����Ϣ
    BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,p_iso);

    //�����ն�Ӧ��
    return BC_SendMessageToTerm(p_iso);
}

//������Ӫ�̲�ѯ����  -- ��Ҫ�̻���Ϣ
int BusT22_send_search_req(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog, ti_bc_mchnt_inf_def *p_tiBcMchntInf )
{
    int iRet = -1;
    int sqlCode;
    char buff[256];
    ISO_data iso;
    ISO_data *p_iso = &iso;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ

    strcpy(resp_cd,"96");

    //��ѯ����(��Ӫ��)����·����
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>start to call tiBcInsInfOpr(ins_id_cd:%s) ��ѯ����(��Ӫ��)����·���� !",__FILE__,__LINE__, p_tiBcMchntInf->verifi_id);
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcMchntInf->verifi_id);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // ��ѯ����(��Ӫ��)��·��Ϣʧ��
        strcpy(resp_cd,"96");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_search_req>call tiBcInsInfOpr return failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","��ѯ����(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>start to call BC_SetIsoMode(line_nm:%s) ���û���(��Ӫ��)����·���� !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
    //������Ӫ����·
    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>BC_SetIsoMode(line_nm:%s) ���û���(��Ӫ��)����·����ʧ�� !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
        strcpy(resp_cd,"96");
        BC_SendExeception("03","���û���(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    //set bit
    memset(p_iso,0,sizeof(ISO_data));
    setbit_t(p_iso, 0, "0100", 4);
    setbit_t(p_iso, 3, "310000", 6);
    SETBIT_T(p_iso, 11, p_BcTransLog->sys_tra_no,0, 6);
    SETBIT_T(p_iso, 12, p_BcTransLog->loc_trans_tm,glb_GetLocalTimeString(buff,7,"%H%M%S"), 6);
    SETBIT_T(p_iso, 13, p_BcTransLog->loc_trans_dt,glb_GetLocalTimeString(buff,7,"%Y%m")+2, 4);
    setbit_t(p_iso, 25, "87", 4);
    SETBIT_T(p_iso, 32, p_BcTransLog->fld32_ins_id_cd, g_center_ins_id_cd, 8);
    SETBIT_T(p_iso, 33, p_BcTransLog->fwd_ins_id_cd, g_center_ins_id_cd, 8);
    SETBIT_T(p_iso, 41, p_BcTransLog->term_id, "        ", 8);
    SETBIT_T(p_iso, 42, p_BcTransLog->mchnt_cd, "               ", 15);

    //48
    //charge mk 
    setbit_t(p_iso, 128, (unsigned char *)"00000000", 8);

    //������Ӫ����Կ��Ϣ
    BC_SetRcvMacKey(p_tiBcInsInf); //����(��Ӫ��)
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>start to call BC_SendMessage ����Ӫ�̷��Ͳ�ѯ��Ϣ��",__FILE__,__LINE__);

    //�峬ʱ��
    iRet = BC_InsertTimeOut1(BC_GetFolderIdByName(p_tiBcInsInf->line_nm),g_iSrcFoldId,I_TIMEOUT,p_iso);
    if ( 0 != iRet )
    {
        strcpy(resp_cd,"96");
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>���볬ʱ��ʧ��",__FILE__,__LINE__);
        BC_SendExeception("03","���棬���볬ʱ��ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>���볬ʱ��ɹ�",__FILE__,__LINE__);

    //���Ͳ�ѯ����
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);
    if ( 0!= iRet ) BC_DeleteTimeOut1(&g_TimeOutRec);

    //���ͼ����Ϣ
    BC_SendMonitor("0","4",p_BcTransLog->trans_chnl,p_iso);
    strcpy(resp_cd,"00");
    return iRet;
}

//������������
int BusT22_send_consume_req(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog,ti_bc_mchnt_inf_def *p_tiBcMchntInf)
{
    int iRet;
    ISO_data iso;
    ISO_data *p_iso = &iso;
    char buff[256];
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ

    strcpy(resp_cd,"96");

    //��ѯ��������·����
    iRet = BC_Search_ins_inf(p_tiBcInsInf, p_tiBcMchntInf, p_BcTransLog->pri_acct_no, p_BcTransLog->pos_entry_md_cd);
    if ( 0 != iRet )
    {
        // ��ѯ������·��Ϣʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_consume_req>call BC_Search_ins_inf failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","��ѯ����(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        BC_SendExeception("03","���û���(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    //set bit
    memset(p_iso,0,sizeof(ISO_data));
    setbit_t(p_iso, 0, "0200", 4);
    setbit_t(p_iso, 3, "190000", 6);
    setbit_t(p_iso, 4, (unsigned char *)p_BcTransLog->trans_at, 12);
    setbit_t(p_iso, 7, (unsigned char *)glb_GetLocalTimeString(buff,5,"%m%d%H%M%S"), 10);

    SETBIT_T(p_iso, 11, p_BcTransLog->sys_tra_no,0, 6);
    SETBIT_T(p_iso, 12, p_BcTransLog->loc_trans_tm,glb_GetLocalTimeString(buff,7,"%H%M%S"), 6);
    SETBIT_T(p_iso, 13, p_BcTransLog->loc_trans_dt,glb_GetLocalTimeString(buff,7,"%Y%m")+2, 4);
    SETBIT_T(p_iso, 22, p_BcTransLog->pos_entry_md_cd, "021", 3);
    setbit_t(p_iso, 25, "81", 2);

    SETBIT_T(p_iso, 32, (unsigned char *)p_BcTransLog->fld32_ins_id_cd, g_center_ins_id_cd, 8);
    SETBIT_T(p_iso, 33, (unsigned char *)p_BcTransLog->fwd_ins_id_cd, g_center_ins_id_cd, 8);
    //35
    //36
    setbit_t(p_iso, 37, (unsigned char *)p_BcTransLog->retri_ref_no, 12);
    setbit_t(p_iso, 41, (unsigned char *)p_BcTransLog->term_id, 8);
    setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog->mchnt_cd, 15);
    setbit_t(p_iso, 43, (unsigned char *)p_BcTransLog->card_accptr_nm_loc, 40);
    setbit_t(p_iso, 49, (unsigned char *)"156", 3);

    //charge mk 
    setbit_t(p_iso, 128, (unsigned char *)"00000000", 8);

    //���û�����Կ��Ϣ
    BC_SetRcvMacKey(p_tiBcInsInf); //����

    //�峬ʱ��
    iRet = BC_InsertTimeOut1(BC_GetFolderIdByName(p_tiBcInsInf->line_nm),g_iSrcFoldId,I_TIMEOUT,p_iso);
    if ( 0 != iRet )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT22_send_consume_req>���볬ʱ��ʧ��",__FILE__,__LINE__);
        BC_SendExeception("03","���棬���볬ʱ��ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_consume_req>���볬ʱ��ɹ�",__FILE__,__LINE__);

    //���ͽ�����Ϣ
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);
    if ( 0!= iRet ) BC_DeleteTimeOut1(&g_TimeOutRec);

    //���ͼ����Ϣ
    BC_SendMonitor("0","2",p_BcTransLog->trans_chnl,p_iso);
    strcpy(resp_cd,"00");
    return iRet;
}

//������Ӫ���������� -- ��Ҫ�̻���Ϣ
int BusT22_send_pay_req(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog, ti_bc_mchnt_inf_def *p_tiBcMchntInf )
{
    int iRet = -1;
    int sqlCode;
    char buff[256];
    ISO_data iso;
    ISO_data *p_iso = &iso;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    strcpy(resp_cd,"96");

    //��ѯ����(��Ӫ��)����·����
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>start to call tiBcInsInfOpr(ins_id_cd:%s) ��ѯ����(��Ӫ��)����·���� !",__FILE__,__LINE__, p_tiBcMchntInf->verifi_id);
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcMchntInf->pay_id);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // ��ѯ����(��Ӫ��)��·��Ϣʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_pay_req>call tiBcInsInfOpr return failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","��ѯ����(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    //������Ӫ����·��Ϣ
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>start to call BC_SetIsoMode(line_nm:%s) ���û���(��Ӫ��)����·���� !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>BC_SetIsoMode(line_nm:%s) ���û���(��Ӫ��)����·����ʧ�� !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
        BC_SendExeception("03","���û���(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    //set bit
    memset(p_iso,0,sizeof(ISO_data));
    setbit_t(p_iso, 0, "0100", 4);
    setbit_t(p_iso, 3, "310000", 6);
    SETBIT_T(p_iso, 11, p_BcTransLog->sys_tra_no,0, 6);
    SETBIT_T(p_iso, 12, p_BcTransLog->loc_trans_tm,glb_GetLocalTimeString(buff,7,"%H%M%S"), 6);
    SETBIT_T(p_iso, 13, p_BcTransLog->loc_trans_dt,glb_GetLocalTimeString(buff,7,"%Y%m")+2, 4);
    setbit_t(p_iso, 25, "87", 4);
    SETBIT_T(p_iso, 32, p_BcTransLog->fld32_ins_id_cd, g_center_ins_id_cd, 8);
    SETBIT_T(p_iso, 33, p_BcTransLog->fwd_ins_id_cd, g_center_ins_id_cd, 8);
    SETBIT_T(p_iso, 41, p_BcTransLog->term_id, "        ", 8);
    SETBIT_T(p_iso, 42, p_BcTransLog->mchnt_cd, "               ", 15);

    //48
    //charge mk 
    setbit_t(p_iso, 128, (unsigned char *)"00000000", 8);

    //���û�����·��Ϣ
    BC_SetRcvMacKey(p_tiBcInsInf); //����(��Ӫ��)
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>start to call BC_SendMessage ����Ӫ�̷������˽��ס�",__FILE__,__LINE__);

    //�峬ʱ��
    iRet = BC_InsertTimeOut1(BC_GetFolderIdByName(p_tiBcInsInf->line_nm),g_iSrcFoldId,I_TIMEOUT,p_iso);
    if ( 0 != iRet )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>���볬ʱ��ʧ��",__FILE__,__LINE__);
        BC_SendExeception("03","���棬���볬ʱ��ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>���볬ʱ��ɹ�",__FILE__,__LINE__);

    //������������
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);
    if ( 0!= iRet ) BC_DeleteTimeOut1(&g_TimeOutRec);
    else strcpy(resp_cd,"00");

    //���ͼ��
    BC_SendMonitor("0","1",p_BcTransLog->trans_chnl,p_iso);
    return iRet;
}


//���ѳ���
int BusT22_send_consume_reversal(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog,ti_bc_mchnt_inf_def *p_tiBcMchntInf)
{
    int iRet;
    ISO_data iso;
    ISO_data *p_iso = &iso;
    char buff[256];
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ

    strcpy(resp_cd,"96");

    //��ѯ��������·����
    iRet = BC_Search_ins_inf(p_tiBcInsInf, p_tiBcMchntInf, p_BcTransLog->pri_acct_no, p_BcTransLog->pos_entry_md_cd);
    if ( 0 != iRet )
    {
        // ��ѯ������·��Ϣʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_consume_reversal>call BC_Search_ins_inf failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","��ѯ����(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    //������·
    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        BC_SendExeception("03","���û���(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    //set bit
    memset(p_iso,0,sizeof(ISO_data));
    setbit_t(p_iso, 0, "0400", 4);
    setbit_t(p_iso, 3, "190000", 6);
    setbit_t(p_iso, 4, (unsigned char *)p_BcTransLog->trans_at, 12);
    setbit_t(p_iso, 7, (unsigned char *)glb_GetLocalTimeString(buff,11,"%m%d%H%M%S"), 10);

    SETBIT_T(p_iso, 11, p_BcTransLog->sys_tra_no,0, 6);
    SETBIT_T(p_iso, 12, p_BcTransLog->loc_trans_tm,glb_GetLocalTimeString(buff,7,"%H%M%S"), 6);
    SETBIT_T(p_iso, 13, p_BcTransLog->loc_trans_dt,glb_GetLocalTimeString(buff,7,"%Y%m")+2, 4);
    SETBIT_T(p_iso, 22, p_BcTransLog->pos_entry_md_cd, "021", 3);
    setbit_t(p_iso, 25, "81", 2);

    SETBIT_T(p_iso, 32, (unsigned char *)p_BcTransLog->fld32_ins_id_cd, g_center_ins_id_cd, 8);
    SETBIT_T(p_iso, 33, (unsigned char *)p_BcTransLog->fwd_ins_id_cd, g_center_ins_id_cd, 8);
    //35
    //36
    setbit_t(p_iso, 37, (unsigned char *)p_BcTransLog->retri_ref_no, 12);
    setbit_t(p_iso, 41, (unsigned char *)p_BcTransLog->term_id, 8);
    setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog->mchnt_cd, 15);
    setbit_t(p_iso, 43, (unsigned char *)p_BcTransLog->card_accptr_nm_loc, 40);
    setbit_t(p_iso, 49, (unsigned char *)"156", 3);

    //charge mk 
    setbit_t(p_iso, 128, (unsigned char *)"00000000", 8);

    //���û�����Կ��Ϣ
    BC_SetRcvMacKey(p_tiBcInsInf); //����

    //���ͳ���
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);

    //���ͼ����Ϣ
    BC_SendMonitor("0","0",p_BcTransLog->trans_chnl,p_iso);
    strcpy(resp_cd,"00");
    return 0; //iRet;
}

//���˳��� -- ��Ҫ�̻���Ϣ
int BusT22_send_pay_reversal(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog, ti_bc_mchnt_inf_def *p_tiBcMchntInf )
{
    int iRet = -1;
    int sqlCode  ;
    char buff[256];
    ISO_data iso;
    ISO_data *p_iso = &iso;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    strcpy(resp_cd,"96");

    //��ѯ����(��Ӫ��)����·����
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_reversal>start to call tiBcInsInfOpr(ins_id_cd:%s) ��ѯ����(��Ӫ��)����·���� !",__FILE__,__LINE__, p_tiBcMchntInf->verifi_id);
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcMchntInf->pay_id);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // ��ѯ����(��Ӫ��)��·��Ϣʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_pay_reversal>call tiBcInsInfOpr return failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","��ѯ����(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_reversal>start to call BC_SetIsoMode(line_nm:%s) ���û���(��Ӫ��)����·���� !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_reversal>BC_SetIsoMode(line_nm:%s) ���û���(��Ӫ��)����·����ʧ�� !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
        BC_SendExeception("03","���û���(��Ӫ��)��·��Ϣʧ��,����id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    //set bit
    memset(p_iso,0,sizeof(ISO_data));
    setbit_t(p_iso, 0, "0420", 4);
    setbit_t(p_iso, 3, "310000", 6);
    SETBIT_T(p_iso, 11, p_BcTransLog->sys_tra_no,0, 6);
    SETBIT_T(p_iso, 12, p_BcTransLog->loc_trans_tm,glb_GetLocalTimeString(buff,7,"%H%M%S"), 6);
    SETBIT_T(p_iso, 13, p_BcTransLog->loc_trans_dt,glb_GetLocalTimeString(buff,7,"%Y%m")+2, 4);
    setbit_t(p_iso, 25, "87", 4);
    SETBIT_T(p_iso, 32, p_BcTransLog->fld32_ins_id_cd, g_center_ins_id_cd, 8);
    SETBIT_T(p_iso, 33, p_BcTransLog->fwd_ins_id_cd, g_center_ins_id_cd, 8);
    SETBIT_T(p_iso, 41, p_BcTransLog->term_id, "        ", 8);
    SETBIT_T(p_iso, 42, p_BcTransLog->mchnt_cd, "               ", 15);

    //48
    //charge mk 
    setbit_t(p_iso, 128, (unsigned char *)"00000000", 8);

    //������Ӫ����Կ��Ϣ
    BC_SetRcvMacKey(p_tiBcInsInf); //����(��Ӫ��)
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_reversal>start to call BC_SendMessage ����Ӫ�̷������˽��ס�",__FILE__,__LINE__);

    //�������˳���
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);

    //���ͼ����Ϣ
    BC_SendMonitor("0","0",p_BcTransLog->trans_chnl,p_iso);
    strcpy(resp_cd,"00");
    return iRet;
}


//�����ն�����
//�����̻�������Ϣ��1������Ӫ�̷�����ѯ����    
//                  2�����跢�Ͳ�ѯ���� -> ֱ�����������Ϳۿ����� 
int BusT2210(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet = -1  ;
    int sqlCode    ;
    char resp_cd[3];

    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    strcpy(p_BcTransLog->flag_lock, "0");

    BC_SendMonitor2("0","3",p_BcTransLog);

    // ��ȡ�̻���Ϣ
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>start to call BC_Search_mchnt_inf to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    iRet = BC_Search_mchnt_inf(p_tiBcMchntInf, p_BcTransLog);
    if ( 0 != iRet )
    {
        //û���ҵ��̻���Ϣ
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

        //���
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = BC_InsertIntoDB(p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>BC_InsertIntoDB ���ʧ��!",__FILE__,__LINE__);
            BC_SendExeception("03","���棬�ɷ��������ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>BC_InsertIntoDB ���ɹ�",__FILE__,__LINE__); 

        //����ʧ��        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>start to call BusT22_send_term_resp!",__FILE__,__LINE__);
        //BC_SendMonitor2("1","0",p_BcTransLog);
        return BusT22_send_term_resp("03", p_BcTransLog );
    }

    // �ҵ����̻���Ϣ����ֵ
    strcpy(p_BcTransLog->mchnt_cd, p_tiBcMchntInf->mchnt_cd);

    // �ж��Ƿ����ѯ
    if ( '1' == *p_tiBcMchntInf->verifi_flag )
    {
        //���
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
        strcpy(p_BcTransLog->flag_verifi, "1");
        iRet = BC_InsertIntoDB(p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","���棬�ɷ��������ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>BC_InsertIntoDB ���ʧ��!",__FILE__,__LINE__);
            //BC_SendMonitor2("1","0",p_BcTransLog);
            return BusT22_send_term_resp("96", p_BcTransLog );
        }

        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>BC_InsertIntoDB ���ɹ�",__FILE__,__LINE__); 

        //������Ӫ�̲�ѯ����
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>start to call BusT22_send_search_req ���Ͳ�ѯ���� !",__FILE__,__LINE__);
        iRet = BusT22_send_search_req(resp_cd,p_BcTransLog,p_tiBcMchntInf);
        if ( 0 != iRet )
        {
            //����ʧ��,���¿�
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2201>BusT22_send_search_req ���Ͳ�ѯ����ʧ��!",__FILE__,__LINE__);
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->flag_verifi, "3");
            strcpy(p_BcTransLog->resp_cd,resp_cd);
            strcpy(p_BcTransLog->bc_settle_in, "0");
            iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
            if ( 0 != sqlCode )
            {
                dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>tlBcTransLogOpr ���¿��¼ʧ��!",__FILE__,__LINE__);
                BC_SendExeception("03","���棬���½ɷ��������ݿ��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
                //BC_SendMonitor2("1","0",p_BcTransLog);
                return BusT22_send_term_resp("96", p_BcTransLog );
            }

            //����ʧ��
            //BC_SendMonitor2("1","0",p_BcTransLog);
            return BusT22_send_term_resp("96", p_BcTransLog );
        }
        //���Ͳ�ѯ����ɹ�
    } else
    {
        //���
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
        strcpy(p_BcTransLog->flag_rcv, "1");
        iRet = BC_InsertIntoDB(p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>BC_InsertIntoDB ���ʧ��!",__FILE__,__LINE__);
            BC_SendExeception("03","���棬�ɷ��������ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            //BC_SendMonitor2("1","0",p_BcTransLog);
            return BusT22_send_term_resp("96", p_BcTransLog );
        }

        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>BC_InsertIntoDB ���ɹ�",__FILE__,__LINE__); 

        //�����ѯ��ֱ�ӷ�����������
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>start to call BusT22_send_consume_req ���ͽɷ����� !",__FILE__,__LINE__);
        iRet = BusT22_send_consume_req(resp_cd,&g_TransMsg.BcTransLog,p_tiBcMchntInf);
        if ( 0 != sqlCode )
        {
            //����ʧ��,���
            BC_SendExeception("02","���棬���ͽɷ�����ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
            strcpy(p_BcTransLog->flag_rcv, "3");
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->resp_cd,resp_cd); //����ʧ��
            strcpy(p_BcTransLog->bc_settle_in, "0");
            iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
            if ( 0 != iRet )
            {
                BC_SendExeception("03","���棬���½ɷ��������ݿ��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
                dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>BC_InsertIntoDB ���ʧ��!",__FILE__,__LINE__);
            }

            //����ʧ��
            //BC_SendMonitor2("1","0",p_BcTransLog);
            return BusT22_send_term_resp("96", p_BcTransLog );
        }
        //������������ɹ�
    }

    return 0;
}

//���ղ�ѯ�ṩ��Ӧ��
//���ݷ�����Ϣ��1���ɹ������������Ϳۿ�����    
//              2��ʧ�ܣ����ն˷���ʧ��Ӧ��    
int BusT2220(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet = -1;
    int sqlCode;
    char resp_cd[3];

    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog;
    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    strcpy(resp_cd,g_TransMsg.BcTransLog.resp_cd);
    strcpy(p_BcTransLog->flag_lock  , "0");

    BC_SendMonitor2("1","4",p_BcTransLog);

    //�жϽ����Ƿ�ɹ�
    if ( 0 != strcmp(resp_cd,"00") )
    {
        //��ѯʧ�ܣ����ն˷���ʧ��Ӧ��
        //���
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->flag_verifi,"2");  
        strcpy(p_BcTransLog->resp_cd, resp_cd);
        strcpy(p_BcTransLog->resp_cd_verifi, resp_cd);
        strcpy(p_BcTransLog->bc_settle_in  , "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","���棬���½ɷ�Ӧ����Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2220>tlBcTransLogOpr �������ݿ���ʧ��!",__FILE__,__LINE__);
        }

        //����ʧ��
        //BC_SendMonitor2("1","0",p_BcTransLog);
        return BusT22_send_term_resp(resp_cd, p_BcTransLog );
    }

    //������������
    //check ����״̬
    if ( memcmp("2",p_BcTransLog->flag_rcv,1) || memcmp("1",p_BcTransLog->flag_rcv,1) )
    {
        //�Ѿ�����
        //���ɽ��ף��Ѿ��������������󣬻��Ѿ�����
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2220>�յ���ѯӦ���ף������ݿ��¼��ʾ�Ѿ������˽ɷ����󣬽��װ����ɣ��ݲ�����!",__FILE__,__LINE__);
        BC_SendExeception("02","�յ���ѯӦ���ף������ݿ��¼��ʾ�Ѿ������˽ɷ����󣬽��װ����ɣ��ݲ�����! ��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);

        //�������
        if ( '0' != *flag )
        {
            iRet = tlBcTransLogOpr(GLB_DB_UPDATE11, p_BcTransLog, &sqlCode);
            if ( 0 != sqlCode )
            {
                dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2220>tlBcTransLogOpr �������ݿ������ʧ��!",__FILE__,__LINE__);
                BC_SendExeception("03","���棬�������ݿ��¼ʧ�ܣ�������ɸü�¼����.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            }
        }
        return -1;
    }

    //�����
    strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
    strcpy(p_BcTransLog->flag_rcv, "1");

    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
        BC_SendExeception("03","���棬���½ɷ�Ӧ����Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        strcpy(p_BcTransLog->resp_cd,"96");
        //BC_SendMonitor2("1","0",p_BcTransLog);
        return BusT22_send_term_resp("96", p_BcTransLog );
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2220>start to call BusT22_send_consume_req ���ͽɷ����� !",__FILE__,__LINE__);
    iRet = BusT22_send_consume_req(resp_cd, p_BcTransLog,p_tiBcMchntInf);
    if ( 0 != sqlCode )
    {
        //����ʧ��,���
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->flag_rcv, "3");
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,resp_cd);
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2220>BC_InsertIntoDB ���ʧ��!",__FILE__,__LINE__);
            BC_SendExeception("03","���棬���½ɷ�Ӧ����Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            strcpy(p_BcTransLog->resp_cd,"96");
            //BC_SendMonitor2("1","0",p_BcTransLog);
        }

        //����ʧ��
        //BC_SendMonitor2("1","0",p_BcTransLog);
        return BusT22_send_term_resp(resp_cd, p_BcTransLog );
    }
    //�ɹ�����
    //BC_SendMonitor2("1","0",p_BcTransLog);
    return 0;
}

//����CUPSӦ��
//���ݷ�����Ϣ��1���ɹ�������Ӫ�̷�����������  
//              2��ʧ�ܣ����ն˷���ʧ��Ӧ��    
int BusT2230(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet = -1;
    int sqlCode;
    char resp_cd[3];

    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog;
    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;

    strcpy(resp_cd,g_TransMsg.BcTransLog.resp_cd);
    strcpy(p_BcTransLog->flag_lock  , "0");

    //�жϽ����Ƿ�ɹ�
    if ( 0 != strcmp(resp_cd,"00") )
    {
        //�ɷ�ʧ�ܣ����ն˷���ʧ��Ӧ��
        //���
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->is_over   , "1");
        strcpy(p_BcTransLog->flag_rcv  , "2");
        strcpy(p_BcTransLog->resp_cd   , resp_cd);
        strcpy(p_BcTransLog->flag_rcv  , resp_cd);
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","���棬���½ɷ�Ӧ����Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr �������ݿ���ʧ��!",__FILE__,__LINE__);
        }

        //����ʧ��
        return BusT22_send_term_resp(resp_cd, p_BcTransLog );
    }

    //������������
    //check ����״̬
    if ( memcmp("2",p_BcTransLog->flag_pay,1) || memcmp("1",p_BcTransLog->flag_rcv,1) )
    {
        //�Ѿ�����
        //���ɽ��ף��Ѿ��������������󣬻��Ѿ�����
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2230>�յ����ѳɹ�Ӧ���ף������ݿ��¼��ʾ�Ѿ��������������󣬽��װ����ɣ��ݲ�����!",__FILE__,__LINE__);

        //�������
        if ( '0' != *flag )
        {
            iRet = tlBcTransLogOpr(GLB_DB_UPDATE11, p_BcTransLog, &sqlCode);
            if ( 0 != sqlCode )
            {
                BC_SendExeception("03","���棬������ݿ�����ʧ�ܣ�������ɸ�����¼����.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
                dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr �������ݿ������ʧ��!",__FILE__,__LINE__);
            }
        }
        return -1;
    }

    //����Ƿ��������̻����룬���û�У������̻���
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>start to call BC_Search_mchnt_inf to search mchnt inf !",__FILE__,__LINE__);
    iRet = BC_Search_mchnt_inf(p_tiBcMchntInf, p_BcTransLog);
    if ( 0 != iRet )
    {
        //û���ҵ��̻���Ϣ
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT2230>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

        //���
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","���棬���½ɷ�Ӧ����Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
        }
        
        //����ʧ��        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>start to call BusT22_send_term_resp!",__FILE__,__LINE__);
        iRet = BusT22_send_term_resp("03", p_BcTransLog );

        //<reversal>��rcv���ͳ�������
        return BusT22_send_consume_reversal(resp_cd, p_BcTransLog, p_tiBcMchntInf);
    }

    // �ҵ����̻���Ϣ����ֵ
    strcpy(p_BcTransLog->mchnt_cd, p_tiBcMchntInf->mchnt_cd);

    //�����
    strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
    strcpy(p_BcTransLog->flag_pay, "1");
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        BC_SendExeception("03","���棬���½ɷ�Ӧ����Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
        iRet = BusT22_send_term_resp("96", p_BcTransLog );

        //<reversal>����շ����ͳ�������
        return BusT22_send_consume_reversal(resp_cd, p_BcTransLog, p_tiBcMchntInf);
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>start to call BusT22_send_pay_req ������������ !",__FILE__,__LINE__);
    iRet = BusT22_send_pay_req(resp_cd, p_BcTransLog ,p_tiBcMchntInf);
    if ( 0 != sqlCode )
    {
        //����ʧ��,���
        strcpy(p_tiBcTransDet->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->flag_pay, "3");
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,resp_cd);
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","���棬���½ɷ�Ӧ����Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
        }

        //����ʧ��
        iRet = BusT22_send_term_resp("96", p_BcTransLog );

        //<reversal>����շ����ͳ�������
        return BusT22_send_consume_reversal(resp_cd, p_BcTransLog, p_tiBcMchntInf);
    }
    //�ɹ�����
    return 0;
}


//������Ӫ��Ӧ��
//���ݷ�����Ϣ��1���ɹ������ն˷��ͳɹ�Ӧ��    
//              2��ʧ�ܣ����ն˷���ʧ��Ӧ��, �� cups ���ͳ��� 
int BusT2240(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet = -1;
    int sqlCode;
    char resp_cd[3];

    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog;
    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    strcpy(resp_cd,g_TransMsg.BcTransLog.resp_cd);
    strcpy(p_BcTransLog->flag_lock  , "0");

    //�жϽ����Ƿ�ɹ�
    if ( 0 != strcmp(resp_cd,"00") )
    {
        //�ɷ�ʧ�ܣ����ն˷���ʧ��Ӧ��
        //���
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->is_over   , "1");
        strcpy(p_BcTransLog->flag_pay  , "2");
        strcpy(p_BcTransLog->resp_cd   , resp_cd);
        strcpy(p_BcTransLog->flag_rcv  , resp_cd);
        strcpy(p_BcTransLog->bc_settle_in  , "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","���棬���½ɷ�Ӧ����Ϣʧ�ܣ����ܵ�������.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2240>tlBcTransLogOpr �������ݿ���ʧ��!",__FILE__,__LINE__);
            //????????
        } else
        {
            //���¿��¼�ɹ�
        }
        //����ʧ��
        BusT22_send_term_resp(resp_cd, p_BcTransLog );

        //���ͳ���
        //��cups
        //����Ӫ��

        return -1;
    }

    //�����
    strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
    strcpy(p_BcTransLog->flag_pay, "2");
    strcpy(p_BcTransLog->resp_cd, resp_cd);
    strcpy(p_BcTransLog->resp_cd_pay, resp_cd);
    strcpy(p_BcTransLog->bc_settle_in, "1");
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        BC_SendExeception("03","���棬���½ɷ�Ӧ����Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2240>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
        //????????
        return BusT22_send_term_resp(resp_cd, p_BcTransLog );
    }

    //���׳ɹ�
    return BusT22_send_term_resp(resp_cd, p_BcTransLog );
}


//��ѯ��ʱ -- ���ն˷��ͳ�ʱ
int BusT2221(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{


    /*��һ�ж��Ƿ����ѯ����ѯ�����ò�ѯ���ױ���*/

    /*���ò�ѯ�ģ����ÿۿ�ױ���*/

    return 0;
}

//cups��ʱ -- ���ն˷��ͳ�ʱ ���������� --  �� cups ������
int BusT2231(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{


    /*��һ�ж��Ƿ����ѯ����ѯ�����ò�ѯ���ױ���*/

    /*���ò�ѯ�ģ����ÿۿ�ױ���*/

    return 0;
}

//��Ӫ�̳�ʱ -- 1��������
//              2����cups���ͳ��������������ͳ���
int BusT2241(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{


    /*��һ�ж��Ƿ����ѯ����ѯ�����ò�ѯ���ױ���*/

    /*���ò�ѯ�ģ����ÿۿ�ױ���*/

    return 0;
}

