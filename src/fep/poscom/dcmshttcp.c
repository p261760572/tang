#include "ibdcs.h"
#include "dccdcs.h"

//globals used only in this file
static   struct trmcom  *g_pTrmCom  =NULL ;
static   struct trmgrpinfo *g_pTrmArray=NULL;
static   struct trminfo *g_pLinkInfo=NULL;
static   char           *g_pcIseHome=NULL ;
static   int             g_iTrmFoldId= -1;
static   int             g_pidTrmSvr = -1;

static   int             g_iCaFoldId =-1; // cash's recvice_folder
static   int             g_iSock = -1;
static   int             sg_iSock=-1;
static   int             sg_index=-1;
static   int             gTime=0;
static   int             gShmId=-1;
//forward declaration
static   void TrmSvrExit(int signo);
static   int MainTrmSvr(int iSeqNo, int nSock,int (*pfnRW)(int, int));
static   int dReadFromNet(int iSeqNo, int iConnSock);
static   int dReadFromFolder(int iSeqNo, int iConnSock);
static   int OpenLog( char *);
void AlarmMsg();
char * ConnectPosvrShm();
int main(int argc ,char *argv[])
{
	int iSize, iTrmCnt, iShmid, i;
	char caShmName[40];
	
	catch_all_signals(TrmSvrExit);
//        signal(17, SIG_IGN);	
g_pidTrmSvr = getpid();
	
 
  if ( argc <2)
  {
  		 printf(" Can not few two parameter !\n");
  			exit(1);
  }
	if(0 > OpenLog(argv[1]))
	{
		  printf(" Open Log file fail !\n");
			exit(1);
	}
  if ( atoi(getenv("SPTIME_SIGN")) >0)
       gTime=1;
  else 
  		 gTime=0;
	if(fold_initsys() < 0)
	{
		dcs_log(0,0, "<PosCom> cannot attach to FOLDER !" );
		goto lblExit;
	}
	dcs_debug(0,0 , "fold_initsys succ");
	//attach to SHM of IBDCS

	if(dcs_connect_shm() < 0)
	{
		dcs_log(0,0,"dcs_connect_shm() failed:%s\n",strerror(errno));
		goto lblExit;
	}
	dcs_debug(0,0," dcs_connect_shm succ");
	//load configuration
   g_pTrmCom = (struct trmcom*)ConnectPosvrShm();
  if ( g_pTrmCom == NULL)
  {
  		dcs_log(0,0,"connect PosCom shm failed:%s\n",strerror(errno));
			goto lblExit;
  }
  for ( i = 0; i<g_pTrmCom->iTrmCnt; i++)
  {
  	  if ( g_pTrmCom->linkArray[i].iGid == atoi(argv[1]))
  	  		 break;
  }
  if ( i >=g_pTrmCom->iTrmCnt )
  {
  		dcs_log(0,0,"Not found group id %s",argv[1]);
  		goto lblExit;
	}
 	iTrmCnt = g_pTrmCom->linkArray[i].minterm;
  g_pLinkInfo =&g_pTrmCom->linkArray[i];
	if(iTrmCnt <= 0) //no cash at all
	{
		dcs_log(0,0, "<PosCom> no cash configured!");
		goto lblExit;
	}
	iSize   =sizeof(struct trmgrpinfo) + (iTrmCnt - 1) * sizeof(struct trminfo);
	sprintf(caShmName,"%s%s",DCS_CASHMEM_NAME,argv[1]);
	g_pTrmArray=(struct trmgrpinfo *)shm_create(caShmName,iSize,&iShmid);
	if(g_pTrmArray==NULL && errno==17)
		g_pTrmArray = (struct trmgrpinfo *)shm_connect(caShmName, NULL);
	if(g_pTrmArray==NULL && errno!=17)
	{
			dcs_log(0,0,"can not create '%s'   %d ",caShmName,(errno));
  		goto lblExit;	 
  }
  dcs_debug(0,0,"create shm succ! shm id=%d",iShmid);
  g_pTrmCom->linkArray[i].trmArray = g_pTrmArray;
  g_pTrmCom->linkArray[i].iShmId = iShmid;
  gShmId = iShmid;

	g_iTrmFoldId = g_pLinkInfo->iFid;
	g_iCaFoldId =  g_pLinkInfo->iApplFid;
	InitTrmArray();
	dcs_debug(0,0,"Role =%d argv[1]=%s",  g_pLinkInfo->iRole,argv[1]);
	if ( g_pLinkInfo->iRole == DCS_ROLE_PASSIVE  )
  {
  		  	  
			  	g_iSock   = tcp_open_server(NULL,g_pLinkInfo->iLocalPort);
					if(g_iSock < 0)
					{
						dcs_log(0,0,"<PosCom> tcp_open_server() port =%d failed %d", g_pLinkInfo->iLocalPort,(errno));
						goto lblExit;
					}
					dcs_debug(0,0,"Open port [%d] , socket id =%d",g_pLinkInfo->iRemotePort,g_iSock);
			
  }
  /*******************start all children   ********************/
 	dcs_debug(0,0,"Make child process num %d",g_pLinkInfo->minterm);
	for(i=0; i < g_pLinkInfo->minterm ; i++)
	{
		if(MakeChild( g_pLinkInfo->iRole,i) > 0) continue;
		
		dcs_log(0,0, "<PosCom> cannot fork child process %d ",(errno));
		goto lblExit;
	}
  g_pLinkInfo->iStatus=DCS_STAT_CONNECTED;
	DoLoop();
lblExit:
	TrmSvrExit(0);
}//main()


int OpenLog(char *IDENT)
{
    char logfile[256];
    char tmpbuf[200];
    //assuming the log file is "$IBDCS_RUNPATH/log/ibdcs.log"
    sprintf(tmpbuf,"log/poscom%s.log",IDENT);
    if(u_fabricatefile(tmpbuf,logfile,sizeof(logfile)) < 0)
        return -1;

    return dcs_log_open(logfile, IDENT);
}
void AlarmMsg()
{
	dcs_log(0,0,"gotting Alarm ( time out )");

		close(sg_iSock);



        if( sg_index >= 0 ){

            struct trmgrpinfo *pSlot = &g_pTrmArray[sg_index];

            *pSlot->sRemoteIp = 0;

            pSlot->iRemotePort = 0;

            pSlot->iStatus = DCS_STAT_STOPPED;

        }

		exit(0); }
void TrmSvrExit(int signo)
{
  int i;


  dcs_log(0,0,"<PosSrv--%s> catch a signal %d !",g_pLinkInfo->caFoldName,signo);

  if(signo == SIGCHLD)
  {
    if(getpid() == g_pidTrmSvr)
        return;
    wait(NULL); kill(getpid(), SIGTERM);
    return;
  }
  
  if(signo != SIGTERM && signo != 0 && signo != SIGSEGV && signo != SIGUSR2)
  {
    return;
  }

  if(getpid() != g_pidTrmSvr)
    goto lblExit;

  //notify all comm processes to exit
  if(g_pTrmArray)
    for(i=0; i < g_pLinkInfo->minterm; i++)
    {
      fold_purge_msg(g_pTrmArray[i].iFid);
      fold_delete_folder(g_pTrmArray[i].iFid);

      if(g_pTrmArray[i].pidComm <= 0)
        continue;
      kill(g_pTrmArray[i].pidComm,SIGTERM);
    }

  //cleanup the link list
  if(g_pTrmArray)
  {
    int iShmid = g_pLinkInfo->iShmId;
    shm_detach((char *)g_pTrmArray);
//    shm_delete(iShmid);
    shm_delete(gShmId);
  }
  g_pLinkInfo->iStatus=DCS_STAT_STOPPED;
  //remove the folder fo myself
  
  fold_purge_msg(g_iTrmFoldId);
  fold_delete_folder(g_iTrmFoldId);
  
lblExit:
//  if( g_myTimerId>=0 )
//    tm_unset_timer( g_myTimerId );
	close(g_iSock);
  exit(signo);
}

/*
int LoadTrmCfg()
{
  int   iFd=-1;
  char cfgfile[256];
  
  int i,k,s,iGrpCnt,iMaxTerm,iPort,iId;
  char iIp[16],iType[12],sHeadType[10],sMsgType[5];
  
  if(u_fabricatefile("config/PosCom.conf",cfgfile,sizeof(cfgfile)) < 0)
  {
    dcs_log(0,0,"cannot get full path name of 'PosCom.conf'\n");
    return -1;
  }

  dcs_debug(0,0,"get full path name of %s succ",cfgfile);
  if(0 > (iFd = conf_open(cfgfile)) )
  {
    dcs_log(0,0,"open file '%s' failed.\n",cfgfile);
    return -1;
  }
  dcs_debug(0,0,"open file '%s' succ!",cfgfile);
  //load item 'comm.direct.maxterm'
	g_pTrmCom->iTrmCnt=0;
	dcs_debug(0,0,"begin get item");
  if(0>conf_get_first_number(iFd,"comm.maxgrp",&iGrpCnt)|| iGrpCnt < 0 )
  {
    dcs_log(0,0,"<PosCom> load config item 'comm.maxgrp' failed!");
                      
    conf_close(iFd);
    goto lblErr;
  }
  dcs_debug(0,0,"Get comm.maxgrp item succ");
  g_pTrmCom->iGroupCnt=iGrpCnt;
  s=0;
  for ( i =0 ;i < iGrpCnt ;i++)
  {
	  	if(0>conf_get_next_number(iFd,"comm.id",&iId)|| iId < 0 )
	    {
	        dcs_log(0,0,"<PosCom>load config item 'comm.id' failed!");
	        conf_close(iFd);
	        goto lblErr;
	    }
//	    dcs_debug ( 0,0,"get comm.id succ [%d]",iId);
	    memset(iType,0,sizeof(iType));
	    if(0>conf_get_next_string(iFd,"comm.type",&iType)|| strlen(iType)<=0  )
	    {
	        dcs_log(0,0,"<PosCom>load config item 'comm.maxterm' failed!");
	        conf_close(iFd);
	        goto lblErr;
	    }
//	    dcs_debug ( 0,0,"get comm.type succ [%s]",iType);
	  	if(0>conf_get_next_number(iFd,"comm.maxterm",&iMaxTerm)|| iMaxTerm < 0 )
	    {
	        dcs_log(0,0,"<PosCom>load config item 'comm.maxterm' failed!");
	        conf_close(iFd);
	        goto lblErr;
	    }
//	    dcs_debug ( 0,0,"get comm.maxterm succ [%d]",iMaxTerm);
	    g_pTrmCom->iTrmCnt = g_pTrmCom->iTrmCnt + iMaxTerm;
	    
	    if(0>conf_get_next_string(iFd,"comm.addr",&sMsgType)|| strlen(sMsgType) <= 0 )
	    {
	        dcs_log(0,0,"<PosCom>load config item 'comm.msgtype' failed!");
	        conf_close(iFd);
	        goto lblErr;
	    }
	    if(0>conf_get_next_string(iFd,"comm.addr",&sHeadType)|| strlen(sHeadType) <= 0 )
	    {
	        dcs_log(0,0,"<PosCom>load config item 'comm.headtype' failed!");
	        conf_close(iFd);
	        goto lblErr;
	    }
	    memset(iIp,0,sizeof(iIp));
	  	 if(0>conf_get_next_string(iFd,"comm.addr",&iIp)|| strlen(iIp) <= 0 )
	    {
	        dcs_log(0,0,"<PosCom>load config item 'comm.Ip' failed!");
	        conf_close(iFd);
	        goto lblErr;
	    }
//	    dcs_debug ( 0,0,"get comm.addr succ [%s]",iIp);
	  	 if(0>conf_get_next_number(iFd,"comm.port",&iPort)|| iPort < 0 )
	    {
	        dcs_log(0,0,"<PosCom>load config item 'comm.port' failed!");
	        conf_close(iFd);
	        goto lblErr;
	    }
//	    dcs_debug ( 0,0,"get comm.port succ [%d]",iPort);
	    for ( k =0; k< iMaxTerm; k++)
	    {
				    g_pTrmArray[s].iGid=iId;
				    if ( memcmp( iType , "passive",7) == 0 )
				    {
				    			g_pTrmArray[s].iType= 0;
				    			g_pTrmArray[s].sRemoteIp[0]=0;
				    			g_pTrmArray[s].iLocalPort=iPort;
				    }
				    else
				    {
				    		g_pTrmArray[s].iType= 1;
				    		strcpy(g_pTrmArray[s].sRemoteIp,iIp);
				    		g_pTrmArray[s].iRemotePort=iPort;
				  	}
				  	if ( memcmp(sHeadType,"trm",3) ==0)
				  				g_pTrmArray[s].iHeadType = DCS_HEAD_TRM;
				  	else if ( memcmp(sHeadType,"bnk",3) ==0)
				  				g_pTrmArray[s].iHeadType = DCS_HEAD_BANK;
				  	else if ( memcmp(sHeadType,"zfy",3) ==0)
				  				g_pTrmArray[s].iHeadType = DCS_HEAD_ZFY;
				    else
				    {
				    		dcs_log(0,0,"error item comm.headtype =%s",sHeadType);
				    		conf_close(iFd);
				    		goto lblErr;
				    }
				    s++;
				    
	  	}
	}
 
  conf_close(iFd);
  return 0;

lblErr:
  conf_close(iFd);
  return -1;
}

*/
int InitTrmArray()
{

  int i;
   
  for(i=0; i <  g_pLinkInfo->minterm; i++)
  {
    
		  	if (  g_pLinkInfo->iRole == DCS_ROLE_PASSIVE )
		    		sprintf(g_pTrmArray[i].caFoldName,"%s%d%d",g_pLinkInfo->caFoldName,g_pLinkInfo->iGid, i);
		    else
		        sprintf(g_pTrmArray[i].caFoldName,"%s%d",g_pLinkInfo->caFoldName,g_pLinkInfo->iGid);
		 		
		    g_pTrmArray[i].iFid = fold_create_folder(g_pTrmArray[i].caFoldName);
		    if(g_pTrmArray[i].iFid < 0 && errno == FOLD_EEXIST)
		      g_pTrmArray[i].iFid = fold_locate_folder(g_pTrmArray[i].caFoldName);
		    if(g_pTrmArray[i].iFid < 0)
		    {
		      dcs_log(0,0,"<PosCom> cannot create folder! %d ",(errno));
		      return -1;
		    }
		    fold_set_maxmsg(g_pTrmArray[i].iFid, 5) ;
		    memcpy(g_pTrmArray[i].sRemoteIp,g_pLinkInfo->sRemoteIp,16);
		    g_pTrmArray[i].iRemotePort=g_pLinkInfo->iRemotePort;
		    g_pTrmArray[i].lFlags |= DCS_FLAG_USED;
		    g_pTrmArray[i].pidComm = -1;
//		    dcs_debug(0,0," InintTrmArry seqno =%d ,port =%d",i,g_pTrmArray[i].sLocalPort);
  }//for
  return 1;

}

int MakeChild( int iType ,int iSeqNo)
{
  int iChld = -1 ;
   
  iChld=fork();
  if(iChld == 0)
  {
    //child here
    
    if(iType == DCS_ROLE_PASSIVE )
    {
    		MainTrmSvr(iSeqNo,g_iSock,dReadFromNet);

        exit(0);
    }
    else 
    {
    	
    	  MainTrmSvr(iSeqNo,-1,dReadFromFolder);
        exit(0);
    	
  	}


  }//if fork()

  //parent return
  dcs_debug(0,0,"child process id=%d, socket=%d ",iChld,g_iSock);
  g_pTrmArray[iSeqNo].pidComm = iChld;
  return iChld;
}

int MainTrmSvr(int iSeqNo, int nSock ,int (*pfnRW)(int, int))
{
  int    iConnSock,iCliAddr,iCliPort;
  struct trmgrpinfo *pSlot;
  struct in_addr in; char *strPeerIp;
  
 
  pSlot =  &g_pTrmArray[iSeqNo];

 
  fold_unset_attr(pSlot->iFid, FOLD_FLAG_NEEDSIG);
    
  if(nSock < 0)  //write and read folder
  {
    pfnRW(iSeqNo, -1);
    exit(0);
  }

  for(;;)
  {
    //waiting for clients
    pSlot->iStatus = DCS_STAT_LISTENING;
    pSlot->sRemoteIp[0]   = 0;
    pSlot->iRemotePort = 0;
    dcs_debug(0,0,"begin wait tcp conneting ..");
    iConnSock = tcp_accept_client(nSock,&iCliAddr, &iCliPort);
    if(iConnSock < 0)
    {
      dcs_log(0,0,"<PosCom--%s> tcp_accept_client() failed: %d!",
                        pSlot->caFoldName,(errno));
      continue;
    }
	
    if(1) //for debug
    {
    

      in.s_addr = iCliAddr;
      strPeerIp = inet_ntoa(in);
      dcs_log(0,0,"<PosCom--%s> accept a conn from %s:%d",
                        pSlot->caFoldName,strPeerIp,iCliPort);
    }

    //the connection established successfully
    pSlot->iStatus     = DCS_STAT_CONNECTED;
    strcpy(pSlot->sRemoteIp  ,strPeerIp);
    pSlot->iRemotePort = iCliPort;

    //read message from network and forward it to ISE,then
    //wait the response from ISE and transfer it to net
    
    pfnRW(iSeqNo,iConnSock);

    if(0) //for debug
    {
    
      in.s_addr = iCliAddr;
      strPeerIp = inet_ntoa(in);
      dcs_log(0,0,"<PosCom--%s>  conn from %s:%d disconnected",
                        pSlot->caFoldName,strPeerIp,iCliPort);
    }

    //close the connection and waiting connection request again
    tcp_close_socket(iConnSock);
    pSlot->sRemoteIp[0]   = 0;
    pSlot->iRemotePort = 0;
  }//for(;;)

}


int DoLoop()
{
    char    caMsg[1024];
    int     nRead, iOrgFid, pidChld, iExitCode;


    fold_set_attr(g_iTrmFoldId, FOLD_FLAG_NEEDSIG);

    for(;;)
    {
        //read request from my folder in non-blocking mode
        memset(caMsg,0,sizeof(caMsg));
        nRead = fold_read(g_iTrmFoldId, &iOrgFid, caMsg,sizeof(caMsg),FALSE);
        if(nRead < 0)
        {
            if(errno != ENOMSG && errno != EINTR)
            {
                //read error
                dcs_log(0,0,"<PosCom> fold_read() failed!");
                break;
            }

            if(fold_sys_active() < 0)
                break;

            //wait a child to die
            pidChld = wait_child_or_signal(&iExitCode);

            if(pidChld > 0)
            {
                //a child dies,try to reload it
                sleep(1);
                RestartChild(pidChld);
                continue;
            }

            //read request from my folder in blocking mode
            nRead = fold_read(g_iTrmFoldId, &iOrgFid, caMsg,sizeof(caMsg),TRUE);
            if(nRead < 0)
            {
	            if(errno == EINTR)
	                continue;
	            else
	                break;
            }
        }

        //a request from someone else arrived, do with it
        AnswerRequest(caMsg);

  }//for(;;)

}

int AnswerRequest(char *pstrMsg)
{
    struct  trmrequest *pQry;
    int     i, iMsgType;

    pQry = (struct  trmrequest *)pstrMsg;

    //convert message type from string to int
    for(iMsgType=0,i=0; i<4; i++)
      iMsgType = iMsgType * 10 + (pQry->caMsgType[i] - '0');

    dcs_log(0,0,"<PosCom> recv a message with type = %.4d",iMsgType);

    switch(iMsgType)
    {
        case DCS_COMMAND_LINE:  //request from command line interface
        {
            struct trmcmd *pCmd;
            pCmd = (struct trmcmd *)pstrMsg;

            if(pCmd->iCommand == DCS_CMD_EXIT)
                raise(SIGTERM);
        }
        break;

        default:
           break;
    }//switch

    return 0;
}

int RestartChild(int pidChld)
{
    int     i;

//    dcs_debug(0,0,"trm cnt=%d",g_pTrmCom->iTrmCnt);
//    for(i=0; i < g_pTrmCom->iTrmCnt; i++)
    for(i=0; i < g_pLinkInfo->minterm; i++)
    {
//        dcs_debug(0,0,"g_pTrmArray[%d].pidComm=[%d]",i,g_pTrmArray[i].pidComm);
        if(g_pTrmArray[i].pidComm != pidChld)
            continue;

        dcs_log(0,0, "<PosCom> %s died, try restart it...",  g_pTrmArray[i].caFoldName);

        g_pTrmArray[i].pidComm = -1;
        MakeChild(g_pLinkInfo->iRole,i);
        return 0;
    }

    dcs_log(0,0, "<PosCom> a child with pid=%d died,"
                      "but cannot locate its link info.", pidChld);
    return -1;
}



int dReadFromNet(int iSeqNo, int iConnSock)
{
  int    iMaxTry = 10000,  iTmout , len,n;
  struct trmgrpinfo   *pSlot;
  int    i, iFid, iOrgFid, iDestFid;
  int    nRead, nWritten;
  char   caBuffer[2048],caBuf[20];
  struct IBDCSPacket *pktptr;

  pSlot = &g_pTrmArray[iSeqNo];
  iTmout = 15*1000;
  sg_index = iSeqNo; 
  iFid   = pSlot->iFid;

	pSlot->iStatus = DCS_STAT_READNET;
	memset(caBuffer,0,sizeof(caBuffer));
	dcs_debug(0,0,"sock id =%d",iConnSock);
	if ( g_pLinkInfo ->iHeadType == DCS_HEAD_TRM || g_pLinkInfo ->iHeadType == DCS_HEAD_TRM1 )
				nRead = read_msg_from_NAC(iConnSock,caBuffer+4,sizeof(caBuffer),iTmout);
  else  if ( g_pLinkInfo ->iHeadType == DCS_HEAD_BANK )
				nRead = read_msg_from_net(iConnSock,caBuffer+4,sizeof(caBuffer),iTmout);
	else  if ( g_pLinkInfo ->iHeadType == DCS_HEAD_AMP )
				nRead = read_msg_from_AMP(iConnSock,caBuffer+4,sizeof(caBuffer),iTmout);		
	if(nRead <= 0)
	{
	//read error
		dcs_log(0,0,"<PosCom--%s> read_msg_from_net() failed:%d!",
	                    pSlot->caFoldName,(errno));
	        close(iConnSock);
		return 0;
	}
	dcs_debug(caBuffer,nRead+4,"<PosCom-->%s read_msg_from_net len=%d ",pSlot->caFoldName,nRead);
	if ( g_pLinkInfo ->iHeadType == DCS_HEAD_TRM )
	{
		 sprintf(caBuf,"%02x%02x",g_pLinkInfo->iGid,iSeqNo);
		 memcpy(caBuffer+4+1,caBuf,4);
	}
	fold_purge_msg(iFid);
	pSlot->iStatus = DCS_STAT_WRITEFOLD;
  	memcpy( caBuffer,g_pLinkInfo->sMsgType,4);

	for(i=0; i<iMaxTry; i++)
	{      
		if ( memcmp(caBuffer,"ZZZZ",4)==0)
		{
			nWritten = fold_write(g_iCaFoldId,iFid,caBuffer+4,nRead);
			nWritten = nWritten + 4;
		}
		else
			nWritten = fold_write(g_iCaFoldId,iFid,caBuffer,nRead+4);
    
		if(nWritten < nRead+4)
		{
			if(errno == FOLD_EMSGOVFLW)
			{
			  usleep(100000); //sleep 200ms
			  continue;
			}
			else
			{
			  dcs_log(0,0,"<PosCom--%s> fold_write() failed,foldsvr id=%d:%d",
			                   pSlot->caFoldName,g_iCaFoldId,(errno));
			  goto lblReturn;
			}
		}
		else  //write ok
			break;
	}//try write to APPL
	if(i == iMaxTry)
	{
		dcs_log(caBuffer+4,nRead,"<PosCom--%s> discard a message from net",
		                pSlot->caFoldName);
		close(iConnSock);
		return 0;
	}
	
	//wait for the response from APPL;current implemetation has a defect
	//because if the APPL dies we will be hung forever
	pSlot->iStatus = DCS_STAT_READFOLD;
	memset(caBuffer,0,sizeof(caBuffer));
	nRead = 0;
	signal(SIGALRM,AlarmMsg);
	alarm(60);
	sg_iSock=iConnSock;

	nRead=fold_read(iFid,&iOrgFid,caBuffer,sizeof(caBuffer),TRUE);
	if(nRead < 0)
	{
		//read error
		dcs_log(0,0,"<PosCom--%s> fold_read() failed",
		                pSlot->caFoldName);
		close(iConnSock);
		alarm(0);
		return 0;
	}

	alarm(0);
	if ( gTime)
	{
		  if ( IsTimeOut(caBuffer,caBuffer+2) )
      {
      	 dcs_log(caBuffer,nRead,"数据在本系统里处理超时，不作转发");
      	 close(iConnSock);
      	 return 0;
      }
      for ( n=0; n<nRead-8; n++)
      {
      	 caBuffer[n]=caBuffer[n+8];
      }
      nRead=nRead-8;
  }
	dcs_debug(caBuffer, nRead,"from folder read data  is %d bytes-->",nRead);

	if(memcmp(caBuffer,"NETNOREAD",9)==0)
	{
		close(iConnSock);
		dcs_log(0,0,"<PosCom--%s> net no ready,"
		           "disconnect with ECR\n", pSlot->caFoldName);
		return 0; 
	}
	if(memcmp(caBuffer,"TIMEOUT",7)==0)
	{
		close(iConnSock);
		dcs_log(0,0,"<PosCom--%s> Get a time out msg,"
		           "disconnect with ECR\n", pSlot->caFoldName);
		return 0; 
	}
	if(memcmp(caBuffer,"ERROR",5)==0)
	{
		close(iConnSock);
		dcs_log(0,0,"<PosCom--%s> error msg,"
		           "disconnect with ECR\n", pSlot->caFoldName);
		return 0; 
	}
	
//	pktptr = (struct IBDCSPacket *)caBuffer;
	pSlot->iStatus = DCS_STAT_WRITENET;
	nWritten =0;
	if ( g_pLinkInfo ->iHeadType == DCS_HEAD_TRM || g_pLinkInfo ->iHeadType == DCS_HEAD_TRM1 )
				nWritten = write_msg_to_NAC(iConnSock,caBuffer,nRead,1000);
	else if ( g_pLinkInfo ->iHeadType == DCS_HEAD_BANK )
				nWritten = write_msg_to_net(iConnSock,caBuffer,nRead,1000);
	else if ( g_pLinkInfo ->iHeadType == DCS_HEAD_AMP )
				nWritten = write_msg_to_AMP(iConnSock,caBuffer,nRead,1000);
	if(nWritten < nRead)
		dcs_log(caBuffer,nRead,"<PosCom--%s> write_msg_from_net()  sock id = %d,ret=%d,failed:%d ",
		                    pSlot->caFoldName, iConnSock,nWritten,(errno));
	close(iConnSock);

lblReturn:
 return 0; 
}

int dReadFromFolder(int iSeqNo, int iConnSock)
{
  int    iMaxTry = 10000,  iTmout , len;
  struct trmgrpinfo   *pSlot;
  int    i, iFid, iOrgFid, iDestFid;
  int    nRead, nWritten;
  char   caBuffer[2048] ,*strIp;
  struct IBDCSPacket *pktptr;
 struct in_addr in;
  pSlot = &g_pTrmArray[iSeqNo];
  iTmout = 59*1000;
  
  iFid   = pSlot->iFid;

	
	for ( ; ; )
	{

					pSlot->iStatus = DCS_STAT_READFOLD;
					memset(caBuffer,0,sizeof(caBuffer));
					dcs_debug(0,0,"begin recvi msg from folder");
					nRead=-1;
					nRead=fold_read(iFid,&iOrgFid,caBuffer,sizeof(caBuffer),TRUE);
					if(nRead < 0)
					{
						//read error
						dcs_log(0,0,"<PosCom--%s> fold_read() failed",pSlot->caFoldName);
						break;
					}
					dcs_debug(caBuffer,nRead,"recv data from folder len=%d",nRead);
					if ( gTime)
						  if ( IsTimeOut(caBuffer,caBuffer+2) )
			        {
			        	 dcs_log(caBuffer,nRead,"数据在本系统里处理超时，不作转发");
			        	 continue;
			        }
					dcs_debug(0,0,"send socket ip=[%s],port=%d",pSlot->sRemoteIp,pSlot->iRemotePort);
					pSlot->iStatus = DCS_STAT_CALLING;
					g_iSock =  tcp_connet_server( pSlot->sRemoteIp, pSlot->iRemotePort,0);
					if ( g_iSock <0 )
					{
						close(g_iSock);
						dcs_log(0,0,"Connect to Ip = %s Port = %d fail:%s",pSlot->sRemoteIp ,pSlot->iRemotePort,strerror(errno));
				    continue;
					}
					pSlot->iStatus = DCS_STAT_WRITENET;
					if ( !gTime)
					{
							if ( g_pLinkInfo ->iHeadType == DCS_HEAD_TRM || g_pLinkInfo ->iHeadType == DCS_HEAD_TRM1 )
										nWritten = write_msg_to_NAC(g_iSock,caBuffer,nRead,1000);
							else if ( g_pLinkInfo ->iHeadType == DCS_HEAD_BANK )
										nWritten = write_msg_to_net(g_iSock,caBuffer,nRead,1000);
							else if ( g_pLinkInfo ->iHeadType == DCS_HEAD_AMP )
										nWritten = write_msg_to_AMP(g_iSock,caBuffer,nRead,1000);
							else
								dcs_log(0,0,"不知名的通信方式");
							dcs_debug(caBuffer,nRead,"<PosCom> write_msg_from_net  len=%d .",nRead);
					}
					else
					{
							nRead=nRead-8;
							if ( g_pLinkInfo ->iHeadType == DCS_HEAD_TRM || g_pLinkInfo ->iHeadType == DCS_HEAD_TRM1 )
									 nWritten = write_msg_to_NAC(g_iSock,caBuffer+8,nRead,1000);
							else if ( g_pLinkInfo ->iHeadType == DCS_HEAD_BANK )
										nWritten = write_msg_to_net(g_iSock,caBuffer+8,nRead,1000);
							else if ( g_pLinkInfo ->iHeadType == DCS_HEAD_AMP )
										nWritten = write_msg_to_AMP(g_iSock,caBuffer+8,nRead,1000);
							else
								 dcs_log(0,0,"不知名的通信方式");
							dcs_debug(caBuffer+8,nRead,"<PosCom> write_msg_from_net yy len=%d . type=%d",nRead,g_pLinkInfo ->iHeadType);
					}
					if(nWritten < nRead)
					{
							dcs_log(caBuffer,nRead,"<PosCom--%s> write_msg_from_net() failed:%d!",
						                    pSlot->caFoldName,(errno));
						  close(g_iSock);
						  continue;
					}

					pSlot->iStatus = DCS_STAT_READNET;
					memset(caBuffer,0,sizeof(caBuffer));
					memcpy(caBuffer,g_pLinkInfo->sMsgType,4);
					if ( g_pLinkInfo ->iHeadType == DCS_HEAD_TRM || g_pLinkInfo ->iHeadType == DCS_HEAD_TRM1 )
								nRead = read_msg_from_NAC(g_iSock,caBuffer+4,sizeof(caBuffer),iTmout);
					else if ( g_pLinkInfo ->iHeadType == DCS_HEAD_BANK )
								nRead = read_msg_from_net(g_iSock,caBuffer+4,sizeof(caBuffer),iTmout);
					else if ( g_pLinkInfo ->iHeadType == DCS_HEAD_AMP )
								nRead = read_msg_from_AMP(g_iSock,caBuffer+4,sizeof(caBuffer),iTmout);
								
					if(nRead <= 0)
					{
					//read error
							dcs_log(0,0,"<PosCom--%s> read_msg_from_net() failed:%d!", pSlot->caFoldName,(errno));
						  close(g_iSock);
							continue;
					}
					close(g_iSock);
					dcs_debug(caBuffer+4,nRead,"<PosCom-->%s read_msg_from_net len=%d ",pSlot->caFoldName,nRead);
					pSlot->iStatus = DCS_STAT_WRITEFOLD;
				
					for(i=0; i<iMaxTry; i++)
					{
						nWritten = fold_write(g_iCaFoldId,iFid,caBuffer,nRead+4);
						if(nWritten < nRead+4)
						{
							if(errno == FOLD_EMSGOVFLW)
							{
							  usleep(200000); //sleep 200ms
							  continue;
							}
							else
							{
							 dcs_log(0,0,"<PosCom--%s> fold_write() failed,foldsvr id=%d:%d",
			                   pSlot->caFoldName,g_iCaFoldId,(errno));
							  break;
							}
						}
						else  //write ok
							break;
					}//try write to APPL
					if(i == iMaxTry)
					{
						dcs_log(0,0,"<PosCom--%s> discard a message from net",
						                pSlot->caFoldName);
						close(g_iSock);
						break;
					}

	}
lblReturn:
 return 0; 
}

int CalcTrmCnt()
{
	 int   iFd=-1;
  char cfgfile[256];
  
  int i,s,iGrpCnt,iMaxTerm;
  
  
  
  
  if(u_fabricatefile("config/PosCom.conf",cfgfile,sizeof(cfgfile)) < 0)
  {
    dcs_log(0,0,"cannot get full path name of 'PosCom.conf'\n");
    return -1;
  }

  
  if(0 > (iFd = conf_open(cfgfile)) )
  {
    dcs_log(0,0,"open file '%s' failed.\n",cfgfile);
    return -1;
  }
  //load item 'comm.direct.maxterm'
	
  if(0>conf_get_first_number(iFd,"comm.maxgrp",&iGrpCnt)|| iGrpCnt < 0 )
  {
    dcs_log(0,0,"<PosCom> load config item 'comm.maxgrp' failed!");
                      
    conf_close(iFd);
    return -1;
  }
  s =0;
//  conf_get_next_number(iFd,"comm.maxterm",&iMaxTerm );
//  s = iMaxTerm;
  for ( i =0; i<iGrpCnt; i++)
  {
  		if ( conf_get_next_number(iFd,"comm.maxterm",&iMaxTerm )<0 || iMaxTerm <0)
    	{
	    	  dcs_log(0,0,"<PosCom> load config item 'comm.maxterm' failed!");
	                      
	    		conf_close(iFd);
	    		return -1;
			}
//			dcs_debug(0,0,"iMaxTerm =%d",iMaxTerm);
		  s =s + iMaxTerm;
		
	}
	conf_close(iFd);
	return s;
}

char * ConnectPosvrShm()
{
	 char *ptr = NULL;

    ptr = shm_connect(DCS_POSCOM_NAME,NULL);
    if(ptr == NULL)
    {
        dcs_log(0,0,"cannot connect SHM '%s':%d!\n",DCS_POSCOM_NAME,
                                 (errno));
    }

	return ptr;
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
