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


#ifndef _TI_BC_IVR_MOBILE_INF_
#define _TI_BC_IVR_MOBILE_INF_

typedef struct{
      char mobileno        [20  + 1 ];
      char preno           [20  + 1 ];
}ti_bc_ivr_mobile_inf_def;

int tiBcIvrMobileInfOpr(int oprType, ti_bc_ivr_mobile_inf_def *p_tiBcIvrMobileInf, int *p_sqlCode);
#endif
