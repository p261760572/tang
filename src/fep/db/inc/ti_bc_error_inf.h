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


#ifndef _TI_BC_ERR_INF_
#define _TI_BC_ERR_INF_

typedef struct
{
	char resp_code[6 + 1];
    char fwd_ins_id_cd[8 + 1];
    char rcv_ins_id_cd[8 + 1];
    char fld_28 [6 + 1];
    char resp_cd [2 + 1];
    char msg [200 + 1];
}ti_bc_err_inf_def;

int tiBcErrInfOpr(int oprType, ti_bc_err_inf_def *p_tiBcErrInf, int *p_sqlCode);

#endif
