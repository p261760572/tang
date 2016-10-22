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
#include <openssl/rsa.h>                         
#include <openssl/err.h> 
#include "gateway.h"
#define MAX_LINK 100 
extern gs_run_mode;
extern fd_set read_set,write_set,error_set;
extern struct FD_REC gl_fd[FD_SETSIZE];
extern int gl_sock[MAX_LINK],role,stop,crypt_flag,iso_mode;
extern struct PARA g_para[MAX_LINK];
#define PUBLIC_EXPONENT RSA_F4
void do_loop2_1()
{
	
  fd_set vread_set,vwrite_set,verror_set;
  struct timeval tv;
  
  int max_fd ,vmax_fd,i ,srv_flg,sock,cli_flg,ret,error,len;
  int flags,l,s;
  unsigned char buffer[8192],tmp[32],rsa_n[513],ver_time[15];
  char * p;
  time_t rsa_time;
  struct  tm *tm;
 
  RSA *r;
  


loop_init:
  max_fd = -1;
  
  srv_flg=0;
  cli_flg =0;
  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&error_set);
  stop =0;
   for ( i=0 ;i < MAX_LINK ; i++ )
	 {
		  	  if( ! g_para[i].used ) continue;
    //创建客户端接收端口
    
    	  sock=tcp_open_server(NULL, g_para[i].loc_port);
    	  if( sock >=0 )
        {
        	add_to_set(sock, &read_set, &max_fd);
        	add_to_set(sock, &error_set, &max_fd);
        	gl_fd[sock].sockfd=sock;
          gl_fd[sock].type = 0;
          gl_fd[sock].timeout =0;
          gl_fd[sock].used=0x30;
          gl_fd[sock].ndx=i;
         
				  strncpy(gl_fd[sock].ip,g_para[i].svr_ip,sizeof(gl_fd[sock].ip)-1);
          gl_fd[sock].port=g_para[i].svr_port;
	        gl_fd[sock].data_len=0;
	        gl_fd[sock].msg_len=0;
	        dcs_log(0,0,"open local port succ ![%d][%d]\n",g_para[i].loc_port,sock);
        }
        else
        {
        	 dcs_log(0,0,"open local port fail ![%d][%s]",g_para[i].loc_port,strerror(errno));
        	 _srv_exit(0);
        		
        }
   }
 
  while (1) 
  {
    
    if ( stop ) break;

    	tv.tv_sec = 1;
      tv.tv_usec = 0;
      vread_set=read_set;
      vwrite_set=write_set;
      verror_set=error_set;
      vmax_fd=0;
      for ( i=max_fd; i>1 ;i--)
      {
      	if( gl_fd[i].used)
      	{
      			 vmax_fd=i;
      			 break;
      	}
      }
      if ( vmax_fd <max_fd) max_fd=vmax_fd;
    	ret=select(max_fd + 1, &vread_set, &vwrite_set, &verror_set, &tv);
//    	dcs_log(0,0," recv select event ret=%d !\n",ret);
      if ( ret < 0) 
    	{
	    	 dcs_log(0,0,"select error! %s max_fd=%d",strerror(errno),max_fd);
	    	 if( errno != EINTR && errno != EAGAIN)
	    	 {
	    	 	 for( i=0; i<=max_fd;i++)
	    	 	 {
	    	 	 	  if(gl_fd[i].used) close(i);
	    	 	 }
	    	 	 goto loop_init;
	    	 }
      }
      else if ( ret == 0) /*空闲检查sock是否超时*/
      {
      	    for( i=0 ;i <=max_fd; i++)
	    	 	  {
//关闭超时连接
	    	 	  	 if ( gl_fd[i].used)
	    	 	  	 {
		    	 	  	 if (gl_fd[i].timeout>0)
		    	 	  	 if(  gl_fd[i].last_time+gl_fd[i].timeout <= time(NULL))
		    	 	  	 {
	    	 	  	 	   close(gl_fd[i].sockfd);
			    	 	  	 FD_CLR(gl_fd[i].sockfd,&write_set);
			    	 	  	 FD_CLR(gl_fd[i].sockfd,&read_set);
			    	 	  	 FD_CLR(gl_fd[i].sockfd,&error_set);
			    	 	  	 gl_fd[i].used=0x00;
			    	 	  	 FD_CLR(gl_fd[i].ndx,&write_set);
			    	 	  	 FD_CLR(gl_fd[i].ndx,&read_set);
			    	 	  	 FD_CLR(gl_fd[i].ndx,&error_set);
			    	 	  	 gl_fd[gl_fd[i].ndx].used=0x00;
			    	 	  	 dcs_log(0,0,"sockfd timeout!");
		    	 	  	 }
		    	 	   }
	    	 	  	
	    	 	  }
      } 
      else 
      {
        i=0;
        do{
//	       	  dcs_debug(0,0,"i=%d",i);  	  	
	    	  	if(FD_ISSET(i, &vwrite_set))/* 对主动建链的链路判断链接是否成功   */
	    	  	{
	    	  		  ret--;
//	    	  		  dcs_debug(0,0,"entry write set process sock=%d",i);
	    	  		  len=sizeof(error);
	  	 	  	 	  getsockopt(gl_fd[i].sockfd,SOL_SOCKET,SO_ERROR,&error,&len) ;                 
                FD_CLR(gl_fd[i].sockfd,&write_set);
                if( 0 != error )
                { 
                   close(gl_fd[i].sockfd);
	 	  	 	  	     gl_fd[i].used=0x00;	  
	 	  	 	  	     FD_CLR(gl_fd[i].sockfd,&error_set);
	 	  	 	  	     close(gl_fd[i].ndx);
	 	  	 	  	     gl_fd[gl_fd[i].ndx].used=0x00;	
	 	  	 	  	     FD_CLR(gl_fd[i].ndx,&error_set);
	 	  	 	  	     FD_CLR(gl_fd[i].ndx,&read_set);
	 	  	 	  	     dcs_log(0,0," app svr socket error!addr=[%s:%d][%s]",gl_fd[gl_fd[i].ndx].ip,gl_fd[gl_fd[i].ndx].port,strerror(errno));	
	 	  	 	          	 	         
                }
                else
                {
                		 add_to_set(i,&read_set,&max_fd);
                		 gl_fd[i].status=1;
                		 gl_fd[i].data_len=0;
                		 gl_fd[i].msg_len=0;
                		 gl_fd[i].last_time= gl_fd[gl_fd[i].ndx].last_time;
                		 dcs_log(0,0,"connect server ready sock=%d",i);
                }
	              
	    	  	}
	    	 	  else if(FD_ISSET(i, &vread_set)) /*对进入接收状态链路进行处理*/
	    	 	  {
	    	 	  		ret--;
	    	 	  	 if (gl_fd[i].type == 1)//应用服务器收到的报文
	    	 	  	 { 

	    	 	  	 	  len=read(i,buffer,sizeof(buffer));
	    	 	  	 	  if ( len <= 0)
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop2;
	    	 	  	 	  	if (len <0)
	    	 	  	 	  		dcs_log(0,0,"posp recv data error![%s]",strerror(errno));
	    	 	  	 	  	close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	  	close(gl_fd[gl_fd[i].ndx].sockfd);
  	 	  	 	  	    gl_fd[gl_fd[i].ndx].used=0x00;
  	 	  	 	  	    FD_CLR(gl_fd[gl_fd[i].ndx].sockfd,&read_set);
  	 	  	 	  	    FD_CLR(gl_fd[gl_fd[i].ndx].sockfd,&error_set);	
	    	 	  	 	  }
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	dcs_log(buffer,len,"from server len=%d sock=%d",len,i);
	    	 	  	 	  	 gl_fd[i].last_time= time(NULL);
			    	 	  	 	 len = write(gl_fd[i].ndx,buffer,len);
  	 	  	 	  	 	   if ( len >0)
   	 	  	 	  		 	    dcs_log(buffer,len,"write to client len=%d,sock=%d",len,gl_fd[i].ndx);
   	 	  	 	  		 	 else
   	 	  	 	  		 	 	  dcs_log( 0,0,"send to client error[%d][%s]",errno,strerror(errno));
	    	 	  	 	  	  
	    	 	  	 	  } 
	    	 	  	 }
	    	 	  	 else if (gl_fd[i].type == 0) //客户端新连接
	    	 	  	 {
	    	 	  	 	
	    	 	  	 	  int client_addr,client_port,sock1;
	    	 	  	 	  sock=tcp_accept_client(i,&client_addr, &client_port);
	    	 	  	 	 	if ( sock <0 )
	    	 	  	 	 	{    
	    	 	  	 	 		 dcs_log(0,0,"accpet client new socket fail ![%s]",strerror(errno));
	
	    	 	  	 	  }
	    	 	  	 	 	else if ( sock >=FD_SETSIZE -1 )
	    	 	  	 	 	{
	    	 	  	 	 	//超出最大连接限制
	    	 	  	 	 	   dcs_log(0,0,"client nums limited!\n");
	    	 	  	 	 		 close(sock);
	    	 	  	 	  } 
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	add_to_set(sock, &read_set, &max_fd);
						        gl_fd[sock].sockfd=sock;
						        gl_fd[sock].type = 2;
						        gl_fd[sock].timeout =90;
						        gl_fd[sock].last_time=time(NULL);
						        gl_fd[sock].used=0x30;
					         
					          sock1=tcp_connet_server(gl_fd[i].ip, gl_fd[i].port,0);
							      if( sock1 >=0 )
							      {
							        add_to_set(sock1, &write_set, &max_fd);
							        add_to_set(sock1, &error_set, &max_fd);
							        gl_fd[sock1].sockfd=sock1;
							        gl_fd[sock1].type = 1;
							        gl_fd[sock1].timeout =90-2;
							        gl_fd[sock1].used=0x30;
							        gl_fd[sock1].ndx=sock;
							        gl_fd[sock1].last_time =time(NULL)-60;
							        gl_fd[sock].ndx=sock1;
							      }
							      else
							      {
							      	dcs_log(0,0,"tcp_connet_server ip=%s port=%d fail![%s]",gl_fd[i].ip,gl_fd[i].port,strerror(errno));
							      	gl_fd[sock].used=0x00;
							      	close(sock);
							      }
							      dcs_log(0,0," new client connected! from sock =%d to sock =%d\n",sock,gl_fd[sock].ndx);
					        }
	    	 	  	 }
	    	 	  	 else  //客户端收到的报文
	    	 	  	 {
	    	 	  	 	 
    	 	  	 	    len = recv(i,buffer,sizeof(buffer),0);
	    	 	  	 	  if ( len <=0 )
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop2;
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	  	close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	
	    	 	  	 	  		FD_CLR(gl_fd[i].ndx,&read_set);
	    	 	  	 	  		FD_CLR(gl_fd[i].ndx,&write_set);
	    	 	  	 	  	  FD_CLR(gl_fd[i].ndx,&error_set);
	    	 	  	 	  //	  FD_CLR(gl_fd[i].ndx,&vread_set);
	    	 	  	 	  //		FD_CLR(gl_fd[i].ndx,&vwrite_set);
	    	 	  	 	  	//  FD_CLR(gl_fd[i].ndx,&verror_set);
	    	 	  	 	  	  close(gl_fd[i].ndx);
	    	 	  	 	  	  gl_fd[gl_fd[i].ndx].used=0x00;
	    	 	  	 	  	  dcs_log(0,0," client link passive closed![%s]",strerror(errno));
	    	 	  	 	  	 	goto cri_loop2;
	    	 	  	 	  }
	    	 	  	 	  dcs_log(buffer,len,"from client len=%d sock=%d",len,i);
    	 	  	      len = write(gl_fd[i].ndx,buffer,len);
    	 	  	      if( len <=0)
    	 	  	      {
    	 	  	      	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	  	close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	
	    	 	  	 	  		FD_CLR(gl_fd[i].ndx,&read_set);
	    	 	  	 	  		FD_CLR(gl_fd[i].ndx,&write_set);
	    	 	  	 	  	  FD_CLR(gl_fd[i].ndx,&error_set);
	    	 	  	 	  	//  FD_CLR(gl_fd[i].ndx,&vread_set);
	    	 	  	 	  	//	FD_CLR(gl_fd[i].ndx,&vwrite_set);
	    	 	  	 	  	//  FD_CLR(gl_fd[i].ndx,&verror_set);
	    	 	  	 	  	  close(gl_fd[i].ndx);
	    	 	  	 	  	  gl_fd[gl_fd[i].ndx].used=0x00;
	    	 	  	 	  	  dcs_log(0,0," server passive closed![%s]",strerror(errno));
    	 	  	      }
    	 	  	      dcs_log(buffer,len,"write to server len=%d sock=%d",len,gl_fd[i].ndx);
               }
	    	 	  }
	    	 	  else if(FD_ISSET(i, &verror_set))
	    	 	  {
	    	 	  	      ret--;
	    	 	  	      close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&write_set);
	    	 	  	 	    FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	   
	    	 	  }
cri_loop2:
	      ;
        } while(ret >0 && (i++)<=max_fd);
      }
    
  }
  
  for ( i=0; i <= max_fd;i++)
  {
  	 if( gl_fd[i].used )
  	 {
  	 	  close(gl_fd[i].sockfd);
  	 	  gl_fd[i].used=0;
  	 }
  }
}
