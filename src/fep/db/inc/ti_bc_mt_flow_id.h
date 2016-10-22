/*
 *  Copyright 2008, China UnionPay Co., Ltd.  All right reserved.
 *
 *
 *  $Id: ti_bc_mchnt_term_charge_inf.h,v 1.0 2010-10-21 14:56:04 weichai Exp $
 *
 *  Edit History:
 *    2010-10-21 14:57:17 - Created by weichai.
 *
 */


#ifndef _TI_BC_MT_FLOW_ID_
#define _TI_BC_MT_FLOW_ID_

typedef struct{
    char term_id      [  8 + 1 ];
	  char mchnt_cd     [ 20 + 1 ];
	  char sys_tra_no   [  6 + 1 ];
}ti_bc_mt_flow_id_def;

int tiBcMTFlowIdOpr(int oprType, ti_bc_mt_flow_id_def *p_tiBcMTFlowId, int *p_sqlCode);
#endif
