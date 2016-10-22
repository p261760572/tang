 #define _MainProg

// 程序名称：secuSrv.c
// 功能描述：
//          安全管理系统服务通讯程序
// 函数清单：
//           main()

	
#include  "dccglobl.h"
#include  "dccpgdef.h"
#include  "dccdcs.h"
#include  "dccsock.h"
#include  "secuSrv.h"
//#include  "queue.h"
#include <pthread.h>

#define DCS_SEC_LOG  "secusrv.log"
#define DCS_SEC_NAME "secsrv.conf"


static   char            *g_pcIseHome = NULL;
static   int              g_SecShmId = -1;
static   char             g_caIdent[64];
static   int g_qid,g_qid1;
// Functions declaration
void SecSrvExit(int signo);
int SecSrvOpenLog();
void MakeConnection(struct secLinkInfo* pLink);
int MakeConnection1(struct secLinkInfo* pLink);
static int LoadCommLnk();

static  int g_LinkCnt,gs_myfid;
struct SECSRVCFG * g_pSecCfg;
int InitShmCfg();
void DoServices();
void TransProc ();
struct secLinkInfo *GetFailTcpLink( );
struct secLinkInfo *GetFreeTcpLink( );
int DeleteLink(struct secLinkInfo *pLink);
/*
  函数名称：main
  参数说明：
           argv[0] 可执行程序名称
           argv[1] 和本通信程序相连的文件夹名
  返 回 值：
           无
  功能描述：
           1.接收来自终端管理系统客户端的报文,并转交给相应的应用进程
           2.接收本地应用的报文,并将其发往网络
*/

int main(int argc, char *argv[])
{
	int i;
	struct monilinkinfo *arg;
    pthread_t sthread;
  	catch_all_signals(SecSrvExit);


  	//prepare the log stuff
  	if (0 > SecSrvOpenLog())
    		exit(0);
   i=0;
   while(i<3)
   { 
    if (fold_initsys() < 0)
    {
           if( i<2)
           {
              usleep(500000);
              i++;continue;
           }  

           dcs_log(0,0," cannot attach to FOLDER !",g_caIdent);

            exit(0);
    }
    break;
  }
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",strerror(errno),__FILE__,__LINE__);
        exit(0);
    }
    gs_myfid = fold_create_folder("SECSRV");
    if(gs_myfid < 0)
        gs_myfid = fold_locate_folder("SECSRV");
    if(gs_myfid < 0)
    {
        dcs_log(0,0,"cannot create folder 'SECSRV':\n"  );
        exit(0);
    }
    if(fold_get_maxmsg(gs_myfid) <2)
            fold_set_maxmsg(gs_myfid, 5) ;

  	if ( (g_LinkCnt=LoadCommCnt())<=0)
  	{
  		  dcs_log(0,0, "<SecSrv LoadCommLnk> Failure to load secsrvlnk.ini !" );
    		exit(0);
  	}
  	g_pSecSrvShmPtr = shm_create("TCSECSRV", g_LinkCnt *sizeof(struct secLinkInfo) +sizeof(struct SECSRVCFG)+3,&g_SecShmId);
  		
  	if ( g_pSecSrvShmPtr == NULL)
  	{
	  		dcs_log(0,0, "Can not create Shm name=TCSECSRV !" );
	  		exit(0);
  	}
  	memset(g_pSecSrvShmPtr,0,g_LinkCnt *sizeof(struct secLinkInfo) +sizeof(struct SECSRVCFG)+3);
  	g_pSecCfg=(struct SECSRVCFG*)g_pSecSrvShmPtr;
  	g_pSecCfg->maxLink = g_LinkCnt;
  	g_pSecCfg->pNextFreeSlot=0;
  	dcs_debug(0,0,"sem id =%d",g_pSecCfg->secSemId);
  	g_pSecCfg->secSemId=sem_create("TCSECSEM", 2);
  	if ( g_pSecCfg->secSemId <0)
  	{
  		dcs_log(0,0, "Can not create Sem name=TCSECSEM !" );
  		goto lblExit;
  	}
  	dcs_debug(0,0,"sem id =%d",g_pSecCfg->secSemId);
  	dcs_debug(0,0,"begining Init SHM !");
  	if ( 0>= InitShmCfg() )
  	{
  		dcs_log(0,0, "Can not Init SHM config  !" );
  		goto lblExit;;
  	}
    dcs_debug(0,0,"Init SHM succ!");
  	
  	for ( i =0; i< g_pSecCfg->maxLink ;i++)
  	{
  			if ( g_pSecCfg->secLink[i].lFlags == DCS_FLAG_USED)
  			{
						g_pSecCfg->secLink[i].iNum=i;
						if ( pthread_create(&sthread, NULL,( void * (*)(void *))MakeConnection,&g_pSecCfg->secLink[i]) !=0)
						{
								dcs_log(0,0, "Can not create thread ");
								goto lblExit;
						}
						pthread_detach(sthread);
				}
	  }
	  for ( i =0 ;i < g_pSecCfg->maxLink; i++)
	  {
	  				if ( pthread_create(&sthread, NULL,( void * (*)(void *))TransProc,NULL) !=0)
						{
								dcs_log(0,0, "Can not create thread ");
								goto lblExit;
						}
						pthread_detach(sthread);
	  }
	  dcs_debug(0,0,"create enctry process group succ!");
  	g_pSecCfg->srvStatus=1;
  		
    DoServices();
   
  

lblExit:
//	PidUnregister();
  	SecSrvExit(0);
}//main()

/*
  函数名称：MoniTcpipExit
  参数说明：
           signo：捕获的信号量
  返 回 值：
           无
  功能描述：
           当捕获的信号为SIGTERM时，该函数释放资源后终止当前进程；否则不做任何处理
*/

void SecSrvExit(int signo)
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
      case 13:
      	  return;
    	default:
      		
	  	      break;
			 
		}
  	g_pSecCfg->srvStatus =0;
  	
  	for ( i=0; i< g_pSecCfg->maxLink;i++)
  	{
  			if( !(g_pSecCfg->secLink[i].lFlags &DCS_FLAG_USED)) 
		 			continue;
		 		if ( g_pSecCfg->secLink[i].iSocketId >=0)
		 			close(g_pSecCfg->secLink[i].iSocketId);
  	}
    sem_delete(g_pSecCfg->secSemId);
    shm_delete(g_SecShmId);
    queue_delete(g_qid);
    queue_delete(g_qid1);
  	exit(signo);
}

/*
  函数名称：SecSrvOpenLog
  参数说明：
           无
  返 回 值：
           0：成功
           -1：失败
  功能描述：
           1.获得系统主目录,将之放在全局变量g_pcIseHome
           2.读取系统配置文件system.conf
           3.设置日志文件和日志方式
*/

int SecSrvOpenLog()
{
  	char   caLogName[256];
    char logfile[400];
  
    memset(caLogName,0,sizeof(caLogName));
    strcpy(caLogName,"log/encrypt.log");
    if(u_fabricatefile(caLogName,logfile,sizeof(logfile)) < 0)
    {
            return -1;
    }
    dcs_log_open(logfile, "Encrypt");

 	return 0;
}


/*
  函数名称：MakeConnection
  参数说明：
           无
  返 回 值：
           >0：成功，返回连接上的socket描述符
           -1：失败
  功能描述：
           根据共享内存中的配置信息判断：
           若通讯进程为服务器，则侦听本地端口号，一旦对方连接上，则返回该链接的socket描述符；
           若通讯进程为客户端，则主动连接对方服务器，连接成功后，返回该链接的socket描述符。
*/

void MakeConnection(struct secLinkInfo* pLink )
{
  	
  	
  	char buffer[100];
  	int i;
  	int   iConnSock;
			
	
	dcs_debug(0,0,"thread begin building tcp link ,sem_id=%d",g_pSecCfg->secSemId);
	 sem_lock( g_pSecCfg->secSemId, 1, 1);
	 dcs_debug(0,0,"lock sem succ!");
	 if ( pLink->iStatus == DCS_STAT_CALLING)
	 {
	 			sem_unlock( g_pSecCfg->secSemId, 1, 1); 
	 			dcs_debug(0,0,"pLink status=%d, calling",pLink->iStatus);
	 			return;
	 	}
	 if ( pLink->iSocketId >=0)
	 {
	 			close(pLink->iSocketId);
	 			pLink->iSocketId = -1;
	 }
	 pLink->iStatus = DCS_STAT_CALLING;
	 sem_unlock( g_pSecCfg->secSemId, 1, 1); 
	 i=0;
	  for ( ; ; )
	  {
	  	   if ( pLink->iStatus != DCS_STAT_CALLING )
	  	   	        return;
	  	    iConnSock = tcp_connet_server(pLink->caRemoteIp,pLink->iRemotePort,pLink->iLocalPort);
          if (iConnSock < 0)
          {
          	      if (!(i % 20))  
                  dcs_debug(0,0,"<Tcpip--%s> tcp_connet_server() failed:%s! ip=%s, port=%d",g_caIdent,strerror(errno),pLink->caRemoteIp,pLink->iRemotePort);

                  sleep(3);
                  if ( i >=60000 )
                      i=0;
                  i++;
                  
                  continue;
					}
          else
                  break;
           
	  }
	  pLink->iSocketId =  iConnSock;
	  pLink->iStatus = DCS_STAT_CONNECTED;
	  dcs_log(0,0,"TCP connect is ESTABLISHED . ip=%-19.19s,port=%d ，sock id =%d",pLink->caRemoteIp,pLink->iRemotePort,iConnSock);	  
}


int MakeConnection1(struct secLinkInfo* pLink )
{
  	
  	
  	char buffer[100];
  	int i;
  	int   iConnSock;
			
	
	
	 sem_lock( g_pSecCfg->secSemId, 1, 1);
	 dcs_debug(0,0,"lock sem succ!");
	 if ( pLink->iStatus == DCS_STAT_CALLING)
	 {
	 			sem_unlock( g_pSecCfg->secSemId, 1, 1); 
	 			dcs_debug(0,0,"pLink status=%d, calling",pLink->iStatus);
	 			return;
	 	}
	 if ( pLink->iSocketId >=0)
	 {
	 			close(pLink->iSocketId);
	 			pLink->iSocketId = -1;
	 }
	 pLink->iStatus = DCS_STAT_CALLING;
	 sem_unlock( g_pSecCfg->secSemId, 1, 1); 
	 iConnSock = tcp_connet_server(pLink->caRemoteIp,pLink->iRemotePort,pLink->iLocalPort);
	 if (iConnSock < 0)
	 {
	        dcs_debug(0,0,"<Tcpip--%s> tcp_connet_server() failed:%s! ip=%s, port=%d",g_caIdent,strerror(errno),pLink->caRemoteIp,pLink->iRemotePort);
	        return -1;
	 }
   pLink->iSocketId =  iConnSock;
	 pLink->iStatus = DCS_STAT_CONNECTED;
	 dcs_log(0,0,"<MakeConnection1>TCP connect is ESTABLISHED . ip=%-19.19s,port=%d ，sock id =%d",pLink->caRemoteIp,pLink->iRemotePort,iConnSock);	  
	 return iConnSock;
}

int LoadCommCnt()
{
  	char   caFileName[256];
  	int    iFd=-1,iCommCnt;  
  	
  	
  	//设定commlnk定义文件的绝对路径，并打开
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/config/%s",getenv("ICS_HOME"),DCS_SEC_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log( 0,0, "<HstSrv> open secusrv.conf fail ! ");
				return -1;
  	}

  	
        //读取配置项'comm.max'
        if ( 0 >conf_get_first_number(iFd, "comm.max",&iCommCnt) || iCommCnt < 0 )
        {
                dcs_log(0,0, "<Sec> load configuration item 'comm.max' failed!");
                conf_close(iFd);
                return (-1);
        }

  	conf_close(iFd);
    
  	return(iCommCnt);
}



int InitShmCfg()
{
	
		char   caFileName[256],caBuffer[512];
  	int    iFd=-1,iRc,iCommCnt,iSlotIndex;  
  	struct secLinkInfo * pLink;
  	char *pEncType,*pAddr,*pPort,*pPort1,*pTime,*pNotiFlag,*pNotiFold,*pNum;
  	
  	//设定commlnk定义文件的绝对路径，并打开
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/config/%s",getenv("ICS_HOME"),DCS_SEC_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log( 0,0, "<HstSrv> open secusrv.conf fail ! ");
				return -1;
  	}
    iSlotIndex=0;
    for( iRc = conf_get_first_string(iFd, "comm",caBuffer);\
                iRc == 0 && iSlotIndex < g_pSecCfg->maxLink;\
                iRc = conf_get_next_string(iFd, "comm",caBuffer))
    {
                pNum        = strtok(caBuffer," \t\r");
                pEncType    = strtok(NULL," \t\r");  //通道类型
                pPort1      = strtok(NULL," \t\r");  //本地端口
                pAddr       = strtok(NULL   ," :\t\r");  //地址串
                pPort       = strtok(NULL   ," \t\r");  //端口号  
                pTime        = strtok(NULL   ," \t\r\n");//通信超时检测时间
                pNotiFlag     = strtok(NULL," \t\r"); //通讯链路状态通知标志
        				pNotiFold    = strtok(NULL," \t\r"); //通讯链路状态通知目标文件夹
                if (pEncType==NULL || pAddr==NULL || pTime==NULL || pNotiFlag==NULL || pNotiFold==NULL ||
                     pPort==NULL  )
                {
                        //invalid address:skip this line
                        continue;
                }
                g_pSecCfg->secLink[iSlotIndex].lFlags |= DCS_FLAG_USED;
                g_pSecCfg->secLink[iSlotIndex].iStatus = DCS_STAT_STOPPED;
                strcpy(g_pSecCfg->secLink[iSlotIndex].caRemoteIp,pAddr);
                memcpy(g_pSecCfg->secLink[iSlotIndex].cEncType,pEncType,3);
                if ( u_stricmp(pPort1,"null") ==0)
                   g_pSecCfg->secLink[iSlotIndex].iLocalPort =0;
                else
                	 g_pSecCfg->secLink[iSlotIndex].iLocalPort = atoi(pPort1);
                g_pSecCfg->secLink[iSlotIndex].iRemotePort = atoi(pPort);
                g_pSecCfg->secLink[iSlotIndex].iSocketId=-1;
                g_pSecCfg->secLink[iSlotIndex].iTimeOut=atoi(pTime);
                g_pSecCfg->secLink[iSlotIndex].iNum = atoi(pNum);
                iSlotIndex++;
    }
    return 1;
}
void DoServices()
{
	struct secLinkInfo * pLink;
	pthread_t sthread;
	int fromfid,nread;
	char caBuffer[200];
	while (1)
	{
		
		 
		 nread = fold_read( gs_myfid, &fromfid, caBuffer, sizeof(caBuffer), 1);
		 if ( nread <0)
		 {
		 			dcs_log(0,0,"Monitor Security service exit!");
		 			break;
		 }
		 dcs_debug(caBuffer,nread,"recv msg");
		 if ( memcmp(caBuffer,"DELE",4)==0)
		 {
				 pLink = GetFailTcpLink(atoi(caBuffer+4) );
				 if ( pLink ==  NULL)
				 {
				 			dcs_log(0,0,"该链路不存在，删除失败!");
				 			continue;
				 }
				 DeleteLink(pLink);
		 } else if ( memcmp(caBuffer,"ADDE",4)==0)
		 {
		 		 pLink = GetFailTcpLink(atoi(caBuffer+4) );
				 if ( pLink !=  NULL)
				 {
				 			dcs_log(0,0,"该链路已存在，不能再增加！");
				 			continue;
				 }
				  pLink = GetFreeTcpLink( );
				 if ( pLink ==  NULL)
				 {
				 		dcs_log(0,0,"没有空闲链路，不能建链！");
				 			continue;
				 	}
				 if ( AddLink(caBuffer+4,pLink) ==0)
				 {
				 	  dcs_log(0,0,"不能从参数文件中找到对应链路信息!");
				 	  continue;
				  }
				 if ( pthread_create(&sthread, NULL,( void * (*)(void *))MakeConnection,pLink) !=0)
					{
							dcs_log(0,0, "Can not create thread ");
							break;
					}
					pthread_detach(sthread);
		 }
		 else if ( memcmp(caBuffer,"MODI",4)==0)
		 {
		 		 pLink = GetFailTcpLink(atoi(caBuffer+4) );
				 if ( pLink ==  NULL)
				 {
				 		 dcs_log(0,0,"该链路不存在，不能修改 link id= [%s]",caBuffer+4);
				 			continue;
				 	}
				 	if (StopLink(caBuffer+4) ==0)
				 	{
				 		 continue;
				 	}
				 	ModifyLink(pLink);
				  if ( pthread_create(&sthread, NULL,( void * (*)(void *))MakeConnection,pLink) !=0)
					{
							dcs_log(0,0, "Can not create thread ");
							break;
					}
					pthread_detach(sthread);
		 } else if ( memcmp(caBuffer,"STOP",4)==0)
		 {
		 			 StopLink(caBuffer+4);
		 } else if ( memcmp(caBuffer,"STAR",4)==0)
		 {
		 	  
		 		 pLink = GetFailTcpLink(atoi(caBuffer+4) );
				 if ( pLink ==  NULL)
				 {
				 		dcs_log(0,0,"链路不存在，不能建链！");
				 			continue;
				 	}
				 if ( pLink->iSocketId >=0)
				 {
				 			close( pLink->iSocketId);
				 			pLink->iSocketId =-1;
				 }
				 if ( pthread_create(&sthread, NULL,( void * (*)(void *))MakeConnection,pLink) !=0)
					{
							dcs_log(0,0, "Can not create thread ");
							break;
					}
					pthread_detach(sthread);
		 }
			
		 
	}
}

struct secLinkInfo *GetFailTcpLink( int num )
{
	int i ;
	for ( i=0; i<g_pSecCfg->maxLink;i++)
	{
		 if( !(g_pSecCfg->secLink[i].lFlags & DCS_FLAG_USED)) 
		 			continue;
	   if ( g_pSecCfg->secLink[i].iNum ==  num)
	   			break;
	   
	}
	if ( i == g_pSecCfg->maxLink)
	    return NULL;
	return &g_pSecCfg->secLink[i];
}
struct secLinkInfo *GetFreeTcpLink( )
{
	int i ;
	for ( i=0; i<g_pSecCfg->maxLink;i++)
	{
		 if( g_pSecCfg->secLink[i].lFlags & DCS_FLAG_USED) 
		 			continue;
		 else
		 		break;
	}
	if ( i == g_pSecCfg->maxLink)
	    return NULL;
	return &g_pSecCfg->secLink[i];
}
int StopLink( char *num)
{
	int i;
    for (i=0; i <= g_pSecCfg->maxLink-1; i++)
  	{
    		if (!(g_pSecCfg->secLink[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (atoi(num) !=g_pSecCfg->secLink[i].iNum)
      			continue;
    		
    		break;
  	}

  	if (i>= g_pSecCfg->maxLink)
  	{
    		dcs_log(0,0,"Link '%s' not exists!\n",num);
    		return 0;
  	}
  	sem_lock( g_pSecCfg->secSemId,1,1);

/*  	if(g_pSecCfg->secLink[i].iWorkFlag == 1)
  	{
  			sem_unlock( g_pSecCfg->secSemId,1,1);
//  			sleep(1);
  			dcs_log(0,0,"当前链路正在工作状态，等待一秒\n");
  			return 0;
  	}
*/
//  	if ( g_pSecCfg->secLink[i].iStatus == DCS_STAT_CONNECTED )
//  	{
  			g_pSecCfg->secLink[i].iStatus =DCS_STAT_STOPPED;
  			close(g_pSecCfg->secLink[i].iSocketId);
  			dcs_log(0,0,"close a link , sockid =%d \n",g_pSecCfg->secLink[i].iSocketId);
  			g_pSecCfg->secLink[i].iSocketId=-1;
  			sem_unlock( g_pSecCfg->secSemId,1,1);
//  			dcs_log(0,0,"close a link , sockid =%d \n",g_pSecCfg->secLink[i].iSocketId);
//  	}
    
  	return 1;
}

int ModifyLink(struct secLinkInfo *pLink )
{
	  char   caFileName[256],caBuffer[512];
  	int    iFd=-1,iRc,iCommCnt,iSlotIndex;  
  	char *pEncType,*pAddr,*pPort,*pPort1,*pTime,*pNotiFlag,*pNotiFold,*pNum;
  	
  	//设定commlnk定义文件的绝对路径，并打开
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/config/%s",getenv("ICS_HOME"),DCS_SEC_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log( 0,0, "<HstSrv> open secusrv.conf fail ! ");
				return -1;
  	}
    iSlotIndex=0;
    for( iRc = conf_get_first_string(iFd, "comm",caBuffer);\
                iRc == 0 ;\
                iRc = conf_get_next_string(iFd, "comm",caBuffer))
    {
                pNum        = strtok(caBuffer," \t\r");
                pEncType    = strtok(NULL," \t\r");  //通道类型
                pPort1      = strtok(NULL," \t\r");  //本地端口
                pAddr       = strtok(NULL   ," :\t\r");  //地址串
                pPort       = strtok(NULL   ," \t\r");  //端口号  
                pTime        = strtok(NULL   ," \t\r\n");//通信超时检测时间
                pNotiFlag     = strtok(NULL," \t\r"); //通讯链路状态通知标志
        				pNotiFold    = strtok(NULL," \t\r"); //通讯链路状态通知目标文件夹
                if (pEncType==NULL || pAddr==NULL || pTime==NULL || pNotiFlag==NULL || pNotiFold==NULL ||
                     pPort==NULL  )
                {
                        //invalid address:skip this line
                        continue;
                }
                if ( atoi(pNum) != pLink->iNum) continue;
                iSlotIndex =1;
                pLink->lFlags |= DCS_FLAG_USED;
                pLink->iStatus = DCS_STAT_STOPPED;
                strcpy(pLink->caRemoteIp,pAddr);
                memcpy(pLink->cEncType,pEncType,3);
                if ( u_stricmp(pPort1,"null") ==0)
                   pLink->iLocalPort =0;
                else
                	 pLink->iLocalPort = atoi(pPort1);
                pLink->iRemotePort = atoi(pPort);
                pLink->iSocketId=-1;
                pLink->iTimeOut=atoi(pTime);

    }
    return iSlotIndex;
}

int  AddLink(char *caNum ,struct secLinkInfo *pLink )
{
	  char   caFileName[256],caBuffer[512];
  	int    iFd=-1,iRc,iCommCnt,iSlotIndex;  
  	char *pEncType,*pAddr,*pPort,*pPort1,*pTime,*pNotiFlag,*pNotiFold,*pNum;
  	
  	//设定commlnk定义文件的绝对路径，并打开
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/config/%s",getenv("ICS_HOME"),DCS_SEC_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log( 0,0, "<HstSrv> open secusrv.conf fail ! ");
				return -1;
  	}
    iSlotIndex=0;
    for( iRc = conf_get_first_string(iFd, "comm",caBuffer);\
                iRc == 0 ;\
                iRc = conf_get_next_string(iFd, "comm",caBuffer))
    {
                pNum        = strtok(caBuffer," \t\r");
                pEncType    = strtok(NULL," \t\r");  //通道类型
                pPort1      = strtok(NULL," \t\r");  //本地端口
                pAddr       = strtok(NULL   ," :\t\r");  //地址串
                pPort       = strtok(NULL   ," \t\r");  //端口号  
                pTime        = strtok(NULL   ," \t\r\n");//通信超时检测时间
                pNotiFlag     = strtok(NULL," \t\r"); //通讯链路状态通知标志
        				pNotiFold    = strtok(NULL," \t\r"); //通讯链路状态通知目标文件夹
                if (pEncType==NULL || pAddr==NULL || pTime==NULL || pNotiFlag==NULL || pNotiFold==NULL ||
                     pPort==NULL  )
                {
                        //invalid address:skip this line
                        continue;
                }
                if ( atoi(pNum) != atoi(caNum)) continue;
                iSlotIndex =1;
                pLink->lFlags |= DCS_FLAG_USED;
                pLink->iStatus = DCS_STAT_STOPPED;
                strcpy(pLink->caRemoteIp,pAddr);
                memcpy(pLink->cEncType,pEncType,3);
                if ( u_stricmp(pPort1,"null") ==0)
                   pLink->iLocalPort =0;
                else
                	 pLink->iLocalPort = atoi(pPort1);
                pLink->iRemotePort = atoi(pPort);
                pLink->iSocketId=-1;
                pLink->iTimeOut=atoi(pTime);
                pLink->iNum = atoi(pNum);

    }
    return iSlotIndex;
}
int DeleteLink(struct secLinkInfo *pLink)
{
	char caBuf[10];
	sprintf(caBuf,"%d",pLink->iNum);
	if ( StopLink(caBuf) ==0)
	{
		 return 0;
	}
	pLink->lFlags &= ~DCS_FLAG_USED;
	return 1;
}
void TransProc ()
{
	int qid,qid1,nRcvLen,nLen,iSockId,iNum;
	char caBuf[4096];
	struct MSG_STRU *pMsg;
	long iType;
	
	qid = queue_create("1000");
	if ( qid <0)
  	qid= queue_connect("1000")	;
  if ( qid <0)
  {	
  		dcs_log(0,0,"Can not connect message queue !");
  		return ; 
  }
      g_qid=qid;
	qid1 = queue_create("2000");
	if ( qid1 <0)
  	qid1= queue_connect("2000")	;
  if ( qid1 <0)
  {	
  		dcs_log(0,0,"Can not connect message queue !");
  		return ; 
  }
      g_qid1=qid1;
	while ( 1)
	{
	
		 memset(caBuf,0,sizeof(caBuf));
		 nRcvLen = queue_recv( qid, caBuf, sizeof(caBuf), 0);
		 if ( nRcvLen <0)
		 {			
		 				dcs_log(0,0,"Recive Message fail!");
		 		   break;
	
		 }
		 pMsg = (struct MSG_STRU *)caBuf;
		 iType = pMsg->iPid;
		 iSockId =  atoi(pMsg->caSocketId);
		 iNum =  pMsg->iNum[0]<<4+pMsg->iNum[1]&0x0f;
		 if ( iNum <0 || iNum >99 )
		 {
		 		memcpy(caBuf+sizeof(long),"FF",2);
		  	queue_send( qid1, caBuf, sizeof(long)+2,1);
		  	dcs_log(pMsg->caMsg,nRcvLen-sizeof(long)-5,"Send Data fail !");
		  	continue;
		 }
      if ( nRcvLen <10 )
      {
      	 dcs_log(0,0,"数据太短,丢弃!");
      	 memcpy(caBuf+sizeof(long),"FF",2);
		  	 queue_send( qid1, caBuf, sizeof(long)+2,1);
		  	 continue;
      }
   		    if(memcmp(pMsg->caMsg+2,"E10", 2)==0)
		  		dcs_debug(pMsg->caMsg, 32,"发送到加密机的数据包 sockid=%d",iSockId);
		  	else
		  		dcs_debug(pMsg->caMsg,nRcvLen-sizeof(long)-5-3,"发送到加密机的数据包 sockid=%d",iSockId);
		  if( 0>= write_msg_to_jmj( iSockId, pMsg->caMsg, nRcvLen-sizeof(long)-5-3,2000))
		  {
		  	iSockId= -1;
		  	dcs_log(0,0,"send data fail, again send!");
		  	iSockId=MakeConnection1(&g_pSecCfg->secLink[iNum]);
		  	if( iSockId>=0 )
		  	{
		  		if( 0>= write_msg_to_jmj( iSockId, pMsg->caMsg, nRcvLen-sizeof(long)-5-3,2000))
	  			{
					  	memcpy(caBuf+sizeof(long),"FF",2);
					  	queue_send( qid1, caBuf, sizeof(long)+2,1);
					  	dcs_log(pMsg->caMsg,nRcvLen-sizeof(long)-5,"Send Data fail !");
					  	continue;

	  			}
		  	}
		  	else 
		  	{
			  	memcpy(caBuf+sizeof(long),"FF",2);
			  	queue_send( qid1, caBuf, sizeof(long)+2,1);
			  	dcs_log(pMsg->caMsg,nRcvLen-sizeof(long)-5,"Send Data fail !");
			  	continue;
		  	}
//		  	return ;
		  }
  		memcpy(caBuf,&iType,sizeof(long));
		  nLen = read_msg_from_NAC(iSockId, caBuf+sizeof(long), sizeof(caBuf),5000);
		  if ( nLen <=0)
		  {
		  	dcs_log(0,0,"recv data fail, again send!");
		  	iSockId= -1;
		  	iSockId=MakeConnection1(&g_pSecCfg->secLink[iNum]);
		  	dcs_debug(0,0,"new sock id =[%d]",iSockId);
		  	if( iSockId>=0 )
		  	{
		  		if( 0>= write_msg_to_jmj( iSockId, pMsg->caMsg, nRcvLen-sizeof(long)-5-3,2000))
	  			{
					  	memcpy(caBuf+sizeof(long),"FF",2);
					  	queue_send( qid1, caBuf, sizeof(long)+2,1);
					  	dcs_log(pMsg->caMsg,nRcvLen-sizeof(long)-5,"Send Data fail !");
					  	continue;

	  			}
		  	}
		  	else
		  	{
		  		memcpy(caBuf,&iType,sizeof(long));
		  		memcpy(caBuf+sizeof(long),"FF",2);
		  	  queue_send( qid1, caBuf, sizeof(long)+2,1);
		  		dcs_log(0,0,"从加密机接收数据失败!");
		  		continue;
		  	}
		  	nLen = read_msg_from_NAC(iSockId, caBuf+sizeof(long), sizeof(caBuf),5000);
		  	if ( nLen <=0)
		  	{
		  		memcpy(caBuf,&iType,sizeof(long));
		  		memcpy(caBuf+sizeof(long),"FF",2);
		  	  queue_send( qid1, caBuf, sizeof(long)+2,1);
		  		dcs_log(0,0,"从加密机接收数据失败!");
		  		continue;
		  	}
//		  		return ;
		  }
		if(memcmp(caBuf+sizeof(long)+2,"E200",4)==0)
		    dcs_debug(caBuf+sizeof(long),  10,"从加密机收到的数据 len =%d",nLen);			
		else
		dcs_debug(caBuf+sizeof(long),nLen,"从加密机收到的数据 len =%d",nLen);
		  queue_send( qid1, caBuf, nLen+sizeof(long),1);
	}
	
	
}
