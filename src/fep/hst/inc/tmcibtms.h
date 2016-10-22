#ifndef IBTMS_H
#define IBTMS_H

#define ISDBANK         "isdbank.conf"  // ��Ա�����ƴ����Ӧ��
#define ISO_DEFINE      "isdisodf.conf"

#define BCDA_SHM_NAME       "BCDA"      // ͨѶ���Ʊ������
#define MAX_BANK_NUM        30

#define TMS_AFTERTOUT_FOLD_NAME   "AfterTmHandler"
#define  TMS_AUTOSAFH_FOLD_NAME   "AutoSAF"
#define  TMS_SYSTEMMONI_FOLD_NAME   "SysMoni"

/*  ��ԱͨѶ���Ʊ�־���� */ 
#define TMS_APCONN_ON       1   // Ӧ�����Ӵ�     
#define TMS_APCONN_OFF      0   // Ӧ�����ӹر�     
#define TMS_SAFLAG_ON       1   // �洢ת����־�� 
#define TMS_SAFLAG_OFF      0   // �洢ת����־�ر� 
#define TMS_DATESW_ON       1   // �����л���־�� 
#define TMS_DATESW_OFF      0   // �����л���־�ر� 
#define TMS_RESETKEY_ON     1   // ������Կ��־�� 
#define TMS_RESETKEY_OFF    0   // ������Կ��־�ر� 

/*  ���׳�ʱ��־���� */ 
#define TMS_TIMEOUT_ON      1   // ���׳�ʱ��־�� 
#define TMS_TIMEOUT_OFF     0   // ���׳�ʱ��־�ر� 

/*  ���׳�ʱ��־���� */ 
#define TMS_TIMEOUT_FUN     1   // ��ʱ���ƴ���     
#define TMS_INFOSWITCH_FUN  0   // ��Ϣ�������ܴ��� 




#define TMS_SHUTDOWN_CMD    0   // ϵͳ�ػ�����     
#define TMS_TIMEOUT_CMD     1   // ��ʱ����
#define TMS_DATESWBGN_CMD   2   // ���п�ʼ��������
#define TMS_DATESWEND_CMD   3   // ���н�����������
#define TMS_RSTKEY_CMD      4   // ������Կ����
#define TMS_SNDDTL_CMD      5   // �����ѻ�������ϸ����
#define TMS_TIMERNOTI_CMD   6   // ��ʱ����ʱ֪ͨ
#define TMS_SNDSAF_CMD      7   // ����SAF����
#define TMS_SNDBLKLIST_CMD  8   // ���ͺ����������·�����

/* add by xiezr 2001-06-07
// define TimeoutTriggerReversalFinancialTranscationCmd   TMS_TIMEOUT_FUN
// end add by xiezr 2001-06-07*/

/*added by myl 2000-10-12 forʵʱ����*/
#define TMS_REALTIMESAF_CMD  9   // ʵʱ����

#define TMS_SNDCMP_CMD      10  // ���Ͷ�������
#define TMS_ADJUST_CMD      11   // \267\242\313\315ADJUST\303\374\301\356


#define TMS_POSI	    2000 // pos���Ͱ�
#define TMS_BANK	    2001 // ���б�׼��
#define TMS_ISEI	    2002 // ISE�ڲ����ĸ�ʽ
#define TMS_CIBC	    2003 // �����ı���
#define TMS_SHOP		2004 // �̳�ǰ�ñ���

#define TMS_MSGCLS_INNER    1000 //�ڲ�����������
#define TMS_MSGCLS_8583REQ  1001 //�ⲿ������8583����
#define TMS_MSGCLS_8583RESP 1002 //�ⲿӦ����8583����

#define  TMS_MSGDEST_SWITCH   1000  //��������Ϊ���ĵ�Ŀ�ĵ�
#define  TMS_MSGDEST_ISSUER   1001  //������Ϊ���ĵ�Ŀ�ĵ�
#define  TMS_MSGDEST_ACQUIRER 1002  //������Ϊ���ĵ�Ŀ�ĵ�

/* comunication cortrol table(BCDA) */
struct BCDA
{
  char caBankCode[11];      /* ��Ա�д���*/
  int  iBankSeq;            /* ��Ա�е�˳��ţ������źŷ���*/
  char caBankName[15];       /* ��Ա������*/
  char cNetFlag;            /* ����������Ŀ*/
  char cAppFlag;            /* Ӧ�����ӱ�־*/
  char cSafFlag;            /* �洢ת����־*/
  char cResetKeyFlag;       /* ������Կ��־*/
  char ReadFlag;          // ���ݽ�����·��־
  char SendFlag;          // ���ݷ�����·��־
  int  iDaySwhMaxTries;     /* �ط����п�ʼ���������Դ���*/
  short iUseFlag;               /*ʹ�ñ�־*/
  char caType[8];	    /* The Type of "sh p POSI ICSC"*/
 
};

struct BCDAst
{
  int    iSemId;            /*�źŵƱ�ʶ��,���ڻ������ */
  int    iBankNum;          /* ��Ա����Ŀ*/
  struct BCDA stBcda[1];    /* BCDA���׵�ַ*/
};

#define  BCDA_NETFLAG            1  // ����������Ŀ
#define  BCDA_APPFLAG            2  // Ӧ�����ӱ�־
#define  BCDA_SAFFLAG            3  // �洢ת����־
#define  BCDA_RESETKEYFLAG       4  // ������Կ��־
#define  BCDA_SECCHECK		 5  // ��ȫ����־
#define  BCDA_DATESWHBGNFLAG     6  // �����л���־

/* �ػ������������ݸ�ʽ*/
struct  SHUTDOWNCMD
{
  char caMsgType[4];        /* �ػ������������Ϣ�����룺0000*/
};

/* ��ʱ��֪ͨ���ݸ�ʽ*/
struct TIMERNOTI
{
  char caMsgFlag[4];        /* �����־:"ISEI"    */
  char caMsgType[4];        /* ��ʱ�����������Ϣ�����룺0006   */
  int  iTimerId;
};

/* ��ʱ������������ݸ�ʽ */
struct TIMEOUTCMD {
/*  char caMsgFlag[4];        // �����־:"ISEI"    */
  char caMsgType[4];        /* ��ʱ�����������Ϣ�����룺0001   */
  char caIbTxnSeq[30];      /* ���н�����ˮ��       */
  char caAcqCode[11];       /* �����������:��Ա�л�POS�ն� */
  char caCardCode[11];      /* �����д���           */
  char caTxnDate[8];        /* ��������         */
  char caProcCode[6];       /* ���״������       */    
  int  iOrgFid;             /* ���ͽ��̵��ļ���Id */
  long iOrgMTI;             /* MTI of original message  */
};

/* ���п�ʼ������������ݸ�ʽ   */
struct DSWBGNCMD {
/*  char caMsgFlag[4];  */
  char caMsgType[4];		/* Message Type: 0002 */
  char caProcessCode[6];	/* Processing Code: 000000*/
  char caBankCode[13];  	/* Len+BankCode && Flag+Len+BankCode */
  char caFunctionType[3];     	/* the 70 field function Code*/
  char cRespCode;        
};

/* ���н���������������ݸ�ʽ   */
struct DSWENDCMD {
  char caMsgType[4];        /* ���п�ʼ������Ϣ���ţ�0003*/
};

typedef struct {
 /* char caMsgFlag[4];   */
  char caMsgType[4]; 		/* Message Type 0004 */
  char caProcessCode[6];	/* Processing Code: 000000*/
  char caBankCode[13];		/* Len+BankCode && Flag+Len+BankCode */
  char cKeyType;     		/* Key Type: '0': Mac Key , '1': Pin Key */
  char cRespCode;           
 } RSTKEYCMD;


struct SNDDTLCMD
{
/*  char caMsgFlag[4];        // �����־:"ISEI"    */
  char caMsgType[4];       /* �ѻ�������ϸ����������ţ�0005*/
  char caBankCode[8];	   /* ��Ա�д���*/
  char cRespCode;	       /* ��Ӧ���룺'0'->���ͳɹ�; '1'����ʱ; '2'->��Ӧ��;*/
  			                /* '3'->�������; '4'->ISE internal error*/
};


struct SNDSAFCMD
{
/*  char caMsgFlag[4];        // �����־:"ISEI"    */
  char caMsgType[4];        /* �ѻ�������ϸ����������ţ�0007*/
  char caBankCode[8];	    /* ��Ա�д���*/
  char caSeqNo[6];	        /* ��ˮ��*/
  char caTxnDateTime[10];       /* ���״������*/
  char cRespCode;	        /* ��Ӧ���룺'0'->�Ѿ�����; '1'->��ʱ��û��Ҫ���͵�SAF��¼*/
};
struct ADJUSTTXNCMD
{
/*    char caMsgFlag[4];      // \303\374\301\356\261\352\326\276"ISEI"*/
    char caMsgType[4];      /* \265\367\325\373\275\273\322\327\303\374\301\356\ 307\353\307\363"0009"*/
    char caBankCode[8];     /* \322\370\320\320\264\372\302\353*/
    char caSeqNo[6];        /* \265\367\325\373\301\367\313\256\272\305*/
    char cRespCode;         /* \317\354\323\246\264\372\302\353\243\272'0'->\316\264\267\242\313\315\315\352; '1'->\267\242\313\315\315\352\261\317*/
};


/* ʵʱ���������ISA֪ͨ��tmxautosaf.x*/
struct RTIMESAFCMD {
/*  char caMsgFlag[4];        // �����־:"ISEI"    */
  char caMsgType[4];        /* ���п�ʼ������Ϣ���ţ�0009*/
  char caBankCode[16];      /* ��Ա�д���*/
  char caSeqNo[8];	        /* ��ˮ��*/
  char caProcCode[6];       /* ���״������*/
};

/* �������������ݿ�*/
struct IBMSG
{
  char caTxnDate[8]; /* ��������*/
  char caTxnTime[6];        /* ��Ϣ����ʱ��*/
  char caMsgData[1500];     /* ������Ϣ*/
};

/* ���н�����ˮ���ݿ�*/
struct IBTXNMSG
{
  char caTxnDate[8]; /* ��������*/
  char caCntCode[11];       /* �������Ĵ���*/
  char caCntSeqNo[6];       /* �������Ľ���˳���*/
  char cTxnOkFlag;          /* ���׳ɹ���־��0��ʧ�ܣ�1���ɹ�*/
  char caIbTxnMsgData[1500];/* ���н��ױ���*/
};

/* �洢ת�����ݿ�*/
struct SAF
{
  char caTxnDate[8]; /* ��������*/
  char caCntCode[11];       /* �����Ĵ���*/
  char caCntSeqNo[6];       /* ԭ����˳���*/
  char cSafOkFlag;          /* �洢ת����ɱ�־��0��δ��ɣ�1�����*/
  char caSafData[1500];     /* �洢ת���ı�������*/
};

/* modify by xiezr 2001-06-01*/
#define  TMS_BANK_CODE_LEN  8
/* modify end  by xiezr 2001-06-01*/


/* Add By Xiezr 2001/07/11*/
/* Add Struct of TextSnd*/
typedef struct {
/*	char caMsgFlag[4];      // FLAG  "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT_TEXT_SND_CMD */
	char caProcessCode[6];	/* Processing Code:: PC_TEXT_SND*/
	char caBankCode[13];	/* Bank Members*/
	char caText[400];	/* Sending Text*/
	} SND_MSG;
/* For Test Line Status*/
typedef struct {
/*	char caMsgFlag[4];      // FLAG  "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT_TEST_BANK_CMD */
	char caProcessCode[6];	/* Processing Code:: PC_TEST_BANK*/
	char caBankCode[13];	/* Bank Members*/
				/* Success Flag: && Bank Code*/
	} LINE_STATUS;

/* For Open Bank*/
typedef struct {
/*	char caMsgFlag[4];      // FLAG  "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT_OPEN_BANK_CMD */
	char caProcessCode[6];	/* Processing Code:: PC_OPEN_BANK*/
	char caBankCode[13];	/* Bank Members*/
	} OPEN_BANK;

/* For Close BanK*/
typedef struct {
/*	char caMsgFlag[4];      // FLAG  "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT_CLOSE_BANK_CMD */
	char caProcessCode[6];	/* Processing Code:: PC_CLOSE_BANK*/
	char caBankCode[13];	/* Bank Members*/
	} CLOSE_BANK;

/* For Recocilication */
typedef struct {
/*	char caMsgFlag[4];	// FLAG "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT__RECONCILE_AS_POS_ACQUIRER_CMD */
	char caProcessCode[6];	/* Processing Code:: PC_RECONCILE_AS_POS_ACQUIRER*/
	char caFlag[1];		/* The Result:: '1': OK, '2': NOT OK, '3':Error*/
	char caBankCode[13];	/* Bank Members*/
	char caSettleDate[8];	/* settle date*/
	char caDebitNum[10];	/* Debit Number*/
	char caRevDebitNum[10]; 
	char caCreditNum[10];
	char caRevCreditNum[10];
	char caTransNum[10];
	char caRevTransNum[10];
	char caBalanceNum[10];
	char caAuthorNum[10];
	char caDebitServiceFee[12];
	char caCreditServiceFee[12];
	char caDebitAmount[16];
	char caRevDebitAmount[16];
	char caCreditAmount[16];
	char caRevCreditAmount[16];
	char caPureAmount[17];
} RECONCILE;

/* For Fee Collication*/
typedef struct {
/*	char caMsgFlag[4];      // FLAG  "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT_FEE_COLLICATION_CMD */
	char caProcessCode[6];	/* Processing Code:: PC_FEE_COLLICATION*/
	char caBankCode[13];	/* Bank Members*/
	char caAccount[21];	/* Len(2)+Account*/
	char caAmount[12];	/* Amount*/
	char caFeeCause[4];	/* 60 field*/
	char caCauseTxt[256];	/* Len(3)+253*/
	} FEE_COLLECTION;
	
/* for Key Reset Apply Command*/
typedef struct {
/*	char caMsgFlag[4];      // FLAG  "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT_KEYRESET_APPLY_CMD, */
	char caProcessCode[6];	/* Processing Code:: PC_KEYRESET_APPLY*/
	char caBankCode[13];	/* Bank Members*/
	char caKeyType[1];	/* 1: pin 2: MAC*/
	} KEYRESETAPPLY;


/* for System Date Switch Begin*/
typedef struct {
	char caMsgType[4];	/* Message Type ::MT_SYS_DSWBGN_CMD*/
	char caProcessCode[6];	/* Processing Code:: PC_SYS_DSWBGN*/
	char caSettledate[8];
	char caFlag;	/* '0':normal, '1':must ;resp '0',succ, '1':fail*/
	} SYS_DSWBGN;

/* for system date switch end*/
typedef struct {
/*	char caMsgFlag[4];      // FLAG  "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT_SYS_DSWEND_CMD*/
	char caProcessCode[6];	/* Processing Code:: PC_SYS_DSWEND*/
	char caRetCode[1];	/* Return Code, '0', succ, '1': fail*/
	} SYS_DSWEND;

typedef struct {
/*	char caMsgFlag[4];      // FLAG  "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT_CTRLERR_MSG_CMD*/
	char caProcessCode[6];	/* Processing Code:: PC_CTRLERR_MSG*/
	char caRetCode[1];	/* Return Code, '0', succ, '1': fail*/
	char caRequest[2];	/* Type:*/
				/* "01" :  Issue Bank Credit*/
				/* "02" :  Issue Bank Debit*/
				/* "03" :  Acquir Bank Credit*/
				/* "04" :  Acquir Bank Debit*/
	char caPointOfService[2]; /* 25 field, Point of services*/
				/* "00" :  Nomal ("00")*/
				/* "13" :  again Request ("13")*/
				/* "17" :  back sheet ("17")*/
				/* "41" :  back sheet again("41");*/

	char caCardNumber[21];	/* Card Number, LL+CardNumber(<19)*/
	char caAmount[12];	/* Trans Amount  // Not Used here*/
	char caMarchineType[4];	/* MarchineType:: (18 field)*/
	char caMerchCode[15];	/* MerchCode*/
	char caTermNumber[8];	/* Terminal Number*/
	char caSettleDate[8];	/* SettleDate  // Not Used*/
	char caAdjAmount[12];	/* Adjust Amount( field 4)*/
	char caOrg_MsgType[4];  /* Trans Type ( for 90 field)*/
	char caOrg_TraceNum[6]; /* Trace Number( for 90 field)*/
	char caOrg_TraceTime[10]; /* Trans Time ( for 90 field)*/
	char caOrg_AcquirCode[13];/* Acquiring Bank Code (for 90 field)*/
	char caOrg_FwdCode[13];   /* forwarding institute  Code (for 90 field)*/
	char caAcquiringBank[13]; /* Acquiring Bank Code ( 32 field) // not used*/
	char caIssueBankCode[13]; /* Receiveing  Bank Code ( 100field)*/
	char caMessageCode[4];	/* Message Cause Code (60 field)*/
	} ADJUST_ACCOUNT;

/* for Settlement manager control*/
typedef struct {
/*	char caMsgFlag[4];      // FLAG  "ISEI"*/
	char caMsgType[4];	/* Message Type ::MT_TEXT_SND_CMD */
	char caProcessCode[6];	/* Processing Code:: PC_TEXT_SND*/
	char caBankCode[13];	/* Bank Members*/
	char caSettledate[8];
	char caSettledata[300];	/* SettleData*/
	} SETTLEDATA;

#endif
