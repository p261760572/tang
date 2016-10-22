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


#ifndef _TI_BC_ROUTE_CARD_BIN_
#define _TI_BC_ROUTE_CARD_BIN_

typedef struct{
      char card_bin       [21  + 1 ];
      char rcv_ins_id_cd  [8  + 1 ];
      char card_attr      [2  + 1 ];
      char iss_ins_id_cd  [8  + 1 ];
      char enable_flag    [1  + 1 ];
      char get_inf_flag   [1  + 1 ];
}ti_bc_route_card_bin_def;

int tiBcRouteCardBinOpr(int oprType, ti_bc_route_card_bin_def *p_tiBcRouteCardBin, int *p_sqlCode);
#endif
