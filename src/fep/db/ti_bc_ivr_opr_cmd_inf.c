
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
           char  filnam[25];
};
static struct sqlcxp sqlfpn =
{
    24,
    "ti_bc_ivr_opr_cmd_inf.pc"
};


static unsigned int sqlctx = 1360054923;


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
5,0,0,1,97,0,4,62,0,0,3,1,0,1,0,2,97,0,0,2,97,0,0,1,97,0,0,
32,0,0,2,0,0,29,84,0,0,0,0,0,1,0,
47,0,0,3,98,0,4,93,0,0,3,1,0,1,0,2,97,0,0,2,97,0,0,1,97,0,0,
74,0,0,4,0,0,29,115,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_ivr_opr_cmd_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

ti_bc_ivr_opr_cmd_inf_def tlBcIvrOprCmdInf ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tiBcIvrOprCmdInfOpr
 *
 *
 *  Parameters:
 * 
 *      oprType - �������ͣ� ���룬 ȡֵ
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode        - ����ֵ�� ���
 *      p_tlBcTransLog   - ��������������
 *                         ���ҽ�������
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tiBcIvrOprCmdInfOpr(int oprType, ti_bc_ivr_opr_cmd_inf_def *p_tlBcIvrOprCmdInf, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char src_opr_cmd  [ 4   + 1 ];
    char dest_opr_cmd  [ 4   + 1 ];
    /* EXEC SQL END DECLARE SECTION; */ 


    int i;
    char buff[256];
    MEMSET_0(src_opr_cmd);
    MEMSET_0(dest_opr_cmd);

    char caTxnMsg[2048];
    switch ( oprType )
    {
    
    case GLB_DB_SELECT1:

        memcpy(src_opr_cmd, p_tlBcIvrOprCmdInf->src_opr_cmd,4) ;
        src_opr_cmd[4]=0 ;
        ADJUST_STRING(src_opr_cmd) ;

        /* EXEC SQL SELECT 
        dest_opr_cmd      ,
        src_opr_cmd     
        INTO :tlBcIvrOprCmdInf
        FROM ti_bc_ivr_opr_cmd_inf
        WHERE src_opr_cmd=:src_opr_cmd; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 3;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select dest_opr_cmd ,src_opr_cmd into :s1 ,:s2   fro\
m ti_bc_ivr_opr_cmd_inf where src_opr_cmd=:b1";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)tlBcIvrOprCmdInf.dest_opr_cmd;
        sqlstm.sqhstl[0] = (unsigned int  )5;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tlBcIvrOprCmdInf.src_opr_cmd;
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)src_opr_cmd;
        sqlstm.sqhstl[2] = (unsigned int  )5;
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
            memcpy(p_tlBcIvrOprCmdInf, &tlBcIvrOprCmdInf, sizeof(ti_bc_ivr_opr_cmd_inf_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            sprintf(caTxnMsg, "<TL_BC_TRANS_LOG.pc:tiBcIvrOprCmdInfOpr:SELECT1>Search Record (src_opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", src_opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        } else
        {
            sprintf(caTxnMsg, "<TL_BC_TRANS_LOG.pc:tiBcIvrOprCmdInfOpr:SELECT1>Search Record (src_opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", src_opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
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

    case GLB_DB_SELECT2:

        memcpy(dest_opr_cmd, p_tlBcIvrOprCmdInf->dest_opr_cmd,4) ;
        dest_opr_cmd[4]=0 ;
        ADJUST_STRING(dest_opr_cmd) ;

        /* EXEC SQL SELECT 
        dest_opr_cmd      ,
        src_opr_cmd     
        INTO :tlBcIvrOprCmdInf
        FROM ti_bc_ivr_opr_cmd_inf
        WHERE dest_opr_cmd=:dest_opr_cmd; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 3;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select dest_opr_cmd ,src_opr_cmd into :s1 ,:s2   fro\
m ti_bc_ivr_opr_cmd_inf where dest_opr_cmd=:b1";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )47;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)tlBcIvrOprCmdInf.dest_opr_cmd;
        sqlstm.sqhstl[0] = (unsigned int  )5;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tlBcIvrOprCmdInf.src_opr_cmd;
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)dest_opr_cmd;
        sqlstm.sqhstl[2] = (unsigned int  )5;
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
            memcpy(p_tlBcIvrOprCmdInf, &tlBcIvrOprCmdInf, sizeof(ti_bc_ivr_opr_cmd_inf_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            sprintf(caTxnMsg, "<TL_BC_TRANS_LOG.pc:tiBcIvrOprCmdInfOpr:SELECT1>Search Record (dest_opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", dest_opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        } else
        {
            sprintf(caTxnMsg, "<TL_BC_TRANS_LOG.pc:tiBcIvrOprCmdInfOpr:SELECT1>Search Record (dest_opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", dest_opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
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
        sqlstm.offset = (unsigned int  )74;
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

