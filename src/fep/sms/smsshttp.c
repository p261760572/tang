#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "ibdcs.h"
#include "bc.h"
#include "shttpd.h"


extern int gs_appSvrFid;

static void appSrvExit(int signo)
{
    if ( signo > 0 )
        dcs_log(0,0,"<FILE:%s,LINE:%d>catch a signal %d\n",__FILE__,__LINE__,signo);
    if ( signo !=0 && signo != SIGTERM && signo != SIGSEGV )
        return;

    dcs_log(0,0,"<FILE:%s,LINE:%d>SMSSHTTP Server terminated.\n",__FILE__,__LINE__);
    DasEnd( COMMIT );
    shm_detach((char *)g_pcBcdaShmPtr);
    exit(signo);
}

static int OpenLog(char *IDENT)
{
    char logfile[256];

    if ( u_fabricatefile("log/smsapp.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

static void show_msg(struct shttpd_arg *arg)
{
	const char	*host, *request_method, *query_string, *request_uri;
	char readBuff[2048];
	int ret;
	char *p;
	request_method = shttpd_get_env(arg, "REQUEST_METHOD");
	request_uri = shttpd_get_env(arg, "REQUEST_URI");
	query_string = shttpd_get_env(arg, "QUERY_STRING");
	if (!strcmp(request_method, "POST")) 
	{
		if (arg->flags & SHTTPD_MORE_POST_DATA)
			return;
		dcs_log(0, 0, "request_method=%s\ninBuf[%d]=%s\n", request_method, arg->in.len, arg->in.buf);
//		(void) shttpd_get_var("data", arg->in.buf, arg->in.len, readBuff, sizeof(readBuff));
		memset(readBuff, 0, sizeof(readBuff));
		memcpy(readBuff, arg->in.buf, MIN(arg->in.len, sizeof(readBuff) - 1));
		ret = xmlRead(readBuff);
	}
	else
	{
		dcs_log(0, 0, "MSG:request_uri=[%s],query_string=[%s]\n", request_method, query_string);
		ret = 0xff;
	}
	shttpd_printf(arg, "%s", "HTTP/1.1 200 OK\r\n");
	switch(ret)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			shttpd_printf(arg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n<SDKContractRoot><SDKSvcCont><ResultCode>00000000</ResultCode><ResultMessage>OK</ResultMessage></SDKSvcCont></SDKContractRoot>");
			break;
		case -1:
			shttpd_printf(arg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n<SDKContractRoot><SDKSvcCont><ResultCode>20000002</ResultCode><ResultMessage>Message Error</ResultMessage></SDKSvcCont></SDKContractRoot>");
			break;
		case -2:
			shttpd_printf(arg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n<SDKContractRoot><SDKSvcCont><ResultCode>20000006</ResultCode><ResultMessage>UserID Error</ResultMessage></SDKSvcCont></SDKContractRoot>");
			break;
		case -4:
			shttpd_printf(arg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n<SDKContractRoot><SDKSvcCont><ResultCode>10000001</ResultCode><ResultMessage>User Password Error</ResultMessage></SDKSvcCont></SDKContractRoot>");
			break;
		case -5:
			shttpd_printf(arg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n<SDKContractRoot><SDKSvcCont><ResultCode>10000002</ResultCode><ResultMessage>Business Error</ResultMessage></SDKSvcCont></SDKContractRoot>");
			break;
		case 0xff:
			shttpd_printf(arg, "Content-Type: text/html\r\n\r\n<html><body><h1>China In Pay</h1></body></html>");
			break;
		default:
			shttpd_printf(arg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n<SDKContractRoot><SDKSvcCont><ResultCode>90000999</ResultCode><ResultMessage>Other Error</ResultMessage></SDKSvcCont></SDKContractRoot>");
			break;	
	}
	arg->flags |= SHTTPD_END_OF_OUTPUT;
	
}

/*
 * This callback function is used to show how to handle 404 error
 */
static void
show_404(struct shttpd_arg *arg)
{
	shttpd_printf(arg, "%s", "HTTP/1.1 200 OK\r\n");
	shttpd_printf(arg, "%s", "Content-Type: text/plain\r\n\r\n");
	shttpd_printf(arg, "%s", "Oops. File not found! ");
	shttpd_printf(arg, "%s", "This is a custom error handler.");
	arg->flags |= SHTTPD_END_OF_OUTPUT;
}


int main()
{
	struct shttpd_ctx       *ctx;
    pthread_t sthread;
    int argc;
    char *argv[4] = {"smsshttp", "-ports", "10522", NULL};
    argc = 3;
    catch_all_signals(appSrvExit);
    if ( 0 > OpenLog("smsapp"))
    {
        BC_SendExeception("04","<FILE:%s,LINE:%d>打开日志失败，SMSSHTTP进程启动失败，退出");
        appSrvExit(0);
    }
    dcs_log(0,0, "<FILE:%s,LINE:%d>SMSSHTTP Servers is starting up...\n",__FILE__,__LINE__);
    
    u_daemonize(NULL);
    
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",__FILE__,__LINE__ ,strerror(errno));
        appSrvExit(0);
    }
    
    g_pcBcdaShmPtr = (char *)shm_connect("BCDA",NULL);
    if ( g_pcBcdaShmPtr == NULL )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><appSrv> cannot connect shared memory 'BCDA'!",__FILE__,__LINE__);
        appSrvExit(0);
    }
    
    if ( DasConectDB()!=0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>Can not open oracle DB !",__FILE__,__LINE__);
        BC_SendExeception("04","连接oracle DB失败，appSrv进程启动失败，退出");
        appSrvExit(0);
    }
    
    if ( fold_initsys() < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d>cannot attach to folder system!", __FILE__, __LINE__);
        return -1;
    }

    gs_appSvrFid = fold_locate_folder("SMSS");
    if ( gs_appSvrFid < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>cannot create folder '%s':%d\n",__FILE__,__LINE__, "SMSS", (errno) );
        appSrvExit(0);
    }
    
	if ( 0 > LoadConfig())
	{
        dcs_log(0,0,"<FILE:%s,LINE:%d> LoadConfig() failed\n",__FILE__,__LINE__);
        appSrvExit(0);
	}
   

    //do my main logic in a infinite loop    

    dcs_log(0,0,"*************************************************\n"
            "!!      SMSSHTTP Application Servers startup completed. !!\n"
            "*************************************************\n");

	ctx = shttpd_init(argc, argv);
	if(ctx == NULL)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Can not shttpd_init!",__FILE__, __LINE__);
		return -1;
	}
	shttpd_register_uri(ctx, "/msg", &show_msg, NULL);
	shttpd_handle_error(ctx, 404, show_404, NULL);
	
	/* Serve connections infinitely until someone kills us */
	for (;;)
	{
		shttpd_poll(ctx, 1000);
		if(0 > fold_locate_folder("SMSS"))break;
	}
	/* Probably unreached, because we will be killed by a signal */
	shttpd_fini(ctx);
    appSrvExit(0);		
}



