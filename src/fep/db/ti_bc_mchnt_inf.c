
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[19];
};
static struct sqlcxp sqlfpn =
{
    18,
    "ti_bc_mchnt_inf.pc"
};


static unsigned int sqlctx = 21230987;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
            short *cud;
   unsigned char  *sqlest;
            char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
              int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
            void  *sqhstv[32];
   unsigned int   sqhstl[32];
            int   sqhsts[32];
            void  *sqindv[32];
            int   sqinds[32];
   unsigned int   sqharm[32];
   unsigned int   *sqharc[32];
   unsigned short  sqadto[32];
   unsigned short  sqtdso[32];
} sqlstm = {12,32};

/* SQLLIB Prototypes */
extern sqlcxt ( void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * );
extern sqlcx2t( void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * );
extern sqlbuft( void **, char * );
extern sqlgs2t( void **, char * );
extern sqlorat( void **, unsigned int *, void * );

/* Forms Interface */
static int IAPSUCC = 0;
static int IAPFAIL = 1403;
static int IAPFTL  = 535;
extern void sqliem( unsigned char *, signed int * );

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,4274,850,0,0,
5,0,0,1,622,0,4,67,0,0,32,1,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,1,
97,0,0,
148,0,0,2,0,0,29,148,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_mchnt_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

ti_bc_mchnt_inf_def tiBcMchntInf ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tiBcMchntInfOpr
 *
 *
 *  Parameters:
 * 
 *      oprType - 操作类型， 输入， 取值
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode      - 返回值， 输出
 *      p_tiBcMchntInf - 查找条件，输入
 *                       查找结果，输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tiBcMchntInfOpr(int oprType, ti_bc_mchnt_inf_def *p_tiBcMchntInf, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char mchnt_cd        [15 + 1 ];  
    /*char mkey_index      [3  + 1 ];
    char mkey_1          [16 + 1 ];
    char mkey_2          [16 + 1 ];
    char mkey_3          [16 + 1 ];*/
    /* EXEC SQL END DECLARE SECTION; */ 


    MEMSET_0(mchnt_cd)
    /*
    MEMSET_0(mkey_index)
    MEMSET_0(mkey_1)
    MEMSET_0(mkey_2)
    MEMSET_0(mkey_3)*/
    
    char caTxnMsg[4096];
    switch (oprType)
    {
    
    case GLB_DB_SELECT1:

        memcpy(mchnt_cd, p_tiBcMchntInf->mchnt_cd,15);
        mchnt_cd[15]=0;
        rtrim(mchnt_cd);

        /* EXEC SQL SELECT 
        acq_ins_id_cd        ,
				mchnt_cd  	         ,
				mchnt_cn_abbr        ,
				mchnt_nm             ,
				settle_ins_id_cd     ,
				settle_acct          ,
				acct_nm              ,
				bank_nm              ,
				area_cd              ,
				send_msg_flag        ,
				linkMan              ,
				email                ,
				linktel              ,
				verifi_flag          ,
				verifi_id            ,
				pay_flag             ,
				pay_id               ,
				reversal_mode        ,
				using_blacklist      ,
				blacklist            ,
				using_whitelist      ,
				whitelist            ,
				debit_rcv_ins_id_cd  ,
				credit_rcv_ins_id_cd ,
				credit_enable_flag   ,
				auto_route_flag      ,
				credit_get_flag      ,
				debit_limit_times    ,
				credit_limit_times   ,
				limit_day            ,
				openflag                        
        INTO 
        	:tiBcMchntInf.acq_ins_id_cd        ,
					:tiBcMchntInf.mchnt_cd  	         , 
					:tiBcMchntInf.mchnt_cn_abbr        ,
					:tiBcMchntInf.mchnt_nm             ,
					:tiBcMchntInf.settle_ins_id_cd     ,
					:tiBcMchntInf.settle_acct          ,
					:tiBcMchntInf.acct_nm              ,
					:tiBcMchntInf.bank_nm              ,
					:tiBcMchntInf.area_cd              ,
					:tiBcMchntInf.send_msg_flag        ,
					:tiBcMchntInf.linkMan              ,
					:tiBcMchntInf.email                ,
					:tiBcMchntInf.linktel              ,
					:tiBcMchntInf.verifi_flag          ,
					:tiBcMchntInf.verifi_id            ,
					:tiBcMchntInf.pay_flag             ,
					:tiBcMchntInf.pay_id               ,
					:tiBcMchntInf.reversal_mode        ,
					:tiBcMchntInf.using_blacklist      ,
					:tiBcMchntInf.blacklist            ,
					:tiBcMchntInf.using_whitelist      ,
					:tiBcMchntInf.whitelist            ,
					:tiBcMchntInf.debit_rcv_ins_id_cd  ,
					:tiBcMchntInf.credit_rcv_ins_id_cd ,
					:tiBcMchntInf.credit_enable_flag   ,
					:tiBcMchntInf.auto_route_flag      ,
					:tiBcMchntInf.credit_get_flag      ,
					:tiBcMchntInf.debit_limit_times    ,
					:tiBcMchntInf.credit_limit_times   ,
					:tiBcMchntInf.limit_day            ,
					:tiBcMchntInf.openflag     
        FROM ti_bc_mchnt_inf
        WHERE rtrim(mchnt_cd)=:mchnt_cd
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 32;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select acq_ins_id_cd ,mchnt_cd ,mchnt_cn_abbr ,mchnt\
_nm ,settle_ins_id_cd ,settle_acct ,acct_nm ,bank_nm ,area_cd ,send_msg_flag \
,linkMan ,email ,linktel ,verifi_flag ,verifi_id ,pay_flag ,pay_id ,reversal_\
mode ,using_blacklist ,blacklist ,using_whitelist ,whitelist ,debit_rcv_ins_i\
d_cd ,credit_rcv_ins_id_cd ,credit_enable_flag ,auto_route_flag ,credit_get_f\
lag ,debit_limit_times ,credit_limit_times ,limit_day ,openflag into :b0,:b1,\
:b2,:b3,:b4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15,:b16,:b17,:b18,\
:b19,:b20,:b21,:b22,:b23,:b24,:b25,:b26,:b27,:b28,:b29,:b30  from ti_bc_mchnt\
_inf where rtrim(mchnt_cd)=:b31";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tiBcMchntInf.acq_ins_id_cd);
        sqlstm.sqhstl[0] = (unsigned int  )9;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tiBcMchntInf.mchnt_cd);
        sqlstm.sqhstl[1] = (unsigned int  )16;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tiBcMchntInf.mchnt_cn_abbr);
        sqlstm.sqhstl[2] = (unsigned int  )41;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tiBcMchntInf.mchnt_nm);
        sqlstm.sqhstl[3] = (unsigned int  )201;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tiBcMchntInf.settle_ins_id_cd);
        sqlstm.sqhstl[4] = (unsigned int  )14;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tiBcMchntInf.settle_acct);
        sqlstm.sqhstl[5] = (unsigned int  )33;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tiBcMchntInf.acct_nm);
        sqlstm.sqhstl[6] = (unsigned int  )81;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tiBcMchntInf.bank_nm);
        sqlstm.sqhstl[7] = (unsigned int  )41;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tiBcMchntInf.area_cd);
        sqlstm.sqhstl[8] = (unsigned int  )7;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tiBcMchntInf.send_msg_flag);
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tiBcMchntInf.linkMan);
        sqlstm.sqhstl[10] = (unsigned int  )21;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tiBcMchntInf.email);
        sqlstm.sqhstl[11] = (unsigned int  )61;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tiBcMchntInf.linktel);
        sqlstm.sqhstl[12] = (unsigned int  )21;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tiBcMchntInf.verifi_flag);
        sqlstm.sqhstl[13] = (unsigned int  )2;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tiBcMchntInf.verifi_id);
        sqlstm.sqhstl[14] = (unsigned int  )9;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tiBcMchntInf.pay_flag);
        sqlstm.sqhstl[15] = (unsigned int  )2;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tiBcMchntInf.pay_id);
        sqlstm.sqhstl[16] = (unsigned int  )9;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tiBcMchntInf.reversal_mode);
        sqlstm.sqhstl[17] = (unsigned int  )2;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tiBcMchntInf.using_blacklist);
        sqlstm.sqhstl[18] = (unsigned int  )2;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tiBcMchntInf.blacklist);
        sqlstm.sqhstl[19] = (unsigned int  )61;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tiBcMchntInf.using_whitelist);
        sqlstm.sqhstl[20] = (unsigned int  )2;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tiBcMchntInf.whitelist);
        sqlstm.sqhstl[21] = (unsigned int  )61;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tiBcMchntInf.debit_rcv_ins_id_cd);
        sqlstm.sqhstl[22] = (unsigned int  )9;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tiBcMchntInf.credit_rcv_ins_id_cd);
        sqlstm.sqhstl[23] = (unsigned int  )9;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tiBcMchntInf.credit_enable_flag);
        sqlstm.sqhstl[24] = (unsigned int  )2;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tiBcMchntInf.auto_route_flag);
        sqlstm.sqhstl[25] = (unsigned int  )2;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tiBcMchntInf.credit_get_flag);
        sqlstm.sqhstl[26] = (unsigned int  )2;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tiBcMchntInf.debit_limit_times);
        sqlstm.sqhstl[27] = (unsigned int  )13;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tiBcMchntInf.credit_limit_times);
        sqlstm.sqhstl[28] = (unsigned int  )13;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tiBcMchntInf.limit_day);
        sqlstm.sqhstl[29] = (unsigned int  )13;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tiBcMchntInf.openflag);
        sqlstm.sqhstl[30] = (unsigned int  )2;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[31] = (unsigned int  )16;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqphss = sqlstm.sqhsts;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqpins = sqlstm.sqinds;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlstm.sqpadto = sqlstm.sqadto;
        sqlstm.sqptdso = sqlstm.sqtdso;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



        /* search result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
            memcpy(p_tiBcMchntInf, &tiBcMchntInf, sizeof(ti_bc_mchnt_inf_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TI_BC_MCHNT_INF.pc:tiBcMchntInfOpr:SELECT1>Search Record (mchnt_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", mchnt_cd,sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<TI_BC_MCHNT_INF.pc:tiBcMchntInfOpr:SELECT1>Search Record (mchnt_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", mchnt_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 32;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )148;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;
/*
    case GLB_DB_UPDATE2:
    
        memcpy(mchnt_cd, p_tiBcMchntInf->mchnt_cd,15);
        mchnt_cd[15]=0;

        memcpy(mkey_1, p_tiBcMchntInf->mkey_1,16);
        ADJUST_STRING(mkey_1)

        memcpy(mkey_2, p_tiBcMchntInf->mkey_2,16);
        ADJUST_STRING(mkey_2)

        memcpy(mkey_3, p_tiBcMchntInf->mkey_3,16);
        ADJUST_STRING(mkey_3)

        glb_GetLocalTimeString(rec_upd_ts,15,"%Y%m%d%H%M%S");

        EXEC SQL UPDATE ti_bc_mchnt_inf
        SET mkey_1=:mkey_1, 
        mkey_2=:mkey_2, 
        mkey_3=:mkey_3, 
        rec_upd_ts=:rec_upd_ts
                   WHERE rtrim(mchnt_cd)=:mchnt_cd
                                        ;
  
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
            memcpy(p_tiBcMchntInf, &tiBcMchntInf, sizeof(ti_bc_mchnt_inf_def));
        } else if ( memcmp(sqlca.sqlerrm.sqlerrmc, "ORA-00001", 0 ) == 0 )
        {
            *p_sqlCode = SQL_ERD_UNIKEY;
            sprintf(caTxnMsg, "<TI_BC_MCHNT_INF.pc:tiBcMchntInfOpr:UPDATE2>Search Record (mchnt_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", mchnt_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        } else
        {
            sprintf(caTxnMsg, "<TI_BC_MCHNT_INF.pc:tiBcMchntInfOpr:UPDATE2>Search Record (mchnt_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", mchnt_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        }
        EXEC SQL COMMIT WORK ;
        break;
*/
    default:
    *p_sqlCode = -1;
        return -1;
    }

    return 0;
}

