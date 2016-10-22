#include <stdlib.h>
#include <stdio.h>
#include    <sys/socket.h>  /* basic socket definitions */
#include    <sys/time.h>    /* timeval{} for select() */
#include    <time.h>        /* timespec{} for pselect() */
#include    <netinet/in.h>
#include    <arpa/inet.h>   /* inet(3) functions */
#include    <errno.h>
#include    <fcntl.h>       /* for nonblocking */
#include    <netdb.h>
#include    <signal.h>
#include    <stdarg.h>
#include   <string.h>
#include   <signal.h>
#include   <stddef.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <dlfcn.h>
#include "nac_sw.h"
extern gs_run_mode;


SOCK_REC gl_fd[4];
int stop=0;
static char logfile[256];
SYS_ARA g_para;
void do_loop();
int connect_sock(int i ,int *max_fd);
fd_set read_set,write_set,error_set;
void _srv_exit( int signo);
 void add_to_set(int fd, fd_set *set, int *max_fd) {
  FD_SET(fd, set);
  if (fd >  *max_fd) {
    *max_fd = (int) fd;
  }
}

typedef void sigfunc_t(int);
sigfunc_t * catch_signal(int sig_no, sigfunc_t *sig_catcher)
{
    //
    //catch a specified signal with the given handler
    //
    
    struct sigaction act, oact;
    
    act.sa_handler = sig_catcher;
    act.sa_flags   = 0;

    //block no signals when this signal is being handled.        
    sigemptyset(&act.sa_mask);

    if(sig_no == SIGALRM || sig_no == SIGUSR1 || sig_no == SIGUSR2)
    {
        #ifdef SA_INTERRUPT        
            act.sa_flags |=  SA_INTERRUPT;    
        #endif        
    }
    else
    {
        #ifdef SA_INTERRUPT        
            act.sa_flags |=  SA_INTERRUPT;    
        #endif        

        //#ifdef SA_RESTART
          // act.sa_flags |=  SA_RESTART;
        //#endif    
    }
    
    if(sigaction(sig_no, &act, &oact) < 0)
        return (sigfunc_t *)0;
    return oact.sa_handler;    
}

//-------------------------------------------------

int catch_all_signals(sigfunc_t *sig_catcher)
{
    int signo;
    
    for(signo=1; signo < NSIG; signo++)
    {
        switch(signo)
        {
            case SIGHUP:             
            case SIGCONT:             
                 catch_signal(signo, SIG_IGN);
                 break;

            case SIGCHLD:
            default:
                 catch_signal(signo,sig_catcher);
                 break;     
        }//switch         
    }//for
    
    return 0;    
}
int tcp_open_server(const char *listen_addr, int listen_port)
{
    //description:
    //this function is called by the server end.
    //before listening on socket waiting for requests
    //from clients, the server should create the socket,
    //then bind its port to it.All is done by this function

    //arguments:
    //listen_addr--IP address or host name of the server,maybe NULL
    //listen_port--port# on which the server listen on

    int   arg=1;
	int on;
    int   sock;
    struct sockaddr_in	sin;
	struct linger linger_str;

    //create the socket
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
	return -1;

    //get the IP address and port#
    memset(&sin,0,sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port   = htons(listen_port);

    if(listen_addr == NULL)
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
    else
    {
        int  addr;
				if ((addr=inet_addr(listen_addr)) != INADDR_NONE)
				    sin.sin_addr.s_addr = addr;
				else	//'listen_addr' may be the host name
				{
				    struct hostent *ph;
			
				    ph=gethostbyname(listen_addr);
				    if(!ph)
					goto lblError;
				    sin.sin_addr.s_addr = ((struct in_addr *)ph->h_addr)->s_addr;
				}
    }

    //set option for socket and bind the (IP,port#) with it
    arg=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&arg,sizeof(arg));
    setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,(char *)&arg,sizeof(arg));
    

    if(bind(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0)
	     goto lblError;

    //put the socket into passive open status
    if(listen(sock,5) < 0)
	     goto lblError;
    linger_str.l_onoff = 0;
    linger_str.l_linger = 1;
    setsockopt(sock,SOL_SOCKET,SO_LINGER,&linger_str,sizeof(struct linger));
	  on = 1;


    return sock;

lblError:
    if(sock >= 0)
	close(sock);
    return -1;
}
int tcp_connet_server(char *server_addr, int server_port,int client_port)
{
    //this function is performed by the client end to
    //try to establish connection with server in blocking
    //mode

    int    arg, sock, addr,flags;
    struct sockaddr_in	sin;
    struct linger linger_str;

    //the address of server must be presented
    if(server_addr == NULL || server_port == 0)
	return -1;

    //create the socket
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
	return -1;

    //set option for socket
    arg=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&arg,sizeof(arg));
    setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,(char *)&arg,sizeof(arg));
    linger_str.l_onoff = 0;
    linger_str.l_linger = 1;
    setsockopt(sock,SOL_SOCKET,SO_LINGER,&linger_str,sizeof(struct linger));
//    struct timeval tv ;
//    tv.tv_sec = 2;
//    tv.tv_usec = 0;
    
//    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    //if 'client_port' presented,then do a binding
    if(client_port > 0)
    {
       memset(&sin,0,sizeof(struct sockaddr_in));
       sin.sin_family = AF_INET;
       sin.sin_port   = htons(client_port);
       sin.sin_addr.s_addr = htonl(INADDR_ANY);

       arg=1;
       setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&arg,sizeof(arg));
       if(0 > bind(sock,(struct sockaddr *)&sin,sizeof(sin)))
             goto lblError;
    }

    //prepare the address of server
    memset(&sin,0,sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port   = htons(server_port);

    if ((addr=inet_addr(server_addr)) != INADDR_NONE)
	sin.sin_addr.s_addr = addr;
    else  //'server_addr' may be the host name
    {
        struct hostent *ph;
        ph=gethostbyname(server_addr);
        if(!ph)
	    goto lblError;

	sin.sin_addr.s_addr = ((struct in_addr *)ph->h_addr)->s_addr;
    }
   flags=fcntl(sock,F_GETFL);
   fcntl(sock,F_SETFL,flags|O_NDELAY);

    //try to connect to server
    if(connect(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0)
    {
    	if( errno !=EINPROGRESS)   goto lblError;
    }
//    connect(sock,(struct sockaddr *)&sin,sizeof(sin));
    return sock;

lblError:
    if(sock >= 0)
        close(sock);
    return -1;
}


int main (int argc, char *argv[] )
{
	int i;
	char tmp[32],tmp1[32];

	memset(&g_para,0,sizeof(SYS_ARA));
	sprintf(logfile,"%s.log",argv[0]);
	dcs_log_open(logfile,NULL);
	catch_all_signals(_srv_exit);
	catch_signal(40,_srv_exit);
	catch_signal(13,SIG_IGN);
	memset(&gl_fd,0,sizeof sizeof(SOCK_REC)*4);
gs_run_mode=0;
		for ( i=1; i<argc ;i++)
		{
		
			 if(memcmp("old_ip",argv[i],6)==0 )
			 {
			 	 if ( strlen( argv[i] ) >6 )
			 	 {
			 	     sscanf(argv[i],"%*[^=]=%s",g_para.old_ip);
			 	     dcs_debug(0,0,"old_ip=%s !\n",g_para.old_ip);
			   }
			   else 
			   {
			   	  dcs_log(0,0,"error entry para =%s",argv[i]);
			   	  return;
			   }
			 }
			 else if(memcmp("new_ip",argv[i],6)==0 )
			 {
			 	 if ( strlen( argv[i] ) >7 )
			 	 {
			 	     sscanf(argv[i],"%*[^=]=%s",g_para.new_ip);
			 	     dcs_debug(0,0,"new_ip=%s !\n",g_para.new_ip);
			   }
			   else 
			   {
			   	  dcs_log(0,0,"error entry para=%s",argv[i]);
			   	  return;
			   }
			 }
			 else if(memcmp("old_port",argv[i],8)==0 )
			 {
			 	  if ( strlen( argv[i] ) >9 )
			 	  {
			 	  		sscanf(argv[i],"%*[^=]=%d",&g_para.old_port);
			 	  		dcs_debug(0,0,"old_port=%d !\n",g_para.old_port);
			 	  }
			 	  else 
			   {
			   	  dcs_log(0,0,"error entry para=[%s]",argv[i]);
			   	  return;
			   }
			 }
			 else if(memcmp("new_port",argv[i],8)==0 )
			 {
			 	  if ( strlen( argv[i] ) >9)
			 	  {
			 	  		sscanf(argv[i],"%*[^=]=%d",&g_para.new_port);
			 	  		dcs_debug(0,0,"new_port=%d !\n",g_para.new_port);
			 	  }
			 	  else 
			   {
			   	  dcs_log(0,0,"error entry para=%s",argv[i]);
			   	  return;
			   }
			 }
			 else if(memcmp("loc_port",argv[i],8)==0 )
			 {
			 	  if ( strlen( argv[i] ) >9 )
			 	  {
				 	  sscanf(argv[i],"%*[^=]=%d",&g_para.loc_port);
				 	  dcs_debug(0,0,"loc_port=%d !\n",g_para.loc_port);  
			 	  }
			 	  else 
			   {
			   	  dcs_log(0,0,"error entry para=%s",argv[i]);
			   	  return;
			   }
			 }
			 else if(memcmp("logfile",argv[i],7)==0 )
			 {
			 	  
				 	  sscanf(argv[i],"%*[^=]=%s",logfile);
				 	  dcs_debug(0,0,"logfile=%s !\n",logfile);
				 	  
			 }
			 else if(memcmp("-p",argv[i],2)==0 )
			 {
			 	  
				 	  fprintf(stderr,"\n version 1.0-%s %s release \n",__DATE__,__TIME__);
				 	  return;
			 }
			 else
			 {
			 	    dcs_log(0,0,"error entry para=%s",argv[i]);
			   	  return;
			}
			 
		}
		dcs_log_open(logfile,NULL);
		do_loop();
	dcs_log_close();
	return 1;
}
int tcp_accept_client(int listen_sock,int *client_addr, int *client_port)
{
    struct sockaddr_in cliaddr;
    socklen_t addr_len;
    int    conn_sock;

    for(;;)
    {
        //try accepting a connection request from clients
        addr_len  = sizeof(cliaddr);
        conn_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &addr_len);

        if(conn_sock >=0)
            break;
        if(conn_sock < 0 && errno == EINTR)
            continue;
        else
            return -1;
    }

    //bring the client info (IP_address, port#) back to caller
    if(client_addr)
        *client_addr = cliaddr.sin_addr.s_addr;
    if(client_port)
        *client_port = ntohs(cliaddr.sin_port);

    return conn_sock;
}



void _srv_exit( int signo)
{
	int i;
	
	if ( signo == 40 )
	{
		  
			if (gs_run_mode)
			{
				dcs_log(0,0,"open log debug mode ! signo=[%d]",signo);
				gs_run_mode=0;
			}
			else
			{
				dcs_log(0,0,"close log debug mode ! signo=[%d]",signo);
				gs_run_mode=1;
			}
			return;
	}
	dcs_log(0,0,"system  exit! signo=[%d]",signo);
	if( signo== 15 )
	{
		 stop=1;
		 return;
	}
	 for ( i=0; i < 4;i++)
   {
  	 if ( gl_fd[i].used )
  	 {
  	 	 close(gl_fd[i].sockfd);
  	 	 gl_fd[i].used=0;
  	 }
   }
	 exit(0);
}


