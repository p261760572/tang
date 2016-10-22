
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
    "ti_bc_trans_inf.pc"
};


static unsigned int sqlctx = 21256971;


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
5,0,0,1,743,0,4,91,0,0,17,5,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,
0,1,97,0,0,1,97,0,0,1,97,0,0,
88,0,0,2,0,0,29,145,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_trans_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

ti_bc_trans_inf_def tiBcTransInf ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tiBcTransInfOpr
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

int tiBcTransInfOpr(int oprType, ti_bc_trans_inf_def *p_tiBcTransInf, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char  trans_chnl      [30+ 1] ; 
    char  msg_tp          [4 + 1] ;  
    char  proc_cd         [6 + 1] ;  
    char  pos_cond_cd     [2 + 1] ;  
    char  mchnt_tp        [4 + 1] ;  
    /* EXEC SQL END DECLARE SECTION; */ 


    char caTxnMsg[4096];

    memset(trans_chnl, 0, sizeof(trans_chnl));
    memset(msg_tp, 0, sizeof(msg_tp));
    memset(proc_cd, 0, sizeof(proc_cd));
    memset(pos_cond_cd, 0, sizeof(pos_cond_cd));
    memset(mchnt_tp, 0, sizeof(mchnt_tp));

    switch ( oprType )
    {
    
    case GLB_DB_SELECT1:

        memcpy(trans_chnl, p_tiBcTransInf->trans_chnl,30);
        trans_chnl[30]=0;
        rtrim(trans_chnl);
        if ( *trans_chnl==0 )
            strcpy(trans_chnl,"*");

        memcpy(msg_tp, p_tiBcTransInf->msg_tp,4);
        msg_tp[4]=0;
        rtrim(msg_tp);

        memcpy(proc_cd, p_tiBcTransInf->proc_cd,6);
        proc_cd[6]=0;
        rtrim(proc_cd);
        if ( *proc_cd==0 )
            strcpy(proc_cd,"*");

        memcpy(pos_cond_cd, p_tiBcTransInf->pos_cond_cd,2);
        pos_cond_cd[2]=0;
        rtrim(pos_cond_cd);
        if ( *pos_cond_cd==0 )
            strcpy(pos_cond_cd,"*");

        memcpy(mchnt_tp, p_tiBcTransInf->mchnt_tp,4);
        mchnt_tp[4]=0;
        rtrim(mchnt_tp);
        if ( *mchnt_tp==0 )
            strcpy(mchnt_tp,"*");

        /* EXEC SQL SELECT 
        trans_chnl      ,
        msg_tp          ,
        proc_cd         ,
        pos_cond_cd     ,
        mchnt_tp        ,
        respFlag        ,
        DbFlag          ,
        CardBinDetFlag  ,
        Mid_chk_flg     ,
        white_black_flag,
        opFlag          ,
        TransType       
        INTO :tiBcTransInf 
        FROM (SELECT 
              trans_chnl      ,
              msg_tp          ,
              proc_cd         ,
              pos_cond_cd     ,
              mchnt_tp        ,
              respFlag        ,
              DbFlag          ,
              CardBinDetFlag  ,
              Mid_chk_flg     ,
              white_black_flag,
              opFlag          ,
              TransType       
              FROM ti_bc_trans_inf
              WHERE ( rtrim(trans_chnl)=:trans_chnl OR rtrim(trans_chnl)='*' )
              AND ( rtrim(msg_tp)=:msg_tp OR rtrim(msg_tp)='*' )
              AND ( rtrim(proc_cd)=:proc_cd OR rtrim(proc_cd)='*' )
              AND ( rtrim(pos_cond_cd)=:pos_cond_cd OR rtrim(pos_cond_cd)='*' )
              AND ( rtrim(mchnt_tp)=:mchnt_tp OR rtrim(mchnt_tp)='*' )
              ORDER BY  trans_chnl desc, msg_tp desc,proc_cd desc,pos_cond_cd desc,mchnt_tp desc
             )
        WHERE rownum<=1
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 17;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select trans_chnl ,msg_tp ,proc_cd ,pos_cond_cd ,mch\
nt_tp ,respFlag ,DbFlag ,CardBinDetFlag ,Mid_chk_flg ,white_black_flag ,opFla\
g ,TransType into :s1 ,:s2 ,:s3 ,:s4 ,:s5 ,:s6 ,:s7 ,:s8 ,:s9 ,:s10 ,:s11 ,:s\
12   from (select trans_chnl ,msg_tp ,proc_cd ,pos_cond_cd ,mchnt_tp ,respFla\
g ,DbFlag ,CardBinDetFlag ,Mid_chk_flg ,white_black_flag ,opFlag ,TransType  \
from ti_bc_trans_inf where (((((rtrim(trans_chnl)=:b1 or rtrim(trans_chnl)='*\
') and (rtrim(msg_tp)=:b2 or rtrim(msg_tp)='*')) and (rtrim(proc_cd)=:b3 or r\
trim(proc_cd)='*')) and (rtrim(pos_cond_cd)=:b4 or rtrim(pos_cond_cd)='*')) a\
nd (rtrim(mchnt_tp)=:b5 or rtrim(mchnt_tp)='*')) order by trans_chnl desc ,ms\
g_tp desc ,proc_cd desc ,pos_cond_cd desc ,mchnt_tp desc  ) where rownum<=1";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)tiBcTransInf.trans_chnl;
        sqlstm.sqhstl[0] = (unsigned int  )31;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tiBcTransInf.msg_tp;
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)tiBcTransInf.proc_cd;
        sqlstm.sqhstl[2] = (unsigned int  )7;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)tiBcTransInf.pos_cond_cd;
        sqlstm.sqhstl[3] = (unsigned int  )3;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)tiBcTransInf.mchnt_tp;
        sqlstm.sqhstl[4] = (unsigned int  )5;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)tiBcTransInf.respFlag;
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)tiBcTransInf.DbFlag;
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)tiBcTransInf.CardBinDetFlag;
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)tiBcTransInf.Mid_chk_flg;
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)tiBcTransInf.white_black_flag;
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)tiBcTransInf.opFlag;
        sqlstm.sqhstl[10] = (unsigned int  )2;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)tiBcTransInf.TransType;
        sqlstm.sqhstl[11] = (unsigned int  )6;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)trans_chnl;
        sqlstm.sqhstl[12] = (unsigned int  )31;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)msg_tp;
        sqlstm.sqhstl[13] = (unsigned int  )5;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)proc_cd;
        sqlstm.sqhstl[14] = (unsigned int  )7;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)pos_cond_cd;
        sqlstm.sqhstl[15] = (unsigned int  )3;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)mchnt_tp;
        sqlstm.sqhstl[16] = (unsigned int  )5;
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
            memcpy(p_tiBcTransInf, &tiBcTransInf, sizeof(ti_bc_trans_inf_def));
            
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            sprintf(caTxnMsg, "<TI_BC_TRANS_INF.pc:tiBcTransInfOpr:SELECT1>Search Record (trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
        } else
        {
            sprintf(caTxnMsg, "<TI_BC_TRANS_INF.pc:tiBcTransInfOpr:SELECT1>Search Record (trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            glb_ErrLog( 1000, caTxnMsg, RPT_TO_LOG, 0, 0 );
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
        sqlstm.offset = (unsigned int  )88;
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

