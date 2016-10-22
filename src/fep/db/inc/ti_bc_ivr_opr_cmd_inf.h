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


#ifndef _TI_BC_IVR_OPR_CMD_INF_
#define _TI_BC_IVR_OPR_CMD_INF_

typedef struct{
      char dest_opr_cmd           [4  + 1 ];
      char src_opr_cmd           [4  + 1 ];
}ti_bc_ivr_opr_cmd_inf_def;

int tiBcIvrOprCmdInfOpr(int oprType, ti_bc_ivr_opr_cmd_inf_def *p_tiBcIvrOprCmdInf, int *p_sqlCode);
#endif
