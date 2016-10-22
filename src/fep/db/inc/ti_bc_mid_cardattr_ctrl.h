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


#ifndef _TI_BC_MID_CARDATTR_CTRL_
#define _TI_BC_MID_CARDATTR_CTRL_

typedef struct{
      char card_attr       [2  + 1 ];
      char mchnt_cd        [15  + 1 ];
      char period          [1  + 1 ];
      int limit_times   ;
      char limit_amt       [12  + 1 ];
}ti_bc_mid_cardattr_ctrl_def;

int tiBcMidCardattrCtrlOpr(int oprType, ti_bc_mid_cardattr_ctrl_def *p_ti_bc_mid_cardattr_ctrl, int *p_sqlCode);
#endif
