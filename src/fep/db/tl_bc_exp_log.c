
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
           char  filnam[17];
};
static struct sqlcxp sqlfpn =
{
    16,
    "tl_bc_exp_log.pc"
};


static unsigned int sqlctx = 5359619;


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
            void  *sqhstv[3];
   unsigned int   sqhstl[3];
            int   sqhsts[3];
            void  *sqindv[3];
            int   sqinds[3];
   unsigned int   sqharm[3];
   unsigned int   *sqharc[3];
   unsigned short  sqadto[3];
   unsigned short  sqtdso[3];
} sqlstm = {12,3};

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
5,0,0,1,145,0,3,62,0,0,3,3,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,
32,0,0,2,0,0,29,86,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/tl_bc_exp_log.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 

/* EXEC SQL BEGIN DECLARE SECTION; */ 

tl_bc_exp_log_def tlBcExpLog ;
/* EXEC SQL END DECLARE SECTION; */ 




/*
 *  Function:  tlBcExpLogOpr
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

int tlBcExpLogOpr(int oprType, tl_bc_exp_log_def *p_tlBcExplLog, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    /* EXEC SQL END DECLARE SECTION; */ 


    int i;
    char caTxnMsg[2048];

    switch ( oprType )
    {

    case GLB_DB_INSERT1:
    

        /*copy buff*/
        memcpy(&tlBcExpLog, p_tlBcExplLog, sizeof(tl_bc_exp_log_def));

        /*rtrim(tlBcExpLog.sys_date);
        rtrim(tlBcExpLog.sys_time);*/
        ADJUST_STRING(tlBcExpLog.rsn_code       )
        ADJUST_STRING(tlBcExpLog.rsn_desc       )

        /* EXEC SQL INSERT INTO tl_bc_exp_log(sys_date,sys_time,rsn_code,rsn_desc,data) values
        (to_char(sysdate,'YYYYMMDD'),
         to_char(sysdate,'HH24MMSS'),
         :tlBcExpLog.rsn_code,
         :tlBcExpLog.rsn_desc,
         :tlBcExpLog.data
        )
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 3;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "insert into tl_bc_exp_log (sys_date,sys_time,rsn_cod\
e,rsn_desc,data) values (to_char(sysdate,'YYYYMMDD'),to_char(sysdate,'HH24MMS\
S'),:b0,:b1,:b2)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcExpLog.rsn_code);
        sqlstm.sqhstl[0] = (unsigned int  )5;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcExpLog.rsn_desc);
        sqlstm.sqhstl[1] = (unsigned int  )201;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcExpLog.data);
        sqlstm.sqhstl[2] = (unsigned int  )2002;
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


        
        /* insert result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {

        } else if ( memcmp(sqlca.sqlerrm.sqlerrmc, "ORA-00001", 9 ) == 0 )
        {
            *p_sqlCode = SQL_ERD_UNIKEY;
            sprintf(caTxnMsg, "<TL_BC_EXP_LOG.pc:tlBcExpLogOpr:INSERT1>Insert Record is exist.(sys_date:%s,sys_time:%s,rsn_code:%s,rsn_desc:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", tlBcExpLog.sys_date,tlBcExpLog.sys_time,tlBcExpLog.rsn_code,tlBcExpLog.rsn_desc, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 100, caTxnMsg, RPT_TO_LOG, 0, 0 );
        } else
        {
            sprintf(caTxnMsg, "<TL_BC_EXP_LOG.pc:tlBcExpLogOpr:INSERT1>Insert Record is exist.(sys_date:%s,sys_time:%s,rsn_code:%s,rsn_desc:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", tlBcExpLog.sys_date,tlBcExpLog.sys_time,tlBcExpLog.rsn_code,tlBcExpLog.rsn_desc, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 100, caTxnMsg, RPT_TO_LOG, 0, 0 );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 3;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )32;
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


