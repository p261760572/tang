#include <stdio.h>
#include <string.h>
#include "dccpgdef.h"
#include "errlog.h"
#include  "dccglobl.h"
#include  "dccdcs.h"

// global variable
char	g_aMonitorIP[20],g_aLocalIP[20];
int	g_iMonitorPort,g_iMonitorTimeOut,g_iLocalPort=0;
static char	*g_pcIseHome = NULL;

// Functions declaration
int LoadMonitorLnk();
int SendToMonitor(char *pSendBuffer,int iLength);

/*
 ��������:  LoadMonitorLnk()
 ����˵��:
           ��
 �� �� ֵ:
           0���ɹ�
           -1��ʧ��
 ��������:
           ��monitorlnk.conf�ж���ļ�ط�����������Ϣ���ص�ȫ�ֱ�����
*/

int LoadMonitorLnk()
{
	int	iFd=-1,iRc;
  	char	caFileName[256],caBuffer[1024];
  	char	*pAddr1,*pIp1,*pPort1,*pAddr2,*pIp2,*pPort2,*pTime;

  	ISE_TRACE_BGN(P_LoadMonitorLnk);

	//��ȡISE����Ŀ¼
  	if (NULL == (g_pcIseHome = (char *)getenv("ISEHOME")))
  	{
    		sprintf(g_caMsg, "<LoadMonitorLnk--getenv> cannot get environment variable 'ISEHOME' !");   
    		ErrLog(1000, g_caMsg, RPT_TO_LOG, 0, 0 );
    		ISE_TRACE_END(-1);
  	}
  
  	//�趨�����ļ�monitorlnk.conf�ľ���·��������
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/etc/dcs/%s",g_pcIseHome,DCS_MONITORLNK_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		ErrLog( 1000, "<LoadMonitorLnk--conf_open> open monitorlnk.conf fail ! ", RPT_TO_LOG, 0, 0 );
    		ISE_TRACE_END(-1);
  	}

  	//��ȡ���е�������'comm.monitor'
  	memset(caBuffer,0,sizeof(caBuffer));
       	conf_get_first_string(iFd, "comm.monitor",caBuffer);

	pAddr1 = strtok(caBuffer," \t\r");//��ַ��1
	pAddr2 = strtok(NULL," \t\r");//��ַ��2
    	pTime = strtok(NULL   ," \t\r");//��ʱʱ�� // add by tang at 2001/03/26
    	if ( pAddr1==NULL || pAddr2==NULL )
    	{
      		//invalid address
      		ErrLog( 1000, "<LoadMonitorLnk> address of local or monitor is NULL! ", RPT_TO_LOG, 0, 0 );
      		conf_close(iFd);
      		ISE_TRACE_END(-1);
    	}

	pIp1 = strtok(pAddr1, ":");
    	pPort1 = strtok(NULL, ":\r\t\n");
    	if (pIp1==NULL || pPort1==NULL)
    	{
      		//invalid address
      		sprintf(g_caMsg,"<LoadMonitorLnk> local IP address [%s] or port [%s] is NULL!",pIp1,pPort1);
    		ErrLog(1000, g_caMsg, RPT_TO_LOG, 0, 0 );
    		conf_close(iFd);
      		ISE_TRACE_END(-1);
    	}

    	pIp2 = strtok(pAddr2, ":");
    	pPort2 = strtok(NULL, ":\r\t\n");
    	if (pIp2==NULL || pPort2==NULL)
    	{
      		//invalid address
      		sprintf(g_caMsg,"<LoadMonitorLnk>monitor IP address [%s] or port [%s] is NULL!",pIp2,pPort2);
    		ErrLog(1000, g_caMsg, RPT_TO_LOG, 0, 0 );
    		conf_close(iFd);
      		ISE_TRACE_END(-1);
    	}

	strcpy(g_aLocalIP,pIp1);
    	g_iLocalPort = atoi(pPort1);
    	strcpy(g_aMonitorIP,pIp2);
    	g_iMonitorPort = atoi(pPort2);
    	g_iMonitorTimeOut = atoi(pTime);

	sprintf(g_caMsg,"<LoadMonitorLnk> local IP address=[%s], port=[%d]",g_aLocalIP,g_iLocalPort);
    	ErrLog(100, g_caMsg, RPT_TO_LOG, 0, 0 );

    	sprintf(g_caMsg,"<LoadMonitorLnk> monitor IP address=[%s], port=[%d], timeout=[%d]",g_aMonitorIP,g_iMonitorPort,g_iMonitorTimeOut);
    	ErrLog(100, g_caMsg, RPT_TO_LOG, 0, 0 );

   	conf_close(iFd);
  	ISE_TRACE_END(0);
}

/*
 ��������:  SendToMonitor
 ����˵��:
           pSendBuffer����Ҫ���͵���ط������ı�׼ISO8583����
           iLength�����͵ı��ĵ��ֽ���
 �� �� ֵ:
           0���ɹ�
           -1��ʧ��
 ��������:
           ����׼ISO8583���ķ��͵���ط�����
*/

int SendToMonitor(char *pSendBuffer,int iLength)
{
	int	iConnSock,iReturn;

	ISE_TRACE_BGN(P_SendToMonitor);
	
	if (pSendBuffer==NULL || iLength<=0)
	{
		sprintf(g_caMsg,"<SendToMonitor> Send buffer [%s] is NULL or length [%d] is too short!",pSendBuffer,iLength);
      		ErrLog(1000, g_caMsg, RPT_TO_LOG, 0, 0 );
      		ISE_TRACE_END(-1);
	}

	iConnSock = tcp_connet_server(g_aMonitorIP,g_iMonitorPort,g_iLocalPort);
    	if(iConnSock < 0)
    	{
      		sprintf(g_caMsg,"<SendToMonitor--tcp_connet_server> tcp_connet_server() failed:%s!",strerror(errno));
      		ErrLog(1000, g_caMsg, RPT_TO_LOG, 0, 0 );
      		ISE_TRACE_END(-1);
	}

	//forward this message to Network
    	iReturn = write_msg_to_net(iConnSock,pSendBuffer,iLength,-1);
    	if (iReturn < iLength)
    	{
      		sprintf(g_caMsg,"<SendToMonitor--write_msg_to_net> write_msg_to_net() failed:%s",strerror(errno));
      		ErrLog(1000, g_caMsg, RPT_TO_LOG, 0, 0 );
      		tcp_close_socket(iConnSock);
      		ISE_TRACE_END(-1);
    	}

    	if (iReturn > 4)  //for debug
    	{
      		sprintf(g_caMsg,"<SendToMonitor> write Net %.4d bytes-->",iReturn);
      		ErrLog(1, g_caMsg, RPT_TO_LOG, pSendBuffer, iReturn);
    	}

	tcp_close_socket(iConnSock);

	ISE_TRACE_END(0);
}
