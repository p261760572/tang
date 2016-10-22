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


#ifndef _TI_BC_TRANS_DET_
#define _TI_BC_TRANS_DET_

typedef struct{
 char  trans_chnl      [ 30+ 1] ;
 char  msg_tp          [ 4 + 1] ;  
 char  proc_cd         [ 6 + 1] ;  
 char  pos_cond_cd     [ 2 + 1] ;  
 char  mchnt_tp        [ 4 + 1] ;  
 char  msg_tp_new      [ 4 + 1] ;
 char  trans_flag      [ 4 + 1] ;  
 char  rcv_ins_id_cd   [ 8 + 1] ;  
 char  procedure       [10 + 1] ;  
 char  trans_flag_s    [ 4 + 1] ;  
 char  trans_flag_e    [ 4 + 1] ;  
 char  trans_flag_o    [ 4 + 1] ;  
 /*char  rec_id          [ 8 + 1] ;*/
}ti_bc_trans_det_def;

int tiBcTransDetOpr(int oprType, ti_bc_trans_det_def *p_tiBcTrans, int *p_sqlCode);
#endif
