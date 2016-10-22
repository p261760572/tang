/*
 *  Copyright 2008, China UnionPay Co., Ltd.  All right reserved.
 *
 *
 *  $Id: ti_bc_trans_chnl_inf.h,v 1.0 2008-11-21 14:56:04 weichai Exp $
 *
 *  Edit History:
 *    2008-11-21 14:57:17 - Created by weichai.
 *
 */


#ifndef _TI_BC_TRANS_CHNL_INF_
#define _TI_BC_TRANS_CHNL_INF_

typedef struct{
       char  trans_chnl      [30+ 1] ; /* Ω”»Î«˛µ¿*/
	     char  packety_type    [4 + 1] ;  
       char  config_nm       [30 + 1] ;  
}ti_bc_trans_chnl_inf_def;

int tiBcTransChnlInfOpr(int oprType, ti_bc_trans_chnl_inf_def *p_tiBcTransChnlInf, int *p_sqlCode);
#endif
