/*
 *  Copyright 2008, China UnionPay Co., Ltd.  All right reserved.
 *
 *
 *  $Id: ti_bc_card_bin.h,v 1.0 2008-11-21 14:56:04 weichai Exp $
 *
 *  Edit History:
 *    2008-11-21 14:57:17 - Created by weichai.
 *
 */


#ifndef _TI_BC_APP_FORK_INF_
#define _TI_BC_APP_FORK_INF_

typedef struct{
    int  app_id ;
    char app_name  [30 + 1] ;
    int fork_num ;
	int max_fork_num;
}ti_bc_app_fork_inf_def;

int tiBcAppForkInfOpr(int oprType, ti_bc_app_fork_inf_def *p_tiBcAppForkInf, int *p_sqlCode);

#endif
