/*
 *  Copyright 2008, China UnionPay Co., Ltd.  All right reserved.
 *
 *
 *  $Id: ti_bc_trans_inf.h,v 1.0 2008-11-21 14:56:04 weichai Exp $
 *
 *  Edit History:
 *    2008-11-21 14:57:17 - Created by weichai.
 *
 */


#ifndef _TI_BC_QS_MCHNT_INF_
#define _TI_BC_QS_MCHNT_INF_

typedef struct{
      /*char acq_ins_id_cd   [8   + 1 ];*/
      char mchnt_cd  	        [20  + 1 ];
      char term_id   	        [ 8  + 1 ];
      char rcv_ins_id_cd     [8   + 1 ];
      char mchnt_cn_abbr     [40  + 1 ];
      /*char mchnt_nm        [200 + 1 ];
      char settle_ins_id_cd  [13  + 1 ];
      char settle_acct       [32  + 1 ];
      char acct_nm           [80  + 1 ];
      char bank_nm           [40  + 1 ];
      char area_cd           [ 6  + 1 ];
	    char send_msg_flag     [ 1  + 1 ];
      char linkMan           [20  + 1 ];
      char email             [60  + 1 ];
      char linktel           [20  + 1 ];
      char verifi_flag       [1   + 1 ];
      char verifi_id         [8   + 1 ];
      char pay_flag          [1   + 1 ];
      char pay_id            [8   + 1 ];*/
      char reversal_mode     [1   + 1 ];/*
      char using_blacklist   [1   + 1 ];
      char blacklist         [60  + 1 ];
      char using_whitelist   [1   + 1 ];
      char whitelist         [60  + 1 ];*/      
      char openflag       	  [1   + 1 ];
      char mkey_index        [3   + 1 ];
      char mkey_1            [16  + 1 ];
      char mkey_2            [16  + 1 ];
      char mkey_3            [16  + 1 ];
/*      char rec_crt_ts	     [14  + 1 ];
      char rec_upd_ts	     [14  + 1 ];*/
}ti_bc_qs_mchnt_inf_def;

int tiBcQsMchntInfOpr(int oprType, ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf, int *p_sqlCode);
#endif
