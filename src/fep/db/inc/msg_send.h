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


#ifndef _MSG_SEND_
#define _MSG_SEND_

typedef struct
{
	char	sys_tra_no[6 + 1];
	char 	mchnt_cd[20 + 1];
	char 	loc_trans_dt[8 + 1];	
	char	msg_id[38  + 1];
	char	phone[16 + 1];
	char	msg_txt[512 + 1];
	char	send_time[14 + 1];
	char	rcv_time[14 + 1];
	char	send_flag[1 + 1];
}msg_send_def;

int MsgSendOpr(int oprType, msg_send_def *p_msgSendInf, int *p_sqlCode);
#endif
