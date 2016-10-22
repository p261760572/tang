/*��ʱ��ʱɨ�躯��
����г�ʱ���ף����ͱ���ͷΪCMDI����Ϣ����Ϊ0001����Ϣ�������ļ���
*/
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include "dbstruct.h"
#include <stdlib.h>

static void TimeScanExit(int signo);
static int CreateMyFolder();
static int DoLoop();
static int OpenLog(char *IDENT);

int g_iTimeScan;
int main(int argc, char *argv[])
{
	
		catch_all_signals(TimeScanExit);
		u_daemonize(NULL);
		/*����־�ļ�*/
		if (0 > OpenLog(argv[0]))
			exit(1);

    dcs_log(0,0, "TimeScan Servers is starting up...\n");
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        exit(1);
    }    
    /*�������ݿ�*/
    if (DasConectDB()!=0)
    {
        dcs_log(0,0,"Can not open oracle DB !");
        TimeScanExit(0);
    }
		/*�����ļ���*/
    if ( CreateMyFolder() < 0 )
    {
        dcs_log(0,0,"Can not CreateMyFolder !");
        TimeScanExit(0);
    }
    dcs_log(0,0,"*************************************************\n"
            "!!        TimeScanner Servers startup completed.        !!\n"
            "*************************************************\n");
		DoLoop( );

		TimeScanExit(0);
   return 0; 
	
}
static void TimeScanExit(int signo)
{
    if (signo > 0)
        dcs_log(0,0,"catch a signal %d\n",signo);
    if (signo !=0 && signo != SIGTERM && signo != SIGSEGV)
        return;

    dcs_log(0,0,"Time Scanner terminated.\n");
    DasEnd( COMMIT );
    exit(signo);
}


static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/appsvr1.log"
    if (u_fabricatefile("log/timescanner.log",logfile,sizeof(logfile)) < 0)
        return -1;

    return dcs_log_open(logfile, IDENT);
}

static int CreateMyFolder()
{
    if (fold_initsys() < 0)
    {
        dcs_log(0,0, "cannot attach to folder system!");
        return -1;
    }

    g_iTimeScan = fold_create_folder("TIMC");
    if (g_iTimeScan < 0)
        g_iTimeScan = fold_locate_folder("TIMC");
    if (g_iTimeScan < 0)
    {
        dcs_log(0,0,"cannot create folder '%s':%s\n", "TIMC", ise_strerror(errno) );
        return -1;
    }

    dcs_log(0,0, "folder g_iTimeScan=%d\n", g_iTimeScan);

    if (fold_get_maxmsg(g_iTimeScan) <2)
        fold_set_maxmsg(g_iTimeScan, 20) ;

    return 0;
}

/*ÿ��1���Ӷ�ȡ���ݿ��г�ʱ��¼*/
static int DoLoop()
{
    struct TimeOut_REC st_rec;
    int iRc;
    for (;;)
    {
		
	while( 1 )
	{
            iRc = ReadDelTimeOutTbl(&st_rec);
            if(iRc == DAS_NORMAL)
            {
		DoTimeOutRecode(&st_rec);
                usleep(10000);
            }
	    else if ( -3114 == iRc || -3135 == iRc)
            {
                    dcs_log(0,0,"<FILE:%s,LINE:%d>���ݿ�����ʧ��!������\n",__FILE__,__LINE__);

                    if ( DasConectDB()==0 )
                    {
                         continue ;
                    } else
                        break;
            }
	    else
	    {
//		        dcs_log(0,0, "read the next time out err!");
		        break;
            }
 	 }
  sleep(1);
  }
}

int DoTimeOutRecode(struct TimeOut_REC *p_rec)
{
	char caSendBuf[256];
	int iRc,iDestFolderId;
	
	memset(caSendBuf,0,sizeof(caSendBuf));
	//if(p_rec->iFlags==0)  /*��ʱ��¼*/
	{
		/*���ͳ�ʱ��Ϣ*/		
		memcpy(caSendBuf,"CMDI0001",8);   /*�ڲ������ʱ*/
		memcpy(caSendBuf+8,p_rec,sizeof(struct TimeOut_REC));
		iDestFolderId = p_rec->iRcvFolderId;
		
    iRc = fold_write(iDestFolderId,g_iTimeScan,caSendBuf, sizeof(struct TimeOut_REC)+8);
    if ( iRc < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><DoTimeOutRecode>Failure to write msg to folder[%d]!",__FILE__,__LINE__,iDestFolderId);
        return -1;
    }		
    dcs_debug(caSendBuf,sizeof(struct TimeOut_REC)+8, "<FILE:%s,LINE:%d><DoTimeOutRecode>success to write msg to folder[%d]!",__FILE__,__LINE__,iDestFolderId);
		
	}
	//else if(p_rec->iFlags==1)  /*��ʱ��*/
	//{
	//}
	return 0;
}
