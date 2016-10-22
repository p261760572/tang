#ifndef __SECUSRV_H__
#define __SECUSRV_H__

struct secLinkInfo
{
    long   lFlags;      //��link�ı�־��
/*    short  iRole;       ��link�����ӽ��������еĽ�ɫ */
    short  iStatus;     //��link��ǰ ��״̬
    short  iWorkFlag;
/*    char   caLocalIp[20]; ���ص�IP��ַ */
    char   caRemoteIp[20];//������Զ�˵�IP��ַ
    char   cEncType[3];//���ܻ�����
    int    iLocalPort;   // ���صĶ˿ں� 
    int    iRemotePort;   //������Զ�˵Ķ˿ں�
    int    iSocketId;
    int    iTimeOut;    //��ʱʱ��
    int    iNum;
};

struct SECSRVCFG
{
				
				int secSemId;
        int srvStatus;
        int maxLink;
        int pNextFreeSlot;
        struct secLinkInfo secLink[1];
};
char *g_pSecSrvShmPtr;   //��ȫ�����������ڴ��׵�ַ

struct MSG_STRU
{
	
	  long iPid;
	  char iNum[3];
	  char caSocketId[5];
	  char caMsg[1];
};
#endif
