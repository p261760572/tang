/*
 *  Copyright 2008, China UnionPay Co., Ltd.  All right reserved.
 *
 *
 *  $Id: tl_bc_bill_log.h,v 1.0 2010-09-18 14:56:04 weichai Exp $
 *
 *  Edit History:
 *    2010-09-18 14:57:17 - Created by weichai.
 *
 */


#ifndef _TL_BC_EXP_LOG_
#define _TL_BC_EXP_LOG_

typedef struct
{
    char sys_date [8+1];
    char sys_time [6+1];
    char rsn_code [4+1];
    char rsn_desc [200+1];
    char data     [2000+2];
}tl_bc_exp_log_def;

int tlBcExpLogOpr(int oprType, tl_bc_exp_log_def *p_tlBcExplLog, int *p_sqlCode);
#endif
