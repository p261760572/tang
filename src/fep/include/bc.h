#ifndef _BC_H_
#define _BC_H_

#include "../db/inc/glbdb.h"
#include "../db/inc/ti_bc_trans_det.h"
#include "../db/inc/ti_bc_trans_inf.h"
#include "../db/inc/ti_bc_trans_chnl_inf.h"
#include "../db/inc/ti_bc_term_inf.h"
#include "../db/inc/ti_bc_mchnt_inf.h"
#include "../db/inc/ti_bc_qs_mchnt_inf.h"
#include "../db/inc/ti_bc_mchnt_term_charge_inf.h"
#include "../db/inc/ti_bc_mt_flow_id.h"
#include "../db/inc/ti_bc_mchnt_det.h"
#include "../db/inc/tl_bc_trans_log.h"
#include "../db/inc/tl_bc_bill_log.h"
#include "../db/inc/ti_bc_ins_inf.h"
#include "../db/inc/ti_bc_card_bin.h"
#include "../db/inc/ti_bc_app_fork_inf.h"
#include "../db/inc/tl_bc_exp_log.h"
#include "../db/inc/ti_bc_mid_day_limit_tims.h"
#include "../db/inc/ti_bc_white_list.h"
#include "../db/inc/ti_bc_black_list.h"
#include "../db/inc/ti_bc_route_card_bin.h"
#include "../db/inc/ti_bc_route_gateway_inf.h"
#include "../db/inc/ti_bc_ivr_inf_key_inf.h"
#include "../db/inc/ti_bc_ivr_opr_impdata_inf.h"
#include "../db/inc/ti_bc_ivr_opr_cmd_inf.h"
#include "../db/inc/ti_bc_ivr_list_inf.h"
#include "../db/inc/tl_bc_cardno_list_inf.h"
#include "../db/inc/ti_bc_error_inf.h"
#include "../include/dbstruct.h"
#include "../include/ivr.h"


/****************************
			     ��������
*****************************/

#define ELSE else
#define ON_BC_FUNC(funcstring,func) else if( 0 == strcmp(procedure,funcstring) ) iRet = func(&g_tiBcTransInf,&g_tiBcTransDet,iso,g_TransLock);
#define ON_BC_TIMEOUT(funcstring,func) else if( 0 == strcmp(procedure,funcstring)) iRet = func();
//#define ON_BC_FUNC_TMOUT(funcstring,func) else if(memcmp(procedure,funcstring,5)==0) iRet = func(&g_tiBcTransInf,&g_tiBcTransDet,pTimeOutRec,g_TransLock);
#define BC_TRACE_END(X) return X;
typedef int (*PF_IFunc_GenMac)(char* mk_index, char* mk_key,char *data, int datalength, char* macout);
#define I_TIMEOUT 30
#define I_TIMEOUT_BILL 300

#define I_TIMEOUT_TIMES 19
#define SEND_SMS_TIMEOUT_TIMES 3
#define INS_HFTX "00843300"
#define INS_YBZF "00842200"
#define INS_IVR  "09005500"
#define INS_GDYL "00944400"
#define INS_AJ   "00945500"
#define INS_YL   "01045500"
#define INS_CENT "00005500"
#define MIN(a,b) a>b?b:a

//���ݲ���
//�̻���ţ�001431701088031
//ip��220.168.94.226
//�˿ڣ�9383
//��Կ��4B40A73D0AE43E7A

//������ַ��222.247.38.108
//������ַ��172.16.129.109
//���ݶ˿ڣ�5689
//zdxt 8000

//�ն�����   0 3
//��������   0 0
//���շ�Ӧ�� 1 2��
//����Ӧ��   1 0

#define RESP_CODE_CUS001 "N000G0"  //�̻��Ѵﵱ�ս��ױ�������
#define RESP_CODE_CUS002 "N000G1"  //�̻��Ѵﵱ�ս��׽������
#define RESP_CODE_CUS003 "N000G2" //�̻��Ѵﵱ�½��ױ�������
#define RESP_CODE_CUS004 "N000G3" //�̻��Ѵﵱ�½��׽������
#define RESP_CODE_CUS005 "N01001" //��bin�ڽ����̻���������
#define RESP_CODE_CUS006 "N00022" //ɾ���˵���ʱ��ʧ��
#define RESP_CODE_CUS007 "N00022" //ɾ�����׳�ʱ��ʧ��
#define RESP_CODE_CUS008 "N00062" //δ�ڿ�bin�����ҵ��ÿ���bin��Ϣ
#define RESP_CODE_CUS009 "N00040" //�̻���֧�����ÿ�����
#define RESP_CODE_CUS010 "N00031" //δ�ҵ�·�ɻ���
#define RESP_CODE_CUS011 "N00031" //��bin�Ѿ�������
#define RESP_CODE_CUS012 "N000G4" //�������̻����ʽ������
#define RESP_CODE_CUS013 "N000G5" //�������̻������ܽ������
#define RESP_CODE_CUS014 "N000G6" //���ų����ܱ�������
#define RESP_CODE_CUS015 "N00096" //��ѯ���ݿⷢ���쳣
#define RESP_CODE_CUS016 "N00091" //��������û���ҵ�
#define RESP_CODE_CUS017 "G000G7" //�̻�ת����Ϣû���ҵ�
#define RESP_CODE_CUS018 "G000G8" //��ȡivr����Ӧ����Ӧ����ʧ��
#define RESP_CODE_CUS019 "G000G9" //��ȡivr����Ӧ����������Ϣʧ��
#define RESP_CODE_CUS020 "G000GA" //�����Ѿ��ɹ��������ظ�����
#define RESP_CODE_CUS021 "G000GB" //δ���ҵ�������Ϣ
#define RESP_CODE_CUS022 "G000GC" //����ivr VALIDDATAʧ��
#define RESP_CODE_CUS023 "G000GD" //����ivr cvn2ʧ��
#define RESP_CODE_CUS024 "G000GE" //���ܵ�#48��ʧ��

#define EXP_CODE_CUS001 "E001" //δ��ti_bc_trans_inf�������øý�����Ϣ

/****************************
			     �ṹ�嶨��
*****************************/

//��ʱ��ṹ
typedef struct{
    char sys_tra_no[6];
    char term_id[8];
    char mchnt_cd[20];
    char end;
}TimeOut_Key;

//��ʱ��ṹ
typedef struct{
    char loc_trans_dt[4];
    char msg_tp[4];
    char proc_cd[4];
    char sys_tra_no[6];
    char term_id[8];
    char mchnt_cd[20];
    char end;
}TimeOut_ReMark;

typedef struct{
    int srcFoldid;
    int OutFoldType; // 0:src; 1:out;
    char OutFoldName[16];
    char packet_type[4];
    char data[4096];
    int datalen;
}Folder_Msg;

typedef struct{
    tl_bc_trans_log_def BcTransLog;
    int len_48;
}BcTransMsg;

typedef struct{
    int type ;
    char mk_index[ 3+1];
	char mk_key  [16+1];
	PF_IFunc_GenMac PFUNC_GEN_KEY;
}BcMacMsg;

//�����Ϣ�ṹ
typedef struct{
    char baglen [4];
    char bagtype[3];
    //char level  [2];
    //char datetime[10];
    char data[4096];
}MONI_STRUCT;

/****************************
			     ȫ�ֱ���
*****************************/

extern int g_IVR_FOLDER_ID;
extern int gs_myFid ;
extern Folder_Msg g_folder_msg;
extern int g_iSrcFoldId;

extern BcTransMsg g_TransMsg;
extern BcTransMsg g_TransMsg_o;

extern BcMacMsg g_MacMsg;  
extern char   *g_pcBcdaShmPtr;
int g_TransLock[2];

extern struct ISO_8583 iso8583_ZDXTconf[128];
extern struct ISO_8583 iso8583_AJXTconf[128];
extern struct ISO_8583 iso8583_GDYLconf[128];
extern struct ISO_8583 iso8583_JAVAconf[128];
extern struct ISO_8583 iso8583_CUPSconf[128];

extern ti_bc_ivr_inf_key_inf_def g_tiBcIvrInfKeyInf;
extern ti_bc_ivr_opr_cmd_inf_def g_tiBcIvrOprCmdInf;
extern ti_bc_ivr_opr_impdata_inf_def g_tiBcIvrImpdataInf;

extern ti_bc_trans_inf_def g_tiBcTransInf;
extern ti_bc_trans_det_def g_tiBcTransDet;
extern ti_bc_term_inf_def g_tiBcTermInf;
extern ti_bc_mchnt_det_def g_tiBcMchntDet;
extern ti_bc_mchnt_inf_def g_tiBcMchntInf;
extern ti_bc_ins_inf_def g_tiBcInsInf;
extern tl_bc_bill_log_def g_tlBcBillLog;
extern ti_bc_qs_mchnt_inf_def g_tiBcQsMchntInf;

//�̻�ת����Ϣ��
extern ti_bc_mchnt_term_charge_inf_def g_tiBcMchntTermChargeInf; 
extern ti_bc_route_card_bin_def g_tiBcRouteCardBinList ;
extern ti_bc_white_list_def g_tiBcWhiteList ;
extern ti_bc_black_list_def g_tiBcBlackList ;

extern ti_bc_mid_day_limit_tims_def g_tiBcMidDayLimitTims;
extern ti_bc_card_bin_def g_tiBcCardBin;

extern char g_center_ins_id_cd[8+1] ;
extern char g_rtIns_id_cd[9] ;

extern char g_Rcv_Buffer[4096] ;
extern int g_RcvBufferSize ;
extern int g_FromFolderId ;
extern struct TimeOut_REC g_TimeOutRec;
extern MONI_STRUCT g_Moni_Data;

extern char g_iSrcFoldName[30+1] ;
extern char g_srcPacketType[5];
extern int g_ForkFlag;
extern char *aj_mchnt_cd;

/****************************
			     ��������
*****************************/

void PrintISO( ISO_data *iso,char *caRemark,int flag);
int TermMsgUnpack( char * srcBuf, ISO_data *iso, int iLen);
int BC_UnPack( ISO_data *iso, char *srcBuf,  int iLen);
int BC_UnPack_Data( ISO_data *iso, char *srcBuf,  int iLen);
int BC_SetIsoMode2(ISO_data *iso, char *mode);
int BC_SetIsoMode(ISO_data *iso, char *trans_chnl);
int BC_GetPacket_Type(char *packet_type, char *trans_chnl);
void BC_StringAdder(char *value, int addvalue, char* outbuff,int outlen);
int BC_StringtoUpper(char *value, int len);
int IsReady(char *pcaName);
int BC_GetFolderIdByName(char *pcaBankName);
int LocateNameByFdid(int iFld,char *caBankName);
int BC_SendMessageToFolder_direct( char *data, int len);
int BC_SendMessageToFolder( char *resp );
int BC_SendMessageToTerm(ISO_data *iso);
int BC_SendMessage(ISO_data *iso,char *trans_chnl);
int BC_SendMessageToSrc(ISO_data *iso);
int BC_SendMessageToSrc2(ISO_data *iso,char *trans_chnl);
int BC_CheckIsoDataField(ISO_data *p_isoData, char *field);
int BC_CopyISOdata( ISO_data *p_isoData, BcTransMsg *p_BcTransMsg);
int BC_InsertIntoDB(tl_bc_trans_log_def *p_BcTransLog,int *SqlCode);
int BC_TermCheckMac(char *key_index,char *key_data, char *Data,int length);
int BC_GenMac(char *key_index,char *key_data,char *macData,int length,char *mac_out);
int BC_TermGenMac(char *key_index,char *key_data,char *macData,int length,char *mac_out);
int BC_SetMacKey(char *key_index,char *key_data);
int BC_SetRcvMacKey(ti_bc_ins_inf_def *p_tiBcInsInf);
int BC_SetTermMacKey(char *key_index,char *key_data);
int BC_SetTermMacKey2(char *term_id);
int BC_SetOPMacKey(char *key_index,char *key_data);
int BC_ErrLog_Func(char *pcaDumpAddr,long lDumpLen, char *pcaDispMsg);
int BC_Debug_Func(char *pcaDumpAddr,long lDumpLen, char *pcaDispMsg);
int BC_Search_mchnt_inf(ti_bc_mchnt_inf_def *p_tiBcMchntInf, tl_bc_trans_log_def *p_BcTransLog);
int BC_InsertTimeOut1(int iflags, int iRcvFolderId,int iOrgFolderId,int iMaxTimeout,ISO_data *iso, BcTransMsg *p_TransMsg);
int BC_DeleteTimeOut1(struct TimeOut_REC *p_rec);
int BC_InsertBillNotify(int iRcvFolderId,int iOrgFolderId,int iMaxTimeout,ISO_data *iso, BcTransMsg *p_TransMsg);
int BC_DeleteBillNotify(struct TimeOut_REC *p_rec);
int BC_SendExeception(char *level, char *log, ...);
int min_strlen(char *src, int len);
int min_strstr(char *str1, char *str2);
int BC_SendMonitor(char *transflag,char *sendflag,char *trans_chnl,ISO_data *iso);
int BC_SendMonitor2(char *transflag,char *sendflag,tl_bc_trans_log_def *p_BcTransLog);

int BC_SendMonitor_trans(char *transflag,char *sendflag,tl_bc_trans_log_def *p_BcTransLog, ti_bc_trans_inf_def *p_tiBcTransInf);
int BC_SendMonitor_iso(char *transflag,char *sendflag,char * trans_chnl,ISO_data *iso, ti_bc_trans_inf_def *p_tiBcTransInf);

int TPSendToAJ(char *pSendData,int iMsgLen,char *ip,int port,int timeout,char *pRecvData);
int BC_TPSendToAJ(char *pSendData,int iMsgLen,char *pRecvData,ti_bc_ins_inf_def *p_tiBcInsInf);
int BC_SendMessageToAJ(ISO_data *iso, char *pRecvData,ti_bc_ins_inf_def *p_tiBcInsInf);
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen);
int UnicodeToGBK(char *inbuf,int inlen,char *outbuf,int outlen);
int GBKToUnicode(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
int UnicodeToGBK_2(char *inbuf,int inlen,char *outbuf,int outlen);
int GBKToUnicode_2(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
int BC_PacketTimeOut(struct TimeOut_REC *p_TimeOutRec, ISO_data *iso, int);

void memcpy_safe(char *des, char *src, int len);
void strcpy_safe(char *des, char *src, int len);
void tiIvrSignIn();

int BC_Find_Route_Ins(char *resp_cd, char *ins_id_cd, ti_bc_mchnt_inf_def *p_tiBcMchntInf, ti_bc_card_bin_def *p_tiBcCardBin, char *pri_acct_no);
int BC_Search_ins_inf(char *resp_cd, ti_bc_ins_inf_def *p_bc_ins_inf, char *ins_id_cd);
int BC_exp_log(char *rsn_code, char *rsn_desc, char *binarydata, int datasize);
int BC_UnLockTrans(tl_bc_trans_log_def *p_BcTransLog);

int POSP_ExpendApp(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int POSP_ExpendUndo(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int POSP_BalanceQuery(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int POSP_ExpendRollBack(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int POSP_ExpendUndoRollBack(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int POSP_KeysResetApp(ti_bc_trans_inf_def *p_tiBcTransInf, ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int POSP_KeysResetReply(ti_bc_trans_inf_def *p_tiBcTransInf, ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);

int POSP_RespondExpendApp(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf); //�ظ�POSP��������
int POSP_RespondExpendUndo(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf); //�ظ�POSP���ѳ���
int POSP_RespondBalanceQuery(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf); //�ظ�POSP����ѯ
int POSP_RespondExpendRollBack(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf); //�ظ�POSP���ѳ���
int POSP_RespondExpendUndoRollBack(char *resp_code, ISO_data *p_iso, ti_bc_ins_inf_def *p_tiBcInsInf); //�ظ�POSP���ѳ���

int CUPS_ExpendAppReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int CUPS_ExpendUndoReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int CUPS_BalanceQueryReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int CUPS_ExpendRollBackReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);
int CUPS_ExpendUndoRoolBackReply(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag);

int CUPS_ExpendApp(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf);
int CUPS_ExpendUndo(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf);
int CUPS_BalanceQuery(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf);
int CUPS_ExpendRollBack(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf);
int CUPS_ExpendUndoRollBack(char *resp_code, char *errInfo, ti_bc_ins_inf_def *p_tiBcInsInf);

void chkCardnoList();

#endif



