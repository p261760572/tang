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


#ifndef _MSG_RCV_
#define _MSG_RCV_

typedef struct
{
      char	phone[16 + 1];
      char	msg_txt[512 + 1];
      char 	ext[10 + 1];
      char	send_time[14 + 1];
}msg_rcv_def;

int MsgRcvOpr(int oprType, msg_rcv_def *p_msgSendInf, int *p_sqlCode);
#endif
