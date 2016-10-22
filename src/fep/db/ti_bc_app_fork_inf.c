
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
           char  filnam[22];
};
static struct sqlcxp sqlfpn =
{
    21,
    "ti_bc_app_fork_inf.pc"
};


static unsigned int sqlctx = 169807627;


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
5,0,0,1,114,0,4,54,0,0,5,1,0,1,0,2,3,0,0,2,97,0,0,2,3,0,0,2,3,0,0,1,3,0,0,
40,0,0,2,0,0,29,77,0,0,0,0,0,1,0,
55,0,0,3,126,0,4,85,0,0,5,1,0,1,0,2,3,0,0,2,97,0,0,2,3,0,0,2,3,0,0,1,3,0,0,
90,0,0,4,0,0,29,103,0,0,0,0,0,1,0,
105,0,0,5,60,0,5,109,0,0,2,2,0,1,0,1,3,0,0,1,3,0,0,
128,0,0,6,0,0,29,118,0,0,0,0,0,1,0,
143,0,0,7,0,0,29,123,0,0,0,0,0,1,0,
158,0,0,8,95,0,3,134,0,0,4,4,0,1,0,1,3,0,0,1,97,0,0,1,3,0,0,1,3,0,0,
189,0,0,9,0,0,29,140,0,0,0,0,0,1,0,
204,0,0,10,0,0,29,145,0,0,0,0,0,1,0,
219,0,0,11,0,0,29,151,0,0,0,0,0,1,0,
234,0,0,12,0,0,29,154,0,0,0,0,0,1,0,
249,0,0,13,126,0,4,161,0,0,5,1,0,1,0,2,3,0,0,2,97,0,0,2,3,0,0,2,3,0,0,1,3,0,0,
284,0,0,14,0,0,29,179,0,0,0,0,0,1,0,
299,0,0,15,60,0,5,186,0,0,2,2,0,1,0,1,3,0,0,1,3,0,0,
322,0,0,16,0,0,29,195,0,0,0,0,0,1,0,
337,0,0,17,0,0,29,200,0,0,0,0,0,1,0,
352,0,0,18,0,0,29,213,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_app_fork_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

ti_bc_app_fork_inf_def tiBcAppForkInf ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tiBcAppForkInfOpr
 *
 *
 *  Parameters:
 * 
 *      oprType - 操作类型， 输入， 取值
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode        - 返回值， 输出
 *      p_tiCardBin      - 查找条件，输入
 *                         查找结果，输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

//int tiBcCardBinOpr(int oprType, ti_bc_card_bin_def *p_tiCardBin, int *p_sqlCode)
int tiBcAppForkInfOpr(int oprType, ti_bc_app_fork_inf_def *p_tiBcAppForkInf, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    int app_id;
    int fork_num;
    /* EXEC SQL END DECLARE SECTION; */ 

    
    switch ( oprType )
    {
    case GLB_DB_SELECT1:

        app_id = p_tiBcAppForkInf->app_id;

        /* EXEC SQL SELECT 
        app_id  ,
        app_name,
        fork_num,
        max_fork_num
        INTO :tiBcAppForkInf
        FROM ti_bc_app_fork_inf        
        WHERE app_id=:app_id
                     ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 5;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select app_id ,app_name ,fork_num ,max_fork_num into\
 :s1 ,:s2 ,:s3 ,:s4   from ti_bc_app_fork_inf where app_id=:b1";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)&tiBcAppForkInf.app_id;
        sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tiBcAppForkInf.app_name;
        sqlstm.sqhstl[1] = (unsigned int  )31;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&tiBcAppForkInf.fork_num;
        sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)&tiBcAppForkInf.max_fork_num;
        sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)&app_id;
        sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
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
            memcpy(p_tiBcAppForkInf, &tiBcAppForkInf, sizeof(ti_bc_app_fork_inf_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TI_BC_APP_FORK_INF.pc:tiBcAppForkInfOpr:SELECT1>Search Record (APP_ID:%d) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", app_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<TI_BC_APP_FORK_INF.pc:tiBcAppForkInfOpr:SELECT1>Search Record (APP_ID:%d) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", app_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );            
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
        sqlstm.offset = (unsigned int  )40;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;


    case GLB_DB_UPDATE2:

        app_id = p_tiBcAppForkInf->app_id;

        /* EXEC SQL SELECT 
        app_id  ,
        app_name,
        fork_num,
        max_fork_num
        INTO :tiBcAppForkInf
        FROM ti_bc_app_fork_inf
        WHERE app_id=:app_id
                     FOR update
                     ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 5;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select app_id ,app_name ,fork_num ,max_fork_num into\
 :s1 ,:s2 ,:s3 ,:s4   from ti_bc_app_fork_inf where app_id=:b1 for update ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )55;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)&tiBcAppForkInf.app_id;
        sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tiBcAppForkInf.app_name;
        sqlstm.sqhstl[1] = (unsigned int  )31;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&tiBcAppForkInf.fork_num;
        sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)&tiBcAppForkInf.max_fork_num;
        sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)&app_id;
        sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
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
            fork_num = tiBcAppForkInf.fork_num;
            if ( fork_num>= tiBcAppForkInf.max_fork_num )
            {
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


                memcpy(p_tiBcAppForkInf, &tiBcAppForkInf, sizeof(ti_bc_app_fork_inf_def));
                *p_sqlCode = SQL_ERD_OUT_FORK_NUM;
                return 0;
            }
            ++ fork_num;
            /* EXEC SQL UPDATE ti_bc_app_fork_inf
            SET fork_num=:fork_num
                         WHERE app_id=:app_id; */ 

{
            struct sqlexd sqlstm;
            sqlorat((void **)0, &sqlctx, &oraca);
            sqlstm.sqlvsn = 12;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "update ti_bc_app_fork_inf  set fork_num=:b0 wher\
e app_id=:b1";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )105;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)4352;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)&fork_num;
            sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)&app_id;
            sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
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
            if ( sqlca.sqlcode == 0 )
            {
                memcpy(p_tiBcAppForkInf, &tiBcAppForkInf, sizeof(ti_bc_app_fork_inf_def));
                p_tiBcAppForkInf->fork_num = fork_num;
                /* EXEC SQL COMMIT WORK ; */ 

{
                struct sqlexd sqlstm;
                sqlorat((void **)0, &sqlctx, &oraca);
                sqlstm.sqlvsn = 12;
                sqlstm.arrsiz = 5;
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


                return 0;
            } else
            {
                glb_Log(0,0,"<TI_BC_APP_FORK_INF.pc:tiBcAppForkInfOpr:SELECT2>Search Record (APP_ID:%d) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", app_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
                /* EXEC SQL COMMIT WORK ; */ 

{
                struct sqlexd sqlstm;
                sqlorat((void **)0, &sqlctx, &oraca);
                sqlstm.sqlvsn = 12;
                sqlstm.arrsiz = 5;
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


                return -1;
            }

        } else if ( sqlca.sqlcode == 1403 )
        {
            tiBcAppForkInf.app_id = app_id ;
            strcpy(tiBcAppForkInf.app_name, " ");
            tiBcAppForkInf.fork_num = 1;
            tiBcAppForkInf.max_fork_num = 100;

            /* EXEC SQL INSERT INTO ti_bc_app_fork_inf(app_id,app_name,fork_num,max_fork_num)values(:tiBcAppForkInf.app_id,:tiBcAppForkInf.app_name,:tiBcAppForkInf.fork_num,:tiBcAppForkInf.max_fork_num); */ 

{
            struct sqlexd sqlstm;
            sqlorat((void **)0, &sqlctx, &oraca);
            sqlstm.sqlvsn = 12;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "insert into ti_bc_app_fork_inf (app_id,app_name,\
fork_num,max_fork_num) values (:b0,:b1,:b2,:b3)";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )158;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)4352;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)&(tiBcAppForkInf.app_id);
            sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)(tiBcAppForkInf.app_name);
            sqlstm.sqhstl[1] = (unsigned int  )31;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)&(tiBcAppForkInf.fork_num);
            sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned int  )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (         void  *)&(tiBcAppForkInf.max_fork_num);
            sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         void  *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned int  )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
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
            if ( sqlca.sqlcode == 0 )
            {
                memcpy(p_tiBcAppForkInf, &tiBcAppForkInf, sizeof(ti_bc_app_fork_inf_def));
                /* EXEC SQL COMMIT WORK ; */ 

{
                struct sqlexd sqlstm;
                sqlorat((void **)0, &sqlctx, &oraca);
                sqlstm.sqlvsn = 12;
                sqlstm.arrsiz = 5;
                sqlstm.sqladtp = &sqladt;
                sqlstm.sqltdsp = &sqltds;
                sqlstm.iters = (unsigned int  )1;
                sqlstm.offset = (unsigned int  )189;
                sqlstm.cud = sqlcud0;
                sqlstm.sqlest = (unsigned char  *)&sqlca;
                sqlstm.sqlety = (unsigned short)4352;
                sqlstm.occurs = (unsigned int  )0;
                sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                return 0;
            } else
            {
                glb_Log(0,0, "<TI_BC_APP_FORK_INF.pc:tiBcAppForkInfOpr:SELECT2>Search Record (APP_ID:%d) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", app_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
                /* EXEC SQL COMMIT WORK ; */ 

{
                struct sqlexd sqlstm;
                sqlorat((void **)0, &sqlctx, &oraca);
                sqlstm.sqlvsn = 12;
                sqlstm.arrsiz = 5;
                sqlstm.sqladtp = &sqladt;
                sqlstm.sqltdsp = &sqltds;
                sqlstm.iters = (unsigned int  )1;
                sqlstm.offset = (unsigned int  )204;
                sqlstm.cud = sqlcud0;
                sqlstm.sqlest = (unsigned char  *)&sqlca;
                sqlstm.sqlety = (unsigned short)4352;
                sqlstm.occurs = (unsigned int  )0;
                sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                return -1;
            }
        } else
        {
            glb_Log(0,0, "<TI_BC_APP_FORK_INF.pc:tiBcAppForkInfOpr:SELECT2>Search Record (APP_ID:%d) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", app_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            /* EXEC SQL COMMIT WORK ; */ 

{
            struct sqlexd sqlstm;
            sqlorat((void **)0, &sqlctx, &oraca);
            sqlstm.sqlvsn = 12;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )219;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)4352;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


            return -1;
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
        sqlstm.offset = (unsigned int  )234;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    case GLB_DB_UPDATE3:

        app_id = p_tiBcAppForkInf->app_id;

        /* EXEC SQL SELECT 
        app_id  ,
        app_name,
        fork_num,
        max_fork_num
        INTO :tiBcAppForkInf
        FROM ti_bc_app_fork_inf        
        WHERE app_id=:app_id
                     FOR update
                     ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 5;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select app_id ,app_name ,fork_num ,max_fork_num into\
 :s1 ,:s2 ,:s3 ,:s4   from ti_bc_app_fork_inf where app_id=:b1 for update ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )249;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)&tiBcAppForkInf.app_id;
        sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tiBcAppForkInf.app_name;
        sqlstm.sqhstl[1] = (unsigned int  )31;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&tiBcAppForkInf.fork_num;
        sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)&tiBcAppForkInf.max_fork_num;
        sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)&app_id;
        sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
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
            fork_num = tiBcAppForkInf.fork_num;
            if ( fork_num>= tiBcAppForkInf.max_fork_num )
            {
                /* EXEC SQL COMMIT WORK ; */ 

{
                struct sqlexd sqlstm;
                sqlorat((void **)0, &sqlctx, &oraca);
                sqlstm.sqlvsn = 12;
                sqlstm.arrsiz = 5;
                sqlstm.sqladtp = &sqladt;
                sqlstm.sqltdsp = &sqltds;
                sqlstm.iters = (unsigned int  )1;
                sqlstm.offset = (unsigned int  )284;
                sqlstm.cud = sqlcud0;
                sqlstm.sqlest = (unsigned char  *)&sqlca;
                sqlstm.sqlety = (unsigned short)4352;
                sqlstm.occurs = (unsigned int  )0;
                sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                memcpy(p_tiBcAppForkInf, &tiBcAppForkInf, sizeof(ti_bc_app_fork_inf_def));
                *p_sqlCode = SQL_ERD_OUT_FORK_NUM;
                return 0;
            }
            if ( fork_num>0 ) --fork_num ;

            /* EXEC SQL UPDATE ti_bc_app_fork_inf
            SET fork_num=:fork_num
                         WHERE app_id=:app_id; */ 

{
            struct sqlexd sqlstm;
            sqlorat((void **)0, &sqlctx, &oraca);
            sqlstm.sqlvsn = 12;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "update ti_bc_app_fork_inf  set fork_num=:b0 wher\
e app_id=:b1";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )299;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)4352;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)&fork_num;
            sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)&app_id;
            sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
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
            if ( sqlca.sqlcode == 0 )
            {
                memcpy(p_tiBcAppForkInf, &tiBcAppForkInf, sizeof(ti_bc_app_fork_inf_def));
                p_tiBcAppForkInf->fork_num = fork_num;
                /* EXEC SQL COMMIT WORK ; */ 

{
                struct sqlexd sqlstm;
                sqlorat((void **)0, &sqlctx, &oraca);
                sqlstm.sqlvsn = 12;
                sqlstm.arrsiz = 5;
                sqlstm.sqladtp = &sqladt;
                sqlstm.sqltdsp = &sqltds;
                sqlstm.iters = (unsigned int  )1;
                sqlstm.offset = (unsigned int  )322;
                sqlstm.cud = sqlcud0;
                sqlstm.sqlest = (unsigned char  *)&sqlca;
                sqlstm.sqlety = (unsigned short)4352;
                sqlstm.occurs = (unsigned int  )0;
                sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                return 0;
            } else
            {
                glb_Log(0,0, "<TI_BC_APP_FORK_INF.pc:tiBcAppForkInfOpr:UPDATE3>Search Record (APP_ID:%d) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", app_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
                /* EXEC SQL COMMIT WORK ; */ 

{
                struct sqlexd sqlstm;
                sqlorat((void **)0, &sqlctx, &oraca);
                sqlstm.sqlvsn = 12;
                sqlstm.arrsiz = 5;
                sqlstm.sqladtp = &sqladt;
                sqlstm.sqltdsp = &sqltds;
                sqlstm.iters = (unsigned int  )1;
                sqlstm.offset = (unsigned int  )337;
                sqlstm.cud = sqlcud0;
                sqlstm.sqlest = (unsigned char  *)&sqlca;
                sqlstm.sqlety = (unsigned short)4352;
                sqlstm.occurs = (unsigned int  )0;
                sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                return -1;
            }

        } else if ( sqlca.sqlcode == 1403 )
        {

            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TI_BC_APP_FORK_INF.pc:tiBcAppForkInfOpr:UPDATE3>Search Record (APP_ID:%d) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", app_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<TI_BC_APP_FORK_INF.pc:tiBcAppForkInfOpr:UPDATE3>Search Record (APP_ID:%d) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", app_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
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
        sqlstm.offset = (unsigned int  )352;
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

