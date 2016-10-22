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
    connect(sock,(struct sockaddr *)&sin,sizeof(sin));
    return sock;

lblError:
    if(sock >= 0)
        close(sock);
    return -1;
}

static int gs_role = 0;
int main (int argc, char *argv[] )
{
	int i;
	char tmp[32],tmp1[32],o;

	
	memset(&gl_fd,0,sizeof sizeof(struct FD_REC)*FD_SETSIZE);

	while(-1 != (o = getopt(argc, argv, "l:"))) {
		switch(o) {
		
		case 'l':
			   if( optarg != NULL )
	    	{
	    		gs_role =1;
	    		strcpy(srv->cfg.log_file_name,optarg);
	    	}
			  else
			  {
			  	fprintf(stderr,"-l para error !");
			  	 return -1;
			  }
			  break;
		default:
			return -1;
		}
	}
		dcs_log_open(logfile,NULL);
		
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
	 for ( i=0; i < FD_SETSIZE;i++)
   {
  	 if ( gl_fd[i].used )
  	 {
  	 	 close(gl_fd[i].sockfd);
  	 	 gl_fd[i].used=0;
  	 }
   }
	 exit(0);
}

int connect_sock(int i, int *max_fd)
{
	 	int sock;
	 	sock=tcp_connet_server(gl_fd[i].ip, gl_fd[i].port,0);
	  if( sock >=0 )
	  {
	    add_to_set(sock, &write_set, max_fd);
	    add_to_set(sock, &error_set, max_fd);
	    gl_fd[sock].sockfd=sock;
	    strncpy(gl_fd[sock].ip,gl_fd[i].ip,sizeof(gl_fd[sock].ip)-1);
	    gl_fd[sock].port=gl_fd[i].port;
	    gl_fd[sock].type =gl_fd[i].type;
	    gl_fd[sock].len_len =gl_fd[i].len_len;
	    gl_fd[sock].data_len=0;
	    gl_fd[sock].msg_len=0;
	    gl_fd[sock].timeout =gl_fd[i].timeout;
	    gl_fd[sock].used=0x30;
	    gl_fd[sock].ndx=gl_fd[i].ndx;
	    gl_fd[sock].status=0;
	    gl_sock[gl_fd[i].ndx]= sock;
	    
	    dcs_debug(0,0,"connect svr_ip succ! [%s]:[%d][sock:%d]\n",gl_fd[i].ip,gl_fd[i].port,sock);
	  }
	  else
	     dcs_debug(0,0,"connect svr_ip fail! [%s]:[%d][sock:%d]\n",gl_fd[i].ip,gl_fd[i].port,sock);
	  return sock;
}

