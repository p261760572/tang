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


#ifndef _TI_BC_MCHNT_DET_
#define _TI_BC_MCHNT_DET_

typedef struct{
    char trans_chnl      [2  + 1 ];
    char msg_tp          [4  + 1 ];
    char proc_cd         [6  + 1 ];
    char pos_cond_cd     [4  + 1 ];
    char mchnt_tp        [4  + 1 ];
    char mchnt_cd        [15 + 1 ]; 
}ti_bc_mchnt_det_def;

int tiBcMchntDetOpr(int oprType, ti_bc_mchnt_det_def *p_tiBcMchntDet, int *p_sqlCode);
#endif
