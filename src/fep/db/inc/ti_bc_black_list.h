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


#ifndef _TI_BC_BLACK_LIST_
#define _TI_BC_BLACK_LIST_

typedef struct{
      char card_idno_inf       [32  + 1 ];
}ti_bc_black_list_def;

int tiBcBlackListOpr(int oprType, ti_bc_black_list_def *p_tiBcBlackList, int *p_sqlCode);
#endif
