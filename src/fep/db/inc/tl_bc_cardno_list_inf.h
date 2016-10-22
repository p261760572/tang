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


#ifndef _TL_BC_CARDNO_LIST_INF_
#define _TL_BC_CARDNO_LIST_INF_

typedef struct
{
    char cardno [19+1];
    char relation1 [100+1];
    char relation_crtday1 [8+1];
    char relation_lastday1 [8+1];
    char relation2 [100+1];
    char relation_crtday2 [8+1];
    char relation_lastday2 [8+1];
    char relation3 [100+1];
    char relation_crtday3 [8+1];
    char relation_lastday3 [8+1];
    char relation4 [100+1];
    char relation_crtday4 [8+1];
    char relation_lastday4 [8+1];
    char relation5 [100+1];
    char relation_crtday5 [8+1];
    char relation_lastday5 [8+1];
    //²»´æÊý¾Ý¿â
    char index;
}tl_bc_cardno_list_inf_def;

int tlBcCardnoListInfOpr(int oprType, tl_bc_cardno_list_inf_def *p_tlBcCardnoListInf, int *p_sqlCode);
#endif
