#define _MainProg

// �������ƣ�dcmhstsv.c
// ����������
//           ������ɻ����Ľ���
// �����嵥��
//           main()
#include <time.h>
#include <pthread.h>
#include  "dccglobl.h"
#include  "dccpgdef.h"
#include  "dccdcs.h"
//#include  "tmctms.h"
#include  "tmcibtms.h"
//#include  "errlog.h"
extern g_iLogFlag;
static struct hstcom	*g_pLinkCtrl = NULL; 
static char		*g_pcIseHome = NULL;
static int		g_iHstSrvFoldId = -1;

static void HstSvrExit(int signo);

//static void DoDieStart();
static void DoSeachCheckPoint();

// function:main()
// arguments:
//           
// returns:
//           
// description:
//           1.����ͨѶ����������غ���
//           2.����ͨѶ���̵Ĺ���

int main(int argc, char * argv[])
{
	int	iCommCnt, i, iShmSize, iTmpShmid;
	pthread_t sthread;
//        g_iLogFlag =1;
  	catch_all_signals(HstSvrExit);
   	if ( argc >=2 && strcmp(argv[1] ,"1")==0)
   			;
   	else
  			u_daemonize(NULL);
 
//  	int sigqueue(pid_t pid, int sig, const union sigval value);
  	//prepare the log stuff
  	if (0 > HstSrvOpenLog())
    		goto lblExit;
    if(dcs_connect_shm() < 0)
    {
        dcs_log(0,0,"dcs_connect_shm() failed:%d\n",(errno));
        exit(1);
    }
    i=0;
    while (i<3 )
    {
		   	if (fold_initsys() < 0)
		  	{
		               if (i< 2)
		               {
		                  usleep(500000);
		                  i++;
		                  continue;
		               }
		    		dcs_log(0,0, "<HstSrv> cannot attach to FOLDER !");
		    		goto lblExit;
		  	}
        break;
    }
  	g_iHstSrvFoldId = fold_create_folder(DCS_HSTSRV_FOLD_NAME);
  	if (g_iHstSrvFoldId < 0)  
      		g_iHstSrvFoldId = fold_locate_folder(DCS_HSTSRV_FOLD_NAME);
  	if (g_iHstSrvFoldId < 0)  
  	{
    		dcs_log(0,0,"<HstSrv> cannot create folder '%s'%d!",\
                      DCS_HSTSRV_FOLD_NAME,(errno));
    		
    		goto lblExit;
  	}
  	fold_set_attr(g_iHstSrvFoldId, FOLD_FLAG_NEEDSIG);
  	//���ӹ����ڴ�BCDA
  
  	g_pcBcdaShmPtr = (char *)shm_connect(DCS_BCDA_NAME,NULL);
  	if (g_pcBcdaShmPtr == NULL)
  	{
    		dcs_log(0,0,"<HstSrv> cannot connect shared memory %s!",DCS_BCDA_NAME);
    		
    		goto lblExit;
  	}
  	//��ȡ�����ļ�,�����·����
  	if ((iCommCnt = LoadCommCounter()) < 0)
  	{
    		dcs_log(0,0, "<HstSrv> Failure to load isdcmlnk.ini !");
    		goto lblExit;
  	}    
  	//���������ڴ�HSTCOM������¼��ͨ�Ž��̵�״̬
  	iShmSize = sizeof(struct hstcom) + (iCommCnt-1) * sizeof(struct linkinfo);
  	g_pLinkCtrl=(struct hstcom *)shm_create(DCS_HSTCOM_NAME,iShmSize,&iTmpShmid);
  	if (g_pLinkCtrl==NULL)
  	{
    		dcs_log(0,0, "<HstSrv> cannot create shared memory as links array!");
    		goto lblExit;
  	}
  	else
  	{  
    		memset(g_pLinkCtrl,0,iShmSize); 
    		g_pLinkCtrl->iLinkCnt = iCommCnt;
    		g_pLinkCtrl->iShmId   = iTmpShmid;
  	}    
 
        dcs_log(0,0,"create dcs_hstcom shared  succ!");
    	//��ȡͨѶ���������ļ�isdcmlnk.conf
  	if (LoadCommLnk() < 0)
  	{
    		dcs_log(0,0, "<HstSrv> Failure to load isdcmlnk.conf !" );
    		goto lblExit;
  	}


    	//try to start up all communication processes
  	if ( 0 > StartComm(-1))
    		goto lblExit;
  
  	dcs_log(0,0, "<HstSrv>startup complete, going to service!");
/*
  	//register
  	if ( PidRegister( "HST", NULL, getpid() ) < 0 )
    		goto lblExit;
*/
  	//service loop
/*  	
			if ( pthread_create(&sthread, NULL,( void * (*)(void *))DoSeachCheckPoint,NULL) !=0)
			{
					dcs_log(0,0, "Can not create DoSeachCheckPoint thread ");
					goto lblExit;
			}
*/
//			pthread_detach(sthread);
/*			
			if ( pthread_create(&sthread, NULL,( void * (*)(void *))DoDieStart,NULL) !=0)
			{
					dcs_log(0,0, "Can not create diestart thread ");
					goto lblExit;
			}
*/
//			pthread_detach(sthread);
  		DoService();

lblExit:  
  	//release all resources and exit
  	
  	HstSvrExit(0);
}//main()


// ��������  HstSvrExit
// ����˵����
//          signo   ������ź�ֵ
// �� �� ֵ��
//          ��
// ����������
//          ��������ź�Ϊ SIGTERMʱ���ú����ͷ���Դ����ֹ��ǰ����;����
//          �����κδ���

void HstSvrExit(int signo)
{
  	int i,iExitCode,pidChld;

 		dcs_log(0,0,"<HstSrv> catch a signal %d !",signo);
    if( signo == SIGCHLD)
    {
    	    pidChld = wait_child_or_signal(&iExitCode);
    			if (pidChld > 0)
    			{
        			//a child dies,try to reload it
//        			usleep(50000);
        			dcs_log(0,0,"<%s>  died process pid=[%d],exit code=[%d]",__FUNCTION__,pidChld,iExitCode);
         			HandleCommDie(pidChld);
    			}
    			else
    			{
    				dcs_log(0,0,"<%s> wait_child_or_signal error! errno=[%d][%s] ",
    				   __FUNCTION__,errno,strerror(errno));
    			}
    	return;
    }
  	if (signo != SIGTERM && signo != 0 && signo != SIGSEGV)
  	{
    		
    		return;
  	}  

  	//notify all comm processes to exit
  	if (g_pLinkCtrl)
  	{
    		for (i=0; i < g_pLinkCtrl->iLinkCnt; i++)  
    		{
      			if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
        			continue;
      			if (g_pLinkCtrl->linkArray[i].pidComm <= 0)
        			continue;
    
//      			kill(g_pLinkCtrl->linkArray[i].pidComm,SIGTERM);
 
    		}
    }
      
  	//cleanup the link list
  	if (g_pLinkCtrl)
  	{
    		int shmid;
    
    		shmid = g_pLinkCtrl->iShmId;
    		shm_detach((char *)g_pLinkCtrl);
    		shm_delete(shmid);
    		g_pLinkCtrl = NULL;
  	}

  	//clear the folder of myself
  
  	fold_purge_msg(g_iHstSrvFoldId);
  	fold_delete_folder(g_iHstSrvFoldId);

  	
  	exit(signo);
}

// ��������  HstSrvOpenLog
// ����˵����
//          ��
// �� �� ֵ��
//          �ɹ�����0,����-1
// ����������
//          1.���ϵͳ��Ŀ¼,��֮����ȫ�ֱ���g_pcIseHome
//          2.������־�ļ�����־��ʽ

int HstSrvOpenLog()
{
  	   
  	char logfile[256];
   g_pcIseHome=getenv("ICS_HOME");
    if(u_fabricatefile("log/hstsvr.log",logfile,sizeof(logfile)) < 0)
        return -1;

    return dcs_log_open(logfile, NULL);
}

// �������ƣ�LoadCommCounter
// ����˵����
//           ��
// �� �� ֵ��
//           �ɹ�ʱ����commlnk�ж����������Ŀ,����-1
// ����������
//           ��ȡcommlnk�ж����������Ŀ

int LoadCommCounter()
{
  	char  caFileName[256];
  	int   iFd=-1, iCommCnt=-1;  
    

  	//�趨commlnk�����ļ��ľ���·��������
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/config/%s",g_pcIseHome,DCS_COMMLNK_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log(0,0, "<HstSrv> open isdcmlnk.ini fail ! ");
    		return (-1);
  	}

  	//��ȡ������'comm.max'
  	if ( 0 >conf_get_first_number(iFd, "comm.max",&iCommCnt) || iCommCnt < 0 )
  	{
    		dcs_log(0,0, "<HstSrv> load configuration item 'comm.max' failed!");
    		conf_close(iFd);                         
    		return (-1);
  	}
    
  	conf_close(iFd);
  	return (iCommCnt);
}


// �������ƣ�LoadCommLnk
// ����˵����
//           ��
// �� �� ֵ��
//           �ɹ�ʱ����0,����-1
// ����������
//           ��commlnk�ж�����ļ��Ŀ���������Ϣ���ص�������

int LoadCommLnk()
{
  	char   caFileName[256], caBuffer[1024];
  	int    iFd=-1, iRc,iSlotIndex = 0;  
  	struct linkinfo *pLinkEntry=NULL;
  	char   *pLinkFldName, *pApplFldName,*pRole, *pAddr1, *pAddr2,*pCheckFlag;
  	char   *pIp1, *pIp2, *pPort1, *pPort2 , *pTime, *pCommType,*pCommNO,*pNotiFold,*pNotiFlag,*pMsgType;     

  	

  	//�趨commlnk�����ļ��ľ���·��������
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/config/%s",g_pcIseHome,DCS_COMMLNK_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log(0,0, "<HstSrv> open isdcmlnk.conf fail ! ");
    		return (-1);
  	}
 //   sleep(2);
  	//��ȡ���е�������'comm'
  	for( iRc = conf_get_first_string(iFd, "comm",caBuffer);\
       		iRc == 0 && iSlotIndex < g_pLinkCtrl->iLinkCnt;\
       		iRc = conf_get_next_string(iFd, "comm",caBuffer))
  	{
                pCommType    = strtok(caBuffer," \t\r");  //ͨ������
  	       printf(" \nend strtok comm type ");	
  		pCommNO      = strtok(NULL," \t\r");   //ͨ��˳���
    		pLinkFldName = strtok(NULL," \t\r");  //�ļ�����
    		pApplFldName = strtok(NULL   ," \t\r");  //��link��Ӧ��Ӧ�ý��̵��ļ�����
    		pRole        = strtok(NULL   ," \t\r");  //��ɫ
    		pAddr1       = strtok(NULL   ," \t\r");  //��ַ��1
    		pAddr2       = strtok(NULL   ," \t\r\n");//��ַ��2
    		pMsgType     = strtok(NULL   ," \t\r\n");//��������
    		pTime        = strtok(NULL   ," \t\r\n");//ͨ�ų�ʱ���ʱ��
    		pCheckFlag   = strtok(NULL   ," \t\r\n");//��·��ⷢ���־
    		pNotiFlag     = strtok(NULL," \t\r"); //ͨѶ��·״̬֪ͨ��־ 
        pNotiFold    = strtok(NULL," \t\r"); //ͨѶ��·״̬֪ͨĿ���ļ��� 
    		if (pCommType==NULL || pLinkFldName==NULL || pApplFldName==NULL || pRole==NULL || pAddr1==NULL || 
    		     pAddr2==NULL || pTime==NULL )
    		{
      			//invalid address:skip this line
      			continue;
    		}   
    
    		pIp1   = strtok(pAddr1, ":");
    		pPort1 = strtok(NULL, ":\r\t\n");
    		pIp2   = strtok(pAddr2, ":");
    		pPort2 = strtok(NULL, ":\r\t\n");
    		if (pIp1==NULL || pPort1==NULL || pIp2==NULL || pPort2==NULL)
    		{
      			//invalid address:skip this line
      			continue;
    		}   
    
    		//���͸�link�������ļ��еĺϷ���,�����ļ��е�����
    		//����Ϊ�Ѿ�����ĳ�Ա������
/*    		if (0 > LocateCommByName(pLinkFldName))
    		{
        		//invalid folder name, skip this line
        		sprintf(g_caMsg,"<HstSrv> folder name %s is not a Comm name!",
                         	pLinkFldName);   
        		ErrLog(1000, g_caMsg, RPT_TO_LOG, 0, 0 );
        		continue;
    		}
  */      
    		//�����͸�link�������ļ���
    		pLinkEntry = &(g_pLinkCtrl->linkArray[iSlotIndex++]);
    		strcpy(pLinkEntry->caFoldName,pLinkFldName);
    		pLinkEntry->iFid = fold_create_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0 && errno == FOLD_EEXIST)
      			pLinkEntry->iFid = fold_locate_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0)
    		{
      			dcs_log(0,0, "<HstSrv> cannot create folder '%s' !",pLinkFldName);   
      			
      			goto lblFailure;
    		}
    

    		//��λ�͸�link������Ӧ�ý��̵��ļ���
    		pLinkEntry->iApplFid = fold_locate_folder(pApplFldName);
    		if (pLinkEntry->iApplFid < 0)
    		{
      			dcs_log(0,0, "<HstSrv> cannot locate folder '%s' !",pApplFldName);   
      			
      			goto lblFailure;
    		}
    		memcpy(pLinkEntry->commType,pCommType,3);
    		memcpy(pLinkEntry->commNO,pCommNO,3);

                 printf("\n locate   app fold name succ! ");
    		//���������ӽ��������б��س䵱�Ľ�ɫ
    		if (u_stricmp(pRole,"passive")==0 )
      			pLinkEntry->iRole = DCS_ROLE_PASSIVE;
    		else if (u_stricmp(pRole,"active")==0 )
      			pLinkEntry->iRole = DCS_ROLE_ACTIVE;
                else if (u_stricmp(pRole,"simplexsvr")==0)
                	pLinkEntry->iRole = DCS_ROLE_SIMPLEXSVR;
                else if (u_stricmp(pRole,"simplexcli")==0)
                	pLinkEntry->iRole = DCS_ROLE_SIMPLEXCLI;
                else
                       goto lblFailure;
                //do with the address
    		strcpy(pLinkEntry->caLocalIp,pIp1);
                if(strcmp(pPort1,"null")==0 )
                     pLinkEntry->sLocalPort=0;
                else
    			pLinkEntry->sLocalPort = atoi(pPort1); //listening port
    		strcpy(pLinkEntry->caRemoteIp,pIp2);
                if(strcmp(pPort2,"null")==0 )
			pLinkEntry->sRemotePort=0;
                else
    			pLinkEntry->sRemotePort = atoi(pPort2);
    		pLinkEntry->iTimeOut = atoi(pTime);
    		pLinkEntry->lFlags |= DCS_FLAG_USED; //mark as used
    		//pLinkEntry->MacFlag=pMacFlag[0];
    		if ( memcmp(pNotiFlag,"yes",3)==0)
    				pLinkEntry->iNotifyFlag=1;
    		else
    				pLinkEntry->iNotifyFlag=0;
    		if ( memcmp(pCheckFlag,"yes",3)==0)
    	  	   pLinkEntry->caCheckFlag = 1;
    	  else
    	  		pLinkEntry->caCheckFlag=0;
				memcpy( pLinkEntry->caMsgType, pMsgType,4);  				
               if ( pNotiFold )
    		memcpy(pLinkEntry->caNotiFold,pNotiFold,(strlen(pNotiFold)<MAX_FOLDNAME_SIZE)? strlen(pNotiFold):MAX_FOLDNAME_SIZE);
    		dcs_debug(0,0, "hst link %d setup finish ",iSlotIndex-1);   
    		
  	}//for

  	conf_close(iFd);
  	return (iSlotIndex > 0 ? 0 : -1);

lblFailure:
  	conf_close(iFd);
  	return (-1);
}


// �������ƣ�StartComm
// ����˵����
//           iSlot ��Ϊ-1,�������е�ͨ�Ž���;
//                 ��������ָ��λ�õ�ͨ�Ž���.
// �� �� ֵ��
//           �ɹ�ʱ����0,����-1
// ����������
//           ����tcpͨ�Ž���

int StartComm(int iSlot)
{
  	int    i, iRc, nLow, nUpper;
  	pid_t  pidChld;
    char  mBuf[40];
  
  	if (iSlot == -1)
  	{
     		nLow = 0; 
     		nUpper = g_pLinkCtrl->iLinkCnt-1;  
  	}
  	else   
    		nLow = nUpper = iSlot;
    	memset(mBuf,0,sizeof(mBuf));
    dcs_debug(0,0,"start process low=%d,upper=%d",nLow,nUpper);
  	for (i=nLow; i <= nUpper; i++)  
  	{
    		if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
        if ( g_pLinkCtrl->linkArray[i].iStatus != DCS_STAT_DISCONNECTED && g_pLinkCtrl->linkArray[i].iStatus != DCS_STAT_STOPPED )
        		continue;
    		if ( (pidChld = fork()) < 0)
    		{
      			dcs_log(0,0, "<HstSrv> cannot fork child process!");
      			return (-1);
    		}
    
    		if (pidChld > 0)
    		{
      			//i am parent,write down the child's pid
      			g_pLinkCtrl->linkArray[i].pidComm = pidChld;  
      			continue;
    		}
        memset(mBuf,0,sizeof(mBuf));
        memcpy(mBuf,g_pLinkCtrl->linkArray[i].commNO,3);
    		//child here, try exec 'dcmhstcp.x'
    		dcs_debug(0,0,"execute dcxhstcp %s %s",g_pLinkCtrl->linkArray[i].caFoldName,mBuf);
    		iRc = execlp ("dcxhstcp", "dcxhstcp", g_pLinkCtrl->linkArray[i].caFoldName,mBuf,(char *)0);
    		if (iRc < 0)
    		{
      			dcs_log(0,0,"<HstSrv> cannot exec executable 'dcxhstcp.x':%s!",strerror(errno));
      			exit(-1);    
    		}
  	}//for
  
  	return (0);
}
  
// �������ƣ�DoService
// ����˵����
//           ��
// �� �� ֵ��
//           always -1
// ����������
//           1.�������е�tcpͨ�Ž��̵�����,��Ҫʱ��������
//           2.��������ISA_SWITCH��֪ͨ,����ȡ��Ӧ����
//           3.����������

int DoService()
{
  	char    caMsg[1024];
  	int     nRead, iOrgFid, iExitCode;
  	pid_t   pidChld;
  
  	
  
  	fold_purge_msg(g_iHstSrvFoldId);
  	dcs_debug(0,0,"entry service !");

  	for (;;)
  	{
    		//read request from my folder in non-blocking mode
//    		dcs_debug(0,0, "<HstSvr--DoService> Read hstsvr fold in non-blocking mode!" );
    		memset(caMsg,0,sizeof(caMsg));
    		nRead = fold_read(g_iHstSrvFoldId, &iOrgFid, caMsg,sizeof(caMsg),TRUE);
    		if (nRead < 0)
    		{
	      			if (errno != ENOMSG && errno != EINTR)
	      			{  
	        			//read error
	        			dcs_log(0,0,"<HstSvr> fold_read() failed:%d!",(errno));
	        			break;
	      			}  
	
	      			if (fold_sys_active() < 0)
	        			break;
	#if 0
							dcs_debug(0,0, "<HstSvr--DoService> Before wait!");
	      			//wait a child to die
	      			pidChld = wait_child_or_signal(&iExitCode);
	
	      			if (pidChld > 0)
	      			{
	          			//a child dies,try to reload it
//	          			usleep(1500000);
             			HandleCommDie(pidChld);
	          			continue;
	      			}
	#endif
	/*
	      			//read request from my folder in blocking mode
	      			dcs_debug(0,0, "<HstSvr--DoService> After wait,Read hstsvr fold in blocking mode!");
	      			nRead = fold_read(g_iHstSrvFoldId, &iOrgFid, caMsg,sizeof(caMsg),TRUE);
	      			if (nRead < 0)
	      			{
				    			if (errno == EINTR)
				        			continue;
				    			else
				        			break;
	      			}
*/
    		}
    
    		//a request from someone else arrived, do with it
//    		AnswerRequest(caMsg);
      	HandleCommandLine(caMsg);

  	}//for(;;)
  
  	return (-1);  
}

int AnswerRequest(char *paMsg)
{
  	struct  hstrequest *pQry;
  	int     i, iMsgType;
  
  	
  
  	pQry = (struct  hstrequest *)paMsg;
  
  	//convert message type from string to int
  	for (iMsgType=0,i=0; i<4; i++)
    	iMsgType = iMsgType * 10 + (pQry->caMsgType[i] - '0'); 

  	dcs_debug(0,0,"recv a message with type = %.4d",iMsgType);
  	

/*  	switch (iMsgType)
  	{
        
    	case DCS_LINKESTAB_NOTI: //link establishment notification from Tcpip
      		HandleLinkEstab(paMsg);
     		break;
    
    	case DCS_COMMAND_LINE:  //request from command line interface
      		HandleCommandLine(paMsg);
      		break;
      
    	default:
      		break;
  	}  
 */ 
  	return (0);
}

int HandleCommDie(pid_t pidChld)
{
  	int  i, find;
  
  	
//    sleep(1);
  	//locate the corresponding comm process
  	for (find=-1,i=0; i < g_pLinkCtrl->iLinkCnt; i++)  
  	{
    		if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (g_pLinkCtrl->linkArray[i].pidComm != pidChld)  
      			continue;
      
    		find = i;
    		break;  
  	}
  
  	if (find < 0)
  	{
    		dcs_log(0,0,"<HstSrv> a comm process died,but cannot "
                    "locate the associated link info pid=[%d]",pidChld);
    		
    		return (-1);
  	}
 
//  	usleep(500);
  	if (g_pLinkCtrl->linkArray[find].iStatus != DCS_STAT_STOPPED)
  	{
   		dcs_log(0,0,"<HstSrv> comm process for [%s %3.3s] died,try reload it...",\
                    	g_pLinkCtrl->linkArray[find].caFoldName,g_pLinkCtrl->linkArray[find].commNO);
    		StartComm(find);
  	}
  
  	return (0);
}

int UpdateNetLink(char *pcLinkName,int iStat)
{
  	int i;
  	struct BCDA * pBCDA;
  
  
  	//determine which issuer timeout
  	i = LocateBankByName( pcLinkName );
  
  	if (i < 0)  //not found          
    		return (-1);

  	pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;

  	//update the net status for this entry
  	if (iStat == DCS_STAT_DISCONNECTED)
  	{
    		pBCDA[i].cNetFlag --; //decrease the link count
    		if (pBCDA[i].cNetFlag < 0)
      			pBCDA[i].cNetFlag = 0;


//    		pBCDA[i].cNetFlag --;  //always set to 0
 /*   		if (pBCDA[i].cNetFlag == 0)
      			pBCDA[i].cAppFlag = 0;
*/
  	}
  	else if (iStat == DCS_STAT_CONNECTED)
  	{
    		if (pBCDA[i].cNetFlag < 0)
      			pBCDA[i].cNetFlag = 0;
    		pBCDA[i].cNetFlag ++; //increase the link count

//    		pBCDA[i].cNetFlag = 1;  //always set to 1
  	}    
  
  	return (0);
}  
/*
int HandleLinkEstab(char *paMsg)
{
  	struct commnotify *pNoti;
  	int  i, find;
  
  	
  
  	pNoti = (struct commnotify *)paMsg;

  	//locate the corresponding comm process
  	for (find=-1,i=0; i < g_pLinkCtrl->iLinkCnt; i++)
  	{
    		if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (memcmp(g_pLinkCtrl->linkArray[i].caFoldName,pNoti->caFoldName,strlen(g_pLinkCtrl->linkArray[i].caFoldName)) != 0)
      			continue;

    		find = i;
    		break;
  	}

  	if (find < 0)
  	{
    		dcs_log(0,0,"<HstSrv> a comm process report change of link status,"\
                    "but cannot locate the associated link info foldname[%s]",pNoti->caFoldName);
    		
    		return (-1);
  	}

  	if (pNoti->iStatus == DCS_STAT_DISCONNECTED)
  	{ 
     		//int pid;

     		//usleep(1500000);
     		//pid = g_pLinkCtrl->linkArray[find].pidComm;
     		//if(pid == waitpid(pid, NULL,  WNOHANG))
         	//HandleCommDie(pid);

     		return (0);
  	}
  	else
  	{
      		char tmpName[28];

      		strcpy(tmpName, pNoti->caFoldName);
      		dcs_log(0,0,"<HstSrv>connection to [%s] established.",tmpName);
      		

      		//update the net status in BCDA
//      		UpdateNetLink(pNoti->caFoldName,DCS_STAT_CONNECTED);
  	}

  	return (0);
}
*/
int HandleCommandLine(char *paMsg)
{
  	struct hstcmd *pCmd;
  	int    i, find;
  
  
  	pCmd = (struct hstcmd *)paMsg;
    dcs_log(0,0,"command=%d, foldname=%s",pCmd->iCommand,pCmd->caFoldName);
  	if (pCmd->iCommand == DCS_CMD_EXIT)
  	{
    		raise(SIGTERM);
    		return (0);
  	}
  	if (pCmd->iCommand ==  DCS_CMD_ADD )
  	{
    	  dcs_debug(0,0,"<HstSrv> add link [%s]",pCmd->caFoldName);
    	  if( strlen(pCmd->caMsgFlag)<1)
    	  		TcpAddLink( pCmd->caFoldName,NULL );
    	  else
    	  		TcpAddLink( pCmd->caFoldName ,pCmd->caMsgFlag);
    		return (0);
		}
		if (pCmd->iCommand ==  DCS_CMD_MODIFY )
		{
			TcpModiLink( pCmd->caFoldName ,pCmd->caMsgFlag);
			return(0);
	  }
		
  	//locate the intended link entry
  	for (find=-1,i=0; i < g_pLinkCtrl->iLinkCnt; i++)  
  	{
    		if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
        if ( strlen(pCmd->caMsgFlag)<1)
        {
		    		if (u_stricmp(g_pLinkCtrl->linkArray[i].caFoldName,pCmd->caFoldName) != 0)  
		      			continue;
		    }
		    else
		    {
		    		if (u_stricmp(g_pLinkCtrl->linkArray[i].caFoldName,pCmd->caFoldName) != 0 || memcmp(g_pLinkCtrl->linkArray[i].commNO,pCmd->caMsgFlag,3)!=0)  
		      			continue;
		    }
      		switch (pCmd->iCommand)
			  	{
			    	case DCS_CMD_STOP:
//			      		pCmd->caFoldName[4]=0;
			      	
			      		if ( g_pLinkCtrl->linkArray[i].iStatus != DCS_STAT_STOPPED)
			      		{
			        		dcs_debug(0,0,"<HstSrv> stop [%s]",pCmd->caFoldName);
			        		if( 0>kill(g_pLinkCtrl->linkArray[i].pidComm,SIGUSR2))
			        		{
			        				dcs_log(0,0,"kill pid[%d] fail![%s]",g_pLinkCtrl->linkArray[i].pidComm,strerror(errno));  //SIGTERM
			        		}
//			        		wait(NULL);
//			        		g_pLinkCtrl->linkArray[i].iStatus = DCS_STAT_STOPPED;

			      		} 
			      		else
			      				dcs_debug(0,0,"<HstSrv> [%s] is stopped",pCmd->caFoldName);  
			      		break;
			
			    	case DCS_CMD_START:
//			      		pCmd->caFoldName[4]=0;
			      		
			      		if ( g_pLinkCtrl->linkArray[i].iStatus == DCS_STAT_STOPPED || \
			          		kill(g_pLinkCtrl->linkArray[i].pidComm,0) <0)
			      		{
			        				dcs_debug(0,0,"<HstSrv> start [%s]",pCmd->caFoldName);
			        				StartComm(i);
			      		}   
			      		break;
			      case DCS_CMD_DEL:
			      	  dcs_debug(0,0,"delete tcp link , free a solt");
			      	  kill(g_pLinkCtrl->linkArray[i].pidComm,SIGUSR2);  //SIGTERM
//			        	wait(NULL);
			      	  g_pLinkCtrl->linkArray[i].lFlags &= ~DCS_FLAG_USED;
			    	default:
			      		break;
			  	}
      
      
//    		find = i;
//   		break;  
  	}
  
//  	if (find < 0)
//    		return (-1);

  
  	return (0);
}

int TcpAddLink ( char * pcFoldName , char *commNO)
 {
 	
 	  char   caFileName[256], caBuffer[1024];
  	int    iFd=-1, iRc,iSlotIndex = 0;  
  	struct linkinfo *pLinkEntry=NULL;
  	char   *pLinkFldName, *pApplFldName,*pRole, *pAddr1, *pAddr2,*pCheckFlag;
  	char   *pIp1, *pIp2, *pPort1, *pPort2 , *pTime, *pCommType,*pCommNO,*pNotiFold,*pNotiFlag,*pMsgType;     

  	

  	//�趨commlnk�����ļ��ľ���·��������
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/config/%s",g_pcIseHome,DCS_COMMLNK_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log(0,0, "<HstSrv> open isdcmlnk.conf fail ! ");
    		return (-1);
  	}
  	//��ȡ���е�������'comm'
  	for( iRc = conf_get_first_string(iFd, "comm",caBuffer);\
       		iRc == 0 ;\
       		iRc = conf_get_next_string(iFd, "comm",caBuffer))
  	{
  		pCommType    = strtok(caBuffer," \t\r");  //ͨ������
  		pCommNO      = strtok(NULL," \t\r");   //ͨ��˳���
    		pLinkFldName = strtok(NULL," \t\r");  //�ļ�����
    		if ( commNO == NULL )
    		{
		    		if ( u_stricmp(pLinkFldName ,pcFoldName ) !=0)
		    				continue;
		    }
		    else
		    {
		    			if ( u_stricmp(pLinkFldName ,pcFoldName ) !=0 || u_stricmp(pCommNO,commNO)!=0)
		    				continue;
		  	}
    		pApplFldName = strtok(NULL   ," \t\r");  //��link��Ӧ��Ӧ�ý��̵��ļ�����
    		pRole        = strtok(NULL   ," \t\r");  //��ɫ
    		pAddr1       = strtok(NULL   ," \t\r");  //��ַ��1
    		pAddr2       = strtok(NULL   ," \t\r\n");//��ַ��2
    		pMsgType     = strtok(NULL   ," \t\r\n");//��������
    		pTime        = strtok(NULL   ," \t\r\n");//ͨ�ų�ʱ���ʱ��
		    pCheckFlag   = strtok(NULL   ," \t\r\n");//��·��ⷢ���־
		    pNotiFlag     = strtok(NULL," \t\r"); //ͨѶ��·״̬֪ͨ��־  
  	  	pNotiFold    = strtok(NULL," \t\r"); //ͨѶ��·״̬֪ͨĿ���ļ��� 
  			
    		if (pCommType==NULL || pLinkFldName==NULL || pApplFldName==NULL || pRole==NULL || pAddr1==NULL || 
    		     pAddr2==NULL || pTime==NULL || pCheckFlag == NULL)
    		{
      			//invalid address:skip this line
      			continue;
    		}   
    
    		pIp1   = strtok(pAddr1, ":");
    		pPort1 = strtok(NULL, ":\r\t\n");
    		pIp2   = strtok(pAddr2, ":");
    		pPort2 = strtok(NULL, ":\r\t\n");
    		if (pIp1==NULL || pPort1==NULL || pIp2==NULL || pPort2==NULL)
    		{
      			//invalid address:skip this line
      			continue;
    		}
    		for ( iSlotIndex =0 ; iSlotIndex < g_pLinkCtrl->iLinkCnt;  iSlotIndex++)
    		{
    			  if ( commNO==NULL )
    			  {
		    			  if ( g_pLinkCtrl->linkArray[iSlotIndex].lFlags & DCS_FLAG_USED
		    			  	   && u_stricmp(g_pLinkCtrl->linkArray[iSlotIndex].caFoldName ,pcFoldName)==0 )
		    			  {
		    			  			dcs_log(0,0,"����·�Ѵ��ڣ��������� [%s]",pcFoldName);
		    			  			conf_close(iFd);
		    			  			return -1;
		    			  }
    			  }
    			  else 
    			  {
		    			  if ( g_pLinkCtrl->linkArray[iSlotIndex].lFlags & DCS_FLAG_USED
		    			  	   && u_stricmp(g_pLinkCtrl->linkArray[iSlotIndex].caFoldName ,pcFoldName)==0 
		    			  	   && memcmp(g_pLinkCtrl->linkArray[iSlotIndex].commNO,commNO,3) == 0) 
		    			  {
		    			  			dcs_log(0,0,"����·�Ѵ��ڣ��������� [%s][%s]",pcFoldName,commNO);
		    			  			conf_close(iFd);
		    			  			return -1;
		    			  }
    			  }
    			  
    		}
    		for ( iSlotIndex =0 ; iSlotIndex < g_pLinkCtrl->iLinkCnt;  iSlotIndex++)
    		{
    			  if ( !(g_pLinkCtrl->linkArray[iSlotIndex].lFlags & DCS_FLAG_USED))
    			  		break;
    		}
    		if ( iSlotIndex >g_pLinkCtrl->iLinkCnt)
    		{
    				dcs_log(0,0, "<HstSrv> not free solt for '%s' !",pcFoldName);   
    				conf_close(iFd);
  					return (-1);
    		}
     		pLinkEntry = &(g_pLinkCtrl->linkArray[iSlotIndex]);
    		strcpy(pLinkEntry->caFoldName,pLinkFldName);
    		pLinkEntry->iFid = fold_create_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0 && errno == FOLD_EEXIST)
      			pLinkEntry->iFid = fold_locate_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0)
    		{
      			dcs_log(0,0, "<HstSrv> cannot create folder '%s' !",pLinkFldName);   
      			
      			conf_close(iFd);
  					return (-1);
    		}
    
    		//��λ�͸�link������Ӧ�ý��̵��ļ���
    		pLinkEntry->iApplFid = fold_locate_folder(pApplFldName);
    		if (pLinkEntry->iApplFid < 0)
    		{
      			dcs_log(0,0, "<HstSrv> cannot locate folder '%s' !",pApplFldName);   
      			
      			conf_close(iFd);
  					return (-1);
    		}
    		memcpy(pLinkEntry->commType,pCommType,3);
    		memcpy(pLinkEntry->commNO,pCommNO,3);

    		//���������ӽ��������б��س䵱�Ľ�ɫ
    		if (u_stricmp(pRole,"passive")==0 )
      			pLinkEntry->iRole = DCS_ROLE_PASSIVE;
    		else if (u_stricmp(pRole,"active")==0 )
      			pLinkEntry->iRole = DCS_ROLE_ACTIVE;
                else if (u_stricmp(pRole,"simplexsvr")==0)
                	pLinkEntry->iRole = DCS_ROLE_SIMPLEXSVR;
                else if (u_stricmp(pRole,"simplexcli")==0)
                	pLinkEntry->iRole = DCS_ROLE_SIMPLEXCLI;
                else
                {
                	conf_close(iFd);
  								return (-1);
  							}
                //do with the address
    		strcpy(pLinkEntry->caLocalIp,pIp1);
    		pLinkEntry->sLocalPort = atoi(pPort1); //listening port
    		strcpy(pLinkEntry->caRemoteIp,pIp2);
    		pLinkEntry->sRemotePort = atoi(pPort2);
    		pLinkEntry->iTimeOut = atoi(pTime);
    		pLinkEntry->lFlags |= DCS_FLAG_USED; //mark as used
    		//pLinkEntry->MacFlag=pMacFlag[0];
    		if ( memcmp(pNotiFlag,"yes",3)==0)
    				pLinkEntry->iNotifyFlag=1;
    		else
    				pLinkEntry->iNotifyFlag=0;
    	  if ( memcmp(pCheckFlag,"yes",3)==0)
    	  	   pLinkEntry->caCheckFlag = 1;
    	  else
    	  		pLinkEntry->caCheckFlag=0;
    		memcpy( pLinkEntry->caMsgType, pMsgType,4);  				
                if (pNotiFold)
    		memcpy(pLinkEntry->caNotiFold,pNotiFold,(strlen(pNotiFold)<MAX_FOLDNAME_SIZE)? strlen(pNotiFold):MAX_FOLDNAME_SIZE);
    		dcs_debug(0,0, "hst link %d setup finish ",iSlotIndex-1);  
    		StartComm( iSlotIndex);
    		
  	}//for

  	conf_close(iFd);
  	return (iSlotIndex > 0 ? 0 : -1);

 	
}

int TcpModiLink ( char * pcFoldName , char *commNO)
 {
 	
 	  char   caFileName[256], caBuffer[1024];
  	int    iFd=-1, iRc,iSlotIndex = 0;  
  	struct linkinfo *pLinkEntry=NULL;
  	char   *pLinkFldName, *pApplFldName,*pRole, *pAddr1, *pAddr2,*pCheckFlag;
  	char   *pIp1, *pIp2, *pPort1, *pPort2 , *pTime, *pCommType,*pCommNO,*pNotiFold,*pNotiFlag,*pMsgType;     

  	

  	//�趨commlnk�����ļ��ľ���·��������
  	memset(caFileName, 0, sizeof(caFileName) );
  	sprintf(caFileName, "%s/config/%s",g_pcIseHome,DCS_COMMLNK_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log(0,0, "<HstSrv> open isdcmlnk.conf fail ! ");
    		return (-1);
  	}
  	//��ȡ���е�������'comm'
  	for( iRc = conf_get_first_string(iFd, "comm",caBuffer);\
       		iRc == 0 ;\
       		iRc = conf_get_next_string(iFd, "comm",caBuffer))
  	{
  		pCommType    = strtok(caBuffer," \t\r");  //ͨ������
  		pCommNO      = strtok(NULL," \t\r");   //ͨ��˳���
    		pLinkFldName = strtok(NULL," \t\r");  //�ļ�����
    		
		    if ( u_stricmp(pLinkFldName ,pcFoldName ) !=0 || u_stricmp(pCommNO,commNO)!=0)
		    			continue;
		  	pApplFldName = strtok(NULL   ," \t\r");  //��link��Ӧ��Ӧ�ý��̵��ļ�����
    		pRole        = strtok(NULL   ," \t\r");  //��ɫ
    		pAddr1       = strtok(NULL   ," \t\r");  //��ַ��1
    		pAddr2       = strtok(NULL   ," \t\r\n");//��ַ��2
    		pMsgType     = strtok(NULL   ," \t\r\n");//��������
    		pTime        = strtok(NULL   ," \t\r\n");//ͨ�ų�ʱ���ʱ��
    		pCheckFlag   = strtok(NULL   ," \t\r\n");//ͨ��������ⱨ�ķ��ͱ��
    		pNotiFlag     = strtok(NULL," \t\r"); //ͨѶ��·״̬֪ͨ��־ 
   			pNotiFold    = strtok(NULL," \t\r"); //ͨѶ��·״̬֪ͨĿ���ļ��� 
    		if (pCommType==NULL || pLinkFldName==NULL || pApplFldName==NULL || pRole==NULL || pAddr1==NULL || 
    		     pAddr2==NULL || pTime==NULL || pCheckFlag==NULL )
    		{
      			//invalid address:skip this line
      			continue;
    		}   
    
    		pIp1   = strtok(pAddr1, ":");
    		pPort1 = strtok(NULL, ":\r\t\n");
    		pIp2   = strtok(pAddr2, ":");
    		pPort2 = strtok(NULL, ":\r\t\n");
    		if (pIp1==NULL || pPort1==NULL || pIp2==NULL || pPort2==NULL)
    		{
      			//invalid address:skip this line
      			continue;
    		}
    		for ( iSlotIndex =0 ; iSlotIndex < g_pLinkCtrl->iLinkCnt;  iSlotIndex++)
    		{
    			  if ( u_stricmp(g_pLinkCtrl->linkArray[iSlotIndex].caFoldName ,pcFoldName)==0 && memcmp(g_pLinkCtrl->linkArray[iSlotIndex].commNO,commNO,3) == 0)
    			  		break;
    		}
    		if ( iSlotIndex >g_pLinkCtrl->iLinkCnt)
    		{
    					dcs_log(0,0, "<HstSrv> not felicity solt for '%s' !",pcFoldName);   
    				conf_close(iFd);
  					return (-1);
    		}
     		pLinkEntry = &(g_pLinkCtrl->linkArray[iSlotIndex]);
    		strcpy(pLinkEntry->caFoldName,pLinkFldName);
    		pLinkEntry->iFid = fold_create_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0 && errno == FOLD_EEXIST)
      			pLinkEntry->iFid = fold_locate_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0)
    		{
      			dcs_log(0,0, "<HstSrv> cannot create folder '%s' !",pLinkFldName);   
      			
      			conf_close(iFd);
  					return (-1);
    		}
    
    		//��λ�͸�link������Ӧ�ý��̵��ļ���
    		pLinkEntry->iApplFid = fold_locate_folder(pApplFldName);
    		if (pLinkEntry->iApplFid < 0)
    		{
      			dcs_log(0,0, "<HstSrv> cannot locate folder '%s' !",pApplFldName);   
      			
      			conf_close(iFd);
  					return (-1);
    		}
    		memcpy(pLinkEntry->commType,pCommType,3);
    		memcpy(pLinkEntry->commNO,pCommNO,3);

    		//���������ӽ��������б��س䵱�Ľ�ɫ
    		if (u_stricmp(pRole,"passive")==0 )
      			pLinkEntry->iRole = DCS_ROLE_PASSIVE;
    		else if (u_stricmp(pRole,"active")==0 )
      			pLinkEntry->iRole = DCS_ROLE_ACTIVE;
                else if (u_stricmp(pRole,"simplexsvr")==0)
                	pLinkEntry->iRole = DCS_ROLE_SIMPLEXSVR;
                else if (u_stricmp(pRole,"simplexcli")==0)
                	pLinkEntry->iRole = DCS_ROLE_SIMPLEXCLI;
                else
                {
                	conf_close(iFd);
  								return (-1);
  							}
                //do with the address
    		strcpy(pLinkEntry->caLocalIp,pIp1);
    		pLinkEntry->sLocalPort = atoi(pPort1); //listening port
    		strcpy(pLinkEntry->caRemoteIp,pIp2);
    		pLinkEntry->sRemotePort = atoi(pPort2);
    		pLinkEntry->iTimeOut = atoi(pTime);
    		pLinkEntry->lFlags |= DCS_FLAG_USED; //mark as used
    		//pLinkEntry->MacFlag=pMacFlag[0];
    		if ( memcmp(pNotiFlag,"yes",3)==0)
    				pLinkEntry->iNotifyFlag=1;
    		else
    				pLinkEntry->iNotifyFlag=0;
    		if ( memcmp(pCheckFlag,"yes",3)==0)
    	  	   pLinkEntry->caCheckFlag = 1;
    	  else
    	  		pLinkEntry->caCheckFlag=0;
    		memcpy( pLinkEntry->caMsgType, pMsgType,4); 
                if(pNotiFold)
    		memcpy(pLinkEntry->caNotiFold,pNotiFold,strlen(pNotiFold)<MAX_FOLDNAME_SIZE? strlen(pNotiFold):MAX_FOLDNAME_SIZE);
    		dcs_debug(0,0, "hst link %d setup finish ",iSlotIndex);  
    		if ( pLinkEntry->iStatus != DCS_STAT_STOPPED)
      	{
      		kill(pLinkEntry->pidComm,SIGUSR2);  //SIGTERM
      		wait(NULL);
//      		usleep(5000);
    		}   
    		StartComm( iSlotIndex);
    		
  	}//for

  	conf_close(iFd);
  	return (iSlotIndex > 0 ? 0 : -1);

 	
}

static void DoSeachCheckPoint()
{
	time_t t;
	int i;
	sigval_t k;
	 for( ; ; )
	 {
			if ( g_pLinkCtrl == NULL ) break;
			time(&t);
			for (i=0; i <= g_pLinkCtrl->iLinkCnt-1; i++)  
			{
          if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
			  			continue;
			    if ( g_pLinkCtrl->linkArray[i].iStatus != DCS_STAT_CONNECTED)
          {
//        		dcs_debug(0,0," status  not s connected ,fold name =[%s]",g_pLinkCtrl->linkArray[i].caFoldName);
			    		continue;
				  }
			    if (!g_pLinkCtrl->linkArray[i].caCheckFlag)
          {
 //                       	dcs_debug(0,0," link notify flag is no ,fold name =[%s]",g_pLinkCtrl->linkArray[i].caFoldName);
			    	  continue;
          }
			    if (g_pLinkCtrl->linkArray[i].iTimeOut <=0)
          {
  //                      	dcs_debug(0,0," link  timeout <=0 ,fold name =[%s]",g_pLinkCtrl->linkArray[i].caFoldName);
			    	  continue;
          }
			    if ( (t- g_pLinkCtrl->linkArray[i].iRcvTime) < (g_pLinkCtrl->linkArray[i].iTimeOut-2) )
			    {
   //                     	dcs_debug(0,0," timeout is not recive ,fold name =[%s],last time=[%ld],now time=[%ld]",g_pLinkCtrl->linkArray[i].caFoldName,g_pLinkCtrl->linkArray[i].iRcvTime,t);
			 	    continue;
          }
    //                        dcs_debug(0,0,"send net check msg pid[%d]",g_pLinkCtrl->linkArray[i].pidCheck);
			    sigqueue(g_pLinkCtrl->linkArray[i].pidCheck, 40, k);
			}
			sleep(1);
	 }
	 
}
