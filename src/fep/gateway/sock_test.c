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
#include <unistd.h>

void do_loop1( int second);

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
    if(listen(sock,50) < 0)
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
//   flags=fcntl(sock,F_GETFL);
//   fcntl(sock,F_SETFL,flags|O_NDELAY);

    //try to connect to server
 //   if(connect(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0)
 //       goto lblError;
    connect(sock,(struct sockaddr *)&sin,sizeof(sin));
    return sock;

lblError:
    if(sock >= 0)
        close(sock);
    return -1;
}
void do_loop( int sock,int second);

int main (int argc, char *argv[] )
{
	int i,child;
	char tmp[32],tmp1[32];


  i=tcp_open_server(NULL,7006);
  
  if( i < 0)
  {
  	 fprintf(stderr,"tcp_open_server error![%s]\n",strerror(errno));
  	 return 1;
  }
  child = fork();
  
  if( child == 0 )
  {
  	 do_loop(i,3);
  }
  if( child <0 ) 
  {
  	 fprintf(stderr,"fork child  error!\n");
  	 return 1;
  }
	else do_loop1(4);

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
void do_loop( int sock,int second)
{
	
  fd_set vread_set,read_set;
  struct timeval tv;
  
  int len,ret,client_addr, client_port,loc_sock,flags;
  unsigned char buffer[8192],tmp[256];
  
  FD_ZERO(&read_set);
  FD_SET(sock,&read_set);
   flags=fcntl(sock,F_GETFL);
   fcntl(sock,F_SETFL,flags|O_NDELAY);
  fprintf(stderr,"<pid=%d> entery working !\n",getpid());
    while(1)
    { 
       

    	tv.tv_sec = 5;
      tv.tv_usec = 0;
      vread_set=read_set;
    
    	ret=select(sock + 1, &vread_set,NULL,NULL, &tv);
      if ( ret < 0) 
    	{
	    	 fprintf(stderr,"<pid=%d>select error![%s]\n",getpid(),strerror(errno));
	    
      }
      else if ( ret == 0)
      {
          
           
      } 
      else 
      {
         fprintf(stderr,"<pid=%d>select ret=%d sleep=%d \n",getpid(),ret,second);
//         sleep(second);
         loc_sock=tcp_accept_client(sock,&client_addr, &client_port );
         if( loc_sock <0 )
         {
         	  fprintf(stderr,"<pid=%d>accept sock fail![%d][%s]\n",getpid(),errno,strerror(errno));
//         	  break;
         }
         else
         	 fprintf(stderr,"<pid=%d> accept sock succ! new sock[%d]\n",getpid(),loc_sock);
      }
    }

 
}

void do_loop1( int second)
{
	
  fd_set vread_set,read_set;
  struct timeval tv;
  
  int len,ret,client_addr, client_port,loc_sock,flags;
  unsigned char buffer[8192],tmp[256];
  int sock;
  sock=tcp_connet_server("127.0.0.1", 7007,0);
  if (sock < 0) return;
  
  FD_ZERO(&read_set);
  FD_SET(sock,&read_set);

  fprintf(stderr,"<pid=%d> connect succ, entery working ! sock=%d",getpid(),sock);
    while(1)
    { 
       

    	tv.tv_sec = 5;
      tv.tv_usec = 0;
      vread_set=read_set;
    
    	ret=select(sock + 1, &vread_set,NULL,NULL, &tv);
      if ( ret < 0) 
    	{
	    	 fprintf(stderr,"<pid=%d>select error![%s]\n",getpid(),strerror(errno));
	    
      }
      else if ( ret == 0)
      {
          
           
      } 
      else 
      {
         fprintf(stderr,"<pid=%d>select ret=%d sleep=%d \n",getpid(),ret,second);
//         sleep(second);
         len=recv(sock,buffer, sizeof(buffer),0 );
         if (len <=0 ) break;
         fprintf(stderr,"<pid=%d> recv len=%d  \n",getpid(),len);
      }
    }

 
}