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


#ifndef _TI_BC_CARD_BIN_
#define _TI_BC_CARD_BIN_

typedef struct{
  char card_bin           [21 + 1 ];
  char pos_entry_md_cd2   [1  + 1 ];
  char pos_entry_md_cd3   [1  + 1 ];
	char card_attr          [2  + 1 ];
	char iss_ins_id_cd      [8  + 1 ];
  char fst_rcv_ins_id_cd  [8  + 1 ];
  char snd_rcv_ins_id_cd  [8  + 1 ];
  char enable_flag        [1  + 1 ];
}ti_bc_card_bin_def;

int tiBcCardBinOpr(int oprType, ti_bc_card_bin_def *p_tiCardBin, int *p_sqlCode);

#endif
