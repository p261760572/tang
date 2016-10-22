
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
           char  filnam[27];
};
static struct sqlcxp sqlfpn =
{
    26,
    "ti_bc_route_gateway_inf.pc"
};


static unsigned int sqlctx = 1147545867;


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
5,0,0,1,297,0,4,67,0,0,5,2,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,
40,0,0,2,0,0,29,100,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/ti_bc_route_gateway_inf.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

ti_bc_route_gateway_inf_def tiBcRouteGatewayInf ;
/* EXEC SQL END DECLARE SECTION; */ 


#define MIN(a,b) a>b?b:a

/*
 *  Function:  tiBcRouteGatewayInfOpr
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

int tiBcRouteGatewayInfOpr(int oprType, ti_bc_route_gateway_inf_def *p_tiBcRouteGatewayInf, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char rcv_ins_id_cd     [  8 + 1 ];
    char iss_ins_id_cd     [  8 + 1 ];
    /* EXEC SQL END DECLARE SECTION; */ 


    MEMSET_0(iss_ins_id_cd)
    MEMSET_0(rcv_ins_id_cd)

    int i;
    switch ( oprType )
    {
    
    case GLB_DB_SELECT1:
        i = MIN(sizeof(iss_ins_id_cd),sizeof(p_tiBcRouteGatewayInf->iss_ins_id_cd));
        memcpy(iss_ins_id_cd, p_tiBcRouteGatewayInf->iss_ins_id_cd, i);
        iss_ins_id_cd[i-1]=0;
        rtrim(iss_ins_id_cd);
        ADJUST_STRING(iss_ins_id_cd);

        i = MIN(sizeof(rcv_ins_id_cd),sizeof(p_tiBcRouteGatewayInf->rcv_ins_id_cd));
        memcpy(rcv_ins_id_cd, p_tiBcRouteGatewayInf->rcv_ins_id_cd, i);
        rcv_ins_id_cd[i-1]=0;
        ADJUST_STRING(rcv_ins_id_cd);

        /* EXEC SQL SELECT 
        rcv_ins_id_cd     ,
        iss_ins_id_cd     ,
        gate_id     
        INTO :tiBcRouteGatewayInf
        FROM (
             SELECT 
        	rcv_ins_id_cd     ,
        	iss_ins_id_cd     ,
        	gate_id     
             FROM ti_bc_route_gateway_inf
             WHERE 
             rtrim(rcv_ins_id_cd)=:rcv_ins_id_cd 
             AND rtrim(iss_ins_id_cd)=:iss_ins_id_cd
             ORDER BY rcv_ins_id_cd desc, iss_ins_id_cd desc
             )                     
        WHERE rownum<=1
        ORDER BY ROWNUM ASC
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 5;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "select rcv_ins_id_cd ,iss_ins_id_cd ,gate_id into :s\
1 ,:s2 ,:s3   from (select rcv_ins_id_cd ,iss_ins_id_cd ,gate_id  from ti_bc_\
route_gateway_inf where (rtrim(rcv_ins_id_cd)=:b1 and rtrim(iss_ins_id_cd)=:b\
2) order by rcv_ins_id_cd desc ,iss_ins_id_cd desc  ) where rownum<=1 order b\
y ROWNUM asc  ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)tiBcRouteGatewayInf.rcv_ins_id_cd;
        sqlstm.sqhstl[0] = (unsigned int  )9;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)tiBcRouteGatewayInf.iss_ins_id_cd;
        sqlstm.sqhstl[1] = (unsigned int  )9;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)tiBcRouteGatewayInf.gate_id;
        sqlstm.sqhstl[2] = (unsigned int  )16;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)rcv_ins_id_cd;
        sqlstm.sqhstl[3] = (unsigned int  )9;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)iss_ins_id_cd;
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
            memcpy(p_tiBcRouteGatewayInf, &tiBcRouteGatewayInf, sizeof(ti_bc_route_gateway_inf_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<FILE:%s,LINE:%d><TI_BC_ROUTE_GATEWAY_INF.pc.pc:tiBcRouteGatewayInfOpr:SELECT1>Search Record (rcv_ins_id_cd:%s,iss_ins_id_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", __FILE__,__LINE__,rcv_ins_id_cd,iss_ins_id_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<FILE:%s,LINE:%d><TI_BC_ROUTE_GATEWAY_INF.pc.pc:tiBcRouteGatewayInfOpr:SELECT1>Search Record (rcv_ins_id_cd:%s,iss_ins_id_cd:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", __FILE__,__LINE__,rcv_ins_id_cd,iss_ins_id_cd, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
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

    default:
        *p_sqlCode = -1;
        return -1;
    }

    return 0;
}


