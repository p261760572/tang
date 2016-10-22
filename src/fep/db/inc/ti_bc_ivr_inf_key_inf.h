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


#ifndef _TI_BC_IVR_INF_KEY_INF_
#define _TI_BC_IVR_INF_KEY_INF_

typedef struct{
      char opr_cmd            [4  + 1 ];
      char rspcode            [6  + 1 ];
      char keyid              [30  + 1 ];
      char keymemo            [100  + 1 ];
      char keyflag            [1  + 1 ];
}ti_bc_ivr_inf_key_inf_def;

int tiBcIvrInfKeyInfOpr(int oprType, ti_bc_ivr_inf_key_inf_def *p_tiBcIvrInfKeyInf, int *p_sqlCode);
#endif
