
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
    "tl_bc_trans_log.pc"
};


static unsigned int sqlctx = 21453699;


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
            void  *sqhstv[72];
   unsigned int   sqhstl[72];
            int   sqhsts[72];
            void  *sqindv[72];
            int   sqinds[72];
   unsigned int   sqharm[72];
   unsigned int   *sqharc[72];
   unsigned short  sqadto[72];
   unsigned short  sqtdso[72];
} sqlstm = {12,72};

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
5,0,0,1,1259,0,4,79,0,0,72,4,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
308,0,0,2,0,0,29,238,0,0,0,0,0,1,0,
323,0,0,3,1240,0,4,254,0,0,71,3,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
622,0,0,4,0,0,29,413,0,0,0,0,0,1,0,
637,0,0,5,1280,0,4,430,0,0,72,4,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
940,0,0,6,0,0,29,589,0,0,0,0,0,1,0,
955,0,0,7,1271,0,4,606,0,0,72,4,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1258,0,0,8,150,0,5,778,0,0,5,5,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,
1293,0,0,9,0,0,29,801,0,0,0,0,0,1,0,
1308,0,0,10,1292,0,4,818,0,0,72,4,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1611,0,0,11,171,0,5,990,0,0,5,5,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,
1646,0,0,12,0,0,29,1015,0,0,0,0,0,1,0,
1661,0,0,13,1096,0,3,1093,0,0,68,68,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,
0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,
0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,
1948,0,0,14,0,0,29,1178,0,0,0,0,0,1,0,
1963,0,0,15,802,0,5,1242,0,0,47,47,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,
0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
2166,0,0,16,0,0,29,1307,0,0,0,0,0,1,0,
2181,0,0,17,1138,0,5,1396,0,0,64,64,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,
0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,
0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
2452,0,0,18,0,0,29,1541,0,0,0,0,0,1,0,
2467,0,0,19,1292,0,4,1559,0,0,72,4,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
2770,0,0,20,258,0,5,1719,0,0,10,10,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
2825,0,0,21,0,0,29,1744,0,0,0,0,0,1,0,
2840,0,0,22,122,0,5,1764,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
2871,0,0,23,0,0,29,1783,0,0,0,0,0,1,0,
2886,0,0,24,150,0,4,1824,0,0,5,3,0,1,0,2,4,0,0,2,4,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,
2921,0,0,25,128,0,4,1832,0,0,4,2,0,1,0,2,4,0,0,2,4,0,0,1,97,0,0,1,97,0,0,
2952,0,0,26,131,0,4,1840,0,0,4,2,0,1,0,2,4,0,0,2,4,0,0,1,97,0,0,1,97,0,0,
2983,0,0,27,0,0,29,1851,0,0,0,0,0,1,0,
2998,0,0,28,0,0,29,1856,0,0,0,0,0,1,0,
3013,0,0,29,0,0,29,1862,0,0,0,0,0,1,0,
3028,0,0,30,155,0,4,1899,0,0,5,3,0,1,0,2,3,0,0,2,3,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,
3063,0,0,31,133,0,4,1906,0,0,4,2,0,1,0,2,3,0,0,2,3,0,0,1,97,0,0,1,97,0,0,
3094,0,0,32,136,0,4,1913,0,0,4,2,0,1,0,2,3,0,0,2,3,0,0,1,97,0,0,1,97,0,0,
3125,0,0,33,0,0,29,1920,0,0,0,0,0,1,0,
3140,0,0,34,0,0,29,1925,0,0,0,0,0,1,0,
3155,0,0,35,0,0,29,1931,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/tl_bc_trans_log.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 

tl_bc_trans_log_def tlBcTransLog ;
/* EXEC SQL END DECLARE SECTION; */ 



/*
 *  Function:  tlBcTransLogOpr
 *
 *
 *  Parameters:
 * 
 *      oprType - 操作类型， 输入， 取值
 *                SELECT1     
 *                UPDATE1     
 *      p_sqlCode        - 返回值， 输出
 *      p_tlBcTransLog   - 查找条件，输入
 *                         查找结果，输出
 *
 *  Return Value:
 *
 *      SUCCESS - success
 *      FAILURE - failure
 */

int tlBcTransLogOpr(int oprType, tl_bc_trans_log_def *p_tlBcTransLog, int *p_sqlCode)
{
    /*EXEC SQL WHENEVER SQLERROR DO sql_error("Oracle error:");*/
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    /*char  mchnt_tp        [4 + 1] ; */
    char  term_id         [8 + 1] ;  
    char  mchnt_cd        [20+ 1] ;  
    char  sys_tra_no      [6 + 1] ;  
    char  loc_trans_dt    [4 + 1] ;
    char  lock_tm         [10+ 4 + 1] ;
    char  c_sql[2048];
    /* EXEC SQL END DECLARE SECTION; */ 


    char buff[256];
    time_t timeold, timenew ;

    /*MEMSET_0(mchnt_tp);*/
    MEMSET_0(term_id);
    MEMSET_0(mchnt_cd);
    MEMSET_0(sys_tra_no);
    MEMSET_0(loc_trans_dt);

    char caTxnMsg[4096];
    switch ( oprType )
    {
    
    case GLB_DB_SELECT1:
        
        strcpy_safe(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, sizeof(loc_trans_dt)-1);
        rtrim_null(loc_trans_dt,' ');
        
        strcpy_safe(term_id, p_tlBcTransLog->term_id, sizeof(term_id)-1);
        rtrim_null(term_id,' ');

        strcpy_safe(mchnt_cd, p_tlBcTransLog->mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');

				 strcpy_safe(sys_tra_no, p_tlBcTransLog->sys_tra_no, sizeof(sys_tra_no)-1);
        rtrim_null(sys_tra_no,' ');
        
        /* EXEC SQL SELECT 
        sys_dt_tm            ,       
        settle_dt            ,
        trans_flag           ,
        is_over              ,
        lock_tm              ,
        flag_lock            ,
        flag_verifi          ,
        flag_rcv             ,
        flag_pay             ,
        flag_reversal        ,
        resp_cd_verifi       ,
        resp_cd_rcv          ,
        resp_cd_pay          ,
        resp_cd_reversal     ,
        trans_chnl           ,
        packet_type          ,
        line_nm_verifi       ,
        line_nm_rcv          ,
        line_nm_pay          ,
        reversal_id          ,
        bc_settle_in         ,
        fld32_ins_id_cd      ,
        fwd_ins_id_cd        ,
        rcv_ins_id_cd        ,
        msg_tp               ,
        pri_acct_no          ,
        proc_cd              ,
        trans_at             ,
        transmsn_dt_tm       ,
        sys_tra_no           ,
        loc_trans_tm         ,
        loc_trans_dt         ,
        mchnt_tp             ,
        pos_entry_md_cd      ,
        pos_cond_cd          ,
        retri_ref_no         ,
        auth_id_resp_cd      ,
        resp_cd              ,
        term_id              ,
        mchnt_cd             ,
        area_cd              ,
        trans_curr_cd        ,
        flag_1               ,
        flag_2               ,
        flag_3               ,
        flag_4               ,
        flag_5               ,
        flag_6               ,
        flag_7               ,
        fld_5                ,
        fld_8                ,
        fld_28               ,
        fld_34               ,
        fld_44               ,
        fld_45               ,
        fld_46               ,
        fld_48               ,
        fld_61               ,
        center_sys_tra_no    ,
        center_term_id       ,
        center_mchnt_cd      ,
        bill_id_rcv          ,
        card_attr            ,
        iss_ins_id_cd        ,
        pname				 ,
        encrypt_info 		 ,
        id_type				 ,
        orig_data_elemts
        INTO 
        	 :tlBcTransLog.sys_dt_tm            ,   
           :tlBcTransLog.settle_dt            ,
           :tlBcTransLog.trans_flag           ,
           :tlBcTransLog.is_over              ,
           :tlBcTransLog.lock_tm              ,
           :tlBcTransLog.flag_lock            ,
           :tlBcTransLog.flag_verifi          ,
           :tlBcTransLog.flag_rcv             ,
           :tlBcTransLog.flag_pay             ,
           :tlBcTransLog.flag_reversal        ,
           :tlBcTransLog.resp_cd_verifi       ,
           :tlBcTransLog.resp_cd_rcv          ,
           :tlBcTransLog.resp_cd_pay          , 
           :tlBcTransLog.resp_cd_reversal     ,
           :tlBcTransLog.trans_chnl           ,
           :tlBcTransLog.packet_type          ,
           :tlBcTransLog.line_nm_verifi       ,
           :tlBcTransLog.line_nm_rcv          ,
           :tlBcTransLog.line_nm_pay          ,
           :tlBcTransLog.reversal_id          ,
           :tlBcTransLog.bc_settle_in         ,
           :tlBcTransLog.fld32_ins_id_cd      ,
           :tlBcTransLog.fwd_ins_id_cd        ,
           :tlBcTransLog.rcv_ins_id_cd        ,
           :tlBcTransLog.msg_tp               ,
           :tlBcTransLog.pri_acct_no          ,
           :tlBcTransLog.proc_cd              ,
           :tlBcTransLog.trans_at             ,
           :tlBcTransLog.transmsn_dt_tm       ,
           :tlBcTransLog.sys_tra_no           ,
           :tlBcTransLog.loc_trans_tm         ,
           :tlBcTransLog.loc_trans_dt         ,
           :tlBcTransLog.mchnt_tp             ,
           :tlBcTransLog.pos_entry_md_cd      ,
           :tlBcTransLog.pos_cond_cd          ,
           :tlBcTransLog.retri_ref_no         ,
           :tlBcTransLog.auth_id_resp_cd      ,
           :tlBcTransLog.resp_cd              ,
           :tlBcTransLog.term_id              ,
           :tlBcTransLog.mchnt_cd             ,
           :tlBcTransLog.area_cd              ,
           :tlBcTransLog.trans_curr_cd        ,
           :tlBcTransLog.flag_1               ,
           :tlBcTransLog.flag_2               ,
           :tlBcTransLog.flag_3               ,
           :tlBcTransLog.flag_4               ,
           :tlBcTransLog.flag_5               ,
           :tlBcTransLog.flag_6               ,
           :tlBcTransLog.flag_7               ,
           :tlBcTransLog.fld_5                ,
           :tlBcTransLog.fld_8                ,
           :tlBcTransLog.fld_28               ,
           :tlBcTransLog.fld_34               ,
           :tlBcTransLog.fld_44               ,
           :tlBcTransLog.fld_45               ,
           :tlBcTransLog.fld_46               ,
           :tlBcTransLog.fld_48               ,
           :tlBcTransLog.fld_61               ,
           :tlBcTransLog.center_sys_tra_no    ,
           :tlBcTransLog.center_term_id       ,
           :tlBcTransLog.center_mchnt_cd      ,
           :tlBcTransLog.bill_id_rcv          ,
           :tlBcTransLog.card_attr            ,
           :tlBcTransLog.iss_ins_id_cd        ,
         	 :tlBcTransLog.pname						 	  ,
         	 :tlBcTransLog.encrypt_info	 	 	    ,
         	 :tlBcTransLog.id_type					,
         	 :tlBcTransLog.originaDataElements					
        FROM tl_bc_trans_log
        WHERE loc_trans_dt=:loc_trans_dt
				AND mchnt_cd=:mchnt_cd
				AND sys_tra_no=:sys_tra_no
				AND term_id=:term_id
				AND rownum<=1
               ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlbuft((void **)0,
          "select sys_dt_tm ,settle_dt ,trans_flag ,is_over ,lock_tm ,flag_l\
ock ,flag_verifi ,flag_rcv ,flag_pay ,flag_reversal ,resp_cd_verifi ,resp_cd\
_rcv ,resp_cd_pay ,resp_cd_reversal ,trans_chnl ,packet_type ,line_nm_verifi\
 ,line_nm_rcv ,line_nm_pay ,reversal_id ,bc_settle_in ,fld32_ins_id_cd ,fwd_\
ins_id_cd ,rcv_ins_id_cd ,msg_tp ,pri_acct_no ,proc_cd ,trans_at ,transmsn_d\
t_tm ,sys_tra_no ,loc_trans_tm ,loc_trans_dt ,mchnt_tp ,pos_entry_md_cd ,pos\
_cond_cd ,retri_ref_no ,auth_id_resp_cd ,resp_cd ,term_id ,mchnt_cd ,area_cd\
 ,trans_curr_cd ,flag_1 ,flag_2 ,flag_3 ,flag_4 ,flag_5 ,flag_6 ,flag_7 ,fld\
_5 ,fld_8 ,fld_28 ,fld_34 ,fld_44 ,fld_45 ,fld_46 ,fld_48 ,fld_61 ,center_sy\
s_tra_no ,center_term_id ,center_mchnt_cd ,bill_id_rcv ,card_attr ,iss_ins_i\
d_cd ,pname ,encrypt_info ,id_type ,orig_data_elemts into :b0,:b1,:b2,:b3,:b\
4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15,:b16,:b17,:b18,:b19,:b20\
,:b21,:b22,:b23,:b24,:b25,:b26,:b27,:b28,:b29,:b30,:b31,:b32,:b33,:b34,:b35,\
:b36,:b37,:b38,:b39,:b40,:b41,:b42,:b43,:b44,:b");
        sqlstm.stmt = "45,:b46,:b47,:b48,:b49,:b50,:b51,:b52,:b53,:b54,:b55\
,:b56,:b57,:b58,:b59,:b60,:b61,:b62,:b63,:b64,:b65,:b66,:b67  from tl_bc_tran\
s_log where ((((loc_trans_dt=:b68 and mchnt_cd=:b69) and sys_tra_no=:b70) and\
 term_id=:b71) and rownum<=1)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )5;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcTransLog.sys_dt_tm);
        sqlstm.sqhstl[0] = (unsigned int  )15;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.settle_dt);
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.trans_flag);
        sqlstm.sqhstl[2] = (unsigned int  )5;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.is_over);
        sqlstm.sqhstl[3] = (unsigned int  )2;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.lock_tm);
        sqlstm.sqhstl[4] = (unsigned int  )11;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.flag_lock);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcTransLog.flag_verifi);
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcTransLog.flag_rcv);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcTransLog.flag_pay);
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcTransLog.flag_reversal);
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcTransLog.resp_cd_verifi);
        sqlstm.sqhstl[10] = (unsigned int  )7;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcTransLog.resp_cd_rcv);
        sqlstm.sqhstl[11] = (unsigned int  )7;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcTransLog.resp_cd_pay);
        sqlstm.sqhstl[12] = (unsigned int  )7;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcTransLog.resp_cd_reversal);
        sqlstm.sqhstl[13] = (unsigned int  )7;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcTransLog.trans_chnl);
        sqlstm.sqhstl[14] = (unsigned int  )31;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcTransLog.packet_type);
        sqlstm.sqhstl[15] = (unsigned int  )5;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tlBcTransLog.line_nm_verifi);
        sqlstm.sqhstl[16] = (unsigned int  )5;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tlBcTransLog.line_nm_rcv);
        sqlstm.sqhstl[17] = (unsigned int  )5;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tlBcTransLog.line_nm_pay);
        sqlstm.sqhstl[18] = (unsigned int  )5;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tlBcTransLog.reversal_id);
        sqlstm.sqhstl[19] = (unsigned int  )9;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tlBcTransLog.bc_settle_in);
        sqlstm.sqhstl[20] = (unsigned int  )2;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tlBcTransLog.fld32_ins_id_cd);
        sqlstm.sqhstl[21] = (unsigned int  )14;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tlBcTransLog.fwd_ins_id_cd);
        sqlstm.sqhstl[22] = (unsigned int  )14;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tlBcTransLog.rcv_ins_id_cd);
        sqlstm.sqhstl[23] = (unsigned int  )14;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tlBcTransLog.msg_tp);
        sqlstm.sqhstl[24] = (unsigned int  )5;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tlBcTransLog.pri_acct_no);
        sqlstm.sqhstl[25] = (unsigned int  )22;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tlBcTransLog.proc_cd);
        sqlstm.sqhstl[26] = (unsigned int  )7;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tlBcTransLog.trans_at);
        sqlstm.sqhstl[27] = (unsigned int  )13;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tlBcTransLog.transmsn_dt_tm);
        sqlstm.sqhstl[28] = (unsigned int  )11;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tlBcTransLog.sys_tra_no);
        sqlstm.sqhstl[29] = (unsigned int  )7;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tlBcTransLog.loc_trans_tm);
        sqlstm.sqhstl[30] = (unsigned int  )7;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)(tlBcTransLog.loc_trans_dt);
        sqlstm.sqhstl[31] = (unsigned int  )5;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqhstv[32] = (         void  *)(tlBcTransLog.mchnt_tp);
        sqlstm.sqhstl[32] = (unsigned int  )5;
        sqlstm.sqhsts[32] = (         int  )0;
        sqlstm.sqindv[32] = (         void  *)0;
        sqlstm.sqinds[32] = (         int  )0;
        sqlstm.sqharm[32] = (unsigned int  )0;
        sqlstm.sqadto[32] = (unsigned short )0;
        sqlstm.sqtdso[32] = (unsigned short )0;
        sqlstm.sqhstv[33] = (         void  *)(tlBcTransLog.pos_entry_md_cd);
        sqlstm.sqhstl[33] = (unsigned int  )4;
        sqlstm.sqhsts[33] = (         int  )0;
        sqlstm.sqindv[33] = (         void  *)0;
        sqlstm.sqinds[33] = (         int  )0;
        sqlstm.sqharm[33] = (unsigned int  )0;
        sqlstm.sqadto[33] = (unsigned short )0;
        sqlstm.sqtdso[33] = (unsigned short )0;
        sqlstm.sqhstv[34] = (         void  *)(tlBcTransLog.pos_cond_cd);
        sqlstm.sqhstl[34] = (unsigned int  )3;
        sqlstm.sqhsts[34] = (         int  )0;
        sqlstm.sqindv[34] = (         void  *)0;
        sqlstm.sqinds[34] = (         int  )0;
        sqlstm.sqharm[34] = (unsigned int  )0;
        sqlstm.sqadto[34] = (unsigned short )0;
        sqlstm.sqtdso[34] = (unsigned short )0;
        sqlstm.sqhstv[35] = (         void  *)(tlBcTransLog.retri_ref_no);
        sqlstm.sqhstl[35] = (unsigned int  )13;
        sqlstm.sqhsts[35] = (         int  )0;
        sqlstm.sqindv[35] = (         void  *)0;
        sqlstm.sqinds[35] = (         int  )0;
        sqlstm.sqharm[35] = (unsigned int  )0;
        sqlstm.sqadto[35] = (unsigned short )0;
        sqlstm.sqtdso[35] = (unsigned short )0;
        sqlstm.sqhstv[36] = (         void  *)(tlBcTransLog.auth_id_resp_cd);
        sqlstm.sqhstl[36] = (unsigned int  )7;
        sqlstm.sqhsts[36] = (         int  )0;
        sqlstm.sqindv[36] = (         void  *)0;
        sqlstm.sqinds[36] = (         int  )0;
        sqlstm.sqharm[36] = (unsigned int  )0;
        sqlstm.sqadto[36] = (unsigned short )0;
        sqlstm.sqtdso[36] = (unsigned short )0;
        sqlstm.sqhstv[37] = (         void  *)(tlBcTransLog.resp_cd);
        sqlstm.sqhstl[37] = (unsigned int  )3;
        sqlstm.sqhsts[37] = (         int  )0;
        sqlstm.sqindv[37] = (         void  *)0;
        sqlstm.sqinds[37] = (         int  )0;
        sqlstm.sqharm[37] = (unsigned int  )0;
        sqlstm.sqadto[37] = (unsigned short )0;
        sqlstm.sqtdso[37] = (unsigned short )0;
        sqlstm.sqhstv[38] = (         void  *)(tlBcTransLog.term_id);
        sqlstm.sqhstl[38] = (unsigned int  )9;
        sqlstm.sqhsts[38] = (         int  )0;
        sqlstm.sqindv[38] = (         void  *)0;
        sqlstm.sqinds[38] = (         int  )0;
        sqlstm.sqharm[38] = (unsigned int  )0;
        sqlstm.sqadto[38] = (unsigned short )0;
        sqlstm.sqtdso[38] = (unsigned short )0;
        sqlstm.sqhstv[39] = (         void  *)(tlBcTransLog.mchnt_cd);
        sqlstm.sqhstl[39] = (unsigned int  )16;
        sqlstm.sqhsts[39] = (         int  )0;
        sqlstm.sqindv[39] = (         void  *)0;
        sqlstm.sqinds[39] = (         int  )0;
        sqlstm.sqharm[39] = (unsigned int  )0;
        sqlstm.sqadto[39] = (unsigned short )0;
        sqlstm.sqtdso[39] = (unsigned short )0;
        sqlstm.sqhstv[40] = (         void  *)(tlBcTransLog.area_cd);
        sqlstm.sqhstl[40] = (unsigned int  )7;
        sqlstm.sqhsts[40] = (         int  )0;
        sqlstm.sqindv[40] = (         void  *)0;
        sqlstm.sqinds[40] = (         int  )0;
        sqlstm.sqharm[40] = (unsigned int  )0;
        sqlstm.sqadto[40] = (unsigned short )0;
        sqlstm.sqtdso[40] = (unsigned short )0;
        sqlstm.sqhstv[41] = (         void  *)(tlBcTransLog.trans_curr_cd);
        sqlstm.sqhstl[41] = (unsigned int  )4;
        sqlstm.sqhsts[41] = (         int  )0;
        sqlstm.sqindv[41] = (         void  *)0;
        sqlstm.sqinds[41] = (         int  )0;
        sqlstm.sqharm[41] = (unsigned int  )0;
        sqlstm.sqadto[41] = (unsigned short )0;
        sqlstm.sqtdso[41] = (unsigned short )0;
        sqlstm.sqhstv[42] = (         void  *)(tlBcTransLog.flag_1);
        sqlstm.sqhstl[42] = (unsigned int  )2;
        sqlstm.sqhsts[42] = (         int  )0;
        sqlstm.sqindv[42] = (         void  *)0;
        sqlstm.sqinds[42] = (         int  )0;
        sqlstm.sqharm[42] = (unsigned int  )0;
        sqlstm.sqadto[42] = (unsigned short )0;
        sqlstm.sqtdso[42] = (unsigned short )0;
        sqlstm.sqhstv[43] = (         void  *)(tlBcTransLog.flag_2);
        sqlstm.sqhstl[43] = (unsigned int  )2;
        sqlstm.sqhsts[43] = (         int  )0;
        sqlstm.sqindv[43] = (         void  *)0;
        sqlstm.sqinds[43] = (         int  )0;
        sqlstm.sqharm[43] = (unsigned int  )0;
        sqlstm.sqadto[43] = (unsigned short )0;
        sqlstm.sqtdso[43] = (unsigned short )0;
        sqlstm.sqhstv[44] = (         void  *)(tlBcTransLog.flag_3);
        sqlstm.sqhstl[44] = (unsigned int  )2;
        sqlstm.sqhsts[44] = (         int  )0;
        sqlstm.sqindv[44] = (         void  *)0;
        sqlstm.sqinds[44] = (         int  )0;
        sqlstm.sqharm[44] = (unsigned int  )0;
        sqlstm.sqadto[44] = (unsigned short )0;
        sqlstm.sqtdso[44] = (unsigned short )0;
        sqlstm.sqhstv[45] = (         void  *)(tlBcTransLog.flag_4);
        sqlstm.sqhstl[45] = (unsigned int  )2;
        sqlstm.sqhsts[45] = (         int  )0;
        sqlstm.sqindv[45] = (         void  *)0;
        sqlstm.sqinds[45] = (         int  )0;
        sqlstm.sqharm[45] = (unsigned int  )0;
        sqlstm.sqadto[45] = (unsigned short )0;
        sqlstm.sqtdso[45] = (unsigned short )0;
        sqlstm.sqhstv[46] = (         void  *)(tlBcTransLog.flag_5);
        sqlstm.sqhstl[46] = (unsigned int  )2;
        sqlstm.sqhsts[46] = (         int  )0;
        sqlstm.sqindv[46] = (         void  *)0;
        sqlstm.sqinds[46] = (         int  )0;
        sqlstm.sqharm[46] = (unsigned int  )0;
        sqlstm.sqadto[46] = (unsigned short )0;
        sqlstm.sqtdso[46] = (unsigned short )0;
        sqlstm.sqhstv[47] = (         void  *)(tlBcTransLog.flag_6);
        sqlstm.sqhstl[47] = (unsigned int  )2;
        sqlstm.sqhsts[47] = (         int  )0;
        sqlstm.sqindv[47] = (         void  *)0;
        sqlstm.sqinds[47] = (         int  )0;
        sqlstm.sqharm[47] = (unsigned int  )0;
        sqlstm.sqadto[47] = (unsigned short )0;
        sqlstm.sqtdso[47] = (unsigned short )0;
        sqlstm.sqhstv[48] = (         void  *)(tlBcTransLog.flag_7);
        sqlstm.sqhstl[48] = (unsigned int  )2;
        sqlstm.sqhsts[48] = (         int  )0;
        sqlstm.sqindv[48] = (         void  *)0;
        sqlstm.sqinds[48] = (         int  )0;
        sqlstm.sqharm[48] = (unsigned int  )0;
        sqlstm.sqadto[48] = (unsigned short )0;
        sqlstm.sqtdso[48] = (unsigned short )0;
        sqlstm.sqhstv[49] = (         void  *)(tlBcTransLog.fld_5);
        sqlstm.sqhstl[49] = (unsigned int  )13;
        sqlstm.sqhsts[49] = (         int  )0;
        sqlstm.sqindv[49] = (         void  *)0;
        sqlstm.sqinds[49] = (         int  )0;
        sqlstm.sqharm[49] = (unsigned int  )0;
        sqlstm.sqadto[49] = (unsigned short )0;
        sqlstm.sqtdso[49] = (unsigned short )0;
        sqlstm.sqhstv[50] = (         void  *)(tlBcTransLog.fld_8);
        sqlstm.sqhstl[50] = (unsigned int  )9;
        sqlstm.sqhsts[50] = (         int  )0;
        sqlstm.sqindv[50] = (         void  *)0;
        sqlstm.sqinds[50] = (         int  )0;
        sqlstm.sqharm[50] = (unsigned int  )0;
        sqlstm.sqadto[50] = (unsigned short )0;
        sqlstm.sqtdso[50] = (unsigned short )0;
        sqlstm.sqhstv[51] = (         void  *)(tlBcTransLog.fld_28);
        sqlstm.sqhstl[51] = (unsigned int  )7;
        sqlstm.sqhsts[51] = (         int  )0;
        sqlstm.sqindv[51] = (         void  *)0;
        sqlstm.sqinds[51] = (         int  )0;
        sqlstm.sqharm[51] = (unsigned int  )0;
        sqlstm.sqadto[51] = (unsigned short )0;
        sqlstm.sqtdso[51] = (unsigned short )0;
        sqlstm.sqhstv[52] = (         void  *)(tlBcTransLog.fld_34);
        sqlstm.sqhstl[52] = (unsigned int  )29;
        sqlstm.sqhsts[52] = (         int  )0;
        sqlstm.sqindv[52] = (         void  *)0;
        sqlstm.sqinds[52] = (         int  )0;
        sqlstm.sqharm[52] = (unsigned int  )0;
        sqlstm.sqadto[52] = (unsigned short )0;
        sqlstm.sqtdso[52] = (unsigned short )0;
        sqlstm.sqhstv[53] = (         void  *)(tlBcTransLog.fld_44);
        sqlstm.sqhstl[53] = (unsigned int  )28;
        sqlstm.sqhsts[53] = (         int  )0;
        sqlstm.sqindv[53] = (         void  *)0;
        sqlstm.sqinds[53] = (         int  )0;
        sqlstm.sqharm[53] = (unsigned int  )0;
        sqlstm.sqadto[53] = (unsigned short )0;
        sqlstm.sqtdso[53] = (unsigned short )0;
        sqlstm.sqhstv[54] = (         void  *)(tlBcTransLog.fld_45);
        sqlstm.sqhstl[54] = (unsigned int  )77;
        sqlstm.sqhsts[54] = (         int  )0;
        sqlstm.sqindv[54] = (         void  *)0;
        sqlstm.sqinds[54] = (         int  )0;
        sqlstm.sqharm[54] = (unsigned int  )0;
        sqlstm.sqadto[54] = (unsigned short )0;
        sqlstm.sqtdso[54] = (unsigned short )0;
        sqlstm.sqhstv[55] = (         void  *)(tlBcTransLog.fld_46);
        sqlstm.sqhstl[55] = (unsigned int  )31;
        sqlstm.sqhsts[55] = (         int  )0;
        sqlstm.sqindv[55] = (         void  *)0;
        sqlstm.sqinds[55] = (         int  )0;
        sqlstm.sqharm[55] = (unsigned int  )0;
        sqlstm.sqadto[55] = (unsigned short )0;
        sqlstm.sqtdso[55] = (unsigned short )0;
        sqlstm.sqhstv[56] = (         void  *)(tlBcTransLog.fld_48);
        sqlstm.sqhstl[56] = (unsigned int  )191;
        sqlstm.sqhsts[56] = (         int  )0;
        sqlstm.sqindv[56] = (         void  *)0;
        sqlstm.sqinds[56] = (         int  )0;
        sqlstm.sqharm[56] = (unsigned int  )0;
        sqlstm.sqadto[56] = (unsigned short )0;
        sqlstm.sqtdso[56] = (unsigned short )0;
        sqlstm.sqhstv[57] = (         void  *)(tlBcTransLog.fld_61);
        sqlstm.sqhstl[57] = (unsigned int  )31;
        sqlstm.sqhsts[57] = (         int  )0;
        sqlstm.sqindv[57] = (         void  *)0;
        sqlstm.sqinds[57] = (         int  )0;
        sqlstm.sqharm[57] = (unsigned int  )0;
        sqlstm.sqadto[57] = (unsigned short )0;
        sqlstm.sqtdso[57] = (unsigned short )0;
        sqlstm.sqhstv[58] = (         void  *)(tlBcTransLog.center_sys_tra_no);
        sqlstm.sqhstl[58] = (unsigned int  )7;
        sqlstm.sqhsts[58] = (         int  )0;
        sqlstm.sqindv[58] = (         void  *)0;
        sqlstm.sqinds[58] = (         int  )0;
        sqlstm.sqharm[58] = (unsigned int  )0;
        sqlstm.sqadto[58] = (unsigned short )0;
        sqlstm.sqtdso[58] = (unsigned short )0;
        sqlstm.sqhstv[59] = (         void  *)(tlBcTransLog.center_term_id);
        sqlstm.sqhstl[59] = (unsigned int  )9;
        sqlstm.sqhsts[59] = (         int  )0;
        sqlstm.sqindv[59] = (         void  *)0;
        sqlstm.sqinds[59] = (         int  )0;
        sqlstm.sqharm[59] = (unsigned int  )0;
        sqlstm.sqadto[59] = (unsigned short )0;
        sqlstm.sqtdso[59] = (unsigned short )0;
        sqlstm.sqhstv[60] = (         void  *)(tlBcTransLog.center_mchnt_cd);
        sqlstm.sqhstl[60] = (unsigned int  )21;
        sqlstm.sqhsts[60] = (         int  )0;
        sqlstm.sqindv[60] = (         void  *)0;
        sqlstm.sqinds[60] = (         int  )0;
        sqlstm.sqharm[60] = (unsigned int  )0;
        sqlstm.sqadto[60] = (unsigned short )0;
        sqlstm.sqtdso[60] = (unsigned short )0;
        sqlstm.sqhstv[61] = (         void  *)(tlBcTransLog.bill_id_rcv);
        sqlstm.sqhstl[61] = (unsigned int  )22;
        sqlstm.sqhsts[61] = (         int  )0;
        sqlstm.sqindv[61] = (         void  *)0;
        sqlstm.sqinds[61] = (         int  )0;
        sqlstm.sqharm[61] = (unsigned int  )0;
        sqlstm.sqadto[61] = (unsigned short )0;
        sqlstm.sqtdso[61] = (unsigned short )0;
        sqlstm.sqhstv[62] = (         void  *)(tlBcTransLog.card_attr);
        sqlstm.sqhstl[62] = (unsigned int  )3;
        sqlstm.sqhsts[62] = (         int  )0;
        sqlstm.sqindv[62] = (         void  *)0;
        sqlstm.sqinds[62] = (         int  )0;
        sqlstm.sqharm[62] = (unsigned int  )0;
        sqlstm.sqadto[62] = (unsigned short )0;
        sqlstm.sqtdso[62] = (unsigned short )0;
        sqlstm.sqhstv[63] = (         void  *)(tlBcTransLog.iss_ins_id_cd);
        sqlstm.sqhstl[63] = (unsigned int  )9;
        sqlstm.sqhsts[63] = (         int  )0;
        sqlstm.sqindv[63] = (         void  *)0;
        sqlstm.sqinds[63] = (         int  )0;
        sqlstm.sqharm[63] = (unsigned int  )0;
        sqlstm.sqadto[63] = (unsigned short )0;
        sqlstm.sqtdso[63] = (unsigned short )0;
        sqlstm.sqhstv[64] = (         void  *)(tlBcTransLog.pname);
        sqlstm.sqhstl[64] = (unsigned int  )211;
        sqlstm.sqhsts[64] = (         int  )0;
        sqlstm.sqindv[64] = (         void  *)0;
        sqlstm.sqinds[64] = (         int  )0;
        sqlstm.sqharm[64] = (unsigned int  )0;
        sqlstm.sqadto[64] = (unsigned short )0;
        sqlstm.sqtdso[64] = (unsigned short )0;
        sqlstm.sqhstv[65] = (         void  *)(tlBcTransLog.encrypt_info);
        sqlstm.sqhstl[65] = (unsigned int  )49;
        sqlstm.sqhsts[65] = (         int  )0;
        sqlstm.sqindv[65] = (         void  *)0;
        sqlstm.sqinds[65] = (         int  )0;
        sqlstm.sqharm[65] = (unsigned int  )0;
        sqlstm.sqadto[65] = (unsigned short )0;
        sqlstm.sqtdso[65] = (unsigned short )0;
        sqlstm.sqhstv[66] = (         void  *)(tlBcTransLog.id_type);
        sqlstm.sqhstl[66] = (unsigned int  )3;
        sqlstm.sqhsts[66] = (         int  )0;
        sqlstm.sqindv[66] = (         void  *)0;
        sqlstm.sqinds[66] = (         int  )0;
        sqlstm.sqharm[66] = (unsigned int  )0;
        sqlstm.sqadto[66] = (unsigned short )0;
        sqlstm.sqtdso[66] = (unsigned short )0;
        sqlstm.sqhstv[67] = (         void  *)(tlBcTransLog.originaDataElements);
        sqlstm.sqhstl[67] = (unsigned int  )44;
        sqlstm.sqhsts[67] = (         int  )0;
        sqlstm.sqindv[67] = (         void  *)0;
        sqlstm.sqinds[67] = (         int  )0;
        sqlstm.sqharm[67] = (unsigned int  )0;
        sqlstm.sqadto[67] = (unsigned short )0;
        sqlstm.sqtdso[67] = (unsigned short )0;
        sqlstm.sqhstv[68] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[68] = (unsigned int  )5;
        sqlstm.sqhsts[68] = (         int  )0;
        sqlstm.sqindv[68] = (         void  *)0;
        sqlstm.sqinds[68] = (         int  )0;
        sqlstm.sqharm[68] = (unsigned int  )0;
        sqlstm.sqadto[68] = (unsigned short )0;
        sqlstm.sqtdso[68] = (unsigned short )0;
        sqlstm.sqhstv[69] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[69] = (unsigned int  )21;
        sqlstm.sqhsts[69] = (         int  )0;
        sqlstm.sqindv[69] = (         void  *)0;
        sqlstm.sqinds[69] = (         int  )0;
        sqlstm.sqharm[69] = (unsigned int  )0;
        sqlstm.sqadto[69] = (unsigned short )0;
        sqlstm.sqtdso[69] = (unsigned short )0;
        sqlstm.sqhstv[70] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[70] = (unsigned int  )7;
        sqlstm.sqhsts[70] = (         int  )0;
        sqlstm.sqindv[70] = (         void  *)0;
        sqlstm.sqinds[70] = (         int  )0;
        sqlstm.sqharm[70] = (unsigned int  )0;
        sqlstm.sqadto[70] = (unsigned short )0;
        sqlstm.sqtdso[70] = (unsigned short )0;
        sqlstm.sqhstv[71] = (         void  *)term_id;
        sqlstm.sqhstl[71] = (unsigned int  )9;
        sqlstm.sqhsts[71] = (         int  )0;
        sqlstm.sqindv[71] = (         void  *)0;
        sqlstm.sqinds[71] = (         int  )0;
        sqlstm.sqharm[71] = (unsigned int  )0;
        sqlstm.sqadto[71] = (unsigned short )0;
        sqlstm.sqtdso[71] = (unsigned short )0;
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
            memcpy(p_tlBcTransLog, &tlBcTransLog, sizeof(tl_bc_trans_log_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<FILE:%s,LINE:%d><TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT1>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",__FILE__,__LINE__, loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<FILE:%s,LINE:%d><TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT1>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",__FILE__,__LINE__, loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )308;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;


    case GLB_DB_SELECT2:
    	
        strcpy_safe(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, sizeof(loc_trans_dt)-1);
        rtrim_null(loc_trans_dt,' ');
        
        strcpy_safe(mchnt_cd, p_tlBcTransLog->mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');

				 strcpy_safe(sys_tra_no, p_tlBcTransLog->sys_tra_no, sizeof(sys_tra_no)-1);
        rtrim_null(sys_tra_no,' ');
        

        /* EXEC SQL SELECT 
        sys_dt_tm            ,                
        settle_dt            ,
        trans_flag           ,
        is_over              ,
        lock_tm              ,
        flag_lock            ,
        flag_verifi          ,
        flag_rcv             ,
        flag_pay             ,
        flag_reversal        ,
        resp_cd_verifi       ,
        resp_cd_rcv          ,
        resp_cd_pay          ,
        resp_cd_reversal     ,
        trans_chnl           ,
        packet_type          ,
        line_nm_verifi       ,
        line_nm_rcv          ,
        line_nm_pay          ,
        reversal_id          ,
        bc_settle_in         ,
        fld32_ins_id_cd      ,
        fwd_ins_id_cd        ,
        rcv_ins_id_cd        ,
        msg_tp               ,
        pri_acct_no          ,
        proc_cd              ,
        trans_at             ,
        transmsn_dt_tm       ,
        sys_tra_no           ,
        loc_trans_tm         ,
        loc_trans_dt         ,
        mchnt_tp             ,
        pos_entry_md_cd      ,
        pos_cond_cd          ,
        retri_ref_no         ,
        auth_id_resp_cd      ,
        resp_cd              ,
        term_id              ,
        mchnt_cd             ,
        area_cd              ,
        trans_curr_cd        ,
        flag_1               ,
        flag_2               ,
        flag_3               ,
        flag_4               ,
        flag_5               ,
        flag_6               ,
        flag_7               ,
        fld_5                ,
        fld_8                ,
        fld_28               ,
        fld_34               ,
        fld_44               ,
        fld_45               ,
        fld_46               ,
        fld_48               ,
        fld_61               ,
        center_sys_tra_no    ,
        center_term_id       ,
        center_mchnt_cd      ,
        bill_id_rcv          ,
        card_attr            ,
        iss_ins_id_cd        ,
        pname								 ,
        encrypt_info         ,
        id_type 			,
		orig_data_elemts        
        INTO 
           :tlBcTransLog.sys_dt_tm            ,   
           :tlBcTransLog.settle_dt            ,
           :tlBcTransLog.trans_flag           ,
           :tlBcTransLog.is_over              ,
           :tlBcTransLog.lock_tm              ,
           :tlBcTransLog.flag_lock            ,
           :tlBcTransLog.flag_verifi          ,
           :tlBcTransLog.flag_rcv             ,
           :tlBcTransLog.flag_pay             ,
           :tlBcTransLog.flag_reversal        ,
           :tlBcTransLog.resp_cd_verifi       ,
           :tlBcTransLog.resp_cd_rcv          ,
           :tlBcTransLog.resp_cd_pay          ,
           :tlBcTransLog.resp_cd_reversal     ,
           :tlBcTransLog.trans_chnl           ,
           :tlBcTransLog.packet_type          ,
           :tlBcTransLog.line_nm_verifi       ,
           :tlBcTransLog.line_nm_rcv          ,
           :tlBcTransLog.line_nm_pay          ,
           :tlBcTransLog.reversal_id          ,
           :tlBcTransLog.bc_settle_in         ,
           :tlBcTransLog.fld32_ins_id_cd      ,
           :tlBcTransLog.fwd_ins_id_cd        ,
           :tlBcTransLog.rcv_ins_id_cd        ,
           :tlBcTransLog.msg_tp               ,
           :tlBcTransLog.pri_acct_no          ,
           :tlBcTransLog.proc_cd              ,
           :tlBcTransLog.trans_at             ,
           :tlBcTransLog.transmsn_dt_tm       ,
           :tlBcTransLog.sys_tra_no           ,
           :tlBcTransLog.loc_trans_tm         ,
           :tlBcTransLog.loc_trans_dt         ,
           :tlBcTransLog.mchnt_tp             ,
           :tlBcTransLog.pos_entry_md_cd      ,
           :tlBcTransLog.pos_cond_cd          ,
           :tlBcTransLog.retri_ref_no         ,
           :tlBcTransLog.auth_id_resp_cd      ,
           :tlBcTransLog.resp_cd              ,
           :tlBcTransLog.term_id              ,
           :tlBcTransLog.mchnt_cd             ,
           :tlBcTransLog.area_cd              ,
           :tlBcTransLog.trans_curr_cd        ,
           :tlBcTransLog.flag_1               ,
           :tlBcTransLog.flag_2               ,
           :tlBcTransLog.flag_3               ,
           :tlBcTransLog.flag_4               ,
           :tlBcTransLog.flag_5               ,
           :tlBcTransLog.flag_6               ,
           :tlBcTransLog.flag_7               ,
           :tlBcTransLog.fld_5                ,
           :tlBcTransLog.fld_8                ,
           :tlBcTransLog.fld_28               ,
           :tlBcTransLog.fld_34               ,
           :tlBcTransLog.fld_44               ,
           :tlBcTransLog.fld_45               ,
           :tlBcTransLog.fld_46               ,
           :tlBcTransLog.fld_48               ,
           :tlBcTransLog.fld_61               ,
           :tlBcTransLog.center_sys_tra_no    ,
           :tlBcTransLog.center_term_id       ,
           :tlBcTransLog.center_mchnt_cd      ,
           :tlBcTransLog.bill_id_rcv          ,
           :tlBcTransLog.card_attr            ,
           :tlBcTransLog.iss_ins_id_cd        ,
         	 :tlBcTransLog.pname						    ,
         	 :tlBcTransLog.encrypt_info					,
           :tlBcTransLog.id_type	,
           :tlBcTransLog.originaDataElements
        FROM tl_bc_trans_log
        WHERE loc_trans_dt=:loc_trans_dt
				AND mchnt_cd=:mchnt_cd
				AND sys_tra_no=:sys_tra_no
				AND rownum<=1
				; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlbuft((void **)0,
          "select sys_dt_tm ,settle_dt ,trans_flag ,is_over ,lock_tm ,flag_l\
ock ,flag_verifi ,flag_rcv ,flag_pay ,flag_reversal ,resp_cd_verifi ,resp_cd\
_rcv ,resp_cd_pay ,resp_cd_reversal ,trans_chnl ,packet_type ,line_nm_verifi\
 ,line_nm_rcv ,line_nm_pay ,reversal_id ,bc_settle_in ,fld32_ins_id_cd ,fwd_\
ins_id_cd ,rcv_ins_id_cd ,msg_tp ,pri_acct_no ,proc_cd ,trans_at ,transmsn_d\
t_tm ,sys_tra_no ,loc_trans_tm ,loc_trans_dt ,mchnt_tp ,pos_entry_md_cd ,pos\
_cond_cd ,retri_ref_no ,auth_id_resp_cd ,resp_cd ,term_id ,mchnt_cd ,area_cd\
 ,trans_curr_cd ,flag_1 ,flag_2 ,flag_3 ,flag_4 ,flag_5 ,flag_6 ,flag_7 ,fld\
_5 ,fld_8 ,fld_28 ,fld_34 ,fld_44 ,fld_45 ,fld_46 ,fld_48 ,fld_61 ,center_sy\
s_tra_no ,center_term_id ,center_mchnt_cd ,bill_id_rcv ,card_attr ,iss_ins_i\
d_cd ,pname ,encrypt_info ,id_type ,orig_data_elemts into :b0,:b1,:b2,:b3,:b\
4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15,:b16,:b17,:b18,:b19,:b20\
,:b21,:b22,:b23,:b24,:b25,:b26,:b27,:b28,:b29,:b30,:b31,:b32,:b33,:b34,:b35,\
:b36,:b37,:b38,:b39,:b40,:b41,:b42,:b43,:b44,:b");
        sqlstm.stmt = "45,:b46,:b47,:b48,:b49,:b50,:b51,:b52,:b53,:b54,:b55\
,:b56,:b57,:b58,:b59,:b60,:b61,:b62,:b63,:b64,:b65,:b66,:b67  from tl_bc_tran\
s_log where (((loc_trans_dt=:b68 and mchnt_cd=:b69) and sys_tra_no=:b70) and \
rownum<=1)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )323;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcTransLog.sys_dt_tm);
        sqlstm.sqhstl[0] = (unsigned int  )15;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.settle_dt);
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.trans_flag);
        sqlstm.sqhstl[2] = (unsigned int  )5;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.is_over);
        sqlstm.sqhstl[3] = (unsigned int  )2;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.lock_tm);
        sqlstm.sqhstl[4] = (unsigned int  )11;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.flag_lock);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcTransLog.flag_verifi);
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcTransLog.flag_rcv);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcTransLog.flag_pay);
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcTransLog.flag_reversal);
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcTransLog.resp_cd_verifi);
        sqlstm.sqhstl[10] = (unsigned int  )7;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcTransLog.resp_cd_rcv);
        sqlstm.sqhstl[11] = (unsigned int  )7;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcTransLog.resp_cd_pay);
        sqlstm.sqhstl[12] = (unsigned int  )7;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcTransLog.resp_cd_reversal);
        sqlstm.sqhstl[13] = (unsigned int  )7;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcTransLog.trans_chnl);
        sqlstm.sqhstl[14] = (unsigned int  )31;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcTransLog.packet_type);
        sqlstm.sqhstl[15] = (unsigned int  )5;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tlBcTransLog.line_nm_verifi);
        sqlstm.sqhstl[16] = (unsigned int  )5;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tlBcTransLog.line_nm_rcv);
        sqlstm.sqhstl[17] = (unsigned int  )5;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tlBcTransLog.line_nm_pay);
        sqlstm.sqhstl[18] = (unsigned int  )5;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tlBcTransLog.reversal_id);
        sqlstm.sqhstl[19] = (unsigned int  )9;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tlBcTransLog.bc_settle_in);
        sqlstm.sqhstl[20] = (unsigned int  )2;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tlBcTransLog.fld32_ins_id_cd);
        sqlstm.sqhstl[21] = (unsigned int  )14;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tlBcTransLog.fwd_ins_id_cd);
        sqlstm.sqhstl[22] = (unsigned int  )14;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tlBcTransLog.rcv_ins_id_cd);
        sqlstm.sqhstl[23] = (unsigned int  )14;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tlBcTransLog.msg_tp);
        sqlstm.sqhstl[24] = (unsigned int  )5;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tlBcTransLog.pri_acct_no);
        sqlstm.sqhstl[25] = (unsigned int  )22;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tlBcTransLog.proc_cd);
        sqlstm.sqhstl[26] = (unsigned int  )7;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tlBcTransLog.trans_at);
        sqlstm.sqhstl[27] = (unsigned int  )13;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tlBcTransLog.transmsn_dt_tm);
        sqlstm.sqhstl[28] = (unsigned int  )11;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tlBcTransLog.sys_tra_no);
        sqlstm.sqhstl[29] = (unsigned int  )7;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tlBcTransLog.loc_trans_tm);
        sqlstm.sqhstl[30] = (unsigned int  )7;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)(tlBcTransLog.loc_trans_dt);
        sqlstm.sqhstl[31] = (unsigned int  )5;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqhstv[32] = (         void  *)(tlBcTransLog.mchnt_tp);
        sqlstm.sqhstl[32] = (unsigned int  )5;
        sqlstm.sqhsts[32] = (         int  )0;
        sqlstm.sqindv[32] = (         void  *)0;
        sqlstm.sqinds[32] = (         int  )0;
        sqlstm.sqharm[32] = (unsigned int  )0;
        sqlstm.sqadto[32] = (unsigned short )0;
        sqlstm.sqtdso[32] = (unsigned short )0;
        sqlstm.sqhstv[33] = (         void  *)(tlBcTransLog.pos_entry_md_cd);
        sqlstm.sqhstl[33] = (unsigned int  )4;
        sqlstm.sqhsts[33] = (         int  )0;
        sqlstm.sqindv[33] = (         void  *)0;
        sqlstm.sqinds[33] = (         int  )0;
        sqlstm.sqharm[33] = (unsigned int  )0;
        sqlstm.sqadto[33] = (unsigned short )0;
        sqlstm.sqtdso[33] = (unsigned short )0;
        sqlstm.sqhstv[34] = (         void  *)(tlBcTransLog.pos_cond_cd);
        sqlstm.sqhstl[34] = (unsigned int  )3;
        sqlstm.sqhsts[34] = (         int  )0;
        sqlstm.sqindv[34] = (         void  *)0;
        sqlstm.sqinds[34] = (         int  )0;
        sqlstm.sqharm[34] = (unsigned int  )0;
        sqlstm.sqadto[34] = (unsigned short )0;
        sqlstm.sqtdso[34] = (unsigned short )0;
        sqlstm.sqhstv[35] = (         void  *)(tlBcTransLog.retri_ref_no);
        sqlstm.sqhstl[35] = (unsigned int  )13;
        sqlstm.sqhsts[35] = (         int  )0;
        sqlstm.sqindv[35] = (         void  *)0;
        sqlstm.sqinds[35] = (         int  )0;
        sqlstm.sqharm[35] = (unsigned int  )0;
        sqlstm.sqadto[35] = (unsigned short )0;
        sqlstm.sqtdso[35] = (unsigned short )0;
        sqlstm.sqhstv[36] = (         void  *)(tlBcTransLog.auth_id_resp_cd);
        sqlstm.sqhstl[36] = (unsigned int  )7;
        sqlstm.sqhsts[36] = (         int  )0;
        sqlstm.sqindv[36] = (         void  *)0;
        sqlstm.sqinds[36] = (         int  )0;
        sqlstm.sqharm[36] = (unsigned int  )0;
        sqlstm.sqadto[36] = (unsigned short )0;
        sqlstm.sqtdso[36] = (unsigned short )0;
        sqlstm.sqhstv[37] = (         void  *)(tlBcTransLog.resp_cd);
        sqlstm.sqhstl[37] = (unsigned int  )3;
        sqlstm.sqhsts[37] = (         int  )0;
        sqlstm.sqindv[37] = (         void  *)0;
        sqlstm.sqinds[37] = (         int  )0;
        sqlstm.sqharm[37] = (unsigned int  )0;
        sqlstm.sqadto[37] = (unsigned short )0;
        sqlstm.sqtdso[37] = (unsigned short )0;
        sqlstm.sqhstv[38] = (         void  *)(tlBcTransLog.term_id);
        sqlstm.sqhstl[38] = (unsigned int  )9;
        sqlstm.sqhsts[38] = (         int  )0;
        sqlstm.sqindv[38] = (         void  *)0;
        sqlstm.sqinds[38] = (         int  )0;
        sqlstm.sqharm[38] = (unsigned int  )0;
        sqlstm.sqadto[38] = (unsigned short )0;
        sqlstm.sqtdso[38] = (unsigned short )0;
        sqlstm.sqhstv[39] = (         void  *)(tlBcTransLog.mchnt_cd);
        sqlstm.sqhstl[39] = (unsigned int  )16;
        sqlstm.sqhsts[39] = (         int  )0;
        sqlstm.sqindv[39] = (         void  *)0;
        sqlstm.sqinds[39] = (         int  )0;
        sqlstm.sqharm[39] = (unsigned int  )0;
        sqlstm.sqadto[39] = (unsigned short )0;
        sqlstm.sqtdso[39] = (unsigned short )0;
        sqlstm.sqhstv[40] = (         void  *)(tlBcTransLog.area_cd);
        sqlstm.sqhstl[40] = (unsigned int  )7;
        sqlstm.sqhsts[40] = (         int  )0;
        sqlstm.sqindv[40] = (         void  *)0;
        sqlstm.sqinds[40] = (         int  )0;
        sqlstm.sqharm[40] = (unsigned int  )0;
        sqlstm.sqadto[40] = (unsigned short )0;
        sqlstm.sqtdso[40] = (unsigned short )0;
        sqlstm.sqhstv[41] = (         void  *)(tlBcTransLog.trans_curr_cd);
        sqlstm.sqhstl[41] = (unsigned int  )4;
        sqlstm.sqhsts[41] = (         int  )0;
        sqlstm.sqindv[41] = (         void  *)0;
        sqlstm.sqinds[41] = (         int  )0;
        sqlstm.sqharm[41] = (unsigned int  )0;
        sqlstm.sqadto[41] = (unsigned short )0;
        sqlstm.sqtdso[41] = (unsigned short )0;
        sqlstm.sqhstv[42] = (         void  *)(tlBcTransLog.flag_1);
        sqlstm.sqhstl[42] = (unsigned int  )2;
        sqlstm.sqhsts[42] = (         int  )0;
        sqlstm.sqindv[42] = (         void  *)0;
        sqlstm.sqinds[42] = (         int  )0;
        sqlstm.sqharm[42] = (unsigned int  )0;
        sqlstm.sqadto[42] = (unsigned short )0;
        sqlstm.sqtdso[42] = (unsigned short )0;
        sqlstm.sqhstv[43] = (         void  *)(tlBcTransLog.flag_2);
        sqlstm.sqhstl[43] = (unsigned int  )2;
        sqlstm.sqhsts[43] = (         int  )0;
        sqlstm.sqindv[43] = (         void  *)0;
        sqlstm.sqinds[43] = (         int  )0;
        sqlstm.sqharm[43] = (unsigned int  )0;
        sqlstm.sqadto[43] = (unsigned short )0;
        sqlstm.sqtdso[43] = (unsigned short )0;
        sqlstm.sqhstv[44] = (         void  *)(tlBcTransLog.flag_3);
        sqlstm.sqhstl[44] = (unsigned int  )2;
        sqlstm.sqhsts[44] = (         int  )0;
        sqlstm.sqindv[44] = (         void  *)0;
        sqlstm.sqinds[44] = (         int  )0;
        sqlstm.sqharm[44] = (unsigned int  )0;
        sqlstm.sqadto[44] = (unsigned short )0;
        sqlstm.sqtdso[44] = (unsigned short )0;
        sqlstm.sqhstv[45] = (         void  *)(tlBcTransLog.flag_4);
        sqlstm.sqhstl[45] = (unsigned int  )2;
        sqlstm.sqhsts[45] = (         int  )0;
        sqlstm.sqindv[45] = (         void  *)0;
        sqlstm.sqinds[45] = (         int  )0;
        sqlstm.sqharm[45] = (unsigned int  )0;
        sqlstm.sqadto[45] = (unsigned short )0;
        sqlstm.sqtdso[45] = (unsigned short )0;
        sqlstm.sqhstv[46] = (         void  *)(tlBcTransLog.flag_5);
        sqlstm.sqhstl[46] = (unsigned int  )2;
        sqlstm.sqhsts[46] = (         int  )0;
        sqlstm.sqindv[46] = (         void  *)0;
        sqlstm.sqinds[46] = (         int  )0;
        sqlstm.sqharm[46] = (unsigned int  )0;
        sqlstm.sqadto[46] = (unsigned short )0;
        sqlstm.sqtdso[46] = (unsigned short )0;
        sqlstm.sqhstv[47] = (         void  *)(tlBcTransLog.flag_6);
        sqlstm.sqhstl[47] = (unsigned int  )2;
        sqlstm.sqhsts[47] = (         int  )0;
        sqlstm.sqindv[47] = (         void  *)0;
        sqlstm.sqinds[47] = (         int  )0;
        sqlstm.sqharm[47] = (unsigned int  )0;
        sqlstm.sqadto[47] = (unsigned short )0;
        sqlstm.sqtdso[47] = (unsigned short )0;
        sqlstm.sqhstv[48] = (         void  *)(tlBcTransLog.flag_7);
        sqlstm.sqhstl[48] = (unsigned int  )2;
        sqlstm.sqhsts[48] = (         int  )0;
        sqlstm.sqindv[48] = (         void  *)0;
        sqlstm.sqinds[48] = (         int  )0;
        sqlstm.sqharm[48] = (unsigned int  )0;
        sqlstm.sqadto[48] = (unsigned short )0;
        sqlstm.sqtdso[48] = (unsigned short )0;
        sqlstm.sqhstv[49] = (         void  *)(tlBcTransLog.fld_5);
        sqlstm.sqhstl[49] = (unsigned int  )13;
        sqlstm.sqhsts[49] = (         int  )0;
        sqlstm.sqindv[49] = (         void  *)0;
        sqlstm.sqinds[49] = (         int  )0;
        sqlstm.sqharm[49] = (unsigned int  )0;
        sqlstm.sqadto[49] = (unsigned short )0;
        sqlstm.sqtdso[49] = (unsigned short )0;
        sqlstm.sqhstv[50] = (         void  *)(tlBcTransLog.fld_8);
        sqlstm.sqhstl[50] = (unsigned int  )9;
        sqlstm.sqhsts[50] = (         int  )0;
        sqlstm.sqindv[50] = (         void  *)0;
        sqlstm.sqinds[50] = (         int  )0;
        sqlstm.sqharm[50] = (unsigned int  )0;
        sqlstm.sqadto[50] = (unsigned short )0;
        sqlstm.sqtdso[50] = (unsigned short )0;
        sqlstm.sqhstv[51] = (         void  *)(tlBcTransLog.fld_28);
        sqlstm.sqhstl[51] = (unsigned int  )7;
        sqlstm.sqhsts[51] = (         int  )0;
        sqlstm.sqindv[51] = (         void  *)0;
        sqlstm.sqinds[51] = (         int  )0;
        sqlstm.sqharm[51] = (unsigned int  )0;
        sqlstm.sqadto[51] = (unsigned short )0;
        sqlstm.sqtdso[51] = (unsigned short )0;
        sqlstm.sqhstv[52] = (         void  *)(tlBcTransLog.fld_34);
        sqlstm.sqhstl[52] = (unsigned int  )29;
        sqlstm.sqhsts[52] = (         int  )0;
        sqlstm.sqindv[52] = (         void  *)0;
        sqlstm.sqinds[52] = (         int  )0;
        sqlstm.sqharm[52] = (unsigned int  )0;
        sqlstm.sqadto[52] = (unsigned short )0;
        sqlstm.sqtdso[52] = (unsigned short )0;
        sqlstm.sqhstv[53] = (         void  *)(tlBcTransLog.fld_44);
        sqlstm.sqhstl[53] = (unsigned int  )28;
        sqlstm.sqhsts[53] = (         int  )0;
        sqlstm.sqindv[53] = (         void  *)0;
        sqlstm.sqinds[53] = (         int  )0;
        sqlstm.sqharm[53] = (unsigned int  )0;
        sqlstm.sqadto[53] = (unsigned short )0;
        sqlstm.sqtdso[53] = (unsigned short )0;
        sqlstm.sqhstv[54] = (         void  *)(tlBcTransLog.fld_45);
        sqlstm.sqhstl[54] = (unsigned int  )77;
        sqlstm.sqhsts[54] = (         int  )0;
        sqlstm.sqindv[54] = (         void  *)0;
        sqlstm.sqinds[54] = (         int  )0;
        sqlstm.sqharm[54] = (unsigned int  )0;
        sqlstm.sqadto[54] = (unsigned short )0;
        sqlstm.sqtdso[54] = (unsigned short )0;
        sqlstm.sqhstv[55] = (         void  *)(tlBcTransLog.fld_46);
        sqlstm.sqhstl[55] = (unsigned int  )31;
        sqlstm.sqhsts[55] = (         int  )0;
        sqlstm.sqindv[55] = (         void  *)0;
        sqlstm.sqinds[55] = (         int  )0;
        sqlstm.sqharm[55] = (unsigned int  )0;
        sqlstm.sqadto[55] = (unsigned short )0;
        sqlstm.sqtdso[55] = (unsigned short )0;
        sqlstm.sqhstv[56] = (         void  *)(tlBcTransLog.fld_48);
        sqlstm.sqhstl[56] = (unsigned int  )191;
        sqlstm.sqhsts[56] = (         int  )0;
        sqlstm.sqindv[56] = (         void  *)0;
        sqlstm.sqinds[56] = (         int  )0;
        sqlstm.sqharm[56] = (unsigned int  )0;
        sqlstm.sqadto[56] = (unsigned short )0;
        sqlstm.sqtdso[56] = (unsigned short )0;
        sqlstm.sqhstv[57] = (         void  *)(tlBcTransLog.fld_61);
        sqlstm.sqhstl[57] = (unsigned int  )31;
        sqlstm.sqhsts[57] = (         int  )0;
        sqlstm.sqindv[57] = (         void  *)0;
        sqlstm.sqinds[57] = (         int  )0;
        sqlstm.sqharm[57] = (unsigned int  )0;
        sqlstm.sqadto[57] = (unsigned short )0;
        sqlstm.sqtdso[57] = (unsigned short )0;
        sqlstm.sqhstv[58] = (         void  *)(tlBcTransLog.center_sys_tra_no);
        sqlstm.sqhstl[58] = (unsigned int  )7;
        sqlstm.sqhsts[58] = (         int  )0;
        sqlstm.sqindv[58] = (         void  *)0;
        sqlstm.sqinds[58] = (         int  )0;
        sqlstm.sqharm[58] = (unsigned int  )0;
        sqlstm.sqadto[58] = (unsigned short )0;
        sqlstm.sqtdso[58] = (unsigned short )0;
        sqlstm.sqhstv[59] = (         void  *)(tlBcTransLog.center_term_id);
        sqlstm.sqhstl[59] = (unsigned int  )9;
        sqlstm.sqhsts[59] = (         int  )0;
        sqlstm.sqindv[59] = (         void  *)0;
        sqlstm.sqinds[59] = (         int  )0;
        sqlstm.sqharm[59] = (unsigned int  )0;
        sqlstm.sqadto[59] = (unsigned short )0;
        sqlstm.sqtdso[59] = (unsigned short )0;
        sqlstm.sqhstv[60] = (         void  *)(tlBcTransLog.center_mchnt_cd);
        sqlstm.sqhstl[60] = (unsigned int  )21;
        sqlstm.sqhsts[60] = (         int  )0;
        sqlstm.sqindv[60] = (         void  *)0;
        sqlstm.sqinds[60] = (         int  )0;
        sqlstm.sqharm[60] = (unsigned int  )0;
        sqlstm.sqadto[60] = (unsigned short )0;
        sqlstm.sqtdso[60] = (unsigned short )0;
        sqlstm.sqhstv[61] = (         void  *)(tlBcTransLog.bill_id_rcv);
        sqlstm.sqhstl[61] = (unsigned int  )22;
        sqlstm.sqhsts[61] = (         int  )0;
        sqlstm.sqindv[61] = (         void  *)0;
        sqlstm.sqinds[61] = (         int  )0;
        sqlstm.sqharm[61] = (unsigned int  )0;
        sqlstm.sqadto[61] = (unsigned short )0;
        sqlstm.sqtdso[61] = (unsigned short )0;
        sqlstm.sqhstv[62] = (         void  *)(tlBcTransLog.card_attr);
        sqlstm.sqhstl[62] = (unsigned int  )3;
        sqlstm.sqhsts[62] = (         int  )0;
        sqlstm.sqindv[62] = (         void  *)0;
        sqlstm.sqinds[62] = (         int  )0;
        sqlstm.sqharm[62] = (unsigned int  )0;
        sqlstm.sqadto[62] = (unsigned short )0;
        sqlstm.sqtdso[62] = (unsigned short )0;
        sqlstm.sqhstv[63] = (         void  *)(tlBcTransLog.iss_ins_id_cd);
        sqlstm.sqhstl[63] = (unsigned int  )9;
        sqlstm.sqhsts[63] = (         int  )0;
        sqlstm.sqindv[63] = (         void  *)0;
        sqlstm.sqinds[63] = (         int  )0;
        sqlstm.sqharm[63] = (unsigned int  )0;
        sqlstm.sqadto[63] = (unsigned short )0;
        sqlstm.sqtdso[63] = (unsigned short )0;
        sqlstm.sqhstv[64] = (         void  *)(tlBcTransLog.pname);
        sqlstm.sqhstl[64] = (unsigned int  )211;
        sqlstm.sqhsts[64] = (         int  )0;
        sqlstm.sqindv[64] = (         void  *)0;
        sqlstm.sqinds[64] = (         int  )0;
        sqlstm.sqharm[64] = (unsigned int  )0;
        sqlstm.sqadto[64] = (unsigned short )0;
        sqlstm.sqtdso[64] = (unsigned short )0;
        sqlstm.sqhstv[65] = (         void  *)(tlBcTransLog.encrypt_info);
        sqlstm.sqhstl[65] = (unsigned int  )49;
        sqlstm.sqhsts[65] = (         int  )0;
        sqlstm.sqindv[65] = (         void  *)0;
        sqlstm.sqinds[65] = (         int  )0;
        sqlstm.sqharm[65] = (unsigned int  )0;
        sqlstm.sqadto[65] = (unsigned short )0;
        sqlstm.sqtdso[65] = (unsigned short )0;
        sqlstm.sqhstv[66] = (         void  *)(tlBcTransLog.id_type);
        sqlstm.sqhstl[66] = (unsigned int  )3;
        sqlstm.sqhsts[66] = (         int  )0;
        sqlstm.sqindv[66] = (         void  *)0;
        sqlstm.sqinds[66] = (         int  )0;
        sqlstm.sqharm[66] = (unsigned int  )0;
        sqlstm.sqadto[66] = (unsigned short )0;
        sqlstm.sqtdso[66] = (unsigned short )0;
        sqlstm.sqhstv[67] = (         void  *)(tlBcTransLog.originaDataElements);
        sqlstm.sqhstl[67] = (unsigned int  )44;
        sqlstm.sqhsts[67] = (         int  )0;
        sqlstm.sqindv[67] = (         void  *)0;
        sqlstm.sqinds[67] = (         int  )0;
        sqlstm.sqharm[67] = (unsigned int  )0;
        sqlstm.sqadto[67] = (unsigned short )0;
        sqlstm.sqtdso[67] = (unsigned short )0;
        sqlstm.sqhstv[68] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[68] = (unsigned int  )5;
        sqlstm.sqhsts[68] = (         int  )0;
        sqlstm.sqindv[68] = (         void  *)0;
        sqlstm.sqinds[68] = (         int  )0;
        sqlstm.sqharm[68] = (unsigned int  )0;
        sqlstm.sqadto[68] = (unsigned short )0;
        sqlstm.sqtdso[68] = (unsigned short )0;
        sqlstm.sqhstv[69] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[69] = (unsigned int  )21;
        sqlstm.sqhsts[69] = (         int  )0;
        sqlstm.sqindv[69] = (         void  *)0;
        sqlstm.sqinds[69] = (         int  )0;
        sqlstm.sqharm[69] = (unsigned int  )0;
        sqlstm.sqadto[69] = (unsigned short )0;
        sqlstm.sqtdso[69] = (unsigned short )0;
        sqlstm.sqhstv[70] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[70] = (unsigned int  )7;
        sqlstm.sqhsts[70] = (         int  )0;
        sqlstm.sqindv[70] = (         void  *)0;
        sqlstm.sqinds[70] = (         int  )0;
        sqlstm.sqharm[70] = (unsigned int  )0;
        sqlstm.sqadto[70] = (unsigned short )0;
        sqlstm.sqtdso[70] = (unsigned short )0;
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
            *p_sqlCode = 0;
            memcpy(p_tlBcTransLog, &tlBcTransLog, sizeof(tl_bc_trans_log_def));            
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT2>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT2>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )622;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

     case GLB_DB_SELECT3:

				 strcpy_safe(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, sizeof(loc_trans_dt)-1);
        rtrim_null(loc_trans_dt,' ');
        
        strcpy_safe(term_id, p_tlBcTransLog->center_term_id, sizeof(term_id)-1);
        rtrim_null(term_id,' ');

        strcpy_safe(mchnt_cd, p_tlBcTransLog->center_mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');

		strcpy_safe(sys_tra_no, p_tlBcTransLog->center_sys_tra_no, sizeof(sys_tra_no)-1);
        rtrim_null(sys_tra_no,' ');
        
        /* EXEC SQL SELECT 
        sys_dt_tm            ,     
        settle_dt            ,
        trans_flag           ,
        is_over              ,
        lock_tm              ,
        flag_lock            ,
        flag_verifi          ,
        flag_rcv             ,
        flag_pay             ,
        flag_reversal        ,
        resp_cd_verifi       ,
        resp_cd_rcv          ,
        resp_cd_pay          ,
        resp_cd_reversal     ,
        trans_chnl           ,
        packet_type          ,
        line_nm_verifi       ,
        line_nm_rcv          ,
        line_nm_pay          ,
        reversal_id          ,
        bc_settle_in         ,
        fld32_ins_id_cd      ,
        fwd_ins_id_cd        ,
        rcv_ins_id_cd        ,
        msg_tp               ,
        pri_acct_no          ,
        proc_cd              ,
        trans_at             ,
        transmsn_dt_tm       ,
        sys_tra_no           ,
        loc_trans_tm         ,
        loc_trans_dt         ,
        mchnt_tp             ,
        pos_entry_md_cd      ,
        pos_cond_cd          ,
        retri_ref_no         ,
        auth_id_resp_cd      ,
        resp_cd              ,
        term_id              ,
        mchnt_cd             ,
        area_cd              ,
        trans_curr_cd        ,
        flag_1               ,
        flag_2               ,
        flag_3               ,
        flag_4               ,
        flag_5               ,
        flag_6               ,
        flag_7               ,
        fld_5                ,
        fld_8                ,
        fld_28               ,
        fld_34               ,
        fld_44               ,
        fld_45               ,
        fld_46               ,
        fld_48               ,
        fld_61               ,
        center_sys_tra_no    ,
        center_term_id       ,
        center_mchnt_cd      ,
        bill_id_rcv          ,
        card_attr            ,
        iss_ins_id_cd        ,
        pname								 ,
        encrypt_info				 ,
        id_type 		,
        orig_data_elemts			
        INTO
           :tlBcTransLog.sys_dt_tm            ,   
           :tlBcTransLog.settle_dt            ,
           :tlBcTransLog.trans_flag           ,
           :tlBcTransLog.is_over              ,
           :tlBcTransLog.lock_tm              ,
           :tlBcTransLog.flag_lock            ,
           :tlBcTransLog.flag_verifi          ,
           :tlBcTransLog.flag_rcv             ,
           :tlBcTransLog.flag_pay             ,
           :tlBcTransLog.flag_reversal        ,
           :tlBcTransLog.resp_cd_verifi       ,
           :tlBcTransLog.resp_cd_rcv          ,
           :tlBcTransLog.resp_cd_pay          ,
           :tlBcTransLog.resp_cd_reversal     ,
           :tlBcTransLog.trans_chnl           ,
           :tlBcTransLog.packet_type          ,
           :tlBcTransLog.line_nm_verifi       ,
           :tlBcTransLog.line_nm_rcv          ,
           :tlBcTransLog.line_nm_pay          ,
           :tlBcTransLog.reversal_id          ,
           :tlBcTransLog.bc_settle_in         ,
           :tlBcTransLog.fld32_ins_id_cd      ,
           :tlBcTransLog.fwd_ins_id_cd        ,
           :tlBcTransLog.rcv_ins_id_cd        ,
           :tlBcTransLog.msg_tp               ,
           :tlBcTransLog.pri_acct_no          ,
           :tlBcTransLog.proc_cd              ,
           :tlBcTransLog.trans_at             ,
           :tlBcTransLog.transmsn_dt_tm       ,
           :tlBcTransLog.sys_tra_no           ,
           :tlBcTransLog.loc_trans_tm         ,
           :tlBcTransLog.loc_trans_dt         ,
           :tlBcTransLog.mchnt_tp             ,
           :tlBcTransLog.pos_entry_md_cd      ,
           :tlBcTransLog.pos_cond_cd          ,
           :tlBcTransLog.retri_ref_no         ,
           :tlBcTransLog.auth_id_resp_cd      ,
           :tlBcTransLog.resp_cd              ,
           :tlBcTransLog.term_id              ,
           :tlBcTransLog.mchnt_cd             ,
           :tlBcTransLog.area_cd              ,
           :tlBcTransLog.trans_curr_cd        ,
           :tlBcTransLog.flag_1               ,
           :tlBcTransLog.flag_2               ,
           :tlBcTransLog.flag_3               ,
           :tlBcTransLog.flag_4               ,
           :tlBcTransLog.flag_5               ,
           :tlBcTransLog.flag_6               ,
           :tlBcTransLog.flag_7               ,
           :tlBcTransLog.fld_5                ,
           :tlBcTransLog.fld_8                ,
           :tlBcTransLog.fld_28               ,
           :tlBcTransLog.fld_34               ,
           :tlBcTransLog.fld_44               ,
           :tlBcTransLog.fld_45               ,
           :tlBcTransLog.fld_46               ,
           :tlBcTransLog.fld_48               ,
           :tlBcTransLog.fld_61               ,
           :tlBcTransLog.center_sys_tra_no    ,
           :tlBcTransLog.center_term_id       ,
           :tlBcTransLog.center_mchnt_cd      ,
           :tlBcTransLog.bill_id_rcv          ,
           :tlBcTransLog.card_attr            ,
           :tlBcTransLog.iss_ins_id_cd        ,
           :tlBcTransLog.pname						    ,
           :tlBcTransLog.encrypt_info		      ,
           :tlBcTransLog.id_type ,
           :tlBcTransLog.originaDataElements
        FROM tl_bc_trans_log
        WHERE loc_trans_dt=:loc_trans_dt
				AND center_mchnt_cd=:mchnt_cd
				AND center_term_id=:term_id
				AND center_sys_tra_no=:sys_tra_no
				AND rownum<=1
               ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlbuft((void **)0,
          "select sys_dt_tm ,settle_dt ,trans_flag ,is_over ,lock_tm ,flag_l\
ock ,flag_verifi ,flag_rcv ,flag_pay ,flag_reversal ,resp_cd_verifi ,resp_cd\
_rcv ,resp_cd_pay ,resp_cd_reversal ,trans_chnl ,packet_type ,line_nm_verifi\
 ,line_nm_rcv ,line_nm_pay ,reversal_id ,bc_settle_in ,fld32_ins_id_cd ,fwd_\
ins_id_cd ,rcv_ins_id_cd ,msg_tp ,pri_acct_no ,proc_cd ,trans_at ,transmsn_d\
t_tm ,sys_tra_no ,loc_trans_tm ,loc_trans_dt ,mchnt_tp ,pos_entry_md_cd ,pos\
_cond_cd ,retri_ref_no ,auth_id_resp_cd ,resp_cd ,term_id ,mchnt_cd ,area_cd\
 ,trans_curr_cd ,flag_1 ,flag_2 ,flag_3 ,flag_4 ,flag_5 ,flag_6 ,flag_7 ,fld\
_5 ,fld_8 ,fld_28 ,fld_34 ,fld_44 ,fld_45 ,fld_46 ,fld_48 ,fld_61 ,center_sy\
s_tra_no ,center_term_id ,center_mchnt_cd ,bill_id_rcv ,card_attr ,iss_ins_i\
d_cd ,pname ,encrypt_info ,id_type ,orig_data_elemts into :b0,:b1,:b2,:b3,:b\
4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15,:b16,:b17,:b18,:b19,:b20\
,:b21,:b22,:b23,:b24,:b25,:b26,:b27,:b28,:b29,:b30,:b31,:b32,:b33,:b34,:b35,\
:b36,:b37,:b38,:b39,:b40,:b41,:b42,:b43,:b44,:b");
        sqlstm.stmt = "45,:b46,:b47,:b48,:b49,:b50,:b51,:b52,:b53,:b54,:b55\
,:b56,:b57,:b58,:b59,:b60,:b61,:b62,:b63,:b64,:b65,:b66,:b67  from tl_bc_tran\
s_log where ((((loc_trans_dt=:b68 and center_mchnt_cd=:b69) and center_term_i\
d=:b70) and center_sys_tra_no=:b71) and rownum<=1)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )637;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcTransLog.sys_dt_tm);
        sqlstm.sqhstl[0] = (unsigned int  )15;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.settle_dt);
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.trans_flag);
        sqlstm.sqhstl[2] = (unsigned int  )5;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.is_over);
        sqlstm.sqhstl[3] = (unsigned int  )2;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.lock_tm);
        sqlstm.sqhstl[4] = (unsigned int  )11;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.flag_lock);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcTransLog.flag_verifi);
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcTransLog.flag_rcv);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcTransLog.flag_pay);
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcTransLog.flag_reversal);
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcTransLog.resp_cd_verifi);
        sqlstm.sqhstl[10] = (unsigned int  )7;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcTransLog.resp_cd_rcv);
        sqlstm.sqhstl[11] = (unsigned int  )7;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcTransLog.resp_cd_pay);
        sqlstm.sqhstl[12] = (unsigned int  )7;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcTransLog.resp_cd_reversal);
        sqlstm.sqhstl[13] = (unsigned int  )7;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcTransLog.trans_chnl);
        sqlstm.sqhstl[14] = (unsigned int  )31;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcTransLog.packet_type);
        sqlstm.sqhstl[15] = (unsigned int  )5;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tlBcTransLog.line_nm_verifi);
        sqlstm.sqhstl[16] = (unsigned int  )5;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tlBcTransLog.line_nm_rcv);
        sqlstm.sqhstl[17] = (unsigned int  )5;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tlBcTransLog.line_nm_pay);
        sqlstm.sqhstl[18] = (unsigned int  )5;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tlBcTransLog.reversal_id);
        sqlstm.sqhstl[19] = (unsigned int  )9;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tlBcTransLog.bc_settle_in);
        sqlstm.sqhstl[20] = (unsigned int  )2;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tlBcTransLog.fld32_ins_id_cd);
        sqlstm.sqhstl[21] = (unsigned int  )14;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tlBcTransLog.fwd_ins_id_cd);
        sqlstm.sqhstl[22] = (unsigned int  )14;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tlBcTransLog.rcv_ins_id_cd);
        sqlstm.sqhstl[23] = (unsigned int  )14;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tlBcTransLog.msg_tp);
        sqlstm.sqhstl[24] = (unsigned int  )5;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tlBcTransLog.pri_acct_no);
        sqlstm.sqhstl[25] = (unsigned int  )22;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tlBcTransLog.proc_cd);
        sqlstm.sqhstl[26] = (unsigned int  )7;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tlBcTransLog.trans_at);
        sqlstm.sqhstl[27] = (unsigned int  )13;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tlBcTransLog.transmsn_dt_tm);
        sqlstm.sqhstl[28] = (unsigned int  )11;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tlBcTransLog.sys_tra_no);
        sqlstm.sqhstl[29] = (unsigned int  )7;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tlBcTransLog.loc_trans_tm);
        sqlstm.sqhstl[30] = (unsigned int  )7;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)(tlBcTransLog.loc_trans_dt);
        sqlstm.sqhstl[31] = (unsigned int  )5;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqhstv[32] = (         void  *)(tlBcTransLog.mchnt_tp);
        sqlstm.sqhstl[32] = (unsigned int  )5;
        sqlstm.sqhsts[32] = (         int  )0;
        sqlstm.sqindv[32] = (         void  *)0;
        sqlstm.sqinds[32] = (         int  )0;
        sqlstm.sqharm[32] = (unsigned int  )0;
        sqlstm.sqadto[32] = (unsigned short )0;
        sqlstm.sqtdso[32] = (unsigned short )0;
        sqlstm.sqhstv[33] = (         void  *)(tlBcTransLog.pos_entry_md_cd);
        sqlstm.sqhstl[33] = (unsigned int  )4;
        sqlstm.sqhsts[33] = (         int  )0;
        sqlstm.sqindv[33] = (         void  *)0;
        sqlstm.sqinds[33] = (         int  )0;
        sqlstm.sqharm[33] = (unsigned int  )0;
        sqlstm.sqadto[33] = (unsigned short )0;
        sqlstm.sqtdso[33] = (unsigned short )0;
        sqlstm.sqhstv[34] = (         void  *)(tlBcTransLog.pos_cond_cd);
        sqlstm.sqhstl[34] = (unsigned int  )3;
        sqlstm.sqhsts[34] = (         int  )0;
        sqlstm.sqindv[34] = (         void  *)0;
        sqlstm.sqinds[34] = (         int  )0;
        sqlstm.sqharm[34] = (unsigned int  )0;
        sqlstm.sqadto[34] = (unsigned short )0;
        sqlstm.sqtdso[34] = (unsigned short )0;
        sqlstm.sqhstv[35] = (         void  *)(tlBcTransLog.retri_ref_no);
        sqlstm.sqhstl[35] = (unsigned int  )13;
        sqlstm.sqhsts[35] = (         int  )0;
        sqlstm.sqindv[35] = (         void  *)0;
        sqlstm.sqinds[35] = (         int  )0;
        sqlstm.sqharm[35] = (unsigned int  )0;
        sqlstm.sqadto[35] = (unsigned short )0;
        sqlstm.sqtdso[35] = (unsigned short )0;
        sqlstm.sqhstv[36] = (         void  *)(tlBcTransLog.auth_id_resp_cd);
        sqlstm.sqhstl[36] = (unsigned int  )7;
        sqlstm.sqhsts[36] = (         int  )0;
        sqlstm.sqindv[36] = (         void  *)0;
        sqlstm.sqinds[36] = (         int  )0;
        sqlstm.sqharm[36] = (unsigned int  )0;
        sqlstm.sqadto[36] = (unsigned short )0;
        sqlstm.sqtdso[36] = (unsigned short )0;
        sqlstm.sqhstv[37] = (         void  *)(tlBcTransLog.resp_cd);
        sqlstm.sqhstl[37] = (unsigned int  )3;
        sqlstm.sqhsts[37] = (         int  )0;
        sqlstm.sqindv[37] = (         void  *)0;
        sqlstm.sqinds[37] = (         int  )0;
        sqlstm.sqharm[37] = (unsigned int  )0;
        sqlstm.sqadto[37] = (unsigned short )0;
        sqlstm.sqtdso[37] = (unsigned short )0;
        sqlstm.sqhstv[38] = (         void  *)(tlBcTransLog.term_id);
        sqlstm.sqhstl[38] = (unsigned int  )9;
        sqlstm.sqhsts[38] = (         int  )0;
        sqlstm.sqindv[38] = (         void  *)0;
        sqlstm.sqinds[38] = (         int  )0;
        sqlstm.sqharm[38] = (unsigned int  )0;
        sqlstm.sqadto[38] = (unsigned short )0;
        sqlstm.sqtdso[38] = (unsigned short )0;
        sqlstm.sqhstv[39] = (         void  *)(tlBcTransLog.mchnt_cd);
        sqlstm.sqhstl[39] = (unsigned int  )16;
        sqlstm.sqhsts[39] = (         int  )0;
        sqlstm.sqindv[39] = (         void  *)0;
        sqlstm.sqinds[39] = (         int  )0;
        sqlstm.sqharm[39] = (unsigned int  )0;
        sqlstm.sqadto[39] = (unsigned short )0;
        sqlstm.sqtdso[39] = (unsigned short )0;
        sqlstm.sqhstv[40] = (         void  *)(tlBcTransLog.area_cd);
        sqlstm.sqhstl[40] = (unsigned int  )7;
        sqlstm.sqhsts[40] = (         int  )0;
        sqlstm.sqindv[40] = (         void  *)0;
        sqlstm.sqinds[40] = (         int  )0;
        sqlstm.sqharm[40] = (unsigned int  )0;
        sqlstm.sqadto[40] = (unsigned short )0;
        sqlstm.sqtdso[40] = (unsigned short )0;
        sqlstm.sqhstv[41] = (         void  *)(tlBcTransLog.trans_curr_cd);
        sqlstm.sqhstl[41] = (unsigned int  )4;
        sqlstm.sqhsts[41] = (         int  )0;
        sqlstm.sqindv[41] = (         void  *)0;
        sqlstm.sqinds[41] = (         int  )0;
        sqlstm.sqharm[41] = (unsigned int  )0;
        sqlstm.sqadto[41] = (unsigned short )0;
        sqlstm.sqtdso[41] = (unsigned short )0;
        sqlstm.sqhstv[42] = (         void  *)(tlBcTransLog.flag_1);
        sqlstm.sqhstl[42] = (unsigned int  )2;
        sqlstm.sqhsts[42] = (         int  )0;
        sqlstm.sqindv[42] = (         void  *)0;
        sqlstm.sqinds[42] = (         int  )0;
        sqlstm.sqharm[42] = (unsigned int  )0;
        sqlstm.sqadto[42] = (unsigned short )0;
        sqlstm.sqtdso[42] = (unsigned short )0;
        sqlstm.sqhstv[43] = (         void  *)(tlBcTransLog.flag_2);
        sqlstm.sqhstl[43] = (unsigned int  )2;
        sqlstm.sqhsts[43] = (         int  )0;
        sqlstm.sqindv[43] = (         void  *)0;
        sqlstm.sqinds[43] = (         int  )0;
        sqlstm.sqharm[43] = (unsigned int  )0;
        sqlstm.sqadto[43] = (unsigned short )0;
        sqlstm.sqtdso[43] = (unsigned short )0;
        sqlstm.sqhstv[44] = (         void  *)(tlBcTransLog.flag_3);
        sqlstm.sqhstl[44] = (unsigned int  )2;
        sqlstm.sqhsts[44] = (         int  )0;
        sqlstm.sqindv[44] = (         void  *)0;
        sqlstm.sqinds[44] = (         int  )0;
        sqlstm.sqharm[44] = (unsigned int  )0;
        sqlstm.sqadto[44] = (unsigned short )0;
        sqlstm.sqtdso[44] = (unsigned short )0;
        sqlstm.sqhstv[45] = (         void  *)(tlBcTransLog.flag_4);
        sqlstm.sqhstl[45] = (unsigned int  )2;
        sqlstm.sqhsts[45] = (         int  )0;
        sqlstm.sqindv[45] = (         void  *)0;
        sqlstm.sqinds[45] = (         int  )0;
        sqlstm.sqharm[45] = (unsigned int  )0;
        sqlstm.sqadto[45] = (unsigned short )0;
        sqlstm.sqtdso[45] = (unsigned short )0;
        sqlstm.sqhstv[46] = (         void  *)(tlBcTransLog.flag_5);
        sqlstm.sqhstl[46] = (unsigned int  )2;
        sqlstm.sqhsts[46] = (         int  )0;
        sqlstm.sqindv[46] = (         void  *)0;
        sqlstm.sqinds[46] = (         int  )0;
        sqlstm.sqharm[46] = (unsigned int  )0;
        sqlstm.sqadto[46] = (unsigned short )0;
        sqlstm.sqtdso[46] = (unsigned short )0;
        sqlstm.sqhstv[47] = (         void  *)(tlBcTransLog.flag_6);
        sqlstm.sqhstl[47] = (unsigned int  )2;
        sqlstm.sqhsts[47] = (         int  )0;
        sqlstm.sqindv[47] = (         void  *)0;
        sqlstm.sqinds[47] = (         int  )0;
        sqlstm.sqharm[47] = (unsigned int  )0;
        sqlstm.sqadto[47] = (unsigned short )0;
        sqlstm.sqtdso[47] = (unsigned short )0;
        sqlstm.sqhstv[48] = (         void  *)(tlBcTransLog.flag_7);
        sqlstm.sqhstl[48] = (unsigned int  )2;
        sqlstm.sqhsts[48] = (         int  )0;
        sqlstm.sqindv[48] = (         void  *)0;
        sqlstm.sqinds[48] = (         int  )0;
        sqlstm.sqharm[48] = (unsigned int  )0;
        sqlstm.sqadto[48] = (unsigned short )0;
        sqlstm.sqtdso[48] = (unsigned short )0;
        sqlstm.sqhstv[49] = (         void  *)(tlBcTransLog.fld_5);
        sqlstm.sqhstl[49] = (unsigned int  )13;
        sqlstm.sqhsts[49] = (         int  )0;
        sqlstm.sqindv[49] = (         void  *)0;
        sqlstm.sqinds[49] = (         int  )0;
        sqlstm.sqharm[49] = (unsigned int  )0;
        sqlstm.sqadto[49] = (unsigned short )0;
        sqlstm.sqtdso[49] = (unsigned short )0;
        sqlstm.sqhstv[50] = (         void  *)(tlBcTransLog.fld_8);
        sqlstm.sqhstl[50] = (unsigned int  )9;
        sqlstm.sqhsts[50] = (         int  )0;
        sqlstm.sqindv[50] = (         void  *)0;
        sqlstm.sqinds[50] = (         int  )0;
        sqlstm.sqharm[50] = (unsigned int  )0;
        sqlstm.sqadto[50] = (unsigned short )0;
        sqlstm.sqtdso[50] = (unsigned short )0;
        sqlstm.sqhstv[51] = (         void  *)(tlBcTransLog.fld_28);
        sqlstm.sqhstl[51] = (unsigned int  )7;
        sqlstm.sqhsts[51] = (         int  )0;
        sqlstm.sqindv[51] = (         void  *)0;
        sqlstm.sqinds[51] = (         int  )0;
        sqlstm.sqharm[51] = (unsigned int  )0;
        sqlstm.sqadto[51] = (unsigned short )0;
        sqlstm.sqtdso[51] = (unsigned short )0;
        sqlstm.sqhstv[52] = (         void  *)(tlBcTransLog.fld_34);
        sqlstm.sqhstl[52] = (unsigned int  )29;
        sqlstm.sqhsts[52] = (         int  )0;
        sqlstm.sqindv[52] = (         void  *)0;
        sqlstm.sqinds[52] = (         int  )0;
        sqlstm.sqharm[52] = (unsigned int  )0;
        sqlstm.sqadto[52] = (unsigned short )0;
        sqlstm.sqtdso[52] = (unsigned short )0;
        sqlstm.sqhstv[53] = (         void  *)(tlBcTransLog.fld_44);
        sqlstm.sqhstl[53] = (unsigned int  )28;
        sqlstm.sqhsts[53] = (         int  )0;
        sqlstm.sqindv[53] = (         void  *)0;
        sqlstm.sqinds[53] = (         int  )0;
        sqlstm.sqharm[53] = (unsigned int  )0;
        sqlstm.sqadto[53] = (unsigned short )0;
        sqlstm.sqtdso[53] = (unsigned short )0;
        sqlstm.sqhstv[54] = (         void  *)(tlBcTransLog.fld_45);
        sqlstm.sqhstl[54] = (unsigned int  )77;
        sqlstm.sqhsts[54] = (         int  )0;
        sqlstm.sqindv[54] = (         void  *)0;
        sqlstm.sqinds[54] = (         int  )0;
        sqlstm.sqharm[54] = (unsigned int  )0;
        sqlstm.sqadto[54] = (unsigned short )0;
        sqlstm.sqtdso[54] = (unsigned short )0;
        sqlstm.sqhstv[55] = (         void  *)(tlBcTransLog.fld_46);
        sqlstm.sqhstl[55] = (unsigned int  )31;
        sqlstm.sqhsts[55] = (         int  )0;
        sqlstm.sqindv[55] = (         void  *)0;
        sqlstm.sqinds[55] = (         int  )0;
        sqlstm.sqharm[55] = (unsigned int  )0;
        sqlstm.sqadto[55] = (unsigned short )0;
        sqlstm.sqtdso[55] = (unsigned short )0;
        sqlstm.sqhstv[56] = (         void  *)(tlBcTransLog.fld_48);
        sqlstm.sqhstl[56] = (unsigned int  )191;
        sqlstm.sqhsts[56] = (         int  )0;
        sqlstm.sqindv[56] = (         void  *)0;
        sqlstm.sqinds[56] = (         int  )0;
        sqlstm.sqharm[56] = (unsigned int  )0;
        sqlstm.sqadto[56] = (unsigned short )0;
        sqlstm.sqtdso[56] = (unsigned short )0;
        sqlstm.sqhstv[57] = (         void  *)(tlBcTransLog.fld_61);
        sqlstm.sqhstl[57] = (unsigned int  )31;
        sqlstm.sqhsts[57] = (         int  )0;
        sqlstm.sqindv[57] = (         void  *)0;
        sqlstm.sqinds[57] = (         int  )0;
        sqlstm.sqharm[57] = (unsigned int  )0;
        sqlstm.sqadto[57] = (unsigned short )0;
        sqlstm.sqtdso[57] = (unsigned short )0;
        sqlstm.sqhstv[58] = (         void  *)(tlBcTransLog.center_sys_tra_no);
        sqlstm.sqhstl[58] = (unsigned int  )7;
        sqlstm.sqhsts[58] = (         int  )0;
        sqlstm.sqindv[58] = (         void  *)0;
        sqlstm.sqinds[58] = (         int  )0;
        sqlstm.sqharm[58] = (unsigned int  )0;
        sqlstm.sqadto[58] = (unsigned short )0;
        sqlstm.sqtdso[58] = (unsigned short )0;
        sqlstm.sqhstv[59] = (         void  *)(tlBcTransLog.center_term_id);
        sqlstm.sqhstl[59] = (unsigned int  )9;
        sqlstm.sqhsts[59] = (         int  )0;
        sqlstm.sqindv[59] = (         void  *)0;
        sqlstm.sqinds[59] = (         int  )0;
        sqlstm.sqharm[59] = (unsigned int  )0;
        sqlstm.sqadto[59] = (unsigned short )0;
        sqlstm.sqtdso[59] = (unsigned short )0;
        sqlstm.sqhstv[60] = (         void  *)(tlBcTransLog.center_mchnt_cd);
        sqlstm.sqhstl[60] = (unsigned int  )21;
        sqlstm.sqhsts[60] = (         int  )0;
        sqlstm.sqindv[60] = (         void  *)0;
        sqlstm.sqinds[60] = (         int  )0;
        sqlstm.sqharm[60] = (unsigned int  )0;
        sqlstm.sqadto[60] = (unsigned short )0;
        sqlstm.sqtdso[60] = (unsigned short )0;
        sqlstm.sqhstv[61] = (         void  *)(tlBcTransLog.bill_id_rcv);
        sqlstm.sqhstl[61] = (unsigned int  )22;
        sqlstm.sqhsts[61] = (         int  )0;
        sqlstm.sqindv[61] = (         void  *)0;
        sqlstm.sqinds[61] = (         int  )0;
        sqlstm.sqharm[61] = (unsigned int  )0;
        sqlstm.sqadto[61] = (unsigned short )0;
        sqlstm.sqtdso[61] = (unsigned short )0;
        sqlstm.sqhstv[62] = (         void  *)(tlBcTransLog.card_attr);
        sqlstm.sqhstl[62] = (unsigned int  )3;
        sqlstm.sqhsts[62] = (         int  )0;
        sqlstm.sqindv[62] = (         void  *)0;
        sqlstm.sqinds[62] = (         int  )0;
        sqlstm.sqharm[62] = (unsigned int  )0;
        sqlstm.sqadto[62] = (unsigned short )0;
        sqlstm.sqtdso[62] = (unsigned short )0;
        sqlstm.sqhstv[63] = (         void  *)(tlBcTransLog.iss_ins_id_cd);
        sqlstm.sqhstl[63] = (unsigned int  )9;
        sqlstm.sqhsts[63] = (         int  )0;
        sqlstm.sqindv[63] = (         void  *)0;
        sqlstm.sqinds[63] = (         int  )0;
        sqlstm.sqharm[63] = (unsigned int  )0;
        sqlstm.sqadto[63] = (unsigned short )0;
        sqlstm.sqtdso[63] = (unsigned short )0;
        sqlstm.sqhstv[64] = (         void  *)(tlBcTransLog.pname);
        sqlstm.sqhstl[64] = (unsigned int  )211;
        sqlstm.sqhsts[64] = (         int  )0;
        sqlstm.sqindv[64] = (         void  *)0;
        sqlstm.sqinds[64] = (         int  )0;
        sqlstm.sqharm[64] = (unsigned int  )0;
        sqlstm.sqadto[64] = (unsigned short )0;
        sqlstm.sqtdso[64] = (unsigned short )0;
        sqlstm.sqhstv[65] = (         void  *)(tlBcTransLog.encrypt_info);
        sqlstm.sqhstl[65] = (unsigned int  )49;
        sqlstm.sqhsts[65] = (         int  )0;
        sqlstm.sqindv[65] = (         void  *)0;
        sqlstm.sqinds[65] = (         int  )0;
        sqlstm.sqharm[65] = (unsigned int  )0;
        sqlstm.sqadto[65] = (unsigned short )0;
        sqlstm.sqtdso[65] = (unsigned short )0;
        sqlstm.sqhstv[66] = (         void  *)(tlBcTransLog.id_type);
        sqlstm.sqhstl[66] = (unsigned int  )3;
        sqlstm.sqhsts[66] = (         int  )0;
        sqlstm.sqindv[66] = (         void  *)0;
        sqlstm.sqinds[66] = (         int  )0;
        sqlstm.sqharm[66] = (unsigned int  )0;
        sqlstm.sqadto[66] = (unsigned short )0;
        sqlstm.sqtdso[66] = (unsigned short )0;
        sqlstm.sqhstv[67] = (         void  *)(tlBcTransLog.originaDataElements);
        sqlstm.sqhstl[67] = (unsigned int  )44;
        sqlstm.sqhsts[67] = (         int  )0;
        sqlstm.sqindv[67] = (         void  *)0;
        sqlstm.sqinds[67] = (         int  )0;
        sqlstm.sqharm[67] = (unsigned int  )0;
        sqlstm.sqadto[67] = (unsigned short )0;
        sqlstm.sqtdso[67] = (unsigned short )0;
        sqlstm.sqhstv[68] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[68] = (unsigned int  )5;
        sqlstm.sqhsts[68] = (         int  )0;
        sqlstm.sqindv[68] = (         void  *)0;
        sqlstm.sqinds[68] = (         int  )0;
        sqlstm.sqharm[68] = (unsigned int  )0;
        sqlstm.sqadto[68] = (unsigned short )0;
        sqlstm.sqtdso[68] = (unsigned short )0;
        sqlstm.sqhstv[69] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[69] = (unsigned int  )21;
        sqlstm.sqhsts[69] = (         int  )0;
        sqlstm.sqindv[69] = (         void  *)0;
        sqlstm.sqinds[69] = (         int  )0;
        sqlstm.sqharm[69] = (unsigned int  )0;
        sqlstm.sqadto[69] = (unsigned short )0;
        sqlstm.sqtdso[69] = (unsigned short )0;
        sqlstm.sqhstv[70] = (         void  *)term_id;
        sqlstm.sqhstl[70] = (unsigned int  )9;
        sqlstm.sqhsts[70] = (         int  )0;
        sqlstm.sqindv[70] = (         void  *)0;
        sqlstm.sqinds[70] = (         int  )0;
        sqlstm.sqharm[70] = (unsigned int  )0;
        sqlstm.sqadto[70] = (unsigned short )0;
        sqlstm.sqtdso[70] = (unsigned short )0;
        sqlstm.sqhstv[71] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[71] = (unsigned int  )7;
        sqlstm.sqhsts[71] = (         int  )0;
        sqlstm.sqindv[71] = (         void  *)0;
        sqlstm.sqinds[71] = (         int  )0;
        sqlstm.sqharm[71] = (unsigned int  )0;
        sqlstm.sqadto[71] = (unsigned short )0;
        sqlstm.sqtdso[71] = (unsigned short )0;
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
            memcpy(p_tlBcTransLog, &tlBcTransLog, sizeof(tl_bc_trans_log_def));
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT3>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT3>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )940;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    case GLB_DB_SELECT11:

        strcpy_safe(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, sizeof(loc_trans_dt)-1);
        rtrim_null(loc_trans_dt,' ');
        
        strcpy_safe(term_id, p_tlBcTransLog->term_id, sizeof(term_id)-1);
        rtrim_null(term_id,' ');

        strcpy_safe(mchnt_cd, p_tlBcTransLog->mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');

		strcpy_safe(sys_tra_no, p_tlBcTransLog->sys_tra_no, sizeof(sys_tra_no)-1);
        rtrim_null(sys_tra_no,' ');

        /* EXEC SQL SELECT 
        sys_dt_tm            ,              
        settle_dt            ,
        trans_flag           ,
        is_over              ,
        lock_tm              ,
        flag_lock            ,
        flag_verifi          ,
        flag_rcv             ,
        flag_pay             ,
        flag_reversal        ,
        resp_cd_verifi       ,
        resp_cd_rcv          ,
        resp_cd_pay          ,
        resp_cd_reversal     ,
        trans_chnl           ,
        packet_type          ,
        line_nm_verifi       ,
        line_nm_rcv          ,
        line_nm_pay          ,
        reversal_id          ,
        bc_settle_in         ,
        fld32_ins_id_cd      ,
        fwd_ins_id_cd        ,
        rcv_ins_id_cd        ,
        msg_tp               ,
        pri_acct_no          ,
        proc_cd              ,
        trans_at             ,
        transmsn_dt_tm       ,
        sys_tra_no           ,
        loc_trans_tm         ,
        loc_trans_dt         ,
        mchnt_tp             ,
        pos_entry_md_cd      ,
        pos_cond_cd          ,
        retri_ref_no         ,
        auth_id_resp_cd      ,
        resp_cd              ,
        term_id              ,
        mchnt_cd             ,
        area_cd              ,
        trans_curr_cd        ,
        flag_1               ,
        flag_2               ,
        flag_3               ,
        flag_4               ,
        flag_5               ,
        flag_6               ,
        flag_7               ,
        fld_5                ,
        fld_8                ,
        fld_28               ,
        fld_34               ,
        fld_44               ,
        fld_45               ,
        fld_46               ,
        fld_48               ,
        fld_61               ,
        center_sys_tra_no    ,
        center_term_id       ,
        center_mchnt_cd      ,
        bill_id_rcv          ,
        card_attr            ,
        iss_ins_id_cd        ,
        pname								 ,
        encrypt_info				 ,
        id_type 		,
        orig_data_elemts
        INTO 
           :tlBcTransLog.sys_dt_tm            ,   
           :tlBcTransLog.settle_dt            ,
           :tlBcTransLog.trans_flag           ,
           :tlBcTransLog.is_over              ,
           :tlBcTransLog.lock_tm              ,
           :tlBcTransLog.flag_lock            ,
           :tlBcTransLog.flag_verifi          ,
           :tlBcTransLog.flag_rcv             ,
           :tlBcTransLog.flag_pay             ,
           :tlBcTransLog.flag_reversal        ,
           :tlBcTransLog.resp_cd_verifi       ,
           :tlBcTransLog.resp_cd_rcv          ,
           :tlBcTransLog.resp_cd_pay          ,
           :tlBcTransLog.resp_cd_reversal     ,
           :tlBcTransLog.trans_chnl           ,
           :tlBcTransLog.packet_type          ,
           :tlBcTransLog.line_nm_verifi       ,
           :tlBcTransLog.line_nm_rcv          ,
           :tlBcTransLog.line_nm_pay          ,
           :tlBcTransLog.reversal_id          ,
           :tlBcTransLog.bc_settle_in         ,
           :tlBcTransLog.fld32_ins_id_cd      ,
           :tlBcTransLog.fwd_ins_id_cd        ,
           :tlBcTransLog.rcv_ins_id_cd        ,
           :tlBcTransLog.msg_tp               ,
           :tlBcTransLog.pri_acct_no          ,
           :tlBcTransLog.proc_cd              ,
           :tlBcTransLog.trans_at             ,
           :tlBcTransLog.transmsn_dt_tm       ,
           :tlBcTransLog.sys_tra_no           ,
           :tlBcTransLog.loc_trans_tm         ,
           :tlBcTransLog.loc_trans_dt         ,
           :tlBcTransLog.mchnt_tp             ,
           :tlBcTransLog.pos_entry_md_cd      ,
           :tlBcTransLog.pos_cond_cd          ,
           :tlBcTransLog.retri_ref_no         ,
           :tlBcTransLog.auth_id_resp_cd      ,
           :tlBcTransLog.resp_cd              ,
           :tlBcTransLog.term_id              ,
           :tlBcTransLog.mchnt_cd             ,
           :tlBcTransLog.area_cd              ,
           :tlBcTransLog.trans_curr_cd        ,
           :tlBcTransLog.flag_1               ,
           :tlBcTransLog.flag_2               ,
           :tlBcTransLog.flag_3               ,
           :tlBcTransLog.flag_4               ,
           :tlBcTransLog.flag_5               ,
           :tlBcTransLog.flag_6               ,
           :tlBcTransLog.flag_7               ,
           :tlBcTransLog.fld_5                ,
           :tlBcTransLog.fld_8                ,
           :tlBcTransLog.fld_28               ,
           :tlBcTransLog.fld_34               ,
           :tlBcTransLog.fld_44               ,
           :tlBcTransLog.fld_45               ,
           :tlBcTransLog.fld_46               ,
           :tlBcTransLog.fld_48               ,
           :tlBcTransLog.fld_61               ,
           :tlBcTransLog.center_sys_tra_no    ,
           :tlBcTransLog.center_term_id       ,
           :tlBcTransLog.center_mchnt_cd      ,
           :tlBcTransLog.bill_id_rcv          ,
           :tlBcTransLog.card_attr            ,
           :tlBcTransLog.iss_ins_id_cd        ,
           :tlBcTransLog.pname						    ,
           :tlBcTransLog.encrypt_info 		    ,
           :tlBcTransLog.id_type		,
           :tlBcTransLog.originaDataElements
        FROM tl_bc_trans_log
        WHERE loc_trans_dt=:loc_trans_dt
				AND mchnt_cd=:mchnt_cd
				AND sys_tra_no=:sys_tra_no
				AND term_id=:term_id
				AND rownum<=1
				FOR UPDATE; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlbuft((void **)0,
          "select sys_dt_tm ,settle_dt ,trans_flag ,is_over ,lock_tm ,flag_l\
ock ,flag_verifi ,flag_rcv ,flag_pay ,flag_reversal ,resp_cd_verifi ,resp_cd\
_rcv ,resp_cd_pay ,resp_cd_reversal ,trans_chnl ,packet_type ,line_nm_verifi\
 ,line_nm_rcv ,line_nm_pay ,reversal_id ,bc_settle_in ,fld32_ins_id_cd ,fwd_\
ins_id_cd ,rcv_ins_id_cd ,msg_tp ,pri_acct_no ,proc_cd ,trans_at ,transmsn_d\
t_tm ,sys_tra_no ,loc_trans_tm ,loc_trans_dt ,mchnt_tp ,pos_entry_md_cd ,pos\
_cond_cd ,retri_ref_no ,auth_id_resp_cd ,resp_cd ,term_id ,mchnt_cd ,area_cd\
 ,trans_curr_cd ,flag_1 ,flag_2 ,flag_3 ,flag_4 ,flag_5 ,flag_6 ,flag_7 ,fld\
_5 ,fld_8 ,fld_28 ,fld_34 ,fld_44 ,fld_45 ,fld_46 ,fld_48 ,fld_61 ,center_sy\
s_tra_no ,center_term_id ,center_mchnt_cd ,bill_id_rcv ,card_attr ,iss_ins_i\
d_cd ,pname ,encrypt_info ,id_type ,orig_data_elemts into :b0,:b1,:b2,:b3,:b\
4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15,:b16,:b17,:b18,:b19,:b20\
,:b21,:b22,:b23,:b24,:b25,:b26,:b27,:b28,:b29,:b30,:b31,:b32,:b33,:b34,:b35,\
:b36,:b37,:b38,:b39,:b40,:b41,:b42,:b43,:b44,:b");
        sqlstm.stmt = "45,:b46,:b47,:b48,:b49,:b50,:b51,:b52,:b53,:b54,:b55\
,:b56,:b57,:b58,:b59,:b60,:b61,:b62,:b63,:b64,:b65,:b66,:b67  from tl_bc_tran\
s_log where ((((loc_trans_dt=:b68 and mchnt_cd=:b69) and sys_tra_no=:b70) and\
 term_id=:b71) and rownum<=1) for update ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )955;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcTransLog.sys_dt_tm);
        sqlstm.sqhstl[0] = (unsigned int  )15;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.settle_dt);
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.trans_flag);
        sqlstm.sqhstl[2] = (unsigned int  )5;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.is_over);
        sqlstm.sqhstl[3] = (unsigned int  )2;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.lock_tm);
        sqlstm.sqhstl[4] = (unsigned int  )11;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.flag_lock);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcTransLog.flag_verifi);
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcTransLog.flag_rcv);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcTransLog.flag_pay);
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcTransLog.flag_reversal);
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcTransLog.resp_cd_verifi);
        sqlstm.sqhstl[10] = (unsigned int  )7;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcTransLog.resp_cd_rcv);
        sqlstm.sqhstl[11] = (unsigned int  )7;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcTransLog.resp_cd_pay);
        sqlstm.sqhstl[12] = (unsigned int  )7;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcTransLog.resp_cd_reversal);
        sqlstm.sqhstl[13] = (unsigned int  )7;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcTransLog.trans_chnl);
        sqlstm.sqhstl[14] = (unsigned int  )31;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcTransLog.packet_type);
        sqlstm.sqhstl[15] = (unsigned int  )5;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tlBcTransLog.line_nm_verifi);
        sqlstm.sqhstl[16] = (unsigned int  )5;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tlBcTransLog.line_nm_rcv);
        sqlstm.sqhstl[17] = (unsigned int  )5;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tlBcTransLog.line_nm_pay);
        sqlstm.sqhstl[18] = (unsigned int  )5;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tlBcTransLog.reversal_id);
        sqlstm.sqhstl[19] = (unsigned int  )9;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tlBcTransLog.bc_settle_in);
        sqlstm.sqhstl[20] = (unsigned int  )2;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tlBcTransLog.fld32_ins_id_cd);
        sqlstm.sqhstl[21] = (unsigned int  )14;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tlBcTransLog.fwd_ins_id_cd);
        sqlstm.sqhstl[22] = (unsigned int  )14;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tlBcTransLog.rcv_ins_id_cd);
        sqlstm.sqhstl[23] = (unsigned int  )14;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tlBcTransLog.msg_tp);
        sqlstm.sqhstl[24] = (unsigned int  )5;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tlBcTransLog.pri_acct_no);
        sqlstm.sqhstl[25] = (unsigned int  )22;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tlBcTransLog.proc_cd);
        sqlstm.sqhstl[26] = (unsigned int  )7;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tlBcTransLog.trans_at);
        sqlstm.sqhstl[27] = (unsigned int  )13;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tlBcTransLog.transmsn_dt_tm);
        sqlstm.sqhstl[28] = (unsigned int  )11;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tlBcTransLog.sys_tra_no);
        sqlstm.sqhstl[29] = (unsigned int  )7;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tlBcTransLog.loc_trans_tm);
        sqlstm.sqhstl[30] = (unsigned int  )7;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)(tlBcTransLog.loc_trans_dt);
        sqlstm.sqhstl[31] = (unsigned int  )5;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqhstv[32] = (         void  *)(tlBcTransLog.mchnt_tp);
        sqlstm.sqhstl[32] = (unsigned int  )5;
        sqlstm.sqhsts[32] = (         int  )0;
        sqlstm.sqindv[32] = (         void  *)0;
        sqlstm.sqinds[32] = (         int  )0;
        sqlstm.sqharm[32] = (unsigned int  )0;
        sqlstm.sqadto[32] = (unsigned short )0;
        sqlstm.sqtdso[32] = (unsigned short )0;
        sqlstm.sqhstv[33] = (         void  *)(tlBcTransLog.pos_entry_md_cd);
        sqlstm.sqhstl[33] = (unsigned int  )4;
        sqlstm.sqhsts[33] = (         int  )0;
        sqlstm.sqindv[33] = (         void  *)0;
        sqlstm.sqinds[33] = (         int  )0;
        sqlstm.sqharm[33] = (unsigned int  )0;
        sqlstm.sqadto[33] = (unsigned short )0;
        sqlstm.sqtdso[33] = (unsigned short )0;
        sqlstm.sqhstv[34] = (         void  *)(tlBcTransLog.pos_cond_cd);
        sqlstm.sqhstl[34] = (unsigned int  )3;
        sqlstm.sqhsts[34] = (         int  )0;
        sqlstm.sqindv[34] = (         void  *)0;
        sqlstm.sqinds[34] = (         int  )0;
        sqlstm.sqharm[34] = (unsigned int  )0;
        sqlstm.sqadto[34] = (unsigned short )0;
        sqlstm.sqtdso[34] = (unsigned short )0;
        sqlstm.sqhstv[35] = (         void  *)(tlBcTransLog.retri_ref_no);
        sqlstm.sqhstl[35] = (unsigned int  )13;
        sqlstm.sqhsts[35] = (         int  )0;
        sqlstm.sqindv[35] = (         void  *)0;
        sqlstm.sqinds[35] = (         int  )0;
        sqlstm.sqharm[35] = (unsigned int  )0;
        sqlstm.sqadto[35] = (unsigned short )0;
        sqlstm.sqtdso[35] = (unsigned short )0;
        sqlstm.sqhstv[36] = (         void  *)(tlBcTransLog.auth_id_resp_cd);
        sqlstm.sqhstl[36] = (unsigned int  )7;
        sqlstm.sqhsts[36] = (         int  )0;
        sqlstm.sqindv[36] = (         void  *)0;
        sqlstm.sqinds[36] = (         int  )0;
        sqlstm.sqharm[36] = (unsigned int  )0;
        sqlstm.sqadto[36] = (unsigned short )0;
        sqlstm.sqtdso[36] = (unsigned short )0;
        sqlstm.sqhstv[37] = (         void  *)(tlBcTransLog.resp_cd);
        sqlstm.sqhstl[37] = (unsigned int  )3;
        sqlstm.sqhsts[37] = (         int  )0;
        sqlstm.sqindv[37] = (         void  *)0;
        sqlstm.sqinds[37] = (         int  )0;
        sqlstm.sqharm[37] = (unsigned int  )0;
        sqlstm.sqadto[37] = (unsigned short )0;
        sqlstm.sqtdso[37] = (unsigned short )0;
        sqlstm.sqhstv[38] = (         void  *)(tlBcTransLog.term_id);
        sqlstm.sqhstl[38] = (unsigned int  )9;
        sqlstm.sqhsts[38] = (         int  )0;
        sqlstm.sqindv[38] = (         void  *)0;
        sqlstm.sqinds[38] = (         int  )0;
        sqlstm.sqharm[38] = (unsigned int  )0;
        sqlstm.sqadto[38] = (unsigned short )0;
        sqlstm.sqtdso[38] = (unsigned short )0;
        sqlstm.sqhstv[39] = (         void  *)(tlBcTransLog.mchnt_cd);
        sqlstm.sqhstl[39] = (unsigned int  )16;
        sqlstm.sqhsts[39] = (         int  )0;
        sqlstm.sqindv[39] = (         void  *)0;
        sqlstm.sqinds[39] = (         int  )0;
        sqlstm.sqharm[39] = (unsigned int  )0;
        sqlstm.sqadto[39] = (unsigned short )0;
        sqlstm.sqtdso[39] = (unsigned short )0;
        sqlstm.sqhstv[40] = (         void  *)(tlBcTransLog.area_cd);
        sqlstm.sqhstl[40] = (unsigned int  )7;
        sqlstm.sqhsts[40] = (         int  )0;
        sqlstm.sqindv[40] = (         void  *)0;
        sqlstm.sqinds[40] = (         int  )0;
        sqlstm.sqharm[40] = (unsigned int  )0;
        sqlstm.sqadto[40] = (unsigned short )0;
        sqlstm.sqtdso[40] = (unsigned short )0;
        sqlstm.sqhstv[41] = (         void  *)(tlBcTransLog.trans_curr_cd);
        sqlstm.sqhstl[41] = (unsigned int  )4;
        sqlstm.sqhsts[41] = (         int  )0;
        sqlstm.sqindv[41] = (         void  *)0;
        sqlstm.sqinds[41] = (         int  )0;
        sqlstm.sqharm[41] = (unsigned int  )0;
        sqlstm.sqadto[41] = (unsigned short )0;
        sqlstm.sqtdso[41] = (unsigned short )0;
        sqlstm.sqhstv[42] = (         void  *)(tlBcTransLog.flag_1);
        sqlstm.sqhstl[42] = (unsigned int  )2;
        sqlstm.sqhsts[42] = (         int  )0;
        sqlstm.sqindv[42] = (         void  *)0;
        sqlstm.sqinds[42] = (         int  )0;
        sqlstm.sqharm[42] = (unsigned int  )0;
        sqlstm.sqadto[42] = (unsigned short )0;
        sqlstm.sqtdso[42] = (unsigned short )0;
        sqlstm.sqhstv[43] = (         void  *)(tlBcTransLog.flag_2);
        sqlstm.sqhstl[43] = (unsigned int  )2;
        sqlstm.sqhsts[43] = (         int  )0;
        sqlstm.sqindv[43] = (         void  *)0;
        sqlstm.sqinds[43] = (         int  )0;
        sqlstm.sqharm[43] = (unsigned int  )0;
        sqlstm.sqadto[43] = (unsigned short )0;
        sqlstm.sqtdso[43] = (unsigned short )0;
        sqlstm.sqhstv[44] = (         void  *)(tlBcTransLog.flag_3);
        sqlstm.sqhstl[44] = (unsigned int  )2;
        sqlstm.sqhsts[44] = (         int  )0;
        sqlstm.sqindv[44] = (         void  *)0;
        sqlstm.sqinds[44] = (         int  )0;
        sqlstm.sqharm[44] = (unsigned int  )0;
        sqlstm.sqadto[44] = (unsigned short )0;
        sqlstm.sqtdso[44] = (unsigned short )0;
        sqlstm.sqhstv[45] = (         void  *)(tlBcTransLog.flag_4);
        sqlstm.sqhstl[45] = (unsigned int  )2;
        sqlstm.sqhsts[45] = (         int  )0;
        sqlstm.sqindv[45] = (         void  *)0;
        sqlstm.sqinds[45] = (         int  )0;
        sqlstm.sqharm[45] = (unsigned int  )0;
        sqlstm.sqadto[45] = (unsigned short )0;
        sqlstm.sqtdso[45] = (unsigned short )0;
        sqlstm.sqhstv[46] = (         void  *)(tlBcTransLog.flag_5);
        sqlstm.sqhstl[46] = (unsigned int  )2;
        sqlstm.sqhsts[46] = (         int  )0;
        sqlstm.sqindv[46] = (         void  *)0;
        sqlstm.sqinds[46] = (         int  )0;
        sqlstm.sqharm[46] = (unsigned int  )0;
        sqlstm.sqadto[46] = (unsigned short )0;
        sqlstm.sqtdso[46] = (unsigned short )0;
        sqlstm.sqhstv[47] = (         void  *)(tlBcTransLog.flag_6);
        sqlstm.sqhstl[47] = (unsigned int  )2;
        sqlstm.sqhsts[47] = (         int  )0;
        sqlstm.sqindv[47] = (         void  *)0;
        sqlstm.sqinds[47] = (         int  )0;
        sqlstm.sqharm[47] = (unsigned int  )0;
        sqlstm.sqadto[47] = (unsigned short )0;
        sqlstm.sqtdso[47] = (unsigned short )0;
        sqlstm.sqhstv[48] = (         void  *)(tlBcTransLog.flag_7);
        sqlstm.sqhstl[48] = (unsigned int  )2;
        sqlstm.sqhsts[48] = (         int  )0;
        sqlstm.sqindv[48] = (         void  *)0;
        sqlstm.sqinds[48] = (         int  )0;
        sqlstm.sqharm[48] = (unsigned int  )0;
        sqlstm.sqadto[48] = (unsigned short )0;
        sqlstm.sqtdso[48] = (unsigned short )0;
        sqlstm.sqhstv[49] = (         void  *)(tlBcTransLog.fld_5);
        sqlstm.sqhstl[49] = (unsigned int  )13;
        sqlstm.sqhsts[49] = (         int  )0;
        sqlstm.sqindv[49] = (         void  *)0;
        sqlstm.sqinds[49] = (         int  )0;
        sqlstm.sqharm[49] = (unsigned int  )0;
        sqlstm.sqadto[49] = (unsigned short )0;
        sqlstm.sqtdso[49] = (unsigned short )0;
        sqlstm.sqhstv[50] = (         void  *)(tlBcTransLog.fld_8);
        sqlstm.sqhstl[50] = (unsigned int  )9;
        sqlstm.sqhsts[50] = (         int  )0;
        sqlstm.sqindv[50] = (         void  *)0;
        sqlstm.sqinds[50] = (         int  )0;
        sqlstm.sqharm[50] = (unsigned int  )0;
        sqlstm.sqadto[50] = (unsigned short )0;
        sqlstm.sqtdso[50] = (unsigned short )0;
        sqlstm.sqhstv[51] = (         void  *)(tlBcTransLog.fld_28);
        sqlstm.sqhstl[51] = (unsigned int  )7;
        sqlstm.sqhsts[51] = (         int  )0;
        sqlstm.sqindv[51] = (         void  *)0;
        sqlstm.sqinds[51] = (         int  )0;
        sqlstm.sqharm[51] = (unsigned int  )0;
        sqlstm.sqadto[51] = (unsigned short )0;
        sqlstm.sqtdso[51] = (unsigned short )0;
        sqlstm.sqhstv[52] = (         void  *)(tlBcTransLog.fld_34);
        sqlstm.sqhstl[52] = (unsigned int  )29;
        sqlstm.sqhsts[52] = (         int  )0;
        sqlstm.sqindv[52] = (         void  *)0;
        sqlstm.sqinds[52] = (         int  )0;
        sqlstm.sqharm[52] = (unsigned int  )0;
        sqlstm.sqadto[52] = (unsigned short )0;
        sqlstm.sqtdso[52] = (unsigned short )0;
        sqlstm.sqhstv[53] = (         void  *)(tlBcTransLog.fld_44);
        sqlstm.sqhstl[53] = (unsigned int  )28;
        sqlstm.sqhsts[53] = (         int  )0;
        sqlstm.sqindv[53] = (         void  *)0;
        sqlstm.sqinds[53] = (         int  )0;
        sqlstm.sqharm[53] = (unsigned int  )0;
        sqlstm.sqadto[53] = (unsigned short )0;
        sqlstm.sqtdso[53] = (unsigned short )0;
        sqlstm.sqhstv[54] = (         void  *)(tlBcTransLog.fld_45);
        sqlstm.sqhstl[54] = (unsigned int  )77;
        sqlstm.sqhsts[54] = (         int  )0;
        sqlstm.sqindv[54] = (         void  *)0;
        sqlstm.sqinds[54] = (         int  )0;
        sqlstm.sqharm[54] = (unsigned int  )0;
        sqlstm.sqadto[54] = (unsigned short )0;
        sqlstm.sqtdso[54] = (unsigned short )0;
        sqlstm.sqhstv[55] = (         void  *)(tlBcTransLog.fld_46);
        sqlstm.sqhstl[55] = (unsigned int  )31;
        sqlstm.sqhsts[55] = (         int  )0;
        sqlstm.sqindv[55] = (         void  *)0;
        sqlstm.sqinds[55] = (         int  )0;
        sqlstm.sqharm[55] = (unsigned int  )0;
        sqlstm.sqadto[55] = (unsigned short )0;
        sqlstm.sqtdso[55] = (unsigned short )0;
        sqlstm.sqhstv[56] = (         void  *)(tlBcTransLog.fld_48);
        sqlstm.sqhstl[56] = (unsigned int  )191;
        sqlstm.sqhsts[56] = (         int  )0;
        sqlstm.sqindv[56] = (         void  *)0;
        sqlstm.sqinds[56] = (         int  )0;
        sqlstm.sqharm[56] = (unsigned int  )0;
        sqlstm.sqadto[56] = (unsigned short )0;
        sqlstm.sqtdso[56] = (unsigned short )0;
        sqlstm.sqhstv[57] = (         void  *)(tlBcTransLog.fld_61);
        sqlstm.sqhstl[57] = (unsigned int  )31;
        sqlstm.sqhsts[57] = (         int  )0;
        sqlstm.sqindv[57] = (         void  *)0;
        sqlstm.sqinds[57] = (         int  )0;
        sqlstm.sqharm[57] = (unsigned int  )0;
        sqlstm.sqadto[57] = (unsigned short )0;
        sqlstm.sqtdso[57] = (unsigned short )0;
        sqlstm.sqhstv[58] = (         void  *)(tlBcTransLog.center_sys_tra_no);
        sqlstm.sqhstl[58] = (unsigned int  )7;
        sqlstm.sqhsts[58] = (         int  )0;
        sqlstm.sqindv[58] = (         void  *)0;
        sqlstm.sqinds[58] = (         int  )0;
        sqlstm.sqharm[58] = (unsigned int  )0;
        sqlstm.sqadto[58] = (unsigned short )0;
        sqlstm.sqtdso[58] = (unsigned short )0;
        sqlstm.sqhstv[59] = (         void  *)(tlBcTransLog.center_term_id);
        sqlstm.sqhstl[59] = (unsigned int  )9;
        sqlstm.sqhsts[59] = (         int  )0;
        sqlstm.sqindv[59] = (         void  *)0;
        sqlstm.sqinds[59] = (         int  )0;
        sqlstm.sqharm[59] = (unsigned int  )0;
        sqlstm.sqadto[59] = (unsigned short )0;
        sqlstm.sqtdso[59] = (unsigned short )0;
        sqlstm.sqhstv[60] = (         void  *)(tlBcTransLog.center_mchnt_cd);
        sqlstm.sqhstl[60] = (unsigned int  )21;
        sqlstm.sqhsts[60] = (         int  )0;
        sqlstm.sqindv[60] = (         void  *)0;
        sqlstm.sqinds[60] = (         int  )0;
        sqlstm.sqharm[60] = (unsigned int  )0;
        sqlstm.sqadto[60] = (unsigned short )0;
        sqlstm.sqtdso[60] = (unsigned short )0;
        sqlstm.sqhstv[61] = (         void  *)(tlBcTransLog.bill_id_rcv);
        sqlstm.sqhstl[61] = (unsigned int  )22;
        sqlstm.sqhsts[61] = (         int  )0;
        sqlstm.sqindv[61] = (         void  *)0;
        sqlstm.sqinds[61] = (         int  )0;
        sqlstm.sqharm[61] = (unsigned int  )0;
        sqlstm.sqadto[61] = (unsigned short )0;
        sqlstm.sqtdso[61] = (unsigned short )0;
        sqlstm.sqhstv[62] = (         void  *)(tlBcTransLog.card_attr);
        sqlstm.sqhstl[62] = (unsigned int  )3;
        sqlstm.sqhsts[62] = (         int  )0;
        sqlstm.sqindv[62] = (         void  *)0;
        sqlstm.sqinds[62] = (         int  )0;
        sqlstm.sqharm[62] = (unsigned int  )0;
        sqlstm.sqadto[62] = (unsigned short )0;
        sqlstm.sqtdso[62] = (unsigned short )0;
        sqlstm.sqhstv[63] = (         void  *)(tlBcTransLog.iss_ins_id_cd);
        sqlstm.sqhstl[63] = (unsigned int  )9;
        sqlstm.sqhsts[63] = (         int  )0;
        sqlstm.sqindv[63] = (         void  *)0;
        sqlstm.sqinds[63] = (         int  )0;
        sqlstm.sqharm[63] = (unsigned int  )0;
        sqlstm.sqadto[63] = (unsigned short )0;
        sqlstm.sqtdso[63] = (unsigned short )0;
        sqlstm.sqhstv[64] = (         void  *)(tlBcTransLog.pname);
        sqlstm.sqhstl[64] = (unsigned int  )211;
        sqlstm.sqhsts[64] = (         int  )0;
        sqlstm.sqindv[64] = (         void  *)0;
        sqlstm.sqinds[64] = (         int  )0;
        sqlstm.sqharm[64] = (unsigned int  )0;
        sqlstm.sqadto[64] = (unsigned short )0;
        sqlstm.sqtdso[64] = (unsigned short )0;
        sqlstm.sqhstv[65] = (         void  *)(tlBcTransLog.encrypt_info);
        sqlstm.sqhstl[65] = (unsigned int  )49;
        sqlstm.sqhsts[65] = (         int  )0;
        sqlstm.sqindv[65] = (         void  *)0;
        sqlstm.sqinds[65] = (         int  )0;
        sqlstm.sqharm[65] = (unsigned int  )0;
        sqlstm.sqadto[65] = (unsigned short )0;
        sqlstm.sqtdso[65] = (unsigned short )0;
        sqlstm.sqhstv[66] = (         void  *)(tlBcTransLog.id_type);
        sqlstm.sqhstl[66] = (unsigned int  )3;
        sqlstm.sqhsts[66] = (         int  )0;
        sqlstm.sqindv[66] = (         void  *)0;
        sqlstm.sqinds[66] = (         int  )0;
        sqlstm.sqharm[66] = (unsigned int  )0;
        sqlstm.sqadto[66] = (unsigned short )0;
        sqlstm.sqtdso[66] = (unsigned short )0;
        sqlstm.sqhstv[67] = (         void  *)(tlBcTransLog.originaDataElements);
        sqlstm.sqhstl[67] = (unsigned int  )44;
        sqlstm.sqhsts[67] = (         int  )0;
        sqlstm.sqindv[67] = (         void  *)0;
        sqlstm.sqinds[67] = (         int  )0;
        sqlstm.sqharm[67] = (unsigned int  )0;
        sqlstm.sqadto[67] = (unsigned short )0;
        sqlstm.sqtdso[67] = (unsigned short )0;
        sqlstm.sqhstv[68] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[68] = (unsigned int  )5;
        sqlstm.sqhsts[68] = (         int  )0;
        sqlstm.sqindv[68] = (         void  *)0;
        sqlstm.sqinds[68] = (         int  )0;
        sqlstm.sqharm[68] = (unsigned int  )0;
        sqlstm.sqadto[68] = (unsigned short )0;
        sqlstm.sqtdso[68] = (unsigned short )0;
        sqlstm.sqhstv[69] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[69] = (unsigned int  )21;
        sqlstm.sqhsts[69] = (         int  )0;
        sqlstm.sqindv[69] = (         void  *)0;
        sqlstm.sqinds[69] = (         int  )0;
        sqlstm.sqharm[69] = (unsigned int  )0;
        sqlstm.sqadto[69] = (unsigned short )0;
        sqlstm.sqtdso[69] = (unsigned short )0;
        sqlstm.sqhstv[70] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[70] = (unsigned int  )7;
        sqlstm.sqhsts[70] = (         int  )0;
        sqlstm.sqindv[70] = (         void  *)0;
        sqlstm.sqinds[70] = (         int  )0;
        sqlstm.sqharm[70] = (unsigned int  )0;
        sqlstm.sqadto[70] = (unsigned short )0;
        sqlstm.sqtdso[70] = (unsigned short )0;
        sqlstm.sqhstv[71] = (         void  *)term_id;
        sqlstm.sqhstl[71] = (unsigned int  )9;
        sqlstm.sqhsts[71] = (         int  )0;
        sqlstm.sqindv[71] = (         void  *)0;
        sqlstm.sqinds[71] = (         int  )0;
        sqlstm.sqharm[71] = (unsigned int  )0;
        sqlstm.sqadto[71] = (unsigned short )0;
        sqlstm.sqtdso[71] = (unsigned short )0;
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
	        memcpy(p_tlBcTransLog, &tlBcTransLog, sizeof(tl_bc_trans_log_def));
	       	if ( '1' == *tlBcTransLog.flag_lock )
		    {
				memset( lock_tm, 0, sizeof(lock_tm) );
				glb_GetLocalTimeString(lock_tm,sizeof(lock_tm)-1,"%Y");            	  
				strcpy_safe(lock_tm+4,tlBcTransLog.lock_tm, 10);
				glb_mktime(&timeold, lock_tm);
				time(&timenew);
				if( 60 < timenew - timeold )
            	{
	        	  	*tlBcTransLog.flag_lock = '0';
	        	  	glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT11>警告，锁标记为已锁定,但锁定时间超时(%s)，自动解锁。 (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",lock_tm, loc_trans_dt,term_id,mchnt_cd,sys_tra_no , sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
            	}
            	else
            	{          	  
					*p_sqlCode = SQL_ERD_LOCKED;
					glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT11>警告，锁标记为已锁定。 (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
				}
			}
            if ( '1' != *tlBcTransLog.flag_lock )
            {
                glb_GetLocalTimeString(lock_tm,11,"%m%d%H%M%S");
                /* EXEC SQL UPDATE tl_bc_trans_log SET flag_lock='1',lock_tm=:lock_tm 
					        WHERE loc_trans_dt=:loc_trans_dt
									AND mchnt_cd=:mchnt_cd
									AND sys_tra_no=:sys_tra_no
									AND term_id=:term_id
									AND rownum<=1; */ 

{
                struct sqlexd sqlstm;
                sqlorat((void **)0, &sqlctx, &oraca);
                sqlstm.sqlvsn = 12;
                sqlstm.arrsiz = 72;
                sqlstm.sqladtp = &sqladt;
                sqlstm.sqltdsp = &sqltds;
                sqlstm.stmt = "update tl_bc_trans_log  set flag_lock='1',lo\
ck_tm=:b0 where ((((loc_trans_dt=:b1 and mchnt_cd=:b2) and sys_tra_no=:b3) an\
d term_id=:b4) and rownum<=1)";
                sqlstm.iters = (unsigned int  )1;
                sqlstm.offset = (unsigned int  )1258;
                sqlstm.cud = sqlcud0;
                sqlstm.sqlest = (unsigned char  *)&sqlca;
                sqlstm.sqlety = (unsigned short)4352;
                sqlstm.occurs = (unsigned int  )0;
                sqlstm.sqhstv[0] = (         void  *)lock_tm;
                sqlstm.sqhstl[0] = (unsigned int  )15;
                sqlstm.sqhsts[0] = (         int  )0;
                sqlstm.sqindv[0] = (         void  *)0;
                sqlstm.sqinds[0] = (         int  )0;
                sqlstm.sqharm[0] = (unsigned int  )0;
                sqlstm.sqadto[0] = (unsigned short )0;
                sqlstm.sqtdso[0] = (unsigned short )0;
                sqlstm.sqhstv[1] = (         void  *)loc_trans_dt;
                sqlstm.sqhstl[1] = (unsigned int  )5;
                sqlstm.sqhsts[1] = (         int  )0;
                sqlstm.sqindv[1] = (         void  *)0;
                sqlstm.sqinds[1] = (         int  )0;
                sqlstm.sqharm[1] = (unsigned int  )0;
                sqlstm.sqadto[1] = (unsigned short )0;
                sqlstm.sqtdso[1] = (unsigned short )0;
                sqlstm.sqhstv[2] = (         void  *)mchnt_cd;
                sqlstm.sqhstl[2] = (unsigned int  )21;
                sqlstm.sqhsts[2] = (         int  )0;
                sqlstm.sqindv[2] = (         void  *)0;
                sqlstm.sqinds[2] = (         int  )0;
                sqlstm.sqharm[2] = (unsigned int  )0;
                sqlstm.sqadto[2] = (unsigned short )0;
                sqlstm.sqtdso[2] = (unsigned short )0;
                sqlstm.sqhstv[3] = (         void  *)sys_tra_no;
                sqlstm.sqhstl[3] = (unsigned int  )7;
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


                if ( sqlca.sqlcode == 0 )
                {
	                *p_sqlCode = 0;
                } else
                {
                    *p_sqlCode = sqlca.sqlcode;
                    glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT11>更新锁标记失败 (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
                }
            }
		} else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT11>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT11>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )1293;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    case GLB_DB_SELECT13:

        strcpy_safe(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, sizeof(loc_trans_dt)-1);
        rtrim_null(loc_trans_dt,' ');
        
        strcpy_safe(term_id, p_tlBcTransLog->center_term_id, sizeof(term_id)-1);
        rtrim_null(term_id,' ');

        strcpy_safe(mchnt_cd, p_tlBcTransLog->center_mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');

		strcpy_safe(sys_tra_no, p_tlBcTransLog->center_sys_tra_no, sizeof(sys_tra_no)-1);
        rtrim_null(sys_tra_no,' ');
        
        /* EXEC SQL SELECT 
        sys_dt_tm            ,     
        settle_dt            ,
        trans_flag           ,
        is_over              ,
        lock_tm              ,
        flag_lock            ,
        flag_verifi          ,
        flag_rcv             ,
        flag_pay             ,
        flag_reversal        ,
        resp_cd_verifi       ,
        resp_cd_rcv          ,
        resp_cd_pay          ,
        resp_cd_reversal     ,
        trans_chnl           ,
        packet_type          ,
        line_nm_verifi       ,
        line_nm_rcv          ,
        line_nm_pay          ,
        reversal_id          ,
        bc_settle_in         ,
        fld32_ins_id_cd      ,
        fwd_ins_id_cd        ,
        rcv_ins_id_cd        ,
        msg_tp               ,
        pri_acct_no          ,
        proc_cd              ,
        trans_at             ,
        transmsn_dt_tm       ,
        sys_tra_no           ,
        loc_trans_tm         ,
        loc_trans_dt         ,
        mchnt_tp             ,
        pos_entry_md_cd      ,
        pos_cond_cd          ,
        retri_ref_no         ,
        auth_id_resp_cd      ,
        resp_cd              ,
        term_id              ,
        mchnt_cd             ,
        area_cd              ,
        trans_curr_cd        ,
        flag_1               ,
        flag_2               ,
        flag_3               ,
        flag_4               ,
        flag_5               ,
        flag_6               ,
        flag_7               ,
        fld_5                ,
        fld_8                ,
        fld_28               ,
        fld_34               ,
        fld_44               ,
        fld_45               ,
        fld_46               ,
        fld_48               ,
        fld_61               ,
        center_sys_tra_no    ,
        center_term_id       ,
        center_mchnt_cd      ,
        bill_id_rcv          ,
        card_attr            ,
        iss_ins_id_cd        ,
        pname								 ,
        encrypt_info				 ,
        id_type 		,
        orig_data_elemts
        INTO 
           :tlBcTransLog.sys_dt_tm            ,   
           :tlBcTransLog.settle_dt            ,
           :tlBcTransLog.trans_flag           ,
           :tlBcTransLog.is_over              ,
           :tlBcTransLog.lock_tm              ,
           :tlBcTransLog.flag_lock            ,
           :tlBcTransLog.flag_verifi          ,
           :tlBcTransLog.flag_rcv             ,
           :tlBcTransLog.flag_pay             ,
           :tlBcTransLog.flag_reversal        ,
           :tlBcTransLog.resp_cd_verifi       ,
           :tlBcTransLog.resp_cd_rcv          ,
           :tlBcTransLog.resp_cd_pay          ,
           :tlBcTransLog.resp_cd_reversal     ,
           :tlBcTransLog.trans_chnl           ,
           :tlBcTransLog.packet_type          ,
           :tlBcTransLog.line_nm_verifi       ,
           :tlBcTransLog.line_nm_rcv          ,
           :tlBcTransLog.line_nm_pay          ,
           :tlBcTransLog.reversal_id          ,
           :tlBcTransLog.bc_settle_in         ,
           :tlBcTransLog.fld32_ins_id_cd      ,
           :tlBcTransLog.fwd_ins_id_cd        ,
           :tlBcTransLog.rcv_ins_id_cd        ,
           :tlBcTransLog.msg_tp               ,
           :tlBcTransLog.pri_acct_no          ,
           :tlBcTransLog.proc_cd              ,
           :tlBcTransLog.trans_at             ,
           :tlBcTransLog.transmsn_dt_tm       ,
           :tlBcTransLog.sys_tra_no           ,
           :tlBcTransLog.loc_trans_tm         ,
           :tlBcTransLog.loc_trans_dt         ,
           :tlBcTransLog.mchnt_tp             ,
           :tlBcTransLog.pos_entry_md_cd      ,
           :tlBcTransLog.pos_cond_cd          ,
           :tlBcTransLog.retri_ref_no         ,
           :tlBcTransLog.auth_id_resp_cd      ,
           :tlBcTransLog.resp_cd              ,
           :tlBcTransLog.term_id              ,
           :tlBcTransLog.mchnt_cd             ,
           :tlBcTransLog.area_cd              ,
           :tlBcTransLog.trans_curr_cd        ,
           :tlBcTransLog.flag_1               ,
           :tlBcTransLog.flag_2               ,
           :tlBcTransLog.flag_3               ,
           :tlBcTransLog.flag_4               ,
           :tlBcTransLog.flag_5               ,
           :tlBcTransLog.flag_6               ,
           :tlBcTransLog.flag_7               ,
           :tlBcTransLog.fld_5                ,
           :tlBcTransLog.fld_8                ,
           :tlBcTransLog.fld_28               ,
           :tlBcTransLog.fld_34               ,
           :tlBcTransLog.fld_44               ,
           :tlBcTransLog.fld_45               ,
           :tlBcTransLog.fld_46               ,
           :tlBcTransLog.fld_48               ,
           :tlBcTransLog.fld_61               ,
           :tlBcTransLog.center_sys_tra_no    ,
           :tlBcTransLog.center_term_id       ,
           :tlBcTransLog.center_mchnt_cd      ,
           :tlBcTransLog.bill_id_rcv          ,
           :tlBcTransLog.card_attr            ,
           :tlBcTransLog.iss_ins_id_cd        ,
           :tlBcTransLog.pname			   			  ,
           :tlBcTransLog.encrypt_info					,
           :tlBcTransLog.id_type		,
           :tlBcTransLog.originaDataElements
        FROM tl_bc_trans_log
        WHERE loc_trans_dt=:loc_trans_dt
				AND center_mchnt_cd=:mchnt_cd
				AND center_term_id=:term_id
				AND center_sys_tra_no=:sys_tra_no
				AND rownum<=1
	            FOR UPDATE; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlbuft((void **)0,
          "select sys_dt_tm ,settle_dt ,trans_flag ,is_over ,lock_tm ,flag_l\
ock ,flag_verifi ,flag_rcv ,flag_pay ,flag_reversal ,resp_cd_verifi ,resp_cd\
_rcv ,resp_cd_pay ,resp_cd_reversal ,trans_chnl ,packet_type ,line_nm_verifi\
 ,line_nm_rcv ,line_nm_pay ,reversal_id ,bc_settle_in ,fld32_ins_id_cd ,fwd_\
ins_id_cd ,rcv_ins_id_cd ,msg_tp ,pri_acct_no ,proc_cd ,trans_at ,transmsn_d\
t_tm ,sys_tra_no ,loc_trans_tm ,loc_trans_dt ,mchnt_tp ,pos_entry_md_cd ,pos\
_cond_cd ,retri_ref_no ,auth_id_resp_cd ,resp_cd ,term_id ,mchnt_cd ,area_cd\
 ,trans_curr_cd ,flag_1 ,flag_2 ,flag_3 ,flag_4 ,flag_5 ,flag_6 ,flag_7 ,fld\
_5 ,fld_8 ,fld_28 ,fld_34 ,fld_44 ,fld_45 ,fld_46 ,fld_48 ,fld_61 ,center_sy\
s_tra_no ,center_term_id ,center_mchnt_cd ,bill_id_rcv ,card_attr ,iss_ins_i\
d_cd ,pname ,encrypt_info ,id_type ,orig_data_elemts into :b0,:b1,:b2,:b3,:b\
4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15,:b16,:b17,:b18,:b19,:b20\
,:b21,:b22,:b23,:b24,:b25,:b26,:b27,:b28,:b29,:b30,:b31,:b32,:b33,:b34,:b35,\
:b36,:b37,:b38,:b39,:b40,:b41,:b42,:b43,:b44,:b");
        sqlstm.stmt = "45,:b46,:b47,:b48,:b49,:b50,:b51,:b52,:b53,:b54,:b55\
,:b56,:b57,:b58,:b59,:b60,:b61,:b62,:b63,:b64,:b65,:b66,:b67  from tl_bc_tran\
s_log where ((((loc_trans_dt=:b68 and center_mchnt_cd=:b69) and center_term_i\
d=:b70) and center_sys_tra_no=:b71) and rownum<=1) for update ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )1308;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcTransLog.sys_dt_tm);
        sqlstm.sqhstl[0] = (unsigned int  )15;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.settle_dt);
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.trans_flag);
        sqlstm.sqhstl[2] = (unsigned int  )5;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.is_over);
        sqlstm.sqhstl[3] = (unsigned int  )2;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.lock_tm);
        sqlstm.sqhstl[4] = (unsigned int  )11;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.flag_lock);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcTransLog.flag_verifi);
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcTransLog.flag_rcv);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcTransLog.flag_pay);
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcTransLog.flag_reversal);
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcTransLog.resp_cd_verifi);
        sqlstm.sqhstl[10] = (unsigned int  )7;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcTransLog.resp_cd_rcv);
        sqlstm.sqhstl[11] = (unsigned int  )7;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcTransLog.resp_cd_pay);
        sqlstm.sqhstl[12] = (unsigned int  )7;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcTransLog.resp_cd_reversal);
        sqlstm.sqhstl[13] = (unsigned int  )7;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcTransLog.trans_chnl);
        sqlstm.sqhstl[14] = (unsigned int  )31;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcTransLog.packet_type);
        sqlstm.sqhstl[15] = (unsigned int  )5;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tlBcTransLog.line_nm_verifi);
        sqlstm.sqhstl[16] = (unsigned int  )5;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tlBcTransLog.line_nm_rcv);
        sqlstm.sqhstl[17] = (unsigned int  )5;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tlBcTransLog.line_nm_pay);
        sqlstm.sqhstl[18] = (unsigned int  )5;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tlBcTransLog.reversal_id);
        sqlstm.sqhstl[19] = (unsigned int  )9;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tlBcTransLog.bc_settle_in);
        sqlstm.sqhstl[20] = (unsigned int  )2;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tlBcTransLog.fld32_ins_id_cd);
        sqlstm.sqhstl[21] = (unsigned int  )14;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tlBcTransLog.fwd_ins_id_cd);
        sqlstm.sqhstl[22] = (unsigned int  )14;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tlBcTransLog.rcv_ins_id_cd);
        sqlstm.sqhstl[23] = (unsigned int  )14;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tlBcTransLog.msg_tp);
        sqlstm.sqhstl[24] = (unsigned int  )5;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tlBcTransLog.pri_acct_no);
        sqlstm.sqhstl[25] = (unsigned int  )22;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tlBcTransLog.proc_cd);
        sqlstm.sqhstl[26] = (unsigned int  )7;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tlBcTransLog.trans_at);
        sqlstm.sqhstl[27] = (unsigned int  )13;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tlBcTransLog.transmsn_dt_tm);
        sqlstm.sqhstl[28] = (unsigned int  )11;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tlBcTransLog.sys_tra_no);
        sqlstm.sqhstl[29] = (unsigned int  )7;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tlBcTransLog.loc_trans_tm);
        sqlstm.sqhstl[30] = (unsigned int  )7;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)(tlBcTransLog.loc_trans_dt);
        sqlstm.sqhstl[31] = (unsigned int  )5;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqhstv[32] = (         void  *)(tlBcTransLog.mchnt_tp);
        sqlstm.sqhstl[32] = (unsigned int  )5;
        sqlstm.sqhsts[32] = (         int  )0;
        sqlstm.sqindv[32] = (         void  *)0;
        sqlstm.sqinds[32] = (         int  )0;
        sqlstm.sqharm[32] = (unsigned int  )0;
        sqlstm.sqadto[32] = (unsigned short )0;
        sqlstm.sqtdso[32] = (unsigned short )0;
        sqlstm.sqhstv[33] = (         void  *)(tlBcTransLog.pos_entry_md_cd);
        sqlstm.sqhstl[33] = (unsigned int  )4;
        sqlstm.sqhsts[33] = (         int  )0;
        sqlstm.sqindv[33] = (         void  *)0;
        sqlstm.sqinds[33] = (         int  )0;
        sqlstm.sqharm[33] = (unsigned int  )0;
        sqlstm.sqadto[33] = (unsigned short )0;
        sqlstm.sqtdso[33] = (unsigned short )0;
        sqlstm.sqhstv[34] = (         void  *)(tlBcTransLog.pos_cond_cd);
        sqlstm.sqhstl[34] = (unsigned int  )3;
        sqlstm.sqhsts[34] = (         int  )0;
        sqlstm.sqindv[34] = (         void  *)0;
        sqlstm.sqinds[34] = (         int  )0;
        sqlstm.sqharm[34] = (unsigned int  )0;
        sqlstm.sqadto[34] = (unsigned short )0;
        sqlstm.sqtdso[34] = (unsigned short )0;
        sqlstm.sqhstv[35] = (         void  *)(tlBcTransLog.retri_ref_no);
        sqlstm.sqhstl[35] = (unsigned int  )13;
        sqlstm.sqhsts[35] = (         int  )0;
        sqlstm.sqindv[35] = (         void  *)0;
        sqlstm.sqinds[35] = (         int  )0;
        sqlstm.sqharm[35] = (unsigned int  )0;
        sqlstm.sqadto[35] = (unsigned short )0;
        sqlstm.sqtdso[35] = (unsigned short )0;
        sqlstm.sqhstv[36] = (         void  *)(tlBcTransLog.auth_id_resp_cd);
        sqlstm.sqhstl[36] = (unsigned int  )7;
        sqlstm.sqhsts[36] = (         int  )0;
        sqlstm.sqindv[36] = (         void  *)0;
        sqlstm.sqinds[36] = (         int  )0;
        sqlstm.sqharm[36] = (unsigned int  )0;
        sqlstm.sqadto[36] = (unsigned short )0;
        sqlstm.sqtdso[36] = (unsigned short )0;
        sqlstm.sqhstv[37] = (         void  *)(tlBcTransLog.resp_cd);
        sqlstm.sqhstl[37] = (unsigned int  )3;
        sqlstm.sqhsts[37] = (         int  )0;
        sqlstm.sqindv[37] = (         void  *)0;
        sqlstm.sqinds[37] = (         int  )0;
        sqlstm.sqharm[37] = (unsigned int  )0;
        sqlstm.sqadto[37] = (unsigned short )0;
        sqlstm.sqtdso[37] = (unsigned short )0;
        sqlstm.sqhstv[38] = (         void  *)(tlBcTransLog.term_id);
        sqlstm.sqhstl[38] = (unsigned int  )9;
        sqlstm.sqhsts[38] = (         int  )0;
        sqlstm.sqindv[38] = (         void  *)0;
        sqlstm.sqinds[38] = (         int  )0;
        sqlstm.sqharm[38] = (unsigned int  )0;
        sqlstm.sqadto[38] = (unsigned short )0;
        sqlstm.sqtdso[38] = (unsigned short )0;
        sqlstm.sqhstv[39] = (         void  *)(tlBcTransLog.mchnt_cd);
        sqlstm.sqhstl[39] = (unsigned int  )16;
        sqlstm.sqhsts[39] = (         int  )0;
        sqlstm.sqindv[39] = (         void  *)0;
        sqlstm.sqinds[39] = (         int  )0;
        sqlstm.sqharm[39] = (unsigned int  )0;
        sqlstm.sqadto[39] = (unsigned short )0;
        sqlstm.sqtdso[39] = (unsigned short )0;
        sqlstm.sqhstv[40] = (         void  *)(tlBcTransLog.area_cd);
        sqlstm.sqhstl[40] = (unsigned int  )7;
        sqlstm.sqhsts[40] = (         int  )0;
        sqlstm.sqindv[40] = (         void  *)0;
        sqlstm.sqinds[40] = (         int  )0;
        sqlstm.sqharm[40] = (unsigned int  )0;
        sqlstm.sqadto[40] = (unsigned short )0;
        sqlstm.sqtdso[40] = (unsigned short )0;
        sqlstm.sqhstv[41] = (         void  *)(tlBcTransLog.trans_curr_cd);
        sqlstm.sqhstl[41] = (unsigned int  )4;
        sqlstm.sqhsts[41] = (         int  )0;
        sqlstm.sqindv[41] = (         void  *)0;
        sqlstm.sqinds[41] = (         int  )0;
        sqlstm.sqharm[41] = (unsigned int  )0;
        sqlstm.sqadto[41] = (unsigned short )0;
        sqlstm.sqtdso[41] = (unsigned short )0;
        sqlstm.sqhstv[42] = (         void  *)(tlBcTransLog.flag_1);
        sqlstm.sqhstl[42] = (unsigned int  )2;
        sqlstm.sqhsts[42] = (         int  )0;
        sqlstm.sqindv[42] = (         void  *)0;
        sqlstm.sqinds[42] = (         int  )0;
        sqlstm.sqharm[42] = (unsigned int  )0;
        sqlstm.sqadto[42] = (unsigned short )0;
        sqlstm.sqtdso[42] = (unsigned short )0;
        sqlstm.sqhstv[43] = (         void  *)(tlBcTransLog.flag_2);
        sqlstm.sqhstl[43] = (unsigned int  )2;
        sqlstm.sqhsts[43] = (         int  )0;
        sqlstm.sqindv[43] = (         void  *)0;
        sqlstm.sqinds[43] = (         int  )0;
        sqlstm.sqharm[43] = (unsigned int  )0;
        sqlstm.sqadto[43] = (unsigned short )0;
        sqlstm.sqtdso[43] = (unsigned short )0;
        sqlstm.sqhstv[44] = (         void  *)(tlBcTransLog.flag_3);
        sqlstm.sqhstl[44] = (unsigned int  )2;
        sqlstm.sqhsts[44] = (         int  )0;
        sqlstm.sqindv[44] = (         void  *)0;
        sqlstm.sqinds[44] = (         int  )0;
        sqlstm.sqharm[44] = (unsigned int  )0;
        sqlstm.sqadto[44] = (unsigned short )0;
        sqlstm.sqtdso[44] = (unsigned short )0;
        sqlstm.sqhstv[45] = (         void  *)(tlBcTransLog.flag_4);
        sqlstm.sqhstl[45] = (unsigned int  )2;
        sqlstm.sqhsts[45] = (         int  )0;
        sqlstm.sqindv[45] = (         void  *)0;
        sqlstm.sqinds[45] = (         int  )0;
        sqlstm.sqharm[45] = (unsigned int  )0;
        sqlstm.sqadto[45] = (unsigned short )0;
        sqlstm.sqtdso[45] = (unsigned short )0;
        sqlstm.sqhstv[46] = (         void  *)(tlBcTransLog.flag_5);
        sqlstm.sqhstl[46] = (unsigned int  )2;
        sqlstm.sqhsts[46] = (         int  )0;
        sqlstm.sqindv[46] = (         void  *)0;
        sqlstm.sqinds[46] = (         int  )0;
        sqlstm.sqharm[46] = (unsigned int  )0;
        sqlstm.sqadto[46] = (unsigned short )0;
        sqlstm.sqtdso[46] = (unsigned short )0;
        sqlstm.sqhstv[47] = (         void  *)(tlBcTransLog.flag_6);
        sqlstm.sqhstl[47] = (unsigned int  )2;
        sqlstm.sqhsts[47] = (         int  )0;
        sqlstm.sqindv[47] = (         void  *)0;
        sqlstm.sqinds[47] = (         int  )0;
        sqlstm.sqharm[47] = (unsigned int  )0;
        sqlstm.sqadto[47] = (unsigned short )0;
        sqlstm.sqtdso[47] = (unsigned short )0;
        sqlstm.sqhstv[48] = (         void  *)(tlBcTransLog.flag_7);
        sqlstm.sqhstl[48] = (unsigned int  )2;
        sqlstm.sqhsts[48] = (         int  )0;
        sqlstm.sqindv[48] = (         void  *)0;
        sqlstm.sqinds[48] = (         int  )0;
        sqlstm.sqharm[48] = (unsigned int  )0;
        sqlstm.sqadto[48] = (unsigned short )0;
        sqlstm.sqtdso[48] = (unsigned short )0;
        sqlstm.sqhstv[49] = (         void  *)(tlBcTransLog.fld_5);
        sqlstm.sqhstl[49] = (unsigned int  )13;
        sqlstm.sqhsts[49] = (         int  )0;
        sqlstm.sqindv[49] = (         void  *)0;
        sqlstm.sqinds[49] = (         int  )0;
        sqlstm.sqharm[49] = (unsigned int  )0;
        sqlstm.sqadto[49] = (unsigned short )0;
        sqlstm.sqtdso[49] = (unsigned short )0;
        sqlstm.sqhstv[50] = (         void  *)(tlBcTransLog.fld_8);
        sqlstm.sqhstl[50] = (unsigned int  )9;
        sqlstm.sqhsts[50] = (         int  )0;
        sqlstm.sqindv[50] = (         void  *)0;
        sqlstm.sqinds[50] = (         int  )0;
        sqlstm.sqharm[50] = (unsigned int  )0;
        sqlstm.sqadto[50] = (unsigned short )0;
        sqlstm.sqtdso[50] = (unsigned short )0;
        sqlstm.sqhstv[51] = (         void  *)(tlBcTransLog.fld_28);
        sqlstm.sqhstl[51] = (unsigned int  )7;
        sqlstm.sqhsts[51] = (         int  )0;
        sqlstm.sqindv[51] = (         void  *)0;
        sqlstm.sqinds[51] = (         int  )0;
        sqlstm.sqharm[51] = (unsigned int  )0;
        sqlstm.sqadto[51] = (unsigned short )0;
        sqlstm.sqtdso[51] = (unsigned short )0;
        sqlstm.sqhstv[52] = (         void  *)(tlBcTransLog.fld_34);
        sqlstm.sqhstl[52] = (unsigned int  )29;
        sqlstm.sqhsts[52] = (         int  )0;
        sqlstm.sqindv[52] = (         void  *)0;
        sqlstm.sqinds[52] = (         int  )0;
        sqlstm.sqharm[52] = (unsigned int  )0;
        sqlstm.sqadto[52] = (unsigned short )0;
        sqlstm.sqtdso[52] = (unsigned short )0;
        sqlstm.sqhstv[53] = (         void  *)(tlBcTransLog.fld_44);
        sqlstm.sqhstl[53] = (unsigned int  )28;
        sqlstm.sqhsts[53] = (         int  )0;
        sqlstm.sqindv[53] = (         void  *)0;
        sqlstm.sqinds[53] = (         int  )0;
        sqlstm.sqharm[53] = (unsigned int  )0;
        sqlstm.sqadto[53] = (unsigned short )0;
        sqlstm.sqtdso[53] = (unsigned short )0;
        sqlstm.sqhstv[54] = (         void  *)(tlBcTransLog.fld_45);
        sqlstm.sqhstl[54] = (unsigned int  )77;
        sqlstm.sqhsts[54] = (         int  )0;
        sqlstm.sqindv[54] = (         void  *)0;
        sqlstm.sqinds[54] = (         int  )0;
        sqlstm.sqharm[54] = (unsigned int  )0;
        sqlstm.sqadto[54] = (unsigned short )0;
        sqlstm.sqtdso[54] = (unsigned short )0;
        sqlstm.sqhstv[55] = (         void  *)(tlBcTransLog.fld_46);
        sqlstm.sqhstl[55] = (unsigned int  )31;
        sqlstm.sqhsts[55] = (         int  )0;
        sqlstm.sqindv[55] = (         void  *)0;
        sqlstm.sqinds[55] = (         int  )0;
        sqlstm.sqharm[55] = (unsigned int  )0;
        sqlstm.sqadto[55] = (unsigned short )0;
        sqlstm.sqtdso[55] = (unsigned short )0;
        sqlstm.sqhstv[56] = (         void  *)(tlBcTransLog.fld_48);
        sqlstm.sqhstl[56] = (unsigned int  )191;
        sqlstm.sqhsts[56] = (         int  )0;
        sqlstm.sqindv[56] = (         void  *)0;
        sqlstm.sqinds[56] = (         int  )0;
        sqlstm.sqharm[56] = (unsigned int  )0;
        sqlstm.sqadto[56] = (unsigned short )0;
        sqlstm.sqtdso[56] = (unsigned short )0;
        sqlstm.sqhstv[57] = (         void  *)(tlBcTransLog.fld_61);
        sqlstm.sqhstl[57] = (unsigned int  )31;
        sqlstm.sqhsts[57] = (         int  )0;
        sqlstm.sqindv[57] = (         void  *)0;
        sqlstm.sqinds[57] = (         int  )0;
        sqlstm.sqharm[57] = (unsigned int  )0;
        sqlstm.sqadto[57] = (unsigned short )0;
        sqlstm.sqtdso[57] = (unsigned short )0;
        sqlstm.sqhstv[58] = (         void  *)(tlBcTransLog.center_sys_tra_no);
        sqlstm.sqhstl[58] = (unsigned int  )7;
        sqlstm.sqhsts[58] = (         int  )0;
        sqlstm.sqindv[58] = (         void  *)0;
        sqlstm.sqinds[58] = (         int  )0;
        sqlstm.sqharm[58] = (unsigned int  )0;
        sqlstm.sqadto[58] = (unsigned short )0;
        sqlstm.sqtdso[58] = (unsigned short )0;
        sqlstm.sqhstv[59] = (         void  *)(tlBcTransLog.center_term_id);
        sqlstm.sqhstl[59] = (unsigned int  )9;
        sqlstm.sqhsts[59] = (         int  )0;
        sqlstm.sqindv[59] = (         void  *)0;
        sqlstm.sqinds[59] = (         int  )0;
        sqlstm.sqharm[59] = (unsigned int  )0;
        sqlstm.sqadto[59] = (unsigned short )0;
        sqlstm.sqtdso[59] = (unsigned short )0;
        sqlstm.sqhstv[60] = (         void  *)(tlBcTransLog.center_mchnt_cd);
        sqlstm.sqhstl[60] = (unsigned int  )21;
        sqlstm.sqhsts[60] = (         int  )0;
        sqlstm.sqindv[60] = (         void  *)0;
        sqlstm.sqinds[60] = (         int  )0;
        sqlstm.sqharm[60] = (unsigned int  )0;
        sqlstm.sqadto[60] = (unsigned short )0;
        sqlstm.sqtdso[60] = (unsigned short )0;
        sqlstm.sqhstv[61] = (         void  *)(tlBcTransLog.bill_id_rcv);
        sqlstm.sqhstl[61] = (unsigned int  )22;
        sqlstm.sqhsts[61] = (         int  )0;
        sqlstm.sqindv[61] = (         void  *)0;
        sqlstm.sqinds[61] = (         int  )0;
        sqlstm.sqharm[61] = (unsigned int  )0;
        sqlstm.sqadto[61] = (unsigned short )0;
        sqlstm.sqtdso[61] = (unsigned short )0;
        sqlstm.sqhstv[62] = (         void  *)(tlBcTransLog.card_attr);
        sqlstm.sqhstl[62] = (unsigned int  )3;
        sqlstm.sqhsts[62] = (         int  )0;
        sqlstm.sqindv[62] = (         void  *)0;
        sqlstm.sqinds[62] = (         int  )0;
        sqlstm.sqharm[62] = (unsigned int  )0;
        sqlstm.sqadto[62] = (unsigned short )0;
        sqlstm.sqtdso[62] = (unsigned short )0;
        sqlstm.sqhstv[63] = (         void  *)(tlBcTransLog.iss_ins_id_cd);
        sqlstm.sqhstl[63] = (unsigned int  )9;
        sqlstm.sqhsts[63] = (         int  )0;
        sqlstm.sqindv[63] = (         void  *)0;
        sqlstm.sqinds[63] = (         int  )0;
        sqlstm.sqharm[63] = (unsigned int  )0;
        sqlstm.sqadto[63] = (unsigned short )0;
        sqlstm.sqtdso[63] = (unsigned short )0;
        sqlstm.sqhstv[64] = (         void  *)(tlBcTransLog.pname);
        sqlstm.sqhstl[64] = (unsigned int  )211;
        sqlstm.sqhsts[64] = (         int  )0;
        sqlstm.sqindv[64] = (         void  *)0;
        sqlstm.sqinds[64] = (         int  )0;
        sqlstm.sqharm[64] = (unsigned int  )0;
        sqlstm.sqadto[64] = (unsigned short )0;
        sqlstm.sqtdso[64] = (unsigned short )0;
        sqlstm.sqhstv[65] = (         void  *)(tlBcTransLog.encrypt_info);
        sqlstm.sqhstl[65] = (unsigned int  )49;
        sqlstm.sqhsts[65] = (         int  )0;
        sqlstm.sqindv[65] = (         void  *)0;
        sqlstm.sqinds[65] = (         int  )0;
        sqlstm.sqharm[65] = (unsigned int  )0;
        sqlstm.sqadto[65] = (unsigned short )0;
        sqlstm.sqtdso[65] = (unsigned short )0;
        sqlstm.sqhstv[66] = (         void  *)(tlBcTransLog.id_type);
        sqlstm.sqhstl[66] = (unsigned int  )3;
        sqlstm.sqhsts[66] = (         int  )0;
        sqlstm.sqindv[66] = (         void  *)0;
        sqlstm.sqinds[66] = (         int  )0;
        sqlstm.sqharm[66] = (unsigned int  )0;
        sqlstm.sqadto[66] = (unsigned short )0;
        sqlstm.sqtdso[66] = (unsigned short )0;
        sqlstm.sqhstv[67] = (         void  *)(tlBcTransLog.originaDataElements);
        sqlstm.sqhstl[67] = (unsigned int  )44;
        sqlstm.sqhsts[67] = (         int  )0;
        sqlstm.sqindv[67] = (         void  *)0;
        sqlstm.sqinds[67] = (         int  )0;
        sqlstm.sqharm[67] = (unsigned int  )0;
        sqlstm.sqadto[67] = (unsigned short )0;
        sqlstm.sqtdso[67] = (unsigned short )0;
        sqlstm.sqhstv[68] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[68] = (unsigned int  )5;
        sqlstm.sqhsts[68] = (         int  )0;
        sqlstm.sqindv[68] = (         void  *)0;
        sqlstm.sqinds[68] = (         int  )0;
        sqlstm.sqharm[68] = (unsigned int  )0;
        sqlstm.sqadto[68] = (unsigned short )0;
        sqlstm.sqtdso[68] = (unsigned short )0;
        sqlstm.sqhstv[69] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[69] = (unsigned int  )21;
        sqlstm.sqhsts[69] = (         int  )0;
        sqlstm.sqindv[69] = (         void  *)0;
        sqlstm.sqinds[69] = (         int  )0;
        sqlstm.sqharm[69] = (unsigned int  )0;
        sqlstm.sqadto[69] = (unsigned short )0;
        sqlstm.sqtdso[69] = (unsigned short )0;
        sqlstm.sqhstv[70] = (         void  *)term_id;
        sqlstm.sqhstl[70] = (unsigned int  )9;
        sqlstm.sqhsts[70] = (         int  )0;
        sqlstm.sqindv[70] = (         void  *)0;
        sqlstm.sqinds[70] = (         int  )0;
        sqlstm.sqharm[70] = (unsigned int  )0;
        sqlstm.sqadto[70] = (unsigned short )0;
        sqlstm.sqtdso[70] = (unsigned short )0;
        sqlstm.sqhstv[71] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[71] = (unsigned int  )7;
        sqlstm.sqhsts[71] = (         int  )0;
        sqlstm.sqindv[71] = (         void  *)0;
        sqlstm.sqinds[71] = (         int  )0;
        sqlstm.sqharm[71] = (unsigned int  )0;
        sqlstm.sqadto[71] = (unsigned short )0;
        sqlstm.sqtdso[71] = (unsigned short )0;
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
            memcpy(p_tlBcTransLog, &tlBcTransLog, sizeof(tl_bc_trans_log_def));

            if ( '1' == *tlBcTransLog.flag_lock )
            {
 				memset( lock_tm, 0, sizeof(lock_tm) );
				glb_GetLocalTimeString(lock_tm,sizeof(lock_tm)-1,"%Y");            	  
				strcpy_safe(lock_tm+4,tlBcTransLog.lock_tm, 10);
				glb_mktime(&timeold, lock_tm);
				time(&timenew);
				if( 60 < timenew - timeold )
				{
	        	  	*tlBcTransLog.flag_lock = '0';
	        	  	glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT11>警告，锁标记为已锁定,但锁定时间超时(%s)，自动解锁。 (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n",lock_tm, loc_trans_dt,term_id,mchnt_cd,sys_tra_no , sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
				}
				else
				{
	               *p_sqlCode = SQL_ERD_LOCKED;                
	                glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT13>警告，锁标记为已锁定。 (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
	            }
            } else
            {
                glb_GetLocalTimeString(lock_tm,11,"%m%d%H%M%S");
                /* EXEC SQL UPDATE tl_bc_trans_log SET flag_lock='1',lock_tm=:lock_tm 
			        WHERE loc_trans_dt=:loc_trans_dt
							AND center_mchnt_cd=:mchnt_cd
							AND center_term_id=:term_id
							AND center_sys_tra_no=:sys_tra_no
							AND rownum<=1
			               ; */ 

{
                struct sqlexd sqlstm;
                sqlorat((void **)0, &sqlctx, &oraca);
                sqlstm.sqlvsn = 12;
                sqlstm.arrsiz = 72;
                sqlstm.sqladtp = &sqladt;
                sqlstm.sqltdsp = &sqltds;
                sqlstm.stmt = "update tl_bc_trans_log  set flag_lock='1',lo\
ck_tm=:b0 where ((((loc_trans_dt=:b1 and center_mchnt_cd=:b2) and center_term\
_id=:b3) and center_sys_tra_no=:b4) and rownum<=1)";
                sqlstm.iters = (unsigned int  )1;
                sqlstm.offset = (unsigned int  )1611;
                sqlstm.cud = sqlcud0;
                sqlstm.sqlest = (unsigned char  *)&sqlca;
                sqlstm.sqlety = (unsigned short)4352;
                sqlstm.occurs = (unsigned int  )0;
                sqlstm.sqhstv[0] = (         void  *)lock_tm;
                sqlstm.sqhstl[0] = (unsigned int  )15;
                sqlstm.sqhsts[0] = (         int  )0;
                sqlstm.sqindv[0] = (         void  *)0;
                sqlstm.sqinds[0] = (         int  )0;
                sqlstm.sqharm[0] = (unsigned int  )0;
                sqlstm.sqadto[0] = (unsigned short )0;
                sqlstm.sqtdso[0] = (unsigned short )0;
                sqlstm.sqhstv[1] = (         void  *)loc_trans_dt;
                sqlstm.sqhstl[1] = (unsigned int  )5;
                sqlstm.sqhsts[1] = (         int  )0;
                sqlstm.sqindv[1] = (         void  *)0;
                sqlstm.sqinds[1] = (         int  )0;
                sqlstm.sqharm[1] = (unsigned int  )0;
                sqlstm.sqadto[1] = (unsigned short )0;
                sqlstm.sqtdso[1] = (unsigned short )0;
                sqlstm.sqhstv[2] = (         void  *)mchnt_cd;
                sqlstm.sqhstl[2] = (unsigned int  )21;
                sqlstm.sqhsts[2] = (         int  )0;
                sqlstm.sqindv[2] = (         void  *)0;
                sqlstm.sqinds[2] = (         int  )0;
                sqlstm.sqharm[2] = (unsigned int  )0;
                sqlstm.sqadto[2] = (unsigned short )0;
                sqlstm.sqtdso[2] = (unsigned short )0;
                sqlstm.sqhstv[3] = (         void  *)term_id;
                sqlstm.sqhstl[3] = (unsigned int  )9;
                sqlstm.sqhsts[3] = (         int  )0;
                sqlstm.sqindv[3] = (         void  *)0;
                sqlstm.sqinds[3] = (         int  )0;
                sqlstm.sqharm[3] = (unsigned int  )0;
                sqlstm.sqadto[3] = (unsigned short )0;
                sqlstm.sqtdso[3] = (unsigned short )0;
                sqlstm.sqhstv[4] = (         void  *)sys_tra_no;
                sqlstm.sqhstl[4] = (unsigned int  )7;
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


                if ( sqlca.sqlcode == 0 )
                {
                    *p_sqlCode = 0;
                } else
                {
                    *p_sqlCode = sqlca.sqlcode;
                    glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT13>更新锁标记失败 (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
                }
            }

        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT13>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0,"<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:SELECT13>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )1646;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    case GLB_DB_INSERT1:    
        /*copy buff*/
        memcpy(&tlBcTransLog, p_tlBcTransLog, sizeof(tl_bc_trans_log_def));

        /* add other field*/
        glb_GetLocalTimeString(tlBcTransLog.sys_dt_tm, sizeof(tlBcTransLog.sys_dt_tm),"%Y%m%d%H%M%S");
        ADJUST_STRING(tlBcTransLog.sys_dt_tm         )
        ADJUST_STRING(tlBcTransLog.settle_dt         )
        ADJUST_STRING(tlBcTransLog.trans_flag        )
        ADJUST_STRING(tlBcTransLog.is_over           )
        ADJUST_STRING(tlBcTransLog.lock_tm           )
        ADJUST_STRING(tlBcTransLog.flag_lock         )
        ADJUST_STRING(tlBcTransLog.flag_verifi       )
        ADJUST_STRING(tlBcTransLog.flag_rcv          )
        ADJUST_STRING(tlBcTransLog.flag_pay          )
        ADJUST_STRING(tlBcTransLog.flag_reversal     )
        ADJUST_STRING(tlBcTransLog.resp_cd_verifi    )
        ADJUST_STRING(tlBcTransLog.resp_cd_rcv       )
        ADJUST_STRING(tlBcTransLog.resp_cd_pay       )
        ADJUST_STRING(tlBcTransLog.resp_cd_reversal  )
        ADJUST_STRING(tlBcTransLog.trans_chnl        )  
        ADJUST_STRING(tlBcTransLog.packet_type       )  
        ADJUST_STRING(tlBcTransLog.line_nm_verifi    )
        ADJUST_STRING(tlBcTransLog.line_nm_rcv       )
        ADJUST_STRING(tlBcTransLog.line_nm_pay       )
        ADJUST_STRING(tlBcTransLog.reversal_id       )
        ADJUST_STRING(tlBcTransLog.bc_settle_in      )
        ADJUST_STRING(tlBcTransLog.fld32_ins_id_cd   )
        ADJUST_STRING(tlBcTransLog.fwd_ins_id_cd     )
        ADJUST_STRING(tlBcTransLog.rcv_ins_id_cd     )
        ADJUST_STRING(tlBcTransLog.msg_tp            )
        ADJUST_STRING(tlBcTransLog.pri_acct_no       )
        ADJUST_STRING(tlBcTransLog.proc_cd           )
        glb_Adjust_Decimal(tlBcTransLog.trans_at, tlBcTransLog.trans_at, 12);
        ADJUST_STRING(tlBcTransLog.transmsn_dt_tm    )
        ADJUST_STRING(tlBcTransLog.sys_tra_no        )
        ADJUST_STRING(tlBcTransLog.loc_trans_tm      )
        ADJUST_STRING(tlBcTransLog.loc_trans_dt      )
        ADJUST_STRING(tlBcTransLog.mchnt_tp          )
        ADJUST_STRING(tlBcTransLog.pos_entry_md_cd   )
        ADJUST_STRING(tlBcTransLog.pos_cond_cd       )
        ADJUST_STRING(tlBcTransLog.retri_ref_no      )
        ADJUST_STRING(tlBcTransLog.auth_id_resp_cd   )
        ADJUST_STRING(tlBcTransLog.resp_cd           )
        ADJUST_STRING(tlBcTransLog.term_id           )
        ADJUST_STRING(tlBcTransLog.mchnt_cd          )
        ADJUST_STRING(tlBcTransLog.area_cd           )
        ADJUST_STRING(tlBcTransLog.trans_curr_cd     )
        ADJUST_STRING(tlBcTransLog.flag_1            )
        ADJUST_STRING(tlBcTransLog.flag_2            )
        ADJUST_STRING(tlBcTransLog.flag_3            )
        ADJUST_STRING(tlBcTransLog.flag_4            )
        ADJUST_STRING(tlBcTransLog.flag_5            )
        ADJUST_STRING(tlBcTransLog.flag_6            )
        ADJUST_STRING(tlBcTransLog.flag_7            )
        ADJUST_STRING(tlBcTransLog.fld_5             )
        ADJUST_STRING(tlBcTransLog.fld_8             )
        ADJUST_STRING(tlBcTransLog.fld_28            )
        ADJUST_STRING(tlBcTransLog.fld_34            )
        ADJUST_STRING(tlBcTransLog.fld_44            )
        ADJUST_STRING(tlBcTransLog.fld_45            )
        ADJUST_STRING(tlBcTransLog.fld_46            )
        ADJUST_STRING(tlBcTransLog.fld_48            )
        ADJUST_STRING(tlBcTransLog.fld_61            )
        ADJUST_STRING(tlBcTransLog.center_sys_tra_no )
        ADJUST_STRING(tlBcTransLog.center_term_id    )
        ADJUST_STRING(tlBcTransLog.center_mchnt_cd   )
        ADJUST_STRING(tlBcTransLog.bill_id_rcv       )        
        ADJUST_STRING(tlBcTransLog.card_attr         )
        ADJUST_STRING(tlBcTransLog.iss_ins_id_cd     )
        ADJUST_STRING(tlBcTransLog.pname             )
        ADJUST_STRING(tlBcTransLog.encrypt_info      )
        ADJUST_STRING(tlBcTransLog.id_type           )
        ADJUST_STRING(tlBcTransLog.originaDataElements			 )

        /* EXEC SQL insert into tl_bc_trans_log(sys_dt_tm,settle_dt,trans_flag,is_over,lock_tm,flag_lock,flag_verifi,flag_rcv,flag_pay,flag_reversal,resp_cd_verifi,resp_cd_rcv,resp_cd_pay,resp_cd_reversal,trans_chnl,packet_type,line_nm_verifi,line_nm_rcv,line_nm_pay,reversal_id,bc_settle_in,fld32_ins_id_cd,fwd_ins_id_cd,rcv_ins_id_cd,msg_tp,pri_acct_no,proc_cd,trans_at,transmsn_dt_tm,sys_tra_no,loc_trans_tm,loc_trans_dt,mchnt_tp,pos_entry_md_cd,pos_cond_cd,retri_ref_no,auth_id_resp_cd,resp_cd,term_id,mchnt_cd,area_cd,trans_curr_cd,flag_1,flag_2,flag_3,flag_4,flag_5,flag_6,flag_7,fld_5,fld_8,fld_28,fld_34,fld_44,fld_45,fld_46,fld_48,fld_61,center_sys_tra_no,center_term_id,center_mchnt_cd,bill_id_rcv,card_attr,iss_ins_id_cd,pname,encrypt_info,id_type,orig_data_elemts)values
        (
        :tlBcTransLog.sys_dt_tm         ,
        :tlBcTransLog.settle_dt         ,
        :tlBcTransLog.trans_flag        ,
        :tlBcTransLog.is_over           ,
        :tlBcTransLog.lock_tm           ,
        :tlBcTransLog.flag_lock         ,
        :tlBcTransLog.flag_verifi       ,
        :tlBcTransLog.flag_rcv          ,
        :tlBcTransLog.flag_pay          ,
        :tlBcTransLog.flag_reversal     ,
        :tlBcTransLog.resp_cd_verifi    ,
        :tlBcTransLog.resp_cd_rcv       ,
        :tlBcTransLog.resp_cd_pay       ,
        :tlBcTransLog.resp_cd_reversal  ,
        :tlBcTransLog.trans_chnl        ,
        :tlBcTransLog.packet_type       ,
        :tlBcTransLog.line_nm_verifi    ,
        :tlBcTransLog.line_nm_rcv       ,
        :tlBcTransLog.line_nm_pay       ,
        :tlBcTransLog.reversal_id       ,
        :tlBcTransLog.bc_settle_in      ,
        :tlBcTransLog.fld32_ins_id_cd   ,
        :tlBcTransLog.fwd_ins_id_cd     ,
        :tlBcTransLog.rcv_ins_id_cd     ,
        :tlBcTransLog.msg_tp            ,
        :tlBcTransLog.pri_acct_no       ,
        :tlBcTransLog.proc_cd           ,
        :tlBcTransLog.trans_at          ,
        :tlBcTransLog.transmsn_dt_tm    ,
        :tlBcTransLog.sys_tra_no        ,
        :tlBcTransLog.loc_trans_tm      ,
        :tlBcTransLog.loc_trans_dt      ,
        :tlBcTransLog.mchnt_tp          ,
        :tlBcTransLog.pos_entry_md_cd   ,
        :tlBcTransLog.pos_cond_cd       ,
        :tlBcTransLog.retri_ref_no      ,
        :tlBcTransLog.auth_id_resp_cd   ,
        :tlBcTransLog.resp_cd           ,
        :tlBcTransLog.term_id           ,
        :tlBcTransLog.mchnt_cd          ,
        :tlBcTransLog.area_cd           ,
        :tlBcTransLog.trans_curr_cd     ,
        :tlBcTransLog.flag_1            ,
        :tlBcTransLog.flag_2            ,
        :tlBcTransLog.flag_3            ,
        :tlBcTransLog.flag_4            ,
        :tlBcTransLog.flag_5            ,
        :tlBcTransLog.flag_6            ,
        :tlBcTransLog.flag_7            ,
        :tlBcTransLog.fld_5             ,
        :tlBcTransLog.fld_8             ,
        :tlBcTransLog.fld_28            ,
        :tlBcTransLog.fld_34            ,
        :tlBcTransLog.fld_44            ,
        :tlBcTransLog.fld_45            ,
        :tlBcTransLog.fld_46            ,
        :tlBcTransLog.fld_48            ,
        :tlBcTransLog.fld_61            ,
        :tlBcTransLog.center_sys_tra_no ,
        :tlBcTransLog.center_term_id    ,
        :tlBcTransLog.center_mchnt_cd   ,
        :tlBcTransLog.bill_id_rcv       ,
        :tlBcTransLog.card_attr         ,
        :tlBcTransLog.iss_ins_id_cd     ,
        :tlBcTransLog.pname             ,
        :tlBcTransLog.encrypt_info      ,
        :tlBcTransLog.id_type		,
        :tlBcTransLog.originaDataElements
        )
        ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlbuft((void **)0,
          "insert into tl_bc_trans_log (sys_dt_tm,settle_dt,trans_flag,is_ov\
er,lock_tm,flag_lock,flag_verifi,flag_rcv,flag_pay,flag_reversal,resp_cd_ver\
ifi,resp_cd_rcv,resp_cd_pay,resp_cd_reversal,trans_chnl,packet_type,line_nm_\
verifi,line_nm_rcv,line_nm_pay,reversal_id,bc_settle_in,fld32_ins_id_cd,fwd_\
ins_id_cd,rcv_ins_id_cd,msg_tp,pri_acct_no,proc_cd,trans_at,transmsn_dt_tm,s\
ys_tra_no,loc_trans_tm,loc_trans_dt,mchnt_tp,pos_entry_md_cd,pos_cond_cd,ret\
ri_ref_no,auth_id_resp_cd,resp_cd,term_id,mchnt_cd,area_cd,trans_curr_cd,fla\
g_1,flag_2,flag_3,flag_4,flag_5,flag_6,flag_7,fld_5,fld_8,fld_28,fld_34,fld_\
44,fld_45,fld_46,fld_48,fld_61,center_sys_tra_no,center_term_id,center_mchnt\
_cd,bill_id_rcv,card_attr,iss_ins_id_cd,pname,encrypt_info,id_type,orig_data\
_elemts) values (:b0,:b1,:b2,:b3,:b4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13\
,:b14,:b15,:b16,:b17,:b18,:b19,:b20,:b21,:b22,:b23,:b24,:b25,:b26,:b27,:b28,\
:b29,:b30,:b31,:b32,:b33,:b34,:b35,:b36,:b37,:b38,:b39,:b40,:b41,:b42,:b43,:\
b44,:b45,:b46,:b47,:b48,:b49,:b50,:b51,:b52,:b5");
        sqlstm.stmt = "3,:b54,:b55,:b56,:b57,:b58,:b59,:b60,:b61,:b62,:b63,\
:b64,:b65,:b66,:b67)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )1661;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcTransLog.sys_dt_tm);
        sqlstm.sqhstl[0] = (unsigned int  )15;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.settle_dt);
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.trans_flag);
        sqlstm.sqhstl[2] = (unsigned int  )5;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.is_over);
        sqlstm.sqhstl[3] = (unsigned int  )2;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.lock_tm);
        sqlstm.sqhstl[4] = (unsigned int  )11;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.flag_lock);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcTransLog.flag_verifi);
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcTransLog.flag_rcv);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcTransLog.flag_pay);
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcTransLog.flag_reversal);
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcTransLog.resp_cd_verifi);
        sqlstm.sqhstl[10] = (unsigned int  )7;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcTransLog.resp_cd_rcv);
        sqlstm.sqhstl[11] = (unsigned int  )7;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcTransLog.resp_cd_pay);
        sqlstm.sqhstl[12] = (unsigned int  )7;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcTransLog.resp_cd_reversal);
        sqlstm.sqhstl[13] = (unsigned int  )7;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcTransLog.trans_chnl);
        sqlstm.sqhstl[14] = (unsigned int  )31;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcTransLog.packet_type);
        sqlstm.sqhstl[15] = (unsigned int  )5;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tlBcTransLog.line_nm_verifi);
        sqlstm.sqhstl[16] = (unsigned int  )5;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tlBcTransLog.line_nm_rcv);
        sqlstm.sqhstl[17] = (unsigned int  )5;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tlBcTransLog.line_nm_pay);
        sqlstm.sqhstl[18] = (unsigned int  )5;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tlBcTransLog.reversal_id);
        sqlstm.sqhstl[19] = (unsigned int  )9;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tlBcTransLog.bc_settle_in);
        sqlstm.sqhstl[20] = (unsigned int  )2;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tlBcTransLog.fld32_ins_id_cd);
        sqlstm.sqhstl[21] = (unsigned int  )14;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tlBcTransLog.fwd_ins_id_cd);
        sqlstm.sqhstl[22] = (unsigned int  )14;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tlBcTransLog.rcv_ins_id_cd);
        sqlstm.sqhstl[23] = (unsigned int  )14;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tlBcTransLog.msg_tp);
        sqlstm.sqhstl[24] = (unsigned int  )5;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tlBcTransLog.pri_acct_no);
        sqlstm.sqhstl[25] = (unsigned int  )22;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tlBcTransLog.proc_cd);
        sqlstm.sqhstl[26] = (unsigned int  )7;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tlBcTransLog.trans_at);
        sqlstm.sqhstl[27] = (unsigned int  )13;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tlBcTransLog.transmsn_dt_tm);
        sqlstm.sqhstl[28] = (unsigned int  )11;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tlBcTransLog.sys_tra_no);
        sqlstm.sqhstl[29] = (unsigned int  )7;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tlBcTransLog.loc_trans_tm);
        sqlstm.sqhstl[30] = (unsigned int  )7;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)(tlBcTransLog.loc_trans_dt);
        sqlstm.sqhstl[31] = (unsigned int  )5;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqhstv[32] = (         void  *)(tlBcTransLog.mchnt_tp);
        sqlstm.sqhstl[32] = (unsigned int  )5;
        sqlstm.sqhsts[32] = (         int  )0;
        sqlstm.sqindv[32] = (         void  *)0;
        sqlstm.sqinds[32] = (         int  )0;
        sqlstm.sqharm[32] = (unsigned int  )0;
        sqlstm.sqadto[32] = (unsigned short )0;
        sqlstm.sqtdso[32] = (unsigned short )0;
        sqlstm.sqhstv[33] = (         void  *)(tlBcTransLog.pos_entry_md_cd);
        sqlstm.sqhstl[33] = (unsigned int  )4;
        sqlstm.sqhsts[33] = (         int  )0;
        sqlstm.sqindv[33] = (         void  *)0;
        sqlstm.sqinds[33] = (         int  )0;
        sqlstm.sqharm[33] = (unsigned int  )0;
        sqlstm.sqadto[33] = (unsigned short )0;
        sqlstm.sqtdso[33] = (unsigned short )0;
        sqlstm.sqhstv[34] = (         void  *)(tlBcTransLog.pos_cond_cd);
        sqlstm.sqhstl[34] = (unsigned int  )3;
        sqlstm.sqhsts[34] = (         int  )0;
        sqlstm.sqindv[34] = (         void  *)0;
        sqlstm.sqinds[34] = (         int  )0;
        sqlstm.sqharm[34] = (unsigned int  )0;
        sqlstm.sqadto[34] = (unsigned short )0;
        sqlstm.sqtdso[34] = (unsigned short )0;
        sqlstm.sqhstv[35] = (         void  *)(tlBcTransLog.retri_ref_no);
        sqlstm.sqhstl[35] = (unsigned int  )13;
        sqlstm.sqhsts[35] = (         int  )0;
        sqlstm.sqindv[35] = (         void  *)0;
        sqlstm.sqinds[35] = (         int  )0;
        sqlstm.sqharm[35] = (unsigned int  )0;
        sqlstm.sqadto[35] = (unsigned short )0;
        sqlstm.sqtdso[35] = (unsigned short )0;
        sqlstm.sqhstv[36] = (         void  *)(tlBcTransLog.auth_id_resp_cd);
        sqlstm.sqhstl[36] = (unsigned int  )7;
        sqlstm.sqhsts[36] = (         int  )0;
        sqlstm.sqindv[36] = (         void  *)0;
        sqlstm.sqinds[36] = (         int  )0;
        sqlstm.sqharm[36] = (unsigned int  )0;
        sqlstm.sqadto[36] = (unsigned short )0;
        sqlstm.sqtdso[36] = (unsigned short )0;
        sqlstm.sqhstv[37] = (         void  *)(tlBcTransLog.resp_cd);
        sqlstm.sqhstl[37] = (unsigned int  )3;
        sqlstm.sqhsts[37] = (         int  )0;
        sqlstm.sqindv[37] = (         void  *)0;
        sqlstm.sqinds[37] = (         int  )0;
        sqlstm.sqharm[37] = (unsigned int  )0;
        sqlstm.sqadto[37] = (unsigned short )0;
        sqlstm.sqtdso[37] = (unsigned short )0;
        sqlstm.sqhstv[38] = (         void  *)(tlBcTransLog.term_id);
        sqlstm.sqhstl[38] = (unsigned int  )9;
        sqlstm.sqhsts[38] = (         int  )0;
        sqlstm.sqindv[38] = (         void  *)0;
        sqlstm.sqinds[38] = (         int  )0;
        sqlstm.sqharm[38] = (unsigned int  )0;
        sqlstm.sqadto[38] = (unsigned short )0;
        sqlstm.sqtdso[38] = (unsigned short )0;
        sqlstm.sqhstv[39] = (         void  *)(tlBcTransLog.mchnt_cd);
        sqlstm.sqhstl[39] = (unsigned int  )16;
        sqlstm.sqhsts[39] = (         int  )0;
        sqlstm.sqindv[39] = (         void  *)0;
        sqlstm.sqinds[39] = (         int  )0;
        sqlstm.sqharm[39] = (unsigned int  )0;
        sqlstm.sqadto[39] = (unsigned short )0;
        sqlstm.sqtdso[39] = (unsigned short )0;
        sqlstm.sqhstv[40] = (         void  *)(tlBcTransLog.area_cd);
        sqlstm.sqhstl[40] = (unsigned int  )7;
        sqlstm.sqhsts[40] = (         int  )0;
        sqlstm.sqindv[40] = (         void  *)0;
        sqlstm.sqinds[40] = (         int  )0;
        sqlstm.sqharm[40] = (unsigned int  )0;
        sqlstm.sqadto[40] = (unsigned short )0;
        sqlstm.sqtdso[40] = (unsigned short )0;
        sqlstm.sqhstv[41] = (         void  *)(tlBcTransLog.trans_curr_cd);
        sqlstm.sqhstl[41] = (unsigned int  )4;
        sqlstm.sqhsts[41] = (         int  )0;
        sqlstm.sqindv[41] = (         void  *)0;
        sqlstm.sqinds[41] = (         int  )0;
        sqlstm.sqharm[41] = (unsigned int  )0;
        sqlstm.sqadto[41] = (unsigned short )0;
        sqlstm.sqtdso[41] = (unsigned short )0;
        sqlstm.sqhstv[42] = (         void  *)(tlBcTransLog.flag_1);
        sqlstm.sqhstl[42] = (unsigned int  )2;
        sqlstm.sqhsts[42] = (         int  )0;
        sqlstm.sqindv[42] = (         void  *)0;
        sqlstm.sqinds[42] = (         int  )0;
        sqlstm.sqharm[42] = (unsigned int  )0;
        sqlstm.sqadto[42] = (unsigned short )0;
        sqlstm.sqtdso[42] = (unsigned short )0;
        sqlstm.sqhstv[43] = (         void  *)(tlBcTransLog.flag_2);
        sqlstm.sqhstl[43] = (unsigned int  )2;
        sqlstm.sqhsts[43] = (         int  )0;
        sqlstm.sqindv[43] = (         void  *)0;
        sqlstm.sqinds[43] = (         int  )0;
        sqlstm.sqharm[43] = (unsigned int  )0;
        sqlstm.sqadto[43] = (unsigned short )0;
        sqlstm.sqtdso[43] = (unsigned short )0;
        sqlstm.sqhstv[44] = (         void  *)(tlBcTransLog.flag_3);
        sqlstm.sqhstl[44] = (unsigned int  )2;
        sqlstm.sqhsts[44] = (         int  )0;
        sqlstm.sqindv[44] = (         void  *)0;
        sqlstm.sqinds[44] = (         int  )0;
        sqlstm.sqharm[44] = (unsigned int  )0;
        sqlstm.sqadto[44] = (unsigned short )0;
        sqlstm.sqtdso[44] = (unsigned short )0;
        sqlstm.sqhstv[45] = (         void  *)(tlBcTransLog.flag_4);
        sqlstm.sqhstl[45] = (unsigned int  )2;
        sqlstm.sqhsts[45] = (         int  )0;
        sqlstm.sqindv[45] = (         void  *)0;
        sqlstm.sqinds[45] = (         int  )0;
        sqlstm.sqharm[45] = (unsigned int  )0;
        sqlstm.sqadto[45] = (unsigned short )0;
        sqlstm.sqtdso[45] = (unsigned short )0;
        sqlstm.sqhstv[46] = (         void  *)(tlBcTransLog.flag_5);
        sqlstm.sqhstl[46] = (unsigned int  )2;
        sqlstm.sqhsts[46] = (         int  )0;
        sqlstm.sqindv[46] = (         void  *)0;
        sqlstm.sqinds[46] = (         int  )0;
        sqlstm.sqharm[46] = (unsigned int  )0;
        sqlstm.sqadto[46] = (unsigned short )0;
        sqlstm.sqtdso[46] = (unsigned short )0;
        sqlstm.sqhstv[47] = (         void  *)(tlBcTransLog.flag_6);
        sqlstm.sqhstl[47] = (unsigned int  )2;
        sqlstm.sqhsts[47] = (         int  )0;
        sqlstm.sqindv[47] = (         void  *)0;
        sqlstm.sqinds[47] = (         int  )0;
        sqlstm.sqharm[47] = (unsigned int  )0;
        sqlstm.sqadto[47] = (unsigned short )0;
        sqlstm.sqtdso[47] = (unsigned short )0;
        sqlstm.sqhstv[48] = (         void  *)(tlBcTransLog.flag_7);
        sqlstm.sqhstl[48] = (unsigned int  )2;
        sqlstm.sqhsts[48] = (         int  )0;
        sqlstm.sqindv[48] = (         void  *)0;
        sqlstm.sqinds[48] = (         int  )0;
        sqlstm.sqharm[48] = (unsigned int  )0;
        sqlstm.sqadto[48] = (unsigned short )0;
        sqlstm.sqtdso[48] = (unsigned short )0;
        sqlstm.sqhstv[49] = (         void  *)(tlBcTransLog.fld_5);
        sqlstm.sqhstl[49] = (unsigned int  )13;
        sqlstm.sqhsts[49] = (         int  )0;
        sqlstm.sqindv[49] = (         void  *)0;
        sqlstm.sqinds[49] = (         int  )0;
        sqlstm.sqharm[49] = (unsigned int  )0;
        sqlstm.sqadto[49] = (unsigned short )0;
        sqlstm.sqtdso[49] = (unsigned short )0;
        sqlstm.sqhstv[50] = (         void  *)(tlBcTransLog.fld_8);
        sqlstm.sqhstl[50] = (unsigned int  )9;
        sqlstm.sqhsts[50] = (         int  )0;
        sqlstm.sqindv[50] = (         void  *)0;
        sqlstm.sqinds[50] = (         int  )0;
        sqlstm.sqharm[50] = (unsigned int  )0;
        sqlstm.sqadto[50] = (unsigned short )0;
        sqlstm.sqtdso[50] = (unsigned short )0;
        sqlstm.sqhstv[51] = (         void  *)(tlBcTransLog.fld_28);
        sqlstm.sqhstl[51] = (unsigned int  )7;
        sqlstm.sqhsts[51] = (         int  )0;
        sqlstm.sqindv[51] = (         void  *)0;
        sqlstm.sqinds[51] = (         int  )0;
        sqlstm.sqharm[51] = (unsigned int  )0;
        sqlstm.sqadto[51] = (unsigned short )0;
        sqlstm.sqtdso[51] = (unsigned short )0;
        sqlstm.sqhstv[52] = (         void  *)(tlBcTransLog.fld_34);
        sqlstm.sqhstl[52] = (unsigned int  )29;
        sqlstm.sqhsts[52] = (         int  )0;
        sqlstm.sqindv[52] = (         void  *)0;
        sqlstm.sqinds[52] = (         int  )0;
        sqlstm.sqharm[52] = (unsigned int  )0;
        sqlstm.sqadto[52] = (unsigned short )0;
        sqlstm.sqtdso[52] = (unsigned short )0;
        sqlstm.sqhstv[53] = (         void  *)(tlBcTransLog.fld_44);
        sqlstm.sqhstl[53] = (unsigned int  )28;
        sqlstm.sqhsts[53] = (         int  )0;
        sqlstm.sqindv[53] = (         void  *)0;
        sqlstm.sqinds[53] = (         int  )0;
        sqlstm.sqharm[53] = (unsigned int  )0;
        sqlstm.sqadto[53] = (unsigned short )0;
        sqlstm.sqtdso[53] = (unsigned short )0;
        sqlstm.sqhstv[54] = (         void  *)(tlBcTransLog.fld_45);
        sqlstm.sqhstl[54] = (unsigned int  )77;
        sqlstm.sqhsts[54] = (         int  )0;
        sqlstm.sqindv[54] = (         void  *)0;
        sqlstm.sqinds[54] = (         int  )0;
        sqlstm.sqharm[54] = (unsigned int  )0;
        sqlstm.sqadto[54] = (unsigned short )0;
        sqlstm.sqtdso[54] = (unsigned short )0;
        sqlstm.sqhstv[55] = (         void  *)(tlBcTransLog.fld_46);
        sqlstm.sqhstl[55] = (unsigned int  )31;
        sqlstm.sqhsts[55] = (         int  )0;
        sqlstm.sqindv[55] = (         void  *)0;
        sqlstm.sqinds[55] = (         int  )0;
        sqlstm.sqharm[55] = (unsigned int  )0;
        sqlstm.sqadto[55] = (unsigned short )0;
        sqlstm.sqtdso[55] = (unsigned short )0;
        sqlstm.sqhstv[56] = (         void  *)(tlBcTransLog.fld_48);
        sqlstm.sqhstl[56] = (unsigned int  )191;
        sqlstm.sqhsts[56] = (         int  )0;
        sqlstm.sqindv[56] = (         void  *)0;
        sqlstm.sqinds[56] = (         int  )0;
        sqlstm.sqharm[56] = (unsigned int  )0;
        sqlstm.sqadto[56] = (unsigned short )0;
        sqlstm.sqtdso[56] = (unsigned short )0;
        sqlstm.sqhstv[57] = (         void  *)(tlBcTransLog.fld_61);
        sqlstm.sqhstl[57] = (unsigned int  )31;
        sqlstm.sqhsts[57] = (         int  )0;
        sqlstm.sqindv[57] = (         void  *)0;
        sqlstm.sqinds[57] = (         int  )0;
        sqlstm.sqharm[57] = (unsigned int  )0;
        sqlstm.sqadto[57] = (unsigned short )0;
        sqlstm.sqtdso[57] = (unsigned short )0;
        sqlstm.sqhstv[58] = (         void  *)(tlBcTransLog.center_sys_tra_no);
        sqlstm.sqhstl[58] = (unsigned int  )7;
        sqlstm.sqhsts[58] = (         int  )0;
        sqlstm.sqindv[58] = (         void  *)0;
        sqlstm.sqinds[58] = (         int  )0;
        sqlstm.sqharm[58] = (unsigned int  )0;
        sqlstm.sqadto[58] = (unsigned short )0;
        sqlstm.sqtdso[58] = (unsigned short )0;
        sqlstm.sqhstv[59] = (         void  *)(tlBcTransLog.center_term_id);
        sqlstm.sqhstl[59] = (unsigned int  )9;
        sqlstm.sqhsts[59] = (         int  )0;
        sqlstm.sqindv[59] = (         void  *)0;
        sqlstm.sqinds[59] = (         int  )0;
        sqlstm.sqharm[59] = (unsigned int  )0;
        sqlstm.sqadto[59] = (unsigned short )0;
        sqlstm.sqtdso[59] = (unsigned short )0;
        sqlstm.sqhstv[60] = (         void  *)(tlBcTransLog.center_mchnt_cd);
        sqlstm.sqhstl[60] = (unsigned int  )21;
        sqlstm.sqhsts[60] = (         int  )0;
        sqlstm.sqindv[60] = (         void  *)0;
        sqlstm.sqinds[60] = (         int  )0;
        sqlstm.sqharm[60] = (unsigned int  )0;
        sqlstm.sqadto[60] = (unsigned short )0;
        sqlstm.sqtdso[60] = (unsigned short )0;
        sqlstm.sqhstv[61] = (         void  *)(tlBcTransLog.bill_id_rcv);
        sqlstm.sqhstl[61] = (unsigned int  )22;
        sqlstm.sqhsts[61] = (         int  )0;
        sqlstm.sqindv[61] = (         void  *)0;
        sqlstm.sqinds[61] = (         int  )0;
        sqlstm.sqharm[61] = (unsigned int  )0;
        sqlstm.sqadto[61] = (unsigned short )0;
        sqlstm.sqtdso[61] = (unsigned short )0;
        sqlstm.sqhstv[62] = (         void  *)(tlBcTransLog.card_attr);
        sqlstm.sqhstl[62] = (unsigned int  )3;
        sqlstm.sqhsts[62] = (         int  )0;
        sqlstm.sqindv[62] = (         void  *)0;
        sqlstm.sqinds[62] = (         int  )0;
        sqlstm.sqharm[62] = (unsigned int  )0;
        sqlstm.sqadto[62] = (unsigned short )0;
        sqlstm.sqtdso[62] = (unsigned short )0;
        sqlstm.sqhstv[63] = (         void  *)(tlBcTransLog.iss_ins_id_cd);
        sqlstm.sqhstl[63] = (unsigned int  )9;
        sqlstm.sqhsts[63] = (         int  )0;
        sqlstm.sqindv[63] = (         void  *)0;
        sqlstm.sqinds[63] = (         int  )0;
        sqlstm.sqharm[63] = (unsigned int  )0;
        sqlstm.sqadto[63] = (unsigned short )0;
        sqlstm.sqtdso[63] = (unsigned short )0;
        sqlstm.sqhstv[64] = (         void  *)(tlBcTransLog.pname);
        sqlstm.sqhstl[64] = (unsigned int  )211;
        sqlstm.sqhsts[64] = (         int  )0;
        sqlstm.sqindv[64] = (         void  *)0;
        sqlstm.sqinds[64] = (         int  )0;
        sqlstm.sqharm[64] = (unsigned int  )0;
        sqlstm.sqadto[64] = (unsigned short )0;
        sqlstm.sqtdso[64] = (unsigned short )0;
        sqlstm.sqhstv[65] = (         void  *)(tlBcTransLog.encrypt_info);
        sqlstm.sqhstl[65] = (unsigned int  )49;
        sqlstm.sqhsts[65] = (         int  )0;
        sqlstm.sqindv[65] = (         void  *)0;
        sqlstm.sqinds[65] = (         int  )0;
        sqlstm.sqharm[65] = (unsigned int  )0;
        sqlstm.sqadto[65] = (unsigned short )0;
        sqlstm.sqtdso[65] = (unsigned short )0;
        sqlstm.sqhstv[66] = (         void  *)(tlBcTransLog.id_type);
        sqlstm.sqhstl[66] = (unsigned int  )3;
        sqlstm.sqhsts[66] = (         int  )0;
        sqlstm.sqindv[66] = (         void  *)0;
        sqlstm.sqinds[66] = (         int  )0;
        sqlstm.sqharm[66] = (unsigned int  )0;
        sqlstm.sqadto[66] = (unsigned short )0;
        sqlstm.sqtdso[66] = (unsigned short )0;
        sqlstm.sqhstv[67] = (         void  *)(tlBcTransLog.originaDataElements);
        sqlstm.sqhstl[67] = (unsigned int  )44;
        sqlstm.sqhsts[67] = (         int  )0;
        sqlstm.sqindv[67] = (         void  *)0;
        sqlstm.sqinds[67] = (         int  )0;
        sqlstm.sqharm[67] = (unsigned int  )0;
        sqlstm.sqadto[67] = (unsigned short )0;
        sqlstm.sqtdso[67] = (unsigned short )0;
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



        /* insert result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
        } else if ( memcmp(sqlca.sqlerrm.sqlerrmc, "ORA-00001", 9 ) == 0 )
        {
            *p_sqlCode = SQL_ERD_UNIKEY;
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:INSERT1>Insert Record is exist.sqlcode:%d,sqlca.sqlerrm.sqlerrmc:%s",sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
        } else
        {
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:INSERT1>Insert Record Failure.sqlcode:%d,sqlca.sqlerrm.sqlerrmc:%s",sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )1948;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

        /* 仅更新相关标志位字段 */
    case GLB_DB_UPDATE1:

        /*copy buff*/
        memcpy(&tlBcTransLog, p_tlBcTransLog, sizeof(tl_bc_trans_log_def));

        strcpy_safe(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, sizeof(loc_trans_dt)-1);
        rtrim_null(loc_trans_dt,' ');
        
        strcpy_safe(term_id, p_tlBcTransLog->term_id, sizeof(term_id)-1);
        rtrim_null(term_id,' ');

        strcpy_safe(mchnt_cd, p_tlBcTransLog->mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');

				 strcpy_safe(sys_tra_no, p_tlBcTransLog->sys_tra_no, sizeof(sys_tra_no)-1);
        rtrim_null(sys_tra_no,' ');        
        
        ADJUST_STRING(tlBcTransLog.settle_dt         )
        ADJUST_STRING(tlBcTransLog.trans_flag        )
        ADJUST_STRING(tlBcTransLog.is_over           )
        ADJUST_STRING(tlBcTransLog.lock_tm           )
        ADJUST_STRING(tlBcTransLog.flag_lock         )
        ADJUST_STRING(tlBcTransLog.flag_verifi       )
        ADJUST_STRING(tlBcTransLog.flag_rcv          )
        ADJUST_STRING(tlBcTransLog.flag_pay          )
        ADJUST_STRING(tlBcTransLog.flag_reversal     )
        ADJUST_STRING(tlBcTransLog.line_nm_verifi    )
        ADJUST_STRING(tlBcTransLog.line_nm_rcv       )
        ADJUST_STRING(tlBcTransLog.line_nm_pay       )
        ADJUST_STRING(tlBcTransLog.resp_cd_verifi    )
        ADJUST_STRING(tlBcTransLog.resp_cd_rcv       )
        ADJUST_STRING(tlBcTransLog.resp_cd_pay       )
        ADJUST_STRING(tlBcTransLog.resp_cd_reversal  )
        ADJUST_STRING(tlBcTransLog.reversal_id       )
        ADJUST_STRING(tlBcTransLog.bc_settle_in      )
        ADJUST_STRING(tlBcTransLog.rcv_ins_id_cd     )
        ADJUST_STRING(tlBcTransLog.retri_ref_no      )
        ADJUST_STRING(tlBcTransLog.auth_id_resp_cd   )
        ADJUST_STRING(tlBcTransLog.resp_cd           )
        ADJUST_STRING(tlBcTransLog.area_cd           )        
        ADJUST_STRING(tlBcTransLog.flag_1            )
        ADJUST_STRING(tlBcTransLog.flag_2            )
        ADJUST_STRING(tlBcTransLog.flag_3            )
        ADJUST_STRING(tlBcTransLog.flag_4            )
        ADJUST_STRING(tlBcTransLog.flag_5            )
        ADJUST_STRING(tlBcTransLog.flag_6            )
        ADJUST_STRING(tlBcTransLog.flag_7            )
        ADJUST_STRING(tlBcTransLog.fld_28            )
        ADJUST_STRING(tlBcTransLog.fld_45            )
        ADJUST_STRING(tlBcTransLog.fld_61            )
        ADJUST_STRING(tlBcTransLog.center_sys_tra_no )
        ADJUST_STRING(tlBcTransLog.center_term_id    )        
        ADJUST_STRING(tlBcTransLog.center_mchnt_cd   ) 
        ADJUST_STRING(tlBcTransLog.bill_id_rcv       )        
        ADJUST_STRING(tlBcTransLog.card_attr         )
        ADJUST_STRING(tlBcTransLog.iss_ins_id_cd     )
        ADJUST_STRING(tlBcTransLog.pname             )
        ADJUST_STRING(tlBcTransLog.encrypt_info      )
        ADJUST_STRING(tlBcTransLog.id_type           )
        ADJUST_STRING(tlBcTransLog.originaDataElements			 )
        /* EXEC SQL UPDATE tl_bc_trans_log SET
        settle_dt         = :tlBcTransLog.settle_dt         ,
        trans_flag        = :tlBcTransLog.trans_flag        ,
        is_over           = :tlBcTransLog.is_over           ,
        lock_tm           = :tlBcTransLog.lock_tm           ,
        flag_lock         = :tlBcTransLog.flag_lock         ,
        flag_verifi       = :tlBcTransLog.flag_verifi       ,
        flag_rcv          = :tlBcTransLog.flag_rcv          ,
        flag_pay          = :tlBcTransLog.flag_pay          ,
        flag_reversal     = :tlBcTransLog.flag_reversal     ,
        resp_cd_verifi    = :tlBcTransLog.resp_cd_verifi    ,
        resp_cd_rcv       = :tlBcTransLog.resp_cd_rcv       ,
        resp_cd_pay       = :tlBcTransLog.resp_cd_pay       ,
        resp_cd_reversal  = :tlBcTransLog.resp_cd_reversal  ,
        line_nm_verifi    = :tlBcTransLog.line_nm_verifi    ,
        line_nm_rcv       = :tlBcTransLog.line_nm_rcv       ,
        line_nm_pay       = :tlBcTransLog.line_nm_pay       ,
        reversal_id       = :tlBcTransLog.reversal_id       ,
        bc_settle_in      = :tlBcTransLog.bc_settle_in      ,
        rcv_ins_id_cd     = :tlBcTransLog.rcv_ins_id_cd     ,
        retri_ref_no      = :tlBcTransLog.retri_ref_no      ,
        auth_id_resp_cd   =  :tlBcTransLog.auth_id_resp_cd  ,
        resp_cd           = :tlBcTransLog.resp_cd           ,
        area_cd           = :tlBcTransLog.area_cd           ,  
        flag_1            = :tlBcTransLog.flag_1            ,
        flag_2            = :tlBcTransLog.flag_2            ,
        flag_3            = :tlBcTransLog.flag_3            ,
        flag_4            = :tlBcTransLog.flag_4            ,
        flag_5            = :tlBcTransLog.flag_5            ,
        flag_6            = :tlBcTransLog.flag_6            ,
        flag_7            = :tlBcTransLog.flag_7            ,
        fld_28            = :tlBcTransLog.fld_28            ,
        fld_45            = :tlBcTransLog.fld_45            ,
        fld_61            = :tlBcTransLog.fld_61            ,
        center_sys_tra_no = :tlBcTransLog.center_sys_tra_no ,
        center_term_id    = :tlBcTransLog.center_term_id    ,
        center_mchnt_cd   = :tlBcTransLog.center_mchnt_cd   ,
        bill_id_rcv       = :tlBcTransLog.bill_id_rcv       ,
        card_attr         = :tlBcTransLog.card_attr         ,
        iss_ins_id_cd     = :tlBcTransLog.iss_ins_id_cd     ,
        pname             = :tlBcTransLog.pname							,
        encrypt_info      = :tlBcTransLog.encrypt_info			,
        id_type  	 	      = :tlBcTransLog.id_type			,
        orig_data_elemts  = :tlBcTransLog.originaDataElements
	        WHERE loc_trans_dt=:loc_trans_dt
					AND mchnt_cd=:mchnt_cd
					AND sys_tra_no=:sys_tra_no
					AND term_id=:term_id
					AND rownum<=1
                            ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "update tl_bc_trans_log  set settle_dt=:b0,trans_flag\
=:b1,is_over=:b2,lock_tm=:b3,flag_lock=:b4,flag_verifi=:b5,flag_rcv=:b6,flag_\
pay=:b7,flag_reversal=:b8,resp_cd_verifi=:b9,resp_cd_rcv=:b10,resp_cd_pay=:b1\
1,resp_cd_reversal=:b12,line_nm_verifi=:b13,line_nm_rcv=:b14,line_nm_pay=:b15\
,reversal_id=:b16,bc_settle_in=:b17,rcv_ins_id_cd=:b18,retri_ref_no=:b19,auth\
_id_resp_cd=:b20,resp_cd=:b21,area_cd=:b22,flag_1=:b23,flag_2=:b24,flag_3=:b2\
5,flag_4=:b26,flag_5=:b27,flag_6=:b28,flag_7=:b29,fld_28=:b30,fld_45=:b31,fld\
_61=:b32,center_sys_tra_no=:b33,center_term_id=:b34,center_mchnt_cd=:b35,bill\
_id_rcv=:b36,card_attr=:b37,iss_ins_id_cd=:b38,pname=:b39,encrypt_info=:b40,i\
d_type=:b41,orig_data_elemts=:b42 where ((((loc_trans_dt=:b43 and mchnt_cd=:b\
44) and sys_tra_no=:b45) and term_id=:b46) and rownum<=1)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )1963;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcTransLog.settle_dt);
        sqlstm.sqhstl[0] = (unsigned int  )5;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.trans_flag);
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.is_over);
        sqlstm.sqhstl[2] = (unsigned int  )2;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.lock_tm);
        sqlstm.sqhstl[3] = (unsigned int  )11;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.flag_lock);
        sqlstm.sqhstl[4] = (unsigned int  )2;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.flag_verifi);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcTransLog.flag_rcv);
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcTransLog.flag_pay);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcTransLog.flag_reversal);
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcTransLog.resp_cd_verifi);
        sqlstm.sqhstl[9] = (unsigned int  )7;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcTransLog.resp_cd_rcv);
        sqlstm.sqhstl[10] = (unsigned int  )7;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcTransLog.resp_cd_pay);
        sqlstm.sqhstl[11] = (unsigned int  )7;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcTransLog.resp_cd_reversal);
        sqlstm.sqhstl[12] = (unsigned int  )7;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcTransLog.line_nm_verifi);
        sqlstm.sqhstl[13] = (unsigned int  )5;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcTransLog.line_nm_rcv);
        sqlstm.sqhstl[14] = (unsigned int  )5;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcTransLog.line_nm_pay);
        sqlstm.sqhstl[15] = (unsigned int  )5;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tlBcTransLog.reversal_id);
        sqlstm.sqhstl[16] = (unsigned int  )9;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tlBcTransLog.bc_settle_in);
        sqlstm.sqhstl[17] = (unsigned int  )2;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tlBcTransLog.rcv_ins_id_cd);
        sqlstm.sqhstl[18] = (unsigned int  )14;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tlBcTransLog.retri_ref_no);
        sqlstm.sqhstl[19] = (unsigned int  )13;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tlBcTransLog.auth_id_resp_cd);
        sqlstm.sqhstl[20] = (unsigned int  )7;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tlBcTransLog.resp_cd);
        sqlstm.sqhstl[21] = (unsigned int  )3;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tlBcTransLog.area_cd);
        sqlstm.sqhstl[22] = (unsigned int  )7;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tlBcTransLog.flag_1);
        sqlstm.sqhstl[23] = (unsigned int  )2;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tlBcTransLog.flag_2);
        sqlstm.sqhstl[24] = (unsigned int  )2;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tlBcTransLog.flag_3);
        sqlstm.sqhstl[25] = (unsigned int  )2;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tlBcTransLog.flag_4);
        sqlstm.sqhstl[26] = (unsigned int  )2;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tlBcTransLog.flag_5);
        sqlstm.sqhstl[27] = (unsigned int  )2;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tlBcTransLog.flag_6);
        sqlstm.sqhstl[28] = (unsigned int  )2;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tlBcTransLog.flag_7);
        sqlstm.sqhstl[29] = (unsigned int  )2;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tlBcTransLog.fld_28);
        sqlstm.sqhstl[30] = (unsigned int  )7;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)(tlBcTransLog.fld_45);
        sqlstm.sqhstl[31] = (unsigned int  )77;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqhstv[32] = (         void  *)(tlBcTransLog.fld_61);
        sqlstm.sqhstl[32] = (unsigned int  )31;
        sqlstm.sqhsts[32] = (         int  )0;
        sqlstm.sqindv[32] = (         void  *)0;
        sqlstm.sqinds[32] = (         int  )0;
        sqlstm.sqharm[32] = (unsigned int  )0;
        sqlstm.sqadto[32] = (unsigned short )0;
        sqlstm.sqtdso[32] = (unsigned short )0;
        sqlstm.sqhstv[33] = (         void  *)(tlBcTransLog.center_sys_tra_no);
        sqlstm.sqhstl[33] = (unsigned int  )7;
        sqlstm.sqhsts[33] = (         int  )0;
        sqlstm.sqindv[33] = (         void  *)0;
        sqlstm.sqinds[33] = (         int  )0;
        sqlstm.sqharm[33] = (unsigned int  )0;
        sqlstm.sqadto[33] = (unsigned short )0;
        sqlstm.sqtdso[33] = (unsigned short )0;
        sqlstm.sqhstv[34] = (         void  *)(tlBcTransLog.center_term_id);
        sqlstm.sqhstl[34] = (unsigned int  )9;
        sqlstm.sqhsts[34] = (         int  )0;
        sqlstm.sqindv[34] = (         void  *)0;
        sqlstm.sqinds[34] = (         int  )0;
        sqlstm.sqharm[34] = (unsigned int  )0;
        sqlstm.sqadto[34] = (unsigned short )0;
        sqlstm.sqtdso[34] = (unsigned short )0;
        sqlstm.sqhstv[35] = (         void  *)(tlBcTransLog.center_mchnt_cd);
        sqlstm.sqhstl[35] = (unsigned int  )21;
        sqlstm.sqhsts[35] = (         int  )0;
        sqlstm.sqindv[35] = (         void  *)0;
        sqlstm.sqinds[35] = (         int  )0;
        sqlstm.sqharm[35] = (unsigned int  )0;
        sqlstm.sqadto[35] = (unsigned short )0;
        sqlstm.sqtdso[35] = (unsigned short )0;
        sqlstm.sqhstv[36] = (         void  *)(tlBcTransLog.bill_id_rcv);
        sqlstm.sqhstl[36] = (unsigned int  )22;
        sqlstm.sqhsts[36] = (         int  )0;
        sqlstm.sqindv[36] = (         void  *)0;
        sqlstm.sqinds[36] = (         int  )0;
        sqlstm.sqharm[36] = (unsigned int  )0;
        sqlstm.sqadto[36] = (unsigned short )0;
        sqlstm.sqtdso[36] = (unsigned short )0;
        sqlstm.sqhstv[37] = (         void  *)(tlBcTransLog.card_attr);
        sqlstm.sqhstl[37] = (unsigned int  )3;
        sqlstm.sqhsts[37] = (         int  )0;
        sqlstm.sqindv[37] = (         void  *)0;
        sqlstm.sqinds[37] = (         int  )0;
        sqlstm.sqharm[37] = (unsigned int  )0;
        sqlstm.sqadto[37] = (unsigned short )0;
        sqlstm.sqtdso[37] = (unsigned short )0;
        sqlstm.sqhstv[38] = (         void  *)(tlBcTransLog.iss_ins_id_cd);
        sqlstm.sqhstl[38] = (unsigned int  )9;
        sqlstm.sqhsts[38] = (         int  )0;
        sqlstm.sqindv[38] = (         void  *)0;
        sqlstm.sqinds[38] = (         int  )0;
        sqlstm.sqharm[38] = (unsigned int  )0;
        sqlstm.sqadto[38] = (unsigned short )0;
        sqlstm.sqtdso[38] = (unsigned short )0;
        sqlstm.sqhstv[39] = (         void  *)(tlBcTransLog.pname);
        sqlstm.sqhstl[39] = (unsigned int  )211;
        sqlstm.sqhsts[39] = (         int  )0;
        sqlstm.sqindv[39] = (         void  *)0;
        sqlstm.sqinds[39] = (         int  )0;
        sqlstm.sqharm[39] = (unsigned int  )0;
        sqlstm.sqadto[39] = (unsigned short )0;
        sqlstm.sqtdso[39] = (unsigned short )0;
        sqlstm.sqhstv[40] = (         void  *)(tlBcTransLog.encrypt_info);
        sqlstm.sqhstl[40] = (unsigned int  )49;
        sqlstm.sqhsts[40] = (         int  )0;
        sqlstm.sqindv[40] = (         void  *)0;
        sqlstm.sqinds[40] = (         int  )0;
        sqlstm.sqharm[40] = (unsigned int  )0;
        sqlstm.sqadto[40] = (unsigned short )0;
        sqlstm.sqtdso[40] = (unsigned short )0;
        sqlstm.sqhstv[41] = (         void  *)(tlBcTransLog.id_type);
        sqlstm.sqhstl[41] = (unsigned int  )3;
        sqlstm.sqhsts[41] = (         int  )0;
        sqlstm.sqindv[41] = (         void  *)0;
        sqlstm.sqinds[41] = (         int  )0;
        sqlstm.sqharm[41] = (unsigned int  )0;
        sqlstm.sqadto[41] = (unsigned short )0;
        sqlstm.sqtdso[41] = (unsigned short )0;
        sqlstm.sqhstv[42] = (         void  *)(tlBcTransLog.originaDataElements);
        sqlstm.sqhstl[42] = (unsigned int  )44;
        sqlstm.sqhsts[42] = (         int  )0;
        sqlstm.sqindv[42] = (         void  *)0;
        sqlstm.sqinds[42] = (         int  )0;
        sqlstm.sqharm[42] = (unsigned int  )0;
        sqlstm.sqadto[42] = (unsigned short )0;
        sqlstm.sqtdso[42] = (unsigned short )0;
        sqlstm.sqhstv[43] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[43] = (unsigned int  )5;
        sqlstm.sqhsts[43] = (         int  )0;
        sqlstm.sqindv[43] = (         void  *)0;
        sqlstm.sqinds[43] = (         int  )0;
        sqlstm.sqharm[43] = (unsigned int  )0;
        sqlstm.sqadto[43] = (unsigned short )0;
        sqlstm.sqtdso[43] = (unsigned short )0;
        sqlstm.sqhstv[44] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[44] = (unsigned int  )21;
        sqlstm.sqhsts[44] = (         int  )0;
        sqlstm.sqindv[44] = (         void  *)0;
        sqlstm.sqinds[44] = (         int  )0;
        sqlstm.sqharm[44] = (unsigned int  )0;
        sqlstm.sqadto[44] = (unsigned short )0;
        sqlstm.sqtdso[44] = (unsigned short )0;
        sqlstm.sqhstv[45] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[45] = (unsigned int  )7;
        sqlstm.sqhsts[45] = (         int  )0;
        sqlstm.sqindv[45] = (         void  *)0;
        sqlstm.sqinds[45] = (         int  )0;
        sqlstm.sqharm[45] = (unsigned int  )0;
        sqlstm.sqadto[45] = (unsigned short )0;
        sqlstm.sqtdso[45] = (unsigned short )0;
        sqlstm.sqhstv[46] = (         void  *)term_id;
        sqlstm.sqhstl[46] = (unsigned int  )9;
        sqlstm.sqhsts[46] = (         int  )0;
        sqlstm.sqindv[46] = (         void  *)0;
        sqlstm.sqinds[46] = (         int  )0;
        sqlstm.sqharm[46] = (unsigned int  )0;
        sqlstm.sqadto[46] = (unsigned short )0;
        sqlstm.sqtdso[46] = (unsigned short )0;
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


        /* update result */

        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
            /*memcpy(p_tlBcTransLog, &tlBcTransLog, sizeof(tl_bc_trans_log_def));*/
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:UPDATE1>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
        	dcs_debug(0, 0, "resp_cd_verifi[%s]resp_cd_rcv[%s]resp_cd_pay[%s]resp_cd_reversal[%s]resp_cd[%s]",tlBcTransLog.resp_cd_verifi,tlBcTransLog.resp_cd_rcv,tlBcTransLog.resp_cd_pay,tlBcTransLog.resp_cd_reversal,tlBcTransLog.resp_cd);
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:UPDATE1>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )2166;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

        /* 更新关键字外的所有字段 */
    case GLB_DB_UPDATE2:

        /*copy buff*/
        memcpy(&tlBcTransLog, p_tlBcTransLog, sizeof(tl_bc_trans_log_def));

        strcpy_safe(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, sizeof(loc_trans_dt)-1);
        rtrim_null(loc_trans_dt,' ');
        
        strcpy_safe(term_id, p_tlBcTransLog->term_id, sizeof(term_id)-1);
        rtrim_null(term_id,' ');

        strcpy_safe(mchnt_cd, p_tlBcTransLog->mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');

				 strcpy_safe(sys_tra_no, p_tlBcTransLog->sys_tra_no, sizeof(sys_tra_no)-1);
        rtrim_null(sys_tra_no,' ');

        ADJUST_STRING(tlBcTransLog.sys_dt_tm         )
        ADJUST_STRING(tlBcTransLog.settle_dt         )
        ADJUST_STRING(tlBcTransLog.trans_flag        )
        ADJUST_STRING(tlBcTransLog.is_over           )
        ADJUST_STRING(tlBcTransLog.lock_tm           )
        ADJUST_STRING(tlBcTransLog.flag_lock         )
        ADJUST_STRING(tlBcTransLog.flag_verifi       )
        ADJUST_STRING(tlBcTransLog.flag_rcv          )
        ADJUST_STRING(tlBcTransLog.flag_pay          )
        ADJUST_STRING(tlBcTransLog.flag_reversal     )
        ADJUST_STRING(tlBcTransLog.resp_cd_verifi    )
        ADJUST_STRING(tlBcTransLog.resp_cd_rcv       )
        ADJUST_STRING(tlBcTransLog.resp_cd_pay       )
        ADJUST_STRING(tlBcTransLog.resp_cd_reversal  )
        ADJUST_STRING(tlBcTransLog.trans_chnl        )
        ADJUST_STRING(tlBcTransLog.packet_type       )
        ADJUST_STRING(tlBcTransLog.line_nm_verifi    )
        ADJUST_STRING(tlBcTransLog.line_nm_rcv       )
        ADJUST_STRING(tlBcTransLog.line_nm_pay       )        
        ADJUST_STRING(tlBcTransLog.reversal_id       )
        ADJUST_STRING(tlBcTransLog.bc_settle_in      )
        ADJUST_STRING(tlBcTransLog.fld32_ins_id_cd   )
        ADJUST_STRING(tlBcTransLog.fwd_ins_id_cd     )
        ADJUST_STRING(tlBcTransLog.rcv_ins_id_cd     )
        ADJUST_STRING(tlBcTransLog.msg_tp            )
        ADJUST_STRING(tlBcTransLog.pri_acct_no       )
        ADJUST_STRING(tlBcTransLog.proc_cd           )
        glb_Adjust_Decimal(tlBcTransLog.trans_at, tlBcTransLog.trans_at, 12);
        ADJUST_STRING(tlBcTransLog.transmsn_dt_tm    )
        ADJUST_STRING(tlBcTransLog.sys_tra_no        )
        ADJUST_STRING(tlBcTransLog.loc_trans_tm      )
        ADJUST_STRING(tlBcTransLog.loc_trans_dt      )
        ADJUST_STRING(tlBcTransLog.mchnt_tp          )
        ADJUST_STRING(tlBcTransLog.pos_entry_md_cd   )
        ADJUST_STRING(tlBcTransLog.pos_cond_cd       )
        ADJUST_STRING(tlBcTransLog.retri_ref_no      )
        ADJUST_STRING(tlBcTransLog.auth_id_resp_cd   )
        ADJUST_STRING(tlBcTransLog.resp_cd           )
        ADJUST_STRING(tlBcTransLog.term_id           )
        ADJUST_STRING(tlBcTransLog.mchnt_cd          )
        ADJUST_STRING(tlBcTransLog.area_cd           )
        ADJUST_STRING(tlBcTransLog.trans_curr_cd     )
        ADJUST_STRING(tlBcTransLog.flag_1            )
        ADJUST_STRING(tlBcTransLog.flag_2            )
        ADJUST_STRING(tlBcTransLog.flag_3            )
        ADJUST_STRING(tlBcTransLog.flag_4            )
        ADJUST_STRING(tlBcTransLog.flag_5            )
        ADJUST_STRING(tlBcTransLog.flag_6            )
        ADJUST_STRING(tlBcTransLog.flag_7            )
        ADJUST_STRING(tlBcTransLog.fld_5             )
        ADJUST_STRING(tlBcTransLog.fld_8             )
        ADJUST_STRING(tlBcTransLog.fld_28            )
        ADJUST_STRING(tlBcTransLog.fld_34            )
        ADJUST_STRING(tlBcTransLog.fld_44            )
        ADJUST_STRING(tlBcTransLog.fld_45            )
        ADJUST_STRING(tlBcTransLog.fld_46            )
        ADJUST_STRING(tlBcTransLog.fld_48            )
        ADJUST_STRING(tlBcTransLog.fld_61            )
        ADJUST_STRING(tlBcTransLog.center_sys_tra_no )
        ADJUST_STRING(tlBcTransLog.center_term_id    )
        ADJUST_STRING(tlBcTransLog.center_mchnt_cd   )
        ADJUST_STRING(tlBcTransLog.bill_id_rcv       )        
        ADJUST_STRING(tlBcTransLog.card_attr         )
        ADJUST_STRING(tlBcTransLog.iss_ins_id_cd     )
        ADJUST_STRING(tlBcTransLog.pname             )
        ADJUST_STRING(tlBcTransLog.encrypt_info      )
        ADJUST_STRING(tlBcTransLog.id_type		       )
        ADJUST_STRING(tlBcTransLog.originaDataElements			)
        /* EXEC SQL UPDATE tl_bc_trans_log SET
        (
        settle_dt         ,
        trans_flag        ,
        is_over           ,
        lock_tm           ,
        flag_lock         ,
        flag_verifi       ,
        flag_rcv          ,
        flag_pay          ,
        flag_reversal     ,
        resp_cd_verifi    ,
        resp_cd_rcv       ,
        resp_cd_pay       ,
        resp_cd_reversal  ,
        trans_chnl        ,
        packet_type       ,
        line_nm_verifi    ,
        line_nm_rcv       ,
        line_nm_pay       ,
        reversal_id       ,
        bc_settle_in      ,
        fld32_ins_id_cd   ,
        fwd_ins_id_cd     ,
        rcv_ins_id_cd     ,
        proc_cd           ,
        trans_at          ,
        transmsn_dt_tm    ,
        loc_trans_tm      ,
        loc_trans_dt      ,
        pos_entry_md_cd   ,
        pos_cond_cd       ,
        retri_ref_no      ,
        auth_id_resp_cd   ,
        resp_cd           ,
        trans_curr_cd     ,
        flag_1            ,
        flag_2            ,
        flag_3            ,
        flag_4            ,
        flag_5            ,
        flag_6            ,
        flag_7            ,
        fld_5             ,
        fld_8             ,
        fld_28            ,
        fld_34            ,
        fld_44            ,
        fld_45            ,
        fld_46            ,
        fld_48            ,
        fld_61            ,
        center_sys_tra_no ,
        center_term_id    ,
        center_mchnt_cd   ,
        bill_id_rcv       ,
        card_attr         ,
        iss_ins_id_cd     ,
        pname 					  ,
        encrypt_info		  ,
        id_type     	,
        orig_data_elemts  
        )=
        (SELECT
         :p_tlBcTransLog->settle_dt         ,
         :tlBcTransLog.trans_flag        ,
         :tlBcTransLog.is_over           ,
         :tlBcTransLog.lock_tm           ,
         :tlBcTransLog.flag_lock         ,
         :tlBcTransLog.flag_verifi       ,
         :tlBcTransLog.flag_rcv          ,
         :tlBcTransLog.flag_pay          ,
         :tlBcTransLog.flag_reversal     ,
         :tlBcTransLog.resp_cd_verifi    ,
         :tlBcTransLog.resp_cd_rcv       ,
         :tlBcTransLog.resp_cd_pay       ,
         :tlBcTransLog.resp_cd_reversal  ,
         :tlBcTransLog.trans_chnl        ,
         :tlBcTransLog.packet_type       ,
         :tlBcTransLog.line_nm_verifi    ,
         :tlBcTransLog.line_nm_rcv       ,
         :tlBcTransLog.line_nm_pay       ,
         :tlBcTransLog.reversal_id       ,
         :tlBcTransLog.bc_settle_in      ,
         :tlBcTransLog.fld32_ins_id_cd   ,
         :tlBcTransLog.fwd_ins_id_cd     ,
         :tlBcTransLog.rcv_ins_id_cd     ,
         :tlBcTransLog.proc_cd           ,
         :tlBcTransLog.trans_at          ,
         :tlBcTransLog.transmsn_dt_tm    ,
         :tlBcTransLog.loc_trans_tm      ,
         :tlBcTransLog.loc_trans_dt      ,
         :tlBcTransLog.pos_entry_md_cd   ,
         :tlBcTransLog.pos_cond_cd       ,
         :tlBcTransLog.retri_ref_no      ,
         :tlBcTransLog.auth_id_resp_cd   ,
         :tlBcTransLog.resp_cd           ,
         :tlBcTransLog.trans_curr_cd     ,
         :tlBcTransLog.flag_1            ,
         :tlBcTransLog.flag_2            ,
         :tlBcTransLog.flag_3            ,
         :tlBcTransLog.flag_4            ,
         :tlBcTransLog.flag_5            ,
         :tlBcTransLog.flag_6            ,
         :tlBcTransLog.flag_7            ,
         :tlBcTransLog.fld_5             ,
         :tlBcTransLog.fld_8             ,
         :tlBcTransLog.fld_28            ,
         :tlBcTransLog.fld_34            ,
         :tlBcTransLog.fld_44            ,
         :tlBcTransLog.fld_45            ,
         :tlBcTransLog.fld_46            ,
         :tlBcTransLog.fld_48            ,
         :tlBcTransLog.fld_61            ,
         :tlBcTransLog.center_sys_tra_no ,
         :tlBcTransLog.center_term_id    ,
         :tlBcTransLog.center_mchnt_cd   ,
         :tlBcTransLog.bill_id_rcv       ,
         :tlBcTransLog.card_attr         ,
         :tlBcTransLog.iss_ins_id_cd     ,
         :tlBcTransLog.pname						 ,
         :tlBcTransLog.encrypt_info  		 ,
         :tlBcTransLog.id_type		,
         :tlBcTransLog.originaDataElements
        FROM DUAL
        )
        WHERE loc_trans_dt=:loc_trans_dt
				AND mchnt_cd=:mchnt_cd
				AND sys_tra_no=:sys_tra_no
				AND term_id=:term_id
                       ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlbuft((void **)0,
          "update tl_bc_trans_log  set (settle_dt,trans_flag,is_over,lock_tm\
,flag_lock,flag_verifi,flag_rcv,flag_pay,flag_reversal,resp_cd_verifi,resp_c\
d_rcv,resp_cd_pay,resp_cd_reversal,trans_chnl,packet_type,line_nm_verifi,lin\
e_nm_rcv,line_nm_pay,reversal_id,bc_settle_in,fld32_ins_id_cd,fwd_ins_id_cd,\
rcv_ins_id_cd,proc_cd,trans_at,transmsn_dt_tm,loc_trans_tm,loc_trans_dt,pos_\
entry_md_cd,pos_cond_cd,retri_ref_no,auth_id_resp_cd,resp_cd,trans_curr_cd,f\
lag_1,flag_2,flag_3,flag_4,flag_5,flag_6,flag_7,fld_5,fld_8,fld_28,fld_34,fl\
d_44,fld_45,fld_46,fld_48,fld_61,center_sys_tra_no,center_term_id,center_mch\
nt_cd,bill_id_rcv,card_attr,iss_ins_id_cd,pname,encrypt_info,id_type,orig_da\
ta_elemts)=(select :b0 ,:b1 ,:b2 ,:b3 ,:b4 ,:b5 ,:b6 ,:b7 ,:b8 ,:b9 ,:b10 ,:\
b11 ,:b12 ,:b13 ,:b14 ,:b15 ,:b16 ,:b17 ,:b18 ,:b19 ,:b20 ,:b21 ,:b22 ,:b23 \
,:b24 ,:b25 ,:b26 ,:b27 ,:b28 ,:b29 ,:b30 ,:b31 ,:b32 ,:b33 ,:b34 ,:b35 ,:b3\
6 ,:b37 ,:b38 ,:b39 ,:b40 ,:b41 ,:b42 ,:b43 ,:b44 ,:b45 ,:b46 ,:b47 ,:b48 ,:\
b49 ,:b50 ,:b51 ,:b52 ,:b53 ,:b54 ,:b55 ,:b56 ,");
        sqlstm.stmt = ":b57 ,:b58 ,:b59  from DUAL ) where (((loc_trans_dt=\
:b60 and mchnt_cd=:b61) and sys_tra_no=:b62) and term_id=:b63)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )2181;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(p_tlBcTransLog->settle_dt);
        sqlstm.sqhstl[0] = (unsigned int  )5;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.trans_flag);
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.is_over);
        sqlstm.sqhstl[2] = (unsigned int  )2;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.lock_tm);
        sqlstm.sqhstl[3] = (unsigned int  )11;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.flag_lock);
        sqlstm.sqhstl[4] = (unsigned int  )2;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.flag_verifi);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcTransLog.flag_rcv);
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcTransLog.flag_pay);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcTransLog.flag_reversal);
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcTransLog.resp_cd_verifi);
        sqlstm.sqhstl[9] = (unsigned int  )7;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcTransLog.resp_cd_rcv);
        sqlstm.sqhstl[10] = (unsigned int  )7;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcTransLog.resp_cd_pay);
        sqlstm.sqhstl[11] = (unsigned int  )7;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcTransLog.resp_cd_reversal);
        sqlstm.sqhstl[12] = (unsigned int  )7;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcTransLog.trans_chnl);
        sqlstm.sqhstl[13] = (unsigned int  )31;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcTransLog.packet_type);
        sqlstm.sqhstl[14] = (unsigned int  )5;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcTransLog.line_nm_verifi);
        sqlstm.sqhstl[15] = (unsigned int  )5;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tlBcTransLog.line_nm_rcv);
        sqlstm.sqhstl[16] = (unsigned int  )5;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tlBcTransLog.line_nm_pay);
        sqlstm.sqhstl[17] = (unsigned int  )5;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tlBcTransLog.reversal_id);
        sqlstm.sqhstl[18] = (unsigned int  )9;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tlBcTransLog.bc_settle_in);
        sqlstm.sqhstl[19] = (unsigned int  )2;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tlBcTransLog.fld32_ins_id_cd);
        sqlstm.sqhstl[20] = (unsigned int  )14;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tlBcTransLog.fwd_ins_id_cd);
        sqlstm.sqhstl[21] = (unsigned int  )14;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tlBcTransLog.rcv_ins_id_cd);
        sqlstm.sqhstl[22] = (unsigned int  )14;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tlBcTransLog.proc_cd);
        sqlstm.sqhstl[23] = (unsigned int  )7;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tlBcTransLog.trans_at);
        sqlstm.sqhstl[24] = (unsigned int  )13;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tlBcTransLog.transmsn_dt_tm);
        sqlstm.sqhstl[25] = (unsigned int  )11;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tlBcTransLog.loc_trans_tm);
        sqlstm.sqhstl[26] = (unsigned int  )7;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tlBcTransLog.loc_trans_dt);
        sqlstm.sqhstl[27] = (unsigned int  )5;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tlBcTransLog.pos_entry_md_cd);
        sqlstm.sqhstl[28] = (unsigned int  )4;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tlBcTransLog.pos_cond_cd);
        sqlstm.sqhstl[29] = (unsigned int  )3;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tlBcTransLog.retri_ref_no);
        sqlstm.sqhstl[30] = (unsigned int  )13;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)(tlBcTransLog.auth_id_resp_cd);
        sqlstm.sqhstl[31] = (unsigned int  )7;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqhstv[32] = (         void  *)(tlBcTransLog.resp_cd);
        sqlstm.sqhstl[32] = (unsigned int  )3;
        sqlstm.sqhsts[32] = (         int  )0;
        sqlstm.sqindv[32] = (         void  *)0;
        sqlstm.sqinds[32] = (         int  )0;
        sqlstm.sqharm[32] = (unsigned int  )0;
        sqlstm.sqadto[32] = (unsigned short )0;
        sqlstm.sqtdso[32] = (unsigned short )0;
        sqlstm.sqhstv[33] = (         void  *)(tlBcTransLog.trans_curr_cd);
        sqlstm.sqhstl[33] = (unsigned int  )4;
        sqlstm.sqhsts[33] = (         int  )0;
        sqlstm.sqindv[33] = (         void  *)0;
        sqlstm.sqinds[33] = (         int  )0;
        sqlstm.sqharm[33] = (unsigned int  )0;
        sqlstm.sqadto[33] = (unsigned short )0;
        sqlstm.sqtdso[33] = (unsigned short )0;
        sqlstm.sqhstv[34] = (         void  *)(tlBcTransLog.flag_1);
        sqlstm.sqhstl[34] = (unsigned int  )2;
        sqlstm.sqhsts[34] = (         int  )0;
        sqlstm.sqindv[34] = (         void  *)0;
        sqlstm.sqinds[34] = (         int  )0;
        sqlstm.sqharm[34] = (unsigned int  )0;
        sqlstm.sqadto[34] = (unsigned short )0;
        sqlstm.sqtdso[34] = (unsigned short )0;
        sqlstm.sqhstv[35] = (         void  *)(tlBcTransLog.flag_2);
        sqlstm.sqhstl[35] = (unsigned int  )2;
        sqlstm.sqhsts[35] = (         int  )0;
        sqlstm.sqindv[35] = (         void  *)0;
        sqlstm.sqinds[35] = (         int  )0;
        sqlstm.sqharm[35] = (unsigned int  )0;
        sqlstm.sqadto[35] = (unsigned short )0;
        sqlstm.sqtdso[35] = (unsigned short )0;
        sqlstm.sqhstv[36] = (         void  *)(tlBcTransLog.flag_3);
        sqlstm.sqhstl[36] = (unsigned int  )2;
        sqlstm.sqhsts[36] = (         int  )0;
        sqlstm.sqindv[36] = (         void  *)0;
        sqlstm.sqinds[36] = (         int  )0;
        sqlstm.sqharm[36] = (unsigned int  )0;
        sqlstm.sqadto[36] = (unsigned short )0;
        sqlstm.sqtdso[36] = (unsigned short )0;
        sqlstm.sqhstv[37] = (         void  *)(tlBcTransLog.flag_4);
        sqlstm.sqhstl[37] = (unsigned int  )2;
        sqlstm.sqhsts[37] = (         int  )0;
        sqlstm.sqindv[37] = (         void  *)0;
        sqlstm.sqinds[37] = (         int  )0;
        sqlstm.sqharm[37] = (unsigned int  )0;
        sqlstm.sqadto[37] = (unsigned short )0;
        sqlstm.sqtdso[37] = (unsigned short )0;
        sqlstm.sqhstv[38] = (         void  *)(tlBcTransLog.flag_5);
        sqlstm.sqhstl[38] = (unsigned int  )2;
        sqlstm.sqhsts[38] = (         int  )0;
        sqlstm.sqindv[38] = (         void  *)0;
        sqlstm.sqinds[38] = (         int  )0;
        sqlstm.sqharm[38] = (unsigned int  )0;
        sqlstm.sqadto[38] = (unsigned short )0;
        sqlstm.sqtdso[38] = (unsigned short )0;
        sqlstm.sqhstv[39] = (         void  *)(tlBcTransLog.flag_6);
        sqlstm.sqhstl[39] = (unsigned int  )2;
        sqlstm.sqhsts[39] = (         int  )0;
        sqlstm.sqindv[39] = (         void  *)0;
        sqlstm.sqinds[39] = (         int  )0;
        sqlstm.sqharm[39] = (unsigned int  )0;
        sqlstm.sqadto[39] = (unsigned short )0;
        sqlstm.sqtdso[39] = (unsigned short )0;
        sqlstm.sqhstv[40] = (         void  *)(tlBcTransLog.flag_7);
        sqlstm.sqhstl[40] = (unsigned int  )2;
        sqlstm.sqhsts[40] = (         int  )0;
        sqlstm.sqindv[40] = (         void  *)0;
        sqlstm.sqinds[40] = (         int  )0;
        sqlstm.sqharm[40] = (unsigned int  )0;
        sqlstm.sqadto[40] = (unsigned short )0;
        sqlstm.sqtdso[40] = (unsigned short )0;
        sqlstm.sqhstv[41] = (         void  *)(tlBcTransLog.fld_5);
        sqlstm.sqhstl[41] = (unsigned int  )13;
        sqlstm.sqhsts[41] = (         int  )0;
        sqlstm.sqindv[41] = (         void  *)0;
        sqlstm.sqinds[41] = (         int  )0;
        sqlstm.sqharm[41] = (unsigned int  )0;
        sqlstm.sqadto[41] = (unsigned short )0;
        sqlstm.sqtdso[41] = (unsigned short )0;
        sqlstm.sqhstv[42] = (         void  *)(tlBcTransLog.fld_8);
        sqlstm.sqhstl[42] = (unsigned int  )9;
        sqlstm.sqhsts[42] = (         int  )0;
        sqlstm.sqindv[42] = (         void  *)0;
        sqlstm.sqinds[42] = (         int  )0;
        sqlstm.sqharm[42] = (unsigned int  )0;
        sqlstm.sqadto[42] = (unsigned short )0;
        sqlstm.sqtdso[42] = (unsigned short )0;
        sqlstm.sqhstv[43] = (         void  *)(tlBcTransLog.fld_28);
        sqlstm.sqhstl[43] = (unsigned int  )7;
        sqlstm.sqhsts[43] = (         int  )0;
        sqlstm.sqindv[43] = (         void  *)0;
        sqlstm.sqinds[43] = (         int  )0;
        sqlstm.sqharm[43] = (unsigned int  )0;
        sqlstm.sqadto[43] = (unsigned short )0;
        sqlstm.sqtdso[43] = (unsigned short )0;
        sqlstm.sqhstv[44] = (         void  *)(tlBcTransLog.fld_34);
        sqlstm.sqhstl[44] = (unsigned int  )29;
        sqlstm.sqhsts[44] = (         int  )0;
        sqlstm.sqindv[44] = (         void  *)0;
        sqlstm.sqinds[44] = (         int  )0;
        sqlstm.sqharm[44] = (unsigned int  )0;
        sqlstm.sqadto[44] = (unsigned short )0;
        sqlstm.sqtdso[44] = (unsigned short )0;
        sqlstm.sqhstv[45] = (         void  *)(tlBcTransLog.fld_44);
        sqlstm.sqhstl[45] = (unsigned int  )28;
        sqlstm.sqhsts[45] = (         int  )0;
        sqlstm.sqindv[45] = (         void  *)0;
        sqlstm.sqinds[45] = (         int  )0;
        sqlstm.sqharm[45] = (unsigned int  )0;
        sqlstm.sqadto[45] = (unsigned short )0;
        sqlstm.sqtdso[45] = (unsigned short )0;
        sqlstm.sqhstv[46] = (         void  *)(tlBcTransLog.fld_45);
        sqlstm.sqhstl[46] = (unsigned int  )77;
        sqlstm.sqhsts[46] = (         int  )0;
        sqlstm.sqindv[46] = (         void  *)0;
        sqlstm.sqinds[46] = (         int  )0;
        sqlstm.sqharm[46] = (unsigned int  )0;
        sqlstm.sqadto[46] = (unsigned short )0;
        sqlstm.sqtdso[46] = (unsigned short )0;
        sqlstm.sqhstv[47] = (         void  *)(tlBcTransLog.fld_46);
        sqlstm.sqhstl[47] = (unsigned int  )31;
        sqlstm.sqhsts[47] = (         int  )0;
        sqlstm.sqindv[47] = (         void  *)0;
        sqlstm.sqinds[47] = (         int  )0;
        sqlstm.sqharm[47] = (unsigned int  )0;
        sqlstm.sqadto[47] = (unsigned short )0;
        sqlstm.sqtdso[47] = (unsigned short )0;
        sqlstm.sqhstv[48] = (         void  *)(tlBcTransLog.fld_48);
        sqlstm.sqhstl[48] = (unsigned int  )191;
        sqlstm.sqhsts[48] = (         int  )0;
        sqlstm.sqindv[48] = (         void  *)0;
        sqlstm.sqinds[48] = (         int  )0;
        sqlstm.sqharm[48] = (unsigned int  )0;
        sqlstm.sqadto[48] = (unsigned short )0;
        sqlstm.sqtdso[48] = (unsigned short )0;
        sqlstm.sqhstv[49] = (         void  *)(tlBcTransLog.fld_61);
        sqlstm.sqhstl[49] = (unsigned int  )31;
        sqlstm.sqhsts[49] = (         int  )0;
        sqlstm.sqindv[49] = (         void  *)0;
        sqlstm.sqinds[49] = (         int  )0;
        sqlstm.sqharm[49] = (unsigned int  )0;
        sqlstm.sqadto[49] = (unsigned short )0;
        sqlstm.sqtdso[49] = (unsigned short )0;
        sqlstm.sqhstv[50] = (         void  *)(tlBcTransLog.center_sys_tra_no);
        sqlstm.sqhstl[50] = (unsigned int  )7;
        sqlstm.sqhsts[50] = (         int  )0;
        sqlstm.sqindv[50] = (         void  *)0;
        sqlstm.sqinds[50] = (         int  )0;
        sqlstm.sqharm[50] = (unsigned int  )0;
        sqlstm.sqadto[50] = (unsigned short )0;
        sqlstm.sqtdso[50] = (unsigned short )0;
        sqlstm.sqhstv[51] = (         void  *)(tlBcTransLog.center_term_id);
        sqlstm.sqhstl[51] = (unsigned int  )9;
        sqlstm.sqhsts[51] = (         int  )0;
        sqlstm.sqindv[51] = (         void  *)0;
        sqlstm.sqinds[51] = (         int  )0;
        sqlstm.sqharm[51] = (unsigned int  )0;
        sqlstm.sqadto[51] = (unsigned short )0;
        sqlstm.sqtdso[51] = (unsigned short )0;
        sqlstm.sqhstv[52] = (         void  *)(tlBcTransLog.center_mchnt_cd);
        sqlstm.sqhstl[52] = (unsigned int  )21;
        sqlstm.sqhsts[52] = (         int  )0;
        sqlstm.sqindv[52] = (         void  *)0;
        sqlstm.sqinds[52] = (         int  )0;
        sqlstm.sqharm[52] = (unsigned int  )0;
        sqlstm.sqadto[52] = (unsigned short )0;
        sqlstm.sqtdso[52] = (unsigned short )0;
        sqlstm.sqhstv[53] = (         void  *)(tlBcTransLog.bill_id_rcv);
        sqlstm.sqhstl[53] = (unsigned int  )22;
        sqlstm.sqhsts[53] = (         int  )0;
        sqlstm.sqindv[53] = (         void  *)0;
        sqlstm.sqinds[53] = (         int  )0;
        sqlstm.sqharm[53] = (unsigned int  )0;
        sqlstm.sqadto[53] = (unsigned short )0;
        sqlstm.sqtdso[53] = (unsigned short )0;
        sqlstm.sqhstv[54] = (         void  *)(tlBcTransLog.card_attr);
        sqlstm.sqhstl[54] = (unsigned int  )3;
        sqlstm.sqhsts[54] = (         int  )0;
        sqlstm.sqindv[54] = (         void  *)0;
        sqlstm.sqinds[54] = (         int  )0;
        sqlstm.sqharm[54] = (unsigned int  )0;
        sqlstm.sqadto[54] = (unsigned short )0;
        sqlstm.sqtdso[54] = (unsigned short )0;
        sqlstm.sqhstv[55] = (         void  *)(tlBcTransLog.iss_ins_id_cd);
        sqlstm.sqhstl[55] = (unsigned int  )9;
        sqlstm.sqhsts[55] = (         int  )0;
        sqlstm.sqindv[55] = (         void  *)0;
        sqlstm.sqinds[55] = (         int  )0;
        sqlstm.sqharm[55] = (unsigned int  )0;
        sqlstm.sqadto[55] = (unsigned short )0;
        sqlstm.sqtdso[55] = (unsigned short )0;
        sqlstm.sqhstv[56] = (         void  *)(tlBcTransLog.pname);
        sqlstm.sqhstl[56] = (unsigned int  )211;
        sqlstm.sqhsts[56] = (         int  )0;
        sqlstm.sqindv[56] = (         void  *)0;
        sqlstm.sqinds[56] = (         int  )0;
        sqlstm.sqharm[56] = (unsigned int  )0;
        sqlstm.sqadto[56] = (unsigned short )0;
        sqlstm.sqtdso[56] = (unsigned short )0;
        sqlstm.sqhstv[57] = (         void  *)(tlBcTransLog.encrypt_info);
        sqlstm.sqhstl[57] = (unsigned int  )49;
        sqlstm.sqhsts[57] = (         int  )0;
        sqlstm.sqindv[57] = (         void  *)0;
        sqlstm.sqinds[57] = (         int  )0;
        sqlstm.sqharm[57] = (unsigned int  )0;
        sqlstm.sqadto[57] = (unsigned short )0;
        sqlstm.sqtdso[57] = (unsigned short )0;
        sqlstm.sqhstv[58] = (         void  *)(tlBcTransLog.id_type);
        sqlstm.sqhstl[58] = (unsigned int  )3;
        sqlstm.sqhsts[58] = (         int  )0;
        sqlstm.sqindv[58] = (         void  *)0;
        sqlstm.sqinds[58] = (         int  )0;
        sqlstm.sqharm[58] = (unsigned int  )0;
        sqlstm.sqadto[58] = (unsigned short )0;
        sqlstm.sqtdso[58] = (unsigned short )0;
        sqlstm.sqhstv[59] = (         void  *)(tlBcTransLog.originaDataElements);
        sqlstm.sqhstl[59] = (unsigned int  )44;
        sqlstm.sqhsts[59] = (         int  )0;
        sqlstm.sqindv[59] = (         void  *)0;
        sqlstm.sqinds[59] = (         int  )0;
        sqlstm.sqharm[59] = (unsigned int  )0;
        sqlstm.sqadto[59] = (unsigned short )0;
        sqlstm.sqtdso[59] = (unsigned short )0;
        sqlstm.sqhstv[60] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[60] = (unsigned int  )5;
        sqlstm.sqhsts[60] = (         int  )0;
        sqlstm.sqindv[60] = (         void  *)0;
        sqlstm.sqinds[60] = (         int  )0;
        sqlstm.sqharm[60] = (unsigned int  )0;
        sqlstm.sqadto[60] = (unsigned short )0;
        sqlstm.sqtdso[60] = (unsigned short )0;
        sqlstm.sqhstv[61] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[61] = (unsigned int  )21;
        sqlstm.sqhsts[61] = (         int  )0;
        sqlstm.sqindv[61] = (         void  *)0;
        sqlstm.sqinds[61] = (         int  )0;
        sqlstm.sqharm[61] = (unsigned int  )0;
        sqlstm.sqadto[61] = (unsigned short )0;
        sqlstm.sqtdso[61] = (unsigned short )0;
        sqlstm.sqhstv[62] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[62] = (unsigned int  )7;
        sqlstm.sqhsts[62] = (         int  )0;
        sqlstm.sqindv[62] = (         void  *)0;
        sqlstm.sqinds[62] = (         int  )0;
        sqlstm.sqharm[62] = (unsigned int  )0;
        sqlstm.sqadto[62] = (unsigned short )0;
        sqlstm.sqtdso[62] = (unsigned short )0;
        sqlstm.sqhstv[63] = (         void  *)term_id;
        sqlstm.sqhstl[63] = (unsigned int  )9;
        sqlstm.sqhsts[63] = (         int  )0;
        sqlstm.sqindv[63] = (         void  *)0;
        sqlstm.sqinds[63] = (         int  )0;
        sqlstm.sqharm[63] = (unsigned int  )0;
        sqlstm.sqadto[63] = (unsigned short )0;
        sqlstm.sqtdso[63] = (unsigned short )0;
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



        /* update result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
            /*memcpy(p_tlBcTransLog, &tlBcTransLog, sizeof(tl_bc_trans_log_def));*/
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:UPDATE2>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:UPDATE2>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )2452;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    case GLB_DB_UPDATE3:

        /* 用于账单支付交易，根据查询结果更新记录 */
        strcpy_safe(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, sizeof(loc_trans_dt)-1);
        rtrim_null(loc_trans_dt,' ');
        
        strcpy_safe(term_id, p_tlBcTransLog->center_term_id, sizeof(term_id)-1);
        rtrim_null(term_id,' ');

        strcpy_safe(mchnt_cd, p_tlBcTransLog->center_mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');

		strcpy_safe(sys_tra_no, p_tlBcTransLog->center_sys_tra_no, sizeof(sys_tra_no)-1);
        rtrim_null(sys_tra_no,' ');

        /* EXEC SQL SELECT 
        sys_dt_tm            ,
        settle_dt            ,
        trans_flag           ,
        is_over              ,
        lock_tm              ,
        flag_lock            ,
        flag_verifi          ,
        flag_rcv             ,
        flag_pay             ,
        flag_reversal        ,
        resp_cd_verifi       ,
        resp_cd_rcv          ,
        resp_cd_pay          ,
        resp_cd_reversal     ,
        trans_chnl           ,
        packet_type          ,
        line_nm_verifi       ,
        line_nm_rcv          ,
        line_nm_pay          ,
        reversal_id          ,
        bc_settle_in         ,
        fld32_ins_id_cd      ,
        fwd_ins_id_cd        ,
        rcv_ins_id_cd        ,
        msg_tp               ,
        pri_acct_no          ,
        proc_cd              ,
        trans_at             ,
        transmsn_dt_tm       ,
        sys_tra_no           ,
        loc_trans_tm         ,
        loc_trans_dt         ,
        mchnt_tp             ,
        pos_entry_md_cd      ,
        pos_cond_cd          ,
        retri_ref_no				 ,
        auth_id_resp_cd      ,
        resp_cd              ,
        term_id              ,
        mchnt_cd             ,
        area_cd              ,
        trans_curr_cd        ,
        flag_1               ,
        flag_2               ,
        flag_3               ,
        flag_4               ,
        flag_5               ,
        flag_6               ,
        flag_7               ,
        fld_5                ,
        fld_8                ,
        fld_28               ,
        fld_34               ,
        fld_44               ,
        fld_45               ,
        fld_46               ,
        fld_48               ,
        fld_61               ,
        center_sys_tra_no    ,
        center_term_id       ,
        center_mchnt_cd      ,
        bill_id_rcv          ,
        card_attr            ,
        iss_ins_id_cd        ,
        pname								 ,
        encrypt_info				 ,
        id_type 		,
        orig_data_elemts
        INTO 
      	 :tlBcTransLog.sys_dt_tm            ,   
      	 :tlBcTransLog.settle_dt            ,
      	 :tlBcTransLog.trans_flag           ,
      	 :tlBcTransLog.is_over              ,
      	 :tlBcTransLog.lock_tm              ,
      	 :tlBcTransLog.flag_lock            ,
      	 :tlBcTransLog.flag_verifi          ,
      	 :tlBcTransLog.flag_rcv             ,
      	 :tlBcTransLog.flag_pay             ,
      	 :tlBcTransLog.flag_reversal        ,
      	 :tlBcTransLog.resp_cd_verifi       ,
      	 :tlBcTransLog.resp_cd_rcv          ,
      	 :tlBcTransLog.resp_cd_pay          ,
      	 :tlBcTransLog.resp_cd_reversal     ,
      	 :tlBcTransLog.trans_chnl           ,
      	 :tlBcTransLog.packet_type          ,
      	 :tlBcTransLog.line_nm_verifi       ,
      	 :tlBcTransLog.line_nm_rcv          ,
      	 :tlBcTransLog.line_nm_pay          ,
      	 :tlBcTransLog.reversal_id          ,
      	 :tlBcTransLog.bc_settle_in         ,
      	 :tlBcTransLog.fld32_ins_id_cd      ,
      	 :tlBcTransLog.fwd_ins_id_cd        ,
      	 :tlBcTransLog.rcv_ins_id_cd        ,
      	 :tlBcTransLog.msg_tp               ,
      	 :tlBcTransLog.pri_acct_no          ,
      	 :tlBcTransLog.proc_cd              ,
      	 :tlBcTransLog.trans_at             ,
      	 :tlBcTransLog.transmsn_dt_tm       ,
      	 :tlBcTransLog.sys_tra_no           ,
      	 :tlBcTransLog.loc_trans_tm         ,
      	 :tlBcTransLog.loc_trans_dt         ,
      	 :tlBcTransLog.mchnt_tp             ,
      	 :tlBcTransLog.pos_entry_md_cd      ,
      	 :tlBcTransLog.pos_cond_cd          ,
      	 :tlBcTransLog.retri_ref_no         ,
      	 :tlBcTransLog.auth_id_resp_cd      ,
      	 :tlBcTransLog.resp_cd              ,
      	 :tlBcTransLog.term_id              ,
      	 :tlBcTransLog.mchnt_cd             ,
      	 :tlBcTransLog.area_cd              ,
      	 :tlBcTransLog.trans_curr_cd        ,
      	 :tlBcTransLog.flag_1               ,
      	 :tlBcTransLog.flag_2               ,
      	 :tlBcTransLog.flag_3               ,
      	 :tlBcTransLog.flag_4               ,
      	 :tlBcTransLog.flag_5               ,
      	 :tlBcTransLog.flag_6               ,
      	 :tlBcTransLog.flag_7               ,
      	 :tlBcTransLog.fld_5                ,
      	 :tlBcTransLog.fld_8                ,
      	 :tlBcTransLog.fld_28               ,
      	 :tlBcTransLog.fld_34               ,
      	 :tlBcTransLog.fld_44               ,
      	 :tlBcTransLog.fld_45               ,
      	 :tlBcTransLog.fld_46               ,
      	 :tlBcTransLog.fld_48               ,
      	 :tlBcTransLog.fld_61               ,
      	 :tlBcTransLog.center_sys_tra_no    ,
      	 :tlBcTransLog.center_term_id       ,
      	 :tlBcTransLog.center_mchnt_cd      ,
      	 :tlBcTransLog.bill_id_rcv          ,
      	 :tlBcTransLog.card_attr            ,
      	 :tlBcTransLog.iss_ins_id_cd        ,
      	 :tlBcTransLog.pname						 	  ,
      	 :tlBcTransLog.encrypt_info			 	  ,
      	 :tlBcTransLog.id_type		,
      	 :tlBcTransLog.originaDataElements
        FROM tl_bc_trans_log
        WHERE loc_trans_dt=:loc_trans_dt
				AND center_mchnt_cd=:mchnt_cd
				AND center_term_id=:term_id
				AND center_sys_tra_no=:sys_tra_no
				AND rownum<=1
				FOR UPDATE
				; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlbuft((void **)0,
          "select sys_dt_tm ,settle_dt ,trans_flag ,is_over ,lock_tm ,flag_l\
ock ,flag_verifi ,flag_rcv ,flag_pay ,flag_reversal ,resp_cd_verifi ,resp_cd\
_rcv ,resp_cd_pay ,resp_cd_reversal ,trans_chnl ,packet_type ,line_nm_verifi\
 ,line_nm_rcv ,line_nm_pay ,reversal_id ,bc_settle_in ,fld32_ins_id_cd ,fwd_\
ins_id_cd ,rcv_ins_id_cd ,msg_tp ,pri_acct_no ,proc_cd ,trans_at ,transmsn_d\
t_tm ,sys_tra_no ,loc_trans_tm ,loc_trans_dt ,mchnt_tp ,pos_entry_md_cd ,pos\
_cond_cd ,retri_ref_no ,auth_id_resp_cd ,resp_cd ,term_id ,mchnt_cd ,area_cd\
 ,trans_curr_cd ,flag_1 ,flag_2 ,flag_3 ,flag_4 ,flag_5 ,flag_6 ,flag_7 ,fld\
_5 ,fld_8 ,fld_28 ,fld_34 ,fld_44 ,fld_45 ,fld_46 ,fld_48 ,fld_61 ,center_sy\
s_tra_no ,center_term_id ,center_mchnt_cd ,bill_id_rcv ,card_attr ,iss_ins_i\
d_cd ,pname ,encrypt_info ,id_type ,orig_data_elemts into :b0,:b1,:b2,:b3,:b\
4,:b5,:b6,:b7,:b8,:b9,:b10,:b11,:b12,:b13,:b14,:b15,:b16,:b17,:b18,:b19,:b20\
,:b21,:b22,:b23,:b24,:b25,:b26,:b27,:b28,:b29,:b30,:b31,:b32,:b33,:b34,:b35,\
:b36,:b37,:b38,:b39,:b40,:b41,:b42,:b43,:b44,:b");
        sqlstm.stmt = "45,:b46,:b47,:b48,:b49,:b50,:b51,:b52,:b53,:b54,:b55\
,:b56,:b57,:b58,:b59,:b60,:b61,:b62,:b63,:b64,:b65,:b66,:b67  from tl_bc_tran\
s_log where ((((loc_trans_dt=:b68 and center_mchnt_cd=:b69) and center_term_i\
d=:b70) and center_sys_tra_no=:b71) and rownum<=1) for update ";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )2467;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)(tlBcTransLog.sys_dt_tm);
        sqlstm.sqhstl[0] = (unsigned int  )15;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.settle_dt);
        sqlstm.sqhstl[1] = (unsigned int  )5;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.trans_flag);
        sqlstm.sqhstl[2] = (unsigned int  )5;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.is_over);
        sqlstm.sqhstl[3] = (unsigned int  )2;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.lock_tm);
        sqlstm.sqhstl[4] = (unsigned int  )11;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.flag_lock);
        sqlstm.sqhstl[5] = (unsigned int  )2;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)(tlBcTransLog.flag_verifi);
        sqlstm.sqhstl[6] = (unsigned int  )2;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)(tlBcTransLog.flag_rcv);
        sqlstm.sqhstl[7] = (unsigned int  )2;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)(tlBcTransLog.flag_pay);
        sqlstm.sqhstl[8] = (unsigned int  )2;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)(tlBcTransLog.flag_reversal);
        sqlstm.sqhstl[9] = (unsigned int  )2;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)(tlBcTransLog.resp_cd_verifi);
        sqlstm.sqhstl[10] = (unsigned int  )7;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)(tlBcTransLog.resp_cd_rcv);
        sqlstm.sqhstl[11] = (unsigned int  )7;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)(tlBcTransLog.resp_cd_pay);
        sqlstm.sqhstl[12] = (unsigned int  )7;
        sqlstm.sqhsts[12] = (         int  )0;
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)(tlBcTransLog.resp_cd_reversal);
        sqlstm.sqhstl[13] = (unsigned int  )7;
        sqlstm.sqhsts[13] = (         int  )0;
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)(tlBcTransLog.trans_chnl);
        sqlstm.sqhstl[14] = (unsigned int  )31;
        sqlstm.sqhsts[14] = (         int  )0;
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)(tlBcTransLog.packet_type);
        sqlstm.sqhstl[15] = (unsigned int  )5;
        sqlstm.sqhsts[15] = (         int  )0;
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)(tlBcTransLog.line_nm_verifi);
        sqlstm.sqhstl[16] = (unsigned int  )5;
        sqlstm.sqhsts[16] = (         int  )0;
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)(tlBcTransLog.line_nm_rcv);
        sqlstm.sqhstl[17] = (unsigned int  )5;
        sqlstm.sqhsts[17] = (         int  )0;
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)(tlBcTransLog.line_nm_pay);
        sqlstm.sqhstl[18] = (unsigned int  )5;
        sqlstm.sqhsts[18] = (         int  )0;
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)(tlBcTransLog.reversal_id);
        sqlstm.sqhstl[19] = (unsigned int  )9;
        sqlstm.sqhsts[19] = (         int  )0;
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)(tlBcTransLog.bc_settle_in);
        sqlstm.sqhstl[20] = (unsigned int  )2;
        sqlstm.sqhsts[20] = (         int  )0;
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)(tlBcTransLog.fld32_ins_id_cd);
        sqlstm.sqhstl[21] = (unsigned int  )14;
        sqlstm.sqhsts[21] = (         int  )0;
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)(tlBcTransLog.fwd_ins_id_cd);
        sqlstm.sqhstl[22] = (unsigned int  )14;
        sqlstm.sqhsts[22] = (         int  )0;
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqhstv[23] = (         void  *)(tlBcTransLog.rcv_ins_id_cd);
        sqlstm.sqhstl[23] = (unsigned int  )14;
        sqlstm.sqhsts[23] = (         int  )0;
        sqlstm.sqindv[23] = (         void  *)0;
        sqlstm.sqinds[23] = (         int  )0;
        sqlstm.sqharm[23] = (unsigned int  )0;
        sqlstm.sqadto[23] = (unsigned short )0;
        sqlstm.sqtdso[23] = (unsigned short )0;
        sqlstm.sqhstv[24] = (         void  *)(tlBcTransLog.msg_tp);
        sqlstm.sqhstl[24] = (unsigned int  )5;
        sqlstm.sqhsts[24] = (         int  )0;
        sqlstm.sqindv[24] = (         void  *)0;
        sqlstm.sqinds[24] = (         int  )0;
        sqlstm.sqharm[24] = (unsigned int  )0;
        sqlstm.sqadto[24] = (unsigned short )0;
        sqlstm.sqtdso[24] = (unsigned short )0;
        sqlstm.sqhstv[25] = (         void  *)(tlBcTransLog.pri_acct_no);
        sqlstm.sqhstl[25] = (unsigned int  )22;
        sqlstm.sqhsts[25] = (         int  )0;
        sqlstm.sqindv[25] = (         void  *)0;
        sqlstm.sqinds[25] = (         int  )0;
        sqlstm.sqharm[25] = (unsigned int  )0;
        sqlstm.sqadto[25] = (unsigned short )0;
        sqlstm.sqtdso[25] = (unsigned short )0;
        sqlstm.sqhstv[26] = (         void  *)(tlBcTransLog.proc_cd);
        sqlstm.sqhstl[26] = (unsigned int  )7;
        sqlstm.sqhsts[26] = (         int  )0;
        sqlstm.sqindv[26] = (         void  *)0;
        sqlstm.sqinds[26] = (         int  )0;
        sqlstm.sqharm[26] = (unsigned int  )0;
        sqlstm.sqadto[26] = (unsigned short )0;
        sqlstm.sqtdso[26] = (unsigned short )0;
        sqlstm.sqhstv[27] = (         void  *)(tlBcTransLog.trans_at);
        sqlstm.sqhstl[27] = (unsigned int  )13;
        sqlstm.sqhsts[27] = (         int  )0;
        sqlstm.sqindv[27] = (         void  *)0;
        sqlstm.sqinds[27] = (         int  )0;
        sqlstm.sqharm[27] = (unsigned int  )0;
        sqlstm.sqadto[27] = (unsigned short )0;
        sqlstm.sqtdso[27] = (unsigned short )0;
        sqlstm.sqhstv[28] = (         void  *)(tlBcTransLog.transmsn_dt_tm);
        sqlstm.sqhstl[28] = (unsigned int  )11;
        sqlstm.sqhsts[28] = (         int  )0;
        sqlstm.sqindv[28] = (         void  *)0;
        sqlstm.sqinds[28] = (         int  )0;
        sqlstm.sqharm[28] = (unsigned int  )0;
        sqlstm.sqadto[28] = (unsigned short )0;
        sqlstm.sqtdso[28] = (unsigned short )0;
        sqlstm.sqhstv[29] = (         void  *)(tlBcTransLog.sys_tra_no);
        sqlstm.sqhstl[29] = (unsigned int  )7;
        sqlstm.sqhsts[29] = (         int  )0;
        sqlstm.sqindv[29] = (         void  *)0;
        sqlstm.sqinds[29] = (         int  )0;
        sqlstm.sqharm[29] = (unsigned int  )0;
        sqlstm.sqadto[29] = (unsigned short )0;
        sqlstm.sqtdso[29] = (unsigned short )0;
        sqlstm.sqhstv[30] = (         void  *)(tlBcTransLog.loc_trans_tm);
        sqlstm.sqhstl[30] = (unsigned int  )7;
        sqlstm.sqhsts[30] = (         int  )0;
        sqlstm.sqindv[30] = (         void  *)0;
        sqlstm.sqinds[30] = (         int  )0;
        sqlstm.sqharm[30] = (unsigned int  )0;
        sqlstm.sqadto[30] = (unsigned short )0;
        sqlstm.sqtdso[30] = (unsigned short )0;
        sqlstm.sqhstv[31] = (         void  *)(tlBcTransLog.loc_trans_dt);
        sqlstm.sqhstl[31] = (unsigned int  )5;
        sqlstm.sqhsts[31] = (         int  )0;
        sqlstm.sqindv[31] = (         void  *)0;
        sqlstm.sqinds[31] = (         int  )0;
        sqlstm.sqharm[31] = (unsigned int  )0;
        sqlstm.sqadto[31] = (unsigned short )0;
        sqlstm.sqtdso[31] = (unsigned short )0;
        sqlstm.sqhstv[32] = (         void  *)(tlBcTransLog.mchnt_tp);
        sqlstm.sqhstl[32] = (unsigned int  )5;
        sqlstm.sqhsts[32] = (         int  )0;
        sqlstm.sqindv[32] = (         void  *)0;
        sqlstm.sqinds[32] = (         int  )0;
        sqlstm.sqharm[32] = (unsigned int  )0;
        sqlstm.sqadto[32] = (unsigned short )0;
        sqlstm.sqtdso[32] = (unsigned short )0;
        sqlstm.sqhstv[33] = (         void  *)(tlBcTransLog.pos_entry_md_cd);
        sqlstm.sqhstl[33] = (unsigned int  )4;
        sqlstm.sqhsts[33] = (         int  )0;
        sqlstm.sqindv[33] = (         void  *)0;
        sqlstm.sqinds[33] = (         int  )0;
        sqlstm.sqharm[33] = (unsigned int  )0;
        sqlstm.sqadto[33] = (unsigned short )0;
        sqlstm.sqtdso[33] = (unsigned short )0;
        sqlstm.sqhstv[34] = (         void  *)(tlBcTransLog.pos_cond_cd);
        sqlstm.sqhstl[34] = (unsigned int  )3;
        sqlstm.sqhsts[34] = (         int  )0;
        sqlstm.sqindv[34] = (         void  *)0;
        sqlstm.sqinds[34] = (         int  )0;
        sqlstm.sqharm[34] = (unsigned int  )0;
        sqlstm.sqadto[34] = (unsigned short )0;
        sqlstm.sqtdso[34] = (unsigned short )0;
        sqlstm.sqhstv[35] = (         void  *)(tlBcTransLog.retri_ref_no);
        sqlstm.sqhstl[35] = (unsigned int  )13;
        sqlstm.sqhsts[35] = (         int  )0;
        sqlstm.sqindv[35] = (         void  *)0;
        sqlstm.sqinds[35] = (         int  )0;
        sqlstm.sqharm[35] = (unsigned int  )0;
        sqlstm.sqadto[35] = (unsigned short )0;
        sqlstm.sqtdso[35] = (unsigned short )0;
        sqlstm.sqhstv[36] = (         void  *)(tlBcTransLog.auth_id_resp_cd);
        sqlstm.sqhstl[36] = (unsigned int  )7;
        sqlstm.sqhsts[36] = (         int  )0;
        sqlstm.sqindv[36] = (         void  *)0;
        sqlstm.sqinds[36] = (         int  )0;
        sqlstm.sqharm[36] = (unsigned int  )0;
        sqlstm.sqadto[36] = (unsigned short )0;
        sqlstm.sqtdso[36] = (unsigned short )0;
        sqlstm.sqhstv[37] = (         void  *)(tlBcTransLog.resp_cd);
        sqlstm.sqhstl[37] = (unsigned int  )3;
        sqlstm.sqhsts[37] = (         int  )0;
        sqlstm.sqindv[37] = (         void  *)0;
        sqlstm.sqinds[37] = (         int  )0;
        sqlstm.sqharm[37] = (unsigned int  )0;
        sqlstm.sqadto[37] = (unsigned short )0;
        sqlstm.sqtdso[37] = (unsigned short )0;
        sqlstm.sqhstv[38] = (         void  *)(tlBcTransLog.term_id);
        sqlstm.sqhstl[38] = (unsigned int  )9;
        sqlstm.sqhsts[38] = (         int  )0;
        sqlstm.sqindv[38] = (         void  *)0;
        sqlstm.sqinds[38] = (         int  )0;
        sqlstm.sqharm[38] = (unsigned int  )0;
        sqlstm.sqadto[38] = (unsigned short )0;
        sqlstm.sqtdso[38] = (unsigned short )0;
        sqlstm.sqhstv[39] = (         void  *)(tlBcTransLog.mchnt_cd);
        sqlstm.sqhstl[39] = (unsigned int  )16;
        sqlstm.sqhsts[39] = (         int  )0;
        sqlstm.sqindv[39] = (         void  *)0;
        sqlstm.sqinds[39] = (         int  )0;
        sqlstm.sqharm[39] = (unsigned int  )0;
        sqlstm.sqadto[39] = (unsigned short )0;
        sqlstm.sqtdso[39] = (unsigned short )0;
        sqlstm.sqhstv[40] = (         void  *)(tlBcTransLog.area_cd);
        sqlstm.sqhstl[40] = (unsigned int  )7;
        sqlstm.sqhsts[40] = (         int  )0;
        sqlstm.sqindv[40] = (         void  *)0;
        sqlstm.sqinds[40] = (         int  )0;
        sqlstm.sqharm[40] = (unsigned int  )0;
        sqlstm.sqadto[40] = (unsigned short )0;
        sqlstm.sqtdso[40] = (unsigned short )0;
        sqlstm.sqhstv[41] = (         void  *)(tlBcTransLog.trans_curr_cd);
        sqlstm.sqhstl[41] = (unsigned int  )4;
        sqlstm.sqhsts[41] = (         int  )0;
        sqlstm.sqindv[41] = (         void  *)0;
        sqlstm.sqinds[41] = (         int  )0;
        sqlstm.sqharm[41] = (unsigned int  )0;
        sqlstm.sqadto[41] = (unsigned short )0;
        sqlstm.sqtdso[41] = (unsigned short )0;
        sqlstm.sqhstv[42] = (         void  *)(tlBcTransLog.flag_1);
        sqlstm.sqhstl[42] = (unsigned int  )2;
        sqlstm.sqhsts[42] = (         int  )0;
        sqlstm.sqindv[42] = (         void  *)0;
        sqlstm.sqinds[42] = (         int  )0;
        sqlstm.sqharm[42] = (unsigned int  )0;
        sqlstm.sqadto[42] = (unsigned short )0;
        sqlstm.sqtdso[42] = (unsigned short )0;
        sqlstm.sqhstv[43] = (         void  *)(tlBcTransLog.flag_2);
        sqlstm.sqhstl[43] = (unsigned int  )2;
        sqlstm.sqhsts[43] = (         int  )0;
        sqlstm.sqindv[43] = (         void  *)0;
        sqlstm.sqinds[43] = (         int  )0;
        sqlstm.sqharm[43] = (unsigned int  )0;
        sqlstm.sqadto[43] = (unsigned short )0;
        sqlstm.sqtdso[43] = (unsigned short )0;
        sqlstm.sqhstv[44] = (         void  *)(tlBcTransLog.flag_3);
        sqlstm.sqhstl[44] = (unsigned int  )2;
        sqlstm.sqhsts[44] = (         int  )0;
        sqlstm.sqindv[44] = (         void  *)0;
        sqlstm.sqinds[44] = (         int  )0;
        sqlstm.sqharm[44] = (unsigned int  )0;
        sqlstm.sqadto[44] = (unsigned short )0;
        sqlstm.sqtdso[44] = (unsigned short )0;
        sqlstm.sqhstv[45] = (         void  *)(tlBcTransLog.flag_4);
        sqlstm.sqhstl[45] = (unsigned int  )2;
        sqlstm.sqhsts[45] = (         int  )0;
        sqlstm.sqindv[45] = (         void  *)0;
        sqlstm.sqinds[45] = (         int  )0;
        sqlstm.sqharm[45] = (unsigned int  )0;
        sqlstm.sqadto[45] = (unsigned short )0;
        sqlstm.sqtdso[45] = (unsigned short )0;
        sqlstm.sqhstv[46] = (         void  *)(tlBcTransLog.flag_5);
        sqlstm.sqhstl[46] = (unsigned int  )2;
        sqlstm.sqhsts[46] = (         int  )0;
        sqlstm.sqindv[46] = (         void  *)0;
        sqlstm.sqinds[46] = (         int  )0;
        sqlstm.sqharm[46] = (unsigned int  )0;
        sqlstm.sqadto[46] = (unsigned short )0;
        sqlstm.sqtdso[46] = (unsigned short )0;
        sqlstm.sqhstv[47] = (         void  *)(tlBcTransLog.flag_6);
        sqlstm.sqhstl[47] = (unsigned int  )2;
        sqlstm.sqhsts[47] = (         int  )0;
        sqlstm.sqindv[47] = (         void  *)0;
        sqlstm.sqinds[47] = (         int  )0;
        sqlstm.sqharm[47] = (unsigned int  )0;
        sqlstm.sqadto[47] = (unsigned short )0;
        sqlstm.sqtdso[47] = (unsigned short )0;
        sqlstm.sqhstv[48] = (         void  *)(tlBcTransLog.flag_7);
        sqlstm.sqhstl[48] = (unsigned int  )2;
        sqlstm.sqhsts[48] = (         int  )0;
        sqlstm.sqindv[48] = (         void  *)0;
        sqlstm.sqinds[48] = (         int  )0;
        sqlstm.sqharm[48] = (unsigned int  )0;
        sqlstm.sqadto[48] = (unsigned short )0;
        sqlstm.sqtdso[48] = (unsigned short )0;
        sqlstm.sqhstv[49] = (         void  *)(tlBcTransLog.fld_5);
        sqlstm.sqhstl[49] = (unsigned int  )13;
        sqlstm.sqhsts[49] = (         int  )0;
        sqlstm.sqindv[49] = (         void  *)0;
        sqlstm.sqinds[49] = (         int  )0;
        sqlstm.sqharm[49] = (unsigned int  )0;
        sqlstm.sqadto[49] = (unsigned short )0;
        sqlstm.sqtdso[49] = (unsigned short )0;
        sqlstm.sqhstv[50] = (         void  *)(tlBcTransLog.fld_8);
        sqlstm.sqhstl[50] = (unsigned int  )9;
        sqlstm.sqhsts[50] = (         int  )0;
        sqlstm.sqindv[50] = (         void  *)0;
        sqlstm.sqinds[50] = (         int  )0;
        sqlstm.sqharm[50] = (unsigned int  )0;
        sqlstm.sqadto[50] = (unsigned short )0;
        sqlstm.sqtdso[50] = (unsigned short )0;
        sqlstm.sqhstv[51] = (         void  *)(tlBcTransLog.fld_28);
        sqlstm.sqhstl[51] = (unsigned int  )7;
        sqlstm.sqhsts[51] = (         int  )0;
        sqlstm.sqindv[51] = (         void  *)0;
        sqlstm.sqinds[51] = (         int  )0;
        sqlstm.sqharm[51] = (unsigned int  )0;
        sqlstm.sqadto[51] = (unsigned short )0;
        sqlstm.sqtdso[51] = (unsigned short )0;
        sqlstm.sqhstv[52] = (         void  *)(tlBcTransLog.fld_34);
        sqlstm.sqhstl[52] = (unsigned int  )29;
        sqlstm.sqhsts[52] = (         int  )0;
        sqlstm.sqindv[52] = (         void  *)0;
        sqlstm.sqinds[52] = (         int  )0;
        sqlstm.sqharm[52] = (unsigned int  )0;
        sqlstm.sqadto[52] = (unsigned short )0;
        sqlstm.sqtdso[52] = (unsigned short )0;
        sqlstm.sqhstv[53] = (         void  *)(tlBcTransLog.fld_44);
        sqlstm.sqhstl[53] = (unsigned int  )28;
        sqlstm.sqhsts[53] = (         int  )0;
        sqlstm.sqindv[53] = (         void  *)0;
        sqlstm.sqinds[53] = (         int  )0;
        sqlstm.sqharm[53] = (unsigned int  )0;
        sqlstm.sqadto[53] = (unsigned short )0;
        sqlstm.sqtdso[53] = (unsigned short )0;
        sqlstm.sqhstv[54] = (         void  *)(tlBcTransLog.fld_45);
        sqlstm.sqhstl[54] = (unsigned int  )77;
        sqlstm.sqhsts[54] = (         int  )0;
        sqlstm.sqindv[54] = (         void  *)0;
        sqlstm.sqinds[54] = (         int  )0;
        sqlstm.sqharm[54] = (unsigned int  )0;
        sqlstm.sqadto[54] = (unsigned short )0;
        sqlstm.sqtdso[54] = (unsigned short )0;
        sqlstm.sqhstv[55] = (         void  *)(tlBcTransLog.fld_46);
        sqlstm.sqhstl[55] = (unsigned int  )31;
        sqlstm.sqhsts[55] = (         int  )0;
        sqlstm.sqindv[55] = (         void  *)0;
        sqlstm.sqinds[55] = (         int  )0;
        sqlstm.sqharm[55] = (unsigned int  )0;
        sqlstm.sqadto[55] = (unsigned short )0;
        sqlstm.sqtdso[55] = (unsigned short )0;
        sqlstm.sqhstv[56] = (         void  *)(tlBcTransLog.fld_48);
        sqlstm.sqhstl[56] = (unsigned int  )191;
        sqlstm.sqhsts[56] = (         int  )0;
        sqlstm.sqindv[56] = (         void  *)0;
        sqlstm.sqinds[56] = (         int  )0;
        sqlstm.sqharm[56] = (unsigned int  )0;
        sqlstm.sqadto[56] = (unsigned short )0;
        sqlstm.sqtdso[56] = (unsigned short )0;
        sqlstm.sqhstv[57] = (         void  *)(tlBcTransLog.fld_61);
        sqlstm.sqhstl[57] = (unsigned int  )31;
        sqlstm.sqhsts[57] = (         int  )0;
        sqlstm.sqindv[57] = (         void  *)0;
        sqlstm.sqinds[57] = (         int  )0;
        sqlstm.sqharm[57] = (unsigned int  )0;
        sqlstm.sqadto[57] = (unsigned short )0;
        sqlstm.sqtdso[57] = (unsigned short )0;
        sqlstm.sqhstv[58] = (         void  *)(tlBcTransLog.center_sys_tra_no);
        sqlstm.sqhstl[58] = (unsigned int  )7;
        sqlstm.sqhsts[58] = (         int  )0;
        sqlstm.sqindv[58] = (         void  *)0;
        sqlstm.sqinds[58] = (         int  )0;
        sqlstm.sqharm[58] = (unsigned int  )0;
        sqlstm.sqadto[58] = (unsigned short )0;
        sqlstm.sqtdso[58] = (unsigned short )0;
        sqlstm.sqhstv[59] = (         void  *)(tlBcTransLog.center_term_id);
        sqlstm.sqhstl[59] = (unsigned int  )9;
        sqlstm.sqhsts[59] = (         int  )0;
        sqlstm.sqindv[59] = (         void  *)0;
        sqlstm.sqinds[59] = (         int  )0;
        sqlstm.sqharm[59] = (unsigned int  )0;
        sqlstm.sqadto[59] = (unsigned short )0;
        sqlstm.sqtdso[59] = (unsigned short )0;
        sqlstm.sqhstv[60] = (         void  *)(tlBcTransLog.center_mchnt_cd);
        sqlstm.sqhstl[60] = (unsigned int  )21;
        sqlstm.sqhsts[60] = (         int  )0;
        sqlstm.sqindv[60] = (         void  *)0;
        sqlstm.sqinds[60] = (         int  )0;
        sqlstm.sqharm[60] = (unsigned int  )0;
        sqlstm.sqadto[60] = (unsigned short )0;
        sqlstm.sqtdso[60] = (unsigned short )0;
        sqlstm.sqhstv[61] = (         void  *)(tlBcTransLog.bill_id_rcv);
        sqlstm.sqhstl[61] = (unsigned int  )22;
        sqlstm.sqhsts[61] = (         int  )0;
        sqlstm.sqindv[61] = (         void  *)0;
        sqlstm.sqinds[61] = (         int  )0;
        sqlstm.sqharm[61] = (unsigned int  )0;
        sqlstm.sqadto[61] = (unsigned short )0;
        sqlstm.sqtdso[61] = (unsigned short )0;
        sqlstm.sqhstv[62] = (         void  *)(tlBcTransLog.card_attr);
        sqlstm.sqhstl[62] = (unsigned int  )3;
        sqlstm.sqhsts[62] = (         int  )0;
        sqlstm.sqindv[62] = (         void  *)0;
        sqlstm.sqinds[62] = (         int  )0;
        sqlstm.sqharm[62] = (unsigned int  )0;
        sqlstm.sqadto[62] = (unsigned short )0;
        sqlstm.sqtdso[62] = (unsigned short )0;
        sqlstm.sqhstv[63] = (         void  *)(tlBcTransLog.iss_ins_id_cd);
        sqlstm.sqhstl[63] = (unsigned int  )9;
        sqlstm.sqhsts[63] = (         int  )0;
        sqlstm.sqindv[63] = (         void  *)0;
        sqlstm.sqinds[63] = (         int  )0;
        sqlstm.sqharm[63] = (unsigned int  )0;
        sqlstm.sqadto[63] = (unsigned short )0;
        sqlstm.sqtdso[63] = (unsigned short )0;
        sqlstm.sqhstv[64] = (         void  *)(tlBcTransLog.pname);
        sqlstm.sqhstl[64] = (unsigned int  )211;
        sqlstm.sqhsts[64] = (         int  )0;
        sqlstm.sqindv[64] = (         void  *)0;
        sqlstm.sqinds[64] = (         int  )0;
        sqlstm.sqharm[64] = (unsigned int  )0;
        sqlstm.sqadto[64] = (unsigned short )0;
        sqlstm.sqtdso[64] = (unsigned short )0;
        sqlstm.sqhstv[65] = (         void  *)(tlBcTransLog.encrypt_info);
        sqlstm.sqhstl[65] = (unsigned int  )49;
        sqlstm.sqhsts[65] = (         int  )0;
        sqlstm.sqindv[65] = (         void  *)0;
        sqlstm.sqinds[65] = (         int  )0;
        sqlstm.sqharm[65] = (unsigned int  )0;
        sqlstm.sqadto[65] = (unsigned short )0;
        sqlstm.sqtdso[65] = (unsigned short )0;
        sqlstm.sqhstv[66] = (         void  *)(tlBcTransLog.id_type);
        sqlstm.sqhstl[66] = (unsigned int  )3;
        sqlstm.sqhsts[66] = (         int  )0;
        sqlstm.sqindv[66] = (         void  *)0;
        sqlstm.sqinds[66] = (         int  )0;
        sqlstm.sqharm[66] = (unsigned int  )0;
        sqlstm.sqadto[66] = (unsigned short )0;
        sqlstm.sqtdso[66] = (unsigned short )0;
        sqlstm.sqhstv[67] = (         void  *)(tlBcTransLog.originaDataElements);
        sqlstm.sqhstl[67] = (unsigned int  )44;
        sqlstm.sqhsts[67] = (         int  )0;
        sqlstm.sqindv[67] = (         void  *)0;
        sqlstm.sqinds[67] = (         int  )0;
        sqlstm.sqharm[67] = (unsigned int  )0;
        sqlstm.sqadto[67] = (unsigned short )0;
        sqlstm.sqtdso[67] = (unsigned short )0;
        sqlstm.sqhstv[68] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[68] = (unsigned int  )5;
        sqlstm.sqhsts[68] = (         int  )0;
        sqlstm.sqindv[68] = (         void  *)0;
        sqlstm.sqinds[68] = (         int  )0;
        sqlstm.sqharm[68] = (unsigned int  )0;
        sqlstm.sqadto[68] = (unsigned short )0;
        sqlstm.sqtdso[68] = (unsigned short )0;
        sqlstm.sqhstv[69] = (         void  *)mchnt_cd;
        sqlstm.sqhstl[69] = (unsigned int  )21;
        sqlstm.sqhsts[69] = (         int  )0;
        sqlstm.sqindv[69] = (         void  *)0;
        sqlstm.sqinds[69] = (         int  )0;
        sqlstm.sqharm[69] = (unsigned int  )0;
        sqlstm.sqadto[69] = (unsigned short )0;
        sqlstm.sqtdso[69] = (unsigned short )0;
        sqlstm.sqhstv[70] = (         void  *)term_id;
        sqlstm.sqhstl[70] = (unsigned int  )9;
        sqlstm.sqhsts[70] = (         int  )0;
        sqlstm.sqindv[70] = (         void  *)0;
        sqlstm.sqinds[70] = (         int  )0;
        sqlstm.sqharm[70] = (unsigned int  )0;
        sqlstm.sqadto[70] = (unsigned short )0;
        sqlstm.sqtdso[70] = (unsigned short )0;
        sqlstm.sqhstv[71] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[71] = (unsigned int  )7;
        sqlstm.sqhsts[71] = (         int  )0;
        sqlstm.sqindv[71] = (         void  *)0;
        sqlstm.sqinds[71] = (         int  )0;
        sqlstm.sqharm[71] = (unsigned int  )0;
        sqlstm.sqadto[71] = (unsigned short )0;
        sqlstm.sqtdso[71] = (unsigned short )0;
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
        	strcpy( tlBcTransLog.flag_verifi, "2" );
            strcpy( tlBcTransLog.is_over, "2" );
            strcpy( tlBcTransLog.fld_28, p_tlBcTransLog->fld_28 );
            strcpy( tlBcTransLog.settle_dt, p_tlBcTransLog->settle_dt );
            strcpy( tlBcTransLog.retri_ref_no, p_tlBcTransLog->retri_ref_no );
            strcpy( tlBcTransLog.bc_settle_in, p_tlBcTransLog->bc_settle_in );
            memcpy( tlBcTransLog.resp_cd, p_tlBcTransLog->fld_28 + 4, 2);
            memcpy( tlBcTransLog.resp_cd_rcv, p_tlBcTransLog->fld_28, 6);
            memcpy(p_tlBcTransLog, &tlBcTransLog, sizeof(tl_bc_trans_log_def));
            /* EXEC SQL UPDATE tl_bc_trans_log SET flag_lock='0',flag_verifi='2',is_over='1',
            			fld_28=:tlBcTransLog.fld_28,
            			settle_dt=:tlBcTransLog.settle_dt,
            			retri_ref_no=:tlBcTransLog.retri_ref_no,
            			bc_settle_in = :tlBcTransLog.bc_settle_in,
            			resp_cd = :tlBcTransLog.resp_cd,
            			resp_cd_rcv = :tlBcTransLog.resp_cd_rcv
					WHERE loc_trans_dt=:loc_trans_dt
                           AND center_mchnt_cd=:mchnt_cd
                           AND center_term_id=:term_id
                           AND center_sys_tra_no=:sys_tra_no
                           ; */ 

{
            struct sqlexd sqlstm;
            sqlorat((void **)0, &sqlctx, &oraca);
            sqlstm.sqlvsn = 12;
            sqlstm.arrsiz = 72;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "update tl_bc_trans_log  set flag_lock='0',flag_v\
erifi='2',is_over='1',fld_28=:b0,settle_dt=:b1,retri_ref_no=:b2,bc_settle_in=\
:b3,resp_cd=:b4,resp_cd_rcv=:b5 where (((loc_trans_dt=:b6 and center_mchnt_cd\
=:b7) and center_term_id=:b8) and center_sys_tra_no=:b9)";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )2770;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)4352;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)(tlBcTransLog.fld_28);
            sqlstm.sqhstl[0] = (unsigned int  )7;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)(tlBcTransLog.settle_dt);
            sqlstm.sqhstl[1] = (unsigned int  )5;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)(tlBcTransLog.retri_ref_no);
            sqlstm.sqhstl[2] = (unsigned int  )13;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned int  )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (         void  *)(tlBcTransLog.bc_settle_in);
            sqlstm.sqhstl[3] = (unsigned int  )2;
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         void  *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned int  )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (         void  *)(tlBcTransLog.resp_cd);
            sqlstm.sqhstl[4] = (unsigned int  )3;
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         void  *)0;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned int  )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqhstv[5] = (         void  *)(tlBcTransLog.resp_cd_rcv);
            sqlstm.sqhstl[5] = (unsigned int  )7;
            sqlstm.sqhsts[5] = (         int  )0;
            sqlstm.sqindv[5] = (         void  *)0;
            sqlstm.sqinds[5] = (         int  )0;
            sqlstm.sqharm[5] = (unsigned int  )0;
            sqlstm.sqadto[5] = (unsigned short )0;
            sqlstm.sqtdso[5] = (unsigned short )0;
            sqlstm.sqhstv[6] = (         void  *)loc_trans_dt;
            sqlstm.sqhstl[6] = (unsigned int  )5;
            sqlstm.sqhsts[6] = (         int  )0;
            sqlstm.sqindv[6] = (         void  *)0;
            sqlstm.sqinds[6] = (         int  )0;
            sqlstm.sqharm[6] = (unsigned int  )0;
            sqlstm.sqadto[6] = (unsigned short )0;
            sqlstm.sqtdso[6] = (unsigned short )0;
            sqlstm.sqhstv[7] = (         void  *)mchnt_cd;
            sqlstm.sqhstl[7] = (unsigned int  )21;
            sqlstm.sqhsts[7] = (         int  )0;
            sqlstm.sqindv[7] = (         void  *)0;
            sqlstm.sqinds[7] = (         int  )0;
            sqlstm.sqharm[7] = (unsigned int  )0;
            sqlstm.sqadto[7] = (unsigned short )0;
            sqlstm.sqtdso[7] = (unsigned short )0;
            sqlstm.sqhstv[8] = (         void  *)term_id;
            sqlstm.sqhstl[8] = (unsigned int  )9;
            sqlstm.sqhsts[8] = (         int  )0;
            sqlstm.sqindv[8] = (         void  *)0;
            sqlstm.sqinds[8] = (         int  )0;
            sqlstm.sqharm[8] = (unsigned int  )0;
            sqlstm.sqadto[8] = (unsigned short )0;
            sqlstm.sqtdso[8] = (unsigned short )0;
            sqlstm.sqhstv[9] = (         void  *)sys_tra_no;
            sqlstm.sqhstl[9] = (unsigned int  )7;
            sqlstm.sqhsts[9] = (         int  )0;
            sqlstm.sqindv[9] = (         void  *)0;
            sqlstm.sqinds[9] = (         int  )0;
            sqlstm.sqharm[9] = (unsigned int  )0;
            sqlstm.sqadto[9] = (unsigned short )0;
            sqlstm.sqtdso[9] = (unsigned short )0;
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
            if( sqlca.sqlcode != 0 )
            {
            	glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:UPDATE3>Update Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
            }
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:UPDATE3>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:UPDATE3>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )2825;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        break;

    case GLB_DB_UPDATE11:

        /*copy buff*/
        memcpy(&tlBcTransLog, p_tlBcTransLog, sizeof(tl_bc_trans_log_def));

        strcpy_safe(loc_trans_dt, p_tlBcTransLog->loc_trans_dt, sizeof(loc_trans_dt)-1);
        rtrim_null(loc_trans_dt,' ');
        
        strcpy_safe(term_id, p_tlBcTransLog->term_id, sizeof(term_id)-1);
        rtrim_null(term_id,' ');

        strcpy_safe(mchnt_cd, p_tlBcTransLog->mchnt_cd, sizeof(mchnt_cd)-1);
        rtrim_null(mchnt_cd,' ');

				 strcpy_safe(sys_tra_no, p_tlBcTransLog->sys_tra_no, sizeof(sys_tra_no)-1);
        rtrim_null(sys_tra_no,' ');

        /* EXEC SQL UPDATE tl_bc_trans_log SET flag_lock='0' 
	        WHERE loc_trans_dt=:loc_trans_dt
					AND mchnt_cd=:mchnt_cd
					AND sys_tra_no=:sys_tra_no
					AND term_id=:term_id
                                                      ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "update tl_bc_trans_log  set flag_lock='0' where (((l\
oc_trans_dt=:b0 and mchnt_cd=:b1) and sys_tra_no=:b2) and term_id=:b3)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )2840;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)loc_trans_dt;
        sqlstm.sqhstl[0] = (unsigned int  )5;
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
        sqlstm.sqhstv[2] = (         void  *)sys_tra_no;
        sqlstm.sqhstl[2] = (unsigned int  )7;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)term_id;
        sqlstm.sqhstl[3] = (unsigned int  )9;
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


        /* update result */
        *p_sqlCode = sqlca.sqlcode;
        if ( sqlca.sqlcode == 0 )
        {
            /*memcpy(p_tlBcTransLog, &tlBcTransLog, sizeof(tl_bc_trans_log_def));*/
        } else if ( sqlca.sqlcode == 1403 )
        {
            *p_sqlCode = SQL_ERD_NONE_RECORD;
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:UPDATE11>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        } else
        {
            glb_Log(0,0, "<TL_BC_TRANS_LOG.pc:tlBcTransLogOpr:UPDATE11>Search Record (loc_trans_dt:%s,term_id:%s,mchnt_cd:%s,sys_tra_no:%s) Error.\n[sqlCode:%d]errmsg:\n----------\n%s\n", loc_trans_dt,term_id,mchnt_cd,sys_tra_no, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
        }
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 72;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )2871;
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


int tlBcTransLogOpr2(double *out_at, double *out_num, 
	char *_settle_dt, char *_pri_acct_no ,char *_mchnt_cd,
	int *p_sqlCode)
{
	/* EXEC SQL WHENEVER SQLERROR continue; */ 

    /* EXEC SQL BEGIN DECLARE SECTION; */ 
   
    char  settle_dt       [8  + 1] ;
    char  mchnt_cd        [20 + 1] ;   
    char  pri_acct_no     [19 + 1] ;
    double at = 0 ;
    double num = 0;
  /* EXEC SQL END DECLARE SECTION; */ 

	
	if( 0 == _settle_dt || 0 == _pri_acct_no && 0 == _mchnt_cd ){
		*p_sqlCode = 0;
		return -1;
	}
	
  memset(settle_dt,   0, sizeof(settle_dt)  );
  memset(mchnt_cd,    0, sizeof(mchnt_cd)   );
  memset(pri_acct_no, 0, sizeof(pri_acct_no));
	
	if(_settle_dt)strcpy_safe(settle_dt, _settle_dt, sizeof(settle_dt)-1 );
	if(_mchnt_cd)strcpy_safe(mchnt_cd,  _mchnt_cd, sizeof(mchnt_cd)-1 );
	if(_pri_acct_no)strcpy_safe(pri_acct_no, _pri_acct_no, sizeof(pri_acct_no)-1 );
	
	/* 按日期 + 卡号查询 + 商户号查询*/
	if( _pri_acct_no && _mchnt_cd){
		glb_Debug(0,0,"<FILE:%s,LINE:%d>按日期%s，卡号%s和商户号%s查询.\n",__FILE__,__LINE__, settle_dt, pri_acct_no, mchnt_cd );
		/* EXEC SQL SELECT COUNT(*), sum(trans_at) into :num, :at FROM tl_bc_trans_log WHERE
			settle_dt=:settle_dt and mchnt_cd=:mchnt_cd and pri_acct_no=:pri_acct_no and bc_settle_in='1'
		; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 72;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(*)  ,sum(trans_at) into :b0,:b1  from tl_bc_t\
rans_log where (((settle_dt=:b2 and mchnt_cd=:b3) and pri_acct_no=:b4) and bc\
_settle_in='1')";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2886;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&num;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(double);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&at;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(double);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)settle_dt;
  sqlstm.sqhstl[2] = (unsigned int  )9;
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
  sqlstm.sqhstv[4] = (         void  *)pri_acct_no;
  sqlstm.sqhstl[4] = (unsigned int  )20;
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

		
	}
	
	/* 按日期 + 商户号查询 */
	else if( _mchnt_cd ){
		glb_Debug(0,0,"<FILE:%s,LINE:%d>按日期%s和商户号%s查询.\n", __FILE__,__LINE__,settle_dt, mchnt_cd );
		/* EXEC SQL SELECT COUNT(*), sum(trans_at) into :num, :at FROM tl_bc_trans_log WHERE
			settle_dt=:settle_dt and mchnt_cd=:mchnt_cd and bc_settle_in='1'
		; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 72;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(*)  ,sum(trans_at) into :b0,:b1  from tl_bc_t\
rans_log where ((settle_dt=:b2 and mchnt_cd=:b3) and bc_settle_in='1')";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2921;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&num;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(double);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&at;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(double);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)settle_dt;
  sqlstm.sqhstl[2] = (unsigned int  )9;
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


	}
	
	/* 按日期 + 卡号查询 */
	else if( _pri_acct_no ){
		glb_Debug(0,0,"<FILE:%s,LINE:%d>按日期%s和卡号%s查询.\n",__FILE__,__LINE__, settle_dt, pri_acct_no );
		/* EXEC SQL SELECT COUNT(*), sum(trans_at) into :num, :at FROM tl_bc_trans_log WHERE
			settle_dt=:settle_dt and pri_acct_no=:pri_acct_no and bc_settle_in='1'
		; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 72;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(*)  ,sum(trans_at) into :b0,:b1  from tl_bc_t\
rans_log where ((settle_dt=:b2 and pri_acct_no=:b3) and bc_settle_in='1')";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )2952;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&num;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(double);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&at;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(double);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)settle_dt;
  sqlstm.sqhstl[2] = (unsigned int  )9;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)pri_acct_no;
  sqlstm.sqhstl[3] = (unsigned int  )20;
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


		glb_Debug(0,0,"<FILE:%s,LINE:%d>按日期%s和卡号%s查询结果笔数%f, 金额%f.\n",__FILE__,__LINE__, settle_dt, pri_acct_no,num, at);
	}
	else{
 		return -1;
	}
		
 *p_sqlCode = sqlca.sqlcode;
 if ( 0 == sqlca.sqlcode ){
 	  /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 72;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )2983;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


 		if(out_at) *out_at =   at;
 		if(out_num) *out_num =  num; 			
 		return 0;
 }else if ( sqlca.sqlcode == 1403 ){
 	  /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 72;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )2998;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


 	  if(out_at) *out_at = 0;
 		if(out_num) *out_num = 0;
 		return 0;
 }else{
 	  glb_Log( 0,0, "tlBcTransLogOpr2 发生数据库异常(%d).\n[sqlCode:%d]errmsg:\n----------\n%s\n", sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
 	  /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 72;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )3013;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


 	  return -1;
 }
 	
	return -1;
}

int tlBcTransLogOpr3(long long *out_at, long long *out_num, 
	char *_month, char *_pri_acct_no ,char *_mchnt_cd,
	int *p_sqlCode)
{
	/* EXEC SQL WHENEVER SQLERROR continue; */ 

  /* EXEC SQL BEGIN DECLARE SECTION; */ 
   
    char  settle_dt       [8  + 1] ;
    char  mchnt_cd        [15 + 1] ;   
    char  pri_acct_no     [19 + 1] ;
    long long at ;
    long long num;
  /* EXEC SQL END DECLARE SECTION; */ 

	
	if( 0 == _month || 0 == _pri_acct_no && 0 == _mchnt_cd ){
		*p_sqlCode = 0;
		return -1;
	}
	
  memset(settle_dt,   0, sizeof( _month )  );
  memset(mchnt_cd,    0, sizeof(mchnt_cd)   );
  memset(pri_acct_no, 0, sizeof(pri_acct_no));
	
	strcpy_safe(settle_dt, _month, 6 );
	strcpy_safe(mchnt_cd,  _mchnt_cd, sizeof(mchnt_cd)-1 );
	strcpy_safe(pri_acct_no, _pri_acct_no, sizeof(pri_acct_no)-1 );
	rtrim(settle_dt);
	strcat(settle_dt, "%");
	
	/* 按日期 + 卡号查询 + 商户号查询*/
	if( _pri_acct_no && _mchnt_cd){
		/* EXEC SQL SELECT COUNT(*), sum(trans_at) into :num, :at FROM tl_bc_trans_log WHERE
			settle_dt like :settle_dt and mchnt_cd=:mchnt_cd and pri_acct_no=:pri_acct_no and bc_settle_in='1'
		; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 72;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(*)  ,sum(trans_at) into :b0,:b1  from tl_bc_t\
rans_log where (((settle_dt like :b2 and mchnt_cd=:b3) and pri_acct_no=:b4) a\
nd bc_settle_in='1')";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3028;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&num;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(long long);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&at;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(long long);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)settle_dt;
  sqlstm.sqhstl[2] = (unsigned int  )9;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)mchnt_cd;
  sqlstm.sqhstl[3] = (unsigned int  )16;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)pri_acct_no;
  sqlstm.sqhstl[4] = (unsigned int  )20;
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

		
	}
	
	/* 按日期 + 商户号查询 */
	else if( _mchnt_cd ){
		/* EXEC SQL SELECT COUNT(*), sum(trans_at) into :num, :at FROM tl_bc_trans_log WHERE
			settle_dt like :settle_dt and mchnt_cd=:mchnt_cd and bc_settle_in='1'
		; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 72;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(*)  ,sum(trans_at) into :b0,:b1  from tl_bc_t\
rans_log where ((settle_dt like :b2 and mchnt_cd=:b3) and bc_settle_in='1')";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3063;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&num;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(long long);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&at;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(long long);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)settle_dt;
  sqlstm.sqhstl[2] = (unsigned int  )9;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)mchnt_cd;
  sqlstm.sqhstl[3] = (unsigned int  )16;
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


	}
	
	/* 按日期 + 卡号查询 */
	else if( _pri_acct_no ){
		/* EXEC SQL SELECT COUNT(*), sum(trans_at) into :num, :at FROM tl_bc_trans_log WHERE
			settle_dt like :settle_dt and pri_acct_no=:pri_acct_no and bc_settle_in='1'
		; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 72;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select count(*)  ,sum(trans_at) into :b0,:b1  from tl_bc_t\
rans_log where ((settle_dt like :b2 and pri_acct_no=:b3) and bc_settle_in='1'\
)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )3094;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&num;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(long long);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&at;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(long long);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)settle_dt;
  sqlstm.sqhstl[2] = (unsigned int  )9;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)pri_acct_no;
  sqlstm.sqhstl[3] = (unsigned int  )20;
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

		
	}
		
 *p_sqlCode = sqlca.sqlcode;
 if ( 0 == sqlca.sqlcode ){
 	  /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 72;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )3125;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


 		if(out_at) *out_at = at;
 		if(out_num) *out_num = num; 			
 		return 0;
 }else if ( sqlca.sqlcode == 1403 ){
 	  /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 72;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )3140;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


 	  if(out_at) *out_at = 0;
 		if(out_num) *out_num = 0;
 		return 0;
 }else{
 	  glb_Log( 0,0, "tlBcTransLogOpr2 发生数据库异常(%d).\n[sqlCode:%d]errmsg:\n----------\n%s\n", sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc );
 	  /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 72;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )3155;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


 	  return -1;
 }
 	
	return -1;
}

