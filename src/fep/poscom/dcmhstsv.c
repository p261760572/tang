#define _MainProg

// �������ƣ�dcmhstsv.c
// ����������
//           ������ɻ����Ľ���
// �����嵥��
//           main()

#include  "ibdcs.h"
#include  "dccdcs.h"

static struct trmcom	*g_pLinkCtrl = NULL; 

static int		g_iPosSrvFoldId = -1;

static void PosSvrExit(int signo);



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

  	catch_all_signals(PosSvrExit);
        signal( 17, SIG_IGN);  
	
   	if ( argc >=2 && strcmp(argv[1] ,"1")==0)
   			;
   	else
  			u_daemonize(NULL);

  	//prepare the log stuff
  	if (0 > PosSrvOpenLog())
    		goto lblExit;
    if(dcs_connect_shm() < 0)
    {
        dcs_log(0,0,"dcs_connect_shm() failed:%s\n",strerror(errno));
        exit(1);
    }
   	if (fold_initsys() < 0)
  	{
    		dcs_log(0,0, "<PosSrv> cannot attach to FOLDER !");
    		goto lblExit;
  	}
  	g_iPosSrvFoldId = fold_create_folder(DCS_POSSRV_FOLD_NAME);
  	if (g_iPosSrvFoldId < 0)  
      		g_iPosSrvFoldId = fold_locate_folder(DCS_POSSRV_FOLD_NAME);
  	if (g_iPosSrvFoldId < 0)  
  	{
    		dcs_log(0,0,"<PosSrv> cannot create folder '%s'%d!",\
                      DCS_POSSRV_FOLD_NAME,(errno));
    		
    		goto lblExit;
  	}
  	fold_set_attr(g_iPosSrvFoldId, FOLD_FLAG_NEEDSIG);
  	//���ӹ����ڴ�BCDA
  
  	g_pcBcdaShmPtr = (char *)shm_connect(DCS_BCDA_NAME,NULL);
  	if (g_pcBcdaShmPtr == NULL)
  	{
    		dcs_log(0,0,"<PosSrv> cannot connect shared memory %s!",DCS_BCDA_NAME);
    		
    		goto lblExit;
  	}
  	//��ȡ�����ļ�,�����·����
  	if ((iCommCnt = LoadCommCounter()) < 0)
  	{
    		dcs_log(0,0, "<PosSrv> Failure to load possvr.conf 1!");
    		goto lblExit;
  	}    
  	//���������ڴ�HSTCOM������¼��ͨ�Ž��̵�״̬
  	iShmSize = sizeof(struct trmcom) + (iCommCnt-1) * sizeof(struct trminfo);
  	g_pLinkCtrl=(struct trmcom *)shm_create(DCS_POSCOM_NAME,iShmSize,&iTmpShmid);
  	if (g_pLinkCtrl==NULL)
  	{
    		dcs_log(0,0, "<PosSrv> cannot create shared memory as links array!");
    		goto lblExit;
  	}
  	else
  	{  
    		memset(g_pLinkCtrl,0,iShmSize); 
    		g_pLinkCtrl->iTrmCnt = iCommCnt;
    		g_pLinkCtrl->iShmId   = iTmpShmid;
  	}    
 
    	//��ȡͨѶ���������ļ�isdcmlnk.conf
  	if (LoadCommLnk() < 0)
  	{
    		dcs_log(0,0, "<PosSrv> Failure to load possvr.conf 2!" );
    		goto lblExit;
  	}

    	//try to start up all communication processes
  	if ( 0 > StartComm(-1))
    		goto lblExit;
  
  	dcs_log(0,0, "<PosSrv>startup complete, going to service!");
  	DoService();

lblExit:  
  	//release all resources and exit
  	
  	PosSvrExit(0);
}//main()


// ��������  HstSvrExit
// ����˵����
//          signo   ������ź�ֵ
// �� �� ֵ��
//          ��
// ����������
//          ��������ź�Ϊ SIGTERMʱ���ú����ͷ���Դ����ֹ��ǰ����;����
//          �����κδ���

void PosSvrExit(int signo)
{
  	int i, logLevel;

  	
  
  	if (signo == SIGUSR2 || signo == SIGCHLD)
    		logLevel = 999;
  	else
    		logLevel = 1000;
  	if (signo != SIGUSR1)  
  	{
    		dcs_log(0,0,"<PosSrv> catch a signal %d !",signo);
    		
  	}  
  
  	if (signo != SIGTERM && signo != 0 && signo != SIGSEGV)
  	{
    		
    		return;
  	}  

  	//notify all comm processes to exit
  	if (g_pLinkCtrl)
  	{
    		for (i=0; i < g_pLinkCtrl->iTrmCnt; i++)  
    		{
      			if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
        			continue;
      			if (g_pLinkCtrl->linkArray[i].pidComm <= 0)
        			continue;
    
      			kill(g_pLinkCtrl->linkArray[i].pidComm,SIGTERM);
 
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
  
  	fold_purge_msg(g_iPosSrvFoldId);
  	fold_delete_folder(g_iPosSrvFoldId);

  	
  	exit(signo);
}

// ��������  PosSrvOpenLog
// ����˵����
//          ��
// �� �� ֵ��
//          �ɹ�����0,����-1
// ����������
//          1.���ϵͳ��Ŀ¼,��֮����ȫ�ֱ���g_pcIseHome
//          2.������־�ļ�����־��ʽ

int PosSrvOpenLog()
{
  	   
  	char logfile[256];

    if(u_fabricatefile("log/possvr.log",logfile,sizeof(logfile)) < 0)
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

  	 if(u_fabricatefile("config/possvr.conf",caFileName,sizeof(caFileName)) < 0)
	  {
	    dcs_log(0,0,"cannot get full path name of 'possvr.conf'\n");
	    return -1;
	  }

  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log(0,0, "<PosSrv> open isdcmlnk.ini fail ! ");
    		return (-1);
  	}

  	//��ȡ������'comm.max'
  	if ( 0 >conf_get_first_number(iFd, "comm.maxgrp",&iCommCnt) || iCommCnt < 0 )
  	{
    		dcs_log(0,0, "<PosSrv> load configuration item 'comm.maxgrp' failed!");
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
  	struct trminfo *pLinkEntry=NULL;
  	char   *pLinkFldName,*pApplFldName,*pRole, *pAddr,*pGrp,*pMax,*pMin;
  	char   *pPort1, *pPort2 , *pTime, *pMsgType,*pHeadType;     

  	

  	//�趨commlnk�����ļ��ľ���·��������
  	memset(caFileName, 0, sizeof(caFileName) );
    if(u_fabricatefile("config/possvr.conf",caFileName,sizeof(caFileName)) < 0)
	  {
	    dcs_log(0,0,"cannot get full path name of 'possvr.conf'\n");
	    return -1;
	  }
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log(0,0, "<PosSrv> open isdcmlnk.conf fail ! ");
    		return (-1);
  	}
   	//��ȡ���е�������'comm'
  	for( iRc = conf_get_first_string(iFd, "comm.grp",caBuffer);\
       		iRc == 0 && iSlotIndex < g_pLinkCtrl->iTrmCnt;\
       		iRc = conf_get_next_string(iFd, "comm.grp",caBuffer))
  	{
  		pGrp    = strtok(caBuffer," \t\r");  //ͨ������
  		pMax      = strtok(NULL," \t\r");   //ͨ��˳���
  		pMin      = strtok(NULL," \t\r");   //ͨ��˳���
    		pLinkFldName = strtok(NULL," \t\r");  //�ļ�����
    		pApplFldName = strtok(NULL," \t\r");  //Ӧ���ļ�����
    		pRole        = strtok(NULL   ," \t\r");  //��ɫ
    		pMsgType        = strtok(NULL   ," \t\r");  //��������
    		pHeadType        = strtok(NULL   ," \t\r");  //����ͷ����
    		pPort1        = strtok(NULL   ," \t\r");  //���ض˿�
    		pAddr       = strtok(NULL   ," :");  //Զ�̵�ַ
    		pPort2       = strtok(NULL   ," :\t\r\n");//Զ�̶˿�
   
    		if ( pLinkFldName==NULL  || pRole==NULL || pAddr==NULL || pMsgType==NULL )
    		{
      			//invalid address:skip this line
      			continue;
    		}   
    
     		//�����͸�link�������ļ���
    		pLinkEntry = &(g_pLinkCtrl->linkArray[iSlotIndex++]);
    		strcpy(pLinkEntry->caFoldName,pLinkFldName);
    		pLinkEntry->iFid = fold_create_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0 && errno == FOLD_EEXIST)
      			pLinkEntry->iFid = fold_locate_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0)
    		{
      			dcs_log(0,0, "<PosSrv> cannot create folder '%s' !",pLinkFldName);   
      			
      			goto lblFailure;
    		}
    
    		//��λ�͸�link������Ӧ�ý��̵��ļ���
    		pLinkEntry->iApplFid = fold_locate_folder(pApplFldName);
    		if (pLinkEntry->iApplFid < 0)
    		{
      			dcs_log(0,0, "<PosSrv> cannot locate folder '%s' !",pApplFldName);   
      			
      			goto lblFailure;
    		}
    		//���������ӽ��������б��س䵱�Ľ�ɫ
    		if (u_stricmp(pRole,"passive")==0 )
      			pLinkEntry->iRole = DCS_ROLE_PASSIVE;
    		else if (u_stricmp(pRole,"active")==0 )
      			pLinkEntry->iRole = DCS_ROLE_ACTIVE;
        else
        {
        			dcs_log(0,0," Unkown Role :[%s]",pRole);
        	     goto lblFailure;         
        }
                //do with the address
          pLinkEntry->iGid= atoi(pGrp);
    		pLinkEntry->maxterm=atoi(pMax);
    		pLinkEntry->minterm=atoi(pMin);
    		pLinkEntry->maxterm=atoi(pMax);
    		memcpy(pLinkEntry->sMsgType,pMsgType,4);
    		if ( u_stricmp(pHeadType,"trm") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_TRM;
    		else if ( u_stricmp(pHeadType,"bnk") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_BANK;
    		else if ( u_stricmp(pHeadType,"zfy") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_ZFY;
    		else if ( u_stricmp(pHeadType,"amp") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_AMP;
    		else if ( u_stricmp(pHeadType,"hex2") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_TRM1;
    		else
    		{
    					dcs_log(0,0,"Unkonw comm type :[%s]",pHeadType);
    					goto lblFailure;
    		}
    		pLinkEntry->iLocalPort = atoi(pPort1); //listening port
    		strcpy(pLinkEntry->sRemoteIp,pAddr);
    		pLinkEntry->iRemotePort = atoi(pPort2);
    		pLinkEntry->lFlags |= DCS_FLAG_USED; //mark as used
    		pLinkEntry->iStatus = DCS_STAT_STOPPED;
    		//pLinkEntry->MacFlag=pMacFlag[0];
    		dcs_debug(0,0, "pos link %d setup finish ,grp id =%d",iSlotIndex-1, pLinkEntry->iGid);   
    		
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
     		nUpper = g_pLinkCtrl->iTrmCnt-1;  
  	}
  	else   
    		nLow = nUpper = iSlot;
    	memset(mBuf,0,sizeof(mBuf));
    dcs_debug(0,0,"begin start process num=[%d]",iSlot);
  	for (i=nLow; i <= nUpper; i++)  
  	{
  		  if ( strlen(g_pLinkCtrl->linkArray[i].caFoldName)<1)
  		  		break;
    		if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;

    		if ( (pidChld = fork()) < 0)
    		{
      			dcs_log(0,0, "<PosSrv> cannot fork child process!");
      			return (-1);
    		}
    
    		if (pidChld > 0)
    		{
      			//i am parent,write down the child's pid
      			g_pLinkCtrl->linkArray[i].pidComm = pidChld;  
      			continue;
    		}
        dcs_debug(0,0,"starting %d process",i);
    		//child here, try exec 'postcp'
    		sprintf(mBuf,"%d",g_pLinkCtrl->linkArray[i].iGid);
    		iRc = execlp ("postcp", "postcp",mBuf ,(char *)0);
    		if (iRc < 0)
    		{
      			dcs_log(0,0,"<PosSrv> cannot exec executable 'postcp.x':%s!",strerror(errno));
      			exit(-1);    
    		}
    		
  	}//for
    dcs_debug(0,0,"starting process num =%d",i-nLow);
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
  
  	
  
  	fold_purge_msg(g_iPosSrvFoldId);

  	for (;;)
  	{
    		//read request from my folder in non-blocking mode
    		dcs_debug(0,0, "<HstSvr--DoService> Read hstsvr fold in non-blocking mode!" );
    		memset(caMsg,0,sizeof(caMsg));
    		nRead = fold_read(g_iPosSrvFoldId, &iOrgFid, caMsg,sizeof(caMsg),TRUE);
    		if (nRead < 0)
    		{
	      			if (errno != ENOMSG && errno != EINTR)
	      			{  
	        			//read error
	        			dcs_log(0,0,"<PosSrv> fold_read() failed:%d!",(errno));
	        			break;
	      			}  
	
	      			if (fold_sys_active() < 0)
	        			break;
/*
	#if 1
							dcs_debug(0,0, "<PosSrv--DoService> Before wait!");
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
	
	      			//read request from my folder in blocking mode
	      			dcs_debug(0,0, "<PosSrv--DoService> After wait,Read hstsvr fold in blocking mode!");
	      			nRead = fold_read(g_iPosSrvFoldId, &iOrgFid, caMsg,sizeof(caMsg),TRUE);
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
    		AnswerRequest(caMsg);

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
  	

  	switch (iMsgType)
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
  
  	return (0);
}

int HandleCommDie(pid_t pidChld)
{
  	int  i, find;
  
  	
  
  	//locate the corresponding comm process
  	for (find=-1,i=0; i < g_pLinkCtrl->iTrmCnt; i++)  
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
    		dcs_log(0,0,"<PosSrv> a comm process died,but cannot "
                    "locate the associated link info");
    		
    		return (-1);
  	}
 /* 
  	if ( kill (g_pLinkCtrl->linkArray[find].pidComm,0) > 0)
  	{
      		//the child still alive
      		return (0);
  	}
*/
  	//update the net status in sharead memory BCDA
//  	UpdateNetLink(g_pLinkCtrl->linkArray[find].caFoldName,DCS_STAT_DISCONNECTED);
                
  	//restart comm
  	sleep(1);
  	if (g_pLinkCtrl->linkArray[find].iStatus != DCS_STAT_STOPPED)
  	{
   		dcs_log(0,0,"<PosSrv> comm process for [%s ] died,try reload it...",\
                    	g_pLinkCtrl->linkArray[find].caFoldName);
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

int HandleLinkEstab(char *paMsg)
{
  	struct commnotify *pNoti;
  	int  i, find;
  
  	
  
  	pNoti = (struct commnotify *)paMsg;

  	//locate the corresponding comm process
  	for (find=-1,i=0; i < g_pLinkCtrl->iTrmCnt; i++)
  	{
    		if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (u_stricmp(g_pLinkCtrl->linkArray[i].caFoldName,pNoti->caFoldName) != 0)
      			continue;

    		find = i;
    		break;
  	}

  	if (find < 0)
  	{
    		dcs_log(0,0,"<PosSrv> a comm process report change of link status,"\
                    "but cannot locate the associated link info");
    		
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
      		dcs_log(0,0,"<PosSrv>connection to [%s] established.",tmpName);
      		

      		//update the net status in BCDA
      		UpdateNetLink(pNoti->caFoldName,DCS_STAT_CONNECTED);
  	}

  	return (0);
}

int HandleCommandLine(char *paMsg)
{
  	struct trmcmd *pCmd;
  	int    i, find;
  
  
  	pCmd = (struct trmcmd *)paMsg;
  
  	if (pCmd->iCommand == DCS_CMD_EXIT)
  	{
    		raise(SIGTERM);
    		return (0);
  	}
  	if (pCmd->iCommand ==  DCS_CMD_ADD )
  	{
    	  dcs_debug(0,0,"<PosSrv> add link [%s]",pCmd->caFoldName);
    	  TcpAddLink( pCmd->caFoldName );
    		return (0);
		}
		if (pCmd->iCommand ==  DCS_CMD_MODIFY )
		{
			dcs_log(0,0,"<PosSrv> modify %s",pCmd->caFoldName);
			TcpModiLink( pCmd->caFoldName );
			return(0);
	  }
		
  	//locate the intended link entry
  	for (find=-1,i=0; i < g_pLinkCtrl->iTrmCnt; i++)  
  	{
    		if (!(g_pLinkCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (g_pLinkCtrl->linkArray[i].iGid != atoi(pCmd->caFoldName)) 
		      			continue;
      		switch (pCmd->iCommand)
			  	{
			    	case DCS_CMD_STOP:
//			      		pCmd->caFoldName[4]=0;
			      		dcs_debug(0,0,"<PosSrv> stop [%s]",pCmd->caFoldName);
			      		if ( g_pLinkCtrl->linkArray[i].iStatus != DCS_STAT_STOPPED)
			      		{
			        		kill(g_pLinkCtrl->linkArray[i].pidComm,SIGUSR2);  //SIGTERM
//			        		wait(NULL);
//			        		g_pLinkCtrl->linkArray[i].iStatus = DCS_STAT_STOPPED;

			      		}   
			      		find = i;
			      		break;
			
			    	case DCS_CMD_START:
//			      		pCmd->caFoldName[4]=0;
			      		dcs_debug(0,0,"<PosSrv> start [%s]",pCmd->caFoldName);
			      		StartComm(i);
			      		find = i;
			      		break;
			      case DCS_CMD_DEL:
			      	  dcs_debug(0,0,"delete tcp link , free a solt");
			      	  kill(g_pLinkCtrl->linkArray[i].pidComm,SIGUSR2);  //SIGTERM
//			        	wait(NULL);
			      	  g_pLinkCtrl->linkArray[i].lFlags &= ~DCS_FLAG_USED;
			      	  find = i;
			    	default:
			      		break;
			  	}
      
      
//    		find = i;
//   		break;  
  	}
  
  	if (find < 0)
    {
          dcs_log(0,0,"not found %s",pCmd->caFoldName);
    			return (-1);
    }

  
  	return (0);
}

int TcpAddLink ( char * pcFoldName )
 {
 	
   	char   caFileName[256], caBuffer[1024];
  	int    iFd=-1, iRc,iSlotIndex = 0;  
  	struct trminfo *pLinkEntry=NULL;
  	char   *pLinkFldName,*pApplFldName,*pRole, *pAddr,*pGrp,*pMax,*pMin;
  	char   *pPort1, *pPort2 , *pTime, *pMsgType,*pHeadType;     

  	

  	//�趨commlnk�����ļ��ľ���·��������
  	memset(caFileName, 0, sizeof(caFileName) );
    if(u_fabricatefile("config/possvr.conf",caFileName,sizeof(caFileName)) < 0)
	  {
	    dcs_log(0,0,"cannot get full path name of 'possvr.conf'\n");
	    return -1;
	  }
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log(0,0, "<PosSrv> open isdcmlnk.conf fail ! ");
    		return (-1);
  	}

  	//��ȡ���е�������'comm'
  	for( iRc = conf_get_first_string(iFd, "comm.grp",caBuffer);\
       		iRc == 0 && iSlotIndex < g_pLinkCtrl->iTrmCnt;\
       		iRc = conf_get_next_string(iFd, "comm.grp",caBuffer))
  	{
  		pGrp    = strtok(caBuffer," \t\r");  //ͨ������
  		pMax      = strtok(NULL," \t\r");   //ͨ��˳���
  		pMin      = strtok(NULL," \t\r");   //ͨ��˳���
    		pLinkFldName = strtok(NULL," \t\r");  //�ļ�����
    		pApplFldName = strtok(NULL," \t\r");  //Ӧ���ļ�����
    		pRole        = strtok(NULL   ," \t\r");  //��ɫ
    		pMsgType        = strtok(NULL   ," \t\r");  //��������
    		pHeadType        = strtok(NULL   ," \t\r");  //����ͷ����
    		pPort1        = strtok(NULL   ," \t\r");  //���ض˿�
    		pAddr       = strtok(NULL   ," :");  //Զ�̵�ַ
    		pPort2       = strtok(NULL   ," :\t\r\n");//Զ�̶˿�
   
    		if ( pLinkFldName==NULL  || pRole==NULL || pAddr==NULL || pMsgType==NULL )
    		{
      			//invalid address:skip this line
      			dcs_debug(0,0,"pgrp id =%s group id =%s",pLinkFldName,pGrp);
      			continue;
    		}   
        if ( u_stricmp( pGrp ,pcFoldName) !=0)
        		continue;
        for ( iSlotIndex =0; iSlotIndex < g_pLinkCtrl->iTrmCnt;iSlotIndex++)
        {
        	     if ( (g_pLinkCtrl->linkArray[iSlotIndex].lFlags &DCS_FLAG_USED )
        	     	    && atoi(pcFoldName)==g_pLinkCtrl->linkArray[iSlotIndex].iGid)
               {
               	   dcs_log(0,0," group is exist,can not add!");
               	   goto lblFailure1;
               }      
        }
        for ( iSlotIndex =0; iSlotIndex < g_pLinkCtrl->iTrmCnt;iSlotIndex++)
        {
        	         if ( !(g_pLinkCtrl->linkArray[iSlotIndex].lFlags &DCS_FLAG_USED ))
                            break;
        }
        if ( iSlotIndex >= g_pLinkCtrl->iTrmCnt)
        {
        	 dcs_log(0,0,"Not free slot");
        	 goto lblFailure1;
        }
     		//�����͸�link�������ļ���
    		pLinkEntry = &(g_pLinkCtrl->linkArray[iSlotIndex]);
    		if ( u_stricmp("NULL",pLinkFldName) != 0 && u_stricmp("null",pLinkFldName) != 0)
    		{
		    		strcpy(pLinkEntry->caFoldName,pLinkFldName);
		    		pLinkEntry->iFid = fold_create_folder(pLinkFldName);
		    		if (pLinkEntry->iFid < 0 && errno == FOLD_EEXIST)
		      			pLinkEntry->iFid = fold_locate_folder(pLinkFldName);
		    		if (pLinkEntry->iFid < 0)
		    		{
		      			dcs_log(0,0, "<PosSrv> cannot create folder '%s' !",pLinkFldName);   
		      			
		      			goto lblFailure1;
		    		}
        }
    		//��λ�͸�link������Ӧ�ý��̵��ļ���
    		pLinkEntry->iApplFid = fold_locate_folder(pApplFldName);
    		if (pLinkEntry->iApplFid < 0)
    		{
      			dcs_log(0,0, "<PosSrv> cannot locate folder '%s' !",pApplFldName);   
      			
      			goto lblFailure1;
    		}
    		//���������ӽ��������б��س䵱�Ľ�ɫ
    		if (u_stricmp(pRole,"passive")==0 )
      			pLinkEntry->iRole = DCS_ROLE_PASSIVE;
    		else if (u_stricmp(pRole,"active")==0 )
      			pLinkEntry->iRole = DCS_ROLE_ACTIVE;
        else
             goto lblFailure1;         
                //do with the address
        pLinkEntry->iGid= atoi(pGrp);
    		pLinkEntry->maxterm=atoi(pMax);
    		pLinkEntry->minterm=atoi(pMin);
    		pLinkEntry->maxterm=atoi(pMax);
    		memcpy(pLinkEntry->sMsgType,pMsgType,4);
    		if ( u_stricmp(pHeadType,"trm") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_TRM;
    		else if ( u_stricmp(pHeadType,"bnk") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_BANK;
    		else if ( u_stricmp(pHeadType,"zfy") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_ZFY;
    		else if ( u_stricmp(pHeadType,"amp") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_AMP;
    		else if ( u_stricmp(pHeadType,"hex2") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_TRM1;
    		else
    				goto lblFailure1;
    		pLinkEntry->iLocalPort = atoi(pPort1); //listening port
    		strcpy(pLinkEntry->sRemoteIp,pAddr);
    		pLinkEntry->iRemotePort = atoi(pPort2);
    		pLinkEntry->lFlags |= DCS_FLAG_USED; //mark as used
    		pLinkEntry->iStatus = DCS_STAT_STOPPED;
    		//pLinkEntry->MacFlag=pMacFlag[0];
    		dcs_debug(0,0, "pos link %d setup finish ",iSlotIndex);   
    		
  	}//for

  	conf_close(iFd);
  	return (iSlotIndex > 0 ? 0 : -1);

lblFailure1:
  	conf_close(iFd);
  	return (-1);

 	
}

int TcpModiLink ( char * pcFoldName )
 {
 	
  	char   caFileName[256], caBuffer[1024];
  	int    iFd=-1, iRc,iSlotIndex = 0;  
  	struct trminfo *pLinkEntry=NULL;
  	char   *pLinkFldName,*pApplFldName,*pRole, *pAddr,*pGrp,*pMax,*pMin;
  	char   *pPort1, *pPort2 , *pTime, *pMsgType,*pHeadType;     

  	

  	//�趨commlnk�����ļ��ľ���·��������
  	memset(caFileName, 0, sizeof(caFileName) );
    if(u_fabricatefile("config/possvr.conf",caFileName,sizeof(caFileName)) < 0)
	  {
	    dcs_log(0,0,"cannot get full path name of 'possvr.conf'\n");
	    return -1;
	  }
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log(0,0, "<PosSrv> open isdcmlnk.conf fail ! ");
    		return (-1);
  	}
 
  	//��ȡ���е�������'comm'
  	for( iRc = conf_get_first_string(iFd, "comm.grp",caBuffer);\
       		iRc == 0 && iSlotIndex < g_pLinkCtrl->iTrmCnt;\
       		iRc = conf_get_next_string(iFd, "comm.grp",caBuffer))
  	{
  		pGrp    = strtok(caBuffer," \t\r");  //ͨ������
  		pMax      = strtok(NULL," \t\r");   //ͨ��˳���
  		pMin      = strtok(NULL," \t\r");   //ͨ��˳���
    		pLinkFldName = strtok(NULL," \t\r");  //�ļ�����
    		pApplFldName = strtok(NULL," \t\r");  //Ӧ���ļ�����
    		pRole        = strtok(NULL   ," \t\r");  //��ɫ
    		pMsgType        = strtok(NULL   ," \t\r");  //��������
    		pHeadType        = strtok(NULL   ," \t\r");  //����ͷ����
    		pPort1        = strtok(NULL   ," \t\r");  //���ض˿�
    		pAddr       = strtok(NULL   ," :");  //Զ�̵�ַ
    		pPort2       = strtok(NULL   ," :\t\r\n");//Զ�̶˿�
   
    		if ( pLinkFldName==NULL  || pRole==NULL || pAddr==NULL || pMsgType==NULL )
    		{
      			//invalid address:skip this line
      			continue;
    		}   
      	if ( u_stricmp( pGrp ,pcFoldName) !=0)
        		continue;
        for ( iSlotIndex =0; iSlotIndex < g_pLinkCtrl->iTrmCnt;iSlotIndex++)
        {
        	     if ( (g_pLinkCtrl->linkArray[iSlotIndex].lFlags &DCS_FLAG_USED )
        	     	    && atoi(pcFoldName)==g_pLinkCtrl->linkArray[iSlotIndex].iGid)
                  break;      
        }
/*
     		for ( iSlotIndex =0; iSlotIndex < g_pLinkCtrl->iTrmCnt;iSlotIndex++)
                 if ( !(g_pLinkCtrl->linkArray[iSlotIndex].lFlags &DCS_FLAG_USED ))
                            break;
*/
        if ( iSlotIndex >= g_pLinkCtrl->iTrmCnt)
        {
        	 dcs_log(0,0,"Not free slot");
        	 goto lblFailure2;
        }
    		pLinkEntry = &(g_pLinkCtrl->linkArray[iSlotIndex]);
    		strcpy(pLinkEntry->caFoldName,pLinkFldName);
    		pLinkEntry->iFid = fold_create_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0 && errno == FOLD_EEXIST)
      			pLinkEntry->iFid = fold_locate_folder(pLinkFldName);
    		if (pLinkEntry->iFid < 0)
    		{
      			dcs_log(0,0, "<PosSrv> cannot create folder '%s' !",pLinkFldName);   
      			
      			goto lblFailure2;
    		}
    
    		//��λ�͸�link������Ӧ�ý��̵��ļ���
    		pLinkEntry->iApplFid = fold_locate_folder(pApplFldName);
    		if (pLinkEntry->iApplFid < 0)
    		{
      			dcs_log(0,0, "<PosSrv> cannot locate folder '%s' !",pApplFldName);   
      			
      			goto lblFailure2;
    		}
    		//���������ӽ��������б��س䵱�Ľ�ɫ
    		if (u_stricmp(pRole,"passive")==0 )
      			pLinkEntry->iRole = DCS_ROLE_PASSIVE;
    		else if (u_stricmp(pRole,"active")==0 )
      			pLinkEntry->iRole = DCS_ROLE_ACTIVE;
        else
             goto lblFailure2;         
                //do with the address
        pLinkEntry->iGid= atoi(pGrp);
    		pLinkEntry->maxterm=atoi(pMax);
    		pLinkEntry->minterm=atoi(pMin);
    		pLinkEntry->maxterm=atoi(pMax);
    		memcpy(pLinkEntry->sMsgType,pMsgType,4);
    		if ( u_stricmp(pHeadType,"trm") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_TRM;
    		else if ( u_stricmp(pHeadType,"bnk") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_BANK;
    		else if ( u_stricmp(pHeadType,"zfy") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_ZFY;
    		else if ( u_stricmp(pHeadType,"amp") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_AMP;
    		else if ( u_stricmp(pHeadType,"hex2") ==0)
    		    		pLinkEntry->iHeadType = DCS_HEAD_TRM1;
    		else
    				goto lblFailure2;
    		pLinkEntry->iLocalPort = atoi(pPort1); //listening port
    		strcpy(pLinkEntry->sRemoteIp,pAddr);
    		pLinkEntry->iRemotePort = atoi(pPort2);
    		pLinkEntry->lFlags |= DCS_FLAG_USED; //mark as used
    		//pLinkEntry->MacFlag=pMacFlag[0];
    		dcs_debug(0,0, "pos link %d setup finish group id =%d",iSlotIndex,pLinkEntry->iGid);   
    		
  	}//for

  	conf_close(iFd);
  	return (iSlotIndex > 0 ? 0 : -1);

lblFailure2:
  	conf_close(iFd);
  	return (-1);
 	
}

