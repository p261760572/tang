#include "ibdcs.h"

//globals
static int g_myfid    = -1;

static void dcmMoniExit(int signo);
static int g_childmonitor = -1;

static int Monitor();
static int StartAllProcesses();
static int CreateMyFolder();
static int OpenLog(char *IDENT);
static struct CFGTBL   g_proctab[MAX_CHILDREN_NUM];
char * DispSubsystemMsg( char *pcCopName );
//forward declaration
void dcmMoniExit(int signo);
void Dummy(int iSigno)
{
	
        dcs_debug( 0,0, "Receive singal SIGUSR1!!" );
	alarm(0);
	return ;
}

int main(int argc, char *argv[])
{
    IBDCS_CFG IbdcsCfg;

    //prepare the logging stuff
    if(0 > OpenLog(argv[0]))
        exit(1);
        
    dcs_log(0,0, "Monitor is starting up...\n");        
    
   
    catch_all_signals(dcmMoniExit);
    
    //try to load configuration from file
    if(dcs_load_config(&IbdcsCfg) < 0)
        exit(1);

    //try to create SHM
    if(0 > dcs_create_shm(IbdcsCfg.iTbTRows))
        exit(1);

    //copy config from temp buffer to SHM
    memcpy(g_pIbdcsCfg,&IbdcsCfg,sizeof(IBDCS_CFG));

    //attach to folder system and create folder of myself
   
    if ( CreateBcda()<=0)
         dcmMoniExit(0);
    //start all process in IDBCS
   

    if( 0 > StartAllProcesses() )
        dcmMoniExit(0);

    //monitor the whole IBCDS
/*    g_childmonitor = fork();
    if( g_childmonitor < 0 )
    {
        //dcs_log(0, 0, "cannot fork child: %d\n", (errno));    
        dcs_log(0, 0, "cannot fork child:%d \n",errno));    
        dcmMoniExit(0);
    }
 */
  if( CreateMyFolder() < 0 )
        dcmMoniExit(0);
    
//    if( g_childmonitor == 0 ) //child
     u_daemonize(NULL);
    g_pIbdcsShm->is_MoniPid = getpid();
    {
        dcs_log(0,0,"*************************************************\n"
                    "!!        Monitor startup completed.           !!\n"
                    "*************************************************\n");
        Monitor();
    }   
/* 
    else  //parent
    {
        dcs_log(0,0,"children wather is ready.");                
//        ChildrenWather();
    }
 */       
    dcmMoniExit(0);
}

static void dcmMoniExit(int signo)
{
    if(signo > 0)
        dcs_debug(0,0,"catch a signal %d\n",signo);
    
//    if(signo !=0 && signo != SIGTERM && signo != SIGSEGV)
//        return;
    
    if(g_pIbdcsShm && g_pIbdcsShm->is_MoniPid != getpid())
    {
        kill( getppid(), SIGTERM );
        return;
    }    
        
    //we telled to exit,release all resource before death
    if(g_pIbdcsShm)
    {
        //bring down all children
        int i;
        for(i = g_pIbdcsShm->is_nchld-1; i>=0; i--)
        {
            if( g_pIbdcsShm->is_children[i] > 0)
            { 
            	kill(g_pIbdcsShm->is_children[i], SIGTERM);
          	}
        }
        
        if( g_childmonitor > 0)
            kill(g_childmonitor, SIGTERM);
           
        g_childmonitor = -1;
        fold_delete_folder( g_myfid );
        dcs_delete_shm();
    }
    RemoveBcda();
    dcs_log(0,0,"monitor terminated.\n");
    exit(signo);
}

static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/ibdcs.log"
    if(u_fabricatefile("log/monitor.log",logfile,sizeof(logfile)) < 0)
        return -1;

    return dcs_log_open(logfile, IDENT);
}

static int CreateMyFolder()
{
    if(fold_initsys() < 0)
    {
        dcs_log(0,0, "cannot attach to folder system!");
        return -1;
    }

    g_myfid = fold_create_folder(MONITOR_FOLD_NAME);
    if(g_myfid < 0)
        g_myfid = fold_locate_folder(MONITOR_FOLD_NAME);
    if(g_myfid < 0)
    {
        dcs_log(0,0, "cannot create folder '%s':%d!",
                            MONITOR_FOLD_NAME,(errno));
        return -1;
    }
    
    fold_purge_msg( g_myfid );
    return 0;
}

static int StartProcess(struct CFGTBL *strExe)
{
    int pidChld = -1, ret,i;
    char path[200];
    pidChld = vfork();
    if(pidChld < 0)
    {
        dcs_log(0,0,"cannot fork child '%s'!\n",strExe->caPrgName);
        return -1;
    }

    if(pidChld > 0)
    {
        dcs_log(0,0, "going to load process '%s'...\n", strExe->caPrgName);
        catch_signal( SIGUSR1, Dummy );
				catch_signal( SIGCLD, SIG_IGN );
				catch_signal( SIGALRM, dcmMoniExit );
//				alarm( 1 );
//				pause();
        return pidChld;
    }    

    //child here
    dcs_debug(0,0,"exec procName=['%s'],para=%s",strExe->caPrgName,strExe->caPara);
    ret = execlp (strExe->caPrgName, strExe->caPrgName,(char *)strExe->caPara,(char *)0);
    if(ret < 0)
    {
      dcs_log(0,0,"cannot exec executable '%s':%d!",strExe->caPrgName,(errno));
      exit(0);
    }
    exit( 0 );
}

static int StartAllProcesses()
{
    int i,s,k;

    
    g_pIbdcsShm->is_nchld = 0;

    if ( GetSvrCfg() <0)
         return -1;
    fprintf(stdout,"\n%s",DispSubsystemMsg(NULL));
    catch_signal( SIGCLD, SIG_IGN );
    catch_signal( SIGALRM, dcmMoniExit );
    for(i=0,s=0; i<MAX_CHILDREN_NUM; i++)
    {
        if( strlen(g_proctab[i].caSubSystem)<=0 )
            break ;
       
//        fprintf(stdout,"提示: 正在启动 %s ...\n",DispSubsystemMsg(g_proctab[i].caSubSystem));
				fprintf(stdout,"提示: 正在启动 %s ...\n",g_proctab[i].caRemark);
        for (k=0; k < g_proctab[i].iUnForkNo ; k++,s++)
        {
        	  if ( s >= MAX_CHILDREN_NUM )
        	  {
        	  	     dcs_log(0,0,"fork process unit overflow!");
        	  	     fprintf(stdout,"fork process unit overflow!\n");
        	  	      return -1;
        	  }
 		        g_pIbdcsShm->is_children[s] = StartProcess( &g_proctab[i] );
		        if(g_pIbdcsShm->is_children[s] < 0)
		        {
		        	   dcs_log(0,0," Start Process Fail , Name =%s",g_proctab[i].caPrgName);
		        	   fprintf(stdout," Start Process Fail , Name =%s \n",g_proctab[i].caPrgName);
		        	    return -1;
		        }
		        g_pIbdcsShm->is_nchld ++;
		        usleep(1000);

      	}
        alarm( 1 );
				pause();
//      	if ( memcmp( g_proctab[i].caSubSystem,"FLD",3)==0)
//      			usleep(1000);
//      	else
//      			sleep(1);
//              usleep(500);
    }//for
    fprintf(stdout,"\n系统启动完毕!!!\n\n");
    return 0;
}

static int Monitor()
{
    char   caBuffer[MAX_IBDCS_MSG_SIZE];
    int    nread, fromfid, n;
    struct IBDCSPacket *pktptr;
    
    //locate the folder id of AppServer2
/*    for(n=0 ;; n++ )
    {
        gs_app2fid = fold_locate_folder( APPSRV_FOLD_NAME );
        if(gs_app2fid >= 0)
            break;

			  sleep(1);
        if ( n >30) break;
    }
 */   
    for(;;)
    {
        //read from my folder in blocking mode
        memset(caBuffer, 0, sizeof(caBuffer));
        nread = fold_read( g_myfid, &fromfid, caBuffer, sizeof(caBuffer), 1);
        if(nread < 0)
        {
            dcs_log(0,0,"fold_read() failed:%d\n",(errno));
            break;
        }
        
        if ( nread ==0)
        {
        	 dcs_log(0,0,"recv msg len=0 ");
        }
        else
        {
        		dcs_log(caBuffer,nread,"recv msg len=%d",nread);
        }
/*
        pktptr = (struct IBDCSPacket *)caBuffer+4;
        switch(pktptr->pkt_cmd)
        {
            case PKT_CMD_TCPCONN:
//                HandleTcpConn( pktptr->pkt_val );
                break;
            
            case PKT_CMD_TIMER:
                dcs_log(0,0, "go a timer notification, timerid = %d\n", 
                                       pktptr->pkt_val);
                break;
                
            default:
                break;
        }//switch */
    }//for

    return 0;
}
/*
int HandleTcpConn(int fConnected)
{
    int val;

    if(fConnected == 0) //断链,重启通信进程
    {
        //update system status
        dcs_get_sysstat(TCP_CONNECTIONS,&val);
        val --;
        if(val < 0) val =0;
        dcs_set_sysstat(TCP_CONNECTIONS,val);
        dcs_set_sysstat(APPL_READY,0);
        dcs_set_sysstat(COMPROC_STATUS,0);
        dcs_set_sysstat(CYH_B_REQ,0);

        dcs_debug(0,0,"notified with TCP conn down.\n");
        return 0;
    }

    else //链路已经建立,通知 AppServer2 发开始请求报文
    {
        struct IBDCSPacket packet;

        dcs_get_sysstat(TCP_CONNECTIONS,&val);
        val ++;
        dcs_set_sysstat(TCP_CONNECTIONS,val);
        dcs_set_sysstat(COMPROC_STATUS,2);

        dcs_debug(0,0,"notified with TCP connection up.\n");

        packet.pkt_cmd = PKT_CMD_CYHREQ;
        packet.pkt_val = 0;
        packet.pkt_bytes = 0;

        if(0 > fold_write( gs_app2fid, g_myfid, &packet, sizeof(packet) ) )
        {
            dcs_log(0,0,"fold_write() failed:%d!\n",(errno));
            return -1;
        }

        return 0;
    }
}
*/
int static ChildrenWather()
{
    int pid;
    
    for( ;; )
    {
        pid = wait( NULL );
        
        if( kill(g_childmonitor, 0) < 0)
            break;
        if(pid < 0 && errno == ECHILD )
            break;
        if( pid < 0)
            continue;
        
//        RestarChildByPid( pid );        
    }//for    
}

static int RestarChildByPid( pid )
{
    int i;
    
    for(i=0; i< g_pIbdcsShm->is_nchld; i++ )
    {
        if( g_pIbdcsShm->is_children[i] != pid )
            continue;
        else
            break;
    }
    
    if( i >= g_pIbdcsShm->is_nchld )            
    {
        dcs_log(0,0,
         "WARN: a child with pid=%d died, but cannot locate its entry.\n", pid);
        return -1;
    }
    
    dcs_log(0,0,"process %s died,try to reload it...\n", g_proctab[i]);
    g_pIbdcsShm->is_children[i] = StartProcess( &g_proctab[i] );
    if(g_pIbdcsShm->is_children[i] < 0)
        return -1;
    return 0;    
}

int GetSvrCfg( )
{
	 int    iRc, iCnt, iArgIdx;


  FILE   *pfServFile;
  char   caFileName[300], caCodefBuf[500];
  char   *pcToken;
  int    iForkIdx;

  
  sprintf( caFileName, "%s/config/%s%s",(char *)getenv("ICS_HOME"),"isdsrver.conf", "\0" );    

  if ( (pfServFile=fopen(caFileName, "r"))==NULL )
  {
    dcs_log(0,0, "<ComponentCfg>Failure to open file [%s]\n", caFileName);
    return -1;
  
  }
  
  iCnt=0;
  iForkIdx = 0;

//  fgets( caCodefBuf, sizeof(caCodefBuf), pfServFile );

  while ( !feof(pfServFile) )
  {
  	memset( caCodefBuf,0,sizeof(caCodefBuf));
  	fgets( caCodefBuf, sizeof(caCodefBuf), pfServFile );
  	if ( strlen(caCodefBuf) < 3)
  			continue;
    pcToken = (char *)strtok(caCodefBuf, DELIMITER);
    if ( pcToken[0]=='#' || pcToken==NULL )
    {
     
      continue;
    }
    if ( iCnt > MAX_CHILDREN_NUM )
    {
    	 dcs_log(0,0," application svr unit overflow !");
  	}
    strcpy( g_proctab[iCnt].caSubSystem, pcToken );
    pcToken = (char *)strtok( NULL, DELIMITER );
    if ( pcToken == NULL )
    {
    		dcs_log(0,0,"Get Para PrgName error ! subsys=[%s],prgname=[%s]",g_proctab[iCnt].caSubSystem,pcToken);
    		return -1;
    }
    strcpy( g_proctab[iCnt].caPrgName, pcToken );
    dcs_debug(0,0,"Subsystem =[%s],svr name =[%s]",g_proctab[iCnt].caSubSystem,pcToken);
    g_proctab[iCnt].iMaxForkNo = atoi( (char *)strtok( NULL, DELIMITER ));
    g_proctab[iCnt].iMinForkNo = atoi( (char *)strtok( NULL, DELIMITER ));
    g_proctab[iCnt].iUnForkNo = g_proctab[iCnt].iMaxForkNo;
    g_proctab[iCnt].iUnFkIdx = iForkIdx;
    
    memset( g_proctab[iCnt].caPara, '\0', sizeof(g_proctab[iCnt].caPara) );
    pcToken = (char *)strtok( NULL, "*\n");
    if ( pcToken != NULL ) {

      if ( strlen(pcToken) > 79 ) {

        dcs_log( 0,0, "<ComponentCfg> Length of paraments[ %d] in file [ %s ]  is overflow ");
        fclose(pfServFile);

      }
      strcpy( g_proctab[iCnt].caPara, pcToken);
    }
    pcToken = (char *)strtok( NULL, "\n");
    if ( pcToken != NULL ) {

      if ( strlen(pcToken) > 99 ) {

        dcs_log( 0,0, "<ComponentCfg> Length of paraments[ %d] in file [ %s ]  is overflow ");
        fclose(pfServFile);

      }
      strcpy( g_proctab[iCnt].caRemark, pcToken);
    }

//    dcs_debug( 0,0, "<Debug>PARA is :%s",g_proctab[iCnt].caPara);
    iCnt++;
//    fgets( caCodefBuf, sizeof(caCodefBuf), pfServFile );
  }

  fclose(pfServFile);

  dcs_debug( 0,0,  "<ComponentCfg> Load CFG in SWA ok!!");
  return 0;
}

char * DispSubsystemMsg( char *pcCopName )
{
	if ( pcCopName == NULL )
	{
		return ( "ICS 系统组件开始启动 ... \n" );
	}
	else if ( memcmp( pcCopName, "FLD", 3 ) == 0 )
	{
		return "Folder server";
	}
	else if ( memcmp( pcCopName, "HST", 3 ) == 0 )
	{
		return ( "Organization communication server" );
	}
	else if ( memcmp( pcCopName, "TIM", 3 ) == 0 )
	{
		return ( "Timer Server " );
	}
	else if ( memcmp( pcCopName, "SHT", 3 ) == 0 )
	{
		return ( "Pos communication server " );
	}
	else if ( memcmp( pcCopName, "ISA", 3 ) == 0 )
	{
		return ( "Applcation pool" );
	}
	else if ( memcmp( pcCopName, "MON", 3 ) == 0 )
	{
		return( "Monitor server" );
	}
	
	fflush( stdout );
	return( 0 );
}
