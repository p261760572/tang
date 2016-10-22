
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
    "ti_bc_mchnt_det.pc"
};


static unsigned int sqlctx = 21230795;


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
5,0,0,1,498,0,4,75,0,0,11,5,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
64,0,0,2,0,0,29,121,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_mchnt_det.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

  ti_bc_mchnt_det_def tiBcMchntDet ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tiBcMchntDetOpr
 *
 *
 *  Parameters:
 * 
 *      oprType - 操作类型， 输入， 取值
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode      - 返回值， 输出
 *      p_tiBcMchntDet - 查找条件，输入
 *                       查找结果，输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tiBcMchntDetOpr(int oprType, ti_bc_mchnt_det_def *p_tiBcMchntDet, int *p_sqlCode)
{
	/*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
	/* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/
	
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

       char  trans_chnl      [30+ 1] ; /* 接入渠道*/
	   char  msg_tp          [4 + 1] ;  
       char  proc_cd         [6 + 1] ;  
       char  pos_cond_cd [4 + 1] ;  
       char  mchnt_tp        [4 + 1] ;  
	/* EXEC SQL END DECLARE SECTION; */ 

  
  char caTxnMsg[4096];
	switch (oprType) {
		
		case GLB_DB_SELECT1:
			
            memcpy(trans_chnl, p_tiBcMchntDet->trans_chnl,30);
			trans_chnl[30]=0;
            rtrim(trans_chnl);

            memcpy(msg_tp, p_tiBcMchntDet->msg_tp,4);
			msg_tp[4]=0;
            rtrim(msg_tp);

            memcpy(proc_cd, p_tiBcMchntDet->proc_cd,6);
			proc_cd[6]=0;
            rtrim(proc_cd);

            memcpy(pos_cond_cd, p_tiBcMchntDet->pos_cond_cd,4);
			pos_cond_cd[4]=0;
            rtrim(pos_cond_cd);

            memcpy(mchnt_tp, p_tiBcMchntDet->mchnt_tp,4);
			mchnt_tp[4]=0;
            rtrim(mchnt_tp);

           /* EXEC SQL SELECT * INTO 
              :tiBcMchntDet.trans_chnl      ,
              :tiBcMchntDet.msg_tp          ,
              :tiBcMchntDet.proc_cd         ,
              :tiBcMchntDet.pos_cond_cd     ,
              :tiBcMchntDet.mchnt_tp        ,
              :tiBcMchntDet.mchnt_cd          
           	FROM 
           (  SELECT 
			        trans_chnl      ,
              msg_tp          ,
              proc_cd         ,
              pos_cond_cd     ,
              mchnt_tp        ,
              mchnt_cd        
             FROM ti_bc_trans_det
             WHERE ( rtrim(msg_tp)=:msg_tp OR rtrim(msg_tp)='*' )
               AND ( rtrim(proc_cd)=:proc_cd OR rtrim(proc_cd)='*' )
               AND ( rtrim(pos_cond_cd)=:pos_cond_cd OR rtrim(pos_cond_cd)='*' )
               AND ( rtrim(mchnt_tp)=:mchnt_tp OR rtrim(mchnt_tp)='*' )
               AND ( rtrim(trans_chnl)=:trans_chnl OR rtrim(trans_chnl)='*' )
             ORDER BY  
               trans_chnl  desc,
               msg_tp      desc,
               proc_cd     desc,
               pos_cond_cd desc,
               mchnt_tp    desc
         )
       WHERE rownum<=1
       ; */ 

{
           struct sqlexd sqlstm;
           sqlorat((void **)0, &sqlctx, &oraca);
           sqlstm.sqlvsn = 12;
           sqlstm.arrsiz = 11;
           sqlstm.sqladtp = &sqladt;
           sqlstm.sqltdsp = &sqltds;
           sqlstm.stmt = "select * into :b0,:b1,:b2,:b3,:b4,:b5  from (sele\
ct trans_chnl ,msg_tp ,proc_cd ,pos_cond_cd ,mchnt_tp ,mchnt_cd  from ti_bc_t\
rans_det where (((((rtrim(msg_tp)=:b6 or rtrim(msg_tp)='*') and (rtrim(proc_c\
d)=:b7 or rtrim(proc_cd)='*')) and (rtrim(pos_cond_cd)=:b8 or rtrim(pos_cond_\
cd)='*')) and (rtrim(mchnt_tp)=:b9 or rtrim(mchnt_tp)='*')) and (rtrim(trans_\
chnl)=:b10 or rtrim(trans_chnl)='*')) order by trans_chnl desc ,msg_tp desc ,\
proc_cd desc ,pos_cond_cd desc ,mchnt_tp desc  ) where rownum<=1";
           sqlstm.iters = (unsigned int  )1;
           sqlstm.offset = (unsigned int  )5;
           sqlstm.selerr = (unsigned short)1;
           sqlstm.cud = sqlcud0;
           sqlstm.sqlest = (unsigned char  *)&sqlca;
           sqlstm.sqlety = (unsigned short)4352;
           sqlstm.occurs = (unsigned int  )0;
           sqlstm.sqhstv[0] = (         void  *)(tiBcMchntDet.trans_chnl);
           sqlstm.sqhstl[0] = (unsigned int  )3;
           sqlstm.sqhsts[0] = (         int  )0;
           sqlstm.sqindv[0] = (         void  *)0;
           sqlstm.sqinds[0] = (         int  )0;
           sqlstm.sqharm[0] = (unsigned int  )0;
           sqlstm.sqadto[0] = (unsigned short )0;
           sqlstm.sqtdso[0] = (unsigned short )0;
           sqlstm.sqhstv[1] = (         void  *)(tiBcMchntDet.msg_tp);
           sqlstm.sqhstl[1] = (unsigned int  )5;
           sqlstm.sqhsts[1] = (         int  )0;
           sqlstm.sqindv[1] = (         void  *)0;
           sqlstm.sqinds[1] = (         int  )0;
           sqlstm.sqharm[1] = (unsigned int  )0;
           sqlstm.sqadto[1] = (unsigned short )0;
           sqlstm.sqtdso[1] = (unsigned short )0;
           sqlstm.sqhstv[2] = (         void  *)(tiBcMchntDet.proc_cd);
           sqlstm.sqhstl[2] = (unsigned int  )7;
           sqlstm.sqhsts[2] = (         int  )0;
           sqlstm.sqindv[2] = (         void  *)0;
           sqlstm.sqinds[2] = (         int  )0;
           sqlstm.sqharm[2] = (unsigned int  )0;
           sqlstm.sqadto[2] = (unsigned short )0;
           sqlstm.sqtdso[2] = (unsigned short )0;
           sqlstm.sqhstv[3] = (         void  *)(tiBcMchntDet.pos_cond_cd);
           sqlstm.sqhstl[3] = (unsigned int  )5;
           sqlstm.sqhsts[3] = (         int  )0;
           sqlstm.sqindv[3] = (         void  *)0;
           sqlstm.sqinds[3] = (         int  )0;
           sqlstm.sqharm[3] = (unsigned int  )0;
           sqlstm.sqadto[3] = (unsigned short )0;
           sqlstm.sqtdso[3] = (unsigned short )0;
           sqlstm.sqhstv[4] = (         void  *)(tiBcMchntDet.mchnt_tp);
           sqlstm.sqhstl[4] = (unsigned int  )5;
           sqlstm.sqhsts[4] = (         int  )0;
           sqlstm.sqindv[4] = (         void  *)0;
           sqlstm.sqinds[4] = (         int  )0;
           sqlstm.sqharm[4] = (unsigned int  )0;
           sqlstm.sqadto[4] = (unsigned short )0;
           sqlstm.sqtdso[4] = (unsigned short )0;
           sqlstm.sqhstv[5] = (         void  *)(tiBcMchntDet.mchnt_cd);
           sqlstm.sqhstl[5] = (unsigned int  )16;
           sqlstm.sqhsts[5] = (         int  )0;
           sqlstm.sqindv[5] = (         void  *)0;
           sqlstm.sqinds[5] = (         int  )0;
           sqlstm.sqharm[5] = (unsigned int  )0;
           sqlstm.sqadto[5] = (unsigned short )0;
           sqlstm.sqtdso[5] = (unsigned short )0;
           sqlstm.sqhstv[6] = (         void  *)msg_tp;
           sqlstm.sqhstl[6] = (unsigned int  )5;
           sqlstm.sqhsts[6] = (         int  )0;
           sqlstm.sqindv[6] = (         void  *)0;
           sqlstm.sqinds[6] = (         int  )0;
           sqlstm.sqharm[6] = (unsigned int  )0;
           sqlstm.sqadto[6] = (unsigned short )0;
           sqlstm.sqtdso[6] = (unsigned short )0;
           sqlstm.sqhstv[7] = (         void  *)proc_cd;
           sqlstm.sqhstl[7] = (unsigned int  )7;
           sqlstm.sqhsts[7] = (         int  )0;
           sqlstm.sqindv[7] = (         void  *)0;
           sqlstm.sqinds[7] = (         int  )0;
           sqlstm.sqharm[7] = (unsigned int  )0;
           sqlstm.sqadto[7] = (unsigned short )0;
           sqlstm.sqtdso[7] = (unsigned short )0;
           sqlstm.sqhstv[8] = (         void  *)pos_cond_cd;
           sqlstm.sqhstl[8] = (unsigned int  )5;
           sqlstm.sqhsts[8] = (         int  )0;
           sqlstm.sqindv[8] = (         void  *)0;
           sqlstm.sqinds[8] = (         int  )0;
           sqlstm.sqharm[8] = (unsigned int  )0;
           sqlstm.sqadto[8] = (unsigned short )0;
           sqlstm.sqtdso[8] = (unsigned short )0;
           sqlstm.sqhstv[9] = (         void  *)mchnt_tp;
           sqlstm.sqhstl[9] = (unsigned int  )5;
           sqlstm.sqhsts[9] = (         int  )0;
           sqlstm.sqindv[9] = (         void  *)0;
           sqlstm.sqinds[9] = (         int  )0;
           sqlstm.sqharm[9] = (unsigned int  )0;
           sqlstm.sqadto[9] = (unsigned short )0;
           sqlstm.sqtdso[9] = (unsigned short )0;
           sqlstm.sqhstv[10] = (         void  *)trans_chnl;
           sqlstm.sqhstl[10] = (unsigned int  )31;
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
     	  memcpy(p_tiBcMchntDet, &tiBcMchntDet, sizeof(ti_bc_mchnt_det_def));
     } 
     else if ( sqlca.sqlcode == 1403 )
     { 
        *p_sqlCode = SQL_ERD_NONE_RECORD;
      	  glb_Log(0,0,"<TI_BC_MCHNT_DET.pc.pc:tiBcTransDetOpr:SELECT1>Search Record (trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
     }
     else
     {
        glb_Log(0,0,"<TI_BC_MCHNT_DET.pc.pc:tiBcTransDetOpr:SELECT1>Search Record (trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", trans_chnl,msg_tp,proc_cd,pos_cond_cd,mchnt_tp, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
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

