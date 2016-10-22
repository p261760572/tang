#include "ibdcs.h"
//#include  "iso8583.h"
//#include "poskey.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>
int gs_myFid    = -1;
char   *g_pcBcdaShmPtr;
static void appSrvExit(int signo);
int port;
int gs_sock;
void TransProc(int iConnSock);
static int OpenLog(char *IDENT);
static int DoLoop();
//void setmsgdata(MsgData *msg,char *src_Buf,int iLen);
//extern struct ISO_8583 *iso8583;
//struct ISO_8583 iso8583_conf[128];


//1.创建子进程后收到的第一个包必须是签到包。

int main(int argc, char *argv[])
{
    catch_all_signals(appSrvExit);

    //prepare the logging stuff
    if(0 > OpenLog(argv[0]))
    	exit(1);
		
		char m_port[10];
		memset(m_port,0,sizeof(m_port));
		if(0>getconfig(m_port))
			exit(1);
		port=atoi(m_port);
		
    dcs_log(0,0, "poskey Servers is starting up...\n");
    u_daemonize(NULL);
  
		dcs_log(0,0,"*************************************************\n"
                "!!        Application Servers startup completed.        !!\n"
                "*************************************************\n");
    DoLoop();

    appSrvExit(0);
}

static void appSrvExit(int signo)
{
    if(signo > 0)
        dcs_log(0,0,"<appSrvExit> catch a signal %d\n",signo);
    if(signo !=0 && signo != SIGTERM && signo != SIGSEGV)
        return;

    dcs_log(0,0,"<appSrvExit> AppServer terminated.\n");
    tcp_close_socket(gs_sock);
    DssEnd( 0);
    //关闭所有线程和sock
    shm_detach((char *)g_pcBcdaShmPtr);
    exit(signo);
}

static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/appsvr1.log"
    if(u_fabricatefile("log/poskey.log",logfile,sizeof(logfile)) < 0)
        return -1;

    return dcs_log_open(logfile, IDENT);
}

static int DoLoop()
{
    char   caBuffer[2048];
    int    iRead, fromFid;
    
    int iConnSock,iListenSock,iCliAddr,iCliPort;
    int i;
    pthread_t pthread;
    signal(SIGCHLD,SIG_IGN);//用于kill掉僵死进程
		iListenSock = tcp_open_server(NULL,port);
		if (iListenSock < 0)
		{
  			dcs_log(0,0,"<DoLoop> <MoniMakeConnection--tcp_open_server> tcp_open_server() failed: %s!",strerror(errno));
  			return -1;
		}
		else dcs_log(0,0,"<DoLoop> server open port : %d",port);
    
    for(;;)
    {
        memset(caBuffer, 0, sizeof(caBuffer));
        
			  iConnSock = tcp_accept_client(iListenSock,&iCliAddr, &iCliPort);
    		if (iConnSock < 0)
  			{
    			dcs_log(0,0,"<DoLoop> <MoniMakeConnection--tcp_accept_client> tcp_accept_client() failed: %s!",\
                       		strerror(errno));
    			tcp_close_socket(iListenSock);
    			continue;
  			}   
  			gs_sock =  iConnSock;
  			i=fork();    
        if ( i==0)
        {
        	 TransProc(iConnSock);
        	 exit(0);
        }
        else if (i <0)
        {
        	 dcs_log(0,0,"<DoLoop> can not create process");
        	 return -1;
        }  
    }

}

void TransProc(int iConnSock)
{
	dcs_log(0,0,"<TransProc> create process");
	char msg_buffer[2048+1];
	int buf_len=0;
	//char limit[256+1];//权限配置
	
	char m_info[4+1];
	
	//MsgData *msg;
	
  if (! DssConect())
  {
    dcs_log(0,0,"<TransProc> Can not open oracle DB !");
    return;
  }
	
 dcs_log(0,0,"database open succ !");
	  
	for(;;)
	{
		char test_random[16+1];
		memset(test_random,0,sizeof(test_random));
		GetRandomData(test_random);
		
		dcs_log(0,0,"<TransProc> [%s] read_msg_from_AMP wait ... ",test_random);
		
		memset(msg_buffer,0,sizeof(msg_buffer));
		
		if( 0>=(buf_len=read_msg_from_AMP(iConnSock, msg_buffer, sizeof(msg_buffer),1800000)) )
		{
			dcs_log(0,0,"<TransProc> read_msg_from_AMP err");
			break;
		}
	  dcs_debug(0,0,"<TransProc> recv succ");
	  dcs_debug(msg_buffer,buf_len,"<TransProc> 收到报文 iConnSock =%d, len=%d",iConnSock,buf_len);
	  
		memset(m_info, 0, sizeof(m_info) );
		memcpy(m_info,msg_buffer+4,4);
	  
	  if ( (memcmp(m_info,"1000",4)==0) || (memcmp(m_info,"2000",4)==0) )
	  {
	  	pos_proc( msg_buffer,iConnSock,buf_len );
	  }
	  else	if ( 	(memcmp(m_info,"0010",4)==0) || \
	  						(memcmp(m_info,"0020",4)==0) || \
	  						(memcmp(m_info,"0030",4)==0) || \
	  						(memcmp(m_info,"0040",4)==0) || \
	  						(memcmp(m_info,"0050",4)==0) || \
	  						(memcmp(m_info,"0060",4)==0) )
	 	{
			pc_proc( msg_buffer,iConnSock,buf_len);	
		}
		else
		{
			dcs_log(0,0,"Unkown Packet,discard !");
			break;
		}
	}
	
	tcp_close_socket(iConnSock);
	
	DssEnd( 0);
	
	dcs_log(0,0,"<TransProc> exit process");
	
	return;
}

int getconfig(char * m_port)
{
 	char	caFileName[512];
 	
 	memset(caFileName, 0, sizeof(caFileName) );
 	
 	sprintf(caFileName, "%s/config/%s",getenv("ICS_HOME"),"poskey.ini");
 	
 	FILE *fp; 
 	char buf[1024+1];
 
	if((fp = fopen(caFileName, "r")) == NULL) 
 	{
 		dcs_log( 0,0, "<getconfig> open %s fail ! ",caFileName);
 		return -1;
 	}
 	while(1)
 	{ 
  	memset(buf, 0x00,sizeof(buf));
  	if(fgets((char *)buf, 1024, fp) == NULL) 
  	{ 
   		if(strlen(buf) == 0) break;
   		if ( memcmp( buf ,"port:",5) ==0 )
   		{
   			memcpy(m_port,buf+5,strlen(buf)-5);
   			break;
   		}	
   		break;
  	} 
   	if ( memcmp( buf ,"port:",5) ==0 )
   	{
   		memcpy(m_port,buf+5,strlen(buf)-5);
   		break;
   	}
 	} 
 	if(fp != NULL) fclose(fp); 
 		
 	if(strlen(m_port)==0) return -1;
 	
 	return 1;
}
