#include "ibdcs.h"
#include "tmcibtms.h"

extern char *g_pcBcdaShmPtr;
// comunication cortrol table(BCDA)

static void appSrvExit(int signo)
{
    if ( signo > 0 )
        dcs_log(0,0,"<FILE:%s,LINE:%d>catch a signal %d\n",__FILE__,__LINE__,signo);
    if ( signo !=0 && signo != SIGTERM && signo != SIGSEGV )
        return;

    dcs_log(0,0,"<FILE:%s,LINE:%d>GDYL_CMLNK_TEST terminated.\n",__FILE__,__LINE__);
    shm_detach((char *)g_pcBcdaShmPtr);
    exit(signo);
}

int main(int argc, char *argv[])
{
    catch_all_signals(appSrvExit);
    if(0 > OpenLog(argv[0])) return -1;
    dcs_log(0,0, "GDYL_CMLNK_TEST is starting up...\n");
    //attach to SHM of IBDCS
    u_daemonize(NULL);
    if(dcs_connect_shm() < 0)
    {
        dcs_log(0,0,"dcs_connect_shm() failed:%s\n",strerror(errno));
        printf("dcs_connect_shm() failed:%s\n",strerror(errno));
        exit(1);
    }
    if(fold_initsys() < 0)
    {
        dcs_log(0,0, "cannot attach to folder system!");
        return -1;
    }
	g_pcBcdaShmPtr = (char *)shm_connect("BCDA",NULL);
    if ( g_pcBcdaShmPtr == NULL )
    {
		dcs_log(0,0,"cannot connect shared memory 'BCDA'!");
		return -1;
    }
    DoLoop();
  	return -1;
}

int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/timersvr.log"
    if(u_fabricatefile("log/GDYL_CMLNK_TEST.log",logfile,sizeof(logfile)) < 0)
        return -1;

    return dcs_log_open(logfile, IDENT);
}

int DoLoop()
{
    int foldid;
    char buff[20];
    char *g_pcBcdaShmPtr;
    int i;
	strcpy(buff, "\x60\x00\x00\x00\x00\x60\x21\x00\x00\x00\x00");    
   	foldid = fold_locate_folder("GDYL");
    for(;;)
    {
    	for(i=0; i<300; i++)
        	usleep(1000000);
//		if(GDYLReady()<0)continue;
		if(IsReady("GDYL")<0)continue;
    	if(0>fold_write( foldid , -1, buff, 5 ))
	    {
	        dcs_log(0,0, "fold_write() failed: %s\n", strerror(errno) );
	        return -1;    
	    }
    }//for
}

/*int GDYLReady()
{
    int iBnkIdx;
    struct BCDAst *pBCDAst;
    pBCDAst = (struct BCDAst *)g_pcBcdaShmPtr;
    if ( !pBCDAst ) return -1;
    iBnkIdx = BC_GetFolderIdByName("GDYL");
    if ( iBnkIdx < 0 )  return -1;
    if ( pBCDAst->stBcda[iBnkIdx].cNetFlag <= 0  ) return -1;
    return 1;
/*    int iBnkIdx;
    struct BCDAst *pBCDAst;
    pBCDAst = (struct BCDAst *)g_pcBcdaShmPtr;
    if ( !pBCDAst )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>通信机构共享内存区尚未连接就绪!",__FILE__,__LINE__);
        return -1;
    }
    iBnkIdx = BC_GetFolderIdByName("GDYL");
    if ( iBnkIdx < 0 )  //invalid bank code
    {
        dcs_debug(0,0,"<FILE:%s,LINE:%d>LocateBankByName invalid pcaName =%s",__FILE__,__LINE__,"GDYL");
        return -1;
    }
    if ( pBCDAst->stBcda[iBnkIdx].cNetFlag <= 0 )
    {
        //无网络连接或没有发开始请求报文
        dcs_debug(0,0,"<FILE:%s,LINE:%d>cNetFlag is 0 net not connect [%s]",__FILE__,__LINE__,"GDYL");
        return -1;
    }
    return 1;
}*/

