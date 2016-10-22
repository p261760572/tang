
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
           char  filnam[13];
};
static struct sqlcxp sqlfpn =
{
    12,
    "ti_bc_ivr.pc"
};


static unsigned int sqlctx = 331755;


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
            void  *sqhstv[8];
   unsigned int   sqhstl[8];
            int   sqhsts[8];
            void  *sqindv[8];
            int   sqinds[8];
   unsigned int   sqharm[8];
   unsigned int   *sqharc[8];
   unsigned short  sqadto[8];
   unsigned short  sqtdso[8];
} sqlstm = {12,8};

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
"select opr_list ,seq_no ,data_list ,data_len ,data_flag ,spe_cmd ,spe_lengt\
h ,memo  from ti_bc_ivr_settle_inf where (rtrim(opr_cmd)=:b0 and rtrim(rspcod\
e)=:b1) order by seq_no asc             ";

 static char *sq0002 = 
"select keyid ,keymemo ,keyflag  from ti_bc_ivr_inf_key_inf where (rtrim(opr\
_cmd)=:b0 and rtrim(rspcode)=:b1)           ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,4274,850,0,0,
5,0,0,1,192,0,9,96,0,0,2,2,0,1,0,1,97,0,0,1,97,0,0,
28,0,0,1,0,0,13,109,0,0,8,0,0,1,0,2,97,0,0,2,3,0,0,2,97,0,0,2,3,0,0,2,97,0,0,2,
97,0,0,2,3,0,0,2,97,0,0,
75,0,0,1,0,0,15,118,0,0,0,0,0,1,0,
90,0,0,1,0,0,15,142,0,0,0,0,0,1,0,
105,0,0,2,119,0,9,146,0,0,2,2,0,1,0,1,97,0,0,1,97,0,0,
128,0,0,1,0,0,15,152,0,0,0,0,0,1,0,
143,0,0,1,0,0,15,174,0,0,0,0,0,1,0,
158,0,0,2,0,0,13,182,0,0,3,0,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,
185,0,0,1,0,0,15,191,0,0,0,0,0,1,0,
200,0,0,2,0,0,15,192,0,0,0,0,0,1,0,
215,0,0,2,0,0,15,242,0,0,0,0,0,1,0,
230,0,0,1,0,0,15,243,0,0,0,0,0,1,0,
245,0,0,2,0,0,15,272,0,0,0,0,0,1,0,
260,0,0,1,0,0,15,273,0,0,0,0,0,1,0,
275,0,0,2,0,0,15,302,0,0,0,0,0,1,0,
290,0,0,1,0,0,15,568,0,0,0,0,0,1,0,
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
#include "./inc/ti_bc_ivr_settle_inf.h"
#include "./inc/ti_bc_ivr_mobile_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

char c_sql[4096];
ti_bc_ivr_inf_key_inf_def tiBcIvrInfKeyInf ;
ti_bc_ivr_settle_inf_def tiBcIvrSettleInf ;
ti_bc_ivr_mobile_inf_def g_tiBcIvrMobileInf ;
/* EXEC SQL END DECLARE SECTION; */ 


#define MIN(a,b) a>b?b:a

/*
 *  Function:  tiBcApplConvToIvrOpr
 *
 *
 *  Parameters:
 * 
 *      InBuf         - 报文输入串
 *      pcaOprCmd     - 指令串
 *      pcaRspCode    - 信息码串
 *      OprListBuf    - 操作码串 输出
 *      OutBuf        - 信息串 输出
 *      p_sqlCode     - 返回值， 输出
 *      piLen         - 操作码串长度, 输出
 *      piLen         - 信息串长度, 输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tiBcApplConvToIvrOpr(char *InBuf, char *pcaOprCmd, char *pcaRspCode, char *OprListBuf, char *OutBuf, int *p_sqlCode, int *piOprListLen, int *piLen)
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
    int    iRet,iOprListLen=0, iLen=0, iLength=0, ioffsetLength=0;

    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char opr_cmd     [  4 + 1 ];
    char rspcode     [  6 + 1 ];
    /* EXEC SQL END DECLARE SECTION; */ 


    ti_bc_ivr_mobile_inf_def *p_tiBcIvrMobileInf = &g_tiBcIvrMobileInf ;
    dcs_debug(pcaOprCmd, strlen(pcaOprCmd), "OprCmd");
    dcs_debug(pcaRspCode, strlen(pcaRspCode), "RspCode");
    MEMSET_0(opr_cmd)
    MEMSET_0(rspcode)

    memcpy(opr_cmd, pcaOprCmd, 4);
    rtrim(opr_cmd);
    ADJUST_STRING(opr_cmd);

    memcpy(rspcode, pcaRspCode, 6);
    rtrim(rspcode);
    ADJUST_STRING(rspcode);

    rtrim(InBuf);
    /* EXEC SQL DECLARE ti_bc_ivr_settle_inf_cur CURSOR FOR
         SELECT opr_list, seq_no, data_list, data_len, data_flag, spe_cmd, spe_length, memo
           FROM ti_bc_ivr_settle_inf
          WHERE rtrim(opr_cmd)=:opr_cmd
            AND rtrim(rspcode)=:rspcode
          ORDER BY seq_no ASC; */ 

    *p_sqlCode = sqlca.sqlcode;
    if ( sqlca.sqlcode )
    {
        sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcApplConvToIvrOpr:SELECT1>declare ti_bc_ivr_settle_inf_cur Record (rspcode:%s,opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", rspcode,opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        return -1;
    }

    /* EXEC SQL OPEN ti_bc_ivr_settle_inf_cur; */ 

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
        sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcApplConvToIvrOpr:SELECT1>open ti_bc_ivr_settle_inf_cur Record (rspcode:%s,opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", rspcode,opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        return -2;
    }

    while ( 1 )
    {
       	memset(&tiBcIvrSettleInf, 0, sizeof(ti_bc_ivr_settle_inf_def));

	/* EXEC SQL FETCH ti_bc_ivr_settle_inf_cur into :tiBcIvrSettleInf.opr_list, :tiBcIvrSettleInf.seq_no, :tiBcIvrSettleInf.data_list, :tiBcIvrSettleInf.data_len, :tiBcIvrSettleInf.data_flag, :tiBcIvrSettleInf.spe_cmd, :tiBcIvrSettleInf.spe_length, :tiBcIvrSettleInf.memo; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
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
 sqlstm.sqhstv[0] = (         void  *)(tiBcIvrSettleInf.opr_list);
 sqlstm.sqhstl[0] = (unsigned int  )3;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&(tiBcIvrSettleInf.seq_no);
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)(tiBcIvrSettleInf.data_list);
 sqlstm.sqhstl[2] = (unsigned int  )1025;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&(tiBcIvrSettleInf.data_len);
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)(tiBcIvrSettleInf.data_flag);
 sqlstm.sqhstl[4] = (unsigned int  )2;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)(tiBcIvrSettleInf.spe_cmd);
 sqlstm.sqhstl[5] = (unsigned int  )3;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)&(tiBcIvrSettleInf.spe_length);
 sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)(tiBcIvrSettleInf.memo);
 sqlstm.sqhstl[7] = (unsigned int  )501;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
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
	if(sqlca.sqlcode  == 1403)
       		break;
	if(sqlca.sqlcode )
	{
        	sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcApplConvToIvrOpr:SELECT1>fetch ti_bc_ivr_settle_inf Record (rspcode:%s,opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", rspcode,opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        	glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
		/* EXEC SQL CLOSE ti_bc_ivr_settle_inf_cur ; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 8;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )75;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return -3;
	}

	/* 操作码串 */
    	memcpy( OprListBuf+iOprListLen, tiBcIvrSettleInf.opr_list, 2);
	iOprListLen = iOprListLen + 2;

	if (tiBcIvrSettleInf.data_flag[0] == '0' && tiBcIvrSettleInf.data_len == 0)
	{
                continue; 
	}
	else
	{           
    		/* EXEC SQL DECLARE ti_bc_ivr_inf_key_inf_cur CURSOR FOR
         		SELECT keyid, keymemo, keyflag  
           		FROM ti_bc_ivr_inf_key_inf
          		WHERE rtrim(opr_cmd)=:opr_cmd
            		AND rtrim(rspcode)=:rspcode; */ 

    		*p_sqlCode = sqlca.sqlcode;
    		if ( sqlca.sqlcode )
    		{
        		sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcApplConvToIvrOpr:SELECT1>declare ti_bc_ivr_inf_key_inf_cur Record (rspcode:%s,opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", rspcode,opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        		glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
			/* EXEC SQL CLOSE ti_bc_ivr_settle_inf_cur ; */ 

{
   struct sqlexd sqlstm;
   sqlorat((void **)0, &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 8;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )90;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        		return -4;
    		}
		
    		/* EXEC SQL OPEN ti_bc_ivr_inf_key_inf_cur; */ 

{
      struct sqlexd sqlstm;
      sqlorat((void **)0, &sqlctx, &oraca);
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = sq0002;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )105;
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
        		sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcApplConvToIvrOpr:SELECT1>open ti_bc_ivr_inf_key_inf_cur Record (rspcode:%s,opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", rspcode,opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        		glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
			/* EXEC SQL CLOSE ti_bc_ivr_settle_inf_cur ; */ 

{
   struct sqlexd sqlstm;
   sqlorat((void **)0, &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 8;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )128;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        		return -5;
    		}
	
		memset( caStrInf, 0, sizeof(caStrInf));
		memset( buff, 0, sizeof(buff));
		memset( info, 0, sizeof(info));
		if ((strlen(tiBcIvrSettleInf.data_list))==0)
		{
    			memcpy( caStrInf, tiBcIvrSettleInf.data_list, strlen(tiBcIvrSettleInf.data_list));
		}
		else
		{
        		iRet = GBKToUTF8(tiBcIvrSettleInf.data_list, strlen(tiBcIvrSettleInf.data_list), info, sizeof(info));
        		if( 0 < iRet )
			{
    				memcpy( caStrInf, info, iRet);
			}
			else
			{
        			sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcApplConvToIvrOpr:GBKToUTF8>Error[%d].\n", iRet );
        			glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
				/* EXEC SQL CLOSE ti_bc_ivr_settle_inf_cur ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 8;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )143;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        			return -100;
			}
		}
    		while(1)
    		{
       			memset(&tiBcIvrInfKeyInf, 0, sizeof(ti_bc_ivr_inf_key_inf_def));
		
			/* EXEC SQL FETCH ti_bc_ivr_inf_key_inf_cur into :tiBcIvrInfKeyInf.keyid, :tiBcIvrInfKeyInf.keymemo, :tiBcIvrInfKeyInf.keyflag; */ 

{
   struct sqlexd sqlstm;
   sqlorat((void **)0, &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 8;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )158;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (           int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (         void  *)(tiBcIvrInfKeyInf.keyid);
   sqlstm.sqhstl[0] = (unsigned int  )31;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)(tiBcIvrInfKeyInf.keymemo);
   sqlstm.sqhstl[1] = (unsigned int  )101;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)(tiBcIvrInfKeyInf.keyflag);
   sqlstm.sqhstl[2] = (unsigned int  )2;
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


       			*p_sqlCode = sqlca.sqlcode;
       			if ( sqlca.sqlcode )
			if(sqlca.sqlcode  == 1403)
        			break;
			if(sqlca.sqlcode )
			{
        			sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcApplConvToIvrOpr:SELECT1>fetch ti_bc_ivr_inf_key_inf Record (rspcode:%s,opr_cmd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", rspcode,opr_cmd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        			glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
				/* EXEC SQL CLOSE ti_bc_ivr_settle_inf_cur ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 8;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )185;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


				/* EXEC SQL CLOSE ti_bc_ivr_inf_key_inf_cur ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 8;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )200;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


				return -6;
			}
			caString = NULL;
    			rtrim(caStrInf);
    			rtrim(tiBcIvrInfKeyInf.keyid);
    			rtrim(tiBcIvrInfKeyInf.keymemo);
        		caString = (char *)strstr( (char *)caStrInf, tiBcIvrInfKeyInf.keyid);  /* 信息串 */
      			sprintf(caTxnMsg, " StrInf[%s] strstr keyid[%s] caString[%s] \n", caStrInf, tiBcIvrInfKeyInf.keyid, caString );
       			glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        		if (caString)
        		{
				if (tiBcIvrInfKeyInf.keyflag[0] == '0')
				{
        				caString1 = (char *)strstr( (char *)InBuf, tiBcIvrInfKeyInf.keyid); /* 报文发送过来的关键字串 */
      					sprintf(caTxnMsg, " StrInf[%s] strstr keyid[%s] caString1[%s] \n", InBuf, tiBcIvrInfKeyInf.keyid, caString1 );
       					glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        				if (caString1)
        				{
    						memset( caStrBuf, 0, sizeof(caStrBuf));
						memcpy( caStrBuf, caStrInf, caString-caStrInf);
        					caString2 = (char *)strstr( (char *)caString1, "<~~/~~>");
        					if (caString2)
						{
							if (!strcmp(tiBcIvrInfKeyInf.keyid, "<~~AMT~~>"))
							{
    								memset( info, 0, sizeof(info));
    								memset( info1, 0, sizeof(info1));
								memcpy( info, caString1+strlen(tiBcIvrInfKeyInf.keyid), caString2 - caString1 - strlen(tiBcIvrInfKeyInf.keyid) );
								sprintf(info1, "%.2lf", atof( info ) / 100.00);
								memcpy( caStrBuf+(caString-caStrInf), info1, strlen(info1));
								strcpy( caStrBuf+(caString-caStrInf)+strlen(info1), caString+strlen(tiBcIvrInfKeyInf.keyid));
      								sprintf(caTxnMsg, " caStrBuf[%s]\n", caStrBuf );
							}
							else if (!strcmp(tiBcIvrInfKeyInf.keyid, "<~~TEL~~>"))
							{
    								memset( info, 0, sizeof(info));
								memcpy( info, caString1+strlen(tiBcIvrInfKeyInf.keyid), caString2 - caString1 - strlen(tiBcIvrInfKeyInf.keyid) );
								if (!memcmp(info, "0", 1))
								{
    									memset( info, 0, sizeof(info));
									memcpy( info, caString1+strlen(tiBcIvrInfKeyInf.keyid)+1, caString2 - caString1 - strlen(tiBcIvrInfKeyInf.keyid)-1);
								}
								memset(&g_tiBcIvrMobileInf, 0, sizeof(ti_bc_ivr_mobile_inf_def));
								memcpy(p_tiBcIvrMobileInf->mobileno, info, strlen(info));
								tiBcIvrMobileInfOpr(GLB_DB_SELECT1, p_tiBcIvrMobileInf, &sqlCode);
       								if ( (0 != sqlCode) && (SQL_ERD_NONE_RECORD != sqlCode) )
								{
        								sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcIvrMobileInfOpr Error.\n");
        								glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
									/* EXEC SQL CLOSE ti_bc_ivr_inf_key_inf_cur ; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 8;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )215;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


									/* EXEC SQL CLOSE ti_bc_ivr_settle_inf_cur ; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 8;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )230;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


									return -8;
								}
								else if (SQL_ERD_NONE_RECORD == sqlCode)
								{
									memcpy( caStrBuf+(caString-caStrInf), "0", 1);
									memcpy( caStrBuf+(caString-caStrInf)+1, info, strlen(info));
									strcpy( caStrBuf+(caString-caStrInf)+1+strlen(info), caString+strlen(tiBcIvrInfKeyInf.keyid));
								}
								else
								{
									rtrim(p_tiBcIvrMobileInf->preno);
									memcpy( caStrBuf+(caString-caStrInf), p_tiBcIvrMobileInf->preno, strlen(p_tiBcIvrMobileInf->preno));
									memcpy( caStrBuf+(caString-caStrInf)+strlen(p_tiBcIvrMobileInf->preno), info, strlen(info));
									strcpy( caStrBuf+(caString-caStrInf)+strlen(p_tiBcIvrMobileInf->preno)+strlen(info), caString+strlen(tiBcIvrInfKeyInf.keyid));
								}
							}
							else  
							{
								memcpy( caStrBuf+(caString-caStrInf), caString1+strlen(tiBcIvrInfKeyInf.keyid), caString2 - caString1 - strlen(tiBcIvrInfKeyInf.keyid) );
								strcpy( caStrBuf+(caString-caStrInf)+(caString2 - caString1 - strlen(tiBcIvrInfKeyInf.keyid)), caString+strlen(tiBcIvrInfKeyInf.keyid));
      								sprintf(caTxnMsg, " caStrBuf[%s]\n", caStrBuf );
       								glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
							}
						}
						else
						{
        						sprintf(caTxnMsg, "<ti_bc_ivr_inf_key_inf.pc:tiBcApplConvToIvrOpr:没有结束符<~~/~~> not exist Error.\n");
        						glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
							/* EXEC SQL CLOSE ti_bc_ivr_inf_key_inf_cur ; */ 

{
       struct sqlexd sqlstm;
       sqlorat((void **)0, &sqlctx, &oraca);
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 8;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )245;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


							/* EXEC SQL CLOSE ti_bc_ivr_settle_inf_cur ; */ 

{
       struct sqlexd sqlstm;
       sqlorat((void **)0, &sqlctx, &oraca);
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 8;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )260;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


							return -7;
						}
					}
					else
					{
    						memset( caStrBuf, 0, sizeof(caStrBuf));
						memcpy( caStrBuf, caStrInf, caString-caStrInf);
						strcpy( caStrBuf+(caString-caStrInf), caString+strlen(tiBcIvrInfKeyInf.keyid)); 
      						sprintf(caTxnMsg, " caStrBuf[%s]\n", caStrBuf );
       						glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
					}
				}
				else
				{
    					memset( caStrBuf, 0, sizeof(caStrBuf));
					memcpy( caStrBuf, caStrInf, caString-caStrInf);
					memcpy( caStrBuf+(caString-caStrInf), tiBcIvrInfKeyInf.keymemo, strlen(tiBcIvrInfKeyInf.keymemo));
					strcpy( caStrBuf+(caString-caStrInf)+strlen(tiBcIvrInfKeyInf.keymemo), caString+strlen(tiBcIvrInfKeyInf.keyid)); 
      					sprintf(caTxnMsg, " caStrBuf[%s]\n", caStrBuf );
       					glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
				}
				memset( caStrInf, 0, sizeof(caStrInf));
    				memcpy( caStrInf, caStrBuf, strlen(caStrBuf));

      				sprintf(caTxnMsg, " caStrInf end [%s]\n", caStrInf );
       				glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
			}
		}
    		/* EXEC SQL CLOSE ti_bc_ivr_inf_key_inf_cur; */ 

{
      struct sqlexd sqlstm;
      sqlorat((void **)0, &sqlctx, &oraca);
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )275;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	
		if (tiBcIvrSettleInf.data_flag[0] == '0')
		{
			memcpy(OutBuf+iLen, caStrInf, tiBcIvrSettleInf.data_len);
			iLen = iLen + tiBcIvrSettleInf.data_len;
		}
		else if (tiBcIvrSettleInf.data_flag[0] == '1')
		{
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%04d", strlen(caStrInf));
			asc_to_bcd(caItem,caItem,4,0);
			memcpy(OutBuf+iLen, caItem, 2);
			iLen = iLen + 2;
			memcpy(OutBuf+iLen, caStrInf, strlen(caStrInf));
			iLen = iLen + strlen(caStrInf);
		}
		else if (tiBcIvrSettleInf.data_flag[0] == '2')
		{
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%02d", strlen(caStrInf));
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			memcpy(OutBuf+iLen, caStrInf, strlen(caStrInf));
			iLen = iLen + strlen(caStrInf);
		}
		else if (tiBcIvrSettleInf.data_flag[0] == '3')
		{
			ioffsetLength = iLen;
			iLen = iLen + 2;
			iLength = 0;
			memset( caItem, 0, sizeof(caItem));
			memcpy( caItem, tiBcIvrSettleInf.spe_cmd, 2);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%02d", tiBcIvrSettleInf.spe_length);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x00, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x00, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%04d", iLength);
			asc_to_bcd(caItem,caItem,4,0);
			memcpy(OutBuf+ioffsetLength, caItem, 2);
		}
		else if (tiBcIvrSettleInf.data_flag[0] == '4')
		{
			ioffsetLength = iLen;
			iLen = iLen + 2;
			iLength = 0;
			memset( caItem, 0, sizeof(caItem));
			memcpy( caItem, tiBcIvrSettleInf.spe_cmd, 2);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%02d", tiBcIvrSettleInf.spe_length);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x01, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x00, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memcpy(OutBuf+iLen, caStrInf, strlen(caStrInf));
			iLen = iLen + strlen(caStrInf);
			iLength = iLength + strlen(caStrInf);
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%04d", iLength);
			asc_to_bcd(caItem,caItem,4,0);
			memcpy(OutBuf+ioffsetLength, caItem, 2);

		}
		else if (tiBcIvrSettleInf.data_flag[0] == '5')
		{
			ioffsetLength = iLen;
			iLen = iLen + 2;
			iLength = 0;
			memset( caItem, 0, sizeof(caItem));
			memcpy( caItem, tiBcIvrSettleInf.spe_cmd, 2);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%02d", tiBcIvrSettleInf.spe_length);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x10, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x00, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%04d", iLength);
			asc_to_bcd(caItem,caItem,4,0);
			memcpy(OutBuf+ioffsetLength, caItem, 2);
		}
		else if (tiBcIvrSettleInf.data_flag[0] == '6')
		{
			ioffsetLength = iLen;
			iLen = iLen + 2;
			iLength = 0;
			memset( caItem, 0, sizeof(caItem));
			memcpy( caItem, tiBcIvrSettleInf.spe_cmd, 2);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%02d", tiBcIvrSettleInf.spe_length);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x11, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x00, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memcpy(OutBuf+iLen, caStrInf, strlen(caStrInf));
			iLen = iLen + strlen(caStrInf);
			iLength = iLength + strlen(caStrInf);
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%04d", iLength);
			asc_to_bcd(caItem,caItem,4,0);
			memcpy(OutBuf+ioffsetLength, caItem, 2);
		}
		else if (tiBcIvrSettleInf.data_flag[0] == '7')
		{
			ioffsetLength = iLen;
			iLen = iLen + 2;
			iLength = 0;
			memset( caItem, 0, sizeof(caItem));
			memcpy( caItem, tiBcIvrSettleInf.spe_cmd, 2);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%02d", tiBcIvrSettleInf.spe_length);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x00, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x11, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%04d", iLength);
			asc_to_bcd(caItem,caItem,4,0);
			memcpy(OutBuf+ioffsetLength, caItem, 2);
		}
		else if (tiBcIvrSettleInf.data_flag[0] == '8')
		{
			ioffsetLength = iLen;
			iLen = iLen + 2;
			iLength = 0;
			memset( caItem, 0, sizeof(caItem));
			memcpy( caItem, tiBcIvrSettleInf.spe_cmd, 2);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%02d", tiBcIvrSettleInf.spe_length);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x01, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x11, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memcpy(OutBuf+iLen, caStrInf, strlen(caStrInf));
			iLen = iLen + strlen(caStrInf);
			iLength = iLength + strlen(caStrInf);
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%04d", iLength);
			asc_to_bcd(caItem,caItem,4,0);
			memcpy(OutBuf+ioffsetLength, caItem, 2);
		}
		else if (tiBcIvrSettleInf.data_flag[0] == '9')
		{
			ioffsetLength = iLen;
			iLen = iLen + 2;
			iLength = 0;
			memset( caItem, 0, sizeof(caItem));
			memcpy( caItem, tiBcIvrSettleInf.spe_cmd, 2);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%02d", tiBcIvrSettleInf.spe_length);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x10, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x11, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%04d", iLength);
			asc_to_bcd(caItem,caItem,4,0);
			memcpy(OutBuf+ioffsetLength, caItem, 2);
		}
		else if (tiBcIvrSettleInf.data_flag[0] == 'a')
		{
			ioffsetLength = iLen;
			iLen = iLen + 2;
			iLength = 0;
			memset( caItem, 0, sizeof(caItem));
			memcpy( caItem, tiBcIvrSettleInf.spe_cmd, 2);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%02d", tiBcIvrSettleInf.spe_length);
			asc_to_bcd(caItem,caItem,2,0);
			memcpy(OutBuf+iLen, caItem, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x11, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memset(OutBuf+iLen, 0x11, 1);
			iLen = iLen + 1;
			iLength = iLength + 1;
			memcpy(OutBuf+iLen, caStrInf, strlen(caStrInf));
			iLen = iLen + strlen(caStrInf);
			iLength = iLength + strlen(caStrInf);
			memset( caItem, 0, sizeof(caItem));
			sprintf( caItem, "%04d", iLength);
			asc_to_bcd(caItem,caItem,4,0);
			memcpy(OutBuf+ioffsetLength, caItem, 2);
		}
	}
    }
    /* EXEC SQL CLOSE ti_bc_ivr_settle_inf_cur ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 8;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )290;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



    *piOprListLen = iOprListLen;
    *piLen = iLen;

    return 0;
}


