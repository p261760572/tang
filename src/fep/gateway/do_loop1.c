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
extern gs_run_mode;
extern fd_set read_set,write_set,error_set;
extern struct FD_REC gl_fd[FD_SETSIZE];
extern int gl_sock[100],role,stop;
extern struct PARA g_para[100];

void do_loop1()
{
	
  fd_set vread_set,vwrite_set,verror_set;
  struct timeval tv;
  
  int max_fd ,i ,sock,loc_flg=0,ret,error,len;
  int s,flags,l,vmax_fd=0;
  unsigned char buffer[8192],tmp[256];
 
 loop_init: 
  max_fd = -1;
  for( i=0 ;i <100; i++)
  	gl_sock[i]=-1;
 
 
  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&error_set);
  stop =0;
   for ( i=0 ;i < 100 ; i++ )
	 {
		  	  if( ! g_para[i].used ) continue;

    //创建数据请求端侦听端口
    
    	  sock=tcp_open_server(NULL, g_para[i].loc_port);
    	  if( sock >=0 )
        {
        	add_to_set(sock, &read_set, &max_fd);
        	add_to_set(sock, &error_set, &max_fd);
        	gl_fd[sock].sockfd=sock;
        	strncpy(gl_fd[sock].ip,g_para[i].svr_ip,sizeof(gl_fd[sock].ip)-1);
        	gl_fd[sock].port= g_para[i].svr_port;
        	gl_fd[sock].type=0;
          gl_fd[sock].len_type = g_para[i].type;
          if( gl_fd[sock].len_type==0) gl_fd[sock].len_len=4;
	        else if( gl_fd[sock].len_type==1) gl_fd[sock].len_len=2;
	        else if( gl_fd[sock].len_type==2) gl_fd[sock].len_len=4;
	        else if( gl_fd[sock].len_type==3) gl_fd[sock].len_len=4;
	        else if( gl_fd[sock].len_type==4) gl_fd[sock].len_len=4;
	        else if( gl_fd[sock].len_type==5 ) gl_fd[sock].len_len=0;
          gl_fd[sock].timeout =0;
          gl_fd[sock].used=0x30;
          gl_fd[sock].ndx=i;
//	        gl_fd[sock].remote_sockfd=gl_sock[i];
	        dcs_log(0,0,"open local port succ ![%d][%d]\n",g_para[i].loc_port,sock);
        }
        else
        {
        	 dcs_log(0,0,"open local port fail ![%d][%s]",g_para[i].loc_port,strerror(errno));
        	 _srv_exit(0);
        		
        }
   }
  	 
    while(1)
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
//    	fprintf(stderr," recv select event ret=%d !\n",ret);
//      dcs_log(0,0," recv select event ret=%d !",ret);
      if ( ret < 0) 
    	{
	    	 dcs_log(0,0,"select error![%s],max_fd=%d",strerror(errno),max_fd);
	    	 if( errno != EINTR && errno != EAGAIN)
	    	 {
	    	 	 for( i=0; i<=max_fd;i++)
	    	 	 {
	    	 	 	  if(gl_fd[i].used) close(i);
	    	 	 }
	    	 	 goto loop_init;
	    	 }
      }
      else if ( ret == 0)
      {
      	    for( i=0 ;i <=max_fd; i++)
	    	 	  {
//关闭超时连接
	    	 	  	 if (gl_fd[i].used)
	    	 	  	 {
		    	 	  	 if ( gl_fd[i].timeout >0)
		    	 	  	 if( gl_fd[i].last_time+gl_fd[i].timeout <= time(NULL))
		    	 	  	 {
			    	 	  	 close(gl_fd[i].sockfd);
			    	 	  	 FD_CLR(gl_fd[i].sockfd,&read_set);
			    	 	  	 gl_fd[i].used=0x00;
			    	 	  	 dcs_log(0,0,"client port timeout![%s]",strerror(errno));
		    	 	  	 }
		    	 	   }
	    	 	  	
	    	 	  }
      } 
      else 
      {
          i=0;
        do{
	        
//	    	  	if ( gl_fd[i].used )
	    	  	if(FD_ISSET(i, &vwrite_set))/* 对主动建链的链路判断链接是否成功   */
	    	  	{
	    	  		  ret--;
	    	  		  len=sizeof(error);
	  	 	  	 	  getsockopt(gl_fd[i].sockfd,SOL_SOCKET,SO_ERROR,&error,&len) ;
	                 
                FD_CLR(gl_fd[i].sockfd,&write_set);
                if( 0 != error )
                { 
                   close(gl_fd[i].sockfd);
	 	  	 	  	     gl_fd[i].used=0x00;
	 	  	 	  	     gl_fd[i].data_len=0;
	 	  	 	  	     gl_fd[i].msg_len=0;
	 	  	 	  	     gl_fd[i].status=0;	  
	 	  	 	  	     FD_CLR(gl_fd[i].sockfd,&error_set);	 	  	 	         
	 	  	 	         dcs_log(0,0,"  connect server socket error!addr=[%s]:[%d][%s]",gl_fd[i].ip,gl_fd[i].port,strerror(errno));	
	 	  	 	         
                }
                else
                {
                	 
                	  add_to_set(gl_fd[i].sockfd,&read_set,&max_fd);
                		if( gl_fd[i].len_type == 2)
                		{
                			memcpy(gl_fd[i].remark,gl_fd[i].buf+4,14);
                			sprintf(buffer,"%08d",gl_fd[i].msg_len-14-12);
                			memcpy(buffer+8,gl_fd[i].buf+4+14,gl_fd[i].msg_len-14);
                			memcpy(gl_fd[i].buf,buffer,gl_fd[i].msg_len-14+8);
                			gl_fd[i].msg_len=gl_fd[i].msg_len-14+8-4;
                		}
                		else if( gl_fd[i].len_type == 3)
                		{
                			memcpy(gl_fd[i].remark,gl_fd[i].buf+4,14);
                			sprintf(buffer,"%08d",gl_fd[i].msg_len-14-14);
                			memcpy(buffer+8,gl_fd[i].buf+4+14,gl_fd[i].msg_len-14);
                			memcpy(gl_fd[i].buf,buffer,gl_fd[i].msg_len-14+8);
                			gl_fd[i].msg_len=gl_fd[i].msg_len-14+8-4;
                		}
                		else if( gl_fd[i].len_type == 4)
                		{
                			sprintf(buffer,"%08d",gl_fd[i].msg_len);
                			memcpy(buffer+8,gl_fd[i].buf+4,gl_fd[i].msg_len);
                			memcpy(gl_fd[i].buf,buffer,gl_fd[i].msg_len+8);
                			gl_fd[i].msg_len=gl_fd[i].msg_len+4;
                		}
 	  	 	  		 	    len = write(gl_fd[i].sockfd,gl_fd[i].buf,gl_fd[i].msg_len+gl_fd[i].len_len);
	 	  	 	  		 	  if ( len <= 0)
	 	  	 	  		 	  {
	 	  	 	  		 	  	//发送数据失败
	 	  	 	  		 	  	dcs_log(0,0,"server recv data send client error![%s]",strerror(errno));
  	 	  	 	  		 	  close(gl_fd[i].sockfd);
  	 	  	 	  		 	  gl_fd[i].used=0x00;
  	 	  	 	  		 	  FD_CLR(gl_fd[i].sockfd,&read_set);
  	 	  	 	  		 	  FD_CLR(gl_fd[i].sockfd,&error_set);
	 	  	 	  		 	  }
	 	  	 	  		 	  else
	 	  	 	  		 	  {
	 	  	 	  		 	  	
	 	  	 	  		 	    dcs_debug(gl_fd[i].buf,len,"client recv data send server succ!len=%d\n",len);
	 	  	 	  		 	    gl_fd[i].status=1;
	 	  	 	  		 	    gl_fd[i].data_len=0;
	 	  	 	  		 	    gl_fd[i].msg_len=0;
	 	  	 	  		 	  }
	
                }
	              
	    	  	}
	    	  	else if(FD_ISSET(i, &vread_set))
	    	 	  {
	    	 	  	 ret--;
	    	 	  	 dcs_debug(0,0,"sockfd=[%d],type=[%d]!\n",gl_fd[i].sockfd,gl_fd[i].type);
	    	 	  	 if (gl_fd[i].type ==2 )    //客户交易请求端报文
	    	 	  	 { 
	 	
	                dcs_debug(0,0,"recv loc_port[%d]:%d",gl_fd[i].ndx,g_para[gl_fd[i].ndx].loc_port);
	    	 	  	 	  memset(buffer,0,sizeof(buffer));
	    	 	  	 	  len=recv(gl_fd[i].sockfd,gl_fd[i].buf+gl_fd[i].data_len,sizeof(gl_fd[i].buf)-gl_fd[i].data_len,0);
	    	 	  	 	  if ( len <= 0)
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop1;
	    	 	  	 	  	close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	    FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	    FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	    gl_sock[gl_fd[i].ndx]=-1;
	    	 	  	 	    if( len <0)
	    	 	  	 	  	dcs_log(0,0,"appsrv recv data error!sockfd=[%d][%s]",gl_fd[i].sockfd,strerror(errno));
//	    	 	  	 	  	if (connect_sock(i,&max_fd)<0)
//							       	_srv_exit(0); 	  
	    	 	  	 	  }
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	int t_flag=1;
	    	 	  	 	  	  dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"recv appsrv data len=%d\n",len);
	    	 	  	 	  	  gl_fd[i].data_len=gl_fd[i].data_len+len;
	    	 	  	 	  	  while(t_flag)
	    	 	  	 	  	  {
			    	 	  	 	 	    if ( gl_fd[i].len_type==1 && gl_fd[i].data_len >2)
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  
	    	 	  	 	  		 	  	gl_fd[i].msg_len= (unsigned char)gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1];
	    	 	  	 	  		 	  	if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
						    	 	  	 	  
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else  if ( gl_fd[i].len_type==0 && gl_fd[i].data_len >4)
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  	   memcpy(buffer,gl_fd[i].buf,4);
	    	 	  	 	  		 	  	   buffer[4]=0x00;
	    	 	  	 	  		 	  	   gl_fd[i].msg_len= atoi(buffer);
	    	 	  	 	  		 	  	   if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
								    	 	  	 	  
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else  if ( gl_fd[i].len_type==4 && gl_fd[i].data_len >4)
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  	   memcpy(buffer,gl_fd[i].buf,8);
	    	 	  	 	  		 	  	   buffer[4]=0x00;
	    	 	  	 	  		 	  	   gl_fd[i].msg_len= atoi(buffer);
	    	 	  	 	  		 	  	   if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
								    	 	  	 	  
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else if ( gl_fd[i].len_type==2 && gl_fd[i].data_len >4)
	    	 	  	 	  		 	  {
//	    	 	  	 	  		 	  	  dcs_debug(0,0,"entry xml data process");
	    	 	  	 	  		 	  	  memcpy(buffer,gl_fd[i].buf,4);
	    	 	  	 	  		 	  	  buffer[4]=0x00;
	    	 	  	 	  		 	  	  gl_fd[i].msg_len= atoi(buffer);
	    	 	  	 	  		 	  	  if ( gl_fd[i].msg_len >0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else if ( gl_fd[i].len_type==3 && gl_fd[i].data_len >4)
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf,4);
	    	 	  	 	  		 	  	 buffer[4]=0x00;
	    	 	  	 	  		 	  	 gl_fd[i].msg_len= atoi(buffer);
	    	 	  	 	  		 	  	 if ( gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  	 if ( gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
	    	 	  	 	  		 	  } 
				    	 	  	 	    if (gl_fd[i].status==4)
				    	 	  	 	    {
						    	 	  	 	  sock=tcp_connet_server(gl_fd[i].ip,gl_fd[i].port,0);
				    	 	  	 	  	  if ( sock < 0)
				    	 	  	 	  	  {
						    	 	  	 	 
				                       dcs_log(0,0,"不能创建与svr_ip%d的连接[%s]%s:%d",gl_fd[i].ndx,strerror(errno),gl_fd[i].ip,gl_fd[i].port);
			//	           	 	  	  	 t_flag =0;
				    	 	  	 	  	  }
				    	 	  	 	  	  else
				    	 	  	 	  	  {
				    	 	  	 	  	  	  dcs_debug(0,0,"connect ip=%s,port=%d sock=%d ",gl_fd[i].ip,gl_fd[i].port,sock);
				    	 	  	 	  	  	  add_to_set(sock, &write_set, &max_fd);
				    	 	  	 	  	  	  add_to_set(sock, &error_set, &max_fd);
												        gl_fd[sock].sockfd=sock;
												        gl_fd[sock].type = 1; //服务端工作链路
												        gl_fd[sock].timeout =g_para[gl_fd[i].ndx].timeout;
												        gl_fd[sock].used=0x30;
												        gl_fd[sock].ndx=gl_fd[i].ndx;
		//										        dcs_debug(0,0," connect ndx=%d",gl_fd[sock].ndx);
												        gl_fd[sock].data_len=gl_fd[i].msg_len+gl_fd[i].len_len;
												        gl_fd[sock].len_len=gl_fd[i].len_len;
												        gl_fd[sock].len_type=gl_fd[i].len_type;
												        gl_fd[sock].msg_len=gl_fd[i].msg_len;
												        strncpy(gl_fd[sock].ip,gl_fd[i].ip,sizeof(gl_fd[sock].ip)-1);
												        gl_fd[sock].port=gl_fd[i].port;
												        memcpy(gl_fd[sock].buf,gl_fd[i].buf,gl_fd[i].msg_len+gl_fd[i].len_len);
//												        if( gl_fd[i].len_type == 2 || gl_fd[i].len_type == 3)
//												        	 memcpy(gl_fd[sock].remark,buffer+4,14);
											          dcs_debug(0,0," new client connected!\n ");
			
				    	 	  	 	  	  }
				    	 	  	 	  	  if ( gl_fd[i].data_len >gl_fd[i].msg_len+gl_fd[i].len_len)
		    	 	  	 	  		 	  {
		    	 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf+gl_fd[i].len_len+gl_fd[i].msg_len,gl_fd[i].data_len-gl_fd[i].len_len-gl_fd[i].msg_len);
		    	 	  	 	  		 	  	 gl_fd[i].data_len= gl_fd[i].data_len- gl_fd[i].len_len-gl_fd[i].msg_len;
		    	 	  	 	  		 	  	 memcpy(gl_fd[i].buf,buffer,gl_fd[i].data_len);
		    	 	  	 	  		 	  	 gl_fd[i].msg_len=0;
		    	 	  	 	  		 	  	 gl_fd[i].status=1;
		    	 	  	 	  		 	  	 
		    	 	  	 	  		 	  }
		    	 	  	 	  		 	  else
		    	 	  	 	  		 	  {
		    	 	  	 	  		 	  	 gl_fd[i].status=1;
		    	 	  	 	  		 	  	 gl_fd[i].data_len=0;
		    	 	  	 	  		 	  	 gl_fd[i].msg_len=0;
		    	 	  	 	  		 	  	 t_flag =0;
		    	 	  	 	  		 	  }
		    	 	  	 	  		 	}
	                    }
	                    dcs_debug(0,0,"exit client recv process");
	    	 	  	 	  }
	               
	    	 	  	 }
	    	 	  	 else if (gl_fd[i].type == 0 )   //长连接客户端侦听端口
	    	 	  	 {
	    	 	  	 	 //新长连接
	                dcs_log(0,0,"<accept new begin>from loc_port%d=[%d]!\n",gl_fd[i].ndx,g_para[gl_fd[i].ndx].loc_port);
	    	 	  	 	  long client_addr,client_port;
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
	    	 	  	 	  	add_to_set(sock, &error_set, &max_fd);
						        gl_fd[sock].sockfd=sock;
						        gl_fd[sock].type=2;
						        gl_fd[sock].len_type = gl_fd[i].len_type;
						        gl_fd[sock].len_len=gl_fd[i].len_len;
			        	   	gl_fd[sock].ndx= gl_fd[i].ndx;
			        	   	if( gl_fd[gl_sock[gl_fd[i].ndx]].used )
			        	   	{
			        	   		close(gl_sock[gl_fd[i].ndx]);
			        	   		FD_CLR(gl_sock[gl_fd[i].ndx],&read_set);
		    	 	  	 	    FD_CLR(gl_sock[gl_fd[i].ndx],&error_set);
		    	 	  	 	    gl_fd[gl_sock[gl_fd[i].ndx]].used=0x00;
			        	   	}
			        	   	gl_sock[gl_fd[sock].ndx]=sock;
						       	gl_fd[sock].status= 1;
						        gl_fd[sock].timeout =gl_fd[i].timeout;
						        gl_fd[sock].used=0x30;
						        gl_fd[sock].data_len=0;
						        gl_fd[sock].msg_len=0;
						        strncpy(gl_fd[sock].ip,gl_fd[i].ip,sizeof(gl_fd[sock].ip)-1);
        	          gl_fd[sock].port= gl_fd[i].port;						        
					          dcs_debug(0,0," new client connected! sock=%d\n",sock);
	
					        }
	    	 	  	 }
	    	 	  	 else
	    	 	  	 {
	    	 	  	 	  //短连接服务端收到的报文
//	    	 	  	 	  memset(buffer,0,sizeof(buffer));
	    	 	  	 	  dcs_debug(0,0,"server recving data sock=[%d],i=%d,data_len=%d ... \n",gl_fd[i].sockfd,i,gl_fd[i].data_len);
	    	 	  	 	  len = recv(gl_fd[i].sockfd,gl_fd[i].buf+gl_fd[i].data_len,sizeof(gl_fd[i].buf)-gl_fd[i].data_len,0);
	    	 	  	 	  if ( len <=0 )
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop1;
	    	 	  	 	  	dcs_log(0,0,"recv server data error![%s]",strerror(errno));
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
		    	 	  	 	  FD_CLR(gl_fd[i].sockfd,&error_set);
		    	 	  	 	  close(gl_fd[i].sockfd);
		    	 	  	 	  gl_fd[i].used=0x00;
//		    	 	  	 	  gl_sock[gl_fd[i].ndx]=-1;
                    goto cri_loop1;
	    	 	  	 	  }
	    	 	  	 	  dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"server recv data len=%d len_type=%d\n",len,gl_fd[i].len_type);
	    	 	  	 	  gl_fd[i].data_len= gl_fd[i].data_len+len;
	    	 	  	 	  if ( gl_fd[i].len_type==1 && gl_fd[i].data_len >2)
 	  	 	  		 	  {
 	  	 	  		 	  
 	  	 	  		 	  	gl_fd[i].msg_len= (unsigned char)gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1];
 	  	 	  		 	  	if ( gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
		    	 	  	 	  
 	  	 	  		 	  }
 	  	 	  		 	  else  if ( gl_fd[i].len_type==0 && gl_fd[i].data_len >4)
 	  	 	  		 	  {
 	  	 	  		 	  	   memcpy(buffer,gl_fd[i].buf,4);
 	  	 	  		 	  	   buffer[4]=0x00;
 	  	 	  		 	  	   gl_fd[i].msg_len= atoi(buffer);
 	  	 	  		 	  	   if ( gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
				    	 	  	 	  
 	  	 	  		 	  }
 	  	 	  		 	  else  if ( gl_fd[i].len_type==4 && gl_fd[i].data_len >8)
 	  	 	  		 	  {
 	  	 	  		 	  	   memcpy(buffer,gl_fd[i].buf,8);
 	  	 	  		 	  	   buffer[8]=0x00;
 	  	 	  		 	  	   gl_fd[i].msg_len= atoi(buffer);
 	  	 	  		 	  	   if ( gl_fd[i].msg_len >0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
				    	 	  	 	  
 	  	 	  		 	  }
 	  	 	  		 	  else if ( gl_fd[i].len_type==2 && gl_fd[i].data_len >8)
 	  	 	  		 	  {
 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf,8);
 	  	 	  		 	  	 buffer[8]=0x00;
 	  	 	  		 	  	 gl_fd[i].msg_len= atoi(buffer)+12;
 	  	 	  		 	  	 if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
 	  	 	  		 	  }
 	  	 	  		 	  else if ( gl_fd[i].len_type==3 && gl_fd[i].data_len >8)
 	  	 	  		 	  {
 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf,8);
 	  	 	  		 	  	 buffer[8]=0x00;
 	  	 	  		 	  	 gl_fd[i].msg_len= atoi(buffer)+14;
 	  	 	  		 	  	 if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
 	  	 	  		 	  }
 	  	 	  		 	  else
 	  	 	  		 	  {
 	  	 	  		 	  	 if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
 	  	 	  		 	  }
//	    	 	  	 	  sprintf(buffer,"%04d",ret+14-8);
//	    	 	  	 	  memcpy(buffer+4,gl_fd[i].remark,14);
	    	 	  	 	  if ( gl_fd[i].status ==  4)
	    	 	  	 	  {
//		    	 	  	 	  dcs_debug(0,0,"begin write data to client ndx=%d,sock=%d",gl_fd[i].ndx,gl_sock[gl_fd[i].ndx]);
		    	 	  	 	  if( gl_fd[i].msg_len>gl_fd[i].data_len)
		    	 	  	 	  {
		    	 	  	 	  		FD_CLR(gl_fd[i].sockfd,&read_set);
				    	 	  	 	  FD_CLR(gl_fd[i].sockfd,&error_set);
				    	 	  	 	  close(gl_fd[i].sockfd);
				    	 	  	 	  gl_fd[i].used=0x00;
				    	 	  	 	  dcs_log(gl_fd[i].buf,gl_fd[i].data_len,"recv server(short) error data message!");
                        goto cri_loop1;
		    	 	  	 	  }
		    	 	  	 	  if ( !gl_fd[gl_sock[gl_fd[i].ndx]].status ) len =-1;
		    	 	  	 	  else 
		    	 	  	 	  {
		    	 	  	 	  	 if( gl_fd[i].len_type==2 || gl_fd[i].len_type== 3)
		    	 	  	 	  	 {
		    	 	  	 	  	 	  sprintf(buffer,"%04d",gl_fd[i].msg_len+14);
		    	 	  	 	  	 	  memcpy(buffer+4,gl_fd[i].remark,14);
		    	 	  	 	  	 	  memcpy(buffer+18,gl_fd[i].buf+8,gl_fd[i].msg_len);
		    	 	  	 	  	 	  len = write(gl_sock[gl_fd[i].ndx],buffer,gl_fd[i].msg_len+18);
		    	 	  	 	  	 }
		    	 	  	 	  	 else if( gl_fd[i].len_type==4 )
		    	 	  	 	  	 {
		    	 	  	 	  	 	  sprintf(buffer,"%04d",gl_fd[i].msg_len);
		    	 	  	 	  	 	  memcpy(buffer+4,gl_fd[i].buf+8,gl_fd[i].msg_len);
		    	 	  	 	  	 	  len = write(gl_sock[gl_fd[i].ndx],buffer,gl_fd[i].msg_len+4);
		    	 	  	 	  	 }
		    	 	  	 	  	 else
		    	 	  	 	  	 	  len = write(gl_sock[gl_fd[i].ndx],gl_fd[i].buf,gl_fd[i].data_len);
		    	 	  	 	  }

		    	 	  	 	  if ( len <=0 ) 	  	 	     	 	  	 	  	
		     	 	  	 	  {
//		     	 	  	 	  		  close(gl_sock[gl_fd[i].ndx]);
		     	 	  	 	  		  close(gl_fd[i].sockfd);	
		     	 	  	 	  		  gl_fd[i].used=0x00;
				    	 	  	 	    gl_fd[i].data_len=0;
				    	 	  	 	    gl_fd[i].status=0;     	 	  	 	  		  
				    	 	  	 	  	gl_fd[gl_sock[gl_fd[i].ndx]].used=0x00;
				    	 	  	 	  	FD_CLR(gl_sock[gl_fd[i].ndx],&read_set);
				    	 	  	 	  	FD_CLR(gl_sock[gl_fd[i].ndx],&error_set);
//				    	 	  	 	  	gl_sock[gl_fd[i].ndx]=-1;
		     	 	  	 	  		  dcs_log(0,0," write client data error![%s]",strerror(errno));
//		     	 	  	 	  		  if (connect_sock(i,&max_fd)<0)
//							       	      _srv_exit(0);
		     	 	  	 	  }
		    	 	  	 	  else
		    	 	  	 	  {
		    	 	  	 	  		FD_CLR(gl_fd[i].sockfd,&read_set);
				    	 	  	 	  FD_CLR(gl_fd[i].sockfd,&error_set);
				    	 	  	 	  close(gl_fd[i].sockfd);
				    	 	  	 	  gl_fd[i].used=0x00;
				    	 	  	 	  gl_fd[i].data_len=0;
				    	 	  	 	  gl_fd[i].status=1;
				    	 	  	 	  if( gl_fd[i].len_type==2 || gl_fd[i].len_type== 3 ||  gl_fd[i].len_type== 4)
				    	 	  	 	  	dcs_debug(buffer,len ,"\n len_type =2,3,4 send client data len=%d\n",len);
				    	 	  	 	  else
		    	 	  	 	  	  	dcs_debug(gl_fd[i].buf,len ,"\nsend client data len=%d\n",len);
		    	 	  	 	  	  	
		    	 	  	 	  }
		    	 	  	 	}
		    	 	  }
	    	 	  }
	    	 	  else if(FD_ISSET(i, &verror_set))
	    	 	  {
	    	 	  	      ret--;
	    	 	  	      dcs_log(0,0,"socket error [%d][%s]",i,strerror(errno));
	    	 	  	      close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&write_set);
	    	 	  	 	    FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  }	
	    	 	  
cri_loop1:	    	 	
    	 	;
        }while(ret >0 && (i++) <=max_fd);
      }
    }

 
  for ( i=0; i <= max_fd;i++)
  {
  	 if ( gl_fd[i].used )
  	 {
  	 	 close(gl_fd[i].sockfd);
  	 	 gl_fd[i].used=0;
  	 }
  }
}