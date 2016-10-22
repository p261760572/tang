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
#include "gateway.h"
#include "iso8583.h"
extern gs_run_mode;


struct FD_REC gl_fd[FD_SETSIZE];
int gl_sock[100],role=0,stop=0,crypt_flag=0,gs_xft=0,gs_tpdu=0;
static char cfgfile[256]="iso8583.conf",logfile[256];
struct ISO_8583 iso8583_conf[128];
struct PARA g_para[100];
int _head_len=0,_msg_id_flg=0,_msg_fld_len_flg=0,_bitmap_flg=0,iso_mode=0;
void do_loop();
void do_loop1();
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
    	if( errno !=EINPROGRESS) goto lblError;
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
//	memset(posp_ip0,0,sizeof(posp_ip0));
//	memset(posp_ip1,0,sizeof(posp_ip1));
	memset(&g_para,0,sizeof(struct PARA) *100);
	sprintf(logfile,"%s.log",argv[0]);
	dcs_log_open(logfile,NULL);
	catch_all_signals(_srv_exit);
	catch_signal(40,_srv_exit);
	catch_signal(13,SIG_IGN);
	memset(&gl_fd,0,sizeof sizeof(struct FD_REC)*FD_SETSIZE);

		for ( i=1; i<argc ;i++)
		{
			 if(memcmp("svr_ip",argv[i],6)==0 )
			 {
			 	 if ( strlen( argv[i] ) >6 && atoi(argv[i]+6)>=0)
			 	 {
			 	     sscanf(argv[i],"%*[^=]=%s",g_para[atoi(argv[i]+6)].svr_ip);
			 	     dcs_debug(0,0,"svr_ip%d=%s !\n",atoi(argv[i]+6),g_para[atoi(argv[i]+6)].svr_ip);
			   }
			   else 
			   {
			   	  dcs_log(0,0,"error entry para[%d] =%s",atoi(argv[i]+6),argv[i]);
			   	  return;
			   }
			 }
			 else if(memcmp("svr_port",argv[i],8)==0 )
			 {
			 	  if ( strlen( argv[i] ) >8 && atoi(argv[i]+8)>=0)
			 	  {
			 	  		sscanf(argv[i],"%*[^=]=%d",&g_para[atoi(argv[i]+8)].svr_port);
			 	  		dcs_debug(0,0,"svr_port%d=%d !\n",atoi(argv[i]+8),g_para[atoi(argv[i]+8)].svr_port);
			 	  }
			 	  else 
			   {
			   	  dcs_log(0,0,"error entry para[%d] =%s",atoi(argv[i]+8),argv[i]);
			   	  return;
			   }
			 }
			 else if(memcmp("loc_port",argv[i],8)==0 )
			 {
			 	  if ( strlen( argv[i] ) >8 && atoi(argv[i]+8)>=0)
			 	  {
				 	  sscanf(argv[i],"%*[^=]=%d",&g_para[atoi(argv[i]+8)].loc_port);
				 	  dcs_debug(0,0,"loc_port%d=%d !\n",atoi(argv[i]+8),g_para[atoi(argv[i]+8)].loc_port);
				 	  g_para[atoi(argv[i]+8)].used=0x31;
			 	  }
			 	  else 
			   {
			   	  dcs_log(0,0,"error entry para[%d] =%s",atoi(argv[i]+8),argv[i]);
			   	  return;
			   }
			 }
//			 else if(memcmp("head_len",argv[i],8)==0 )
//			 {
//			 	  
//				 	  sscanf(argv[i],"%*[^=]=%d",&_head_len);
//				 	  dcs_log(0,0,"head_len=%d !\n",_head_len);
//			 }
//			 else if(memcmp("msg_id_flg",argv[i],10)==0 )
//			 {
//			 	  
//				 	  sscanf(argv[i],"%*[^=]=%d",&_msg_id_flg);
//				 	  dcs_log(0,0,"msg_id_flg=%d !\n",_msg_id_flg);
//			 }
//			 else if(memcmp("msg_fld_len_flg",argv[i],15)==0 )
//			 {
//			 	  
//				 	  sscanf(argv[i],"%*[^=]=%d",&_msg_fld_len_flg);
//				 	  dcs_log(0,0,"msg_fld_len_flg=%d !\n",_msg_fld_len_flg);
//			 }
			 else if(memcmp("role",argv[i],4)==0 )
			 {
			 	  sscanf(argv[i],"%*[^=]=%d",&role);
			 	  dcs_debug(0,0,"role=%d !\n",role);
			 }
			 else if(memcmp("crypt_flag",argv[i],10)==0 )
			 {
			 	  sscanf(argv[i],"%*[^=]=%d",&crypt_flag);
			 	  dcs_debug(0,0,"crypt_flag=%d !\n",crypt_flag);
			 }
			 else if(memcmp("type",argv[i],4)==0 )
			 {
			 	  if ( strlen( argv[i] ) >4 && atoi(argv[i]+4)>=0)
			 	  {
			 	    sscanf(argv[i],"%*[^=]=%d",&g_para[atoi(argv[i]+4)].type);
			 	    dcs_debug(0,0,"type%d=%d !\n",atoi(argv[i]+4),g_para[atoi(argv[i]+4)].type);
			 	 }
			 	  else 
			   {
			   	  dcs_log(0,0,"error entry para=%s",argv[i]);
			   	  return;
			   }
			 }
			 else if(memcmp("cfgfile",argv[i],7)==0 )
			 {
			 	  
				 	  sscanf(argv[i],"%*[^=]=%s",cfgfile);
				 	  if ( strlen( argv[i] ) >7 && atoi(argv[i]+7)>=0)
			 	    {
			 	       sscanf(argv[i],"%*[^=]=%s",g_para[atoi(argv[i]+6)].cfgfile);
			 	       dcs_debug(0,0,"cfgfile%d=%s !\n",atoi(argv[i]+6),g_para[atoi(argv[i]+6)].cfgfile);
			 	       iso_mode=1;
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
			 else if(memcmp("xft",argv[i],3)==0 )
			 {
			 	  
				 	  sscanf(argv[i],"%*[^=]=%d",&gs_xft);
				 	  dcs_debug(0,0,"xft=%d !\n",gs_xft);
				 	  
			 }
			 else if(memcmp("-p",argv[i],2)==0 )
			 {
			 	  
				 	  fprintf(stderr,"\n version 2.0-%s %s release \n",__DATE__,__TIME__);
				 	  return;
			 }
			 else
			 {
			 	    dcs_log(0,0,"error entry para=%s",argv[i]);
			   	  return;
			}
			 
		}
		dcs_log_open(logfile,NULL);
		
	if ( role == 0)
	{
		if( iso_mode)
		{
			memset(&iso8583_conf[0],0,sizeof(struct ISO_8583)*128);
		  if ( IsoLoad8583config(&iso8583_conf[0],cfgfile) < 0 )
		  {
		
		        dcs_log(0,0,"<FILE:%s,LINE:%d>Load interface iso8583config() failed:%s\n",__FILE__,__LINE__,strerror(errno));
		        return 0;
		  }
	  }
		do_loop();
	}
	else if ( role == 1)
		do_loop1();
  else if ( role == 2)
  	do_loop2();
  else if ( role == 3)
  	do_loop2_1();
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
int get_sockfd(char *buffer ,int len,int max,int sockfd)
{
char	seq_no[7],shop_no[16],pos_id[9],key[40];
ISO_data iso;	
int i;	
	memset(&iso,0,sizeof(iso));
	memset(seq_no,0,sizeof(seq_no));
	memset(shop_no,0,sizeof(shop_no));
	memset(pos_id,0,sizeof(pos_id));
	iso.iso8583=&iso8583_conf[0];    
 if(_head_len >0 && _head_len <50 )iso.headlen=_head_len;     
 else iso.headlen=0;
 iso.head_flag=1;
 if (_msg_id_flg  >0 ) SetIsoHeardFlag(&iso,1);
 else SetIsoHeardFlag(&iso,0);
 if( _msg_fld_len_flg  >0 ) SetIsoFieldLengthFlag(&iso,1);
 else SetIsoFieldLengthFlag(&iso,0);
 if( _bitmap_flg ) SetIsoBitmapFlag(&iso,1);
 else SetIsoBitmapFlag(&iso,0);
 if( strtoiso(buffer,&iso,len ) <0 ) return 0;
 
	getbit(&iso,11,seq_no);
	getbit(&iso,41,pos_id);
	getbit(&iso,42,shop_no);
	memcpy(key,seq_no,6);
	memcpy(key+6,shop_no,15);
	memcpy(key+21,pos_id,8);
	dcs_log(0,0,"key=%s,",key);
	for( i =0 ; i <=max;i++)
	{
		 if(gl_fd[i].used)
		 if( memcmp(key,gl_fd[i].remark+1,29)==0 )
		 {
		 		memset(gl_fd[i].remark+1,0,30);
		 		dcs_log(0,0,"found sock [%d]",i);
		 		return gl_fd[i].sockfd;
		 }
	}
	return -1;
}
int set_sockfd(char *buffer,int len,int sockfd)
{
char	seq_no[7],shop_no[16],pos_id[9],key[40];
ISO_data iso;	

	memset(&iso,0,sizeof(iso));
	memset(seq_no,0,sizeof(seq_no));
	memset(shop_no,0,sizeof(shop_no));
	memset(pos_id,0,sizeof(pos_id));
	if ( sockfd <0 || sockfd >=FD_SETSIZE ) return 0;
	iso.iso8583=&iso8583_conf[0];
	if(_head_len >0 && _head_len <50 ) iso.headlen=_head_len;     
  else iso.headlen=0;
  iso.head_flag=1;
  if (_msg_id_flg  >0 ) SetIsoHeardFlag(&iso,1);
  else SetIsoHeardFlag(&iso,0);
  if( _msg_fld_len_flg  >0 ) SetIsoFieldLengthFlag(&iso,1);
  else SetIsoFieldLengthFlag(&iso,0);
  if( _bitmap_flg ) SetIsoBitmapFlag(&iso,1);
  else SetIsoBitmapFlag(&iso,0);
 	if( strtoiso(buffer,&iso,len ) <0 ) return 0;
  
	getbit(&iso,11,seq_no);
	getbit(&iso,41,pos_id);
	getbit(&iso,42,shop_no);
	memcpy(key,seq_no,6);
	memcpy(key+6,shop_no,15);
	memcpy(key+21,pos_id,8);
	memcpy(gl_fd[sockfd].remark+1,key,29);
	return 1;
}
