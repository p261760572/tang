
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
           char  filnam[18];
};
static struct sqlcxp sqlfpn =
{
    17,
    "tl_bc_bill_log.pc"
};


static unsigned int sqlctx = 10706435;


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
            void  *sqhstv[19];
   unsigned int   sqhstl[19];
            int   sqhsts[19];
            void  *sqindv[19];
            int   sqinds[19];
   unsigned int   sqharm[19];
   unsigned int   *sqharc[19];
   unsigned short  sqadto[19];
   unsigned short  sqtdso[19];
} sqlstm = {12,19};

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
5,0,0,1,372,0,4,74,0,0,19,3,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
96,0,0,2,0,0,29,111,0,0,0,0,0,1,0,
111,0,0,3,351,0,4,125,0,0,18,2,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,
198,0,0,4,0,0,29,161,0,0,0,0,0,1,0,
213,0,0,5,112,0,4,178,0,0,3,2,0,1,0,2,3,0,0,1,97,0,0,1,97,0,0,
240,0,0,6,0,0,29,199,0,0,0,0,0,1,0,
255,0,0,7,282,0,3,230,0,0,16,16,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,
334,0,0,8,0,0,29,265,0,0,0,0,0,1,0,
349,0,0,9,177,0,5,293,0,0,9,9,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,
0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
400,0,0,10,0,0,29,318,0,0,0,0,0,1,0,
415,0,0,11,283,0,5,357,0,0,16,16,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1,97,0,0,1,97,0,0,1,97,0,0,
494,0,0,12,0,0,29,388,0,0,0,0,0,1,0,
509,0,0,13,107,0,4,431,0,0,3,2,0,1,0,2,97,0,0,1,97,0,0,1,97,0,0,
536,0,0,14,0,0,29,456,0,0,0,0,0,1,0,
551,0,0,15,120,0,5,489,0,0,5,5,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,
586,0,0,16,0,0,29,505,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/tl_bc_bill_log.h"
#include "./inc/tl_bc_trans_log.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

tl_bc_bill_log_def tlBcBillLog ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tlBcTransLogOpr
 *
 *
 *  Parameters:
 * 
 *      oprType - 操作类型， 输入， 取值
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode        - 返回值， 输出
 *      p_tlBcTransLog   - 查找条件，输入
 *                         查找结果，输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tlBcBillLogOpr(int oprType, tl_bc_bill_log_def *p_tlBcBillLog, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char loc_trans_dt  [ 4   + 1 ];
    char sys_tra_no    [ 19  + 1 ];
    char mchnt_cd      [ 15   + 1 ];
    char bill_id       [ 19  + 1 ];
    char fwd_ins_id_cd [  8  + 1 ];
    long bill_num;
    /* EXEC SQL END DECLARE SECTION; */ 


    int i;
    char buff[256];
    MEMSET_0(loc_trans_dt);
    MEMSET_0(sys_tra_no);
    MEMSET_0(bill_id);
    MEMSET_0(fwd_ins_id_cd);
    MEMSET_0(mchnt_cd);

    switch ( oprType )
    {
    
    case GLB_DB_SELECT1:

				 strcpy_safe(mchnt_cd, p_tlBcBillLog->mchnt_cd,sizeof(mchnt_cd)-1) ;
        rtrim_null(mchnt_cd, ' ') ;
        
        strcpy_safe(loc_trans_dt, p_tlBcBillLog->loc_trans_dt,sizeof(loc_trans_dt)-1) ;
        rtrim_null(loc_trans_dt, ' ') ;
        
        strcpy_safe(sys_tra_no, p_tlBcBillLog->sys_tra_no,sizeof(sys_tra_no)-1) ;
        rtrim_null(sys_tra_no, ' ') ;

        /* EXEC SQL SELECT 
        bc_settle_in       ,
        trans_at           ,
        mobile_no          ,
        sys_tra_no         ,
        loc_trans_tm       ,
        loc_trans_dt       ,
        settle_dt          ,
        resp_code          ,
        fwd_ins_id_cd      ,
        retri_ref_no      ,
        resp_cd            ,
        mchnt_cd           ,
        bill_id            ,
        rec_crt_ts         ,
        rec_upd_ts         ,
        bill_id_rcv
        INTO :tlBcBillLog
        FROM tl_bc_bill_log
        WHERE loc_trans_dt=:loc_trans_dt
			AND mchnt_cd=:mchnt_cd
			AND sys_tra_no=:sys_tra_no
                           ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select bc_settle_in ,trans_at ,mobile_no ,sys_tra_no\
 ,loc_trans_tm ,loc_trans_dt ,settle_dt ,resp_code ,fwd_ins_id_cd ,retri_ref_\
no ,resp_cd ,mchnt_cd ,bill_id ,rec_crt_ts ,rec_upd_ts ,bill_id_rcv into :s1 \
,:s2 ,:s3 ,:s4 ,:s5 ,:s6 ,:s7 ,:s8 ,:s9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,\
:s16   from tl_bc_bill_log where ((loc_trans_dt=:b1 and mchnt_cd=:b2) and sys\
_tra_no=:b3)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)tlBcBillLog.bc_settle_in;
        sqlstm.sqhstl[0] = (unsigned int  )2;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tlBcBillLog.trans_at;
        sqlstm.sqhstl[1] = (unsigned int  )13;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)tlBcBillLog.mobile_no;
        sqlstm.sqhstl[2] = (unsigned int  )31;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)tlBcBillLog.sys_tra_no;
        sqlstm.sqhstl[3] = (unsigned int  )7;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)tlBcBillLog.loc_trans_tm;
        sqlstm.sqhstl[4] = (unsigned int  )7;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)tlBcBillLog.loc_trans_dt;
        sqlstm.sqhstl[5] = (unsigned int  )5;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)tlBcBillLog.settle_dt;
        sqlstm.sqhstl[6] = (unsigned int  )9;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)tlBcBillLog.resp_code;
        sqlstm.sqhstl[7] = (unsigned int  )7;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)tlBcBillLog.fwd_ins_id_cd;
        sqlstm.sqhstl[8] = (unsigned int  )14;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)tlBcBillLog.retri_ref_no;
        sqlstm.sqhstl[9] = (unsigned int  )13;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)tlBcBillLog.resp_cd;
        sqlstm.sqhstl[10] = (unsigned int  )3;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)tlBcBillLog.mchnt_cd;
        sqlstm.sqhstl[11] = (unsigned int  )16;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)tlBcBillLog.bill_id;
        sqlstm.sqhstl[12] = (unsigned int  )77;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)tlBcBillLog.rec_crt_ts;
        sqlstm.sqhstl[13] = (unsigned int  )15;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)tlBcBillLog.rec_upd_ts;
        sqlstm.sqhstl[14] = (unsigned int  )15;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)tlBcBillLog.bill_id_rcv;
        sqlstm.sqhstl[15] = (unsigned int  )22;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[16] = (unsigned int  )5;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[17] = (unsigned int  )16;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[18] = (unsigned int  )20;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
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
            memcpy(p_tlBcBillLog, &tlBcBillLog, sizeof(tl_bc_bill_log_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0,"<TL_BC_TRANS_LOG.pc:tlBcBillLogOpr:SELECT1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr:SELECT1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )96;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    case GLB_DB_SELECT2:

        memcpy(fwd_ins_id_cd, p_tlBcBillLog->fwd_ins_id_cd,8);
        fwd_ins_id_cd[8]=0;
        ADJUST_STRING(fwd_ins_id_cd);

        i = glb_min_strlen(p_tlBcBillLog->bill_id ,sizeof (bill_id)-1);
        memcpy(bill_id, p_tlBcBillLog->bill_id, i );
        bill_id[i]=0;
        ADJUST_STRING(bill_id);

        /* EXEC SQL SELECT 
        bc_settle_in       ,
        trans_at           ,
        mobile_no          ,
        sys_tra_no         ,
        loc_trans_tm       ,
        loc_trans_dt       ,
        settle_dt          ,
        resp_code          ,
        fwd_ins_id_cd      ,
        retri_ref_no     ,
        resp_cd            ,
        mchnt_cd           ,
        bill_id            ,
        rec_crt_ts         ,
        rec_upd_ts         ,
        bill_id_rcv
        INTO :tlBcBillLog
        FROM tl_bc_bill_log
        WHERE fwd_ins_id_cd=:fwd_ins_id_cd
                            AND bill_id=:bill_id                            	
                            ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select bc_settle_in ,trans_at ,mobile_no ,sys_tra_no\
 ,loc_trans_tm ,loc_trans_dt ,settle_dt ,resp_code ,fwd_ins_id_cd ,retri_ref_\
no ,resp_cd ,mchnt_cd ,bill_id ,rec_crt_ts ,rec_upd_ts ,bill_id_rcv into :s1 \
,:s2 ,:s3 ,:s4 ,:s5 ,:s6 ,:s7 ,:s8 ,:s9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,\
:s16   from tl_bc_bill_log where (fwd_ins_id_cd=:b1 and bill_id=:b2)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )111;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)tlBcBillLog.bc_settle_in;
        sqlstm.sqhstl[0] = (unsigned int  )2;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tlBcBillLog.trans_at;
        sqlstm.sqhstl[1] = (unsigned int  )13;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)tlBcBillLog.mobile_no;
        sqlstm.sqhstl[2] = (unsigned int  )31;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)tlBcBillLog.sys_tra_no;
        sqlstm.sqhstl[3] = (unsigned int  )7;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)tlBcBillLog.loc_trans_tm;
        sqlstm.sqhstl[4] = (unsigned int  )7;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)tlBcBillLog.loc_trans_dt;
        sqlstm.sqhstl[5] = (unsigned int  )5;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)tlBcBillLog.settle_dt;
        sqlstm.sqhstl[6] = (unsigned int  )9;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)tlBcBillLog.resp_code;
        sqlstm.sqhstl[7] = (unsigned int  )7;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)tlBcBillLog.fwd_ins_id_cd;
        sqlstm.sqhstl[8] = (unsigned int  )14;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)tlBcBillLog.retri_ref_no;
        sqlstm.sqhstl[9] = (unsigned int  )13;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)tlBcBillLog.resp_cd;
        sqlstm.sqhstl[10] = (unsigned int  )3;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)tlBcBillLog.mchnt_cd;
        sqlstm.sqhstl[11] = (unsigned int  )16;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)tlBcBillLog.bill_id;
        sqlstm.sqhstl[12] = (unsigned int  )77;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)tlBcBillLog.rec_crt_ts;
        sqlstm.sqhstl[13] = (unsigned int  )15;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)tlBcBillLog.rec_upd_ts;
        sqlstm.sqhstl[14] = (unsigned int  )15;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)tlBcBillLog.bill_id_rcv;
        sqlstm.sqhstl[15] = (unsigned int  )22;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)fwd_ins_id_cd;
        sqlstm.sqhstl[16] = (unsigned int  )9;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)bill_id;
        sqlstm.sqhstl[17] = (unsigned int  )20;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
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
            memcpy(p_tlBcBillLog, &tlBcBillLog, sizeof(tl_bc_bill_log_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0,"<TL_BC_TRANS_LOG.pc:tlBcBillLogOpr:SELECT1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr:SELECT1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )198;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;


    case GLB_DB_SELECT3:

        bill_num = 0;

        memcpy(fwd_ins_id_cd, p_tlBcBillLog->fwd_ins_id_cd,8);
        fwd_ins_id_cd[8]=0;
        ADJUST_STRING(fwd_ins_id_cd);

        i = glb_min_strlen(p_tlBcBillLog->bill_id ,sizeof (bill_id)-1);
        memcpy(bill_id, p_tlBcBillLog->bill_id, i );
        bill_id[i]=0;
        ADJUST_STRING(bill_id);

        /* EXEC SQL SELECT 
        count(*) INTO :bill_num
        FROM tl_bc_bill_log
        WHERE fwd_ins_id_cd=:fwd_ins_id_cd
                            AND bill_id=:bill_id
                            AND bc_settle_in<>'0'
                            ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select count(*)  into :b0  from tl_bc_bill_log where\
 ((fwd_ins_id_cd=:b1 and bill_id=:b2) and bc_settle_in<>'0')";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )213;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)&bill_num;
        sqlstm.sqhstl[0] = (unsigned int  )sizeof(long);
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)fwd_ins_id_cd;
        sqlstm.sqhstl[1] = (unsigned int  )9;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)bill_id;
        sqlstm.sqhstl[2] = (unsigned int  )20;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
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

        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0,"<TL_BC_TRANS_LOG.pc:tlBcBillLogOpr:SELECT1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr:SELECT1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )240;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    case GLB_DB_INSERT1:    
        glb_Log(0,0,"enter GLB_DB_INSERT1");

        /*copy buff*/
        memcpy(&tlBcBillLog, p_tlBcBillLog, sizeof(tl_bc_bill_log_def));

        /* add other field*/
        glb_GetLocalTimeString(tlBcBillLog.rec_crt_ts,15,"%Y%m%d%H%M%S");
        glb_GetLocalTimeString(tlBcBillLog.rec_upd_ts,15,"%Y%m%d%H%M%S");

        ADJUST_STRING(tlBcBillLog.bc_settle_in      )
        glb_Adjust_Decimal(tlBcBillLog.trans_at, tlBcBillLog.trans_at, 12);
        ADJUST_STRING(tlBcBillLog.mobile_no         )
        ADJUST_STRING(tlBcBillLog.sys_tra_no        )
        ADJUST_STRING(tlBcBillLog.loc_trans_tm      )
        ADJUST_STRING(tlBcBillLog.loc_trans_dt      )
        ADJUST_STRING(tlBcBillLog.settle_dt         )  
        ADJUST_STRING(tlBcBillLog.resp_code         )
        ADJUST_STRING(tlBcBillLog.fwd_ins_id_cd     )
        ADJUST_STRING(tlBcBillLog.retri_ref_no     )
        ADJUST_STRING(tlBcBillLog.resp_cd           )
        ADJUST_STRING(tlBcBillLog.mchnt_cd          )        
        ADJUST_STRING(tlBcBillLog.bill_id           )
        ADJUST_STRING(tlBcBillLog.bill_id_rcv       )

        /* execute insert sql */
        /*EXEC SQL EXECUTE S using*/

        /* EXEC SQL INSERT INTO tl_bc_bill_log(bc_settle_in,trans_at,mobile_no,sys_tra_no,loc_trans_tm,loc_trans_dt,settle_dt,resp_code,fwd_ins_id_cd,retri_ref_no,resp_cd,mchnt_cd,bill_id,rec_crt_ts,rec_upd_ts,bill_id_rcv)values
        (
        :tlBcBillLog.bc_settle_in       ,
        :tlBcBillLog.trans_at           ,
        :tlBcBillLog.mobile_no          ,
        :tlBcBillLog.sys_tra_no         ,
        :tlBcBillLog.loc_trans_tm       ,
        :tlBcBillLog.loc_trans_dt       ,
        :tlBcBillLog.settle_dt          ,
        :tlBcBillLog.resp_code          ,
        :tlBcBillLog.fwd_ins_id_cd      ,
        :tlBcBillLog.retri_ref_no     ,
        :tlBcBillLog.resp_cd            ,
        :tlBcBillLog.mchnt_cd           ,
        :tlBcBillLog.bill_id            ,
        :tlBcBillLog.rec_crt_ts         ,
        :tlBcBillLog.rec_upd_ts         ,
        :tlBcBillLog.bill_id_rcv
        )
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "insert into tl_bc_bill_log (bc_settle_in,trans_at,mo\
bile_no,sys_tra_no,loc_trans_tm,loc_trans_dt,settle_dt,resp_code,fwd_ins_id_c\
d,retri_ref_no,resp_cd,mchnt_cd,bill_id,rec_crt_ts,rec_upd_ts,bill_id_rcv) va\
lues (:b0,:b1,:b2,:b3,:b4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )255;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcBillLog.bc_settle_in);
        sqlstm.sqhstl[0] = (unsigned int  )2;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcBillLog.trans_at);
        sqlstm.sqhstl[1] = (unsigned int  )13;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcBillLog.mobile_no);
        sqlstm.sqhstl[2] = (unsigned int  )31;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcBillLog.sys_tra_no);
        sqlstm.sqhstl[3] = (unsigned int  )7;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcBillLog.loc_trans_tm);
        sqlstm.sqhstl[4] = (unsigned int  )7;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcBillLog.loc_trans_dt);
        sqlstm.sqhstl[5] = (unsigned int  )5;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcBillLog.settle_dt);
        sqlstm.sqhstl[6] = (unsigned int  )9;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcBillLog.resp_code);
        sqlstm.sqhstl[7] = (unsigned int  )7;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcBillLog.fwd_ins_id_cd);
        sqlstm.sqhstl[8] = (unsigned int  )14;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcBillLog.retri_ref_no);
        sqlstm.sqhstl[9] = (unsigned int  )13;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcBillLog.resp_cd);
        sqlstm.sqhstl[10] = (unsigned int  )3;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcBillLog.mchnt_cd);
        sqlstm.sqhstl[11] = (unsigned int  )16;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcBillLog.bill_id);
        sqlstm.sqhstl[12] = (unsigned int  )77;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcBillLog.rec_crt_ts);
        sqlstm.sqhstl[13] = (unsigned int  )15;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcBillLog.rec_upd_ts);
        sqlstm.sqhstl[14] = (unsigned int  )15;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcBillLog.bill_id_rcv);
        sqlstm.sqhstl[15] = (unsigned int  )22;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
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



        /* insert result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {


        } else if ( memcmp(sqlca.sqlerrm.sqlerrmc, "ORA-00001", 9 ) == 0 )
        {
            *p_sqlCode = SQL_ERD_UNIKEY;
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr:INSERT1>Insert Record is exist.(loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", tlBcBillLog.loc_trans_dt,tlBcBillLog.mchnt_cd,tlBcBillLog.sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr:INSERT1>Insert Record Failure. (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", tlBcBillLog.loc_trans_dt,tlBcBillLog.mchnt_cd,tlBcBillLog.sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )334;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

        /* 仅更新相关标志位字段 */
    case GLB_DB_UPDATE1:

        /*copy buff*/
        memcpy(&tlBcBillLog, p_tlBcBillLog, sizeof(tl_bc_bill_log_def));

        strcpy_safe(mchnt_cd, p_tlBcBillLog->mchnt_cd,sizeof(mchnt_cd)-1) ;
        rtrim_null(mchnt_cd, ' ') ;
        
        strcpy_safe(loc_trans_dt, p_tlBcBillLog->loc_trans_dt,sizeof(loc_trans_dt)-1) ;
        rtrim_null(loc_trans_dt, ' ') ;
        
        strcpy_safe(sys_tra_no, p_tlBcBillLog->sys_tra_no,sizeof(sys_tra_no)-1) ;
        rtrim_null(sys_tra_no, ' ') ;

        glb_GetLocalTimeString(tlBcBillLog.rec_upd_ts,15,"%Y%m%d%H%M%S");

        ADJUST_STRING(tlBcBillLog.settle_dt         )
        ADJUST_STRING(tlBcBillLog.bc_settle_in      )
        ADJUST_STRING(tlBcBillLog.resp_code         )
        ADJUST_STRING(tlBcBillLog.mchnt_cd          )
        ADJUST_STRING(tlBcBillLog.bill_id           )        
        ADJUST_STRING(tlBcBillLog.rec_upd_ts        )
        ADJUST_STRING(tlBcBillLog.bill_id_rcv       )

        /* EXEC SQL UPDATE tl_bc_bill_log SET
        settle_dt         =:tlBcBillLog.settle_dt,
        bc_settle_in      =:tlBcBillLog.bc_settle_in,
        resp_code         =:tlBcBillLog.resp_code,
        bill_id           =:tlBcBillLog.bill_id,
        rec_upd_ts        =:tlBcBillLog.rec_upd_ts,
        bill_id_rcv       =:tlBcBillLog.bill_id_rcv
                           WHERE loc_trans_dt=:loc_trans_dt
                           AND mchnt_cd=:mchnt_cd
                           AND sys_tra_no=:sys_tra_no
                           ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "update tl_bc_bill_log  set settle_dt=:b0,bc_settle_i\
n=:b1,resp_code=:b2,bill_id=:b3,rec_upd_ts=:b4,bill_id_rcv=:b5 where ((loc_tr\
ans_dt=:b6 and mchnt_cd=:b7) and sys_tra_no=:b8)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )349;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcBillLog.settle_dt);
        sqlstm.sqhstl[0] = (unsigned int  )9;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcBillLog.bc_settle_in);
        sqlstm.sqhstl[1] = (unsigned int  )2;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcBillLog.resp_code);
        sqlstm.sqhstl[2] = (unsigned int  )7;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcBillLog.bill_id);
        sqlstm.sqhstl[3] = (unsigned int  )77;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcBillLog.rec_upd_ts);
        sqlstm.sqhstl[4] = (unsigned int  )15;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcBillLog.bill_id_rcv);
        sqlstm.sqhstl[5] = (unsigned int  )22;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[6] = (unsigned int  )5;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[7] = (unsigned int  )16;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[8] = (unsigned int  )20;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
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



        /* update result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
            /*memcpy(p_tlBcBillLog, &tlBcBillLog, sizeof(tl_bc_bill_log_def));*/
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr:UPDATE1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr:UPDATE1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )400;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

        /* 更新key以外的所有字段 */
    case GLB_DB_UPDATE2:

        /*copy buff*/
        memcpy(&tlBcBillLog, p_tlBcBillLog, sizeof(tl_bc_bill_log_def));

        memcpy(fwd_ins_id_cd, p_tlBcBillLog->fwd_ins_id_cd,8);
        fwd_ins_id_cd[8]=0;
        ADJUST_STRING(fwd_ins_id_cd);

        i = glb_min_strlen(p_tlBcBillLog->bill_id ,sizeof (bill_id));
        memcpy(bill_id, p_tlBcBillLog->bill_id, i );
        bill_id[i]=0;
        ADJUST_STRING(bill_id);

        /* add other field*/
        glb_GetLocalTimeString(tlBcBillLog.rec_crt_ts,15,"%Y%m%d%H%M%S");
        glb_GetLocalTimeString(tlBcBillLog.rec_upd_ts,15,"%Y%m%d%H%M%S");

        ADJUST_STRING(tlBcBillLog.bc_settle_in      )
        glb_Adjust_Decimal(tlBcBillLog.trans_at, tlBcBillLog.trans_at, 12);
        ADJUST_STRING(tlBcBillLog.mobile_no         )
        ADJUST_STRING(tlBcBillLog.sys_tra_no        )
        ADJUST_STRING(tlBcBillLog.loc_trans_tm      )
        ADJUST_STRING(tlBcBillLog.loc_trans_dt      )
        ADJUST_STRING(tlBcBillLog.settle_dt         )  
        ADJUST_STRING(tlBcBillLog.resp_code         )
        ADJUST_STRING(tlBcBillLog.fwd_ins_id_cd     )
        ADJUST_STRING(tlBcBillLog.retri_ref_no     )
        ADJUST_STRING(tlBcBillLog.resp_cd           )
        ADJUST_STRING(tlBcBillLog.mchnt_cd          )
        ADJUST_STRING(tlBcBillLog.bill_id           )
        ADJUST_STRING(tlBcBillLog.bill_id_rcv       )

        /* execute insert sql */
        /*EXEC SQL EXECUTE S using*/
        /* EXEC SQL UPDATE tl_bc_bill_log set
        bc_settle_in               =:tlBcBillLog.bc_settle_in       ,
        trans_at                   =:tlBcBillLog.trans_at           ,
        mobile_no                  =:tlBcBillLog.mobile_no          ,
        sys_tra_no                 =:tlBcBillLog.sys_tra_no         ,
        loc_trans_tm               =:tlBcBillLog.loc_trans_tm       ,
        loc_trans_dt               =:tlBcBillLog.loc_trans_dt       ,
        settle_dt                  =:tlBcBillLog.settle_dt          ,
        resp_code                  =:tlBcBillLog.resp_code          ,
        retri_ref_no               =:tlBcBillLog.retri_ref_no          ,	
        resp_cd                    =:tlBcBillLog.resp_cd            ,
        mchnt_cd                   =:tlBcBillLog.mchnt_cd           ,
        rec_crt_ts                 =:tlBcBillLog.rec_crt_ts         ,
        rec_upd_ts                 =:tlBcBillLog.rec_upd_ts         ,
        bill_id_rcv                =:tlBcBillLog.bill_id_rcv        
                                    WHERE fwd_ins_id_cd=:fwd_ins_id_cd AND bill_id=:bill_id
                                    ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "update tl_bc_bill_log  set bc_settle_in=:b0,trans_at\
=:b1,mobile_no=:b2,sys_tra_no=:b3,loc_trans_tm=:b4,loc_trans_dt=:b5,settle_dt\
=:b6,resp_code=:b7,retri_ref_no=:b8,resp_cd=:b9,mchnt_cd=:b10,rec_crt_ts=:b11\
,rec_upd_ts=:b12,bill_id_rcv=:b13 where (fwd_ins_id_cd=:b14 and bill_id=:b15)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )415;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcBillLog.bc_settle_in);
        sqlstm.sqhstl[0] = (unsigned int  )2;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcBillLog.trans_at);
        sqlstm.sqhstl[1] = (unsigned int  )13;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcBillLog.mobile_no);
        sqlstm.sqhstl[2] = (unsigned int  )31;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcBillLog.sys_tra_no);
        sqlstm.sqhstl[3] = (unsigned int  )7;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcBillLog.loc_trans_tm);
        sqlstm.sqhstl[4] = (unsigned int  )7;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcBillLog.loc_trans_dt);
        sqlstm.sqhstl[5] = (unsigned int  )5;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcBillLog.settle_dt);
        sqlstm.sqhstl[6] = (unsigned int  )9;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcBillLog.resp_code);
        sqlstm.sqhstl[7] = (unsigned int  )7;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcBillLog.retri_ref_no);
        sqlstm.sqhstl[8] = (unsigned int  )13;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcBillLog.resp_cd);
        sqlstm.sqhstl[9] = (unsigned int  )3;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcBillLog.mchnt_cd);
        sqlstm.sqhstl[10] = (unsigned int  )16;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcBillLog.rec_crt_ts);
        sqlstm.sqhstl[11] = (unsigned int  )15;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcBillLog.rec_upd_ts);
        sqlstm.sqhstl[12] = (unsigned int  )15;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcBillLog.bill_id_rcv);
        sqlstm.sqhstl[13] = (unsigned int  )22;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)fwd_ins_id_cd;
        sqlstm.sqhstl[14] = (unsigned int  )9;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)bill_id;
        sqlstm.sqhstl[15] = (unsigned int  )20;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
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



        /* update result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
            /*memcpy(p_tlBcBillLog, &tlBcBillLog, sizeof(tl_bc_bill_log_def));*/
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr:UPDATE2>Search Record (fwd_ins_id_cd:%s,bill_id:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", fwd_ins_id_cd,bill_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr:UPDATE2>Search Record (fwd_ins_id_cd:%s,bill_id:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",  fwd_ins_id_cd,bill_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )494;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    default:
        *p_sqlCode = -1;
        return -1;
    }

    return 0;
}

int tlBcBillLogOpr2(int oprType, tl_bc_bill_log_def *p_tlBcBillLog, int *p_sqlCode, int *ReturnNum)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char bill_id       [ 76  + 1 ];
    char fwd_ins_id_cd [  8  + 1 ];
    long bill_num;
    /* EXEC SQL END DECLARE SECTION; */ 


    int i;
    char buff[256];
    MEMSET_0(bill_id);
    MEMSET_0(fwd_ins_id_cd);

    char caTxnMsg[1024];
    switch ( oprType )
    {
    case GLB_DB_SELECT1:

        bill_num = 0;

        memcpy(fwd_ins_id_cd, p_tlBcBillLog->fwd_ins_id_cd,8);
        fwd_ins_id_cd[8]=0;
        ADJUST_STRING(fwd_ins_id_cd);

        i = glb_min_strlen(p_tlBcBillLog->bill_id ,sizeof (bill_id));
        memcpy(bill_id, p_tlBcBillLog->bill_id, i );
        bill_id[i]=0;
        ADJUST_STRING(bill_id);

        /* EXEC SQL SELECT bc_settle_in INTO :tlBcBillLog.bc_settle_in
        FROM tl_bc_bill_log
        WHERE fwd_ins_id_cd=:fwd_ins_id_cd
                            AND bill_id=:bill_id
                            AND ROWNUM<=1
                            ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select bc_settle_in into :b0  from tl_bc_bill_log wh\
ere ((fwd_ins_id_cd=:b1 and bill_id=:b2) and ROWNUM<=1)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )509;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcBillLog.bc_settle_in);
        sqlstm.sqhstl[0] = (unsigned int  )2;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)fwd_ins_id_cd;
        sqlstm.sqhstl[1] = (unsigned int  )9;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)bill_id;
        sqlstm.sqhstl[2] = (unsigned int  )77;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
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

        sprintf(caTxnMsg, "<TL_BC_TRANS_LOG.pc:tlBcBillLogOpr2:SELECT1>bill_num%d,%s\n", *ReturnNum, *p_sqlCode);
        glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );

        if ( sqlca.sqlcode == 0 )
        {
            strcpy(p_tlBcBillLog->bc_settle_in, tlBcBillLog.bc_settle_in);
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0,"<TL_BC_TRANS_LOG.pc:tlBcBillLogOpr2:SELECT1>Search Record (fwd_ins_id_cd:%s,bill_id:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", fwd_ins_id_cd,bill_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            *ReturnNum = 0;
        } else
        {
            glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogOpr2:SELECT1>Search Record (fwd_ins_id_cd:%s,bill_id:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", fwd_ins_id_cd,bill_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 19;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )536;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    default:
        *p_sqlCode = -1;
        return -1;
    }

    return 0;
}

int tlBcBillLogUpdate(int oprType, tl_bc_trans_log_def *p_tlBcTransLog, int *p_sqlCode)
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 

	    char loc_trans_dt  [ 4   + 1 ];
	    char sys_tra_no    [ 6  + 1 ];
	    char mchnt_cd      [ 15   + 1 ];
	    char resp_code[6 + 1];
	    char bc_settle_in[1 + 1];
    /* EXEC SQL END DECLARE SECTION; */ 

	switch(oprType)
	{
		case GLB_DB_UPDATE1:
			memset(loc_trans_dt, 0, sizeof(loc_trans_dt));
			memset(sys_tra_no, 0, sizeof(sys_tra_no));
			memset(mchnt_cd, 0, sizeof(mchnt_cd));
			memset(resp_code, 0, sizeof(resp_code));
			memset(bc_settle_in, 0, sizeof(bc_settle_in));
			memcpy(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, 4);
			memcpy(sys_tra_no, p_tlBcTransLog->sys_tra_no, 6);
			memcpy(mchnt_cd, p_tlBcTransLog->mchnt_cd, 15);
			memcpy(resp_code, p_tlBcTransLog->fld_28, 6);
			memcpy(bc_settle_in, p_tlBcTransLog->bc_settle_in,1);
			/* EXEC SQL UPDATE tl_bc_bill_log SET
				bc_settle_in = :bc_settle_in,
				resp_code = :resp_code
				WHERE loc_trans_dt = :loc_trans_dt
					AND mchnt_cd = :mchnt_cd
					AND sys_tra_no = :sys_tra_no; */ 

{
   struct sqlexd sqlstm;
   sqlorat((void **)0, &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 19;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "update tl_bc_bill_log  set bc_settle_in=:b0,resp_code=:b1\
 where ((loc_trans_dt=:b2 and mchnt_cd=:b3) and sys_tra_no=:b4)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )551;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)bc_settle_in;
   sqlstm.sqhstl[0] = (unsigned int  )2;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)resp_code;
   sqlstm.sqhstl[1] = (unsigned int  )7;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)loc_trans_dt;
   sqlstm.sqhstl[2] = (unsigned int  )5;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         void  *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned int  )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (         void  *)mchnt_cd;
   sqlstm.sqhstl[3] = (unsigned int  )16;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         void  *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned int  )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (         void  *)sys_tra_no;
   sqlstm.sqhstl[4] = (unsigned int  )7;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         void  *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned int  )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
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


			*p_sqlCode = sqlca.sqlcode;
			if(sqlca.sqlcode == 1403)
			{
				*p_sqlCode = SQL_ERD_NONE_RECORD;
				glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogUpdate:UPDATE1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
			}
			else if(sqlca.sqlcode != 0)
			{
				glb_Log(0,0,"<TL_BC_BILL_LOG.pc:tlBcBillLogUpdate:UPDATE1>Search Record (loc_trans_dt:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
			}
			/* EXEC SQL COMMIT WORK ; */ 

{
   struct sqlexd sqlstm;
   sqlorat((void **)0, &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 19;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )586;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


			break;
			
	}
	return 0;
}

