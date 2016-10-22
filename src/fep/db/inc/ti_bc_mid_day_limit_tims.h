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


#ifndef _TI_BC_MID_DAY_LIMIT_TIMS_
#define _TI_BC_MID_DAY_LIMIT_TIMS_

typedef struct{
      char fwd_ins_id_cd [8  + 1 ];
      int debit_times_day  ;
      int credit_times_day ;
}ti_bc_mid_day_limit_tims_def;

int tiBcMidDayLimitTimsOpr(int oprType, ti_bc_mid_day_limit_tims_def *p_ti_bc_mid_day_limit_tims, int *p_sqlCode);
#endif
