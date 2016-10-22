#include "dccsock.h"
//#include "errlog.h"
//#include <xti.h>

#define closesocket  close

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
	//if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on)))
//	goto lblError;

    return sock;

lblError:
    if(sock >= 0)
	closesocket(sock);
    return -1;
}

//---------------------------------------------------------------------------

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

//---------------------------------------------------------------------------

int tcp_connet_server(char *server_addr, int server_port,int client_port)
{
    //this function is performed by the client end to
    //try to establish connection with server in blocking
    //mode

    int    arg, sock, addr;
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

    //try to connect to server
    if(connect(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0)
        goto lblError;
    return sock;

lblError:
    if(sock >= 0)
        closesocket(sock);
    return -1;
}

//---------------------------------------------------------------------------

int tcp_close_socket(int sockfd)
{
    return closesocket(sockfd);
}

//---------------------------------------------------------------------------

int tcp_write_nbytes(int conn_sock, const void *buffer, int nbytes)
{
  //
  // write "nbytes" bytes to a connected socket
  //

  int  nleft,nwritten;
  char *ptr;

  for(ptr = (char *)buffer, nleft = nbytes; nleft > 0;)
  {
    if ((nwritten = write(conn_sock, ptr, nleft)) <= 0)
    {
	    if (errno == EINTR || errno == EAGAIN)
	        nwritten = 0; //and call write() again
	    else
	        return -1;  //error
    }

    nleft -= nwritten;
    ptr   += nwritten;
  }//for

  return  (nbytes - nleft);
}

//---------------------------------------------------------------------------

int tcp_read_nbytes(int conn_sock, void *buffer, int nbytes)
{
  //
  // read "nbytes" bytes from a connected socket descriptor
  //

  int    nleft,nread;
  char	*ptr;

  for(ptr=(char *)buffer,nleft=nbytes;nleft > 0;)
  {
    if ((nread = read(conn_sock, ptr, nleft)) < 0)
    {
	    if (errno == EINTR || errno == EAGAIN)
	    {
	        nread = 0; // and call read() again
	    }
	    else
	    {
	        return(-1);
	    }
    }
    else if (nread == 0)
    {
	    break;  //EOF
    }

    nleft -= nread;
    ptr   += nread;
  } //for

  return(nbytes - nleft);  // return >= 0
}

//---------------------------------------------------------------------------

#define  CHECK_READABLE   1
#define  CHECK_WRITABLE   2

static int tcp_check_rw_helper(int conn_sockfd,int ntimeout,int check_what);

int tcp_check_readable(int conn_sockfd,int ntimeout)
{
    //determine if the socket is readable within 'ntimeout' ms.
    //if readable, this function returns 0
    //if timeout,  returns -1 and errno == ETIMEDOUT
    //other error, returns -1 and error indicates the detailed info

    return tcp_check_rw_helper(conn_sockfd,ntimeout,CHECK_READABLE);
}

int tcp_check_writable(int conn_sockfd,int ntimeout)
{
    //determine if the socket is writable within 'ntimeout' ms.
    //if writable, this function returns 0
    //if timeout,  returns -1 and errno == ETIMEDOUT
    //other error, returns -1 and error indicates the detailed info

    return tcp_check_rw_helper(conn_sockfd,ntimeout,CHECK_WRITABLE);
}

//---------------------------------------------------------------------------


static int tcp_check_rw_helper(int conn_sockfd,int ntimeout,int check_what)
{
    int    nready;
    long n;
    fd_set sock_set, *prset, *pwset;
    struct timeval  tmval, *tmval_ptr;
/*
    //convert the time in ms to timeval struct
    if(ntimeout < 0) //waiting in blocked mode
        tmval_ptr = NULL;
    else
    {
        tmval_ptr = &tmval;
        tmval_ptr->tv_sec  = ntimeout / 1000;
        tmval_ptr->tv_usec = (ntimeout % 1000) * 1000;
    }
*/
    if ( ntimeout <=0 ) n=999999999;
    else n = ntimeout / 1000;
    for(;;)
    {
        //select the connected socket
        if ( n <=0 ) break;
        FD_ZERO(&sock_set);
        FD_SET(conn_sockfd, &sock_set);
        tmval_ptr = &tmval;
        tmval_ptr->tv_sec  = 1;
        tmval_ptr->tv_usec = 0;
        //wait for readability or writability
        switch(check_what)
        {
            case CHECK_READABLE:
                 prset = &sock_set;
                 pwset = NULL;
                 break;

            case CHECK_WRITABLE:
            default:
                 prset = NULL;
                 pwset = &sock_set;
                 break;
        }//switch

        nready = select(conn_sockfd+1,prset,pwset,NULL,tmval_ptr);
        if(nready < 0)
        {
            if(errno == EINTR)  //select again
                 continue;
//           dcs_debug(0,0,"select <0");
            return -1;
        }
        else  if(nready == 0)  //timeout
        {
            n--;
//            dcs_debug(0,0,"select ==0");
            continue;
//            errno = ETIMEDOUT;
//            return -1;
        }
        else  //ok
        {
//        	dcs_debug(0,0,"select >0");
        	 return 0;
        }
    }//for
    if( n==0 ) dcs_log(0,0,"socket time out !");
    return -1;
}

//--------------------------------------------------------------------------




int read_msg_from_jmj(int conn_sockfd, void *msg_buffer, int nbufsize,int ntimeout)
{
    //
    // read a message from net. the format of a message
    // is (length_indictor, message_content)
    //

    
    int   ret, bytes_to_read, msg_length,flags;

    //clear the error number
    errno = -1;

    //check for readability within 'ntimeout' ms
    if(tcp_check_readable(conn_sockfd,ntimeout) < 0)
    {
        //timeout or other error
        dcs_log(0,0,"网络端超时!");
        return -1;
    }
    flags=fcntl(conn_sockfd,F_GETFL);
    fcntl(conn_sockfd,F_SETFL,flags|O_NDELAY);

    //first 4 bytes is length indictor
//    ret =tcp_read_nbytes(conn_sockfd,(char *)msg_buffer,1024);
    ret =read(conn_sockfd, (char *)msg_buffer, 1024);
    if ( ret >0);
//    dcs_debug(msg_buffer,ret,"从网络端接收到的数据");
 /*   
    for(bytes_to_read=0; ;  )
    {
        ret = tcp_read_nbytes(conn_sockfd,(char *)msg_buffer+bytes_to_read,1);
        
        if( ret >0)
            bytes_to_read++;
        else if ( ret ==0 )
            break;
        else
        {
               if( bytes_to_read <=0)
			return -1;
               break;
	}
    }//for

*/
    //return the number of bytes placed into 'msg_buffer'
   return ret;
//    return bytes_to_read;
}
//--------------------------------------------------------------------------



int write_msg_to_jmj(int conn_sockfd, void *msg_buffer, int nbytes,int ntimeout)
{
    //
    // write a message to net. the format of a message
    // is (length_indictor, message_content)
    //

    
    int   ret;

    //clear the error number
    errno = -1;

    //check for writablity within 'ntimeout' ms
    if(tcp_check_writable(conn_sockfd,ntimeout) < 0)
        //timeout or other error
    {
    	    dcs_log(0,0,"tcp_check_writable fail");
    	    return -1;
    }
//        dcs_debug(msg_buffer,nbytes,"发送到网络端数据 长度＝%d",nbytes);
//    flags=fcntl(conn_sockfd,F_GETFL);
//    fcntl(conn_sockfd,F_SETFL,flags|O_DELAY);
//    ret = write(conn_sockfd, msg_buffer, nbytes);
        ret = tcp_write_nbytes(conn_sockfd, msg_buffer, nbytes);
        if(ret < nbytes) //write error
            return -1;
     //return the number of bytes written to socket
    return ret;
}
//--------------------------------------------------------------------------

int tcp_pton(char *strAddr)
{
  int addr;

  if(strAddr==NULL || *strAddr==0)
    return -1;

  if((addr=inet_addr(strAddr)) != INADDR_NONE)
	  return addr;
  else  //'strAddr' may be the host name
  {
    struct hostent *ph;
    ph=gethostbyname(strAddr);
    if(!ph)
	    return -1;

    return ((struct in_addr *)ph->h_addr)->s_addr;
  }
}

//--------------------------------------------------------------------------
int read_msg_from_NAC(int conn_sockfd, void *msg_buffer, int nbufsize,int ntimeout)
{
    //
    // read a message from net. the format of a message
    // is (length_indictor, message_content)
    //

    char  buffer[1024];
    unsigned char  bufferlen[1024];
    int   ret, bytes_to_read, msg_length,flags;
                int nleft, nbytes,s,i;
    //clear the error number
    errno = -1;

    //check for readability within 'ntimeout' ms
    if(tcp_check_readable(conn_sockfd,ntimeout) < 0)
    {
        //timeout or other error
        return -1;
    }
    flags=fcntl(conn_sockfd,F_GETFL);
    fcntl(conn_sockfd,F_SETFL,flags|O_NDELAY);

    //first 4 bytes is length indictor
    
    while(1)
    {
    	ret = read(conn_sockfd, msg_buffer, 2);
    	if(ret <=0) //read error
    	{
                if( ret <0 && ( errno == EINTR || errno == EAGAIN)) continue;
        	 dcs_log(0,0,"recv error 1");
         	return -1;
    	}
    	else if ( ret <2) return 0;
      	break;
    }

    memset( bufferlen, 0, sizeof(bufferlen));
    memcpy( bufferlen, msg_buffer,2);
//    dcs_log(bufferlen,2,"recv data");
    dcs_log(msg_buffer,2,"recv data");
    msg_length=(*bufferlen)*256+(*(bufferlen+1));
    dcs_log(0,0,"recv data len=%d",msg_length);
    bytes_to_read=0;
    if (nbufsize <msg_length) return 0;
        if(tcp_check_readable(conn_sockfd,1000) < 0)
    {
        //timeout or other error
        return -1;
    }
    flags=fcntl(conn_sockfd,F_GETFL);
                fcntl(conn_sockfd,F_SETFL,flags|O_NDELAY);
//    if(tcp_check_readable(conn_sockfd,1000) < 0)
 //   {  
        //timeout or other error
//        return -1;
//    }
    ret = read(conn_sockfd, msg_buffer+2, msg_length);
    if ( ret <=0)
    {
        dcs_log(0,0,"recv error 2");
         return -1;
    }
//    dcs_debug(msg_buffer,ret,"recv data 1 len=%d",ret);
    bytes_to_read = ret;
    if ( bytes_to_read >=msg_length) return bytes_to_read+2;
        
    if(tcp_check_readable(conn_sockfd,1000) < 0)
    {   
        //timeout or other error
        return -1;
    }

    flags=fcntl(conn_sockfd,F_GETFL);
                fcntl(conn_sockfd,F_SETFL,flags|O_NDELAY);
    ret = read(conn_sockfd, (char *)msg_buffer+2+bytes_to_read, msg_length-bytes_to_read);
    if ( ret <=0)
    {
        dcs_log(0,0,"recv error 3");
                 return -1;
    }
    bytes_to_read = bytes_to_read+ret+2;
    if(bytes_to_read < msg_length) //read error
        return -1;

    //return the number of bytes placed into 'msg_buffer'
    return bytes_to_read+2;
}
