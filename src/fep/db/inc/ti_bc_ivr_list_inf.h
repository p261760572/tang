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


#ifndef _TL_BC_IVR_LIST_INF_
#define _TL_BC_IVR_LIST_INF_

typedef struct{
      char packkeyid          [100  + 1 ];
      char foldername         [30  + 1 ];
      char createdate         [8  + 1 ];
}ti_bc_ivr_list_inf_def;

int tiBcIvrListInfOpr(int oprType, ti_bc_ivr_list_inf_def *p_tiBcIvrListInf, int *p_sqlCode);
#endif
