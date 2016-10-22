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


#ifndef _TI_BC_ROUTE_GATEWAY_INF_
#define _TI_BC_ROUTE_GATEWAY_INF_

typedef struct{
      char rcv_ins_id_cd       [8  + 1 ];
      char iss_ins_id_cd       [8  + 1 ];
      char gate_id             [15  + 1 ];
}ti_bc_route_gateway_inf_def;

int tiBcRouteGatewayInfOpr(int oprType, ti_bc_route_gateway_inf_def *p_tiBcRouteGatewayInf, int *p_sqlCode);
#endif
