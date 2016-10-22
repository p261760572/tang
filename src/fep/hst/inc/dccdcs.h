#ifndef DCCDCS_H
#define DCCDCS_H

#include <folder.h>

// ���峣��
#define IS_TEST_OFF		0	// ����ģʽ�ر�
#define DCS_BCDA_NAME		"BCDA"	// �����ڴ�BCDA������
#define DCS_HSTCOM_NAME  	"HstC"  // ͨѶ�������Ĺ����ڴ���

// �����ռ�������
#define HSTSRV_FOLD_NAME       "HSTSRV"
#define DCS_HSTSRV_FOLD_NAME	HSTSRV_FOLD_NAME //defined in emcems.h("HSTC")
#define DCS_CASHREC_NAME        CASHREC_FOLD_NAME  //defined in emcems.h("cash")

#define DCS_COMMLNK_NAME	"isdcmlnk.conf"
					// �������ͨѶ���ӵ��ļ���

#define DCS_HSTSRV_LOG		"hst"
					// ����ͨѶ��־�ļ�
#define DCS_NET_CONECT_ON	'1'	// �������ӱ�־����
#define DCS_NET_CONECT_OFF	'0'	// �������ӱ�־�ر�



// Add by xiezr 2001/08/29
#define MONITOR_CONNECT_WAIT_TIME	10
// add end by xiezr


//����ͳ�Ա��ͨ�ŵ�ÿ�����ӵ���Ϣ
struct linkinfo
{
    char   caFoldName[MAX_FOLDNAME_SIZE + 1];
                        //��link��Ӧ�ļ��е�����
    int    iFid;        //��link��Ӧ�ļ��е�Id  
    int    iApplFid;    //��link��Ӧ��Ӧ�ý��̵��ļ��е�Id
    long   lFlags;      //��link�ı�־��
    char   commType[4];  //ͨ������
    char   commNO[3];    //ͨ��˳���
    short  iRole;       //��link�����ӽ��������еĽ�ɫ
    short  iStatus;     //��link��ǰ ��״̬
    pid_t  pidComm;     //��link��Ӧ��TCP���̵�Id
    pid_t  pidCheck;
    char   caLocalIp[20]; //���ص�IP��ַ
    char   caRemoteIp[20];//������Զ�˵�IP��ַ
    int    sLocalPort;    //���صĶ˿ں�
    int    sRemotePort;   //������Զ�˵Ķ˿ں�
    int    iTimeOut;      // ������ⳬʱʱ��   // add for tang by 2001/03/26
    int    iRemoteIp0;     //ʵ�ʵ�Զ�˵�IP��ַ
    int    sRemotePort0;   //ʵ�ʵ�Զ�˵Ķ˿ں� 
    int    iNotifyFlag;    //��·״̬֪ͨ��־
    char   caMsgType[4];   //��������
    char   caNotiFold[MAX_FOLDNAME_SIZE+1];
    long    iRcvTime;      //�ϴ���ȡ����ʱ��
    char   caCheckFlag;    //ͨ�ż������������   
};

//���������кͳ�Ա��������Ϣ�Ĺ����ڴ�ṹ
struct hstcom
{
    int    iLinkCnt;    //linkArray[]�����еĲ�λ����
    int    iShmId;      //�����ڴ��Id
    struct linkinfo linkArray[1]; 
};

//
//HstSrv ���յ���Ϣ�Ľṹ
//
struct hstrequest     //ͨ�ø�ʽ
{
  char caMsgFlag[4];  // �����־:"ISEI"            
  char caMsgType[4];  //��Ϣ����
  char caMsgData[1];  //��Ϣ����
};

struct commnotify     //Tcpip���̵�֪ͨ��ʽ
{
  char caMsgFlag[4];  // �����־:"ISEI"            
  char caMsgType[4];  //��Ϣ����
  char caFoldName[4]; //�ļ������Ƽ���·����
  int  iCommPid;
  int  iStatus;       //��·��ǰ״̬,0��ʾ�Ͽ�,1��ʾ����
};

struct hstcmd         //�����нӿ����������ʽ
{
  char caMsgFlag[4];  // �����־:"ISEI"        
  char caMsgType[4];  //��Ϣ����
  char caFoldName[30]; //�ļ������Ƽ���·����
  int  iCommand;      //���������
};

#define   DCS_FLAG_USED           0x00000001 //ռ�ñ�־
#define   DCS_FLAG_DIRECT         0x00000002 //POS������ֱ��
#define   DCS_FLAG_NACREADER      0x00000004
#define   DCS_FLAG_NACWRITER      0x00000008

#define   DCS_ROLE_PASSIVE        0 //������passive����ɫ
#define   DCS_ROLE_ACTIVE         1 //����(active)��ɫ
#define   DCS_ROLE_SIMPLEXSVR     2 //������ɫ����
#define   DCS_ROLE_SIMPLEXCLI     3 //������ɫ����

#define   DCS_STAT_DISCONNECTED   0 //�Ѿ����ڶϿ�״̬
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


//��NAC����ʱ�����ݰ�ͷ��֮�ṹ

#define  BYTE  unsigned char

// ǰ�����ֽ�Ϊ���հ��ĳ��ȣ���5���ֽ�Ϊ���ظ��ӵ�TPDUͷ
// ����Ӧ������ʱ���뽻��Դ��ַ��Ŀ�ĵ�ַ��ֵ
struct nac_header
{
    BYTE  length[2];
    BYTE  dummy1[1];		//0x60
    BYTE  destaddr[2];		//Դ��ַ
    BYTE  srcaddr[2];		//Ŀ�ĵ�ַ
};
struct monilinkinfo
{
    long   lFlags;      //��link�ı�־��
    short  iRole;       //��link�����ӽ��������еĽ�ɫ
    short  iStatus;     //��link��ǰ ��״̬
    char   caLocalIp[20]; //���ص�IP��ַ
    char   caRemoteIp[20];//������Զ�˵�IP��ַ
    int    sLocalPort;    //���صĶ˿ں�
    int    sRemotePort;   //������Զ�˵Ķ˿ں�
    int    iTimeOut;      // ������ⳬʱʱ��   // add for tang by 2001/03/26
    int    iRemoteIp0;     //ʵ�ʵ�Զ�˵�IP��ַ
    int    sRemotePort0;   //ʵ�ʵ�Զ�˵Ķ˿ں� 
    int    timeOut;        //ͨ�ż��ʱ��
    int    socket_id;
};
#endif

typedef  struct comm_link
{
	int num;
	int iRole;
	int iSocket[16];
	int flag[16];
}SOCKET_LINK_STATUS;
