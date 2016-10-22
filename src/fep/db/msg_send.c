
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
           char  filnam[12];
};
static struct sqlcxp sqlfpn =
{
    11,
    "msg_send.pc"
};


static unsigned int sqlctx = 161403;


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
            void  *sqhstv[7];
   unsigned int   sqhstl[7];
            int   sqhsts[7];
            void  *sqindv[7];
            int   sqinds[7];
   unsigned int   sqharm[7];
   unsigned int   *sqharc[7];
   unsigned short  sqadto[7];
   unsigned short  sqtdso[7];
} sqlstm = {12,7};

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
5,0,0,1,124,0,3,61,0,0,7,7,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1,97,0,0,1,97,0,0,
48,0,0,2,119,0,4,87,0,0,5,4,0,1,0,2,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
83,0,0,3,0,0,29,117,0,0,0,0,0,1,0,
98,0,0,4,80,0,5,127,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
129,0,0,5,0,0,29,141,0,0,0,0,0,1,0,
144,0,0,6,124,0,5,155,0,0,6,6,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,
0,1,97,0,0,
183,0,0,7,0,0,29,171,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <stdlib.h>
#include <sqlda.h>
#include <sqlcpr.h>
#include <oraca.h>
#include "./inc/glbdb.h"
#include "./inc/msg_send.h"

/* EXEC ORACLE OPTION (ORACA=YES); */ 


/*
 *  Function:  msgSendInfOpr
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

int MsgSendOpr(int oprType, msg_send_def *p_msgSendInf, int *p_sqlCode)
{
    /* EXEC SQL WHENEVER SQLERROR continue; */ 
 /*DO sql_error("Oracle error:");*/

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char	msg_id		[ 38 + 1 ];
    char 	phone 		[ 16 + 1 ];
    char	mchnt_cd	[ 20 + 1 ];
    char	loc_trans_dt[  8 + 1 ];
    char	sys_tra_no	[  6 + 1 ];	
    msg_send_def 	msgSendInf;
    /* EXEC SQL END DECLARE SECTION; */ 


    MEMSET_0(msg_id)

    switch ( oprType )
    {
	    case GLB_DB_INSERT1:
	    	memcpy(&msgSendInf, p_msgSendInf, sizeof(msg_send_def));
			ADJUST_STRING(msgSendInf.mchnt_cd    	)
			ADJUST_STRING(msgSendInf.loc_trans_dt	)
			ADJUST_STRING(msgSendInf.sys_tra_no		)
			ADJUST_STRING(msgSendInf.phone    		)
			ADJUST_STRING(msgSendInf.msg_txt		)
			ADJUST_STRING(msgSendInf.send_time		)
			memset(msg_id, ' ', sizeof(msg_id));
			msg_id[sizeof(msg_id)-1] = 0;
			memcpy(msg_id, p_msgSendInf->mchnt_cd, strlen(p_msgSendInf->mchnt_cd));
			memcpy(msg_id+16, p_msgSendInf->phone, strlen(p_msgSendInf->phone));
			/* EXEC SQL INSERT INTO MSG_SEND(MCHNT_CD, LOC_TRANS_DT, SYS_TRA_NO, MSG_ID, PHONE, MSG_TEXT, SEND_TIME) VALUES
			(
				:msgSendInf.mchnt_cd	,
				:msgSendInf.loc_trans_dt,
				:msgSendInf.sys_tra_no	,
				:msg_id   				,
				:msgSendInf.phone		,
				:msgSendInf.msg_txt		,
				:msgSendInf.send_time
			); */ 

{
   struct sqlexd sqlstm;
   sqlorat((void **)0, &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 7;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "insert into MSG_SEND (MCHNT_CD,LOC_TRANS_DT,SYS_TRA_NO,MS\
G_ID,PHONE,MSG_TEXT,SEND_TIME) values (:b0,:b1,:b2,:b3,:b4,:b5,:b6)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)(msgSendInf.mchnt_cd);
   sqlstm.sqhstl[0] = (unsigned int  )21;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)(msgSendInf.loc_trans_dt);
   sqlstm.sqhstl[1] = (unsigned int  )9;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)(msgSendInf.sys_tra_no);
   sqlstm.sqhstl[2] = (unsigned int  )7;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         void  *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned int  )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (         void  *)msg_id;
   sqlstm.sqhstl[3] = (unsigned int  )39;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         void  *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned int  )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (         void  *)(msgSendInf.phone);
   sqlstm.sqhstl[4] = (unsigned int  )17;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         void  *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned int  )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (         void  *)(msgSendInf.msg_txt);
   sqlstm.sqhstl[5] = (unsigned int  )513;
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         void  *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned int  )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (         void  *)(msgSendInf.send_time);
   sqlstm.sqhstl[6] = (unsigned int  )15;
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


        	*p_sqlCode = sqlca.sqlcode;
        	if (sqlca.sqlcode ==0)
        	{
        	
        	}
			if (  memcmp(sqlca.sqlerrm.sqlerrmc, "ORA-00001", 9 ) == 0  )
			{
	            *p_sqlCode = SQL_ERD_UNIKEY;
	        	strcpy_safe(mchnt_cd, p_msgSendInf->mchnt_cd, sizeof(mchnt_cd)-1);
	        	rtrim_null(mchnt_cd, ' ') ;
	        	strcpy_safe(loc_trans_dt, p_msgSendInf->loc_trans_dt, sizeof(loc_trans_dt)-1);
	        	rtrim_null(loc_trans_dt, ' ') ;
	        	strcpy_safe(sys_tra_no, p_msgSendInf->sys_tra_no, sizeof(sys_tra_no)-1);
	        	rtrim_null(sys_tra_no, ' ') ;
	        	strcpy_safe(phone, p_msgSendInf->phone, sizeof(phone)-1);
	        	rtrim_null(phone, ' ') ;
	            /* EXEC SQL SELECT SEND_FLAG
	            INTO :msgSendInf.send_flag
	            FROM MSG_SEND
				WHERE MCHNT_CD		=	:mchnt_cd
					AND LOC_TRANS_DT=	:loc_trans_dt
					AND	SYS_TRA_NO	=	:sys_tra_no
					AND PHONE		=	:phone; */ 

{
             struct sqlexd sqlstm;
             sqlorat((void **)0, &sqlctx, &oraca);
             sqlstm.sqlvsn = 12;
             sqlstm.arrsiz = 7;
             sqlstm.sqladtp = &sqladt;
             sqlstm.sqltdsp = &sqltds;
             sqlstm.stmt = "select SEND_FLAG into :b0  from MSG_SEND where \
(((MCHNT_CD=:b1 and LOC_TRANS_DT=:b2) and SYS_TRA_NO=:b3) and PHONE=:b4)";
             sqlstm.iters = (unsigned int  )1;
             sqlstm.offset = (unsigned int  )48;
             sqlstm.selerr = (unsigned short)1;
             sqlstm.cud = sqlcud0;
             sqlstm.sqlest = (unsigned char  *)&sqlca;
             sqlstm.sqlety = (unsigned short)4352;
             sqlstm.occurs = (unsigned int  )0;
             sqlstm.sqhstv[0] = (         void  *)(msgSendInf.send_flag);
             sqlstm.sqhstl[0] = (unsigned int  )2;
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
             sqlstm.sqhstv[2] = (         void  *)loc_trans_dt;
             sqlstm.sqhstl[2] = (unsigned int  )9;
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
             sqlstm.sqhstv[4] = (         void  *)phone;
             sqlstm.sqhstl[4] = (unsigned int  )17;
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


				if(sqlca.sqlcode == 0)
				{
					memcpy(p_msgSendInf->send_flag, msgSendInf.send_flag, sizeof(msgSendInf.send_flag));
				}
				else
				{
					*p_sqlCode = sqlca.sqlcode;
		            glb_Log(0,0,"<MSG_SEND.pc:MsgSendOpr:INSERT1-SELECT>Insert Record Failure. "
		            			"(MCHNT_CD:%s,LOC_TRANS_DT:%s,SYS_TRA_NO:%s,PHONE:%s,MSG_TEXT:%s,SEND_TIME:%s) Error.\n"
		            			"[sqlCode:%d]errmsg:\n----------\n%s\n", 
		            			msgSendInf.mchnt_cd, msgSendInf.loc_trans_dt, msgSendInf.sys_tra_no, msgSendInf.phone, 
		            			msgSendInf.msg_txt, msgSendInf.send_time, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
					
				}
           	}
			else if(sqlca.sqlcode != 0)
	        {
	            glb_Log(0,0,"<MSG_SEND.pc:MsgSendOpr:INSERT1>Insert Record Failure. "
	            			"(MCHNT_CD:%s,LOC_TRANS_DT:%s,SYS_TRA_NO:%s,PHONE:%s,MSG_TEXT:%s,SEND_TIME:%s) Error.\n"
	            			"[sqlCode:%d]errmsg:\n----------\n%s\n", 
	            			msgSendInf.mchnt_cd, msgSendInf.loc_trans_dt, msgSendInf.sys_tra_no, msgSendInf.phone, 
	            			msgSendInf.msg_txt, msgSendInf.send_time, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
	        }
	        /* EXEC SQL COMMIT WORK ; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 7;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )83;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	        break;
	    case GLB_DB_UPDATE1:
	    	memcpy(&msgSendInf, p_msgSendInf, sizeof(msg_send_def));
         	strcpy_safe(msg_id, p_msgSendInf->msg_id, sizeof(msg_id)-1);
        	rtrim_null(msg_id, ' ') ;
	       	strcpy_safe(phone, p_msgSendInf->phone, sizeof(phone)-1);
        	rtrim_null(phone, ' ') ;
			ADJUST_STRING(msgSendInf.rcv_time )
			ADJUST_STRING(msgSendInf.send_flag)
	        /* EXEC SQL UPDATE MSG_SEND 
	        SET	RCV_TIME	=	:msgSendInf.rcv_time ,
				SEND_FLAG	=	:msgSendInf.send_flag 
			WHERE MSG_ID	=	:msg_id
				AND PHONE	=	:phone; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 7;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "update MSG_SEND  set RCV_TIME=:b0,SEND_FLAG=:b1 whe\
re (MSG_ID=:b2 and PHONE=:b3)";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )98;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)(msgSendInf.rcv_time);
         sqlstm.sqhstl[0] = (unsigned int  )15;
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)(msgSendInf.send_flag);
         sqlstm.sqhstl[1] = (unsigned int  )2;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)msg_id;
         sqlstm.sqhstl[2] = (unsigned int  )39;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         void  *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned int  )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (         void  *)phone;
         sqlstm.sqhstl[3] = (unsigned int  )17;
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
			if(sqlca.sqlcode != 0)
	        {
	            glb_Log(0,0,"<MSG_SEND.pc:MsgSendOpr:UPDATE1>UPDATE Record Failure. "
	            			"(MSG_ID:%s,PHONE:%s,RCV_TIME:%s,SEND_FLAG:%s) Error.\n"
	            			"[sqlCode:%d]errmsg:\n----------\n%s\n", 
	            			msg_id, phone, msgSendInf.rcv_time, msgSendInf.send_flag,
	            			sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
	        }
	        /* EXEC SQL COMMIT WORK ; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 7;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )129;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	        break;    
	    case GLB_DB_UPDATE2:
	    	memcpy(&msgSendInf, p_msgSendInf, sizeof(msg_send_def));
        	strcpy_safe(mchnt_cd, p_msgSendInf->mchnt_cd, sizeof(mchnt_cd)-1);
        	rtrim_null(mchnt_cd, ' ') ;
        	strcpy_safe(loc_trans_dt, p_msgSendInf->loc_trans_dt, sizeof(loc_trans_dt)-1);
        	rtrim_null(loc_trans_dt, ' ') ;
        	strcpy_safe(sys_tra_no, p_msgSendInf->sys_tra_no, sizeof(sys_tra_no)-1);
        	rtrim_null(sys_tra_no, ' ') ;
        	strcpy_safe(phone, p_msgSendInf->phone, sizeof(phone)-1);
        	rtrim_null(phone, ' ') ;
			ADJUST_STRING(msgSendInf.msg_id)
			ADJUST_STRING(msgSendInf.send_flag)
	        /* EXEC SQL UPDATE MSG_SEND 
	        SET	MSG_ID		= 	:msgSendInf.msg_id,
				SEND_FLAG	=	:msgSendInf.send_flag 
			WHERE MCHNT_CD		=	:mchnt_cd
				AND LOC_TRANS_DT=	:loc_trans_dt
				AND	SYS_TRA_NO	=	:sys_tra_no
				AND PHONE		=	:phone; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 7;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "update MSG_SEND  set MSG_ID=:b0,SEND_FLAG=:b1 where\
 (((MCHNT_CD=:b2 and LOC_TRANS_DT=:b3) and SYS_TRA_NO=:b4) and PHONE=:b5)";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )144;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)(msgSendInf.msg_id);
         sqlstm.sqhstl[0] = (unsigned int  )39;
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)(msgSendInf.send_flag);
         sqlstm.sqhstl[1] = (unsigned int  )2;
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
         sqlstm.sqhstv[3] = (         void  *)loc_trans_dt;
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
         sqlstm.sqhstv[5] = (         void  *)phone;
         sqlstm.sqhstl[5] = (unsigned int  )17;
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


	        *p_sqlCode = sqlca.sqlcode;
			if(sqlca.sqlcode != 0)
	        {
	            glb_Log(0,0,"<MSG_SEND.pc:MsgSendOpr:UPDATE2>UPDATE Record Failure. "
	            			"(MCHNT_CD:%s,LOC_TRANS_DT:%s,SYS_TRA_NO:%s,PHONE:%s,MSG_ID:%s,SEND_FLAG:%s) Error.\n"
	            			"[sqlCode:%d]errmsg:\n----------\n%s\n", 
	            			mchnt_cd, loc_trans_dt, sys_tra_no, phone, msgSendInf.msg_id, msgSendInf.send_flag, 
	            			sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
	        }
	        /* EXEC SQL COMMIT WORK ; */ 

{
         struct sqlexd sqlstm;
         sqlorat((void **)0, &sqlctx, &oraca);
         sqlstm.sqlvsn = 12;
         sqlstm.arrsiz = 7;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )183;
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


