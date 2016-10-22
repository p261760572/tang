#define _MainProg

// 程序名称：dcmtcp.c
// 功能描述：
//           成员行TCP/IP通信程序
// 函数清单：
//           main()

#include <time.h>
#include <math.h>
#include  "dccglobl.h"
#include  "dccpgdef.h"
#include  "dccdcs.h"
#include  "dccsock.h"
//#include  "errlog.h"
//#include  "tmctms.h"
#include  "tmcibtms.h"
#include <signal.h>
#include <pthread.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)
static   char            *g_pcIseHome = NULL;
static   pid_t            g_pidChild=-1,g_pidParent=-1;
static   int              g_iSock     = -1;
static   struct linkinfo *g_pLinkEntry = NULL;
static   int              g_iHstSrvFoldId = -1;
static   char             g_caIdent[64]={0x00};
static   char             g_iType[5];
static   short            g_pTime=0;
static   short            g_shutdown_flag =0;
struct hstcom *pLinkCtrl = NULL;

pthread_t sthread,sthread1;
//pthread_t gthread;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int ReadFromNet( struct linkinfo * spLinkEntry);
int ReadFromIse( struct linkinfo * spLinkEntry);
static void TcpipExit(int signo);
struct linkinfo *  GetLinkInfo(char *pcFoldName,char * commNO);
static int MakeConnection( struct linkinfo *);
static int NotifyMoni ( char *buf );
void SendCheckNetMsg();
struct BCDAst *g_pcBcdaShm=NULL;
int WriteTraceFile(char * msgTrace,int nbytes,char cDirect);

// 函数名称：main()
// 参数说明：
//           argv[0] 可执行程序名称
//           argv[1] 和本通信程序相连的文件夹名
// 返 回 值：
//           无
// 功能描述：
//           1.接收来自成员行的报文,并转交给相应的应用进程
//           2.接收本地应用的报文,并将其发往网络

void  quit()
{
	 pthread_exit(0);
}
int main(int argc, char *argv[])
{
	  char buf[512],tbuf[10];
  	int i;
  	
    struct  tm *tm;   time_t  t;
  	catch_all_signals(TcpipExit);
//  	signal(SIGCHLD,SIG_IGN);
  	signal(SIGPIPE, SIG_IGN);

    g_pidParent=-1;
  	//prepare the log stuff
  	sprintf(buf,"%s%s",argv[1],argv[2]);
  	if (0 > TcpipOpenLog(buf))
    		goto lblExit;
    if ( atoi(getenv("SPTIME_SIGN")) >0)
          g_pTime=1;
    //check the command line and get my folder's name
  	if (argc < 3)
  	{
    		dcs_log(0,0,"<Tcpip> Usage: %s folderName.",argv[0]);
    		goto lblExit;
  	}
  	else //argv[1] is the folder name
  	{
     		memset(g_caIdent, 0, sizeof(g_caIdent));
     		sprintf(g_caIdent, "%s%s", argv[1],argv[2]);
  	}
  	if(dcs_connect_shm() < 0)
    {
  	  dcs_log(0,0,"dcs_connect_shm() failed:%s\n",strerror(errno));
   		 exit(1);
    }
  	//连接folder系统
  	dcs_debug(0,0,"connect folder system");
  	i=0;
  	while(1)
  	{
	  	if (fold_initsys() < 0)
	  	{
	    		if( i<100 )
	    		{
	    		  i++;
	    		  usleep(50000);
	    		  continue;
	    	  }
	    		dcs_log(0,0,"<Tcpip--%s> cannot attach to FOLDER !",g_caIdent);
	    		
	    		goto lblExit;
	  	}
	  	break;
    }
  	//定位HstSrv的文件夹
  	dcs_debug(0,0,"locate folder id");
  	g_iHstSrvFoldId = fold_locate_folder(DCS_HSTSRV_FOLD_NAME);
  	if (g_iHstSrvFoldId < 0)
  	{
    		dcs_log(0,0,"<Tcpip--%s>cannot locate folder %s!",g_caIdent,DCS_HSTSRV_FOLD_NAME);
    		
    		goto lblExit;
  	}
  	//连接共享内存BCDA
   	g_pcBcdaShm = (struct BCDAst *)shm_connect(DCS_BCDA_NAME,NULL);
  	if (g_pcBcdaShm == NULL)
  	{
    		dcs_log(0,0,"<Tcpip--%s> cannot connect shared memory %s!",g_caIdent,DCS_BCDA_NAME);
    		goto lblExit;
  	}
  	//连接共享内存HSTCOM,获得与自己对应的link信息
  	dcs_debug(0,0,"locate link msg");
  	g_pLinkEntry = GetLinkInfo(argv[1],argv[2]);
  	if (g_pLinkEntry == NULL)
  	{
    		dcs_log(0,0,"<Tcpip--%s> cannot get corresponding item in link info array !",g_caIdent);
    		
    		goto lblExit;
  	}
  	dcs_debug(0,0,"<Tcpip--%s> link role [%d]",g_caIdent,g_pLinkEntry->iRole);
    memcpy(g_iType,g_pLinkEntry->caMsgType,4);
  	//建立和对方的网络链接,获得连接套接字
  	g_iSock = MakeConnection(g_pLinkEntry);
  	dcs_debug(0,0,"create socket succ! sockid=[%d]",g_iSock);
  	if (g_iSock < 0)
    		goto lblExit;
    TcpipUpdateBCDANet( DCS_STAT_CONNECTED );
		memset(tbuf,0,sizeof(tbuf));
		strcpy(tbuf,argv[1]);
    time(&t);
 		tm = localtime(&t);
 		sprintf(buf,"SYSICOMML %s %3.3s %d connect ok[%02d:%02d:%02d]",tbuf,g_pLinkEntry->commNO,g_pLinkEntry->iStatus,tm->tm_hour,tm->tm_min,tm->tm_sec);
		NotifyMoni( buf );
//		sthread1=0;

    dcs_debug(0,0,"check_flag =%d",g_pLinkEntry->caCheckFlag);
		if(g_pLinkEntry->caCheckFlag )
  	{
  		  if ( g_pLinkEntry->iRole != DCS_ROLE_SIMPLEXSVR )
				{
					if ( pthread_create(&sthread1, NULL,( void * (*)(void *))SendCheckNetMsg,g_pLinkEntry) !=0)
			    {
						dcs_log(0,0, "Can not create SendCheckNetMsg thread ");
						goto lblExit;
			    }
//			    pthread_detach(sthread1);
		    }
		}
//		gthread=pthread_self();
    if ( g_pLinkEntry->iRole == DCS_ROLE_SIMPLEXSVR )
    {
//     	  gthread=gettid();
		    signal(SIGTERM,TcpipExit);
     	  ReadFromNet(g_pLinkEntry);
    	  goto lblExit;

  	}
  	else if ( g_pLinkEntry->iRole == DCS_ROLE_SIMPLEXCLI )
    {
 //    	  gthread=gettid();
		    signal(SIGTERM,TcpipExit);
     	  ReadFromIse(g_pLinkEntry);
    	  goto lblExit;

  	}
  	else if ( pthread_create(&sthread, NULL,( void * (*)(void *))ReadFromIse,g_pLinkEntry) !=0)
		{
					dcs_log(0,0, "Can not create ReadFromIse thread ");
					goto lblExit;
		}

//		pthread_detach(sthread);
		
		signal(SIGTERM,TcpipExit);
		ReadFromNet(g_pLinkEntry);
	

lblExit:
  	
  	TcpipExit(0);
}//main()


// 函数名称  TcpipExit
// 参数说明：
//          signo   捕获的信号值
// 返 回 值：
//          无
// 功能描述：
//          当捕获的信号为 SIGTERM时，该函数释放资源后终止当前进程;否则
//          不做任何处理

void TcpipExit(int signo)
{
  	char buf[512],tbuf[10];
  	int i;
        struct  tm *tm;   time_t  t;

  	dcs_debug(0,0,"<Tcpip--%s> catch a signal %d !",g_caIdent, signo);
  	

  	//parent process
  	switch (signo)
  	{
  		
      	case SIGUSR2: //HstSrv want me to stop
    		  
    		  dcs_log(0,0,"stop pre status=%d",g_pLinkEntry->iStatus);
    		  if( g_pLinkEntry->iStatus == DCS_STAT_CONNECTED)
    		  {
//	      		g_pLinkEntry->iStatus = DCS_STAT_STOPPED;
	  		    dcs_log(0,0,"comm link will stop");
	  		    TcpipUpdateBCDANet(DCS_STAT_DISCONNECTED);
	  		  }
	  		  else
	  		  	dcs_log(0,0,"link is not DCS_STAT_CONNECTED ,no update bcda net!");
	  		  g_pLinkEntry->iStatus = DCS_STAT_STOPPED;
		      memset(tbuf,0,sizeof(tbuf));
	      	strcpy(tbuf,g_pLinkEntry->caFoldName);
					{

			      time(&t);
			 			tm = localtime(&t);
			      sprintf(buf,"SYSICOMML %s %3.3s %d disconnect[%02d:%02d:%02d]",tbuf,g_pLinkEntry->commNO,g_pLinkEntry->iStatus,tm->tm_hour,tm->tm_min,tm->tm_sec);
						NotifyMoni( buf );
						/*
						if ( g_pLinkEntry->iNotifyFlag == 1 )
					  {
					  	sprintf(buf,"SYSI0001COMML$%s$%3.3s$%d$%d",g_pLinkEntry->caFoldName,g_pLinkEntry->commNO,g_pLinkEntry->iRole,g_pLinkEntry->iStatus);
							if ( strlen(g_pLinkEntry->caNotiFold)>0)
								fold_write(fold_locate_folder(g_pLinkEntry->caNotiFold),g_pLinkEntry->iFid,buf,strlen(buf))  ;	
					  	else
								fold_write(g_pLinkEntry->iApplFid,g_pLinkEntry->iFid,buf,strlen(buf))  ;	
					  }
					  */
					} 
					g_shutdown_flag=1;
					tcp_close_socket(g_iSock);

					exit(signo);
       		break;
    	case 0:
    	case 11:
    	case SIGQUIT:
    	case SIGTERM:
    		  
					if ( g_pLinkEntry->iStatus ==DCS_STAT_CONNECTED )
					{
			
				    g_pLinkEntry->iStatus = DCS_STAT_DISCONNECTED;
						TcpipUpdateBCDANet(DCS_STAT_DISCONNECTED);
						
						memset(tbuf,0,sizeof(tbuf));
						strcpy(tbuf,g_pLinkEntry->caFoldName);
			      time(&t);
			 			tm = localtime(&t);
			      sprintf(buf,"SYSICOMML %s %3.3s %d disconnect[%02d:%02d:%02d]",tbuf,g_pLinkEntry->commNO,g_pLinkEntry->iStatus,tm->tm_hour,tm->tm_min,tm->tm_sec);
						NotifyMoni( buf );
						
						dcs_debug(0,0,"notify end");
						
					}
      		break;

      	default:
          if( g_pLinkEntry->iStatus ==DCS_STAT_CONNECTED)
          {
	      	  g_pLinkEntry->iStatus = DCS_STAT_DISCONNECTED;
						TcpipUpdateBCDANet(DCS_STAT_DISCONNECTED);
				  }
					memset(tbuf,0,sizeof(tbuf));
					strcpy(tbuf,g_pLinkEntry->caFoldName);
          time(&t);
 			    tm = localtime(&t);
      		sprintf(buf,"SYSICOMML %s %3.3s %d disconnect[%02d:%02d:%02d]",tbuf,g_pLinkEntry->commNO,g_pLinkEntry->iStatus,tm->tm_hour,tm->tm_min,tm->tm_sec);

					NotifyMoni( buf );
					
      		break;
      		

//  	  }
  	}
        
    g_pLinkEntry->iStatus = DCS_STAT_DISCONNECTED;
  	tcp_close_socket(g_iSock);
 
    dcs_debug(0,0,"exit system signo =%d",signo);
  	exit(signo);

}

// 函数名称  TcpipOpenLog
// 参数说明：
//          无
// 返 回 值：
//          成功返回0,否则-1
// 功能描述：
//          1.获得系统主目录,将之放在全局变量g_pcIseHome
//          2.设置日志文件和日志方式

int TcpipOpenLog( char * buf)
{
  	char   caLogName[256];
 		char logfile[256];
  	
    memset(caLogName,0,sizeof(caLogName));
  	sprintf(caLogName,"log/hst%s.log",buf);
    if(u_fabricatefile(caLogName,logfile,sizeof(logfile)) < 0)
    {
//    		  printf(stderr,"can not open log file:[%s]",caLogName);
    	    return -1;
    }
//    printf(stderr," open log file:[%s] succ!",caLogName);
  	return  dcs_log_open(logfile, buf);
}


// 函数名称  GetLinkInfo
// 参数说明：
//          pcFoldName:文件夹的名称,用作查询条件
// 返 回 值：
//          若找到成功,则返回对应信息槽位的指针,否则NULL
// 功能描述：
//          1.连接共享内存HSTCOM
//          2.在数组LinkArray[]中查找和给定文件夹名称匹配的项

struct linkinfo * GetLinkInfo(char *pcFoldName ,char *commNO)
{
  	int    i,s;
//  	char mBuf[40];

  	
//  	memset(mBuf,0,sizeof(mBuf));
        s=0;
  	//连接共享内存HSTCOM
  	pLinkCtrl = (struct hstcom *)shm_connect(DCS_HSTCOM_NAME, NULL);
  	if (pLinkCtrl == NULL)
  	{
    		dcs_log(0,0,"<Tcpip--%s> cannot connect share memory %s!",g_caIdent,DCS_HSTCOM_NAME);
    		
    		
    		return NULL;
  	}

  	for (i=0; i < pLinkCtrl->iLinkCnt; i++)
  	{
    		if (!(pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (u_stricmp(pLinkCtrl->linkArray[i].caFoldName,pcFoldName) == 0 && memcmp(pLinkCtrl->linkArray[i].commNO,commNO,3)==0)
     		{
 		  		  dcs_log(0,0,"<Tcpip--%s>  %s sock[%d]!",g_caIdent,pLinkCtrl->linkArray[i].caFoldName,i);
   			
     				break;
     		}
     		else
      			continue;
    		
  	}
    if ( i>=pLinkCtrl->iLinkCnt)
        	return NULL;
  	return &pLinkCtrl->linkArray[i];
	
}

// 函数名称  MakeConnection
// 参数说明：
//          无
// 返 回 值：
//          成功返回0,否则返回套接字的描述符
// 功能描述：
//          建立和对方的套接字连接

int MakeConnection( struct linkinfo *spLinkEntry)
{
int flags;

  	if (spLinkEntry->iRole == DCS_ROLE_PASSIVE ||spLinkEntry->iRole==DCS_ROLE_SIMPLEXSVR)
  	{
    		//we are the server end, create a listening socket and
    		//wait to accept the clients' connection request
    		int   iListenSock, iConnSock, iCliAddr;
    		int   iCliPort;
    		struct in_addr in;
//				dcs_debug(0,0, "begin open socket");
    		iListenSock = tcp_open_server(NULL,spLinkEntry->sLocalPort);
    		if (iListenSock < 0)
    		{
      			dcs_log(0,0,"<Tcpip--%s> tcp_open_server() failed: %s! port:%d",g_caIdent,strerror(errno),spLinkEntry->sLocalPort);
      			
      			
      			return -1;
    		}
				dcs_debug(0,0, " open socket succ");
    		spLinkEntry->iStatus = DCS_STAT_LISTENING;

    		for (;;)
    		{
      			//waiting for clients
      			iConnSock = tcp_accept_client(iListenSock,&iCliAddr, &iCliPort);
      			if (iConnSock < 0)
      			{
        			dcs_log(0,0,"<Tcpip--%s> tcp_accept_client() failed: %s!",\
                           		g_caIdent,strerror(errno));
        			
        			tcp_close_socket(iListenSock);
        			return (-1);
      			}
						dcs_debug(0,0, "accept socket succ sock=[%d]",iConnSock);
      			//validate the client
      			in.s_addr = iCliAddr;
      			if (1)
      			{
        			int   iAddr, sPort;
              if ( strcmp(spLinkEntry->caRemoteIp,"null")==0 )
                     iAddr=INADDR_NONE;
              else
        				iAddr = tcp_pton(spLinkEntry->caRemoteIp);
        			sPort = spLinkEntry->sRemotePort;

        			if ( ((sPort >0) && (sPort != iCliPort)) || ((iAddr != INADDR_NONE) && (iCliAddr != iAddr)))
        			{
          			
          				

          				dcs_log(0,0,"<Tcpip--%s> accept a conn from %s:%d,but refused!",\
                    				g_caIdent,inet_ntoa(in),iCliPort);
          				
          				tcp_close_socket(iConnSock);
          				continue;  //accept() again
        			}
      			}
      			//the connection established successfully
      			spLinkEntry->iStatus = DCS_STAT_CONNECTED;
      			spLinkEntry->iRemoteIp0   = iCliAddr;
      			spLinkEntry->sRemotePort0 = iCliPort;
            dcs_log(0,0,"<Tcpip--%s> accept a conn from %s:%d!",\
                    				g_caIdent,inet_ntoa(in),iCliPort);
      			tcp_close_socket(iListenSock);
//      			flags=fcntl(iConnSock,F_GETFL);
//            fcntl(iConnSock,F_SETFL,flags|O_NDELAY);
      			return (iConnSock);
    		}//for(;;)
  	} //if(g_pLinkEntry->iRole == DCS_ROLE_PASSIVE)
  	else
  	{
    		//we are the client end, create a socket and
    		//try to connect the server
    		int   iConnSock,i;
		    			dcs_debug(0,0,"<Tcpip--%s> try connect to %s:%d with local port=%d...",\
		                                g_caIdent,\
		                                spLinkEntry->caRemoteIp,\
		                                spLinkEntry->sRemotePort,\
		                                spLinkEntry->sLocalPort);
		    		spLinkEntry->iStatus = DCS_STAT_CALLING;
		
				for( i=0; ;)
				{
		    		iConnSock = tcp_connet_server(spLinkEntry->caRemoteIp,\
		                                  spLinkEntry->sRemotePort,\
		                                  spLinkEntry->sLocalPort);
		    		if (iConnSock < 0)
		    		{
		      			
		      			if ( !(i %20))
		      			{
		      				 dcs_debug(0,0,"<Tcpip--%s> tcp_connet_server() failed:%s! ip=[%s] ,port=[%d]",g_caIdent,strerror(errno),spLinkEntry->caRemoteIp,spLinkEntry->sRemotePort);
		      			}
		      			if ( i == 100) i=0;
		      			i++;
								sleep(2);    			
								continue;
		    		}
						else
								break;
				}
    		//connection established successfully
    		spLinkEntry->iStatus = DCS_STAT_CONNECTED;
//    		flags=fcntl(iConnSock,F_GETFL);
//        fcntl(iConnSock,F_SETFL,flags|O_NDELAY);
    		return(iConnSock);
  	}//else
}

// 函数名称  ReadFromNet
// 参数说明：
//          无
// 返 回 值：
//          always -1
// 功能描述：
//          从网络上读取交易报文,然后将其转交给相应的应用进程

int ReadFromNet( struct linkinfo * spLinkEntry)
{
  struct  tm *tm;  
	time_t  t;
 	char  caBuffer[8192], *ptr,buf[512],tbuf[10];
  	int   nBytesRead, nBytesWritten,iRc;
  	int   i,  iMaxTry = 5 ,itime;
	  int sock;	
//	struct timeval t1;
    sock = g_iSock;
        if ( spLinkEntry==NULL)
        {
        	dcs_log(0,0,"InValid para!");
         	return (-1);
        }
        
        if ( memcmp(spLinkEntry->commType,"bnk",3)!=0&&
           memcmp(spLinkEntry->commType,"trm",3)!=0 &&
           memcmp(spLinkEntry->commType,"fsk",3)!=0 &&
           memcmp(spLinkEntry->commType,"amp",3)!=0 	)
        	        	
        {
        	dcs_log(0,0,"InValid comm type!");
      	 	return (-1);
	      }			

  	
  	if (spLinkEntry->iTimeOut <=0 || spLinkEntry->iTimeOut >500)
       		itime = -1;
  	else
       		itime = spLinkEntry->iTimeOut *1000;
  
    
  	for (;;)
  	{
//  		 memset(caBuffer,0,sizeof(caBuffer));
  		 memcpy(caBuffer,g_iType,4);
  	   ptr = caBuffer+4+sizeof(time_t);
  		 if (  memcmp(spLinkEntry->commType,"trm",3)==0)
  			 nBytesRead = read_msg_from_NAC(sock, ptr,sizeof(caBuffer)-4-sizeof(time_t), itime);
  		 else if (  memcmp(spLinkEntry->commType,"bnk",3)==0)
    			nBytesRead = read_msg_from_net(sock, ptr,sizeof(caBuffer)-4-sizeof(time_t), itime);
    	 else if (  memcmp(spLinkEntry->commType,"amp",3)==0)
    			nBytesRead = read_msg_from_AMP(sock, ptr,sizeof(caBuffer)-4-sizeof(time_t), itime);
    	 else if (  memcmp(spLinkEntry->commType,"fsk",3)==0)
    			nBytesRead = read_msg_from_fsk(sock, ptr,sizeof(caBuffer)-4-sizeof(time_t), itime);

    		if (nBytesRead < 0)
    		{
      			//read error
      			close(sock);
      			dcs_log(0,0,"<Tcpip--%s> read_msg_from_net() failed:[%d]%s!",g_caIdent,errno,strerror(errno));
      			
      			break;
    		}
		   time(&t);
//       spLinkEntry->iRcvTime=t;
    	 if (nBytesRead == 0 ) //a echo test request from client
    	 {
 //       		if ( spLinkEntry->iRole == DCS_ROLE_PASSIVE || spLinkEntry->iRole==DCS_ROLE_SIMPLEXSVR)
        		{
        			
//             if (errno != EINTR && errno != EAGAIN)
  
        		}
//        		dcs_debug(0,0,"recv echo test msg");
        		continue;    //read from net again
    	}
      
  		if (1)  //for debug
  		{
    			ptr[nBytesRead] = 0;
    			if ( nBytesRead >4)
    					dcs_debug(caBuffer+4+sizeof(time_t),nBytesRead,"<Tcpip--%s> Read Net %.4d bytes-->",g_caIdent,nBytesRead);
    			
  		}
		  WriteTraceFile(caBuffer+4+sizeof(time_t), nBytesRead,'R');
      memcpy(caBuffer+4,&t,sizeof(time_t));
    		//forward this message to App
	    for (i=0; i<iMaxTry; i++)
	    {
	
	      			//nBytesRead += 6;
	      if ( memcmp(caBuffer,"ZZZZ",4)==0)
				{
	      	nBytesWritten = fold_write(spLinkEntry->iApplFid,spLinkEntry->iFid,caBuffer+4,nBytesRead+sizeof(time_t));
					nBytesWritten = nBytesWritten+4;	
				}
				else
	      	 nBytesWritten = fold_write(spLinkEntry->iApplFid,spLinkEntry->iFid,caBuffer,nBytesRead+4+sizeof(time_t));
	      if (nBytesWritten <= 0)
	      {
	        	if (errno == FOLD_EMSGOVFLW)
	        	{
						  if (i == iMaxTry)
				   		{
				            sprintf(buf,"ERRC%s%3.3s queue broke  msg num=[%d] ",tbuf,g_pLinkEntry->commNO,fold_count_messags(spLinkEntry->iApplFid));
					        	NotifyMoni( buf );
							      dcs_log(caBuffer+4,nBytesRead,"<Tcpip--%s> discard a msg from net:,errno=%d",g_caIdent,errno);
							      break;
						  }
	          	u_sleep_us(200000); //sleep 100ms
	          	continue;
	        	}
	        	else
	        	{
	          	dcs_log(0,0,"<Tcpip--%s> fold_write() failed: errno=%d",g_caIdent,(errno));
	          	break;
	        	}
	      }
	      else  //write ok
	      {
/*	      	
	      	  dcs_log(0,0,"<ReadFromNet> fold_write() failed,break. errno=%d nBytesWritten=%d ,nBytesRead=%d",errno,nBytesWritten,nBytesRead);
*/	      	 	break;

	      }
	   }
  	}//for(;;)

	  g_shutdown_flag=1;
  	kill(getpid(), SIGTERM);

  	return (-1);
}

// 函数名称  ReadFromIse
// 参数说明：
//          无
// 返 回 值：
//          always -1
// 功能描述：
//          从文件夹读取应用进程产生的交易报文,然后将其发向网络

int ReadFromIse(struct linkinfo * spLinkEntry)
{
  	char  caBuffer[8192];
  	int   nBytesRead, nBytesWritten;
  	int   iOrgFid;
	  struct timeval t1;
    int sock;
    time_t  t;
  	signal(SIGQUIT,quit);
  	sock =g_iSock;
    fold_set_maxmsg(g_pLinkEntry->iFid, 5) ;
  	for (;;)
  	{
    		memset(caBuffer,0,sizeof(caBuffer));
    		nBytesRead=fold_read(g_pLinkEntry->iFid,&iOrgFid,caBuffer,sizeof(caBuffer),TRUE);
    		if (nBytesRead < 0)
    		{
      			//read error
      			dcs_log(0,0,"<Tcpip--%s> fold_read() failed:%d",g_caIdent,(errno));
      			
      			break;
    		}
 
        if ( !g_pTime)
        {
 	         
           pthread_mutex_lock(&lock);//锁住全局变量，一次只许一个写线程自加
		    		if ( memcmp(spLinkEntry->commType,"trm",3)==0)
		    			nBytesWritten = write_msg_to_NAC(sock,caBuffer,nBytesRead,1500);
		    		else if ( memcmp(spLinkEntry->commType,"bnk",3)==0)
		    		{
		    				if (nBytesRead>4)
		    					nBytesWritten = write_msg_to_net(sock,caBuffer,nBytesRead,1500);
		    				else
		    				{
		    						nBytesWritten = write_msg_to_net(sock,caBuffer,0,1500);
		    						nBytesWritten=nBytesRead;
		    				}
		    		}
		    		else if ( memcmp(spLinkEntry->commType,"amp",3)==0)
		    		  nBytesWritten = write_msg_to_AMP(sock,caBuffer,nBytesRead,1500);
		    		else if ( memcmp(spLinkEntry->commType,"fsk",3)==0)
		    		  nBytesWritten = write_msg_to_fsk(sock,caBuffer,nBytesRead,1500);
		    		pthread_mutex_unlock(&lock);
		    		if (nBytesWritten < nBytesRead)
		    		{
		      			dcs_log(0,0,"<ReadFromIse Tcpip--%s> write_msg_to_net() failed:%s",g_caIdent,strerror(errno));
		      			close(sock);
		      			break;
		    		}
    		}
    		else
    		{
    			  if ( IsTimeOut(caBuffer,caBuffer+2) )
		        {
		        	 dcs_log(caBuffer,nBytesRead,"数据在本系统里处理超时，不作转发");
		        	 continue;
		        }
           pthread_mutex_lock(&lock);//锁住全局变量，一次只许一个写线程自加

    				if ( memcmp(spLinkEntry->commType,"trm",3)==0)
		    			nBytesWritten = write_msg_to_NAC(sock,caBuffer+8,nBytesRead-8,1500);
		    		else if ( memcmp(spLinkEntry->commType,"bnk",3)==0)
		    		{
		    				if ( nBytesRead-8 >4)
		    						nBytesWritten = write_msg_to_net(sock,caBuffer+8,nBytesRead-8,1500);
		    				else
		    				{
		    							nBytesWritten = write_msg_to_net(sock,caBuffer+8,0,1500);
		    							nBytesWritten=nBytesRead-8;
		    				}
		    		}
		    		else if ( memcmp(spLinkEntry->commType,"amp",3)==0)
		    		  nBytesWritten = write_msg_to_AMP(sock,caBuffer+8,nBytesRead-8,1500);
		    		else if ( memcmp(spLinkEntry->commType,"fsk",3)==0)
		    		  nBytesWritten = write_msg_to_fsk(sock,caBuffer+8,nBytesRead-8,1500);
		    		pthread_mutex_unlock(&lock);
		    		if (nBytesWritten < nBytesRead-8)
		    		{
		      			
		      			dcs_log(0,0,"<ReadFromIse Tcpip--%s> write_msg_to_net() failed:%s",g_caIdent,strerror(errno));
		      			
		      			break;
		    		}
		    		

		
    		}
    		if (nBytesWritten > 4)  //for debug
    		{
      			if ( g_pTime)
      			   dcs_debug(caBuffer+8,nBytesWritten,"<Tcpip--%s> write Net %.4d bytes-->",g_caIdent,nBytesWritten);
      			else
      				 dcs_debug(caBuffer,nBytesWritten,"<Tcpip--%s> write Net %.4d bytes-->",g_caIdent,nBytesWritten);
						WriteTraceFile(caBuffer, nBytesWritten,'S');
    		}
        if ( g_pLinkEntry->iRole == DCS_ROLE_SIMPLEXCLI )
        {
        	 time(&t);
           spLinkEntry->iRcvTime=t;
        }
    		//SendToMonitor(caBuffer,nBytesRead);

  	}//for(;;)
//		TcpipUpdateBCDANet( DCS_STAT_DISCONNECTED);
		tcp_close_socket(sock);
//  	spLinkEntry->iStatus = DCS_STAT_DISCONNECTED;
  	fold_purge_msg(spLinkEntry->iFid);
  	dcs_log(0,0,"ReadFromIse thread exit");
/*  	if(g_pLinkEntry->caCheckFlag)
		{
			if( pthread_kill(sthread1,0)!=ESRCH)
			   pthread_kill(sthread1,SIGQUIT);
			
	  }
	 
	  	
	  if ( g_pLinkEntry->iRole != DCS_ROLE_SIMPLEXCLI )
	  {   dcs_debug(0,0,"checking master thread!");
	  	  if( pthread_kill(gthread,0)!=ESRCH)
  	    {
  	    	  dcs_debug(0,0,"killing master thread!");
  	    	  pthread_kill(gthread,SIGTERM);
  	    }
//  	if ( g_pLinkEntry->iRole != DCS_ROLE_SIMPLEXCLI )
       
  	    sleep(1);
  	
  	}
 */
  	g_shutdown_flag=1;
  	kill(getpid(), SIGTERM);
  	return (-1);
}


// 函数名称  NotifyHstSvr
// 参数说明：
//          stat：网络连接状态，在本进程中始终为DCS_STAT_CONNECTED
// 返 回 值：
//
// 功能描述：
//          当链路建立后发生改变时通知HstSvr

int NotifyHstSvrConn(int stat)
{
  	struct commnotify stNoti;
  	int    nWritten;

  	
  	
  	dcs_debug(0,0,"<Tcpip--NotifyHstSvrConn> fold name=[%s] Link status = [%d]",g_pLinkEntry->caFoldName,stat);
    	

  	if (stat != DCS_STAT_CONNECTED && stat != DCS_STAT_DISCONNECTED)
    		return (-1);

  	sprintf(stNoti.caMsgType ,"%.4d", DCS_LINKESTAB_NOTI);
  	memcpy(stNoti.caFoldName,g_pLinkEntry->caFoldName,4);
  	stNoti.iStatus  = stat;

	dcs_debug(0,0, "<Tcpip--NotifyHstSvrConn> Before call fold_write!");

  	nWritten = fold_write(g_iHstSrvFoldId,-1,&stNoti,sizeof(stNoti));
  	if (nWritten < sizeof(stNoti))
  	{
    		dcs_log(0,0,"<Tcpip--%s> fold_write() failed:%d",g_caIdent,(errno));
    		
    		return (-1);
  	}
  	
  	dcs_debug(0,0, "<Tcpip--NotifyHstSvrConn> Write notify message to hstsvr successfully!");

  	return (0);
}

/* Mary add begin, 2001-7-9 */
/*
  函数名称：TcpipUpdateBCDANet
  参数说明：
           stat：网络连接状态，在本进程中始终为DCS_STAT_CONNECTED
  返 回 值：
           0：成功
           -1：失败
  功能描述：
           当链路建立后直接修改共享内存BCDA中对应通讯进程的状态
  （原来系统中，成员行连接上后，通讯进程向通讯监控进程发送消息要求修改BCDA中通讯进程的状态，
  但是监控进程可能由于进程注册等未完成，接收不到该消息，消息队列中也没有，造成ISE认为成员行
  未连接上，不能做交易，所以增加该函数，直接修改BCDA中通讯进程的状态）
*/

int TcpipUpdateBCDANet(int iStat)
{
  	struct BCDA 	*pBCDA;
  	int		i= -1;

  	

	//连接共享内存BCDA
  	g_pcBcdaShmPtr = (char *)shm_connect(DCS_BCDA_NAME,NULL);
  	if (g_pcBcdaShmPtr == NULL)
  	{
    		dcs_log(0,0,"<TcpipUpdateBCDANet> cannot connect shared memory %s!",DCS_BCDA_NAME);
    		
    		return (-1);
  	}

  	//determine which issuer timeout
  	i = LocateBankByName( g_pLinkEntry->caFoldName );

  	if (i < 0)  //not found
  	{
  		dcs_log(0,0,"<Tcpip--TcpipUpdateBCDANet> fold name [%s] is not found in BCDA!",g_pLinkEntry->caFoldName);
    	shm_detach((char *)g_pcBcdaShmPtr);	
    		return (-1);
    }

  	pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;

  	//update the net status for this entry
  	if (iStat == DCS_STAT_DISCONNECTED)
  	{
//    		pBCDA[i].cNetFlag --; //decrease the link count
    	if ( g_pLinkEntry->iRole == DCS_ROLE_SIMPLEXCLI && pBCDA[i].SendFlag >0)
    		pBCDA[i].SendFlag--;
    	else if ( g_pLinkEntry->iRole == DCS_ROLE_SIMPLEXSVR  && pBCDA[i].ReadFlag >0 )
    		pBCDA[i].ReadFlag--;
      else
      {
      	 if ( pBCDA[i].SendFlag>0)
      	 pBCDA[i].SendFlag--;
      	 if( pBCDA[i].ReadFlag >0)
      	 pBCDA[i].ReadFlag--;
      }
   		if (pBCDA[i].ReadFlag <= 0 || pBCDA[i].SendFlag<=0)
      			pBCDA[i].cNetFlag = 0;
/*
    		pBCDA[i].cNetFlag = 0;  //always set to 0
    		if (pBCDA[i].cNetFlag == 0)
      			pBCDA[i].cAppFlag = 0;
*/
  	}
  	else if (iStat == DCS_STAT_CONNECTED)
  	{
   		if ( g_pLinkEntry->iRole == DCS_ROLE_SIMPLEXCLI )
    		pBCDA[i].SendFlag++;
    	else if ( g_pLinkEntry->iRole == DCS_ROLE_SIMPLEXSVR )
    		pBCDA[i].ReadFlag++;
      else
      {
      	 pBCDA[i].SendFlag++;
      	 pBCDA[i].ReadFlag++;
      }
   		if (pBCDA[i].ReadFlag > 0 && pBCDA[i].SendFlag >0)
      			pBCDA[i].cNetFlag = 1;
//    		pBCDA[i].cNetFlag ++; //increase the link count

 //   		pBCDA[i].cNetFlag = 1;  //always set to 1
  	} 
  	else
  		dcs_log(0,0,"[%s] link not update! stat=[%d]",g_pLinkEntry->caFoldName,iStat);   
//   memcpy(g_iType,pBCDA[i].caType,4);
	shm_detach((char *)g_pcBcdaShmPtr);

  	return (0);
}  

int NotifyMoni ( char *buf )
{
	int qid,iRc;
	struct msqid_ds MsgQid_Buf;
	
//	dcs_debug(0,0,"entry notify"); 
	qid=queue_connect("monitor");
	if ( qid <=0 )
	{
	 	dcs_log(0,0,"File:%s,Line:%d: Connect Moni Error ",__FILE__,__LINE__);
		
	     	return -1;
	}
	iRc = msgctl(qid,IPC_STAT,&MsgQid_Buf);
	if(iRc < 0)
	{
		dcs_log(0,0,"File:%s,Line:%d: Get Stat Error,Error Code = %d",__FILE__,__LINE__,iRc);
		
		return -1;
	}
	if(MsgQid_Buf.msg_qnum > 40)
	{
		dcs_log(0,0,"File:%s,Line:%d: Send Moni Message limited  ",__FILE__,__LINE__);
		
	    	return -1;
	}	
	queue_send( qid, buf, strlen(buf),0);
	dcs_debug(0,0," finilsh send data =[%s]",buf);
	return 1;
	
}

int OpenTrceFile(char cDirect)
{
	int iIseLog,IsLog;
	char	tracefile[128],caBuf[64];

	memset(tracefile,0,sizeof(tracefile));
	memset(caBuf,0,sizeof(caBuf));
	//判断是否记录流水
	IsLog = GetTraceOpenFlag();

	if(IsLog != 1)
		return -1;
	
	GetSystemDate(caBuf);
	
	if(cDirect=='R' || cDirect=='r')
	{
		sprintf(tracefile,"%s/data/%sRecv%s",getenv("ICS_HOME"),g_caIdent,caBuf);
	}
	else if(cDirect=='S' || cDirect=='s')
	{
		sprintf(tracefile,"%s/data/%sSend%s",getenv("ICS_HOME"),g_caIdent,caBuf);
	}
	else
	{
		dcs_log(0,0,"File:%s,Line:%d:OpenTrceFile cDirect[%c] Error  ",
			__FILE__,__LINE__,cDirect);
		
	    	return -1;
	}

	iIseLog = open(tracefile,O_WRONLY|O_APPEND|O_CREAT,0666);
	if(iIseLog < 0)
		return -1;
	else
		return iIseLog;
}

int WriteTraceFile(char * msgTrace,int nbytes,char cDirect)
{

    struct  tm *tm;
    time_t  t;
    char    caBuf[64];
	int  tracefd    = -1 ,iRetry =0;

	
	tracefd = OpenTrceFile(cDirect);
    if(tracefd < 0)
 		return -1;

    //format the message
    time(&t);
    tm = localtime(&t);

    memset(caBuf,0,sizeof(caBuf));
    sprintf(caBuf,"%4d/%02d/%02d %02d:%02d:%02d %c(%.4dbytes) ",
            tm->tm_year+1900,
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec,
            cDirect,
            nbytes);
  	while (lockf(tracefd,F_LOCK,0) == -1) 
  	{
	      usleep(10000);
	      if ( (iRetry ++) > 40 ) 
	      {
	          close(tracefd);
	          return -1;
	      }
    }
    write(tracefd, caBuf, strlen(caBuf));
    write(tracefd, msgTrace, nbytes);
    write(tracefd, "||\n", 3);
		close(tracefd);
    return nbytes;

}


int GetTraceOpenFlag()
{
    int pos,iBnkNum=0,i;
//    struct BCDA *pBCDA;
    int Val;

  	if (g_pcBcdaShm == NULL)
  	{
    		dcs_log(0,0,"<GetTraceOpenFlag> cannot connect shared memory %s!",DCS_BCDA_NAME);
    		
    		return(-1);
  	}

//  	pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;

    iBnkNum = g_pcBcdaShm->iBankNum;
		
    for(i=0; i < iBnkNum; i++)
    {
        if(memcmp(g_caIdent, g_pcBcdaShm->stBcda[i].caBankName,4) == 0)
        {
							Val = (int)g_pcBcdaShm->stBcda[i].cAppFlag;
							return Val;
        }
    }
	return -1;
}

int IsTimeOut(char * pDay, char * pTime)
{
     struct  tm *tm;   time_t  t;
     char caDay[3], caHour[3],caMin[3],caSec[3];
     long t1,t2;	
   
     
     if ( strlen(pDay)<2 || strlen(pTime) <6)
     {
				dcs_log(0,0,"无效的输入参数!");
        return 1; 
     }
     dcs_debug(0,0,"day=[%2.2s] time=[%6.6s]",pDay,pTime);
     memset(caDay,0,sizeof(caDay));
     memset(caHour,0,sizeof(caHour));
     memset(caMin,0,sizeof(caMin));
     memset(caSec,0,sizeof(caSec)); 
     memcpy(caDay,pDay,2);
     memcpy(caHour,pTime,2);
     memcpy(caMin,pTime+2,2);
     memcpy(caSec,pTime+4,2);

     time(&t);
     tm = localtime(&t);
     t1=tm->tm_hour*3600+tm->tm_min*60+tm->tm_sec;
     t2=atoi(caHour)*3600+atoi(caMin)*60+atoi(caSec);
     if(atoi(caDay)==tm->tm_mday)
     {
        if( t1 -t2 <15)
                return 0;
        else 
						return 1;
     }else 
     {
         if( t1 >=15 )
							return 1;
         else
							return 0;
     }
     
}
void SendCheckNetMsg( void * link)
{
    char ptr[100];
    int i,s=-1,ret;
    time_t t;
    signal(SIGPIPE, SIG_IGN);
    signal(SIGQUIT,quit);
    i = ((struct linkinfo * )link)->iTimeOut -3;
    if ( i <=0 ) i=3;
    time(&t);
    ((struct linkinfo * )link)->iRcvTime=t;
    while ( i >0 )
    {
    	if ( g_shutdown_flag ) break;
		  
			if ( g_iSock >=0)
			{
	//			write(g_iSock, "0000",4);
	        time(&t);
	        if( (t- ((struct linkinfo * )link)->iRcvTime) < i)
	        {    
	        	  if ( g_pLinkEntry->iRole == DCS_ROLE_SIMPLEXCLI )
						  {
						  	fd_set sock_set;
						  	struct timeval  tmval, *tmval_ptr;
						  	FD_ZERO(&sock_set);
				        FD_SET(g_iSock, &sock_set);
				        tmval_ptr = &tmval;
        				tmval_ptr->tv_sec  = i-(t- ((struct linkinfo * )link)->iRcvTime);
        				tmval_ptr->tv_usec = 0;
						  	ret=select(g_iSock+1,&sock_set,NULL,NULL,tmval_ptr);
						  	if( ret <0 ) continue;
						  	else if( ret ==0 ) continue;
						  	else
						  	{
						  		 ret=read(g_iSock,ptr,sizeof(ptr));
						  		 if ( ret <=0 ) 
						  		 {
						  		    close(g_iSock);
						  		 		break;
						  		 }
//						  		 ((struct linkinfo * )link)->iRcvTime=time(NULL);
						  	}
						  }
	        	  else
	        	  	 sleep(i-(t- ((struct linkinfo * )link)->iRcvTime));
	        	  continue;
	        }
	        	  
	         pthread_mutex_lock(&lock);//锁住全局变量，一次只许一个写线程自加
	        if (  memcmp(((struct linkinfo * )link)->commType,"bnk",3)==0)
			        s= write(g_iSock,"0000",4);
			    else if (  memcmp(((struct linkinfo * )link)->commType,"trm",3)==0)
			        s= write(g_iSock,"\x00\x00",2);
	        if( s <0)
	        {
	       		  i=-1;
	       		  g_shutdown_flag =1;
	       		  dcs_log(0,0,"send echo test fail!");
	       		  tcp_close_socket(g_iSock);
	       		  pthread_mutex_unlock(&lock);
	       		  continue;
	       		  
	        }
//	        dcs_debug(0,0,"send echo test msg");
	       
			    ((struct linkinfo * )link)->iRcvTime=t;
			    pthread_mutex_unlock(&lock);
			    
			    continue;
	//			dcs_debug(0,0,"send check net msg ! sock id=[%d]",g_iSock);
			}
			break;
	  }
	  dcs_log(0,0,"SendCheckNetMsg thread exit");
	  TcpipExit(0);
//	  if ( g_pLinkEntry->iRole == DCS_ROLE_SIMPLEXCLI)
//	        TcpipExit(0);
//	  pthread_exit(0);
}
