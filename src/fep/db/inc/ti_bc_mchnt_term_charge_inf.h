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


#ifndef _TI_BC_MCHNT_TERM_CHARGE_DET_
#define _TI_BC_MCHNT_TERM_CHARGE_DET_

typedef struct{
    char mchnt_tp     [  4 + 1 ];
    char term_id      [  8 + 1 ];
	  char mchnt_cd     [ 20 + 1 ];
	  char rcv_ins_id_cd[  8 + 1 ];
	  char term_id_c    [  8 + 1 ];
	  char mchnt_cd_c   [ 20 + 1 ];
	  char sys_tra_no   [  6 + 1  ];
}ti_bc_mchnt_term_charge_inf_def;

int tiBcMchntTermChargeInfOpr(int oprType, ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf, int *p_sqlCode);
#endif
