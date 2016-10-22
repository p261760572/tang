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


#ifndef _TI_BC_INS_INF_
#define _TI_BC_INS_INF_

typedef struct{
    char ins_id_cd   [8   + 1 ];
    char ins_type    [1   + 1 ];
    char ins_cn_abbr [40  + 1 ];
    char ins_nm      [100 + 1 ];
    char openflag    [1   + 1 ];
    char key_type    [10  + 1 ];
    char key_index   [3   + 1 ];
    char key_1       [32  + 1 ];
    char key_2       [32  + 1 ];
    char key_3       [32  + 1 ];
    char line_nm     [30  + 1 ];
    char packet_type [ 4  + 1 ];
    char ip_addr     [15  + 1 ];
	  char port        [5   + 1 ];
	  char link_tp     [1   + 1 ];
}ti_bc_ins_inf_def;

int tiBcInsInfOpr(int oprType, ti_bc_ins_inf_def *p_tiBcInsInf, int *p_sqlCode);
#endif
