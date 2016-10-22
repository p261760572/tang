
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
           char  filnam[31];
};
static struct sqlcxp sqlfpn =
{
    30,
    "ti_bc_mchnt_term_charge_inf.pc"
};


static unsigned int sqlctx = 1064946443;


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
            void  *sqhstv[11];
   unsigned int   sqhstl[11];
            int   sqhsts[11];
            void  *sqindv[11];
            int   sqinds[11];
   unsigned int   sqharm[11];
   unsigned int   *sqharc[11];
   unsigned short  sqadto[11];
   unsigned short  sqtdso[11];
} sqlstm = {12,11};

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
5,0,0,1,503,0,4,75,0,0,11,4,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
64,0,0,2,0,0,29,116,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_mchnt_term_charge_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

char c_sql[4096];
ti_bc_mchnt_term_charge_inf_def tiBcMchntTermChargeInf ;
/* EXEC SQL END DECLARE SECTION; */ 


#define MIN(a,b) a>b?b:a

/*
 *  Function:  tiBcMchntTermChargeInfOpr
 *
 *
 *  Parameters:
 * 
 *      oprType - 操作类型， 输入， 取值
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode     - 返回值， 输出
 *      p_tiBcTrans   - 查找条件，输入
 *                      查找结果，输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tiBcMchntTermChargeInfOpr(int oprType, ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char mchnt_tp     [  4 + 1 ];
    char term_id      [  8 + 1 ];
    char mchnt_cd     [ 15 + 1 ];
    char rcv_ins_id_cd   [ 15 + 1 ];
    /* EXEC SQL END DECLARE SECTION; */ 


    MEMSET_0(mchnt_tp)
    MEMSET_0(term_id)
    MEMSET_0(mchnt_cd)
    MEMSET_0(rcv_ins_id_cd)

    int i;
    char caTxnMsg[4096];
    switch ( oprType )
    {
    
    case GLB_DB_SELECT1:
    case GLB_DB_SELECT2:
        memcpy_safe(mchnt_tp, p_tiBcMchntTermChargeInf->mchnt_tp, sizeof(mchnt_tp)-1);
        rtrim_null(mchnt_tp,' ');
        
        memcpy_safe(term_id, p_tiBcMchntTermChargeInf->term_id, sizeof(term_id)-1);
        rtrim_null(term_id,' ');
        
        memcpy_safe(mchnt_cd, p_tiBcMchntTermChargeInf->mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');
        
        memcpy_safe(rcv_ins_id_cd, p_tiBcMchntTermChargeInf->rcv_ins_id_cd, sizeof(rcv_ins_id_cd)-1);
        rtrim_null(rcv_ins_id_cd,' ');

        /* EXEC SQL SELECT 
        mchnt_tp     ,
        term_id      ,
        mchnt_cd     ,
        rcv_ins_id_cd,
        term_id_c    ,
        mchnt_cd_c   ,
        '0'
        INTO :tiBcMchntTermChargeInf        	
        FROM (
             SELECT 
             mchnt_tp     ,
             term_id      ,
             mchnt_cd     ,
             rcv_ins_id_cd,
             term_id_c    ,
             mchnt_cd_c   
             FROM ti_bc_mchnt_term_charge_inf
             WHERE ( mchnt_tp=:mchnt_tp OR rtrim(mchnt_tp)='*' )
             AND ( term_id=:term_id OR rtrim(term_id)='*' )
             AND mchnt_cd=:mchnt_cd
             AND  ( rcv_ins_id_cd=:rcv_ins_id_cd OR rtrim(rcv_ins_id_cd)='*' )
             ORDER BY mchnt_tp desc, term_id desc, mchnt_cd desc, rcv_ins_id_cd desc
             )                     
        WHERE rownum<=1
        ORDER BY ROWNUM ASC
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 11;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select mchnt_tp ,term_id ,mchnt_cd ,rcv_ins_id_cd ,t\
erm_id_c ,mchnt_cd_c ,'0' into :s1 ,:s2 ,:s3 ,:s4 ,:s5 ,:s6 ,:s7   from (sele\
ct mchnt_tp ,term_id ,mchnt_cd ,rcv_ins_id_cd ,term_id_c ,mchnt_cd_c  from ti\
_bc_mchnt_term_charge_inf where ((((mchnt_tp=:b1 or rtrim(mchnt_tp)='*') and \
(term_id=:b2 or rtrim(term_id)='*')) and mchnt_cd=:b3) and (rcv_ins_id_cd=:b4\
 or rtrim(rcv_ins_id_cd)='*')) order by mchnt_tp desc ,term_id desc ,mchnt_cd\
 desc ,rcv_ins_id_cd desc  ) where rownum<=1 order by ROWNUM asc  ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)tiBcMchntTermChargeInf.mchnt_tp;
        sqlstm.sqhstl[0] = (unsigned int  )5;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tiBcMchntTermChargeInf.term_id;
        sqlstm.sqhstl[1] = (unsigned int  )9;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)tiBcMchntTermChargeInf.mchnt_cd;
        sqlstm.sqhstl[2] = (unsigned int  )21;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)tiBcMchntTermChargeInf.rcv_ins_id_cd;
        sqlstm.sqhstl[3] = (unsigned int  )9;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)tiBcMchntTermChargeInf.term_id_c;
        sqlstm.sqhstl[4] = (unsigned int  )9;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)tiBcMchntTermChargeInf.mchnt_cd_c;
        sqlstm.sqhstl[5] = (unsigned int  )21;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)tiBcMchntTermChargeInf.sys_tra_no;
        sqlstm.sqhstl[6] = (unsigned int  )7;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)mchnt_tp;
        sqlstm.sqhstl[7] = (unsigned int  )5;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)term_id;
        sqlstm.sqhstl[8] = (unsigned int  )9;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[9] = (unsigned int  )16;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)rcv_ins_id_cd;
        sqlstm.sqhstl[10] = (unsigned int  )16;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
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
            memcpy(p_tiBcMchntTermChargeInf, &tiBcMchntTermChargeInf, sizeof(ti_bc_mchnt_term_charge_inf_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<FILE:%s,LINE:%d><TI_BC_MCHNT_TERM_CHARGE_INF.pc:tiBcMchntTermChargeInfOpr:SELECT1>Search Record (mchnt_tp:%s,mchnt_cd:%s,term_id:%s,rcv_ins_id_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",__FILE__,__LINE__, mchnt_tp,mchnt_cd,term_id, rcv_ins_id_cd,sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<FILE:%s,LINE:%d><TI_BC_MCHNT_TERM_CHARGE_INF.pc:tiBcMchntTermChargeInfOpr:SELECT1>Search Record (mchnt_tp:%s,mchnt_cd:%s,term_id:%s,rcv_ins_id_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", __FILE__,__LINE__, mchnt_tp,mchnt_cd,term_id,rcv_ins_id_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 11;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )64;
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


