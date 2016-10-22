
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
           char  filnam[36];
};
static struct sqlcxp sqlfpn =
{
    35,
    "/home/tang/tang/src/fep/db/glbdb.pc"
};


static unsigned int sqlctx = 693445035;


static struct sqlexd {
   unsigned long  sqlvsn;
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
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
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
   unsigned char  *sqhstv[4];
   unsigned long  sqhstl[4];
            int   sqhsts[4];
            short *sqindv[4];
            int   sqinds[4];
   unsigned long  sqharm[4];
   unsigned long  *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {12,4};

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
5,0,0,1,0,0,32,45,0,0,0,0,0,1,0,
20,0,0,0,0,0,27,55,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,10,0,0,1,10,0,0,
};


#define SQLCA_STORAGE_CLASS extern 
#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include <stdarg.h>
#include <time.h>
#include "./inc/glbdb.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


#define UNAME_LEN      20 
#define PWD_LEN        11 
#define SQLNOTFOUND  1403

typedef char asciiz[PWD_LEN]; 

/* EXEC SQL TYPE asciiz IS CHARZ(PWD_LEN) REFERENCE; */ 
 
asciiz     username; 
asciiz     password; 

void sql_error(msg) 
    char *msg;
{ 
    char err_msg[512],buff[1024];
    size_t buf_len, msg_len;

    /* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 


    printf("\n%s\n", msg);

/* Call sqlglm() to get the complete text of the
 * error message.
 */
    buf_len = sizeof (err_msg);
    sqlglm(err_msg, &buf_len, &msg_len);
    printf("%.*s\n", msg_len, err_msg);
    
    sprintf(buff, "<sql_error>%.*s\n", msg_len, err_msg);
    /*glb_ErrLog( 1000, buff, RPT_TO_LOG, 0, 0 );*/

    /* EXEC SQL ROLLBACK RELEASE; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 0;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


    exit(EXIT_FAILURE);
} 

int glbdb_connect()
{
  strcpy(username, "test");
  strcpy(password, "test");
 
  /* EXEC SQL WHENEVER SQLERROR DO sql_error("ORACLE error--"); */ 

  /* EXEC SQL CONNECT :username IDENTIFIED BY :password; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )10;
  sqlstm.offset = (unsigned int  )20;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)username;
  sqlstm.sqhstl[0] = (unsigned long )11;
  sqlstm.sqhsts[0] = (         int  )11;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)password;
  sqlstm.sqhstl[1] = (unsigned long )11;
  sqlstm.sqhsts[1] = (         int  )11;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
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
  if (sqlca.sqlcode < 0) sql_error("ORACLE error--");
}

 
  printf("\nConnected to ORACLE as user: %s\n", username);
  
  return 0;  
}

int glbdb_disconnect()
{
	return 0;
}
                                                                     
struct tm *GetLocalTimeStruct()                                      
{                                                                    
	time_t time_cl ;                                                   
	time(&time_cl) ;                                                   
	return localtime( &time_cl );                                      
}                                                                    
                                                                     
char* glb_GetLocalTimeString(char *buf,int bufsize,const char *format)
{                                                                    
	struct tm *time_tm;                                                
	time_tm=GetLocalTimeStruct();                                      
	if( strftime(buf,bufsize,format,time_tm) != 0 )                    
		return buf;                                                      
	else                                                               
		return 0;                                                        
}            

int glb_ErrLog_Default(int iErrCode, char *pcaDispMsg,char cDumpDev,char *pcaDumpAddr,long lDumpLen)
{
	printf("<glbdb.pc:glb_ErrLog>ErrCode:[%d],%s\n", iErrCode, pcaDispMsg);
}

//typedef int (*PFUNC_LOG)(int, char *,char,char*,long);
typedef int (*PFUNC_LOG)( char *, int size, char* );
PFUNC_LOG PFunc_ErrLog = (PFUNC_LOG) &glb_ErrLog_Default;
PFUNC_LOG PFunc_Debug = (PFUNC_LOG) &glb_ErrLog_Default;

int glb_Log(char *binary, int size, char *buff, ...)
{
	char temp[1024*8];
  va_list ap;
  va_start(ap, buff);
  vsnprintf(temp, sizeof(temp), buff, ap);
  va_end(ap);
  if ( PFunc_ErrLog ){
     PFunc_ErrLog( binary, size, temp);
  }
}

int glb_Debug(char *binary, int size, char *buff, ...)
{
	char temp[1024*8];
  va_list ap;
  va_start(ap, buff);
  vsnprintf(temp, sizeof(temp), buff, ap);
  va_end(ap);
  if ( PFunc_ErrLog ){
     PFunc_Debug( binary, size, temp);
  }
}

int glb_ErrLog(int iErrCode, char *pcaDispMsg,char cDumpDev,char *pcaDumpAddr,long lDumpLen)
{
	if( iErrCode >= 1000 ){
		glb_Debug(pcaDumpAddr,lDumpLen,pcaDispMsg);
	}else{
		glb_Log(pcaDumpAddr,lDumpLen,pcaDispMsg);
	}
}

int glb_SetLog_FuncPtr(int addr)
{
	if( addr == (int)PFunc_ErrLog )return 0;
	if( addr == 0 ) PFunc_ErrLog = (PFUNC_LOG) &glb_ErrLog_Default;
		else PFunc_ErrLog = (PFUNC_LOG) addr;
	return 0;
}

int glb_SetDebug_FuncPtr(int addr)
{
	if( addr == (int)PFunc_Debug )return 0;
	if( addr == 0 ) PFunc_Debug = (PFUNC_LOG) &glb_ErrLog_Default;
		else PFunc_Debug = (PFUNC_LOG) addr;
	return 0;
}

char* rtrim(char* str)
{    
    char *p=str+strlen(str)-1;
    while( p >= str && isspace(*p) ) p--;
    *(p+1)=0;
    return str;
}

char* rtrim_null(char* str, char nullchar)
{
	char *p=str+strlen(str)-1;
    while( p >= str && isspace(*p) ) p--;
    if(p==str-1){
    	*++p=nullchar;
    }
    *(p+1)='\0';    
    return str;
}

int glb_Adjust_Decimal(char *output, char *decimal, int len)
{
    char format[10], buff[50],caMsg[1024],caTmp[100];
    int i,slen;
    strcpy(buff, decimal);
    rtrim(buff);
    memset(output,'0',len);
    output[len]=0;
    slen=strlen(buff);
    for(i=slen; i>=0&&(buff[i]<0x30||buff[i]>0x39); i--);
    for(; i>=0&&((buff[i]>=0x30&&buff[i]<=0x39)||buff[i]=='.')&&len>0; i--)
    {
       if(buff[i]!='.') output[--len] = buff[i];
    }
    return 0;
}

int glb_min_strlen(char *src, int len)
{
    int l=0;
    while ( (l<len) && *(src+l)!=0 ) l++;
    return l;
}


int glb_mktime(time_t *time, char *ymd)
{
	int i;
	char buff[8];
  struct tm t;
  
  strcpy_safe(buff, ymd, 4);
  t.tm_year  =   atoi(buff);
  
  strcpy_safe(buff, ymd+4, 2);
  t.tm_mon   =   atoi(buff);   
  
  strcpy_safe(buff, ymd+6, 2);
  t.tm_mday   =   atoi(buff);
  
  strcpy_safe(buff, ymd+8, 2);
  t.tm_hour   =   atoi(buff);
  
  strcpy_safe(buff, ymd+10, 2);
  t.tm_min   =   atoi(buff);
  
  strcpy_safe(buff, ymd+12, 2);
  t.tm_sec   =   atoi(buff);
  
  *time   =   mktime(   &t   );   
  
  return 0;
}

