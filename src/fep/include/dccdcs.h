#ifndef DCCDCS_H
#define DCCDCS_H

#include <folder.h>


// �����ռ�������
#define DCS_CASHMEM_NAME      "CashMem"   
#define DCS_CASHREC_NAME        "ISAN"
#define DCS_CASHMAG_NAME       "CashMag"
#define DCS_POSSRV_FOLD_NAME   "PosSvr"
#define DCS_BCDA_NAME          "BCDA"
#define DCS_POSCOM_NAME        "STCP"
#define   DCS_FLAG_USED           0x00000001 //ռ�ñ�־
#define   DCS_FLAG_DIRECT         0x00000002 //POS������ֱ��

#define   DCS_ROLE_PASSIVE        1 //������passive����ɫ
#define   DCS_ROLE_ACTIVE         2 //����(active)��ɫ

#define   DCS_STAT_DISCONNECTED   0 //�Ѿ���������״̬
#define   DCS_STAT_CALLING        1 //���ں��� 
#define   DCS_STAT_LISTENING      2 //���ڼ���
#define   DCS_STAT_CONNECTED      3 //�Ѿ���������״̬
#define   DCS_STAT_STOPPED        4 //����ֹͣ״̬

//for termcom
#define   DCS_STAT_READNET        5 //���ڶ�����
#define   DCS_STAT_WRITENET       6 //����д����
#define   DCS_STAT_READFOLD       7 
#define   DCS_STAT_WRITEFOLD      8 
#define   DCS_STAT_WAITREQ        9
#define   DCS_STAT_SENDREQ        10
#define   DCS_STAT_WAITRESP       11 

//HstSrv���յ���Ϣ����
#define	  DCS_LINKESTAB_NOTI      1001 //����ͨ�Ž��̵���·����֪ͨ
#define	  DCS_COMMAND_LINE        1002 //���������нӿڵ�����


//���������нӿڵ���������
#define   DCS_CMD_STOP            1   //ֹͣһ����������̬����·
#define   DCS_CMD_START           2   //����һ������ֹ̬ͣ����·
#define   DCS_CMD_EXIT            3   //�������˳�����
#define   DCS_CMD_ADD             4   //����һ����·
#define   DCS_CMD_MODIFY          5   //�޸�һ����·
#define   DCS_CMD_DEL             6   //�޸�һ����·


// ����ͷ����

#define DCS_HEAD_TRM              1
#define DCS_HEAD_BANK             2
#define DCS_HEAD_ZFY              3
#define DCS_HEAD_AMP              4
#define DCS_HEAD_TRM1             5
struct trmgrpinfo
{
	char caFoldName[MAX_FOLDNAME_SIZE + 1];
	long lFlags;         //��־��
	int    iFid;
	pid_t pidComm;      //���̺�
	short  iStatus;     //��terminal��ǰ ��״̬
  char    sRemoteIp[16];   //ʵ�ʵ�Զ�˵�IP��ַ
  int    iRemotePort; //ʵ�ʵ�Զ�˵Ķ˿ں�
  int    iLocalPort;  //���ض˿�  
};
struct trminfo
{
    char   caFoldName[MAX_FOLDNAME_SIZE + 1];
                        //��terminal��Ӧ�ļ��е�����
    int    iGid;        //���
    int    iFid;         
    int    iApplFid;
    int    iRole;               
    long   lFlags;      //��terminal�ı�־��
    short  iStatus;     //��terminal��ǰ ��״̬
    int    maxterm;
    int    minterm;
    int    iShmId;
    pid_t  pidComm;     //��terminal��Ӧ��TCP���̵�Id
    int    iType ;      //ͨѶ��ʽ���������Ǳ���    
    char   sMsgType[5];  //��������
    int    iHeadType;    //����ͷ���� 
    char   sRemoteIp[16];   //ʵ�ʵ�Զ�˵�IP��ַ
    int    iRemotePort; //ʵ�ʵ�Զ�˵Ķ˿ں�
    int    iLocalPort;  //���ض˿�    
    struct trmgrpinfo *trmArray;
};

struct trmcom
{
    int    iTrmCnt;  //elements# in array trmArray[]
    int    iShmId;
    struct trminfo linkArray[1]; 
};

//
//TrmSvr���յ���Ϣ����
//
struct trmrequest     //ͨ�ø�ʽ
{
  char caMsgType[4];  //��Ϣ����
  char caMsgData[1];  //��Ϣ����
};

struct hstrequest     //ͨ�ø�ʽ
{
  char caMsgFlag[4];  // �����־:"ISEI"
  char caMsgType[4];  //��Ϣ����
  char caMsgData[1];  //��Ϣ����
};

// comunication cortrol table(BCDA)
struct BCDA
{
  char caBankCode[11];// ��Ա�д���
  int  iBankSeq;// ��Ա�е�˳��ţ������źŷ���
  char caBankName[15];// ��Ա������
  char cNetFlag;// ����������Ŀ
  char cAppFlag;            // Ӧ�����ӱ�־
  char cSafFlag;            // �洢ת����־
  char cResetKeyFlag;       // ������Կ��־
  char ReadFlag;          // ���ݽ�����·��־
  char SendFlag;          // ���ݷ�����·��־
  int  iDaySwhMaxTries;     // �ط����п�ʼ���������Դ���
  char caType[8];           // The Type of "sh p POSI ICSC"

};

struct BCDAst
{
  int    iSemId;            //�źŵƱ�ʶ��,���ڻ������
  int    iBankNum;          // ��Ա����Ŀ
  struct BCDA stBcda[1];    // BCDA���׵�ַ
};


struct commnotify     //Tcpip���̵�֪ͨ��ʽ
{
  char caMsgFlag[4];  // �����־:"ISEI"
  char caMsgType[4];  //��Ϣ����
  char caFoldName[4]; //�ļ������Ƽ���·����
  int  iCommPid;
  int  iStatus;       //��·��ǰ״̬,0��ʾ�Ͽ�,1��ʾ����
};

struct trmcmd         //�����нӿ����������ʽ
{
  char caMsgFlag[4];  // �����־:"ISEI"
  char caMsgType[4];  //��Ϣ����
  char caFoldName[30]; //�ļ������Ƽ���·����
  int  iCommand;      //���������
};


char   *g_pcBcdaShmPtr;             // �����ڴ�BCDA���׵�ַ

#endif
