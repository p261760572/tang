
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
           char  filnam[20];
};
static struct sqlcxp sqlfpn =
{
    19,
    "ti_bc_mt_flow_id.pc"
};


static unsigned int sqlctx = 42513771;


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

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,4274,850,0,0,
5,0,0,1,158,0,4,63,0,0,5,2,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,
40,0,0,2,94,0,5,82,0,0,3,3,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,
67,0,0,3,84,0,3,99,0,0,2,2,0,1,0,1,97,0,0,1,97,0,0,
90,0,0,4,0,0,29,113,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_mt_flow_id.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

ti_bc_mt_flow_id_def tiBcMTFlowId ;
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

int tiBcMTFlowIdOpr(int oprType, ti_bc_mt_flow_id_def *p_tiBcMTFlowId, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char term_id      [  8 + 1 ];
	  char mchnt_cd     [ 20 + 1 ];
    /* EXEC SQL END DECLARE SECTION; */ 


    MEMSET_0(term_id)
    MEMSET_0(mchnt_cd)

    int i;
    switch ( oprType )
    {
    
    case GLB_DB_SELECT1:
        strcpy_safe(term_id, p_tiBcMTFlowId->term_id, sizeof(term_id)-1);
        rtrim(term_id);
        if( 0 == *term_id){ strcpy(term_id,"00000000");}
        
        strcpy_safe(mchnt_cd, p_tiBcMTFlowId->mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim(mchnt_cd);

        /* EXEC SQL SELECT 
        term_id      ,
        mchnt_cd     ,
        sys_tra_no   
        INTO :tiBcMTFlowId
        FROM ti_bc_mt_flow_id
        WHERE rtrim(mchnt_cd)=:mchnt_cd AND rtrim(term_id)=:term_id AND rownum<=1
           FOR update
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 5;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select term_id ,mchnt_cd ,sys_tra_no into :s1 ,:s2 ,\
:s3   from ti_bc_mt_flow_id where ((rtrim(mchnt_cd)=:b1 and rtrim(term_id)=:b\
2) and rownum<=1) for update ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)tiBcMTFlowId.term_id;
        sqlstm.sqhstl[0] = (unsigned int  )9;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tiBcMTFlowId.mchnt_cd;
        sqlstm.sqhstl[1] = (unsigned int  )21;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)tiBcMTFlowId.sys_tra_no;
        sqlstm.sqhstl[2] = (unsigned int  )7;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[3] = (unsigned int  )21;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)term_id;
        sqlstm.sqhstl[4] = (unsigned int  )9;
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



        /* search result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
            memcpy(p_tiBcMTFlowId, &tiBcMTFlowId, sizeof(ti_bc_mt_flow_id_def));
            i = atoi(tiBcMTFlowId.sys_tra_no)%999999;
            sprintf(tiBcMTFlowId.sys_tra_no,"%.6d", ++i);
            strcpy(p_tiBcMTFlowId->sys_tra_no, tiBcMTFlowId.sys_tra_no );

             /* EXEC SQL UPDATE ti_bc_mt_flow_id 
             SET sys_tra_no=:tiBcMTFlowId.sys_tra_no
                 WHERE rtrim(mchnt_cd)=:mchnt_cd
                 AND rtrim(term_id)=:term_id
                ; */ 

{
             struct sqlexd sqlstm;
             sqlorat((void **)0, &sqlctx, &oraca);
             sqlstm.sqlvsn = 12;
             sqlstm.arrsiz = 5;
             sqlstm.sqladtp = &sqladt;
             sqlstm.sqltdsp = &sqltds;
             sqlstm.stmt = "update ti_bc_mt_flow_id  set sys_tra_no=:b0 whe\
re (rtrim(mchnt_cd)=:b1 and rtrim(term_id)=:b2)";
             sqlstm.iters = (unsigned int  )1;
             sqlstm.offset = (unsigned int  )40;
             sqlstm.cud = sqlcud0;
             sqlstm.sqlest = (unsigned char  *)&sqlca;
             sqlstm.sqlety = (unsigned short)4352;
             sqlstm.occurs = (unsigned int  )0;
             sqlstm.sqhstv[0] = (         void  *)(tiBcMTFlowId.sys_tra_no);
             sqlstm.sqhstl[0] = (unsigned int  )7;
             sqlstm.sqhsts[0] = (         int  )0;
             sqlstm.sqindv[0] = (         void  *)0;
             sqlstm.sqinds[0] = (         int  )0;
             sqlstm.sqharm[0] = (unsigned int  )0;
             sqlstm.sqadto[0] = (unsigned short )0;
             sqlstm.sqtdso[0] = (unsigned short )0;
             sqlstm.sqhstv[1] = (         void  *)mchnt_cd;
             sqlstm.sqhstl[1] = (unsigned int  )21;
             sqlstm.sqhsts[1] = (         int  )0;
             sqlstm.sqindv[1] = (         void  *)0;
             sqlstm.sqinds[1] = (         int  )0;
             sqlstm.sqharm[1] = (unsigned int  )0;
             sqlstm.sqadto[1] = (unsigned short )0;
             sqlstm.sqtdso[1] = (unsigned short )0;
             sqlstm.sqhstv[2] = (         void  *)term_id;
             sqlstm.sqhstl[2] = (unsigned int  )9;
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
            if ( 0 == sqlca.sqlcode )
            {
            }else
            {
                glb_Log(0,0,"<ti_bc_mt_flow_id.pc:tiBcMTFlowIdOpr:SELECT1>update Record sys_tra_no error.(mchnt_cd:%s,term_id:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", mchnt_cd,term_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );                
            }
            
        } else if ( sqlca.sqlcode == 1403 )
        {

            strcpy(p_tiBcMTFlowId->sys_tra_no,"000001");
            /* EXEC SQL INSERT INTO ti_bc_mt_flow_id(term_id,mchnt_cd,sys_tra_no)
             VALUES (:term_id,:mchnt_cd,'000001'); */ 

{
            struct sqlexd sqlstm;
            sqlorat((void **)0, &sqlctx, &oraca);
            sqlstm.sqlvsn = 12;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "insert into ti_bc_mt_flow_id (term_id,mchnt_cd,s\
ys_tra_no) values (:b0,:b1,'000001')";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )67;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)4352;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)term_id;
            sqlstm.sqhstl[0] = (unsigned int  )9;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)mchnt_cd;
            sqlstm.sqhstl[1] = (unsigned int  )21;
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
            if( 0 == sqlca.sqlcode )
            {
                //success
            }else{
                //exception
                glb_Log(0,0, "<ti_bc_mt_flow_id.pc:tiBcMTFlowIdOpr:SELECT1>Insert Record (mchnt_cd:%s,term_id:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", mchnt_cd,term_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );   
            }
        } else
        {
            glb_Log(0,0, "<ti_bc_mt_flow_id.pc:tiBcMTFlowIdOpr:SELECT1>Search Record (mchnt_cd:%s,term_id:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", mchnt_cd,term_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 5;
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


        break;

    default:
        *p_sqlCode = -1;
        return -1;
    }

    return 0;
}

