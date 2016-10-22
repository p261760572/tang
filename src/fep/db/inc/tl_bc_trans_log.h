/*
 *  Copyright 2008, China UnionPay Co., Ltd.  All right reserved.
 *
 *
 *  $Id: ti_ise_ins_inf.h,v 1.0 2008-11-21 14:56:04 weichai Exp $
 *
 *  Edit History:
 *    2008-11-21 14:57:17 - Created by weichai.
 *
 */


#ifndef _TL_BC_TRANS_LOG_
#define _TL_BC_TRANS_LOG_

typedef struct{
  char sys_dt_tm            [ 14  + 1 ];
  char settle_dt            [ 4   + 1 ];
  char trans_flag           [ 4   + 1 ];
  char is_over              [ 1   + 1 ];
  char lock_tm              [ 10  + 1 ];
  char flag_lock            [ 1   + 1 ];
  char flag_verifi          [ 1   + 1 ];
  char flag_rcv             [ 1   + 1 ];
  char flag_pay             [ 1   + 1 ];
  char flag_reversal        [ 1   + 1 ];
  char resp_cd_verifi       [ 6   + 1 ];
  char resp_cd_rcv          [ 6   + 1 ];
  char resp_cd_pay          [ 6   + 1 ];
  char resp_cd_reversal     [ 6   + 1 ];
  char trans_chnl           [30   + 1 ];  
  char packet_type          [4    + 1 ];
  char line_nm_verifi       [4    + 1 ]; 
  char line_nm_rcv          [4    + 1 ]; 
  char line_nm_pay          [4    + 1 ];
  char reversal_id          [ 8   + 1 ];
  char bc_settle_in         [ 1   + 1 ];
  char fld32_ins_id_cd      [ 13  + 1 ];
  char fwd_ins_id_cd        [ 13  + 1 ];
  char rcv_ins_id_cd        [ 13  + 1 ];
  char msg_tp               [ 4   + 1 ];
  char pri_acct_no          [ 21  + 1 ];
  char proc_cd              [ 6   + 1 ];
  char trans_at             [ 12  + 1 ];
  char transmsn_dt_tm       [ 10  + 1 ];
  char sys_tra_no           [ 6   + 1 ];
  char loc_trans_tm         [ 6   + 1 ];
  char loc_trans_dt         [ 4   + 1 ];
  char mchnt_tp             [ 4   + 1 ];
  char pos_entry_md_cd      [ 3   + 1 ];
  char pos_cond_cd          [ 2   + 1 ];
  char retri_ref_no         [ 12  + 1 ];
  char auth_id_resp_cd      [ 6   + 1 ];
  char resp_cd              [ 2   + 1 ];
  char term_id              [ 8   + 1 ];
  char mchnt_cd             [ 15  + 1 ];
  char area_cd              [ 6   + 1 ];
  char trans_curr_cd        [ 3   + 1 ];
  char flag_1               [ 1   + 1 ];
  char flag_2               [ 1   + 1 ];
  char flag_3               [ 1   + 1 ];
  char flag_4               [ 1   + 1 ]; 
  char flag_5               [ 1   + 1 ];
  char flag_6               [ 1   + 1 ];
  char flag_7               [ 1   + 1 ];
  char fld_5                [12  + 1 ];
  char fld_8                [ 8  + 1 ];
  char fld_28               [ 6  + 1 ];
  char fld_34               [28  + 1 ];
  char fld_44               [ 27 + 1 ];
  char fld_45               [ 76 + 1 ];
  char fld_46               [ 30 + 1 ]; 
  char fld_48               [190 + 1 ];
  char fld_61               [ 30 + 1 ]; 
  char center_sys_tra_no    [ 6   + 1 ];
  char center_term_id       [ 8   + 1 ];
  char center_mchnt_cd      [ 20  + 1 ];
  char bill_id_rcv          [ 21  + 1 ];
  char card_attr            [ 2   + 1 ]; 
  char iss_ins_id_cd        [ 8   + 1 ]; 
  char pname                [ 210 + 1 ]; 
  char encrypt_info         [ 48 + 1 ];
  char id_type	            [ 2 + 1 ];
  char originaDataElements  [42 + 2];
  //²»´æÊý¾Ý¿â
  char addtnl_amt 			[ 40 + 1 ];
  char track_2_data			[ 37 + 1 ];
  char track_3_data			[107 + 1 ];
  char pin_data				[  8 + 1 ];  
}tl_bc_trans_log_def;

int tlBcTransLogOpr(int oprType, tl_bc_trans_log_def *p_tlBcTransLog, int *p_sqlCode);
int tlBcTransLogOpr2(double *out_at, double *out_num, char *_month, char *_pri_acct_no ,char *_mchnt_cd,int *p_sqlCode);
int tlBcTransLogOpr3(long long *out_at, long long *out_num, char *_month, char *_pri_acct_no ,char *_mchnt_cd,int *p_sqlCode);

#endif
