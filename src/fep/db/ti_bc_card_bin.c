
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
           char  filnam[18];
};
static struct sqlcxp sqlfpn =
{
    17,
    "ti_bc_card_bin.pc"
};


static unsigned int sqlctx = 10605211;


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
5,0,0,1,542,0,4,65,0,0,11,3,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
64,0,0,2,0,0,29,113,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_card_bin.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

ti_bc_card_bin_def tiCardBin ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tiBcCardBinOpr
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

int tiBcCardBinOpr(int oprType, ti_bc_card_bin_def *p_tiCardBin, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char card_bin        [21 + 1] ; 
    char pos_entry_md_cd2 [1+1];
    char pos_entry_md_cd3 [1+1];
    /* EXEC SQL END DECLARE SECTION; */ 


    memset(card_bin,0,sizeof(card_bin));
    memset(pos_entry_md_cd2,0,sizeof(pos_entry_md_cd2));
    memset(pos_entry_md_cd3,0,sizeof(pos_entry_md_cd3));
    
    switch (oprType)
    {
    case GLB_DB_SELECT1:

        memcpy_safe(card_bin, p_tiCardBin->card_bin, sizeof(card_bin)-1);
        rtrim_null(card_bin,' ');
        
        memcpy_safe(pos_entry_md_cd2, p_tiCardBin->pos_entry_md_cd2, sizeof(pos_entry_md_cd2)-1);
        rtrim_null(pos_entry_md_cd2,' ');
        
        memcpy_safe(pos_entry_md_cd3, p_tiCardBin->pos_entry_md_cd3, sizeof(pos_entry_md_cd3)-1);
        rtrim_null(pos_entry_md_cd3,' ');
        
        /* EXEC SQL SELECT 
        card_bin      ,
           pos_entry_md_cd2,
           pos_entry_md_cd3,
        card_attr     ,
        iss_ins_id_cd ,
        fst_rcv_ins_id_cd ,
        snd_rcv_ins_id_cd ,
        enable_flag 
        INTO 
          :tiCardBin.card_bin               ,
          :tiCardBin.pos_entry_md_cd2       ,
          :tiCardBin.pos_entry_md_cd3       ,
          :tiCardBin.card_attr              ,
          :tiCardBin.iss_ins_id_cd          ,
          :tiCardBin.fst_rcv_ins_id_cd      ,
          :tiCardBin.snd_rcv_ins_id_cd      ,
          :tiCardBin.enable_flag            
        FROM( select 
              card_bin      ,
              pos_entry_md_cd2,
              pos_entry_md_cd3,
              card_attr     ,
              iss_ins_id_cd ,
              fst_rcv_ins_id_cd ,
              snd_rcv_ins_id_cd ,
              enable_flag 
              FROM ti_bc_card_bin
              WHERE ( :card_bin like rtrim(card_bin)||'%' or card_bin='*' ) 
                 AND (pos_entry_md_cd2=:pos_entry_md_cd2 or pos_entry_md_cd2='*' ) 
                 AND (pos_entry_md_cd3=:pos_entry_md_cd3 or pos_entry_md_cd3='*' )  
              order by length(rtrim(card_bin)) desc
            ) where rownum<=1
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 11;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select card_bin ,pos_entry_md_cd2 ,pos_entry_md_cd3 \
,card_attr ,iss_ins_id_cd ,fst_rcv_ins_id_cd ,snd_rcv_ins_id_cd ,enable_flag \
into :b0,:b1,:b2,:b3,:b4,:b5,:b6,:b7  from (select card_bin ,pos_entry_md_cd2\
 ,pos_entry_md_cd3 ,card_attr ,iss_ins_id_cd ,fst_rcv_ins_id_cd ,snd_rcv_ins_\
id_cd ,enable_flag  from ti_bc_card_bin where (((:b8 like (rtrim(card_bin)||'\
%') or card_bin='*') and (pos_entry_md_cd2=:b9 or pos_entry_md_cd2='*')) and \
(pos_entry_md_cd3=:b10 or pos_entry_md_cd3='*')) order by length(rtrim(card_b\
in)) desc  ) where rownum<=1";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tiCardBin.card_bin);
        sqlstm.sqhstl[0] = (unsigned int  )22;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tiCardBin.pos_entry_md_cd2);
        sqlstm.sqhstl[1] = (unsigned int  )2;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tiCardBin.pos_entry_md_cd3);
        sqlstm.sqhstl[2] = (unsigned int  )2;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tiCardBin.card_attr);
        sqlstm.sqhstl[3] = (unsigned int  )3;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tiCardBin.iss_ins_id_cd);
        sqlstm.sqhstl[4] = (unsigned int  )9;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tiCardBin.fst_rcv_ins_id_cd);
        sqlstm.sqhstl[5] = (unsigned int  )9;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tiCardBin.snd_rcv_ins_id_cd);
        sqlstm.sqhstl[6] = (unsigned int  )9;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tiCardBin.enable_flag);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)card_bin;
        sqlstm.sqhstl[8] = (unsigned int  )22;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)pos_entry_md_cd2;
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)pos_entry_md_cd3;
        sqlstm.sqhstl[10] = (unsigned int  )2;
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
            memcpy(p_tiCardBin, &tiCardBin, sizeof(ti_bc_card_bin_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0,"<FILE:%s,LINE:%d><TI_BC_CARD_BIN.pc:tiCardBinOpr:SELECT1>Search Record (card_bin:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",__FILE__,__LINE__, card_bin, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<FILE:%s,LINE:%d><TI_BC_CARD_BIN.pc:tiCardBinOpr:SELECT1>Search Record (card_bin:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",__FILE__,__LINE__, card_bin, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
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

