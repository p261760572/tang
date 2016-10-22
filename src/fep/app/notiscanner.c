/*账单通知表定时扫描函数
如果有超时交易，发送报文头为CMDI，消息类型为0002的信息给接收文件夹
*/
#include <stdio.h>
#include <errno.h>
#include "dbstruct.h"
#include <signal.h>
#include <string.h>
#include <stdlib.h>

static int OpenLog(char *IDENT);
static int CreateMyFolder();
static void NotiScanExit(int signo);
static int DoLoop();

int g_iNotiScan;
int g_iSafInterval=60;
int main(int argc, char *argv[])
{

    if ( argc >= 2 )
    {
        if ( atoi(argv[1]) > 0 && atoi(argv[1]) <1000)
            g_iSafInterval = atoi(argv[1]);
    }

    catch_all_signals(NotiScanExit);
    u_daemonize(NULL);
    /*打开日志文件*/
    if ( 0 > OpenLog(argv[0]) )
        exit(1);

    dcs_log(0,0, "NotiScan Servers is starting up...\n");
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",strerror(errno),__FILE__,__LINE__);
        exit(1);
    }
    /*连接数据库*/
    if ( DasConectDB()!=0 )
    {
        dcs_log(0,0,"Can not open oracle DB !");
        NotiScanExit(0);
    }
    /*创建文件夹*/
    if ( CreateMyFolder() < 0 )
    {
        dcs_log(0,0,"Can not CreateMyFolder !");
        NotiScanExit(0);
    }
    dcs_log(0,0,"*************************************************\n"
            "!!        NotiScanner Servers startup completed.        !!\n"
            "*************************************************\n");
    DoLoop( );

    NotiScanExit(0);

    return 0;
}

static void NotiScanExit(int signo)
{
    if ( signo > 0 )
        dcs_log(0,0,"catch a signal %d\n",signo);
    if ( signo !=0 && signo != SIGTERM && signo != SIGSEGV )
        return;

    dcs_log(0,0,"Noti Scanner terminated.\n");
    DasEnd( COMMIT );
    exit(signo);
}


static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/appsvr1.log"
    if ( u_fabricatefile("log/notiscanner.log",logfile,sizeof(logfile)) < 0 )
        return -1;

    return dcs_log_open(logfile, IDENT);
}

static int CreateMyFolder()
{
    if ( fold_initsys() < 0 )
    {
        dcs_log(0,0, "cannot attach to folder system!");
        return -1;
    }

    g_iNotiScan = fold_create_folder("NOTI");
    if ( g_iNotiScan < 0 )
        g_iNotiScan = fold_locate_folder("NOTI");
    if ( g_iNotiScan < 0 )
    {
        dcs_log(0,0,"cannot create folder '%s':%s\n", "NOTI", ise_strerror(errno) );
        return -1;
    }

    dcs_log(0,0, "folder g_iNotiScan=%d\n", g_iNotiScan);

    if ( fold_get_maxmsg(g_iNotiScan) <2 )
        fold_set_maxmsg(g_iNotiScan, 20) ;

    return 0;
}

/*每隔0.1秒钟读取数据库中超时记录*/
static int DoLoop()
{
    struct TimeOut_REC st_rec;
    long now;
    int iRc;
    for ( ;; )
    {

        while ( 1 )
        {
            time(&now);
            iRc = ReadDelNotiOutTbl(&st_rec);
            if ( -3114 == iRc || -3135 == iRc )
            {
                    dcs_log(0,0,"<FILE:%s,LINE:%d>数据库连接失败!需重连\n",__FILE__,__LINE__);

                    if ( DasConectDB()==0 )
                    {
			continue;
                    } else
                        break;
            }
	    if ( iRc == DAS_NORMAL )
            {
                DoTimeOutRecode(&st_rec);
                usleep(10000);
            } else 
            {
//                dcs_log(0,0, "read the next time out err!");
                break;
            }
        }

        sleep(g_iSafInterval);
    }
}

int DoTimeOutRecode(struct TimeOut_REC *p_rec)
{
    char caSendBuf[256];
    int iRc,iDestFolderId;

    memset(caSendBuf,0,sizeof(caSendBuf));
    //if ( p_rec->iFlags==0 )  /*账单通知记录*/
    {
        /*发送账单通知信息*/        
        memcpy(caSendBuf,"CMDI0002",8);   /*内部命令，账单通知*/
        memcpy(caSendBuf+8,p_rec,sizeof(struct TimeOut_REC));
        iDestFolderId = p_rec->iRcvFolderId;

        iRc = fold_write(iDestFolderId,g_iNotiScan,caSendBuf, sizeof(struct TimeOut_REC)+8);
        if ( iRc < 0 )
        {
            dcs_log(0,0, "<FILE:%s,LINE:%d><DoTimeOutRecode>Failure to write msg to folder[%d]!",__FILE__,__LINE__,iDestFolderId);
            return -1;
        }
        dcs_debug(caSendBuf,sizeof(struct TimeOut_REC)+8, "<FILE:%s,LINE:%d><DoTimeOutRecode>success to write msg to folder[%d]!",__FILE__,__LINE__,iDestFolderId);

    } //else if ( p_rec->iFlags==1 )  /*定时器*/
    //{
    //}
    return 0;
}
