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
	//if (setsockopt(sock, SOL_TCP, SO_NODELAY, (char *)&on, sizeof(on)))
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
//    dcs_debug(buffer,nbytes - nleft,"recv data len=%d",nbytes - nleft);
    if ((nread = read(conn_sock, ptr, nleft)) < 0)
    {
	    if (errno == EINTR || errno == EAGAIN)
	    {
	        nread = 0; // and call read() again
	    }
	    else
	    {
	        dcs_log(0,0,"socket recv error! errno=%d,errstr=%s",errno,strerror(errno));
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
int read_msg_from_fsk(int conn_sockfd, void *msg_buffer,int nbufsize,int ntimeout)
{
    //
    // read a message from net. the format of a message
    // is (length_indictor, message_content)
    //

    unsigned char  buffer[256];
    int   i,ret, bytes_to_read, msg_length,flags,s;

    //clear the error number
//    errno = -1;

    //check for readability within 'ntimeout' ms
    if(tcp_check_readable(conn_sockfd,ntimeout) < 0)
    {
        //timeout or other error
        dcs_log(0,0,"tcp_check_readable error");
        return -1;
    }
    memset(buffer,0,sizeof(buffer));
//    ret = tcp_read_nbytes(conn_sockfd,(char *)buffer,4);
    ret = recv(conn_sockfd,(char *)buffer,1,0);
    if ( ret <=0)
    {
    	dcs_log(0,0,"<read_msg_from_fsk>link closed[%d]",strerror(errno));
    	return -1;
    }
    if ( buffer[0] != 0x03)
    {
    	dcs_log(0,0,"<read_msg_from_fsk>invalid flag03 [%2x]",(unsigned char)buffer[0]);
    	return -1;
    }
    
    ret = recv(conn_sockfd,(char *)buffer,2,0);
    if ( ret<2 )
    {  
     if ( ret >0 )
      dcs_debug(buffer,ret,"recv data len error");
     else  if ( ret == 0)
     	  dcs_log(0,0,"<read_msg_from_fsk>link closed[%d]",strerror(errno));
     else
     	 	dcs_log(0,0,"<read_msg_from_fsk>recv data len error[%d]-%s",errno,strerror(errno));
      return -1;
    }
//    dcs_debug(buffer,2,"recv  len data ");
    if ( (buffer[0]&0x0f) >9 ||
    	   (buffer[0]>>4) >9 ||
    	   (buffer[1]&0x0f) >9 ||
    	   (buffer[1]>>4) >9 )
    {
    	 dcs_log(buffer,2,"recv error len ");
  		 return -1;
    }
    
	  msg_length=(((buffer[0]>>4)&0x0f)*1000) +((buffer[0]&0x0f)*100)+(((buffer[1]>>4)&0x0f)*10)+(buffer[1]&0x0f);
//    dcs_debug(buffer,2,"recv  len[%d] ",msg_length);
    if ( msg_length ==0 )
    {
//    	   dcs_log(0,0,"error len =%d",msg_length);
    		 return 0;
    }
  	if ( nbufsize <msg_length )  msg_length=nbufsize;
  	 
  	bytes_to_read =0;
  	s=msg_length;
  	for ( i=0; i <4;i++)
  	{ 
				if(tcp_check_readable(conn_sockfd,1000) < 0)
				{
					 dcs_debug(buffer,ret,"<for>tcp_check_readable error[%d]-%s",errno,strerror(errno));
					 return -1;
		    }

		    ret = read(conn_sockfd,(char *)msg_buffer+bytes_to_read,s);
        if( ret >0)
        {
            bytes_to_read=bytes_to_read+ret;
        }
        else if ( ret==0)
        	  break;
        else
        {
        	  dcs_log(0,0,"socket read data return -1,bytes_to_read=%d,ret=%d",bytes_to_read,ret);
        	  return -1;
        }
        s=s-ret;
      	if (bytes_to_read >=msg_length) return bytes_to_read;
      	if ( s<=0) return bytes_to_read;
					
		}
		
    //return the number of bytes placed into 'msg_buffer'
    return bytes_to_read;
}
int read_msg_from_net(int conn_sockfd, void *msg_buffer,int nbufsize,int ntimeout)
{
    //
    // read a message from net. the format of a message
    // is (length_indictor, message_content)
    //

    char  buffer[256];
    int   i,ret, bytes_to_read, msg_length,flags,s;

    //clear the error number
//    errno = -1;

    //check for readability within 'ntimeout' ms
    if(tcp_check_readable(conn_sockfd,ntimeout) < 0)
    {
        //timeout or other error
        dcs_log(0,0,"tcp_check_readable error");
        return -1;
    }
/*    
    flags=fcntl(conn_sockfd,F_GETFL);
    fcntl(conn_sockfd,F_SETFL,flags|O_NDELAY);
*/    memset(buffer,0,sizeof(buffer));
//    ret = tcp_read_nbytes(conn_sockfd,(char *)buffer,4);
    ret = recv(conn_sockfd,(char *)buffer,4,0);
    if ( ret<4 )
    {  
     if ( ret >0 )
     {
     	 dcs_debug(buffer,ret,"recv data len error");
     	 return 0;
     }
     else  if ( ret == 0)
     	  dcs_log(0,0,"<read_msg_from_net>link closed[%d]",errno);
     else
     {
        if (errno == EINTR || errno == EAGAIN) return 0;
     	 	dcs_log(0,0,"<read_msg_from_net>recv data len error[%d]-%s",errno,strerror(errno));
     }
      return -1;
    }
	  msg_length=atoi(buffer);
   // dcs_log(0,0,"recv data len=%d",msg_length);
    if (msg_length <0)
  	{
  		 dcs_debug(buffer,4,"recv error len ");
  		 return 0;
  	}
  	else if (msg_length == 0)
  		 return 0;
  	if ( nbufsize <msg_length )  msg_length=nbufsize;
  	 
  	bytes_to_read =0;
  	s=msg_length;
  	for ( i=0; i <4;i++)
  	{ 
				if(tcp_check_readable(conn_sockfd,1000) < 0)
				{
					 dcs_debug(buffer,ret,"<for>tcp_check_readable error[%d]-%s",errno,strerror(errno));
					 return -1;
		    }
/*		    
		    flags=fcntl(conn_sockfd,F_GETFL);
	      fcntl(conn_sockfd,F_SETFL,flags|O_NDELAY);
*/
		    ret = read(conn_sockfd,(char *)msg_buffer+bytes_to_read,s);
        if( ret >0)
        {
            bytes_to_read=bytes_to_read+ret;
        }
        else
        {
        	if (errno == EINTR || errno == EAGAIN) continue;
        	  dcs_log(0,0,"socket read data error,bytes_to_read=%d,ret=%d",bytes_to_read,ret);
        	  return -1;
        }
        s=s-ret;
      	if (bytes_to_read >=msg_length) return bytes_to_read;
      	if ( s<=0) return bytes_to_read;
					
		}
		
    //return the number of bytes placed into 'msg_buffer'
    return bytes_to_read;
}
int read_msg_from_net1(int conn_sockfd, void *msg_buffer,int nbufsize,int ntimeout)
{
    //
    // read a message from net. the format of a message
    // is (length_indictor, message_content)
    //

    char  buffer[256];
    int   i,ret, bytes_to_read, msg_length,flags,s;

    //clear the error number
//    errno = -1;

    //check for readability within 'ntimeout' ms
    if(tcp_check_readable(conn_sockfd,ntimeout) < 0)
    {
        //timeout or other error
        return -1;
    }
    /*
    flags=fcntl(conn_sockfd,F_GETFL);
    fcntl(conn_sockfd,F_SETFL,flags|O_NDELAY);
    */
    memset(buffer,0,sizeof(buffer));
    ret = tcp_read_nbytes(conn_sockfd,(char *)buffer,4);
    if ( ret<4 )
            return -1;
	  msg_length=atoi(buffer)-4;
    dcs_log(0,0,"recv data len=%d",msg_length);
    if (msg_length <0)
  	{
  		 dcs_debug(buffer,4,"recv data len error");
  		 return 0;
  	}
  	else if (msg_length == 0)
  		 return 0;
  	if ( nbufsize <msg_length )  msg_length=nbufsize;
  	 
  	bytes_to_read =0;
  	s=msg_length;
  	for ( i=0; i <4;i++)
  	{ 
				if(tcp_check_readable(conn_sockfd,1000) < 0)
				{
					   return -1;
		    }
		    
		    ret = read(conn_sockfd,(char *)msg_buffer+bytes_to_read,s);
        if( ret >0)
        {
            bytes_to_read=bytes_to_read+ret;
        }
        else if ( ret==0)
        	  break;
        else
        {
        	  dcs_log(0,0,"socket read data return -1,bytes_to_read=%d,ret=%d",bytes_to_read,ret);
        	  return -1;
        }
        s=s-ret;
      	if (bytes_to_read >=msg_length) return bytes_to_read;
      	if ( s<=0) return bytes_to_read;
					
		}
		
    //return the number of bytes placed into 'msg_buffer'
    return bytes_to_read;
}

//--------------------------------------------------------------------------

int write_msg_to_fsk(int conn_sockfd, void *msg_buffer, int nbytes,int ntimeout)
{
    //
    // write a message to net. the format of a message
    // is (length_indictor, message_content)
    //

    char  buffer[8192+4];
    int   ret;

    //clear the error number
    errno = -1;
    //check for writablity within 'ntimeout' ms
    if(tcp_check_writable(conn_sockfd,ntimeout) < 0)
        //timeout or other error
    {
    	   dcs_log(0,0,"<write_msg_to_fsk>tcp_check_writable error[%d]-%s",errno,strerror(errno));
    	   dcs_log(0,0," comm socket timeout ! sockfd=[%d]",conn_sockfd);
    	   return -1;
    }

   
    if(nbytes < 0)  nbytes = 0;
    buffer[0]=0x03;
    sprintf(buffer+1,"%.4d",nbytes);
    buffer[1]=(buffer[1]<<4)|(buffer[2]&0x0f);
    buffer[2]=(buffer[3]<<4)|(buffer[4]&0x0f);
    memcpy(buffer+3,msg_buffer,nbytes);
    dcs_debug(buffer,nbytes+3,"send data len=%d",nbytes+3);
    //write the message
    if(nbytes >= 0 )
    {
				ret = write(conn_sockfd, buffer, nbytes+3);
        if(ret < (nbytes+3)) //write error
        {
        	dcs_log(0,0,"<write_msg_to_fsk>write fail[%d]-%s",errno,strerror(errno));
        	return -1;
        }
    }

    //return the number of bytes written to socket
    return ret-3;
}

int write_msg_to_net(int conn_sockfd, void *msg_buffer, int nbytes,int ntimeout)
{
    //
    // write a message to net. the format of a message
    // is (length_indictor, message_content)
    //

    char  buffer[8192+4];
    int   ret;

    //clear the error number
    errno = -1;
    //check for writablity within 'ntimeout' ms
    if(tcp_check_writable(conn_sockfd,ntimeout) < 0)
        //timeout or other error
    {
    	   dcs_log(0,0,"<write_msg_to_net>tcp_check_writable error[%d]-%s",errno,strerror(errno));
    	   dcs_log(0,0," comm socket timeout ! sockfd=[%d]",conn_sockfd);
    	   return -1;
    }

    //first 4 bytes is length indictor
    if(nbytes < 0)  nbytes = 0;
    sprintf(buffer,"%.4d",nbytes);
    memcpy(buffer+4,msg_buffer,nbytes);
    //write the message
    if(nbytes >= 0 && msg_buffer)
    {
				while(1)
				{
					ret = write(conn_sockfd, buffer, nbytes+4);
	//        ret = tcp_write_nbytes(conn_sockfd, buffer, nbytes+4);
	        if(ret < (nbytes+4)) //write error
	        {
	        	if( ret <0)
	        	if (errno == EINTR || errno == EAGAIN) continue;
	        	dcs_log(0,0,"<write_msg_to_net>write fail[%d]-%s",errno,strerror(errno));
	        	return -1;
	        }
	        break;
       }
    }

    //return the number of bytes written to socket
    return ret-4;
}
int write_msg_to_net1(int conn_sockfd, void *msg_buffer, int nbytes,int ntimeout)
{
    //
    // write a message to net. the format of a message
    // is (length_indictor, message_content)
    //

    char  buffer[8192+4];
    int   ret;

    //clear the error number
    errno = -1;
    //check for writablity within 'ntimeout' ms
    if(tcp_check_writable(conn_sockfd,ntimeout) < 0)
        //timeout or other error
    {
    	   dcs_log(0,0,"<write_msg_to_net1>tcp_check_writable fail[%d]-%s",errno,strerror(errno));
    	    return -1;
    }

    //first 4 bytes is length indictor
    if(nbytes < 0)  nbytes = 0;
    sprintf(buffer,"%.4d",nbytes+4);
    memcpy(buffer+4,msg_buffer,nbytes);
    //write the message
    if(nbytes >= 0 && msg_buffer)
    {
				while(1)
				{
				ret = write(conn_sockfd, buffer, nbytes+4);
//        ret = tcp_write_nbytes(conn_sockfd, buffer, nbytes+4);
        if(ret < (nbytes+4)) //write error
        {
        	if( ret <0)
	        	if (errno == EINTR || errno == EAGAIN) continue;
        	dcs_log(0,0,"<write_msg_to_net1>write fail[%d]-%s",errno,strerror(errno));
        	    return -1;
        }
        break;
        }
    }

    //return the number of bytes written to socket
    return ret-4;
}
//--------------------------------------------------------------------------
int read_msg_from_NAC(int conn_sockfd, void *msg_buffer, int nbufsize,int ntimeout)
{
    //
    // read a message from net. the format of a message
    // is (length_indictor, message_content)
    //

    char  buffer[1024];
    int   ret, bytes_to_read, msg_length,flags;
		int nleft, nbytes,s,i;
    //clear the error number
//    errno = -1;

    //check for readability within 'ntimeout' ms
    if(tcp_check_readable(conn_sockfd,ntimeout) < 0)
    {
        //timeout or other error
        return -1;
    }
    /*
    flags=fcntl(conn_sockfd,F_GETFL);
    fcntl(conn_sockfd,F_SETFL,flags|O_NDELAY);
*/
    //first 4 bytes is length indictor
    ret = read(conn_sockfd, buffer, 2);
    if(ret <0) //read error
    {
    	 
	     if (errno == EINTR || errno == EAGAIN) return 0;
    	 dcs_log(0,0,"<read_msg_from_NAC>recv error1[%d]-%s",errno,strerror(errno));
    	 return -1;
    }
    else if(ret ==0) //read error
    {
//    	 ret=errno;
    	 dcs_log(0,0,"<read_msg_from_NAC>recv link closed!");
//    	 errno=ret;
    	 return -1;
    }
    else if ( ret <2) return 0;
    
    buffer[2]   = 0;
    msg_length = (unsigned char)buffer[0]*256+(unsigned char)buffer[1];
//    dcs_debug(buffer,2,"recv data 1 [%d]",msg_length);
    bytes_to_read=0;
    if (nbufsize <msg_length) return 0;
  	if(tcp_check_readable(conn_sockfd,1000) < 0)
    {
        //timeout or other error
        return -1;
    }
    
//    if(tcp_check_readable(conn_sockfd,1000) < 0)
 //   {
        //timeout or other error
//        return -1;
//    }
    ret = read(conn_sockfd, msg_buffer, msg_length);
    if ( ret <=0)
    {
    	if (errno == EINTR || errno == EAGAIN) 
    		;
    	else
    	{
    	  dcs_log(0,0,"<read_msg_from_NAC>recv error2[%d]-%s",errno,strerror(errno));
    	  return -1;
    	}
    }
//    dcs_debug(msg_buffer,ret,"recv data 1 len=%d",ret);
    bytes_to_read = ret;	
    if ( bytes_to_read >=msg_length) return bytes_to_read;

    if(tcp_check_readable(conn_sockfd,1000) < 0)
    {
        //timeout or other error
        return -1;
    }

    
    ret = read(conn_sockfd, (char *)msg_buffer+bytes_to_read, msg_length-bytes_to_read);
    if ( ret <=0)
    {
    	dcs_log(0,0,"<read_msg_from_NAC>recv error3[%d]-%s",errno,strerror(errno));
    		 return -1;
    }
    bytes_to_read = bytes_to_read+ret;
    if(bytes_to_read < msg_length) //read error
        return -1;
   
    //return the number of bytes placed into 'msg_buffer'
    return bytes_to_read;
}


int read_msg_from_AMP(int conn_sockfd, void *msg_buffer, int nbufsize,int ntimeout)
{
    //
    // read a message from net. the format of a message
    // is (length_indictor, message_content)
    //

    
    int   ret, bytes_to_read, msg_length,flags,i;

    //clear the error number
//    errno = -1;

    //check for readability within 'ntimeout' ms
    if(tcp_check_readable(conn_sockfd,ntimeout) < 0)
    {
        //timeout or other error
        dcs_debug(0,0,"recv timeout");
        return -1;
    }
//    flags=fcntl(conn_sockfd,F_GETFL);
//    fcntl(conn_sockfd,F_SETFL,flags|O_NDELAY);
//    ret = tcp_read_nbytes(conn_sockfd,(char *)msg_buffer,nbufsize);

//    ret = read(conn_sockfd, (char *)msg_buffer, nbufsize) ; 
   
	    ret = recv(conn_sockfd, (char *)msg_buffer, nbufsize, O_NONBLOCK);  
	    
	    if( ret >0)
	        bytes_to_read=ret;
	    else
	    {
	    	if (errno == EINTR || errno == EAGAIN) return 0 ;
	    		dcs_debug(0,0,"recv data fail");
	    	  bytes_to_read = -1;
	    	  return -1;
	    }
    return bytes_to_read;
}
//--------------------------------------------------------------------------

int write_msg_to_NAC(int conn_sockfd, void *msg_buffer, int nbytes,int ntimeout)
{
    //
    // write a message to net. the format of a message
    // is (length_indictor, message_content)
    //

    char  buffer[2048+4];
    int   ret;

    //clear the error number
    errno = -1;

    //check for writablity within 'ntimeout' ms
    if(tcp_check_writable(conn_sockfd,ntimeout) < 0)
        //timeout or other error
    {
           dcs_log(0,0,"<write_msg_to_nac> tcp_check error!");	
    	    return -1;
    }

    //first 4 bytes is length indictor
    if(nbytes < 0)  nbytes = 0;
    buffer[0]=(unsigned char)(nbytes/256);
    buffer[1]=(unsigned char)(nbytes%256);
    memcpy(buffer+2,msg_buffer,nbytes);
    //write the message
    if(nbytes >= 0 && msg_buffer)
    {
        while(1)
        {
	        ret = write(conn_sockfd, buffer, nbytes+2);
	        if(ret < (nbytes+2)) //write error
	        {
	        	    if( ret <0)
	        	    if (errno == EINTR || errno == EAGAIN) continue;
	        	    return -1;
	        }
          break;
        }
    }

    //return the number of bytes written to socket
    return ret-2;
}

int write_msg_to_AMP(int conn_sockfd, void *msg_buffer, int nbytes,int ntimeout)
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
        return -1;

    while(1)
    {
	    ret = write(conn_sockfd, msg_buffer, nbytes);
	    
	     if( ret <0 )
	    	if (errno == EINTR || errno == EAGAIN) continue;
	    if(ret < nbytes) //write error
	         return -1;
	    break;
    }
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
