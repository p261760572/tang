
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
    "ti_bc_error_inf.pc"
};


static unsigned int sqlctx = 21235083;


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
            void  *sqhstv[9];
   unsigned int   sqhstl[9];
            int   sqhsts[9];
            void  *sqindv[9];
            int   sqinds[9];
   unsigned int   sqharm[9];
   unsigned int   *sqharc[9];
   unsigned short  sqadto[9];
   unsigned short  sqtdso[9];
} sqlstm = {12,9};

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
5,0,0,1,208,0,4,61,0,0,9,3,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
56,0,0,2,0,0,29,99,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_error_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

ti_bc_err_inf_def tiBcErrInf ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tiBcErrInfOpr
 *
 *  Parameters:
 * 
 *      oprType - 操作类型， 输入， 取值
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode     - 返回值， 输出
 *      p_tiBcErrInf  - 查找条件，输入
 *                      查找结果，输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tiBcErrInfOpr(int oprType, ti_bc_err_inf_def *p_tiBcErrInf, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char resp_code       [6   + 1 ]; 
    char fwd_ins_id_cd   [8  + 1 ]; 
    char rcv_ins_id_cd   [8  + 1 ];
    /* EXEC SQL END DECLARE SECTION; */ 


    switch ( oprType )
    {
    	
    case GLB_DB_SELECT1:
		memset(resp_code, 0, sizeof(resp_code));
		memset(fwd_ins_id_cd, 0, sizeof(fwd_ins_id_cd));
		memset(rcv_ins_id_cd, 0, sizeof(rcv_ins_id_cd));
        memcpy(resp_code, p_tiBcErrInf->resp_code,sizeof(resp_code)-1);
        memcpy(fwd_ins_id_cd, p_tiBcErrInf->fwd_ins_id_cd,sizeof(fwd_ins_id_cd)-1);
        memcpy(rcv_ins_id_cd, p_tiBcErrInf->rcv_ins_id_cd,sizeof(rcv_ins_id_cd)-1);
        rtrim(resp_code);
        rtrim(fwd_ins_id_cd);
        rtrim(fwd_ins_id_cd);

        /* EXEC SQL SELECT 
        resp_code       ,
        fwd_ins_id_cd        ,
        rcv_ins_id_cd     ,
        fld_28          ,
        resp_cd        ,
        msg       
        INTO 
        :tiBcErrInf.resp_code       ,
        :tiBcErrInf.fwd_ins_id_cd        ,
        :tiBcErrInf.rcv_ins_id_cd     ,
        :tiBcErrInf.fld_28          ,
        :tiBcErrInf.resp_cd        ,
        :tiBcErrInf.msg       
        FROM ti_bc_error_inf
        WHERE 
        rtrim(resp_code)=:resp_code and 
        rtrim(fwd_ins_id_cd)=:fwd_ins_id_cd and 
        rtrim(rcv_ins_id_cd)=:rcv_ins_id_cd; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 9;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select resp_code ,fwd_ins_id_cd ,rcv_ins_id_cd ,fld_\
28 ,resp_cd ,msg into :b0,:b1,:b2,:b3,:b4,:b5  from ti_bc_error_inf where ((r\
trim(resp_code)=:b6 and rtrim(fwd_ins_id_cd)=:b7) and rtrim(rcv_ins_id_cd)=:b\
8)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tiBcErrInf.resp_code);
        sqlstm.sqhstl[0] = (unsigned int  )7;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tiBcErrInf.fwd_ins_id_cd);
        sqlstm.sqhstl[1] = (unsigned int  )9;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tiBcErrInf.rcv_ins_id_cd);
        sqlstm.sqhstl[2] = (unsigned int  )9;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tiBcErrInf.fld_28);
        sqlstm.sqhstl[3] = (unsigned int  )7;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tiBcErrInf.resp_cd);
        sqlstm.sqhstl[4] = (unsigned int  )3;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tiBcErrInf.msg);
        sqlstm.sqhstl[5] = (unsigned int  )201;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)resp_code;
        sqlstm.sqhstl[6] = (unsigned int  )7;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)fwd_ins_id_cd;
        sqlstm.sqhstl[7] = (unsigned int  )9;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)rcv_ins_id_cd;
        sqlstm.sqhstl[8] = (unsigned int  )9;
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



        /* search result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
            memcpy(p_tiBcErrInf, &tiBcErrInf, sizeof(ti_bc_err_inf_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = 0;
            memset(p_tiBcErrInf->fld_28, '0', 6);
            p_tiBcErrInf->fld_28[6]=0;
            memcpy(p_tiBcErrInf->fld_28, p_tiBcErrInf->resp_code, min_strlen(p_tiBcErrInf->resp_code, 6));
            memcpy(p_tiBcErrInf->resp_cd, p_tiBcErrInf->resp_code, min_strlen(p_tiBcErrInf->resp_code, 2));
            strcpy(p_tiBcErrInf->msg, "未知错误"); 
            glb_Log(0,0,"<TI_BC_ERR_INF.pc:tiBcInsInfOpr:SELECT1>Search Record (resp_code:%s,fwd_ins_id_cd:%s,rcv_ins_id_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", resp_code, fwd_ins_id_cd, rcv_ins_id_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<TI_BC_ERR_INF.pc:tiBcInsInfOpr:SELECT1>Search Record (resp_code:%s,fwd_ins_id_cd:%s,rcv_ins_id_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", resp_code, fwd_ins_id_cd, rcv_ins_id_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 9;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )56;
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

