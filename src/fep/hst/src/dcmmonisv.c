 #define _MainProg

// �������ƣ�dcmmonisv.c
// ����������
//          �ն˹���ϵͳTCP/IPͨѶ����
// �����嵥��
//           main()

	
#include  "dccglobl.h"
#include  "dccpgdef.h"
#include  "dccdcs.h"
#include  "dccsock.h"

#include <pthread.h>

#define DCS_MONISRV_LOG  "monitor"
#define DCS_MONILNK_NAME "monilnk.conf"
static g_s=0;
static int g_qid;
struct monilinkinfo   g_linkinfo[10];

static   char            *g_pcIseHome = NULL;
static   pid_t            g_pidChild  = -1;
static   int              g_iMoniMntFoldId = -1,g_iListenSock= -1;
static   char             g_caIdent[64];
static   int    g_port;
// Functions declaration
void MoniTcpipExit(int signo);
int MoniTcpipOpenLog();
void MoniMakeConnection();
void DoMoni();
int MoniReadFromIse();
static int LoadCommLnk();
static  pthread_t g_thread;
static  int g_CommCnt;


/*
  �������ƣ�main
  ����˵����
           argv[0] ��ִ�г�������
           argv[1] �ͱ�ͨ�ų����������ļ�����
  �� �� ֵ��
           ��
  ����������
           1.���������ն˹���ϵͳ�ͻ��˵ı���,��ת������Ӧ��Ӧ�ý���
           2.���ձ���Ӧ�õı���,�����䷢������
*/

int main(int argc, char *argv[])
{
	int i;
	struct monilinkinfo *arg;
	pthread_t sthread;
  	catch_all_signals(MoniTcpipExit);

	memset(&g_thread,0,sizeof(pthread_t));
	memset(&g_linkinfo[0],0,sizeof(struct monilinkinfo)*10);

	

  	//prepare the log stuff
  	if (0 > MoniTcpipOpenLog())
    		goto lblExit;

  	g_CommCnt = 10;
  	if ( LoadCommLnk()<0)
  	{
  		dcs_log(0,0, "<Moni LoadCommLnk> Failure to load monilnk.ini !" );
    		goto lblExit;
  	}
  	
  		
  		if ( pthread_create(&g_thread, NULL,( void * (*)(void *))MoniMakeConnection,NULL) !=0)
  		{
  			dcs_log(0,0, "Can not create thread ");
  			goto lblExit;
  		};
  		
			if ( pthread_create(&sthread, NULL,( void * (*)(void *))DoMoni,NULL) !=0)
			{
					dcs_log(0,0, "Can not create void DoMoni thread ");
					goto lblExit;
			}
			pthread_detach(sthread);

  	dcs_debug(0,0, "begin MoniReadFromISE " );
	MoniReadFromIse();

lblExit:
//	PidUnregister();
  	MoniTcpipExit(0);
}//main()

/*
  �������ƣ�MoniTcpipExit
  ����˵����
           signo��������ź���
  �� �� ֵ��
           ��
  ����������
           ��������ź�ΪSIGTERMʱ���ú����ͷ���Դ����ֹ��ǰ���̣��������κδ���
*/

void MoniTcpipExit(int signo)
{
	int i;
  	dcs_debug(0,0,"<MoniTcpipExit--%s> catch a signal %d !",g_caIdent, signo);

   	//parent process
  	switch (signo)
  	{
    	case SIGUSR2: //monimnt want me to stop
     		break;

    	case 0:
    	case SIGTERM:
      		break;

    	case SIGCHLD:
		return;

    	default:
      		
      		return;
  	}
		for( i =0; i <g_CommCnt;i++)
  	{
			  if ( g_linkinfo[i].iStatus == DCS_STAT_CONNECTED)
			  {
	  	  			tcp_close_socket(g_linkinfo[i].socket_id);
			  }
		}
  	pthread_join(g_thread, NULL);
        queue_delete(g_qid);
  	exit(signo);
}

/*
  �������ƣ�MoniTcpipOpenLog
  ����˵����
           ��
  �� �� ֵ��
           0���ɹ�
           -1��ʧ��
  ����������
           1.���ϵͳ��Ŀ¼,��֮����ȫ�ֱ���g_pcIseHome
           2.��ȡϵͳ�����ļ�system.conf
           3.������־�ļ�����־��ʽ
*/

int MoniTcpipOpenLog()
{
  	char   caLogName[256];
    char logfile[400];
  
    memset(caLogName,0,sizeof(caLogName));
        strcpy(caLogName,"log/AlarmMoni.log");
    if(u_fabricatefile(caLogName,logfile,sizeof(logfile)) < 0)
    {
            return -1;
    }
     dcs_log_open(logfile, "Alarm");

 	return 0;
}


/*
  �������ƣ�MoniMakeConnection
  ����˵����
           ��
  �� �� ֵ��
           >0���ɹ������������ϵ�socket������
           -1��ʧ��
  ����������
           ���ݹ����ڴ��е�������Ϣ�жϣ�
           ��ͨѶ����Ϊ�����������������ض˿ںţ�һ���Է������ϣ��򷵻ظ����ӵ�socket��������
           ��ͨѶ����Ϊ�ͻ��ˣ����������ӶԷ������������ӳɹ��󣬷��ظ����ӵ�socket��������
*/

void MoniMakeConnection( )
{
  	
  	struct monilinkinfo * arg;
  	char buffer[100];
  	int i,nready;
  	int   iListenSock, iConnSock, iCliAddr;
	  int   iCliPort,maxfd;
		struct in_addr in;	    		
			    		
    fd_set sock_set,sock_set1;    
   
    FD_ZERO(&sock_set);
    FD_ZERO(&sock_set1);
		g_iListenSock = tcp_open_server(NULL,g_port);
		if (g_iListenSock < 0)
		{
  			dcs_log(0,0,"<MoniMakeConnection--tcp_open_server--%s> tcp_open_server() failed: %s!",g_caIdent,strerror(errno));
  			exit( -1);
		}

		FD_SET(g_iListenSock, &sock_set);
		FD_SET(g_iListenSock, &sock_set1);
		iConnSock = g_iListenSock; 
		maxfd=iConnSock;
     for ( ; ;)
  	{  
  		 		
			    		//we are the server end, create a listening socket and
			    		//wait to accept the clients' connection request
			    	 memcpy(&sock_set,&sock_set1,sizeof(sock_set));
		          dcs_debug(0,0,"begin Moni event for socket ! server port=%d, server sock id=%d",	g_port,g_iListenSock);
			    	  nready = select(maxfd+1,&sock_set,NULL,NULL,NULL);
//			    	  nready = select(4000+1,&sock_set,NULL,&sock_set,NULL);
			        dcs_debug(0,0,"recv a socket message ret_code=%d",nready);
		          
				      if ( FD_ISSET( g_iListenSock,&sock_set ))
				      {
				      			dcs_debug(0,0,"entry accept");
				      			iConnSock = tcp_accept_client(g_iListenSock,&iCliAddr, &iCliPort);
				      			if (iConnSock < 0)
			      			 {
			        			dcs_log(0,0,"<MoniMakeConnection--tcp_accept_client--%s> tcp_accept_client() failed: %s!",\
			                           		g_caIdent,strerror(errno));
			        			tcp_close_socket(g_iListenSock);
			        			exit (-1);
			      			}
			      			in.s_addr = iCliAddr;
									for( i =0; i <g_CommCnt;i++)
			      			{
			      				  if ( g_linkinfo[i].iStatus == DCS_STAT_CONNECTED)
			      				  {
			      											      				  	  	
			      				  	    if ( strcmp(g_linkinfo[i].caRemoteIp,inet_ntoa(in))==0)
			      				  	    {
			      				  	    	g_linkinfo[i].iStatus = DCS_STAT_STOPPED;
			      				  	    	memset(g_linkinfo[i].caRemoteIp,0,sizeof(g_linkinfo[i].caRemoteIp));
			      				  	    	g_linkinfo[i].sRemotePort = 0;
			      				  	     	tcp_close_socket(g_linkinfo[i].socket_id);
			      				  	    	dcs_debug(0,0,"repeat link ip= %s, port=%d,  close old link port =%d !!",inet_ntoa(in),iCliPort,g_linkinfo[i].socket_id);
			      				  	  		g_linkinfo[i].socket_id =-1;
			      				  	    	break;
			      				  	    }
			      				  	    
			      				  }
			      			}
			      			for( i =0; i <g_CommCnt;i++)
			      			{
			      				  if ( g_linkinfo[i].iStatus != DCS_STAT_CONNECTED)
			      				  {
			      								g_linkinfo[i].socket_id = iConnSock;
			      				  	  	g_linkinfo[i].iStatus = DCS_STAT_CONNECTED;
			      				  	    strcpy(g_linkinfo[i].caRemoteIp,inet_ntoa(in));
			      				  	    g_linkinfo[i].sRemotePort = iCliPort;
			      				  	    break;
			      				  }
			      			}
			      			if ( i >= g_CommCnt)
			      			{
			      					dcs_log(0,0," Not found free slot ,Can not Add new link, current link closeing ip=%s ,port =%d",inet_ntoa(in),iCliPort);
			      					continue;
			      			}
			      			FD_SET(g_linkinfo[i].socket_id, &sock_set1);
			      			if (g_linkinfo[i].socket_id >maxfd) maxfd = g_linkinfo[i].socket_id;
			      			dcs_debug(0,0,"new link ip= %s, port=%d",inet_ntoa(in),iCliPort);
			      			
				      }
	
		      		for( i =0; i <g_CommCnt;i++)
	      			{
	      				  
	      				  if ( g_linkinfo[i].iStatus == DCS_STAT_CONNECTED)
	      				  {
	      				  	  if ( FD_ISSET( g_linkinfo[i].socket_id,&sock_set ))
	      				  	  {
	      				   					dcs_debug(0,0,"entry disconnect");
	      				  	  			g_linkinfo[i].iStatus = DCS_STAT_STOPPED;
	      				  	  			tcp_close_socket(g_linkinfo[i].socket_id);
	      				  	  			FD_CLR(g_linkinfo[i].socket_id, &sock_set1);
	      				  	  			dcs_debug(0,0,"close a link %s, port=%d",g_linkinfo[i].caRemoteIp,g_linkinfo[i].sRemotePort);
	      				  	  			memset(&g_linkinfo[i].caRemoteIp[0],0,sizeof(g_linkinfo[i].caRemoteIp));
	      				  	  			g_linkinfo[i].sRemotePort=0;
	      				  	  			g_linkinfo[i].socket_id=-1;
	      				  	  			
	      				  	   }
	      				  }
	      		  }
		}
}


/*
  �������ƣ�MoniReadFromIse
  ����˵����
           ��
  �� �� ֵ��
           always return -1
  ����������
           ���ļ��ж�ȡӦ�ý��̲����Ľ��ױ���,Ȼ���䷢������
*/



int MoniReadFromIse()
{
  	char  caBuffer[2048];
  	int   nBytesRead, nBytesWritten;
  	int   iOrgFid,s=0,qid,i;


	if ((qid=queue_connect("monitor"))<0)
		if ( (qid=queue_create("monitor"))<0)
		{
			dcs_log(0,0, "create queue fail" );
			return -1;
		}
       g_qid=qid;
  	for (;;)
  	{
    		dcs_debug(0,0, "<MoniReadFromIse--begin read--data> " );
  		memset(caBuffer,0,sizeof(caBuffer));
  		if ( (nBytesRead=queue_recv( qid, caBuffer, sizeof(caBuffer), 0))<0)  		
    		{
      			//read error
      			
      			dcs_log(0,0, "<MoniReadFromIse--read-->  failed" );
      			break;
    		}

    		dcs_debug(caBuffer,nBytesRead, "<MoniReadFromIse--read--data> " );
    		
    		for ( i =0 ; i <g_CommCnt;i++)
    		{
    					if ( g_linkinfo[i].iStatus == DCS_STAT_CONNECTED)
    					{
			    						nBytesWritten = write_msg_to_net(g_linkinfo[i].socket_id,caBuffer,nBytesRead,0);
											dcs_debug(caBuffer , nBytesRead, "Send Data  " );
											if (nBytesWritten < nBytesRead)
					    				{
							      			dcs_log(0,0,"<Tcpip--%s> write_msg_to_net() failed:%s",g_caIdent,strerror(errno));
							      			close(g_linkinfo[i].socket_id);
							      			g_linkinfo[i].iStatus = DCS_STAT_STOPPED;
							      			
							    		}
		    			}
    		}
		 	       

  	}//for(;;)

  
  	return (-1);
}
int LoadCommLnk()
{
  	char   caFileName[256], caBuffer[1024];
  	int    iFd=-1, iRc,iSlotIndex = 0;  
  	struct monilinkinfo *pLinkEntry=NULL;
  	char   *pLinkFldName, *pApplFldName,*pRole, *pAddr1, *pAddr2;
  	char  *pPort;    

  	
  	//�趨commlnk�����ļ��ľ���·��������
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/config/%s",getenv("ICS_HOME"),DCS_MONILNK_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log( 0,0, "<HstSrv> open isdcmlnk.conf fail ! ");
//    		ISE_TRACE_END(-1);
		return -1;
  	}

  	//��ȡ���е�������'comm'
  	for( iRc = conf_get_first_string(iFd, "comm",caBuffer);\
       		iRc == 0 && iSlotIndex <10;\
       		iRc = conf_get_next_string(iFd, "comm",caBuffer))
  	{
  		dcs_debug(0,0, "%s ",caBuffer);   
  		pPort    = strtok(caBuffer," \t\n");  //ͨ������
  		if ( pPort !=NULL)
      		g_port = atoi(pPort);
      else
      	g_port =-1;
  	}//for

  	conf_close(iFd);
  	if (g_port >0)
  		return(1);
  	else
  		return(-1);

lblFailure:
  	conf_close(iFd);
  	return(-1);
}

void DoMoni()
{
  	char    caMsg[512];
  	int     nRead, iOrgFid,foldId,i;
  	pid_t   pidChld;
        i=0;
     while( i<3) 
     {
  	if (fold_initsys() < 0)
  	{
               if( i<2)
               {
                   usleep(500000);
                   i++;
                   continue;
               }	
        	dcs_log(0,0, "<HstSrv> cannot attach to FOLDER !");
    		MoniTcpipExit(0);
  	}
        break;
     }
  	foldId = fold_create_folder("Monitor");
  	if (foldId < 0)  
      		foldId = fold_locate_folder("Monitor");
  	if (foldId < 0)  
  	{
    		dcs_log(0,0,"<HstSrv> cannot create folder '%s'%d!",\
                      "Monitor",(errno));
    		
    		MoniTcpipExit(0);
  	}
  	fold_set_attr(foldId, FOLD_FLAG_NEEDSIG);
  
  	fold_purge_msg(foldId);

  	for (;;)
  	{
 
    		memset(caMsg,0,sizeof(caMsg));
    		nRead = fold_read(foldId, &iOrgFid, caMsg,sizeof(caMsg),TRUE);
    		if (nRead < 0)
    		{
	      			if (fold_sys_active() < 0)
	        			MoniTcpipExit(0);
    		}

  	}//for(;;)
  
  	return ;  
}


