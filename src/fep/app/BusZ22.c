#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

char* getIdStr(char *idType, char *idStr);

/*缴费请求*/
int BusZ_BillCopy(tl_bc_trans_log_def *p_BcTransLog, tl_bc_bill_log_def *pBcBillLog)
{
    #define BusZ_BillCopy_MEMCPY(X,Y) memcpy(X,Y, min(sizeof(X),sizeof(Y)));
    memset(pBcBillLog, 0, sizeof(tl_bc_bill_log_def) );

    //copy
    BusZ_BillCopy_MEMCPY(pBcBillLog->bc_settle_in       , p_BcTransLog->bc_settle_in        )
    BusZ_BillCopy_MEMCPY(pBcBillLog->trans_at           , p_BcTransLog->trans_at            )
    BusZ_BillCopy_MEMCPY(pBcBillLog->mobile_no          , p_BcTransLog->fld_5               )
    BusZ_BillCopy_MEMCPY(pBcBillLog->sys_tra_no         , p_BcTransLog->sys_tra_no          )
    BusZ_BillCopy_MEMCPY(pBcBillLog->loc_trans_tm       , p_BcTransLog->loc_trans_tm        )
    BusZ_BillCopy_MEMCPY(pBcBillLog->loc_trans_dt       , p_BcTransLog->loc_trans_dt        )
    BusZ_BillCopy_MEMCPY(pBcBillLog->settle_dt          , p_BcTransLog->settle_dt           )
    BusZ_BillCopy_MEMCPY(pBcBillLog->resp_code          , p_BcTransLog->fld_28              )
    BusZ_BillCopy_MEMCPY(pBcBillLog->fwd_ins_id_cd      , p_BcTransLog->fwd_ins_id_cd       )
    BusZ_BillCopy_MEMCPY(pBcBillLog->retri_ref_no       , p_BcTransLog->retri_ref_no             )
    BusZ_BillCopy_MEMCPY(pBcBillLog->resp_cd            , p_BcTransLog->resp_cd             )
    BusZ_BillCopy_MEMCPY(pBcBillLog->mchnt_cd           , p_BcTransLog->mchnt_cd            )
    BusZ_BillCopy_MEMCPY(pBcBillLog->bill_id            , p_BcTransLog->fld_45              )
    BusZ_BillCopy_MEMCPY(pBcBillLog->bill_id_rcv        , p_BcTransLog->bill_id_rcv         )
    return 0;
}

//发送安捷非绑定账单支付报文
// resp_flag 应答方 0 为本系统
int BusZ2210_1(BcTransMsg *p_BcTransMsg,int *resp_flag,char *resp_code, 
               ti_bc_trans_inf_def *p_tiBcTransInf,
               ti_bc_ins_inf_def *p_tiBcInsInf , 
               ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf,
               ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf)
{
    int iLen;
    int iRet;
    int sqlCode;
    char buff[2048],temp[256];
    char packet_type[5];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ISO_data iso;
    ISO_data *p_iso = &iso ;
    *resp_flag = 1;

    //组建报文
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        *resp_flag = 0;
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ2210_1>BC_SetIsoMode failure(ins_id_cd:%s,packet_type:%s)。",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd, p_tiBcInsInf->packet_type);
        BC_TRACE_END( -1 );
    }

    //转换编码，注意长度，不用strlen计算，以防出现问题
    iLen = GBKToUnicode_2(p_BcTransLog->fld_48,g_TransMsg.len_48,buff,sizeof(buff)-1);
    if ( iLen >= 0 )
    {
        buff[iLen] = 0;
    }

    setbit_t(p_iso, 0 , "0820", 4); 
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "810000", 6);
    setbit_t(p_iso, 4 , (unsigned char *)p_BcTransLog->trans_at, 12);
    setbit_t(p_iso, 5 , (unsigned char *)p_BcTransLog->fld_5, 12);
    setbit_t(p_iso, 8 , (unsigned char *)p_BcTransLog->fld_8, 8);
    setbit_t(p_iso, 11, (unsigned char *)p_tiBcMchntTermChargeInf->sys_tra_no, 6);
    setbit_t(p_iso, 12, (unsigned char *)p_BcTransLog->loc_trans_tm, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
    setbit_t(p_iso, 42, aj_mchnt_cd, 15);
    setbit_t(p_iso, 46, (unsigned char *)p_tiBcMchntTermChargeInf->mchnt_cd_c, 15);
    setbit_t(p_iso, 48, (unsigned char *)buff, iLen);    
    setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,30));
    setbit_t(p_iso, 61, (unsigned char *)p_BcTransLog->fld_61, strlen(p_BcTransLog->fld_61));
    if ( 0 != *p_BcTransLog->fld_45 )
    {
        strcpy(buff, p_BcTransLog->fld_45);
        rtrim(buff);
        setbit_t(p_iso, 45 , (unsigned char *)buff, strlen(buff));
    }
    setbit_t(p_iso, 64, "00000000", 8);

    //发送监控信息
    BC_SendMonitor_iso("0","0", "AJXT", p_iso, p_tiBcTransInf);
    
    //发送签到并更新数据表
    memset(buff, 0, sizeof(buff) );

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ2210_1>start call BC_SendMessageToAJ 发送数据给安捷。",__FILE__,__LINE__);

    //转发给安捷    
    memset(buff, 0, sizeof(buff) );
    PrintISO( p_iso,"发送给安捷的非绑定支付报文",0);
    iLen = BC_SendMessageToAJ(p_iso, buff,p_tiBcInsInf);
    if ( iLen <= 0 )
    {
        //发送给安捷失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210_1>send to rcv failure.",__FILE__,__LINE__);

        *resp_flag = 0;
        strcpy(resp_code,"N00091");

        //更新库记录
        BC_SendExeception("03","警告，发送安捷非绑定支付报文失败.");
        return -1;
    }
    //解包
    memcpy(resp_code,buff,6);
    resp_code[6] = 0;
    if ( 0==memcmp(resp_code,"N00000",6) )
    {
        //插入账单超时表
    	setbit_t(p_iso, 42, (unsigned char *)p_tiBcMchntTermChargeInf->mchnt_cd_c, 15);
        iRet = BC_InsertBillNotify(gs_myFid,g_iSrcFoldId,I_TIMEOUT_BILL,p_iso, &g_TransMsg_o);
        if ( 0== iRet )
        {
            dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210_1>BC_InsertBillNotify success.(p_tiBcInsInf->packet_type:%s)",__FILE__,__LINE__,p_tiBcInsInf->packet_type);
        } else
        {
            dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210_1>BC_InsertBillNotify failure.(p_tiBcInsInf->packet_type:%s)",__FILE__,__LINE__,p_tiBcInsInf->packet_type);
        }
    }
    return 0;
}

//非绑定用户支付 0850 请求
int BusZ2210(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet ;

    int sqlCode    ;
    int resp_flag ;
    char resp_code[6+1];
    char buff[1024] ;
    int isExitFlag = 0;

    ti_bc_mt_flow_id_def tiBcMTFlowId ;
    ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
    ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf = (ti_bc_mchnt_term_charge_inf_def *) &g_tiBcMchntTermChargeInf; //商户终端转换信息
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;

    strcpy(p_BcTransLog->flag_lock, "0");
    BC_SendMonitor_trans("0","3", p_BcTransLog,p_tiBcTransInf);

    //first 处理#48域编码,保证为UTF-8编码
    //获取#33域机构信息
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog->fwd_ins_id_cd );
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(账单系统)的机构信息 !",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // 查询机构信息失败
        strcpy(resp_code,"96");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>call tiBcInsInfOpr return failure,cann't find ins[%s] inf.",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
        BC_SendExeception("查询机构线路信息失败,机构id:[%s].",p_tiBcInsInf->ins_id_cd);

       //异常记录入库
				if( 0 > BC_exp_log("0091","查询机构信息失败",g_Rcv_Buffer,g_RcvBufferSize) )
				{
					dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcExpLogOpr 异常库记录入库失败.流水号:[%s],终端号:[%s],商户号:[%s]!",__FILE__,__LINE__,p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
         BC_SendExeception("03","警告，非绑定用户请求记录异常表失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
				}

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N000D1", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
        BC_SetMacKey(0,0);
		    //BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso);
        return 0;
    }

    //校验账单机构密钥
    if ( *p_tiBcInsInf->key_type != '0' )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_TermCheckMac(ins_id_cd:%s) 校验机构(账单系统)的机构密钥信息 !",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
        if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) )
        {
            //更新数据库
            //dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

            //入库
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->resp_cd,"A0");
            strcpy(p_BcTransLog->fld_28 ,"N000A0");
            strcpy(p_BcTransLog->bc_settle_in, "0");
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
            
            //异常记录入库
				    if( 0 > BC_exp_log("00A0","校验账单机构密钥信息失败",g_Rcv_Buffer,g_RcvBufferSize) )
				    {
					     dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcExpLogOpr 异常库记录入库失败.流水号:[%s],终端号:[%s],商户号:[%s]!",__FILE__,__LINE__,p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
              BC_SendExeception("03","警告，非绑定用户请求记录异常表失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
				    }

            //交易失败        
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso,28,"N000A0", 6 );
            setbit_t(iso,64,"00000000", 8 );
            BC_SetRcvMacKey(p_tiBcInsInf);
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            BC_SendMessageToSrc(iso);
            return 0;
        }
    }
    iRet = ToGBK(p_BcTransLog->fld_48,g_TransMsg.len_48,
                  buff,sizeof(buff)-1);
    if ( iRet>0 )
    {
        iRet = MIN(iRet,sizeof(p_BcTransLog->fld_48)-1);
        memcpy(p_BcTransLog->fld_48,buff, iRet);
        p_BcTransLog->fld_48[iRet] = 0;
        g_TransMsg.len_48 = iRet;
    }
	else
	{
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00091", "48域转GBK转码失败！", 0);
		return ReturnPayMessageToRecive("N00091", iso, p_tiBcTransInf, p_BcTransLog);
	}

    //账单入库
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tlBcBillLogOpr !",__FILE__,__LINE__ );
    isExitFlag = 0;
    BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
    iRet = tlBcBillLogOpr(GLB_DB_INSERT1, p_tlBcBillLog, &sqlCode);
    if ( SQL_ERD_UNIKEY == sqlCode )
    {
        //查询该账单交易状态
        tlBcBillLogOpr(GLB_DB_SELECT2, p_tlBcBillLog, &sqlCode);
        if ( 0 == sqlCode )
        {   
            //搜索到了该账单号有成功交易或待处理交易 NC0001
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>账单号重复(fwd_ins_id_cd:%s,bill_id:%s)。!",__FILE__,__LINE__,p_BcTransLog->fwd_ins_id_cd, p_tlBcBillLog->bill_id );
            BC_SendExeception("02","警告，非绑定用户请求入库失败.(fwd_ins_id_cd:%s,bill_id:%s)",p_BcTransLog->fwd_ins_id_cd, p_tlBcBillLog->bill_id );
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            if ( '1'==*p_tlBcBillLog->bc_settle_in )
            {
                strcpy(resp_code, "NC0001");
                setbit_t(iso,28, resp_code, 6 ) ;  //账单已经成功支付
                setbit_t(iso,64, "00000000", 8 ) ;
                setbitoff(iso,48);
                BC_SetRcvMacKey(p_tiBcInsInf);
                isExitFlag = 1;
                strcpy(p_BcTransLog->is_over, "1");
                strcpy(p_BcTransLog->fld_28, resp_code);
                strcpy(p_BcTransLog->resp_cd, resp_code+4);
                
            } else if ( '0'==*p_tlBcBillLog->bc_settle_in )
            {
                BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
                strcpy(p_tlBcBillLog->bc_settle_in, " ");
                iRet = tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
                if ( 0!= sqlCode )
                {
                    //update failure
                    strcpy(resp_code, "N00096");
                    setbit_t(iso,28, resp_code, 6 );
                    setbit_t(iso,64,"00000000", 8 );
                    setbitoff(iso,48);
                    BC_SetRcvMacKey(p_tiBcInsInf);
                    isExitFlag = 1;
                    strcpy(p_BcTransLog->is_over, "1");
                    strcpy(p_BcTransLog->fld_28, resp_code);
                    strcpy(p_BcTransLog->resp_cd, resp_code+4);
                }
                //更新了状态，继续进行交易
            } else
            {
                strcpy(resp_code, "N00009");
                setbit_t(iso,28, resp_code, 6 );
                setbit_t(iso,64,"00000000", 8 );
                setbitoff(iso,48);
                BC_SetRcvMacKey(p_tiBcInsInf);
                isExitFlag = 1;
                strcpy(p_BcTransLog->is_over, "1");
                strcpy(p_BcTransLog->fld_28, resp_code);
                strcpy(p_BcTransLog->resp_cd, resp_code+4);
            }            
        } else
        {
            //查询该账单交易状态失败
            strcpy(resp_code, "N00096");
            setbit_t(iso,28, resp_code, 6 );
            setbit_t(iso,64,"00000000", 8 );
            setbitoff(iso,48);
            BC_SetRcvMacKey(p_tiBcInsInf);
            isExitFlag = 1;
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->fld_28, resp_code);
            strcpy(p_BcTransLog->resp_cd, resp_code+4);
        }
    }
    else if ( 0 != sqlCode )
    {
        strcpy(resp_code, "N00096");
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcBillLogOpr 入库失败!",__FILE__,__LINE__);
        BC_SendExeception("03","警告，账单BILL入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,resp_code, 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
	      BC_SetRcvMacKey(p_tiBcInsInf);
        isExitFlag = 1;
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->fld_28, resp_code);
        strcpy(p_BcTransLog->resp_cd, resp_code+4);
    } else
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcBillLogOpr 入库成功",__FILE__,__LINE__); 

    //入库
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_InsertIntoDB !",__FILE__,__LINE__ );
    BC_InsertIntoDB(p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        if( sqlCode == SQL_ERD_UNIKEY ){
            strcpy(resp_code, "N00094");
        } else{
            strcpy(resp_code, "N00096");
        }
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>BC_InsertIntoDB 入库失败!",__FILE__,__LINE__);
        BC_SendExeception("03","警告，非绑定用户请求入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,resp_code, 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
	      BC_SetRcvMacKey(p_tiBcInsInf);
        isExitFlag = 1;
    } else
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>BC_InsertIntoDB 入库成功",__FILE__,__LINE__);

    //判断是否需要退出
    if( isExitFlag ){
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }
   
    // 获取商户信息
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog->mchnt_cd);
    iRet = tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到商户信息
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

        //更新库
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->fld_28 ,"N00003");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
            BC_SendExeception("03","警告，非绑定用户请求更新库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库成功",__FILE__,__LINE__); 

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00003", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);        
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }

    //area_cd
    strcpy(p_BcTransLog->area_cd, p_tiBcMchntInf->area_cd);

    //检查商户转换信息表 ti_bc_mchnt_term_charge_inf_def g_tiBcMchntTermChargeInf;
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcMchntTermChargeInfOpr 获取商户转换信息 !",__FILE__,__LINE__ );
    strcpy(p_tiBcMchntTermChargeInf->mchnt_tp,  p_BcTransLog->mchnt_tp);
    strcpy(p_tiBcMchntTermChargeInf->term_id ,  p_BcTransLog->term_id);
    strcpy(p_tiBcMchntTermChargeInf->mchnt_cd,  p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcMchntTermChargeInf->rcv_ins_id_cd,  g_rtIns_id_cd);
    iRet = tiBcMchntTermChargeInfOpr(GLB_DB_SELECT1, p_tiBcMchntTermChargeInf, &sqlCode);
    if( 0 != sqlCode )
    {
        // 查询商户转换信息表失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>call tiBcMchntTermChargeInfOpr failure,cann't find ins(%s) inf.",__FILE__,__LINE__, p_tiBcMchntTermChargeInf->mchnt_cd);
        BC_SendExeception("03","查询商户转换信息失败,mchnt_cd:[%s].",p_tiBcMchntTermChargeInf->mchnt_cd);
        
        //没有找到商户信息
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

        //更新库
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->fld_28 ,"N00003");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
            BC_SendExeception("03","警告，非绑定用户请求更新库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库成功",__FILE__,__LINE__); 

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00003", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);        
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }

    //获取转换流水号
    strcpy(tiBcMTFlowId.term_id   ,  p_tiBcMchntTermChargeInf->term_id_c);
    strcpy(tiBcMTFlowId.mchnt_cd  ,  p_tiBcMchntTermChargeInf->mchnt_cd_c);
    tiBcMTFlowIdOpr(GLB_DB_SELECT1, &tiBcMTFlowId, &sqlCode);

    //转换流水号，终端号，商户号
    strcpy(p_tiBcMchntTermChargeInf->sys_tra_no, tiBcMTFlowId.sys_tra_no);
    strcpy(p_BcTransLog->center_sys_tra_no,  tiBcMTFlowId.sys_tra_no);
    strcpy(p_BcTransLog->center_term_id   ,  p_tiBcMchntTermChargeInf->term_id_c);
    strcpy(p_BcTransLog->center_mchnt_cd  ,  p_tiBcMchntTermChargeInf->mchnt_cd_c);

    // 获取清算商户信息
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcQsMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__, p_tiBcMchntTermChargeInf->mchnt_cd_c);
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_tiBcMchntTermChargeInf->mchnt_cd_c);
    strcpy(p_tiBcQsMchntInf->term_id, p_tiBcMchntTermChargeInf->term_id_c);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_tiBcMchntTermChargeInf->rcv_ins_id_cd);
    iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到清算商户信息
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd,sqlCode );
        BC_SendExeception("03","cant not find qs mchnt.(mchnt_cd:%s,sqlcode=%d) ",p_tiBcQsMchntInf->mchnt_cd,sqlCode );
        
        //更新库
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->fld_28 ,"N00003");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
            BC_SendExeception("03","警告，非绑定用户请求更新库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库成功",__FILE__,__LINE__); 

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00003", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }
    
    //搜索路由机构信息，即#100域机构信息
    if( 0!= BC_Search_ins_inf(resp_code, &tiBcInsInf, g_rtIns_id_cd))
    {
    	// 查询接收机构线路信息失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>call BC_Search_ins_inf failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("03","查询机构线路信息失败,机构id:[%s].", g_rtIns_id_cd);
        
        //更新库
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->fld_28 ,"N00091");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
            BC_SendExeception("03","警告，非绑定用户请求更新库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库成功",__FILE__,__LINE__); 

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00091", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);        
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }

    //check field
    if ( 0 != BC_CheckIsoDataField(iso, "2,3,4,5,8,11,12,13,42,48,61,64") )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ2210>doBus_CheckIsoDataField failure.",__FILE__,__LINE__);        
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"30");
        strcpy(p_BcTransLog->fld_28,"N00030");
        strcpy(p_BcTransLog->bc_settle_in, "0");

        dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ2210>start to call BC_InsertIntoDB.",__FILE__,__LINE__);
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库失败!",__FILE__,__LINE__);
            BC_SendExeception("03","警告，非绑定用户请求更新库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库成功",__FILE__,__LINE__); 

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BusT22_send_term_resp!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00030", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
        BC_SetRcvMacKey(p_tiBcInsInf);        
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso);
        return 0;
    }

    //更新库标记
    strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
    strcpy(p_BcTransLog->flag_rcv, "0");  //正在发送

    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新失败!",__FILE__,__LINE__);
        BC_SendExeception("警告，非绑定用户支付更新库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00096", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
        BC_SetRcvMacKey(p_tiBcInsInf);        
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso);
        return 0;
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库成功",__FILE__,__LINE__); 
    //发送给安捷
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BusZ01_1 向安捷发送非绑定支付交易.",__FILE__,__LINE__); 
    iRet =BusZ2210_1(p_BcTransMsg,&resp_flag, resp_code, p_tiBcTransInf,&tiBcInsInf,p_tiBcQsMchntInf, p_tiBcMchntTermChargeInf );
    if ( iRet < 0 )
    {
        //发送给安捷失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>send to rcv failure.",__FILE__,__LINE__);

        //更新库记录
        strcpy(p_BcTransLog->flag_rcv, "3");
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,resp_code+4);
        strcpy(p_BcTransLog->bc_settle_in, "0");
        strcpy(p_BcTransLog->fld_28,resp_code);
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);

        //交易失败
        //BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
        strcpy(p_tlBcBillLog->bc_settle_in, "0");
        strcpy(p_tlBcBillLog->resp_code, resp_code);
        iRet = tlBcBillLogOpr(GLB_DB_INSERT1, p_tlBcBillLog, &sqlCode);     
    } else
    {
        strcpy(p_BcTransLog->flag_rcv, "2");
        strcpy(p_BcTransLog->resp_cd, resp_code+4);
        if ( 0 == memcmp(resp_code, "N00000", 6) )
        {
            //strcpy(p_BcTransLog->bc_settle_in, "1");
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);

        } else
        {
            strcpy(p_BcTransLog->bc_settle_in, "0");
            strcpy(p_BcTransLog->fld_28,resp_code);
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        }
    }

    //更新数据库
    strcpy(p_BcTransLog->rcv_ins_id_cd, tiBcInsInf.ins_id_cd);
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr 更新库记录失败!",__FILE__,__LINE__);
        BC_SendExeception("03","警告，更新账单请求数据库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }

    //应答给账单系统
    BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
    setbit_t(iso,0,buff,4 );
    setbit_t(iso,28,resp_code, 6  );
    setbit_t(iso,64,"00000000", 8 );
    setbitoff(iso,48);
    BC_SetRcvMacKey(p_tiBcInsInf);
    BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
    BC_SendMessageToSrc(iso);
    return 0;
}

//发送安捷非绑定支付报文给账单系统
int BusZ2211_1(BcTransMsg *p_BcTransMsg,char *resp_code, ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_ins_inf_def *p_tiBcInsInf)
{
    int iLen;
    int iRet;
    int sqlCode;
    char buff[2048];
    char packet_type[5];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog1 = &g_TransMsg_o.BcTransLog;
    ISO_data iso;
    ISO_data *p_iso = &iso ;

    //组建报文
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0 != BC_SetIsoMode(p_iso, p_BcTransLog1->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ2211_1>BC_SetIsoMode failure(p_BcTransLog->trans_chnl:%s)。",__FILE__,__LINE__,p_BcTransLog->trans_chnl);
        BC_TRACE_END( -1 );
    }

    setbit_t(p_iso, 0 , "0870", 4); 
    rtrim(p_BcTransLog1->pri_acct_no);
    setbit_t(p_iso, 2 , p_BcTransLog1->pri_acct_no, strlen(p_BcTransLog1->pri_acct_no));
    setbit_t(p_iso, 3 , "811001", 6);
    setbit_t(p_iso, 4 , (unsigned char *)p_BcTransLog->trans_at, 12);
    setbit_t(p_iso, 5 , (unsigned char *)p_BcTransLog->fld_5, min_strlen(p_BcTransLog->fld_5,12));
    setbit_t(p_iso, 8 , (unsigned char *)p_BcTransLog->fld_8, min_strlen(p_BcTransLog->fld_8,8));
    setbit_t(p_iso, 11, (unsigned char *)p_BcTransLog1->sys_tra_no, 6);
    setbit_t(p_iso, 12, (unsigned char *)p_BcTransLog1->loc_trans_tm, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog1->loc_trans_dt, 4);
    setbit_t(p_iso, 28, (unsigned char *)resp_code, 6);	
    setbit_t(p_iso, 33, (unsigned char *)p_BcTransLog1->fwd_ins_id_cd, 8);
    setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog1->mchnt_cd, 15);
	  if(memcmp(resp_code,"N00000",6)==0)//交易成功
	  {
	  	setbit_t(p_iso, 15, (unsigned char *)p_BcTransLog->settle_dt, 4);
	  	setbit_t(p_iso, 37, (unsigned char *)p_BcTransLog->retri_ref_no, 12);
		}
    if ( 0 != *p_BcTransLog1->fld_45 )
    {
        strcpy(buff, p_BcTransLog1->fld_45);
        rtrim(buff);
        setbit_t(p_iso, 45 , (unsigned char *)buff, strlen(buff));
    }
    setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog1->trans_curr_cd, min_strlen(p_BcTransLog1->trans_curr_cd,30));
    setbit_t(p_iso, 64, "00000000", 8);

    //memset(buff, 0, sizeof(buff) );
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ2211_1>start call BC_SendMessageToAJ 发送数据给安捷。",__FILE__,__LINE__);

    //转发给账单系统 
    PrintISO( p_iso,"中心发送给账单系统非绑定支付报文应答",0);
    if ( '0' == *p_tiBcInsInf->key_type )
    {
        BC_SetMacKey(0,0);
    } else
    {
        BC_SetMacKey(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1);
    }

    //发送监控信息
    BC_SendMonitor_iso("0","0",p_BcTransLog1->trans_chnl,p_iso,p_tiBcTransInf);
    return BC_SendMessage(p_iso, p_BcTransLog1->trans_chnl );
}

//非绑定用户支付应答 0830
int BusZ2211(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int succeed = 1 ;
    int sys_succeed = 1 ;
    int iRet = -1   ;
    int sqlCode     ;
    char resp_code[6+1];
    char buff[256] ;

    ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog; // from db
    tl_bc_trans_log_def *p_BcTransLog1 = &g_TransMsg.BcTransLog;  // new resp
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;

    strcpy(p_BcTransLog->flag_lock, "0");
    BC_SendMonitor_iso("1","2", "AJXT", iso, p_tiBcTransInf);
    memcpy(resp_code,p_BcTransLog1->fld_28, 6);
    *(resp_code+6) = 0;

    // 获取中心商户信息
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->center_term_id);
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->center_mchnt_cd);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, g_rtIns_id_cd);
    iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到商户信息
        dcs_log(p_BcTransLog, sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_BcTransLog->center_mchnt_cd,sqlCode );
        BC_SendExeception("02","警告，非绑定用户应答交易获取中心商户信息失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->center_sys_tra_no,p_BcTransLog->center_term_id,p_BcTransLog->center_mchnt_cd);
        sys_succeed = 0;
    }

    //校验接收机构商户密钥
    if ( 1 == succeed )
    {
  	  if( 0!= BC_Search_ins_inf(buff, p_tiBcInsInf, "00945500"))
    	{
        	dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>查询安捷机构信息失败!",__FILE__,__LINE__);
        	BC_TRACE_END( -1 );
    	}
      if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) )
        {
            sys_succeed = 0;
            strcpy(resp_code,"N000A0");

            //异常日志
            dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>安捷商户MAC校验未通过.流水号:[%s],终端号:[%s],商户号:[%s] ",__FILE__,__LINE__,p_BcTransLog->sys_tra_no,p_BcTransLog->center_term_id,p_BcTransLog->center_mchnt_cd );
            BC_SendExeception("02","警告，商户MAC校验未通过.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->center_sys_tra_no,p_BcTransLog->center_term_id,p_BcTransLog->center_mchnt_cd);

            //异常入库
            tl_bc_exp_log_def tlBcExpLog;
            strcpy(tlBcExpLog.rsn_code,"00A0");
            strcpy(tlBcExpLog.rsn_desc,"MAC校验错");
            bcd_to_asc(tlBcExpLog.data, g_Rcv_Buffer, MIN(sizeof(tlBcExpLog.data),g_RcvBufferSize),0);
            tlBcExpLogOpr(GLB_DB_INSERT1, &tlBcExpLog, &sqlCode);
            if ( 0 != sqlCode )
            {
                dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcExpLogOpr 异常库记录入库失败.流水号:[%s],终端号:[%s],商户号:[%s]!",__FILE__,__LINE__,p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
                BC_SendExeception("03","警告，非绑定用户请求记录异常表失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            }
        }
    }

    //判断交易是否成功
    strcpy(p_BcTransLog->is_over, "1");
    strcpy(p_BcTransLog->fld_28, resp_code);
    strcpy(p_BcTransLog->resp_cd, resp_code+4);
    strcpy(p_BcTransLog->resp_cd_rcv, resp_code);
    strcpy(p_BcTransLog->flag_rcv, "2" );
    strcpy(p_BcTransLog->bill_id_rcv, p_BcTransLog1->fld_45);
    strcpy(p_BcTransLog->flag_6, "2" );
    if ( 0 != memcmp(resp_code,"N00000",6) )
    {
        //交易不成功
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e );
        strcpy(p_BcTransLog->bc_settle_in, "0");
        succeed = 0;
    } else
    {
        //交易成功
        chkCardnoList();
        SMS_Send(1);
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s );
        strcpy(p_BcTransLog->bc_settle_in, "1");
    }

    //获取#100接收机构信息
    memset(&tiBcInsInf.ins_id_cd, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy(tiBcInsInf.ins_id_cd, p_BcTransLog->rcv_ins_id_cd );
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211_1>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(账单系统)的机构信息 !",__FILE__,__LINE__, tiBcInsInf.ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // 查询机构信息失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2211_1>call tiBcInsInfOpr return failure,cann't find ins[%s] inf.",__FILE__,__LINE__, tiBcInsInf.ins_id_cd);
        BC_SendExeception("03","查询机构线路信息失败,机构id:[%s].",tiBcInsInf.ins_id_cd);    

        //依然认为成功,解锁
        sys_succeed = 0;
        strcpy(resp_code,"N00001");
    }

    //check field
    if ( 1 == succeed ){
        if ( 0 != BC_CheckIsoDataField(iso, "3,4,5,8,11,12,13,28,42,48,49") )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ2211>doBus_CheckIsoDataField failure.",__FILE__,__LINE__);        
            BC_SendExeception("03","警告，非绑定用户请求更新库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            sys_succeed = 0;
        }
    }
    //更新数据库
    strcpy(p_BcTransLog->settle_dt, p_BcTransLog1->settle_dt);
    strcpy(p_BcTransLog->retri_ref_no, p_BcTransLog1->retri_ref_no);
    dcs_debug(0,0, "<FILE:%s,LINE:%d>mchnt_cd[%s],term_id[%s],sys_tra_no[%s],loc_trans_dt[%s],settle_dt[%s]",__FILE__,__LINE__,p_BcTransLog->mchnt_cd,p_BcTransLog->term_id,p_BcTransLog->sys_tra_no,p_BcTransLog->loc_trans_dt,p_BcTransLog->settle_dt);
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211>tlBcTransLogOpr 更新库记录失败!",__FILE__,__LINE__);
        BC_SendExeception("03","警告，更新账单请求数据库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        sys_succeed = 0;
        strcpy(resp_code,"N00096");
    }

    //更新bill库
    BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
    iRet = tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        sys_succeed = 0;
        succeed = 0;
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211>tlBcBillLogOpr 更新失败(bill_id:%s,bill_id_rcv:%s, sqlCode:%d)!",__FILE__,__LINE__,p_tlBcBillLog->bill_id,p_tlBcBillLog->bill_id_rcv,sqlCode);
        BC_SendExeception("03","警告，账单BILL入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    } else
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211>tlBcBillLogOpr 更新库成功",__FILE__,__LINE__); 
    }     

    //给安捷应答
    BC_SendMessageToFolder_direct( (sys_succeed==1)?"N00000":resp_code,6);
    if ( 0 != iRet )
    {
        strcpy(resp_code,"96");
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>插入超时表失败",__FILE__,__LINE__);
        BC_SendExeception("03","警告，插入超时表失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }

    //获取#33域机构信息
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog->fwd_ins_id_cd );
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211_1>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(账单系统)的机构信息 !",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // 查询机构信息失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2211_1>call tiBcInsInfOpr return failure,cann't find ins[%s] inf.",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
        BC_SendExeception("03","查询机构线路信息失败,机构id:[%s].",p_tiBcInsInf->ins_id_cd);              
        return -1;
    }

    //应答给账单系统,重新组报文
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211>准备转发给账单系统(sys_succeed,%d)",__FILE__,__LINE__, sys_succeed);
    if ( 1 == sys_succeed )
    {
        BC_SetRcvMacKey(p_tiBcInsInf);
        return BusZ2211_1(&g_TransMsg_o,resp_code,p_tiBcTransInf,p_tiBcInsInf);
    }
    return 0;
}

//非绑定用户支付通知应答, -- 账单系统发起 0880
int BusZ2212(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet = -1   ;
    int succeed = 1 ;
    int sys_succeed = 1 ;    
    int sqlCode     ;
    char resp_code[6+1];
    char buff[256] ;

    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog; // from db
    tl_bc_trans_log_def *p_BcTransLog1 = &g_TransMsg.BcTransLog;  // new resp
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //依据原始交易进行处理

    BC_SendMonitor_trans("1","3",p_BcTransLog1, p_tiBcTransInf);

    //获取#33域机构信息
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog->fwd_ins_id_cd );
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcInsInfOpr(ins_id_cd:%s) 查询机构(账单系统)的机构信息 !",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
        // 查询机构信息失败
        strcpy(resp_code,"96");
        dcs_log(0, 0,"<FILE:%s,LINE:%d>call tiBcInsInfOpr return failure,cann't find ins[%s] inf.",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
        BC_SendExeception("查询机构信息失败,机构id:[%s].",p_tiBcInsInf->ins_id_cd);
        BC_UnLockTrans(p_BcTransLog_o);
        return -1;
    } else
    {
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);

        if ( *p_tiBcInsInf->key_type != '0' )
        {
            if ( 0!= BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) ){
                BC_UnLockTrans(p_BcTransLog_o);
                return -1;
            }
        }
    }

    //更新数据库，解锁
    strcpy(p_BcTransLog->flag_lock, "0");
    strcpy(p_BcTransLog->is_over, "1");
    //strcpy(p_BcTransLog->flag_pay, "2");
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d>tlBcTransLogOpr 更新库记录失败!",__FILE__,__LINE__);
        BC_SendExeception("03","警告，更新非绑定用户支付通知应答数据库记录失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        return -1;
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d>更新库记录成功!",__FILE__,__LINE__);
    return 0;
}

//绑定用户支付
int BusZ2220(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    return -1;
}


int BusZ2221(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    return 0;
}

//发送安捷交易结果查询报文
int BusZ2210_T_S(char *resp_code, ti_bc_ins_inf_def *p_tiBcInsInf, ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf )
{
    int iLen;
    int iRet;
    int sqlCode;
    char buff[4096];
    char packet_type[5];
    TimeOut_Key tk;
    
    ISO_data iso;
    BcTransMsg bcTransMsg;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //依据原始交易进行处理

    //组建报文
    memset(&iso,0,sizeof(ISO_data));
    clearbit(&iso);

    if ( 0 != BC_SetIsoMode(&iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d>设置机构报文类型失败。",__FILE__,__LINE__);
        return -1;
    }
    
    setbit_t(&iso, 0 , "0621", 4); 
    setbit_t(&iso, 3 , "610001", 6);
    setbit_t(&iso, 13, (unsigned char *)p_BcTransLog_o->loc_trans_dt, 4);
    setbit_t(&iso, 42, aj_mchnt_cd, 15);
    setbit_t(&iso, 46, (unsigned char *)p_BcTransLog_o->center_mchnt_cd, 15);
    setbit_t(&iso, 48, (unsigned char *)p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(&iso, 64, "00000000", 8);

    //发送监控信息

	BC_SendMonitor_iso("0","0", "AJXT", &iso, 0);

    //向安捷发送交易结果查询报文
    memset(buff, 0, sizeof(buff) );    
    PrintISO( &iso, "发送给安捷的交易结果查询报文", 0);
    iLen = BC_SendMessageToAJ(&iso, buff,p_tiBcInsInf);
    if ( iLen <= 0 )
    {
        //发送给安捷失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d>send to rcv failure.",__FILE__,__LINE__);

        //更新库记录
        BC_SendExeception("03","警告，发送安捷商户交易结果信息失败.");
        strcpy(resp_code,"N00098");
        return -1;
    }

    //解包
    memset(&iso,0,sizeof(ISO_data));
    clearbit(&iso);
    if ( 0!= BC_SetIsoMode(&iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d>BC_SetIsoMode failure。",__FILE__,__LINE__);
        return -1;
    }
    dcs_debug(0,0,"<FILE:%s,LINE:%d>start call BC_UnPack_Data 解包安捷数据。",__FILE__,__LINE__);
    iRet = BC_UnPack_Data( &iso, buff,  iLen);
    PrintISO( &iso, "收到安捷交易结果应答报文", 0);

    //发送给安捷
    dcs_debug(0, 0,"<FILE:%s,LINE:%d>已经收到安捷交易结果回复报文.",__FILE__,__LINE__);
    if ( iRet >= 0 )
    {
        BC_SendMonitor_iso("1","2", "AJXT",&iso, 0);
        dcs_debug(0, 0,"<FILE:%s,LINE:%d>交易结果回复报文解包成功.",__FILE__,__LINE__);

        memset(&bcTransMsg, 0, sizeof(bcTransMsg));
        iRet = BC_CopyISOdata( &iso, &bcTransMsg);
        if ( iRet <0 )
        {
            strcpy(resp_code,"N00030");
            return -1;
        }

        dcs_debug(0, 0,"<FILE:%s,LINE:%d>start to 检查收到的交易结果回复报文.",__FILE__,__LINE__);

        //check
        if ( 0 != memcmp(bcTransMsg.BcTransLog.msg_tp, "0631", 4) 
             || 0 != memcmp(bcTransMsg.BcTransLog.proc_cd, "610001", 6) 
             || 0 != memcmp(bcTransMsg.BcTransLog.fld_46, p_BcTransLog_o->center_mchnt_cd,15)
           )
        {
            dcs_log(buff,50,"<FILE:%s,LINE:%d>check #0 #3 #46[%s](%s) failure。",__FILE__,__LINE__, bcTransMsg.BcTransLog.fld_46, p_BcTransLog_o->center_mchnt_cd);
            strcpy(resp_code,"N00096");
            return -1;
        }

        //update 订单信息
        strcpy(resp_code,bcTransMsg.BcTransLog.fld_28);
        if ( 0 == memcmp(resp_code, "N00000", 6) )
        {
            //查询成功
            dcs_log(buff,50,"<FILE:%s,LINE:%d>查询成功，准备更新相关支付交易状态。",__FILE__,__LINE__ );
            int i;
            int len = strlen(bcTransMsg.BcTransLog.fld_48);
            memcpy(buff, bcTransMsg.BcTransLog.fld_48, len);
            rtrim(buff);
            for ( i=0; i< len/20; i++ )
            {
                tl_bc_trans_log_def bcTransLog;
                memset(&bcTransLog, 0, sizeof(tl_bc_trans_log_def));
                memcpy(bcTransLog.loc_trans_dt, buff+i*20, 4);
                memcpy(bcTransLog.fld_28,       buff+i*20+4 , 6);
                if(memcmp(bcTransLog.fld_28, "N00000", 6) == 0 && memcmp(buff+i*20+16, "0000", 4))
                {
                	memcpy(bcTransLog.settle_dt, buff+i*20+16, 4);
                	strcpy(bcTransLog.bc_settle_in, "1");
                }
                else
                {
                	strcpy(bcTransLog.settle_dt, " ");
                	strcpy(bcTransLog.bc_settle_in, "0");
                }
                strcpy(bcTransLog.retri_ref_no, "  ");
                memcpy(bcTransLog.center_sys_tra_no,   buff+i*20+10, 6);
                memcpy(bcTransLog.center_mchnt_cd  ,   p_BcTransLog_o->center_mchnt_cd, 15);
                tlBcTransLogOpr(GLB_DB_UPDATE3, &bcTransLog, &sqlCode);
                if ( 0!= sqlCode )
                {
                    dcs_log(&bcTransLog,sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d>根据查询结果更新交易记录信息失败sqlcode[%d]。",__FILE__,__LINE__,sqlCode);
                }
				tlBcBillLogUpdate(GLB_DB_UPDATE1,&bcTransLog, &sqlCode);
               dcs_debug(0,0,"<FILE:%s,LINE:%d>根据查询结果更新交易记录信息成功center_sys_tra_no[%s]。",__FILE__,__LINE__,bcTransLog.center_sys_tra_no);
               
		    		   memset(&tk, ' ', sizeof(TimeOut_Key));
				       memcpy(tk.sys_tra_no, bcTransLog.center_sys_tra_no, 6);
				       memcpy(tk.mchnt_cd, bcTransLog.center_mchnt_cd, 15);
				       tk.end = '\0';
		        	 memcpy(g_TimeOutRec.caKey, (char *)&tk, sizeof(TimeOut_Key) );
        	    
        	     //删除账单超时表
        	     BC_DeleteBillNotify(&g_TimeOutRec);
              break;
            }
        }

        return 0;
    }
    return -1;
}

//0820 交易超时的处理
int BusZ2210_T( )
{
	struct TimeOut_REC *pTimeOutRec;
	
	ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf;
	ti_bc_trans_det_def *p_tiBcTransDet = &g_tiBcTransDet;
		
  //发往安捷的非绑定支付账单交易超时，需要发送交易结果查询报文
    int iLen;
    int iRet;
    int sqlCode;
    char resp_code[6+1];
    char buff[2048],temp[256];

    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;//原始交易信息
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息   
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //商户信息
    
    if ( *p_BcTransLog_o->is_over == '1' )
    {
        //交易处理结束，无需进行交易超时处理
        BC_SendExeception("01","超时处理程序，发现该笔交易已经处理结束，无需进行超时处理.（mchnt_cd:[%s],sys_tra_no:[%s]）",p_BcTransLog_o->mchnt_cd, p_BcTransLog_o->sys_tra_no);

        //原交易解锁
        BC_UnLockTrans(p_BcTransLog_o);
        return 0;
    }

    // 获取清算商户信息
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210_T>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog_o->mchnt_cd);
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog_o->center_mchnt_cd);
    strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog_o->center_term_id);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到商户信息
        dcs_log(0, 0,"<FILE:%s,LINE:%d>cant not find qs mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd,sqlCode );

        //原交易解锁
        BC_UnLockTrans(p_BcTransLog_o);

        //交易失败
        return -1;
    }

    //获取接收机构信息
    dcs_debug(0,0,"<FILE:%s,LINE:%d>start to call tiBcInsInfOpr for select.",__FILE__,__LINE__,RPT_TO_LOG,0,0);
    memset( &tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy( tiBcInsInf.ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode  || '0' == *tiBcInsInf.openflag )
    {
        dcs_log(0, 0,"<FILE:%s,LINE:%d>call BC_CheckIsoDataField  #33域机构不存在.",__FILE__,__LINE__);
        BC_SendExeception("02","#33域机构不存在.");

        //原交易解锁
        BC_UnLockTrans(p_BcTransLog_o);
        return -1;
    }

    //发送安捷交易结果查询报文
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call BusZ2210_T_S 向安捷发送交易结果查询。",__FILE__,__LINE__); 
    iRet = BusZ2210_T_S( resp_code , &tiBcInsInf, p_tiBcQsMchntInf);
    if ( iRet < 0 )
    {
        dcs_log(0, 0,"<FILE:%s,LINE:%d>查询交易结果失败.",__FILE__,__LINE__);

        //原交易解锁
        BC_UnLockTrans(p_BcTransLog_o);
				return -1;
    }
    //原交易解锁
    BC_UnLockTrans(p_BcTransLog_o);
    return 0;
}

void chkCardnoList()
{
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    tl_bc_cardno_list_inf_def tlBcCardnoListInf, *p_tlBcCardnoListInf;
    char tmpBuf[1024];
    char idStr[20];
    int sqlCode;
    p_tlBcCardnoListInf = &tlBcCardnoListInf;
    memset(p_tlBcCardnoListInf, 0, sizeof(tl_bc_cardno_list_inf_def));
    rtrim(p_BcTransLog_o->pname);
    rtrim(p_BcTransLog_o->pri_acct_no);
    rtrim(p_BcTransLog_o->fld_5);
    rtrim(p_BcTransLog_o->id_type);
    rtrim(p_BcTransLog_o->fld_61);
    strcpy(p_tlBcCardnoListInf->cardno, p_BcTransLog_o->pri_acct_no);
    memset(p_tlBcCardnoListInf->relation1, ' ', sizeof(p_tlBcCardnoListInf->relation1));
    memcpy(p_tlBcCardnoListInf->relation1, p_BcTransLog_o->fld_5, strlen(p_BcTransLog_o->fld_5));
    memcpy(p_tlBcCardnoListInf->relation1+15, p_BcTransLog_o->id_type, strlen(p_BcTransLog_o->id_type));
    memcpy(p_tlBcCardnoListInf->relation1+17, p_BcTransLog_o->fld_61, strlen(p_BcTransLog_o->fld_61));
    memcpy(p_tlBcCardnoListInf->relation1+37, p_BcTransLog_o->pname, MIN(strlen(p_BcTransLog_o->pname),62));
    tlBcCardnoListInfOpr(GLB_DB_SELECT1, p_tlBcCardnoListInf, &sqlCode);
    if (0 == sqlCode)
    {
    	switch(p_tlBcCardnoListInf->index)
    	{
    		case '1':
    			return;
    		case '2':
    		case '3':
    		case '4':
    		case '5':
    			sprintf(tmpBuf,"卡号[%s],支付手机[%s],%s号[%s],姓名[%s],原交易信息[%s]",p_BcTransLog_o->pri_acct_no,p_BcTransLog_o->fld_5,getIdStr(p_BcTransLog_o->id_type, idStr),p_BcTransLog_o->fld_61,p_BcTransLog_o->pname,p_tlBcCardnoListInf->relation1);
    			break;
    		default:
    			sprintf(tmpBuf,"卡号[%s], 已保存5个，请进行清理！支付手机[%s],%s号[%s],姓名[%s]",p_BcTransLog_o->pri_acct_no,p_BcTransLog_o->fld_5,getIdStr(p_BcTransLog_o->id_type, idStr),p_BcTransLog_o->fld_61,p_BcTransLog_o->pname);
				break;    			
    	}
		BC_SendExeception("03", "警告, %s", tmpBuf);
		if( 0 > BC_exp_log("CA", tmpBuf, g_Rcv_Buffer, g_RcvBufferSize))
		{
	  		BC_SendExeception("03","警告, 记录异常表失败.");
	  		dcs_log(0, 0, "<FILE:%s,LINE:%d>,记录异常表失败.",__FILE__,__LINE__);
		}
		dcs_log(0,0,"<FILE:%s,LINE:%d>%s\n",__FILE__,__LINE__, tmpBuf);
    }
}

char* getIdStr(char *idType, char *idStr)
{
	memset(idStr, 0, sizeof(idStr));
	if(memcmp("00", idType, 2) == 0)strcpy(idStr,"身份证");
	else if(memcmp("01", idType, 2) == 0)strcpy(idStr,"护照");
	else if(memcmp("02", idType, 2) == 0)strcpy(idStr,"军官证");
	else if(memcmp("03", idType, 2) == 0)strcpy(idStr,"士兵证");
	else if(memcmp("04", idType, 2) == 0)strcpy(idStr,"回乡证");
	else if(memcmp("05", idType, 2) == 0)strcpy(idStr,"户口本");
	else if(memcmp("06", idType, 2) == 0)strcpy(idStr,"外国护照");
	else strcpy(idStr,"其它证件");		
	return idStr;
}
