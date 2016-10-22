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


#ifndef _TI_BC_WHITE_LIST_
#define _TI_BC_WHITE_LIST_

typedef struct{
      char card_idno_inf       [32  + 1 ];
      char mchnt_cd            [15  + 1 ];
}ti_bc_white_list_def;

int tiBcWhiteListOpr(int oprType, ti_bc_white_list_def *p_tiBcWhiteList, int *p_sqlCode);
#endif
