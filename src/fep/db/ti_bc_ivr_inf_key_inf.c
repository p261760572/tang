
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
    "ti_bc_ivr_inf_key_inf.pc"
};


static unsigned int sqlctx = 1359977739;


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
            void  *sqhstv[5];
   unsigned int   sqhstl[5];
            int   sqhsts[5];
            void  *sqindv[5];
            int   sqinds[5];
   unsigned int   sqharm[5];
   unsigned int   *sqharc[5];
   unsigned short  sqadto[5];
   unsigned short  sqtdso[5];
} sqlstm = {12,5};

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

 static char *sq0001 = 
"select opr_cmd ,rspcode ,keyid ,keymemo ,keyflag  from ti_bc_ivr_inf_key_in\
f where (rtrim(opr_cmd)=:b0 and rtrim(rspcode)=:b1)           ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,4274,850,0,0,
5,0,0,1,137,0,9,80,0,0,2,2,0,1,0,1,97,0,0,1,97,0,0,
28,0,0,1,0,0,13,93,0,0,5,0,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
63,0,0,1,0,0,15,101,0,0,0,0,0,1,0,
78,0,0,1,0,0,15,106,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_ivr_inf_key_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

char c_sql[4096];
ti_bc_ivr_inf_key_inf_def tiBcIvrInfKeyInf ;
/* EXEC SQL END DECLARE SECTION; */ 


#define MIN(a,b) a>b?b:a

/*
 *  Function:  tiBcApplConvToIvrOpr
 *
 *
 *  Parameters:
 * 
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tiBcIvrInfKeyInfOpr(int oprType, ti_bc_ivr_inf_key_inf_def *p_tiBcIvrInfKeyInf, int *p_sqlCode)
{
    char   *caString;
    char   *caString1;
    char   *caString2;
    char   caStrBuf[4096];;
    char   caTxnMsg[4096];
    char   caStrInf[4096];
    char   buff[4096];
    char   info[4096];
    char   info1[4096];
    char   caItem[4096];
    int    sqlCode;
    int    iFlag=0;
    int    iRet,iOprListLen=0, iLen=0, iLength=0, ioffsetLength=0;

    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char opr_cmd     [  4 + 1 ];
    char rspcode     [  6 + 1 ];
    /* EXEC SQL END DECLARE SECTION; */ 


    MEMSET_0(opr_cmd)
    MEMSET_0(rspcode)

    memcpy(opr_cmd, p_tiBcIvrInfKeyInf->opr_cmd, 4);
    rtrim(opr_cmd);
    ADJUST_STRING(opr_cmd);

    memcpy(rspcode, p_tiBcIvrInfKeyInf->rspcode, 6);
    rtrim(rspcode);
    ADJUST_STRING(rspcode);

    /* EXEC SQL DECLARE ti_bc_ivr_inf_key_inf_cur CURSOR FOR
         SELECT opr_cmd, rspcode, keyid, keymemo, keyflag
           FROM ti_bc_ivr_inf_key_inf
          WHERE rtrim(opr_cmd)=:opr_cmd
            AND rtrim(rspcode)=:rspcode; */ 

    *p_sqlCode = sqlca.sqlcode;
    if ( sqlca.sqlcode )
    {
        sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcIvrInfKeyInfOpr:SELECT1>declare ti_bc_ivr_inf_key_inf_cur Record (rspcode:%s,opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", rspcode,opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        return -1;
    }

    /* EXEC SQL OPEN ti_bc_ivr_inf_key_inf_cur; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 2;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = sq0001;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5;
    sqlstm.selerr = (unsigned short)1;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqcmod = (unsigned int )0;
    sqlstm.sqhstv[0] = (         void  *)opr_cmd;
    sqlstm.sqhstl[0] = (unsigned int  )5;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)rspcode;
    sqlstm.sqhstl[1] = (unsigned int  )7;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
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
    if ( sqlca.sqlcode )
    {
        sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcIvrInfKeyInfOpr:SELECT1>open ti_bc_ivr_inf_key_inf_cur Record (rspcode:%s,opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", rspcode,opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        return -2;
    }

    while ( 1 )
    {
       	memset(&tiBcIvrInfKeyInf, 0, sizeof(ti_bc_ivr_inf_key_inf_def));

	/* EXEC SQL FETCH ti_bc_ivr_inf_key_inf_cur into :tiBcIvrInfKeyInf.opr_cmd, :tiBcIvrInfKeyInf.rspcode, :tiBcIvrInfKeyInf.keyid, :tiBcIvrInfKeyInf.keymemo, :tiBcIvrInfKeyInf.keyflag; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 5;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )28;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)(tiBcIvrInfKeyInf.opr_cmd);
 sqlstm.sqhstl[0] = (unsigned int  )5;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)(tiBcIvrInfKeyInf.rspcode);
 sqlstm.sqhstl[1] = (unsigned int  )7;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)(tiBcIvrInfKeyInf.keyid);
 sqlstm.sqhstl[2] = (unsigned int  )31;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)(tiBcIvrInfKeyInf.keymemo);
 sqlstm.sqhstl[3] = (unsigned int  )101;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)(tiBcIvrInfKeyInf.keyflag);
 sqlstm.sqhstl[4] = (unsigned int  )2;
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
	if(sqlca.sqlcode  == 1403)
       		break;
	if(sqlca.sqlcode )
	{
        	sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcApplConvToIvrOpr:SELECT1>fetch ti_bc_ivr_settle_inf Record (rspcode:%s,opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", rspcode,opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        	glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
		/* EXEC SQL CLOSE ti_bc_ivr_inf_key_inf_cur ; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )63;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return -3;
	}
	iFlag = 1;
    }
    /* EXEC SQL CLOSE ti_bc_ivr_inf_key_inf_cur ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 5;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )78;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


    if (iFlag == 0)
	*p_sqlCode  = SQL_ERD_NONE_RECORD;
    else
	*p_sqlCode  = 0;

    return 0;
}


