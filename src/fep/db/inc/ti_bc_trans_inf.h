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


#ifndef _TI_BC_TRANS_INF_
#define _TI_BC_TRANS_INF_

typedef struct{
       char  trans_chnl             [30 + 1] ;
	     char  msg_tp                 [4  + 1] ;
       char  proc_cd                [6  + 1] ;
       char  pos_cond_cd            [2  + 1] ;
       char  mchnt_tp               [4  + 1] ;
       char  respFlag               [1  + 1] ;
       char  DbFlag                 [1  + 1] ;
       char  CardBinDetFlag         [1  + 1] ; /* 1：需要根据卡bin判断路由机构  */
			 /*char  CardBinLimitFlag       [1  + 1] ; /* 1: 需要根据卡bin检查，是否可进行交易 */
			 /*char  MchntCardBinCheckFlag  [1  + 1] ; */
       char  Mid_chk_flg      [1  + 1] ;  /* 1: 需要检查卡bin的商户路由及卡bin合法性 */
       char  white_black_flag [1  + 1] ;  /* 1: 需要根据卡bin检查，是否可进行交易 */
       char  opFlag           [1  + 1] ;
       char  TransType        [5  + 1] ;
}ti_bc_trans_inf_def;

int tiBcTransInfOpr(int oprType, ti_bc_trans_inf_def *p_tiBcTransInf, int *p_sqlCode);
#endif
