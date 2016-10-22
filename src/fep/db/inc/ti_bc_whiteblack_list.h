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


#ifndef _TI_BC_WHITEBLACK_LIST_
#define _TI_BC_WHITEBLACK_LIST_

typedef struct{
      char card_idno_inf       [32  + 1 ];
      char mchnt_cd            [15  + 1 ];
      char list_flag           [1   + 1 ];
      char rec_crt_ts          [14  + 1 ];
      char rec_upd_ts          [14  + 1 ];
}ti_bc_whiteblack_list_def;

int tiBcWhiteblackListOpr(int oprType, ti_bc_whiteblack_list_def *p_tiBcWhiteblackList, int *p_sqlCode);
#endif
