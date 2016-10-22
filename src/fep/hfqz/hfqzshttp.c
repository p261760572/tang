/*
 * This file is an example of how to embed web-server functionality
 * into existing application.
 * Compilation line:
 * cc example.c shttpd.c -DEMBEDDED
 */

#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <pthread.h>

#define ALIAS_URI "/my_etc"
#define ALIAS_DIR "/etc/"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "ibdcs.h"
#include "bc.h"


void ExpendAppReply(char *retStr, int retSize, int flag, ISO_data *p_iso);

#include "shttpd.h"

int gs_appSvrFid = -1;

static void appSrvExit(int signo)
{
    if ( signo > 0 )
        dcs_log(0,0,"<FILE:%s,LINE:%d>catch a signal %d\n",__FILE__,__LINE__,signo);
    if ( signo !=0 && signo != SIGTERM && signo != SIGSEGV )
        return;

    dcs_log(0,0,"<FILE:%s,LINE:%d>HFQZSHTTP Server terminated.\n",__FILE__,__LINE__);
    DasEnd( COMMIT );
    shm_detach((char *)g_pcBcdaShmPtr);
    exit(signo);
}

static void show_result(struct shttpd_arg *arg)
{
	const char *request_method, *request_uri;
	char dst[2048];
	char ordId[16 + 1];
	int dst_len,iRet;
	request_method = shttpd_get_env(arg, "REQUEST_METHOD");
	request_uri = shttpd_get_env(arg, "REQUEST_URI");
	if(!strcmp(request_method, "POST"))
	{
		if(arg->flags & SHTTPD_MORE_POST_DATA)
		{
			return;
		}
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>request_method[%s]", __FILE__, __LINE__, request_method);
		memcpy(dst, "HTTP", 4);
		dst_len = _shttpd_url_decode(arg->in.buf, arg->in.len, dst+4, sizeof(dst)-4);
		memset(ordId, 0, sizeof(ordId));
		if(0 <= get_var("OrdId", dst+4, dst_len, ordId, sizeof(ordId)))
		{
		    iRet = fold_write(gs_appSvrFid, -1, dst, dst_len + 4);
		    if ( iRet < 0 )
		    {
		        dcs_log(0,0, "<FILE:%s,LINE:%d>Failure to write msg to folder[%d]!",__FILE__,__LINE__, gs_appSvrFid);
		    } 
		    else
		    {
		        dcs_debug(0,0, "<FILE:%s,LINE:%d>Success to write msg to folder[%d]",__FILE__,__LINE__, gs_appSvrFid);
		    }
		}
 	}
	else
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>request_method[%s]", __FILE__, __LINE__, request_method);
	}
	shttpd_printf(arg, "HTTP/1.1 200 OK\r\nRECV_ORD_ID_%s", ordId);
	dcs_debug(0, 0, "<FILE:%s,LINE:%d>HTTP/1.1 200 OK\r\nRECV_ORD_ID_%s", __FILE__, __LINE__, ordId);
	arg->flags |= SHTTPD_END_OF_OUTPUT;
	return;
}

/*
 * This function will be called on SSI directive <!--#if true -->, or
 * <!--#elif true -->, and 'returns' TRUE condition
 */
static void
ssi_test_true(struct shttpd_arg	*arg)
{
	arg->flags |= SHTTPD_SSI_EVAL_TRUE;
}

/*
 * This function will be called on SSI directive <!--#if false -->, or
 * <!--#elif false -->, and 'returns' FALSE condition
 */
static void
ssi_test_false(struct shttpd_arg *arg)
{
	/* Do not set SHTTPD_SSI_EVAL_TRUE flag, that means FALSE */
}

/*
 * This function will be called on SSI <!--#call print_stuff -->
 */
static void
ssi_print_stuff(struct shttpd_arg *arg)
{
	time_t	t = time(NULL);

	shttpd_printf(arg,
	    "SSI user callback output: Current time: %s", ctime(&t));
	if (arg->in.buf != NULL)
		shttpd_printf(arg, "SSI param passed: [%s]", arg->in.buf);
	arg->flags |= SHTTPD_END_OF_OUTPUT;
}

/*
 * Make sure we have ho zombies from CGIs
 */
static void
signal_handler(int sig_num)
{
	switch (sig_num) {
#ifndef _WIN32
	case SIGCHLD:
		while (waitpid(-1, &sig_num, WNOHANG) > 0) ;
		break;
#endif /* !_WIN32 */
	default:
		break;
	}
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

static int OpenLog(char *IDENT)
{
    char logfile[256];

    if ( u_fabricatefile("log/hfqzapp.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

int get_var(const char *var, const char *buf, int buf_len,	char *value, int value_len)
{
	const char	*p, *e, *s;
	size_t		var_len;
	var_len = strlen(var);
	e = buf + buf_len;		/* End of QUERY_STRING buffer	*/

	/* buf is "var1=val1&var2=val2...". Find variable first */
	for (p = buf; p + var_len < e; p++)
		if ((p == buf || p[-1] == '&') &&
		    p[var_len] == '=' &&
		    !_shttpd_strncasecmp(var, p, var_len)) {

			/* Point 'p' to var value, 's' to the end of value */
			p += var_len + 1;	
			if( *p == '{' )
			{
				if ((s = memchr(p, '}', e - p)) == NULL)
					s = e;
				else
					s++;
			}
			else
			{
				if ((s = memchr(p, '&', e - p)) == NULL)
					s = e;
			}
			/* URL-decode value. Return result length */
			return (_shttpd_url_decode(p, s - p, value, value_len));
		}
	return (-1);
}


int main(/*int argc, char *argv[]*/)
{
	struct shttpd_ctx  *ctx;
    char *argv[4] = {"hfqzshttp", "-ports", "6000", NULL};
    int argc = 3;
    catch_all_signals(appSrvExit);
    if ( 0 > OpenLog("hfqzapp"))
    {
        BC_SendExeception("04","<FILE:%s,LINE:%d>打开日志失败，HFQZSHTTP进程启动失败，退出");
        appSrvExit(0);
    }
    dcs_log(0,0, "<FILE:%s,LINE:%d>HFQZSHTTP Servers is starting up...\n",__FILE__,__LINE__);
    
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
    
    if ( fold_initsys() < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d>cannot attach to folder system!", __FILE__, __LINE__);
        return -1;
    }

    gs_appSvrFid = fold_locate_folder("HFQZ");
    if ( gs_appSvrFid < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>cannot create folder '%s':%d\n",__FILE__,__LINE__, "HFQZ", (errno) );
        appSrvExit(0);
    }
   

    //do my main logic in a infinite loop    

    dcs_log(0,0,"*************************************************\n"
            "!!      HFQZSHTTP Application Servers startup completed. !!\n"
            "*************************************************\n");

	ctx = shttpd_init(argc, argv);
	if(ctx == NULL)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Can not HFQZSHTTP shttpd_init!",__FILE__, __LINE__);
		appSrvExit(0);
	}
	shttpd_register_uri(ctx, "/result", &show_result, NULL);
	shttpd_register_ssi_func(ctx, "true", ssi_test_true, NULL);
	shttpd_register_ssi_func(ctx, "false", ssi_test_false, NULL);
	shttpd_register_ssi_func(ctx, "print_stuff", ssi_print_stuff, NULL);	
	shttpd_handle_error(ctx, 404, show_404, NULL);
	/* Serve connections infinitely until someone kills us */
	for (;;)
	{
		shttpd_poll(ctx, 1000);
		if(0 > fold_locate_folder("HFQZ"))break;
	}
	/* Probably unreached, because we will be killed by a signal */
	shttpd_fini(ctx);	
    appSrvExit(0);
}

