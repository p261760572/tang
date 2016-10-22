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


#ifndef _TI_BC_IVR_OPR_IMPDATA_INF_
#define _TI_BC_IVR_OPR_IMPDATA_INF_

typedef struct{
      char opr_list           [2  + 1 ];
      char keyid          [30  + 1 ];
      int  data_len          ;
      char data_flag          [1  + 1 ];
      char pin_flag          [1  + 1 ];
      char expand_flag          [1  + 1 ];
}ti_bc_ivr_opr_impdata_inf_def;

int tiBcIvrOprImpdataInfOpr(int oprType, ti_bc_ivr_opr_impdata_inf_def *p_tiBcIvrOprImpDataInf, int *p_sqlCode);
#endif
