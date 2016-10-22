/*
 *  Copyright 2008, China UnionPay Co., Ltd.  All right reserved.
 *
 *
 *  $Id: tl_bc_bill_log.h,v 1.0 2010-09-18 14:56:04 weichai Exp $
 *
 *  Edit History:
 *    2010-09-18 14:57:17 - Created by weichai.
 *
 */


#ifndef _TL_BC_BILL_LOG_
#define _TL_BC_BILL_LOG_

typedef struct
{
    /*
    char trans_chnl         [ 30  + 1 ]; */
    char bc_settle_in       [ 1   + 1 ]; /*
    char msg_tp             [ 4   + 1 ];
    char pri_acct_no        [ 21  + 1 ];
    char proc_cd            [ 6   + 1 ]; */
    char trans_at           [ 12  + 1 ]; 
    char mobile_no          [ 30  + 1 ]; /*
    char transmsn_dt_tm     [ 10  + 1 ];
    char bus_type           [ 8   + 1 ]; */
    char sys_tra_no         [ 6   + 1 ];
    char loc_trans_tm       [ 6   + 1 ];
    char loc_trans_dt       [ 4   + 1 ];
    char settle_dt          [ 8   + 1 ]; /*
    char mchnt_tp           [ 4   + 1 ];
    char pos_entry_md_cd    [ 3   + 1 ];
    char pos_cond_cd        [ 2   + 1 ]; */
    char resp_code          [ 6   + 1 ]; /*
    char fld32_ins_id_cd    [ 13  + 1 ]; */
    char fwd_ins_id_cd      [ 13  + 1 ]; /*
    char payment_no         [ 28  + 1 ];*/
    char retri_ref_no       [ 12  + 1 ];   /*
    char auth_id_resp_cd    [ 6   + 1 ]; */
    char resp_cd            [ 2   + 1 ]; 
    char mchnt_cd           [ 15  + 1 ]; /*
    char term_id            [ 8   + 1 ];  
    char card_accptr_nm_loc [ 40  + 1 ]; */
    char bill_id            [ 76  + 1 ]; /*
    char trans_mchnt_cd     [ 15  + 1 ];
    char goods_desc         [ 550 + 1 ];
    char trans_curr_cd      [ 3   + 1 ];
    char resident_id        [ 18  + 1 ];
    char rcv_ins_id_cd      [ 13  + 1 ]; */
    char rec_crt_ts         [ 14  + 1 ];
    char rec_upd_ts         [ 14  + 1 ]; 
    char bill_id_rcv        [ 21  + 1 ];
}tl_bc_bill_log_def;

int tlBcBillLogOpr(int oprType, tl_bc_bill_log_def *p_tlBcBillLog, int *p_sqlCode);
int tlBcBillLogOpr2(int oprType, tl_bc_bill_log_def *p_tlBcBillLog, int *p_sqlCode, int *ReturnNum);
#endif
