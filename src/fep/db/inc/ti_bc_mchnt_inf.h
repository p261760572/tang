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


#ifndef _TI_BC_MCHNT_INF_
#define _TI_BC_MCHNT_INF_

typedef struct{
      char acq_ins_id_cd            [8   + 1 ];
      char mchnt_cd  	              [15  + 1 ];
      char mchnt_cn_abbr            [40  + 1 ];
      char mchnt_nm                 [200 + 1 ];
      char settle_ins_id_cd         [13  + 1 ];
      char settle_acct              [32  + 1 ];
      char acct_nm                  [80  + 1 ];
      char bank_nm                  [40  + 1 ];
      char area_cd                  [ 6  + 1 ];
      char send_msg_flag            [ 1  + 1 ];
      char linkMan                  [20  + 1 ];
      char email                    [60  + 1 ];
      char linktel                  [20  + 1 ];
      char verifi_flag              [1   + 1 ];
      char verifi_id                [8   + 1 ];
      char pay_flag                 [1   + 1 ];
      char pay_id                   [8   + 1 ];
      char reversal_mode            [1   + 1 ];
      char using_blacklist          [1   + 1 ];
      char blacklist                [60  + 1 ];
      char using_whitelist          [1   + 1 ];
      char whitelist                [60  + 1 ]; 
      char debit_rcv_ins_id_cd      [ 8  + 1 ]; /* 借记卡接收机构代码	，为空根据卡bin路由                          */
	    char credit_rcv_ins_id_cd     [ 8  + 1 ]; /* 信用卡接收机构代码	，为空根据卡bin路由                          */
	    char credit_enable_flag       [ 1  + 1 ]; /* 信用卡支持标志 0：不支持 1：支持                                */
      char auto_route_flag          [ 1  + 1 ]; /* 卡是否自动路由标志 0：不支持卡自动路由1：支持卡自动路由         */
	    char credit_get_flag          [ 1  + 1 ]; /* 信用卡关键信息获取方式 0：通过IVR系统获得1：受理方报文发送获得  */
      char debit_limit_times        [ 12 + 1 ]; /* 借记卡每次限额 为0表示不限制                                    */
      char credit_limit_times       [ 12 + 1 ]; /* 贷记卡每次限额为0表示不限制                                     */
      char limit_day                [ 12 + 1 ]; /* 商户每天限额为0表示不限制                                       */
      char openflag                 [1   + 1 ];       
      /*
      char mkey_index               [3   + 1 ];
      char mkey_1                   [16  + 1 ];
      char mkey_2                   [16  + 1 ];
      char mkey_3                   [16  + 1 ];
      char rec_crt_ts	              [14  + 1 ];
      char rec_upd_ts	              [14  + 1 ];
      */
}ti_bc_mchnt_inf_def;

int tiBcMchntInfOpr(int oprType, ti_bc_mchnt_inf_def *p_tiBcMchntInf, int *p_sqlCode);
#endif
