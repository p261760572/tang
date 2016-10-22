
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
    "tl_bc_cardno_list_inf.pc"
};


static unsigned int sqlctx = 1370034315;


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
            void  *sqhstv[17];
   unsigned int   sqhstl[17];
            int   sqhsts[17];
            void  *sqindv[17];
            int   sqinds[17];
   unsigned int   sqharm[17];
   unsigned int   *sqharc[17];
   unsigned short  sqadto[17];
   unsigned short  sqtdso[17];
} sqlstm = {12,17};

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
5,0,0,1,55,0,4,61,0,0,1,0,0,1,0,2,97,0,0,
24,0,0,2,402,0,4,73,0,0,17,1,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,1,97,0,0,
107,0,0,3,0,0,29,206,0,0,0,0,0,1,0,
122,0,0,4,357,0,5,228,0,0,16,16,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,
201,0,0,5,0,0,29,259,0,0,0,0,0,1,0,
216,0,0,6,346,0,3,286,0,0,16,16,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,
295,0,0,7,0,0,29,321,0,0,0,0,0,1,0,
310,0,0,8,0,0,29,327,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/tl_bc_cardno_list_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

tl_bc_cardno_list_inf_def tlBcCardnoListInf ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tlBcCardnoListInfOpr
 *
 *
 *  Parameters:
 * 
 *      oprType - 操作类型， 输入， 取值
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode        - 返回值， 输出
 *      p_tlBcCardnoListInf   - 查找条件，输入
 *                         查找结果，输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tlBcCardnoListInfOpr(int oprType, tl_bc_cardno_list_inf_def *p_tlBcCardnoListInf, int *p_sqlCode)
{
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char  cardno         [19 + 1] ;  
    char  sysdate         [100 + 1] ;  
    char  c_sql[2048];
    /* EXEC SQL END DECLARE SECTION; */ 


    char buff[256];

    MEMSET_0(cardno);

    char caTxnMsg[4096];
    switch ( oprType )
    {
    
    case GLB_DB_SELECT1:
        
        strcpy_safe(cardno, p_tlBcCardnoListInf->cardno, sizeof(cardno)-1);
        rtrim_null(cardno,' ');
        
	memset(sysdate, 0, sizeof(sysdate));
        /* EXEC SQL SELECT 
		 to_char(sysdate, 'YYYYMMDD')
		 INTO :sysdate
		 FROM dual; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 1;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select to_char(sysdate,'YYYYMMDD') into :b0  from du\
al ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)sysdate;
        sqlstm.sqhstl[0] = (unsigned int  )101;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
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
        if ( sqlca.sqlcode != 0 )
	{
            	glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcCardnoListInfOpr:SELECT1>select Record Failure.sqlcode:%d,sqlca.sqlerrm.sqlerrmc:%s",sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
		return -1;	
	}
	rtrim(sysdate);

        /* EXEC SQL SELECT 
    		cardno,
    		relation1,
    		relation_crtday1,
    		relation_lastday1,
    		relation2,
    		relation_crtday2,
    		relation_lastday2,
    		relation3,
    		relation_crtday3,
    		relation_lastday3,
    		relation4,
    		relation_crtday4,
    		relation_lastday4,
    		relation5,
    		relation_crtday5,
    		relation_lastday5
        INTO 
           :tlBcCardnoListInf.cardno               ,   
           :tlBcCardnoListInf.relation1            ,
           :tlBcCardnoListInf.relation_crtday1     ,
           :tlBcCardnoListInf.relation_lastday1    ,
           :tlBcCardnoListInf.relation2            ,
           :tlBcCardnoListInf.relation_crtday2     ,
           :tlBcCardnoListInf.relation_lastday2    ,
           :tlBcCardnoListInf.relation3            ,
           :tlBcCardnoListInf.relation_crtday3     ,
           :tlBcCardnoListInf.relation_lastday3    ,
           :tlBcCardnoListInf.relation4            ,
           :tlBcCardnoListInf.relation_crtday4     ,
           :tlBcCardnoListInf.relation_lastday4    ,
           :tlBcCardnoListInf.relation5            ,
           :tlBcCardnoListInf.relation_crtday5     ,
           :tlBcCardnoListInf.relation_lastday5
        FROM tl_bc_cardno_list_inf
        WHERE cardno=:cardno
          AND rownum<=1
          FOR update; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 17;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select cardno ,relation1 ,relation_crtday1 ,relation\
_lastday1 ,relation2 ,relation_crtday2 ,relation_lastday2 ,relation3 ,relatio\
n_crtday3 ,relation_lastday3 ,relation4 ,relation_crtday4 ,relation_lastday4 \
,relation5 ,relation_crtday5 ,relation_lastday5 into :b0,:b1,:b2,:b3,:b4,:b5,\
:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15  from tl_bc_cardno_list_inf whe\
re (cardno=:b16 and rownum<=1) for update ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )24;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcCardnoListInf.cardno);
        sqlstm.sqhstl[0] = (unsigned int  )20;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcCardnoListInf.relation1);
        sqlstm.sqhstl[1] = (unsigned int  )101;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcCardnoListInf.relation_crtday1);
        sqlstm.sqhstl[2] = (unsigned int  )9;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcCardnoListInf.relation_lastday1);
        sqlstm.sqhstl[3] = (unsigned int  )9;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcCardnoListInf.relation2);
        sqlstm.sqhstl[4] = (unsigned int  )101;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcCardnoListInf.relation_crtday2);
        sqlstm.sqhstl[5] = (unsigned int  )9;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcCardnoListInf.relation_lastday2);
        sqlstm.sqhstl[6] = (unsigned int  )9;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcCardnoListInf.relation3);
        sqlstm.sqhstl[7] = (unsigned int  )101;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcCardnoListInf.relation_crtday3);
        sqlstm.sqhstl[8] = (unsigned int  )9;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcCardnoListInf.relation_lastday3);
        sqlstm.sqhstl[9] = (unsigned int  )9;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcCardnoListInf.relation4);
        sqlstm.sqhstl[10] = (unsigned int  )101;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcCardnoListInf.relation_crtday4);
        sqlstm.sqhstl[11] = (unsigned int  )9;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcCardnoListInf.relation_lastday4);
        sqlstm.sqhstl[12] = (unsigned int  )9;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcCardnoListInf.relation5);
        sqlstm.sqhstl[13] = (unsigned int  )101;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcCardnoListInf.relation_crtday5);
        sqlstm.sqhstl[14] = (unsigned int  )9;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcCardnoListInf.relation_lastday5);
        sqlstm.sqhstl[15] = (unsigned int  )9;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)cardno;
        sqlstm.sqhstl[16] = (unsigned int  )20;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
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
        	ADJUST_STRING(tlBcCardnoListInf.relation1         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday1        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday1           )
        	ADJUST_STRING(tlBcCardnoListInf.relation2         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday2        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday2           )
        	ADJUST_STRING(tlBcCardnoListInf.relation3         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday3        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday3           )
        	ADJUST_STRING(tlBcCardnoListInf.relation4         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday4        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday4           )
        	ADJUST_STRING(tlBcCardnoListInf.relation5         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday5        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday5           )

        	ADJUST_STRING(p_tlBcCardnoListInf->relation1         )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_crtday1        )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_lastday1           )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation2         )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_crtday2        )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_lastday2           )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation3         )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_crtday3        )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_lastday3           )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation4         )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_crtday4        )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_lastday4           )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation5         )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_crtday5        )
        	ADJUST_STRING(p_tlBcCardnoListInf->relation_lastday5           )
		if (!strcmp(tlBcCardnoListInf.relation1, p_tlBcCardnoListInf->relation1)) 
		{
			tlBcCardnoListInf.index='1';
			strcpy(tlBcCardnoListInf.relation_lastday1, sysdate);
		}
		else if (!strcmp(tlBcCardnoListInf.relation2, p_tlBcCardnoListInf->relation1))
		{
			tlBcCardnoListInf.index='2';
			strcpy(tlBcCardnoListInf.relation_lastday2, sysdate);
		}
		else if (!strcmp(tlBcCardnoListInf.relation3, p_tlBcCardnoListInf->relation1))
		{
			tlBcCardnoListInf.index='3';
			strcpy(tlBcCardnoListInf.relation_lastday3, sysdate);
		}
		else if (!strcmp(tlBcCardnoListInf.relation4, p_tlBcCardnoListInf->relation1))
		{
			tlBcCardnoListInf.index='4';
			strcpy(tlBcCardnoListInf.relation_lastday4, sysdate);
		}
		else if (!strcmp(tlBcCardnoListInf.relation5, p_tlBcCardnoListInf->relation1))
		{
			tlBcCardnoListInf.index='5';
			strcpy(tlBcCardnoListInf.relation_lastday5, sysdate);
		}
		else
		{
			if (strlen(rtrim(tlBcCardnoListInf.relation2))==0) 
			{
				tlBcCardnoListInf.index='2';
				strcpy(tlBcCardnoListInf.relation2, p_tlBcCardnoListInf->relation1);
				strcpy(tlBcCardnoListInf.relation_crtday2, sysdate);
				strcpy(tlBcCardnoListInf.relation_lastday2, sysdate);
			}
			else if (strlen(rtrim(tlBcCardnoListInf.relation3))==0) 
			{
				tlBcCardnoListInf.index='3';
				strcpy(tlBcCardnoListInf.relation3, p_tlBcCardnoListInf->relation1);
				strcpy(tlBcCardnoListInf.relation_crtday3, sysdate);
				strcpy(tlBcCardnoListInf.relation_lastday3, sysdate);
			}
			else if (strlen(rtrim(tlBcCardnoListInf.relation4))==0) 
			{
				tlBcCardnoListInf.index='4';
				strcpy(tlBcCardnoListInf.relation4, p_tlBcCardnoListInf->relation1);
				strcpy(tlBcCardnoListInf.relation_crtday4, sysdate);
				strcpy(tlBcCardnoListInf.relation_lastday4, sysdate);
			}
			else if (strlen(rtrim(tlBcCardnoListInf.relation5))==0)
			{
				tlBcCardnoListInf.index='5';
				strcpy(tlBcCardnoListInf.relation5, p_tlBcCardnoListInf->relation1);
				strcpy(tlBcCardnoListInf.relation_crtday5, sysdate);
				strcpy(tlBcCardnoListInf.relation_lastday5, sysdate);
			}
			else
			{
		  		memcpy((char *)p_tlBcCardnoListInf, (char *)&tlBcCardnoListInf, sizeof(tl_bc_cardno_list_inf_def));
				p_tlBcCardnoListInf->index='6';
        			/* EXEC SQL COMMIT WORK ; */ 

{
           struct sqlexd sqlstm;
           sqlorat((void **)0, &sqlctx, &oraca);
           sqlstm.sqlvsn = 12;
           sqlstm.arrsiz = 17;
           sqlstm.sqladtp = &sqladt;
           sqlstm.sqltdsp = &sqltds;
           sqlstm.iters = (unsigned int  )1;
           sqlstm.offset = (unsigned int  )107;
           sqlstm.cud = sqlcud0;
           sqlstm.sqlest = (unsigned char  *)&sqlca;
           sqlstm.sqlety = (unsigned short)4352;
           sqlstm.occurs = (unsigned int  )0;
           sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


				break;
			}
		}

        	ADJUST_STRING(tlBcCardnoListInf.cardno         )
        	ADJUST_STRING(tlBcCardnoListInf.relation1         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday1        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday1           )
        	ADJUST_STRING(tlBcCardnoListInf.relation2         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday2        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday2           )
        	ADJUST_STRING(tlBcCardnoListInf.relation3         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday3        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday3           )
        	ADJUST_STRING(tlBcCardnoListInf.relation4         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday4        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday4           )
        	ADJUST_STRING(tlBcCardnoListInf.relation5         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday5        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday5           )

        	/* EXEC SQL UPDATE tl_bc_cardno_list_inf SET
        		relation1         = :tlBcCardnoListInf.relation1         ,
        		relation_crtday1  = :tlBcCardnoListInf.relation_crtday1  ,
        		relation_lastday1 = :tlBcCardnoListInf.relation_lastday1 ,
        		relation2         = :tlBcCardnoListInf.relation2         ,
        		relation_crtday2  = :tlBcCardnoListInf.relation_crtday2  ,
        		relation_lastday2 = :tlBcCardnoListInf.relation_lastday2 ,
        		relation3         = :tlBcCardnoListInf.relation3         ,
        		relation_crtday3  = :tlBcCardnoListInf.relation_crtday3  ,
        		relation_lastday3 = :tlBcCardnoListInf.relation_lastday3 ,
        		relation4         = :tlBcCardnoListInf.relation4         ,
        		relation_crtday4  = :tlBcCardnoListInf.relation_crtday4  ,
        		relation_lastday4 = :tlBcCardnoListInf.relation_lastday4 ,
        		relation5         = :tlBcCardnoListInf.relation5         ,
        		relation_crtday5  = :tlBcCardnoListInf.relation_crtday5  ,
        		relation_lastday5 = :tlBcCardnoListInf.relation_lastday5
        	  WHERE cardno=:cardno
          	    AND rownum<=1; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 17;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "update tl_bc_cardno_list_inf  set relation1=:b0,rel\
ation_crtday1=:b1,relation_lastday1=:b2,relation2=:b3,relation_crtday2=:b4,re\
lation_lastday2=:b5,relation3=:b6,relation_crtday3=:b7,relation_lastday3=:b8,\
relation4=:b9,relation_crtday4=:b10,relation_lastday4=:b11,relation5=:b12,rel\
ation_crtday5=:b13,relation_lastday5=:b14 where (cardno=:b15 and rownum<=1)";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )122;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)(tlBcCardnoListInf.relation1);
         sqlstm.sqhstl[0] = (unsigned int  )101;
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)(tlBcCardnoListInf.relation_crtday1);
         sqlstm.sqhstl[1] = (unsigned int  )9;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)(tlBcCardnoListInf.relation_lastday1);
         sqlstm.sqhstl[2] = (unsigned int  )9;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         void  *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned int  )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (         void  *)(tlBcCardnoListInf.relation2);
         sqlstm.sqhstl[3] = (unsigned int  )101;
         sqlstm.sqhsts[3] = (         int  )0;
         sqlstm.sqindv[3] = (         void  *)0;
         sqlstm.sqinds[3] = (         int  )0;
         sqlstm.sqharm[3] = (unsigned int  )0;
         sqlstm.sqadto[3] = (unsigned short )0;
         sqlstm.sqtdso[3] = (unsigned short )0;
         sqlstm.sqhstv[4] = (         void  *)(tlBcCardnoListInf.relation_crtday2);
         sqlstm.sqhstl[4] = (unsigned int  )9;
         sqlstm.sqhsts[4] = (         int  )0;
         sqlstm.sqindv[4] = (         void  *)0;
         sqlstm.sqinds[4] = (         int  )0;
         sqlstm.sqharm[4] = (unsigned int  )0;
         sqlstm.sqadto[4] = (unsigned short )0;
         sqlstm.sqtdso[4] = (unsigned short )0;
         sqlstm.sqhstv[5] = (         void  *)(tlBcCardnoListInf.relation_lastday2);
         sqlstm.sqhstl[5] = (unsigned int  )9;
         sqlstm.sqhsts[5] = (         int  )0;
         sqlstm.sqindv[5] = (         void  *)0;
         sqlstm.sqinds[5] = (         int  )0;
         sqlstm.sqharm[5] = (unsigned int  )0;
         sqlstm.sqadto[5] = (unsigned short )0;
         sqlstm.sqtdso[5] = (unsigned short )0;
         sqlstm.sqhstv[6] = (         void  *)(tlBcCardnoListInf.relation3);
         sqlstm.sqhstl[6] = (unsigned int  )101;
         sqlstm.sqhsts[6] = (         int  )0;
         sqlstm.sqindv[6] = (         void  *)0;
         sqlstm.sqinds[6] = (         int  )0;
         sqlstm.sqharm[6] = (unsigned int  )0;
         sqlstm.sqadto[6] = (unsigned short )0;
         sqlstm.sqtdso[6] = (unsigned short )0;
         sqlstm.sqhstv[7] = (         void  *)(tlBcCardnoListInf.relation_crtday3);
         sqlstm.sqhstl[7] = (unsigned int  )9;
         sqlstm.sqhsts[7] = (         int  )0;
         sqlstm.sqindv[7] = (         void  *)0;
         sqlstm.sqinds[7] = (         int  )0;
         sqlstm.sqharm[7] = (unsigned int  )0;
         sqlstm.sqadto[7] = (unsigned short )0;
         sqlstm.sqtdso[7] = (unsigned short )0;
         sqlstm.sqhstv[8] = (         void  *)(tlBcCardnoListInf.relation_lastday3);
         sqlstm.sqhstl[8] = (unsigned int  )9;
         sqlstm.sqhsts[8] = (         int  )0;
         sqlstm.sqindv[8] = (         void  *)0;
         sqlstm.sqinds[8] = (         int  )0;
         sqlstm.sqharm[8] = (unsigned int  )0;
         sqlstm.sqadto[8] = (unsigned short )0;
         sqlstm.sqtdso[8] = (unsigned short )0;
         sqlstm.sqhstv[9] = (         void  *)(tlBcCardnoListInf.relation4);
         sqlstm.sqhstl[9] = (unsigned int  )101;
         sqlstm.sqhsts[9] = (         int  )0;
         sqlstm.sqindv[9] = (         void  *)0;
         sqlstm.sqinds[9] = (         int  )0;
         sqlstm.sqharm[9] = (unsigned int  )0;
         sqlstm.sqadto[9] = (unsigned short )0;
         sqlstm.sqtdso[9] = (unsigned short )0;
         sqlstm.sqhstv[10] = (         void  *)(tlBcCardnoListInf.relation_crtday4);
         sqlstm.sqhstl[10] = (unsigned int  )9;
         sqlstm.sqhsts[10] = (         int  )0;
         sqlstm.sqindv[10] = (         void  *)0;
         sqlstm.sqinds[10] = (         int  )0;
         sqlstm.sqharm[10] = (unsigned int  )0;
         sqlstm.sqadto[10] = (unsigned short )0;
         sqlstm.sqtdso[10] = (unsigned short )0;
         sqlstm.sqhstv[11] = (         void  *)(tlBcCardnoListInf.relation_lastday4);
         sqlstm.sqhstl[11] = (unsigned int  )9;
         sqlstm.sqhsts[11] = (         int  )0;
         sqlstm.sqindv[11] = (         void  *)0;
         sqlstm.sqinds[11] = (         int  )0;
         sqlstm.sqharm[11] = (unsigned int  )0;
         sqlstm.sqadto[11] = (unsigned short )0;
         sqlstm.sqtdso[11] = (unsigned short )0;
         sqlstm.sqhstv[12] = (         void  *)(tlBcCardnoListInf.relation5);
         sqlstm.sqhstl[12] = (unsigned int  )101;
         sqlstm.sqhsts[12] = (         int  )0;
         sqlstm.sqindv[12] = (         void  *)0;
         sqlstm.sqinds[12] = (         int  )0;
         sqlstm.sqharm[12] = (unsigned int  )0;
         sqlstm.sqadto[12] = (unsigned short )0;
         sqlstm.sqtdso[12] = (unsigned short )0;
         sqlstm.sqhstv[13] = (         void  *)(tlBcCardnoListInf.relation_crtday5);
         sqlstm.sqhstl[13] = (unsigned int  )9;
         sqlstm.sqhsts[13] = (         int  )0;
         sqlstm.sqindv[13] = (         void  *)0;
         sqlstm.sqinds[13] = (         int  )0;
         sqlstm.sqharm[13] = (unsigned int  )0;
         sqlstm.sqadto[13] = (unsigned short )0;
         sqlstm.sqtdso[13] = (unsigned short )0;
         sqlstm.sqhstv[14] = (         void  *)(tlBcCardnoListInf.relation_lastday5);
         sqlstm.sqhstl[14] = (unsigned int  )9;
         sqlstm.sqhsts[14] = (         int  )0;
         sqlstm.sqindv[14] = (         void  *)0;
         sqlstm.sqinds[14] = (         int  )0;
         sqlstm.sqharm[14] = (unsigned int  )0;
         sqlstm.sqadto[14] = (unsigned short )0;
         sqlstm.sqtdso[14] = (unsigned short )0;
         sqlstm.sqhstv[15] = (         void  *)cardno;
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



		  memcpy((char *)p_tlBcCardnoListInf, (char *)&tlBcCardnoListInf, sizeof(tl_bc_cardno_list_inf_def));
        	  *p_sqlCode = sqlca.sqlcode;
        	  if ( sqlca.sqlcode == 0 )
        	  {
        	  } else if ( sqlca.sqlcode == 1403 )
        	  {
            	  	*p_sqlCode = SQL_ERD_NONE_RECORD;
            	  	glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcCardnoListInfOpr:UPDATE1>Search Record (cardno:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", cardno,sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        	  } else
        	  {
            	  	glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcCardnoListInfOpr:UPDATE1>Search Record (cardno:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", cardno,sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        	  }
        	  /* EXEC SQL COMMIT WORK ; */ 

{
           struct sqlexd sqlstm;
           sqlorat((void **)0, &sqlctx, &oraca);
           sqlstm.sqlvsn = 12;
           sqlstm.arrsiz = 17;
           sqlstm.sqladtp = &sqladt;
           sqlstm.sqltdsp = &sqltds;
           sqlstm.iters = (unsigned int  )1;
           sqlstm.offset = (unsigned int  )201;
           sqlstm.cud = sqlcud0;
           sqlstm.sqlest = (unsigned char  *)&sqlca;
           sqlstm.sqlety = (unsigned short)4352;
           sqlstm.occurs = (unsigned int  )0;
           sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        	  break;
        } else if ( sqlca.sqlcode == 1403 )
        {
        	/*copy buff*/
        	memcpy(&tlBcCardnoListInf, p_tlBcCardnoListInf, sizeof(tl_bc_cardno_list_inf_def));

        	/* add other field*/
        	ADJUST_STRING(tlBcCardnoListInf.cardno         )
		strcpy(tlBcCardnoListInf.relation_crtday1, sysdate);
		strcpy(tlBcCardnoListInf.relation_lastday1, sysdate);
        	ADJUST_STRING(tlBcCardnoListInf.relation1         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday1        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday1           )
        	ADJUST_STRING(tlBcCardnoListInf.relation2         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday2        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday2           )
        	ADJUST_STRING(tlBcCardnoListInf.relation3         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday3        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday3           )
        	ADJUST_STRING(tlBcCardnoListInf.relation4         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday4        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday4           )
        	ADJUST_STRING(tlBcCardnoListInf.relation5         )
        	ADJUST_STRING(tlBcCardnoListInf.relation_crtday5        )
        	ADJUST_STRING(tlBcCardnoListInf.relation_lastday5           )

        	/* EXEC SQL insert into tl_bc_cardno_list_inf(cardno,relation1,relation_crtday1,relation_lastday1,relation2,relation_crtday2,relation_lastday2,relation3,relation_crtday3,relation_lastday3,relation4,relation_crtday4,relation_lastday4,relation5,relation_crtday5,relation_lastday5) values
        	(
        	:tlBcCardnoListInf.cardno            ,
        	:tlBcCardnoListInf.relation1         ,
        	:tlBcCardnoListInf.relation_crtday1  ,
        	:tlBcCardnoListInf.relation_lastday1 ,
        	:tlBcCardnoListInf.relation2         ,
        	:tlBcCardnoListInf.relation_crtday2  ,
        	:tlBcCardnoListInf.relation_lastday2 ,
        	:tlBcCardnoListInf.relation3         ,
        	:tlBcCardnoListInf.relation_crtday3  ,
        	:tlBcCardnoListInf.relation_lastday3 ,
        	:tlBcCardnoListInf.relation4         ,
        	:tlBcCardnoListInf.relation_crtday4  ,
        	:tlBcCardnoListInf.relation_lastday4 ,
        	:tlBcCardnoListInf.relation5         ,
        	:tlBcCardnoListInf.relation_crtday5  ,
        	:tlBcCardnoListInf.relation_lastday5
        	)
        	; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 17;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "insert into tl_bc_cardno_list_inf (cardno,relation1\
,relation_crtday1,relation_lastday1,relation2,relation_crtday2,relation_lastd\
ay2,relation3,relation_crtday3,relation_lastday3,relation4,relation_crtday4,r\
elation_lastday4,relation5,relation_crtday5,relation_lastday5) values (:b0,:b\
1,:b2,:b3,:b4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15)";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )216;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)(tlBcCardnoListInf.cardno);
         sqlstm.sqhstl[0] = (unsigned int  )20;
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)(tlBcCardnoListInf.relation1);
         sqlstm.sqhstl[1] = (unsigned int  )101;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)(tlBcCardnoListInf.relation_crtday1);
         sqlstm.sqhstl[2] = (unsigned int  )9;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         void  *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned int  )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (         void  *)(tlBcCardnoListInf.relation_lastday1);
         sqlstm.sqhstl[3] = (unsigned int  )9;
         sqlstm.sqhsts[3] = (         int  )0;
         sqlstm.sqindv[3] = (         void  *)0;
         sqlstm.sqinds[3] = (         int  )0;
         sqlstm.sqharm[3] = (unsigned int  )0;
         sqlstm.sqadto[3] = (unsigned short )0;
         sqlstm.sqtdso[3] = (unsigned short )0;
         sqlstm.sqhstv[4] = (         void  *)(tlBcCardnoListInf.relation2);
         sqlstm.sqhstl[4] = (unsigned int  )101;
         sqlstm.sqhsts[4] = (         int  )0;
         sqlstm.sqindv[4] = (         void  *)0;
         sqlstm.sqinds[4] = (         int  )0;
         sqlstm.sqharm[4] = (unsigned int  )0;
         sqlstm.sqadto[4] = (unsigned short )0;
         sqlstm.sqtdso[4] = (unsigned short )0;
         sqlstm.sqhstv[5] = (         void  *)(tlBcCardnoListInf.relation_crtday2);
         sqlstm.sqhstl[5] = (unsigned int  )9;
         sqlstm.sqhsts[5] = (         int  )0;
         sqlstm.sqindv[5] = (         void  *)0;
         sqlstm.sqinds[5] = (         int  )0;
         sqlstm.sqharm[5] = (unsigned int  )0;
         sqlstm.sqadto[5] = (unsigned short )0;
         sqlstm.sqtdso[5] = (unsigned short )0;
         sqlstm.sqhstv[6] = (         void  *)(tlBcCardnoListInf.relation_lastday2);
         sqlstm.sqhstl[6] = (unsigned int  )9;
         sqlstm.sqhsts[6] = (         int  )0;
         sqlstm.sqindv[6] = (         void  *)0;
         sqlstm.sqinds[6] = (         int  )0;
         sqlstm.sqharm[6] = (unsigned int  )0;
         sqlstm.sqadto[6] = (unsigned short )0;
         sqlstm.sqtdso[6] = (unsigned short )0;
         sqlstm.sqhstv[7] = (         void  *)(tlBcCardnoListInf.relation3);
         sqlstm.sqhstl[7] = (unsigned int  )101;
         sqlstm.sqhsts[7] = (         int  )0;
         sqlstm.sqindv[7] = (         void  *)0;
         sqlstm.sqinds[7] = (         int  )0;
         sqlstm.sqharm[7] = (unsigned int  )0;
         sqlstm.sqadto[7] = (unsigned short )0;
         sqlstm.sqtdso[7] = (unsigned short )0;
         sqlstm.sqhstv[8] = (         void  *)(tlBcCardnoListInf.relation_crtday3);
         sqlstm.sqhstl[8] = (unsigned int  )9;
         sqlstm.sqhsts[8] = (         int  )0;
         sqlstm.sqindv[8] = (         void  *)0;
         sqlstm.sqinds[8] = (         int  )0;
         sqlstm.sqharm[8] = (unsigned int  )0;
         sqlstm.sqadto[8] = (unsigned short )0;
         sqlstm.sqtdso[8] = (unsigned short )0;
         sqlstm.sqhstv[9] = (         void  *)(tlBcCardnoListInf.relation_lastday3);
         sqlstm.sqhstl[9] = (unsigned int  )9;
         sqlstm.sqhsts[9] = (         int  )0;
         sqlstm.sqindv[9] = (         void  *)0;
         sqlstm.sqinds[9] = (         int  )0;
         sqlstm.sqharm[9] = (unsigned int  )0;
         sqlstm.sqadto[9] = (unsigned short )0;
         sqlstm.sqtdso[9] = (unsigned short )0;
         sqlstm.sqhstv[10] = (         void  *)(tlBcCardnoListInf.relation4);
         sqlstm.sqhstl[10] = (unsigned int  )101;
         sqlstm.sqhsts[10] = (         int  )0;
         sqlstm.sqindv[10] = (         void  *)0;
         sqlstm.sqinds[10] = (         int  )0;
         sqlstm.sqharm[10] = (unsigned int  )0;
         sqlstm.sqadto[10] = (unsigned short )0;
         sqlstm.sqtdso[10] = (unsigned short )0;
         sqlstm.sqhstv[11] = (         void  *)(tlBcCardnoListInf.relation_crtday4);
         sqlstm.sqhstl[11] = (unsigned int  )9;
         sqlstm.sqhsts[11] = (         int  )0;
         sqlstm.sqindv[11] = (         void  *)0;
         sqlstm.sqinds[11] = (         int  )0;
         sqlstm.sqharm[11] = (unsigned int  )0;
         sqlstm.sqadto[11] = (unsigned short )0;
         sqlstm.sqtdso[11] = (unsigned short )0;
         sqlstm.sqhstv[12] = (         void  *)(tlBcCardnoListInf.relation_lastday4);
         sqlstm.sqhstl[12] = (unsigned int  )9;
         sqlstm.sqhsts[12] = (         int  )0;
         sqlstm.sqindv[12] = (         void  *)0;
         sqlstm.sqinds[12] = (         int  )0;
         sqlstm.sqharm[12] = (unsigned int  )0;
         sqlstm.sqadto[12] = (unsigned short )0;
         sqlstm.sqtdso[12] = (unsigned short )0;
         sqlstm.sqhstv[13] = (         void  *)(tlBcCardnoListInf.relation5);
         sqlstm.sqhstl[13] = (unsigned int  )101;
         sqlstm.sqhsts[13] = (         int  )0;
         sqlstm.sqindv[13] = (         void  *)0;
         sqlstm.sqinds[13] = (         int  )0;
         sqlstm.sqharm[13] = (unsigned int  )0;
         sqlstm.sqadto[13] = (unsigned short )0;
         sqlstm.sqtdso[13] = (unsigned short )0;
         sqlstm.sqhstv[14] = (         void  *)(tlBcCardnoListInf.relation_crtday5);
         sqlstm.sqhstl[14] = (unsigned int  )9;
         sqlstm.sqhsts[14] = (         int  )0;
         sqlstm.sqindv[14] = (         void  *)0;
         sqlstm.sqinds[14] = (         int  )0;
         sqlstm.sqharm[14] = (unsigned int  )0;
         sqlstm.sqadto[14] = (unsigned short )0;
         sqlstm.sqtdso[14] = (unsigned short )0;
         sqlstm.sqhstv[15] = (         void  *)(tlBcCardnoListInf.relation_lastday5);
         sqlstm.sqhstl[15] = (unsigned int  )9;
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



		memcpy((char *)p_tlBcCardnoListInf, (char *)&tlBcCardnoListInf, sizeof(tl_bc_cardno_list_inf_def));
		p_tlBcCardnoListInf->index='1';
        	/* insert result */
        	*p_sqlCode = sqlca.sqlcode;
        	if ( sqlca.sqlcode == 0 )
        	{
        	} else if ( memcmp(sqlca.sqlerrm.sqlerrmc, "ORA-00001", 9 ) == 0 )
        	{
            		*p_sqlCode = SQL_ERD_UNIKEY;
            		glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcCardnoListInfOpr:INSERT1>Insert Record is exist.sqlcode:%d,sqlca.sqlerrm.sqlerrmc:%s",sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
        	} else
        	{
            		glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcCardnoListInfOpr:INSERT1>Insert Record Failure.sqlcode:%d,sqlca.sqlerrm.sqlerrmc:%s",sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
        	}
        	/* EXEC SQL COMMIT WORK ; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 17;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )295;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        	break;
        } else
        {
            	glb_Log(0,0, "<FILE:%s,LINE:%d><TL_BC_TRANS_LOG.pc:tlBcCardnoListInfOpr:SELECT1>Search Record (cardno:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",__FILE__,__LINE__, cardno, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 17;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )310;
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

