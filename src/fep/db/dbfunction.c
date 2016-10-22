
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
           char  filnam[14];
};
static struct sqlcxp sqlfpn =
{
    13,
    "dbfunction.pc"
};


static unsigned int sqlctx = 567451;


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
            void  *sqhstv[8];
   unsigned int   sqhstl[8];
            int   sqhsts[8];
            void  *sqindv[8];
            int   sqinds[8];
   unsigned int   sqharm[8];
   unsigned int   *sqharc[8];
   unsigned short  sqadto[8];
   unsigned short  sqtdso[8];
} sqlstm = {12,8};

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

typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,4274,850,0,0,
5,0,0,0,0,0,27,51,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,10,0,0,1,10,0,0,
36,0,0,2,0,0,32,75,0,0,0,0,0,1,0,
51,0,0,3,0,0,30,82,0,0,0,0,0,1,0,
66,0,0,4,183,0,4,113,0,0,8,1,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,
97,0,0,2,97,0,0,1,68,0,0,
113,0,0,5,59,0,5,128,0,0,2,2,0,1,0,1,68,0,0,1,97,0,0,
136,0,0,6,0,0,29,133,0,0,0,0,0,1,0,
151,0,0,7,0,0,29,141,0,0,0,0,0,1,0,
166,0,0,8,0,0,29,151,0,0,0,0,0,1,0,
181,0,0,9,179,0,4,177,0,0,8,1,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,
97,0,0,2,97,0,0,1,68,0,0,
228,0,0,10,42,0,2,191,0,0,1,1,0,1,0,1,97,0,0,
247,0,0,11,0,0,29,192,0,0,0,0,0,1,0,
262,0,0,12,0,0,29,198,0,0,0,0,0,1,0,
277,0,0,13,0,0,29,208,0,0,0,0,0,1,0,
292,0,0,14,129,0,3,244,0,0,7,7,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,
97,0,0,1,97,0,0,
335,0,0,15,0,0,29,251,0,0,0,0,0,1,0,
350,0,0,16,0,0,29,256,0,0,0,0,0,1,0,
365,0,0,17,0,0,29,262,0,0,0,0,0,1,0,
380,0,0,18,156,0,4,285,0,0,8,1,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,
97,0,0,2,97,0,0,1,97,0,0,
427,0,0,19,42,0,2,300,0,0,1,1,0,1,0,1,97,0,0,
446,0,0,20,0,0,29,301,0,0,0,0,0,1,0,
461,0,0,21,0,0,29,307,0,0,0,0,0,1,0,
476,0,0,22,0,0,29,313,0,0,0,0,0,1,0,
491,0,0,23,132,0,3,347,0,0,7,7,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,
97,0,0,1,97,0,0,
534,0,0,24,0,0,29,354,0,0,0,0,0,1,0,
549,0,0,25,0,0,29,360,0,0,0,0,0,1,0,
564,0,0,26,0,0,29,366,0,0,0,0,0,1,0,
579,0,0,27,159,0,4,388,0,0,8,1,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,
97,0,0,2,97,0,0,1,97,0,0,
626,0,0,28,45,0,2,403,0,0,1,1,0,1,0,1,97,0,0,
645,0,0,29,0,0,29,404,0,0,0,0,0,1,0,
660,0,0,30,0,0,29,410,0,0,0,0,0,1,0,
675,0,0,31,0,0,29,416,0,0,0,0,0,1,0,
};


#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include "../include/dbstruct.h"


struct  _varchar {
  unsigned short len;
  unsigned char  arr[1];
};

typedef struct _varchar VARCHAR;

  /* EXEC SQL BEGIN DECLARE SECTION; */ 

       /* VARCHAR  vcaUserName[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } vcaUserName;

       /* VARCHAR  vcaPassWord[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } vcaPassWord;

  /* EXEC SQL END   DECLARE SECTION; */ 


/*   EXEC SQL INCLUDE sqlca;
 */ 
/*
 * $Header: sqlca.h 24-apr-2003.12:50:58 mkandarp Exp $ sqlca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */


int
DasConectDB()
{
  char  caDbCryptFlag[4];
  char  caDbPwd[20];
  char  caDbPwdCryp[20];
  char  caLogName[256];

  sprintf( vcaUserName.arr, "%s", (char *)getenv( "DBUSR" ));
  vcaUserName.len = strlen( vcaUserName.arr );


  memset( caDbPwdCryp, 0, 20 );
  sprintf( caDbPwdCryp, "%s", (char *)getenv( "DBPWD" ));



    memset( caDbPwd, 0, 20 );
    memcpy( caDbPwd, caDbPwdCryp, strlen( caDbPwdCryp ));


  memcpy( vcaPassWord.arr, caDbPwd, strlen( caDbPwd ));



  vcaPassWord.len = strlen( caDbPwd );

  /* EXEC SQL CONNECT :vcaUserName IDENTIFIED BY :vcaPassWord; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )10;
  sqlstm.offset = (unsigned int  )5;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&vcaUserName;
  sqlstm.sqhstl[0] = (unsigned int  )22;
  sqlstm.sqhsts[0] = (         int  )22;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&vcaPassWord;
  sqlstm.sqhstl[1] = (unsigned int  )22;
  sqlstm.sqhsts[1] = (         int  )22;
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
  sqlstm.sqlcmax = (unsigned int )100;
  sqlstm.sqlcmin = (unsigned int )2;
  sqlstm.sqlcincr = (unsigned int )1;
  sqlstm.sqlctimeout = (unsigned int )0;
  sqlstm.sqlcnowait = (unsigned int )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



  if ( sqlca.sqlcode == 0 )
  {

    return(0);

  } else {


    return(-1);

  }
}

int
DasEnd( int iDasOperate )
{


  switch ( iDasOperate) {
    case ROLLBACK:
  
     /* EXEC SQL ROLLBACK WORK;*/
      /* EXEC SQL ROLLBACK RELEASE; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )36;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



      break;

    case COMMIT:

      /*EXEC SQL COMMIT WORK;*/
      /* EXEC SQL COMMIT RELEASE; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )51;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      break;

    default :
      return( -1 );
  }

  return( 0 );

}


/*读取账单通知表*/
int ReadDelNotiOutTbl(struct TimeOut_REC *p_rec)
{


	/* EXEC SQL BEGIN DECLARE SECTION; */ 

  int    iRcvFolderId;		 
  int    iOrgFolderId;		 
  int    iFlags;           
  int    iMaxTimeout;			
  long   iExpiredTime;    
  char   caKey[50];	     
  char   caRemark[80];
	unsigned long now;
	/* EXEC SQL END DECLARE SECTION; */ 



	time(&now);

	/* EXEC SQL SELECT RcvFolderId,OrgFolderId,Flags,MaxTimeout,ExpiredTime,Key,Remark
	INTO :iRcvFolderId,:iOrgFolderId,:iFlags,:iMaxTimeout,:iExpiredTime,:caKey,:caRemark
	FROM tl_bc_bill_notify where ExpiredTime <= :now and rownum=1 for update; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select RcvFolderId ,OrgFolderId ,Flags ,MaxTimeout ,Expired\
Time ,Key ,Remark into :b0,:b1,:b2,:b3,:b4,:b5,:b6  from tl_bc_bill_notify wh\
ere (ExpiredTime<=:b7 and rownum=1) for update ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )66;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&iRcvFolderId;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&iOrgFolderId;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&iFlags;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&iMaxTimeout;
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&iExpiredTime;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(long);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)caKey;
 sqlstm.sqhstl[5] = (unsigned int  )50;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)caRemark;
 sqlstm.sqhstl[6] = (unsigned int  )80;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&now;
 sqlstm.sqhstl[7] = (unsigned int  )sizeof(long);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
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
		memcpy(p_rec->caKey,caKey,50);
		memcpy(p_rec->caRemark,caRemark,80);
		p_rec->iRcvFolderId=iRcvFolderId;
		p_rec->iOrgFolderId=iOrgFolderId;
		p_rec->iFlags=iFlags;
		p_rec->iMaxTimeout=iMaxTimeout;
		p_rec->iExpiredTime=iExpiredTime;
                now = now+300;
/*		exec sql delete from tl_bc_bill_notify where Key=:caKey; */
		/* exec sql update tl_bc_bill_notify set ExpiredTime=:now where Key=:caKey; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 8;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update tl_bc_bill_notify  set ExpiredTime=:b0 where Key=:b\
1";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )113;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&now;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(long);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)caKey;
  sqlstm.sqhstl[1] = (unsigned int  )50;
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

 
         if( sqlca.sqlcode !=0 )
         {
               dcs_log(sqlca.sqlerrm.sqlerrmc,sqlca.sqlerrm.sqlerrml,"update table tl_bc_bill_notify fail!");
	 }
  	 /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 8;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )136;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


  	dcs_debug(0,0, "update  noti out table success...caKey[%s],time[%ld]\n",caKey,now);
  	 
   	return DAS_NORMAL;
  }
  else if( sqlca.sqlcode == 1403 )
  {
/*  	    dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:delete  noti out table failuer...now[%d]\n",now); */
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 8;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )151;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return DAS_NOT_FOUND_ERR;
  }
  else if( sqlca.sqlcode == -3114 )
  {
	return -3114;
  }
  else
  {
      dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:delete  noti table failuer...\n");
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 8;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )166;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return DAS_OTHER_ERR;
  }	
}



/*读取超时表*/
int ReadDelTimeOutTbl(struct TimeOut_REC *p_rec)
{


	/* EXEC SQL BEGIN DECLARE SECTION; */ 

  int    iRcvFolderId;		 
  int    iOrgFolderId;		 
  int    iFlags;           
  int    iMaxTimeout;			
  long   iExpiredTime;    
  char   caKey[50];	     
  char   caRemark[80];
	unsigned long now;
	/* EXEC SQL END DECLARE SECTION; */ 



	time(&now);

	/* EXEC SQL SELECT RcvFolderId,OrgFolderId,Flags,MaxTimeout,ExpiredTime,Key,Remark
	INTO :iRcvFolderId,:iOrgFolderId,:iFlags,:iMaxTimeout,:iExpiredTime,:caKey,:caRemark
	FROM tl_bc_time_out where ExpiredTime < :now and rownum=1 for update; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select RcvFolderId ,OrgFolderId ,Flags ,MaxTimeout ,Expired\
Time ,Key ,Remark into :b0,:b1,:b2,:b3,:b4,:b5,:b6  from tl_bc_time_out where\
 (ExpiredTime<:b7 and rownum=1) for update ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )181;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&iRcvFolderId;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&iOrgFolderId;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&iFlags;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&iMaxTimeout;
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&iExpiredTime;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(long);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)caKey;
 sqlstm.sqhstl[5] = (unsigned int  )50;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)caRemark;
 sqlstm.sqhstl[6] = (unsigned int  )80;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&now;
 sqlstm.sqhstl[7] = (unsigned int  )sizeof(long);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
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
		memcpy(p_rec->caKey,caKey,50);
		memcpy(p_rec->caRemark,caRemark,80);
		p_rec->iRcvFolderId=iRcvFolderId;
		p_rec->iOrgFolderId=iOrgFolderId;
		p_rec->iFlags=iFlags;
		p_rec->iMaxTimeout=iMaxTimeout;
		p_rec->iExpiredTime=iExpiredTime;

		/* exec sql delete from tl_bc_time_out where Key=:caKey; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 8;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "delete  from tl_bc_time_out  where Key=:b0";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )228;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)caKey;
  sqlstm.sqhstl[0] = (unsigned int  )50;
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


  	 	/* EXEC SQL COMMIT WORK ; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 12;
     sqlstm.arrsiz = 8;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )247;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)4352;
     sqlstm.occurs = (unsigned int  )0;
     sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   		return DAS_NORMAL;
  }
  else if( sqlca.sqlcode == 1403 )
  {
    /*  dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:delete o time out table failuer...\n"); */
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 8;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )262;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return DAS_NOT_FOUND_ERR;
  }
  else if( sqlca.sqlcode == -3114 )
  {
	return -3114;
  }
  else
  {
        dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:delete  into time out table failuer...\n");
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 8;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )277;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return DAS_OTHER_ERR;
  }	

}



/*插入超时表*/
int InsTimeOutTBL(struct TimeOut_REC *p_rec)
{
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

  int    iRcvFolderId;		 
  int    iOrgFolderId;		 
  int    iFlags;           
  int    iMaxTimeout;			
  long   iExpiredTime;    
  char   caKey[50];	     
  char   caRemark[80];
	/* EXEC SQL END DECLARE SECTION; */ 


	long now;

	time(&now);

	memset(	caKey,0,sizeof(caKey));
	memset(	caRemark,0,sizeof(caRemark));
	memcpy(caKey,p_rec->caKey,50);
	memcpy(caRemark,p_rec->caRemark,80);
	iRcvFolderId=p_rec->iRcvFolderId;
	iOrgFolderId=p_rec->iOrgFolderId;
	iFlags=p_rec->iFlags;
	iMaxTimeout=p_rec->iMaxTimeout;
	iExpiredTime=now + iMaxTimeout;

	
	/* EXEC SQL insert into tl_bc_time_out
	(RcvFolderId,OrgFolderId,Flags,MaxTimeout,ExpiredTime,Key,Remark)
	 values
	(:iRcvFolderId,:iOrgFolderId,:iFlags,:iMaxTimeout,:iExpiredTime,:caKey,:caRemark); */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "insert into tl_bc_time_out (RcvFolderId,OrgFolderId,Flags,M\
axTimeout,ExpiredTime,Key,Remark) values (:b0,:b1,:b2,:b3,:b4,:b5,:b6)";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )292;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&iRcvFolderId;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&iOrgFolderId;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&iFlags;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&iMaxTimeout;
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&iExpiredTime;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(long);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)caKey;
 sqlstm.sqhstl[5] = (unsigned int  )50;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)caRemark;
 sqlstm.sqhstl[6] = (unsigned int  )80;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
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
       /* EXEC SQL COMMIT WORK ; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 8;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )335;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


       return DAS_NORMAL;
      }
      else if( sqlca.sqlcode == 1403 )
      {
          /* EXEC SQL COMMIT WORK ; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 12;
          sqlstm.arrsiz = 8;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )350;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)4352;
          sqlstm.occurs = (unsigned int  )0;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


          dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:insert into time out table failuer...\n");
          return DAS_DUPLICATE_ERR;
      }
      else
      {
          /* EXEC SQL COMMIT WORK ; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 12;
          sqlstm.arrsiz = 8;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )365;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)4352;
          sqlstm.occurs = (unsigned int  )0;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


          dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:insert into time out table failuer...\n");
          return DAS_OTHER_ERR;
      }
}
/*删除超时表*/
int DelTimeOutTBL(struct TimeOut_REC *p_rec)
{

	/* EXEC SQL BEGIN DECLARE SECTION; */ 

  int    iRcvFolderId;		 
  int    iOrgFolderId;		 
  int    iFlags;           
  int    iMaxTimeout;			
  long   iExpiredTime;    
  char   caKey[50];	     
  char   caRemark[80];
	/* EXEC SQL END DECLARE SECTION; */ 


	memset(	caKey,0,sizeof(caKey));
	memset(	caRemark,0,sizeof(caRemark));
	memcpy(caKey,p_rec->caKey,50);

	/* EXEC SQL SELECT RcvFolderId,OrgFolderId,Flags,MaxTimeout,ExpiredTime,Key,Remark
	INTO :iRcvFolderId,:iOrgFolderId,:iFlags,:iMaxTimeout,:iExpiredTime,:caKey,:caRemark
	FROM tl_bc_time_out where Key=:caKey for update
    ; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select RcvFolderId ,OrgFolderId ,Flags ,MaxTimeout ,Expired\
Time ,Key ,Remark into :b0,:b1,:b2,:b3,:b4,:b5,:b6  from tl_bc_time_out where\
 Key=:b5 for update ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )380;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&iRcvFolderId;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&iOrgFolderId;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&iFlags;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&iMaxTimeout;
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&iExpiredTime;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(long);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)caKey;
 sqlstm.sqhstl[5] = (unsigned int  )50;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)caRemark;
 sqlstm.sqhstl[6] = (unsigned int  )80;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)caKey;
 sqlstm.sqhstl[7] = (unsigned int  )50;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
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
	memcpy(p_rec->caKey,caKey,50);
	memcpy(p_rec->caRemark,caRemark,80);
	p_rec->iRcvFolderId=iRcvFolderId;
	p_rec->iOrgFolderId=iOrgFolderId;
	p_rec->iFlags=iFlags;
	p_rec->iMaxTimeout=iMaxTimeout;
	p_rec->iExpiredTime=iExpiredTime;

	/* exec sql delete from tl_bc_time_out where Key=:caKey; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "delete  from tl_bc_time_out  where Key=:b0";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )427;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)caKey;
 sqlstm.sqhstl[0] = (unsigned int  )50;
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


   /* EXEC SQL COMMIT WORK ; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 8;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )446;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   return DAS_NORMAL;
  }
  else if( sqlca.sqlcode == 1403 )
  {
  	dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:delete o time out table failuer...\n");
    /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 8;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )461;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return DAS_NOT_FOUND_ERR;
  }
  else
  {
  	dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:delete  into time out table failuer...\n");
    /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 8;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )476;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return DAS_OTHER_ERR;
  }
}


/*插入账单超时表*/
int InsTimeOutTBL_2(struct TimeOut_REC *p_rec)
{
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

  int    iRcvFolderId;		 
  int    iOrgFolderId;		 
  int    iFlags;           
  int    iMaxTimeout;			
  long   iExpiredTime;    
  char   caKey[50];	     
  char   caRemark[80];
	/* EXEC SQL END DECLARE SECTION; */ 


	long now;

	time(&now);

	memset(	caKey,0,sizeof(caKey));
	memset(	caRemark,0,sizeof(caRemark));
	memcpy(caKey,p_rec->caKey,50);
	memcpy(caRemark,p_rec->caRemark,80);
	iRcvFolderId=p_rec->iRcvFolderId;
	iOrgFolderId=p_rec->iOrgFolderId;
	iFlags=p_rec->iFlags;
	iMaxTimeout=p_rec->iMaxTimeout;
	iExpiredTime=now + iMaxTimeout;

	
	/* EXEC SQL insert into tl_bc_bill_notify
	(RcvFolderId,OrgFolderId,Flags,MaxTimeout,ExpiredTime,Key,Remark)
	 values
	(:iRcvFolderId,:iOrgFolderId,:iFlags,:iMaxTimeout,:iExpiredTime,:caKey,:caRemark); */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "insert into tl_bc_bill_notify (RcvFolderId,OrgFolderId,Flag\
s,MaxTimeout,ExpiredTime,Key,Remark) values (:b0,:b1,:b2,:b3,:b4,:b5,:b6)";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )491;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&iRcvFolderId;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&iOrgFolderId;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&iFlags;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&iMaxTimeout;
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&iExpiredTime;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(long);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)caKey;
 sqlstm.sqhstl[5] = (unsigned int  )50;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)caRemark;
 sqlstm.sqhstl[6] = (unsigned int  )80;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
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
       /* EXEC SQL COMMIT WORK ; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 8;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )534;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


       return DAS_NORMAL;
      }
      else if( sqlca.sqlcode == 1403 )
      {
      	dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:insert into time out table failuer...\n");
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 8;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )549;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


				return DAS_DUPLICATE_ERR;
      }
      else
      {
      	dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:insert into time out table failuer...\n");
        /* EXEC SQL COMMIT WORK ; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 8;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )564;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


				return DAS_OTHER_ERR;
      }
}
/*删除账单超时表*/
int DelTimeOutTBL_2(struct TimeOut_REC *p_rec)
{

	/* EXEC SQL BEGIN DECLARE SECTION; */ 

  int    iRcvFolderId;		 
  int    iOrgFolderId;		 
  int    iFlags;           
  int    iMaxTimeout;			
  long   iExpiredTime;    
  char   caKey[50];	     
  char   caRemark[80];
	/* EXEC SQL END DECLARE SECTION; */ 


	memset(	caKey,0,sizeof(caKey));
	memset(	caRemark,0,sizeof(caRemark));
	memcpy(caKey,p_rec->caKey,50);

	/* EXEC SQL SELECT RcvFolderId,OrgFolderId,Flags,MaxTimeout,ExpiredTime,Key,Remark
	INTO :iRcvFolderId,:iOrgFolderId,:iFlags,:iMaxTimeout,:iExpiredTime,:caKey,:caRemark
	FROM tl_bc_bill_notify where Key=:caKey for update
    ; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select RcvFolderId ,OrgFolderId ,Flags ,MaxTimeout ,Expired\
Time ,Key ,Remark into :b0,:b1,:b2,:b3,:b4,:b5,:b6  from tl_bc_bill_notify wh\
ere Key=:b5 for update ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )579;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&iRcvFolderId;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&iOrgFolderId;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&iFlags;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&iMaxTimeout;
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&iExpiredTime;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(long);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)caKey;
 sqlstm.sqhstl[5] = (unsigned int  )50;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)caRemark;
 sqlstm.sqhstl[6] = (unsigned int  )80;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)caKey;
 sqlstm.sqhstl[7] = (unsigned int  )50;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
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
	memcpy(p_rec->caKey,caKey,50);
	memcpy(p_rec->caRemark,caRemark,80);
	p_rec->iRcvFolderId=iRcvFolderId;
	p_rec->iOrgFolderId=iOrgFolderId;
	p_rec->iFlags=iFlags;
	p_rec->iMaxTimeout=iMaxTimeout;
	p_rec->iExpiredTime=iExpiredTime;

	/* exec sql delete from tl_bc_bill_notify where Key=:caKey; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "delete  from tl_bc_bill_notify  where Key=:b0";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )626;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)caKey;
 sqlstm.sqhstl[0] = (unsigned int  )50;
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


   /* EXEC SQL COMMIT WORK ; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 8;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )645;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   return DAS_NORMAL;
  }
  else if( sqlca.sqlcode == 1403 )
  {
  	dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:delete o time out table failuer...\n");
    /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 8;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )660;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return DAS_NOT_FOUND_ERR;
  }
  else
  {
  	dcs_log(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml, "ERR:delete  into time out table failuer...\n");
    /* EXEC SQL COMMIT WORK ; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 8;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )675;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return DAS_OTHER_ERR;
  }
}

/*
int GetAllTPASafRecord(char *pSettDate)
{

	EXEC SQL BEGIN DECLARE SECTION;
	char caWordDate[8+1];
	EXEC SQL END DECLARE SECTION;
	int iSqlCode=-1;
	
	memset(caWordDate,0,sizeof(caWordDate));
	memcpy(caWordDate,pSettDate,8);
	EXEC SQL DECLARE TAX_CURSOR CURSOR FOR
	select TAX_WORKDATE,TAX_ORIRESP,TAX_BANKNO,TAX_ORIORGCODE,TAX_ORIENTRUSTDATE,TAX_ORITRANO,TAX_VOUNO,TAX_DATETIME
	 	from taxpaystatus where TAX_WORKDATE=:caWordDate and TAX_SNDFLAG=0;
	EXEC SQL OPEN TAX_CURSOR;
	iSqlCode = sqlca.sqlcode;
	if(iSqlCode != 0)
	{
    ErrLog( 1000, "GetAllTPASafRecord oracle err", RPT_TO_LOG, sqlca.sqlerrm.sqlerrmc,
            sqlca.sqlerrm.sqlerrml );

	}
	return iSqlCode;
}

int dbclosecursor( )
{

	EXEC SQL CLOSE TAX_CURSOR;
	return 0;
	
}

int GetNextSafRecord(struct TAXPAYSTATUSst *stTaxPay)
{

	EXEC SQL BEGIN DECLARE SECTION;
	char TAX_WORKDATE       [9+1];            
	char TAX_ORIRESP        [5+1];
	char TAX_BANKNO         [12+1];
	char TAX_ORIORGCODE     [12+1];
	char TAX_ORIENTRUSTDATE [8+1];
	char TAX_ORITRANO       [8+1];
	char TAX_VOUNO          [18+1];
	char TAX_DATETIME       [14+1];
	EXEC SQL END DECLARE SECTION;
	int iSqlCode=-1;

	memset(TAX_WORKDATE,0,sizeof(TAX_WORKDATE));			
	memset(TAX_ORIRESP,0,sizeof(TAX_ORIRESP));			
	memset(TAX_BANKNO,0,sizeof(TAX_BANKNO));			
	memset(TAX_ORIORGCODE,0,sizeof(TAX_ORIORGCODE));			
	memset(TAX_ORIENTRUSTDATE,0,sizeof(TAX_ORIENTRUSTDATE));			
	memset(TAX_ORITRANO,0,sizeof(TAX_ORITRANO));			
	memset(TAX_VOUNO,0,sizeof(TAX_VOUNO));			
	memset(TAX_DATETIME,0,sizeof(TAX_DATETIME));
	
	EXEC SQL FETCH TAX_CURSOR into 
		:TAX_WORKDATE,:TAX_ORIRESP,:TAX_BANKNO,:TAX_ORIORGCODE,:TAX_ORIENTRUSTDATE,:TAX_ORITRANO,:TAX_VOUNO,:TAX_DATETIME;
	iSqlCode = sqlca.sqlcode;
	if(iSqlCode==0)
	{
				memcpy(	stTaxPay->tax_workdate,TAX_WORKDATE,8);
				memcpy(	stTaxPay->tax_oriresp,TAX_ORIRESP,5);
				memcpy(	stTaxPay->tax_bankno,TAX_BANKNO,12);
				memcpy(	stTaxPay->tax_oriorgcode,TAX_ORIORGCODE,12);
				memcpy(	stTaxPay->tax_orientrustdate,TAX_ORIENTRUSTDATE,8);
				memcpy(	stTaxPay->tax_oritrano,TAX_ORITRANO,8);
				memcpy(	stTaxPay->tax_vouno,TAX_VOUNO,18);
				memcpy(	stTaxPay->tax_datetime,TAX_DATETIME,14);

	}
	
	if(iSqlCode != 0)
	{
    ErrLog( 1000, "GetNextSafRecord oracle err", RPT_TO_LOG, sqlca.sqlerrm.sqlerrmc,sqlca.sqlerrm.sqlerrml );

	}
	return iSqlCode;
}

*/
