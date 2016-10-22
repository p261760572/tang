#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

/*缴费请求*/
#define SETBIT_T(ISO,INDEX,DATA,DATA_O,LEN) if( 0 != *(DATA))setbit_t(ISO, INDEX, (unsigned char *)DATA, LEN); else setbit_t(ISO, INDEX, (unsigned char *)DATA_O, LEN);
#define SETBIT_N(ISO,INDEX,DATA,LEN) if( 0 != *(DATA))setbit_t(ISO, INDEX, (unsigned char *)DATA, LEN);

//发送缴费应答给终端
int BusT22_send_term_resp(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog )
{
    ISO_data iso;
    ISO_data *p_iso = &iso;

    //发送给终端应答
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

    //设置终端密钥
    BC_SetTermMacKey2(p_BcTransLog->term_id);

    //发送监控信息
    BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,p_iso);

    //发送终端应答
    return BC_SendMessageToTerm(p_iso);
}

//发送运营商查询请求  -- 需要商户信息
int BusT22_send_search_req(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog, ti_bc_mchnt_inf_def *p_tiBcMchntInf )
{
    int iRet = -1;
    int sqlCode;
    char buff[256];
    ISO_data iso;
    ISO_data *p_iso = &iso;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息

    strcpy(resp_cd,"96");

    //查询机构(运营商)的线路名称
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(运营商)的线路名称 !",__FILE__,__LINE__, p_tiBcMchntInf->verifi_id);
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcMchntInf->verifi_id);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // 查询机构(运营商)线路信息失败
        strcpy(resp_cd,"96");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_search_req>call tiBcInsInfOpr return failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","查询机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>start to call BC_SetIsoMode(line_nm:%s) 设置机构(运营商)的线路名称 !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
    //设置运营商线路
    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>BC_SetIsoMode(line_nm:%s) 设置机构(运营商)的线路名称失败 !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
        strcpy(resp_cd,"96");
        BC_SendExeception("03","设置机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
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

    //设置运营商密钥信息
    BC_SetRcvMacKey(p_tiBcInsInf); //机构(运营商)
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>start to call BC_SendMessage 向运营商发送查询信息。",__FILE__,__LINE__);

    //插超时表
    iRet = BC_InsertTimeOut1(BC_GetFolderIdByName(p_tiBcInsInf->line_nm),g_iSrcFoldId,I_TIMEOUT,p_iso);
    if ( 0 != iRet )
    {
        strcpy(resp_cd,"96");
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>插入超时表失败",__FILE__,__LINE__);
        BC_SendExeception("03","警告，插入超时表失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>插入超时表成功",__FILE__,__LINE__);

    //发送查询交易
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);
    if ( 0!= iRet ) BC_DeleteTimeOut1(&g_TimeOutRec);

    //发送监控信息
    BC_SendMonitor("0","4",p_BcTransLog->trans_chnl,p_iso);
    strcpy(resp_cd,"00");
    return iRet;
}

//发送消费请求
int BusT22_send_consume_req(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog,ti_bc_mchnt_inf_def *p_tiBcMchntInf)
{
    int iRet;
    ISO_data iso;
    ISO_data *p_iso = &iso;
    char buff[256];
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息

    strcpy(resp_cd,"96");

    //查询机构的线路名称
    iRet = BC_Search_ins_inf(p_tiBcInsInf, p_tiBcMchntInf, p_BcTransLog->pri_acct_no, p_BcTransLog->pos_entry_md_cd);
    if ( 0 != iRet )
    {
        // 查询机构线路信息失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_consume_req>call BC_Search_ins_inf failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","查询机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        BC_SendExeception("03","设置机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
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

    //设置机构密钥信息
    BC_SetRcvMacKey(p_tiBcInsInf); //机构

    //插超时表
    iRet = BC_InsertTimeOut1(BC_GetFolderIdByName(p_tiBcInsInf->line_nm),g_iSrcFoldId,I_TIMEOUT,p_iso);
    if ( 0 != iRet )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT22_send_consume_req>插入超时表失败",__FILE__,__LINE__);
        BC_SendExeception("03","警告，插入超时表失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_consume_req>插入超时表成功",__FILE__,__LINE__);

    //发送交易信息
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);
    if ( 0!= iRet ) BC_DeleteTimeOut1(&g_TimeOutRec);

    //发送监控信息
    BC_SendMonitor("0","2",p_BcTransLog->trans_chnl,p_iso);
    strcpy(resp_cd,"00");
    return iRet;
}

//发送运营商上账请求 -- 需要商户信息
int BusT22_send_pay_req(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog, ti_bc_mchnt_inf_def *p_tiBcMchntInf )
{
    int iRet = -1;
    int sqlCode;
    char buff[256];
    ISO_data iso;
    ISO_data *p_iso = &iso;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    strcpy(resp_cd,"96");

    //查询机构(运营商)的线路名称
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(运营商)的线路名称 !",__FILE__,__LINE__, p_tiBcMchntInf->verifi_id);
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcMchntInf->pay_id);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // 查询机构(运营商)线路信息失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_pay_req>call tiBcInsInfOpr return failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","查询机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    //设置运营商线路信息
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>start to call BC_SetIsoMode(line_nm:%s) 设置机构(运营商)的线路名称 !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>BC_SetIsoMode(line_nm:%s) 设置机构(运营商)的线路名称失败 !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
        BC_SendExeception("03","设置机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
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

    //设置机构线路信息
    BC_SetRcvMacKey(p_tiBcInsInf); //机构(运营商)
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>start to call BC_SendMessage 向运营商发送上账交易。",__FILE__,__LINE__);

    //插超时表
    iRet = BC_InsertTimeOut1(BC_GetFolderIdByName(p_tiBcInsInf->line_nm),g_iSrcFoldId,I_TIMEOUT,p_iso);
    if ( 0 != iRet )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>插入超时表失败",__FILE__,__LINE__);
        BC_SendExeception("03","警告，插入超时表失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_req>插入超时表成功",__FILE__,__LINE__);

    //发送上账请求
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);
    if ( 0!= iRet ) BC_DeleteTimeOut1(&g_TimeOutRec);
    else strcpy(resp_cd,"00");

    //发送监控
    BC_SendMonitor("0","1",p_BcTransLog->trans_chnl,p_iso);
    return iRet;
}


//消费冲正
int BusT22_send_consume_reversal(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog,ti_bc_mchnt_inf_def *p_tiBcMchntInf)
{
    int iRet;
    ISO_data iso;
    ISO_data *p_iso = &iso;
    char buff[256];
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息

    strcpy(resp_cd,"96");

    //查询机构的线路名称
    iRet = BC_Search_ins_inf(p_tiBcInsInf, p_tiBcMchntInf, p_BcTransLog->pri_acct_no, p_BcTransLog->pos_entry_md_cd);
    if ( 0 != iRet )
    {
        // 查询机构线路信息失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_consume_reversal>call BC_Search_ins_inf failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","查询机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    //设置线路
    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        BC_SendExeception("03","设置机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
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

    //设置机构密钥信息
    BC_SetRcvMacKey(p_tiBcInsInf); //机构

    //发送冲正
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);

    //发送监控信息
    BC_SendMonitor("0","0",p_BcTransLog->trans_chnl,p_iso);
    strcpy(resp_cd,"00");
    return 0; //iRet;
}

//上账冲正 -- 需要商户信息
int BusT22_send_pay_reversal(char* resp_cd,tl_bc_trans_log_def *p_BcTransLog, ti_bc_mchnt_inf_def *p_tiBcMchntInf )
{
    int iRet = -1;
    int sqlCode  ;
    char buff[256];
    ISO_data iso;
    ISO_data *p_iso = &iso;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    strcpy(resp_cd,"96");

    //查询机构(运营商)的线路名称
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_reversal>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(运营商)的线路名称 !",__FILE__,__LINE__, p_tiBcMchntInf->verifi_id);
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcMchntInf->pay_id);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // 查询机构(运营商)线路信息失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_pay_reversal>call tiBcInsInfOpr return failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("02","查询机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
        BC_TRACE_END( -1 );
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_reversal>start to call BC_SetIsoMode(line_nm:%s) 设置机构(运营商)的线路名称 !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
    if ( 0 != BC_SetIsoMode(p_iso,p_tiBcInsInf->line_nm) )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_reversal>BC_SetIsoMode(line_nm:%s) 设置机构(运营商)的线路名称失败 !",__FILE__,__LINE__, p_tiBcInsInf->line_nm);
        BC_SendExeception("03","设置机构(运营商)线路信息失败,机构id:[%s].",p_tiBcMchntInf->verifi_id);
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

    //设置运营商密钥信息
    BC_SetRcvMacKey(p_tiBcInsInf); //机构(运营商)
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT22_send_pay_reversal>start to call BC_SendMessage 向运营商发送上账交易。",__FILE__,__LINE__);

    //发送上账冲正
    iRet = BC_SendMessage(p_iso,p_tiBcInsInf->line_nm);

    //发送监控信息
    BC_SendMonitor("0","0",p_BcTransLog->trans_chnl,p_iso);
    strcpy(resp_cd,"00");
    return iRet;
}


//接收终端请求
//根据商户配置信息，1。向运营商发出查询请求    
//                  2。无需发送查询请求 -> 直接向银联发送扣款请求 
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

    // 获取商户信息
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>start to call BC_Search_mchnt_inf to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    iRet = BC_Search_mchnt_inf(p_tiBcMchntInf, p_BcTransLog);
    if ( 0 != iRet )
    {
        //没有找到商户信息
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

        //入库
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = BC_InsertIntoDB(p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>BC_InsertIntoDB 入库失败!",__FILE__,__LINE__);
            BC_SendExeception("03","警告，缴费请求入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>BC_InsertIntoDB 入库成功",__FILE__,__LINE__); 

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>start to call BusT22_send_term_resp!",__FILE__,__LINE__);
        //BC_SendMonitor2("1","0",p_BcTransLog);
        return BusT22_send_term_resp("03", p_BcTransLog );
    }

    // 找到了商户信息，赋值
    strcpy(p_BcTransLog->mchnt_cd, p_tiBcMchntInf->mchnt_cd);

    // 判断是否需查询
    if ( '1' == *p_tiBcMchntInf->verifi_flag )
    {
        //入库
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
        strcpy(p_BcTransLog->flag_verifi, "1");
        iRet = BC_InsertIntoDB(p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","警告，缴费请求入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>BC_InsertIntoDB 入库失败!",__FILE__,__LINE__);
            //BC_SendMonitor2("1","0",p_BcTransLog);
            return BusT22_send_term_resp("96", p_BcTransLog );
        }

        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>BC_InsertIntoDB 入库成功",__FILE__,__LINE__); 

        //发送运营商查询请求
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>start to call BusT22_send_search_req 发送查询请求 !",__FILE__,__LINE__);
        iRet = BusT22_send_search_req(resp_cd,p_BcTransLog,p_tiBcMchntInf);
        if ( 0 != iRet )
        {
            //发送失败,更新库
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2201>BusT22_send_search_req 发送查询请求失败!",__FILE__,__LINE__);
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->flag_verifi, "3");
            strcpy(p_BcTransLog->resp_cd,resp_cd);
            strcpy(p_BcTransLog->bc_settle_in, "0");
            iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
            if ( 0 != sqlCode )
            {
                dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>tlBcTransLogOpr 更新库记录失败!",__FILE__,__LINE__);
                BC_SendExeception("03","警告，更新缴费请求数据库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
                //BC_SendMonitor2("1","0",p_BcTransLog);
                return BusT22_send_term_resp("96", p_BcTransLog );
            }

            //交易失败
            //BC_SendMonitor2("1","0",p_BcTransLog);
            return BusT22_send_term_resp("96", p_BcTransLog );
        }
        //发送查询请求成功
    } else
    {
        //入库
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
        strcpy(p_BcTransLog->flag_rcv, "1");
        iRet = BC_InsertIntoDB(p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>BC_InsertIntoDB 入库失败!",__FILE__,__LINE__);
            BC_SendExeception("03","警告，缴费请求入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            //BC_SendMonitor2("1","0",p_BcTransLog);
            return BusT22_send_term_resp("96", p_BcTransLog );
        }

        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2210>BC_InsertIntoDB 入库成功",__FILE__,__LINE__); 

        //无需查询，直接发送消费请求
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>start to call BusT22_send_consume_req 发送缴费请求 !",__FILE__,__LINE__);
        iRet = BusT22_send_consume_req(resp_cd,&g_TransMsg.BcTransLog,p_tiBcMchntInf);
        if ( 0 != sqlCode )
        {
            //发送失败,入库
            BC_SendExeception("02","警告，发送缴费请求失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
            strcpy(p_BcTransLog->flag_rcv, "3");
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->resp_cd,resp_cd); //发送失败
            strcpy(p_BcTransLog->bc_settle_in, "0");
            iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
            if ( 0 != iRet )
            {
                BC_SendExeception("03","警告，更新缴费请求数据库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
                dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>BC_InsertIntoDB 入库失败!",__FILE__,__LINE__);
            }

            //交易失败
            //BC_SendMonitor2("1","0",p_BcTransLog);
            return BusT22_send_term_resp("96", p_BcTransLog );
        }
        //发送消费请求成功
    }

    return 0;
}

//接收查询提供方应答
//根据返回信息，1。成功，向银联发送扣款请求    
//              2。失败，向终端发送失败应答    
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

    //判断交易是否成功
    if ( 0 != strcmp(resp_cd,"00") )
    {
        //查询失败，向终端发送失败应答
        //入库
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->flag_verifi,"2");  
        strcpy(p_BcTransLog->resp_cd, resp_cd);
        strcpy(p_BcTransLog->resp_cd_verifi, resp_cd);
        strcpy(p_BcTransLog->bc_settle_in  , "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","警告，更新缴费应答信息失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2220>tlBcTransLogOpr 更新数据库标记失败!",__FILE__,__LINE__);
        }

        //交易失败
        //BC_SendMonitor2("1","0",p_BcTransLog);
        return BusT22_send_term_resp(resp_cd, p_BcTransLog );
    }

    //发送消费请求
    //check 消费状态
    if ( memcmp("2",p_BcTransLog->flag_rcv,1) || memcmp("1",p_BcTransLog->flag_rcv,1) )
    {
        //已经发送
        //可疑交易，已经发送了消费请求，或已经处理
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2220>收到查询应答交易，但数据库记录显示已经发送了缴费请求，交易包可疑，暂不处理!",__FILE__,__LINE__);
        BC_SendExeception("02","收到查询应答交易，但数据库记录显示已经发送了缴费请求，交易包可疑，暂不处理! 流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);

        //解除锁定
        if ( '0' != *flag )
        {
            iRet = tlBcTransLogOpr(GLB_DB_UPDATE11, p_BcTransLog, &sqlCode);
            if ( 0 != sqlCode )
            {
                dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2220>tlBcTransLogOpr 更新数据库锁标记失败!",__FILE__,__LINE__);
                BC_SendExeception("03","警告，解锁数据库记录失败，可能造成该记录死锁.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            }
        }
        return -1;
    }

    //先入库
    strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
    strcpy(p_BcTransLog->flag_rcv, "1");

    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2201>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
        BC_SendExeception("03","警告，更新缴费应答信息失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        strcpy(p_BcTransLog->resp_cd,"96");
        //BC_SendMonitor2("1","0",p_BcTransLog);
        return BusT22_send_term_resp("96", p_BcTransLog );
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2220>start to call BusT22_send_consume_req 发送缴费请求 !",__FILE__,__LINE__);
    iRet = BusT22_send_consume_req(resp_cd, p_BcTransLog,p_tiBcMchntInf);
    if ( 0 != sqlCode )
    {
        //发送失败,入库
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->flag_rcv, "3");
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,resp_cd);
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2220>BC_InsertIntoDB 入库失败!",__FILE__,__LINE__);
            BC_SendExeception("03","警告，更新缴费应答信息失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            strcpy(p_BcTransLog->resp_cd,"96");
            //BC_SendMonitor2("1","0",p_BcTransLog);
        }

        //交易失败
        //BC_SendMonitor2("1","0",p_BcTransLog);
        return BusT22_send_term_resp(resp_cd, p_BcTransLog );
    }
    //成功发送
    //BC_SendMonitor2("1","0",p_BcTransLog);
    return 0;
}

//接收CUPS应答
//根据返回信息，1。成功，向运营商发送上账请求  
//              2。失败，向终端发送失败应答    
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

    //判断交易是否成功
    if ( 0 != strcmp(resp_cd,"00") )
    {
        //缴费失败，向终端发送失败应答
        //入库
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->is_over   , "1");
        strcpy(p_BcTransLog->flag_rcv  , "2");
        strcpy(p_BcTransLog->resp_cd   , resp_cd);
        strcpy(p_BcTransLog->flag_rcv  , resp_cd);
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","警告，更新缴费应答信息失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr 更新数据库标记失败!",__FILE__,__LINE__);
        }

        //交易失败
        return BusT22_send_term_resp(resp_cd, p_BcTransLog );
    }

    //发送上账请求
    //check 消费状态
    if ( memcmp("2",p_BcTransLog->flag_pay,1) || memcmp("1",p_BcTransLog->flag_rcv,1) )
    {
        //已经发送
        //可疑交易，已经发送了上账请求，或已经处理
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2230>收到消费成功应答交易，但数据库记录显示已经发送了上账请求，交易包可疑，暂不处理!",__FILE__,__LINE__);

        //解除锁定
        if ( '0' != *flag )
        {
            iRet = tlBcTransLogOpr(GLB_DB_UPDATE11, p_BcTransLog, &sqlCode);
            if ( 0 != sqlCode )
            {
                BC_SendExeception("03","警告，解除数据库锁定失败，可能造成该条记录死锁.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
                dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr 更新数据库锁标记失败!",__FILE__,__LINE__);
            }
        }
        return -1;
    }

    //检查是否上送了商户代码，如果没有，搜索商户号
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>start to call BC_Search_mchnt_inf to search mchnt inf !",__FILE__,__LINE__);
    iRet = BC_Search_mchnt_inf(p_tiBcMchntInf, p_BcTransLog);
    if ( 0 != iRet )
    {
        //没有找到商户信息
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT2230>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

        //入库
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","警告，更新缴费应答信息失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
        }
        
        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>start to call BusT22_send_term_resp!",__FILE__,__LINE__);
        iRet = BusT22_send_term_resp("03", p_BcTransLog );

        //<reversal>向rcv发送冲正交易
        return BusT22_send_consume_reversal(resp_cd, p_BcTransLog, p_tiBcMchntInf);
    }

    // 找到了商户信息，赋值
    strcpy(p_BcTransLog->mchnt_cd, p_tiBcMchntInf->mchnt_cd);

    //先入库
    strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
    strcpy(p_BcTransLog->flag_pay, "1");
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        BC_SendExeception("03","警告，更新缴费应答信息失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
        iRet = BusT22_send_term_resp("96", p_BcTransLog );

        //<reversal>向接收方发送冲正交易
        return BusT22_send_consume_reversal(resp_cd, p_BcTransLog, p_tiBcMchntInf);
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>start to call BusT22_send_pay_req 发送上账请求 !",__FILE__,__LINE__);
    iRet = BusT22_send_pay_req(resp_cd, p_BcTransLog ,p_tiBcMchntInf);
    if ( 0 != sqlCode )
    {
        //发送失败,入库
        strcpy(p_tiBcTransDet->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->flag_pay, "3");
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,resp_cd);
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","警告，更新缴费应答信息失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2230>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
        }

        //交易失败
        iRet = BusT22_send_term_resp("96", p_BcTransLog );

        //<reversal>向接收方发送冲正交易
        return BusT22_send_consume_reversal(resp_cd, p_BcTransLog, p_tiBcMchntInf);
    }
    //成功发送
    return 0;
}


//接收运营商应答
//根据返回信息，1。成功，向终端发送成功应答    
//              2。失败，向终端发送失败应答, 向 cups 发送冲正 
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

    //判断交易是否成功
    if ( 0 != strcmp(resp_cd,"00") )
    {
        //缴费失败，向终端发送失败应答
        //入库
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        strcpy(p_BcTransLog->is_over   , "1");
        strcpy(p_BcTransLog->flag_pay  , "2");
        strcpy(p_BcTransLog->resp_cd   , resp_cd);
        strcpy(p_BcTransLog->flag_rcv  , resp_cd);
        strcpy(p_BcTransLog->bc_settle_in  , "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != sqlCode )
        {
            BC_SendExeception("03","警告，更新缴费应答信息失败，可能导致死锁.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusT2240>tlBcTransLogOpr 更新数据库标记失败!",__FILE__,__LINE__);
            //????????
        } else
        {
            //更新库记录成功
        }
        //交易失败
        BusT22_send_term_resp(resp_cd, p_BcTransLog );

        //发送冲正
        //向cups
        //向运营商

        return -1;
    }

    //先入库
    strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
    strcpy(p_BcTransLog->flag_pay, "2");
    strcpy(p_BcTransLog->resp_cd, resp_cd);
    strcpy(p_BcTransLog->resp_cd_pay, resp_cd);
    strcpy(p_BcTransLog->bc_settle_in, "1");
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        BC_SendExeception("03","警告，更新缴费应答信息失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusT2240>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
        //????????
        return BusT22_send_term_resp(resp_cd, p_BcTransLog );
    }

    //交易成功
    return BusT22_send_term_resp(resp_cd, p_BcTransLog );
}


//查询超时 -- 向终端发送超时
int BusT2221(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{


    /*第一判断是否需查询，查询，调用查询交易报文*/

    /*不用查询的，调用扣款交易报文*/

    return 0;
}

//cups超时 -- 向终端发送超时 ？？？？？ --  向 cups 发冲正
int BusT2231(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{


    /*第一判断是否需查询，查询，调用查询交易报文*/

    /*不用查询的，调用扣款交易报文*/

    return 0;
}

//运营商超时 -- 1。不处理
//              2。向cups发送冲正，向银联发送冲正
int BusT2241(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{


    /*第一判断是否需查询，查询，调用查询交易报文*/

    /*不用查询的，调用扣款交易报文*/

    return 0;
}

