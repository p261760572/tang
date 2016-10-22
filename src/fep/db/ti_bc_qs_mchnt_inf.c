
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
    "ti_bc_qs_mchnt_inf.pc"
};


static unsigned int sqlctx = 170112395;


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
            void  *sqhstv[13];
   unsigned int   sqhstl[13];
            int   sqhsts[13];
            void  *sqindv[13];
            int   sqinds[13];
   unsigned int   sqharm[13];
   unsigned int   *sqharc[13];
   unsigned short  sqadto[13];
   unsigned short  sqtdso[13];
} sqlstm = {12,13};

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
5,0,0,1,249,0,4,73,0,0,13,3,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
72,0,0,2,0,0,29,113,0,0,0,0,0,1,0,
87,0,0,3,124,0,5,136,0,0,6,6,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,
0,1,97,0,0,
126,0,0,4,0,0,29,154,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_qs_mchnt_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

ti_bc_qs_mchnt_inf_def tiBcQsMchntInf ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tiBcQsMchntInfOpr
 *
 *
 *  Parameters:
 * 
 *      oprType - 操作类型， 输入， 取值
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode      - 返回值， 输出
 *      p_tiBcQsMchntInf - 查找条件，输入
 *                       查找结果，输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tiBcQsMchntInfOpr(int oprType, ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char term_id         [ 8 + 1 ];
    char mchnt_cd        [20 + 1 ];  
    char rcv_ins_id_cd   [15 + 1 ];
    char mkey_index      [3  + 1 ];
    char mkey_1          [16 + 1 ];
    char mkey_2          [16 + 1 ];
    char mkey_3          [16 + 1 ];
    /* EXEC SQL END DECLARE SECTION; */ 


    memset(term_id,0,sizeof(term_id));
    memset(mchnt_cd,0,sizeof(mchnt_cd));
    memset(rcv_ins_id_cd,0,sizeof(rcv_ins_id_cd));
    memset(mkey_index,0,sizeof(mkey_index));
    memset(mkey_1,0,sizeof(mkey_1));
    memset(mkey_2,0,sizeof(mkey_2));
    memset(mkey_3,0,sizeof(mkey_3));

    switch (oprType)
    {
    case GLB_DB_SELECT1:

        strcpy_safe(term_id, p_tiBcQsMchntInf->term_id,sizeof(term_id)-1);
        rtrim_null(term_id,' ');
        
        strcpy_safe(mchnt_cd, p_tiBcQsMchntInf->mchnt_cd,sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');
        
        strcpy_safe(rcv_ins_id_cd, p_tiBcQsMchntInf->rcv_ins_id_cd,sizeof(rcv_ins_id_cd)-1);
        rtrim_null(rcv_ins_id_cd,' ');

        /* EXEC SQL SELECT 
        mchnt_cd  	       ,
        term_id   	       ,
        rcv_ins_id_cd      ,
        mchnt_cn_abbr      ,
        reversal_mode      ,
        openflag           ,
        mkey_index         ,
        mkey_1             ,
        mkey_2             ,
        mkey_3             
        INTO 
        :tiBcQsMchntInf.mchnt_cd  	       ,
        :tiBcQsMchntInf.term_id   	       ,
        :tiBcQsMchntInf.rcv_ins_id_cd      ,
        :tiBcQsMchntInf.mchnt_cn_abbr      ,
        :tiBcQsMchntInf.reversal_mode      ,
        :tiBcQsMchntInf.openflag           ,
        :tiBcQsMchntInf.mkey_index         ,
        :tiBcQsMchntInf.mkey_1             ,
        :tiBcQsMchntInf.mkey_2             ,
        :tiBcQsMchntInf.mkey_3             
        	
        FROM ti_bc_qs_mchnt_inf
        WHERE mchnt_cd=:mchnt_cd and term_id=:term_id and rcv_ins_id_cd=:rcv_ins_id_cd
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 13;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select mchnt_cd ,term_id ,rcv_ins_id_cd ,mchnt_cn_ab\
br ,reversal_mode ,openflag ,mkey_index ,mkey_1 ,mkey_2 ,mkey_3 into :b0,:b1,\
:b2,:b3,:b4,:b5,:b6,:b7,:b8,:b9  from ti_bc_qs_mchnt_inf where ((mchnt_cd=:b1\
0 and term_id=:b11) and rcv_ins_id_cd=:b12)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tiBcQsMchntInf.mchnt_cd);
        sqlstm.sqhstl[0] = (unsigned int  )21;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tiBcQsMchntInf.term_id);
        sqlstm.sqhstl[1] = (unsigned int  )9;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tiBcQsMchntInf.rcv_ins_id_cd);
        sqlstm.sqhstl[2] = (unsigned int  )9;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tiBcQsMchntInf.mchnt_cn_abbr);
        sqlstm.sqhstl[3] = (unsigned int  )41;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tiBcQsMchntInf.reversal_mode);
        sqlstm.sqhstl[4] = (unsigned int  )2;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tiBcQsMchntInf.openflag);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tiBcQsMchntInf.mkey_index);
        sqlstm.sqhstl[6] = (unsigned int  )4;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tiBcQsMchntInf.mkey_1);
        sqlstm.sqhstl[7] = (unsigned int  )17;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tiBcQsMchntInf.mkey_2);
        sqlstm.sqhstl[8] = (unsigned int  )17;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tiBcQsMchntInf.mkey_3);
        sqlstm.sqhstl[9] = (unsigned int  )17;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[10] = (unsigned int  )21;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)term_id;
        sqlstm.sqhstl[11] = (unsigned int  )9;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)rcv_ins_id_cd;
        sqlstm.sqhstl[12] = (unsigned int  )16;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
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
            memcpy(p_tiBcQsMchntInf, &tiBcQsMchntInf, sizeof(ti_bc_qs_mchnt_inf_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0,"<FILE:%s,LINE:%d><ti_bc_qs_mchnt_inf.pc:tiBcQsMchntInfOpr:SELECT1>Search Record (mchnt_cd:%s,term_id:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",__FILE__,__LINE__, mchnt_cd,term_id,sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<FILE:%s,LINE:%d><ti_bc_qs_mchnt_inf.pc:tiBcQsMchntInfOpr:SELECT1>Search Record (mchnt_cd:%s,term_id:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",__FILE__,__LINE__, mchnt_cd,term_id, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 13;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )72;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    case GLB_DB_UPDATE2:

        strcpy_safe(term_id, p_tiBcQsMchntInf->term_id,sizeof(term_id)-1);
        rtrim_null(term_id,' ');
        
        strcpy_safe(mchnt_cd, p_tiBcQsMchntInf->mchnt_cd,sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');
        
        strcpy_safe(rcv_ins_id_cd, p_tiBcQsMchntInf->rcv_ins_id_cd,sizeof(rcv_ins_id_cd)-1);
        rtrim_null(rcv_ins_id_cd,' ');

        strcpy_safe(mkey_1, p_tiBcQsMchntInf->mkey_1,16);
        rtrim_null(mkey_1, ' ');        

				 strcpy_safe(mkey_2, p_tiBcQsMchntInf->mkey_2,16);
        rtrim_null(mkey_2, ' '); 
        
        strcpy_safe(mkey_3, p_tiBcQsMchntInf->mkey_3,16);
        rtrim_null(mkey_3, ' '); 

        /* EXEC SQL UPDATE ti_bc_qs_mchnt_inf
        SET mkey_1=:mkey_1, mkey_2=:mkey_2, mkey_3=:mkey_3
         WHERE mchnt_cd=:mchnt_cd and term_id=:term_id and rcv_ins_id_cd=:rcv_ins_id_cd
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 13;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "update ti_bc_qs_mchnt_inf  set mkey_1=:b0,mkey_2=:b1\
,mkey_3=:b2 where ((mchnt_cd=:b3 and term_id=:b4) and rcv_ins_id_cd=:b5)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )87;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)mkey_1;
        sqlstm.sqhstl[0] = (unsigned int  )17;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)mkey_2;
        sqlstm.sqhstl[1] = (unsigned int  )17;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)mkey_3;
        sqlstm.sqhstl[2] = (unsigned int  )17;
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
        sqlstm.sqhstv[5] = (         void  *)rcv_ins_id_cd;
        sqlstm.sqhstl[5] = (unsigned int  )16;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
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
        	
        } else if ( memcmp(sqlca.sqlerrm.sqlerrmc, "ORA-00001", 0 ) == 0 )
        {
            *p_sqlCode = SQL_ERD_UNIKEY;
            glb_Log(0,0,"<ti_bc_qs_mchnt_inf.pc:tiBcQsMchntInfOpr:UPDATE2>Search Record (mchnt_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", mchnt_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<ti_bc_qs_mchnt_inf.pc:tiBcQsMchntInfOpr:UPDATE2>Search Record (mchnt_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", mchnt_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 13;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )126;
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

