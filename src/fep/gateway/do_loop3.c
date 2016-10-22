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
extern int gl_sock,role,stop,crypt_flag,iso_mode;
extern struct PARA g_para;
#define PUBLIC_EXPONENT RSA_F4
void do_loop3()
{
	
  fd_set vread_set,vwrite_set,verror_set;
  struct timeval tv;
  
  int max_fd ,vmax_fd,i ,srv_flg,sock,cli_flg,ret,error,len;
  int flags,l,s;
  unsigned char buffer[8192],tmp[32],rsa_n[513],ver_time[15];
  char * p;
  time_t rsa_time;
  struct  tm *tm;

  


loop_init:
  max_fd = -1;
  
  srv_flg=0;
  cli_flg =0;
  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&error_set);
  stop =0;
   
    //创建客户端接收端口
    
  sock=tcp_open_server(NULL, g_para.loc_port);
  if( sock >=0 )
  {
  	add_to_set(sock, &read_set, &max_fd);
  	add_to_set(sock, &error_set, &max_fd);
  	gl_fd[sock].sockfd=sock;
    gl_fd[sock].type = 0;
    gl_fd[sock].timeout =0;
    gl_fd[sock].used=0x30;
    gl_fd[sock].len_type=1;
    gl_fd[sock].len_len=2;
    strncpy(gl_fd[sock].ip,g_para.svr_ip,sizeof(gl_fd[sock].ip)-1);
    gl_fd[sock].port=g_para.svr_port;
    gl_fd[sock].data_len=0;
    gl_fd[sock].msg_len=0;
    dcs_log(0,0,"open local port succ ![%d][%d]\n",g_para.loc_port,sock);
  }
  else
  {
  	 dcs_log(0,0,"open local port fail ![%d][%s]",g_para.loc_port,strerror(errno));
  	 _srv_exit(0);
  		
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
		    	 	  	 {	
			    	 	  	 if(  gl_fd[i].last_time+gl_fd[i].timeout <= time(NULL))
			    	 	  	 {
		    	 	  	 	   close(gl_fd[i].sockfd);
				    	 	  	 FD_CLR(gl_fd[i].sockfd,&write_set);
				    	 	  	 FD_CLR(gl_fd[i].sockfd,&read_set);
				    	 	  	 FD_CLR(gl_fd[i].sockfd,&error_set);
				    	 	  	 gl_fd[i].used=0x00;
				    	 	  	 perror("sockfd timeout!");
			    	 	  	 }
		    	 	  	 }
		    	 	   }
	    	 	  	
	    	 	  }
      } 
      else 
      {
        i=0;
        do{
	       	  dcs_debug(0,0,"i=%d",i);  	  	
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
	 	  	 	  	    
	 	  	 	  	     dcs_log(0,0," app svr socket error!addr=[%s:%d][%s]",gl_fd[i].ip,gl_fd[i].port,strerror(errno));	
	 	  	 	          	 	         
                }
                else
                {
                	   memcpy(gl_fd[i].remark,gl_fd[i].buf+2,1);
                	   memcpy(gl_fd[i].remark+1,gl_fd[i].buf+5,2);
                	   memcpy(gl_fd[i].remark+3,gl_fd[i].buf+3,2);
                		 len= write(gl_fd[i].sockfd,gl_fd[i].buf,gl_fd[i].data_len,0);
                		 if ( len <=0)
                		 {
                		 	   close(gl_fd[i].sockfd);
				 	  	 	  	     gl_fd[i].used=0x00;	  
				 	  	 	  	     FD_CLR(gl_fd[i].sockfd,&error_set);
				 	  	 	  	     
                		 }
                		 add_to_set(gl_fd[i].sockfd,&read_set,&max_fd);
                		 gl_fd[i].status=1;
                		 gl_fd[i].data_len=0;
                		 gl_fd[i].msg_len=0;
                		 gl_fd[i].last_time= time(NULL);
                		 dcs_debug(gl_fd[i].buf,len,"send posp data");
                		 
                }
	              
	    	  	}
	    	 	  else if(FD_ISSET(i, &vread_set)) /*对进入接收状态链路进行处理*/
	    	 	  {
	    	 	  		ret--;
	    	 	  		dcs_debug(0,0,"sockfd=[%d],type=[%d]!\n",gl_fd[i].sockfd,gl_fd[i].type);
	    	 	  	 if (gl_fd[i].type == 1)//应用服务器收到的报文
	    	 	  	 { 
//	    	 	  	 	  memset(buffer,0,sizeof(buffer));
	    	 	  	 	  len=read(gl_fd[i].sockfd,gl_fd[i].buf+gl_fd[i].data_len,sizeof(gl_fd[i].buf)-gl_fd[i].data_len);
	    	 	  	 	  if ( len <= 0)
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop2;
	    	 	  	 	  	close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	  	
	    	 	  	 	  	if( len <0)
	    	 	  	 	  	dcs_log(0,0,"posp recv data error![%s]",strerror(errno));

	    	 	  	 	  }
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	  gl_fd[i].last_time= time(NULL);
	
			    	 	  	 	  dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"\nrecv posp data len=%d,data_len=%d,len_type=%d crypt_flag=%d\n",len,gl_fd[i].data_len,gl_fd[i].len_type,crypt_flag);
			    	 	  	 	  gl_fd[i].data_len= gl_fd[i].data_len+len;

  	 	  	 	  		 	  if ( gl_fd[i].len_type==1 && gl_fd[i].data_len >2)
  	 	  	 	  		 	  {
  	 	  	 	  		 	  
  	 	  	 	  		 	  	gl_fd[i].msg_len=(unsigned char)gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1];
  	 	  	 	  		 	  	if ( gl_fd[i].msg_len>0 &&  gl_fd[i].msg_len<= gl_fd[i].data_len)
  	 	  	 	  		 	  	{	 
  	 	  	 	  		 	  		gl_fd[i].status=4;
	
					    	 	  	 	}
					    	 	  	 	
					    	 	  	 	if ( gl_fd[i].msg_len >8170  )
					    	 	  	 	{
					    	 	  	 		 gl_fd[i].msg_len=0;
					    	 	  	 		 gl_fd[i].data_len=0;
					    	 	  	 		 gl_fd[i].status=1;
					    	 	  	 		 goto cri_loop2;
					    	 	  	 	}
  	 	  	 	  		 	  }
  	 	  	 	  		 	  
  	 	  	 	  		 	  if ( gl_fd[i].status==4)
  	 	  	 	  		 	  {
  	 	  	 	  		 	    dcs_debug(gl_fd[i].remark,5,"begin write to client data,remark data,len_len=%d,msg_len=%d",gl_fd[i].len_len,gl_fd[i].msg_len);
  	 	  	 	  		 	    
  	 	  	 	  		      memcpy(gl_fd[i].buf+2,gl_fd[i].remark,5);
  	 	  	 	  	 	    	len = write(gl_sock,gl_fd[i].buf,gl_fd[i].msg_len+gl_fd[i].len_len);
  	 	  	 	  	
    	 	  	 	  		 	  if ( len <= 0)
    	 	  	 	  		 	   		 	  	//发送数据失败
    	 	  	 	  		 	  	 dcs_log(gl_fd[i].buf,gl_fd[i].msg_len+gl_fd[i].len_len,"posp recv data send client error![%s]",strerror(errno));
    	 	  	 	  		 	  else
  	 	  	 	  		 	  	 	 dcs_debug(gl_fd[i].buf,gl_fd[i].msg_len+gl_fd[i].len_len,"posp recv data send client succ!\n");

	 	  	 	  		 	  		 gl_fd[i].msg_len=0;
	 	  	 	  		 	  		 gl_fd[i].data_len=0;
	 	  	 	  		 	  		 gl_fd[i].status=1;
	 	  	 	  		 	  		 close(gl_fd[i].sockfd);
		    	 	  	 	  	   gl_fd[i].used=0x00;
		    	 	  	 	  	   FD_CLR(gl_fd[i].sockfd,&read_set);
		    	 	  	 	  	   FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	  		}
	    	 	  	 	  }
	    	 	  	 }
	    	 	  	 else if (gl_fd[i].type == 0) //客户端新连接
	    	 	  	 {
	    	 	  	 	
	    	 	  	 	  int client_addr,client_port;
	    	 	  	 	  sock=tcp_accept_client(gl_fd[i].sockfd,&client_addr, &client_port);
	    	 	  	 	 	if ( sock <0 )
	    	 	  	 	 	{    
	    	 	  	 	 		 dcs_log(0,0,"accpet client new socket fail ![%s]",strerror(errno));
	    	 	  	 	 		 close(gl_fd[i].sockfd);
	    	 	  	 	 		 gl_fd[i].used=0x00;
	//    	 	  	 	 		 cli_flg=0;
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
						        gl_fd[sock].len_type=gl_fd[i].len_type;
						        gl_fd[sock].len_len=gl_fd[i].len_len;
						        strncpy(gl_fd[sock].ip,gl_fd[i].ip,sizeof(gl_fd[i].ip)-1);
                    gl_fd[sock].port=gl_fd[i].port;
						        gl_fd[sock].status=1;
						        gl_fd[sock].data_len=0;
						        gl_fd[sock].msg_len=0;
						        gl_fd[sock].timeout =0;
						        gl_fd[sock].last_time=time(NULL);
						        gl_fd[sock].used=0x30;
						        gl_sock=sock;
					          dcs_debug(0,0," new client connected! sock=%d\n",sock);
					        }
	    	 	  	 }
	    	 	  	 else  //客户端收到的报文
	    	 	  	 {
	    	 	  	 	 
	    	 	  	 	  dcs_debug(0,0,"client recving data ... \n");
	
	    	 	  	 	  len = recv(gl_fd[i].sockfd,gl_fd[i].buf+gl_fd[i].data_len,sizeof(gl_fd[i].buf)-gl_fd[i].data_len,0);
	    	 	  	 	  if ( len <=0 )
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop2;
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	  	close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	if ( len == 0)
	    	 	  	 	  	{
	    	 	  	 	  		
	    	 	  	 	  	  dcs_log(0,0," client link passive closed![%s]",strerror(errno));
	    	 	  	 	  	}
	    	 	  	 	  	else dcs_log(0,0,"recv client data error![%s]",strerror(errno));
	    	 	  	 	  	goto cri_loop2;
	    	 	  	 	  }

	    	 	  	 	  gl_fd[i].last_time= time(NULL);
	    	 	  	 	  dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"client recv data len=%d status=%d crypt_flag=%d\n",len,gl_fd[i].status,crypt_flag);
	    	 	  	 	  gl_fd[i].data_len=gl_fd[i].data_len+len;    	 	  	 	  
	    	 	  	 	 if( gl_fd[i].len_type == 1 && gl_fd[i].data_len >2)//两字节长度包处理
	    	 	  	 	 {
		    	 	  	 	 	gl_fd[i].msg_len= (unsigned char)gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1];
//                    dcs_debug(0,0,"entry status 1 proc msg_len=%d,data_len=%d",gl_fd[i].msg_len,gl_fd[i].data_len);
		    	 	  	 	 	if ( gl_fd[i].msg_len >8170  )
		    	 	  	 	 	{
		    	 	  	 	 		FD_CLR(gl_fd[i].sockfd,&read_set);
		    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
		    	 	  	 	  	close(gl_fd[i].sockfd);
		    	 	  	 	  	gl_fd[i].used=0x00;
		    	 	  	 	  	dcs_log(0,0,"recv client len error!len=[%d]",gl_fd[i].msg_len);
		    	 	  	 	  	goto cri_loop2;
		    	 	  	 	 	}
		    	 	  	 	
	    	 	  	 	  	   if ( gl_fd[i].msg_len <= gl_fd[i].data_len && gl_fd[i].msg_len>0) gl_fd[i].status=4;

	    	 	  	 	 }
	 
	    	 	  	 	 if( gl_fd[i].status ==  4)
	    	 	  	 	 {
	    	 	  	
							      sock=tcp_connet_server(gl_fd[i].ip, gl_fd[i].port,0);
							      if( sock >=0 )
							      {
							        add_to_set(sock, &write_set, &max_fd);
							        add_to_set(sock, &error_set, &max_fd);
							        gl_fd[sock].sockfd=sock;
							        gl_fd[sock].type = 1;
							        gl_fd[sock].timeout =90-2;
							        gl_fd[sock].used=0x30;
							        gl_fd[sock].last_time =time(NULL)-60;
							        gl_fd[sock].len_type=gl_fd[i].len_type;
							        gl_fd[sock].len_len=gl_fd[i].len_len;
							        gl_fd[sock].status=0;
							        memcpy(gl_fd[sock].buf,gl_fd[i].buf,gl_fd[i].data_len);
							        gl_fd[sock].data_len=gl_fd[i].data_len;
							        gl_fd[sock].msg_len=gl_fd[i].msg_len;
							        gl_fd[i].msg_len=0;
							        gl_fd[i].data_len=0;
							        dcs_debug(0,0,"connect svr_ip succ! [%s]:[%d][sock:%d]\n",gl_fd[i].ip,gl_fd[i].port,sock);
							      }
		    	 	  	 	  else 
		    	 	  	 	  {
		    	 	  	 	  	close(gl_fd[i].sockfd);
		    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
		    	 	  	 	    FD_CLR(gl_fd[i].sockfd,&error_set);
		    	 	  	 	    gl_fd[i].used=0x00;
		    	 	  	 	  
		    	 	  	 	  	dcs_debug(0,0," connect posp fail [%s:%d][%s]",gl_fd[i].ip,gl_fd[i].port,strerror(errno));
		    	 	  	 	  	
		    	 	  	 	  }

	    	 	  	 	}
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
