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


#ifndef _TI_BC_TERM_DET_
#define _TI_BC_TERM_DET_

typedef struct{

 char term_id         [8  + 1 ];
 char term_nm         [40 + 1 ];
 char openflag     	  [1  + 1 ];
 char mkey_index      [3  + 1 ];
 char mkey_1          [16 + 1 ];
 char mkey_2          [16 + 1 ];
 char mkey_3          [16 + 1 ];
}ti_bc_term_inf_def;

int tiBcTermInfOpr(int oprType, ti_bc_term_inf_def *p_tiBcTermInf, int *p_sqlCode);
#endif
