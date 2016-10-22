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


#ifndef _TI_BC_IVR_SETTLE_INF_
#define _TI_BC_IVR_SETTLE_INF_

typedef struct{
      char opr_cmd            [4  + 1 ];
      char rspcode            [6  + 1 ];
      char opr_list           [2  + 1 ];
      int  seq_no             ;
      char data_list          [1024  + 1 ];
      int  data_len           ;
      char data_flag          [1  + 1 ];
      char spe_cmd            [2  + 1 ];
      int  spe_length         ;
      char memo               [500  + 1 ];
}ti_bc_ivr_settle_inf_def;

int tiBcIvrSettleInfOpr(int oprType, ti_bc_ivr_settle_inf_def *p_tiBcIvrSettleInf, int *p_sqlCode);
#endif
