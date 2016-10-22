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
#include "p_sw.h"
extern gs_run_mode;
extern fd_set read_set,write_set,error_set;
extern SOCK_REC gl_fd[4];
extern int stop;
extern SYS_ARA g_para;
extern key_rec  g_keys;
int gl_sock=-1;
void do_loop()
{
	
  fd_set vread_set,vwrite_set,verror_set;
  struct timeval tv;
  time_t t;
  int max_fd ,i ,sock,loc_flg=0,ret,error,len;
  int s,flags,l,vmax_fd=0;
  unsigned char buffer[8192];
 
  loop_init:
  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&error_set);
  stop =0;
  max_fd=0;
    //创建数据请求端侦听端口
    
	  sock=tcp_open_server(NULL, g_para.old_loc_port);
	  if( sock >0 )
    {
    	add_to_set(sock, &read_set, &max_fd);
      gl_fd[0].type=0;
      gl_fd[0].used=1;
      gl_fd[0].data_len=0;
      gl_fd[0].msg_len=0;
      gl_fd[0].sockfd=sock;
      gl_fd[0].status=1;
      dcs_log(0,0,"open local port succ ![%d][%d]\n",g_para.old_loc_port,sock);
    }
    else
    {
    	 dcs_log(0,0,"open local port fail ![%d][%s]",g_para.old_loc_port,strerror(errno));
    	 _srv_exit(0);
    		
    }
    dcs_debug(0,0,"listen old port=%d ",g_para.old_loc_port);
     sock=tcp_open_server(NULL, g_para.new_loc_port);
	  if( sock >0 )
    {
    	add_to_set(sock, &read_set, &max_fd);
      gl_fd[1].type=0;
      gl_fd[1].used=1;
      gl_fd[1].data_len=0;
      gl_fd[1].msg_len=0;
      gl_fd[1].sockfd=sock;
      gl_fd[1].status=1;
    }
    else
    {
    	 dcs_log(0,0,"open local new port fail ![%d][%s]",g_para.new_loc_port,strerror(errno));
    	 _srv_exit(0);
    		
    }
    dcs_debug(0,0,"listen new port=%d ",g_para.new_loc_port);
    sock=tcp_connet_server(g_para.ip,g_para.port,0);
	  if ( sock < 0)
	  {
       dcs_log(0,0,"p的连接[%s]%s:%d",strerror(errno),g_para.ip,g_para.port);
	  }
	  else
	  {
	  	  dcs_debug(0,0,"connect ip=%s,port=%d sock=%d ",g_para.ip,g_para.port,sock);
	  	  add_to_set(sock, &write_set, &max_fd);
        gl_fd[2].type = 2; //服务端工作链路
        gl_fd[2].used=1;
        gl_fd[2].sockfd=sock;
        gl_fd[2].status=0;
	  }
	  dcs_debug(0,0,"connect ip=%s,port=%d ",g_para.ip,g_para.port);
	  
	  dcs_debug(0,0,"begin entry select while!");
    while(1)
    { 
      if ( stop ) break;
    	tv.tv_sec = 1;
      tv.tv_usec = 0;
      vread_set=read_set;
      vwrite_set=write_set;
      verror_set=error_set;

    	ret=select(max_fd + 1, &vread_set, &vwrite_set, NULL, &tv);

      if ( ret < 0) 
    	{
	    	 dcs_log(0,0,"select error![%s],max_fd=%d",strerror(errno),max_fd);
	       sleep(1);
      }
      else if ( ret == 0)
      {
           if( !gl_fd[2].used )
           {
           	  sock=tcp_connet_server(g_para.ip,g_para.port,0);
						  if ( sock < 0)
						  {
					       dcs_log(0,0,"ip的连接[%s]%s:%d",strerror(errno),g_para.ip,g_para.port);
						  }
						  else
						  {
						  	  dcs_debug(0,0,"connect ip=%s,port=%d sock=%d ",g_para.ip,g_para.port,sock);
						  	  add_to_set(sock, &write_set, &max_fd);
					        gl_fd[2].type = 2; //服务端工作链路
					        gl_fd[2].used=1;
					        gl_fd[2].sockfd=sock;
					        gl_fd[2].status=0;
						  }
           }
           t=time(NULL);
           if( gl_fd[2].used && gl_fd[2].status == 1)
           {
           		if( t- gl_fd[2].t >60)
           		{		
           			write(gl_fd[2].sockfd,"0000",4);
//           			dcs_debug(0,0,"send server echo test");
           			gl_fd[2].t=t;
           	  }
           }
           if( gl_fd[3].used && gl_fd[3].status == 1)
           {
           		if( t- gl_fd[3].t >50)
           		{		
           			write(gl_fd[3].sockfd,"0000",4);
//           			dcs_debug(0,0,"send client echo test");
           			gl_fd[3].t=t;
           	  }
           }
           if( gl_fd[4].used && gl_fd[4].status == 1)
           {
           		if( t- gl_fd[4].t >50)
           		{		
           			write(gl_fd[4].sockfd,"0000",4);
//           			dcs_debug(0,0,"send client echo test");
           			gl_fd[4].t=t;
           	  }
           }
           for ( i=0; i<g_keys.cnt;i++)
           {
           	  if( (t -100) >g_keys.t[i])
           	  {
           	  	dcs_debug(0,0,"delete timout key=[%s]",g_keys.keys[i]);
           	  	if( i != (g_keys.cnt-1))
           	  	{	
           	  		memcpy(g_keys.keys[i],g_keys.keys[g_keys.cnt-1],30);
           	  		g_keys.socks[i]=g_keys.socks[g_keys.cnt-1];
           	  		g_keys.t[i]=g_keys.t[g_keys.cnt-1];
           	  		i--;
           	  	}
           	  	g_keys.cnt--;
           	  } 
           }
      } 
      else 
      {
        i=0;
	        dcs_debug(0,0,"select ret=%d",ret);
        do{
	    	  if ( gl_fd[i].used )
	    	  {
	    	  	if(FD_ISSET(gl_fd[i].sockfd, &vwrite_set))/* 对主动建链的链路判断链接是否成功   */
	    	  	{
	    	  		  ret--;
	    	  		  len=sizeof(error);
	  	 	  	 	  getsockopt(gl_fd[i].sockfd,SOL_SOCKET,SO_ERROR,&error,&len) ;               
                FD_CLR(gl_fd[i].sockfd,&write_set);
                if( 0 != error )
                { 
                   close(gl_fd[i].sockfd);
	 	  	 	  	     gl_fd[i].used=0x00;
	 	  	 	  	     gl_fd[i].status=0;	  
	 	  	 	  	     gl_fd[i].sockfd=0;
	 	  	 	  	     if( i==	2 )	  	 	         
	 	  	 	         		dcs_log(0,0,"  connect server socket error!addr=[%s]:[%d][%s]",g_para.ip,g_para.port,strerror(errno));	
	 	  	 	         
                }
                else
                {
                	  add_to_set(gl_fd[i].sockfd,&read_set,&max_fd);
                	  gl_fd[i].status=1;
                	  gl_fd[i].data_len=0;
                    gl_fd[i].msg_len=0;
                    gl_fd[i].t=time(NULL);
	
                }
	              
	    	  	}
	    	  	else if(FD_ISSET(gl_fd[i].sockfd, &vread_set))
	    	 	  {
	    	 	  	 ret--;
	    	 	  	 dcs_debug(0,0,"sockfd=[%d],type=[%d]!\n",gl_fd[i].sockfd,gl_fd[i].type);
	    	 	  	 if (gl_fd[i].type ==2 ) //从应用服务器收到的交易
	    	 	  	 { 
	    	 	  	 	  memset(buffer,0,sizeof(buffer));
	    	 	  	 	  len=recv(gl_fd[i].sockfd,gl_fd[i].buf+gl_fd[i].data_len,sizeof(gl_fd[i].buf)-gl_fd[i].data_len,0);
	    	 	  	 	  if ( len <= 0)
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop;
	    	 	  	 	  	close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	gl_fd[i].status=0;
	    	 	  	 	    FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	    if( gl_fd[i].sockfd == max_fd ) max_fd--;
	    	 	  	 	    gl_fd[i].sockfd=0;
	    	 	  	 	  	dcs_log(0,0,"appsrv recv data error!sockfd=[%d][%s]",gl_fd[i].sockfd,strerror(errno));	  
	    	 	  	 	  }
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	  int t_flag=1;
	    	 	  	 	  	  dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"recv appsrv data len=%d\n",len);
	    	 	  	 	  	  gl_fd[i].data_len=gl_fd[i].data_len+len;
	    	 	  	 	  	  while(t_flag)
	    	 	  	 	  	  {
//			    	 	  	 	 	    dcs_debug(0,0," server recv  t_flag");
			    	 	  	 	 	    if ( gl_fd[i].data_len >=4)
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	    char tmp[5];
	    	 	  	 	  		 	    memcpy(tmp,gl_fd[i].buf,4);
	    	 	  	 	  		 	    tmp[4]=0x00;
	    	 	  	 	  		 	  	gl_fd[i].msg_len= atoi(tmp);
	    	 	  	 	  		 	  	if( gl_fd[i].msg_len > 2048 )
	    	 	  	 	  		 	  	{
	    	 	  	 	  		 	  		 dcs_log(gl_fd[i].buf,gl_fd[i].data_len,"error data");
	    	 	  	 	  		 	  		 gl_fd[i].msg_len=0;
	    	 	  	 	  		 	  		 gl_fd[i].data_len=0;
	    	 	  	 	  		 	  		 gl_fd[i].status=1;
	    	 	  	 	  		 	  		 break;
	    	 	  	 	  		 	  	}
	    	 	  	 	  		 	  	if( gl_fd[i].msg_len == 0 )
	    	 	  	 	  		 	  	{
	    	 	  	 	  		 	  		dcs_log(gl_fd[i].buf,gl_fd[i].data_len,"echo test");
	    	 	  	 	  		 	  		 gl_fd[i].msg_len=0;
	    	 	  	 	  		 	  		 gl_fd[i].data_len=0;
	    	 	  	 	  		 	  		 gl_fd[i].status=1;
	    	 	  	 	  		 	  		 break;
	    	 	  	 	  		 	  	}
	    	 	  	 	  		 	  	if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
						    	 	  	 	  
	    	 	  	 	  		 	  }
				    	 	  	 	    if (gl_fd[i].status==4)
				    	 	  	 	    {
	                            // 发送数据
//	                            dcs_debug(0,0,"get_sockfd data msg_len=%d",msg_len);
	                          s=get_sockfd(gl_fd[i].buf+4,gl_fd[i].msg_len);
	                          if( s <0 || s >5) 
	                          {
	                          	dcs_log(0,0,"get_sockfd error s=[%d]",s);
	                          	if ( gl_fd[i].data_len >gl_fd[i].msg_len+4)
		    	 	  	 	  		 	    {
			    	 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf+4+gl_fd[i].msg_len,gl_fd[i].data_len-4-gl_fd[i].msg_len);
			    	 	  	 	  		 	  	 gl_fd[i].data_len= gl_fd[i].data_len- 4-gl_fd[i].msg_len;
			    	 	  	 	  		 	  	 memcpy(gl_fd[i].buf,buffer,gl_fd[i].data_len);
			    	 	  	 	  		 	  	 gl_fd[i].msg_len=0;
			    	 	  	 	  		 	  	 gl_fd[i].status=1;
		    	 	  	 	  		 	  	 
		    	 	  	 	  		 	    }
		    	 	  	 	  		 	    else
		    	 	  	 	  		 	    {
		    	 	  	 	  		 	    	 dcs_log(gl_fd[i].buf,gl_fd[i].data_len,"clean invalid data");
		    	 	  	 	  		 	  		 gl_fd[i].msg_len=0;
		    	 	  	 	  		 	  		 gl_fd[i].data_len=0;
		    	 	  	 	  		 	  		 gl_fd[i].status=1;
		    	 	  	 	  		 	    }
	                          	break;
	                          }  
	                          if( gl_fd[s].used && gl_fd[s].status )
	                          {
	                          	 len=write(gl_fd[s].sockfd,gl_fd[i].buf,gl_fd[i].msg_len+4);
	                          	 if( len <=0 )
	                          	 {
	                          	 	  gl_fd[s].used=0;
	                          	 	  gl_fd[s].status=0;
	                          	 	  dcs_log(0,0,"network abnormal ,send data fail[%s]",strerror(errno));
	                          	 	  FD_CLR(gl_fd[s].sockfd,&read_set);
	                          	 	  close(gl_fd[s].sockfd);
	                          	 	  gl_fd[s].sockfd=0;
	                          	 	  
	                          	 	  if( gl_fd[s].sockfd == max_fd ) max_fd--;
	                          	 }
	                           	 gl_fd[s].t=time(NULL);
	                           	 dcs_debug(gl_fd[i].buf,len,"send client data len=%d",len);
	                          }
	                          else
	                          {
	                          	 dcs_log(gl_fd[i].buf,gl_fd[i].msg_len+4," network not ready, data discard!");
	                          }
				    	 	  	 	  	  if ( gl_fd[i].data_len >gl_fd[i].msg_len+4)
		    	 	  	 	  		 	  {
		    	 	  	 	  		 	  	 dcs_debug(0,0,"gl_fd[i].data_len >gl_fd[i].msg_len+4");
		    	 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf+4+gl_fd[i].msg_len,gl_fd[i].data_len-4-gl_fd[i].msg_len);
		    	 	  	 	  		 	  	 gl_fd[i].data_len= gl_fd[i].data_len- 4-gl_fd[i].msg_len;
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
		    	 	  	 	  		 	else break;
	                    }
	                    dcs_debug(0,0,"exit client recv process");
	    	 	  	 	  }
	               
	    	 	  	 }
	    	 	  	 else if (gl_fd[i].type == 0 )   //侦听到新的连接
	    	 	  	 {
	    	 	  	 	 //新长连接
	    	 	  	 	  
	    	 	  	 	  int client_addr,client_port;
	    	 	  	 	  if( i==0)
	                	dcs_log(0,0,"<accept old begin>from loc_port=[%d]!\n",g_para.old_loc_port);
	                else dcs_log(0,0,"<accept new begin>from loc_port=[%d]!\n",g_para.new_loc_port);
	    	 	  	 	  sock=tcp_accept_client(gl_fd[i].sockfd,&client_addr, &client_port);
	    	 	  	 	 	if ( sock <0 )
	    	 	  	 	 	{    
	    	 	  	 	 		 dcs_log(0,0,"accpet client new socket fail ![%s]",strerror(errno));

	    	 	  	 	  }
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	struct in_addr in;
	    	 	  	 	  	if( i==0)
	    	 	  	 	  	{	
		                  if( gl_fd[3].used && gl_fd[3].sockfd > 0) 
		                  {
		                  	FD_CLR(gl_fd[3].sockfd,&read_set);
		                  	if( gl_fd[3].sockfd == max_fd ) max_fd--;
		                  	close(gl_fd[3].sockfd);
		                  }
		    	 	  	 	  	add_to_set(sock, &read_set, &max_fd);
							        gl_fd[3].sockfd=sock;
							        gl_fd[3].type=1;
							        gl_fd[3].used= 1;
				        	   	gl_fd[3].status= 1;
				        	   	gl_fd[3].data_len=0;
	                    gl_fd[3].msg_len=0;
	                    gl_fd[3].t=time(NULL);
                    }
                    else
                    {
                    	 if( gl_fd[4].used && gl_fd[4].sockfd > 0)
                    	 {
                    	 		FD_CLR(gl_fd[4].sockfd,&read_set);
                    	 		if( gl_fd[4].sockfd == max_fd ) max_fd--;
                    	 		close(gl_fd[4].sockfd);
                    	 }
		    	 	  	 	  	add_to_set(sock, &read_set, &max_fd);
							        gl_fd[4].sockfd=sock;
							        gl_fd[4].type=1;
							        gl_fd[4].used= 1;
				        	   	gl_fd[4].status= 1;
				        	   	gl_fd[4].data_len=0;
	                    gl_fd[4].msg_len=0;
	                    gl_fd[4].t=time(NULL);
                    }
			        	   	in.s_addr = client_addr;
						        dcs_debug(0,0," new client link connected! ip=[%s],port=%d,sock=%d\n",inet_ntoa(in),sock,client_port);
	
					        }
	    	 	  	 }
	    	 	  	 else //从client收到的交易
	    	 	  	 {
	                int t_flag=1;
	    	 	  	 	  dcs_debug(0,0,"recving from client data sock=[%d],i=%d,data_len=%d ... \n",gl_fd[i].sockfd,i,gl_fd[i].data_len);
	    	 	  	 	  len = recv(gl_fd[i].sockfd,gl_fd[i].buf+gl_fd[i].data_len,sizeof(gl_fd[i].buf)-gl_fd[i].data_len,0);
	    	 	  	 	  if ( len <=0 )
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop;
	    	 	  	 	  	dcs_log(0,0,"recv server data error![%s]",strerror(errno));
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	if( gl_fd[i].sockfd == max_fd ) max_fd--;
		    	 	  	 	  close(gl_fd[i].sockfd);
		    	 	  	 	  gl_fd[i].sockfd=0;
		    	 	  	 	  gl_fd[i].used=0x00;
		    	 	  	 	  gl_fd[i].status=0x00;
                    goto cri_loop;
	    	 	  	 		}
	    	 	  	 		dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"recv data len=%d \n",len);
	    	 	  	 		gl_fd[i].data_len= gl_fd[i].data_len+len;
		    	 	  	 	while(t_flag)
		    	 	  	 	{
		    	 	  	 	  if (gl_fd[i].data_len >4)
	 	  	 	  		 	  {
	 	  	 	  		 	  	char tmp[5];
	 	  	 	  		 	  	memcpy(tmp,gl_fd[i].buf,4);
	 	  	 	  		 	  	tmp[4]=0x00;
	 	  	 	  		 	  	gl_fd[i].msg_len= atoi(tmp);
	 	  	 	  		 	  	if( gl_fd[i].msg_len > 2048 )
	 	  	 	  		 	  	{
	 	  	 	  		 	  		 dcs_log(gl_fd[i].buf,gl_fd[i].data_len,"error data");
	 	  	 	  		 	  		 gl_fd[i].msg_len=0;
	 	  	 	  		 	  		 gl_fd[i].data_len=0;
	 	  	 	  		 	  		 gl_fd[i].status=1;
	 	  	 	  		 	  	}
	 	  	 	  		 	  	if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
	 	  	 	  		 	  }
		    	 	  	 	  if ( gl_fd[i].status ==  4 && gl_fd[2].status==1)
		    	 	  	 	  {
		 	 	  	 	  	 	  if( 0>=set_sockfd(gl_fd[i].buf+4 ,gl_fd[i].msg_len,(unsigned char )i))
		 	 	  	 	  	 	  {	
		 	 	  	 	  	 	  	dcs_log(0,0,"set_sockfd fail!");
		 	 	  	 	  	 	  	if ( gl_fd[i].data_len >gl_fd[i].msg_len+4)
	 	  	 	  		 	  		{
			 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf+4+gl_fd[i].msg_len,gl_fd[i].data_len-4-gl_fd[i].msg_len);
			 	  	 	  		 	  	 gl_fd[i].data_len= gl_fd[i].data_len- 4-gl_fd[i].msg_len;
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
		 	 	  	 	  	 	  	break;
		 	 	  	 	  	 	  } 
		 	 	  	 	  	 	  len = write(gl_fd[2].sockfd,gl_fd[i].buf,gl_fd[i].msg_len+4);
			    	 	  	 	  if ( len <=0 ) 	  	 	     	 	  	 	  	
			     	 	  	 	  {
			     	 	  	 	  		  close(gl_fd[i].sockfd);	
			     	 	  	 	  		  gl_fd[2].used=0x00;
					    	 	  	 	    gl_fd[2].data_len=0;
					    	 	  	 	    gl_fd[2].status=0;     	 	  	 	  		  
					    	 	  	 	  	FD_CLR(gl_fd[2].sockfd,&read_set);
					    	 	  	 	  	if( gl_fd[2].sockfd == max_fd ) max_fd--;
					    	 	  	 	  	gl_fd[2].sockfd=0;
	                          if( len == 0)
	                          	dcs_log(0,0," write server not ready!");
	                          else
			     	 	  	 	  		  	dcs_log(0,0," write server data error![%s]",strerror(errno));
			     	 	  	 	  }
			    	 	  	 	  else
			    	 	  	 	  {
					    	 	  	 	 	gl_fd[2].t=time(NULL);
					    	 	  	 	 	dcs_debug(gl_fd[i].buf,len ,"\nsend server data len=%d\n",len);  	
			    	 	  	 	  }
			    	 	  	 	}
			    	 	  	 	else if ( gl_fd[i].status ==  4 )
			    	 	  	 		dcs_log(gl_fd[i].buf,gl_fd[i].msg_len+4,"server not ready,discard!data len[%d]",gl_fd[i].msg_len+4);
			    	 	  	 	if ( gl_fd[i].data_len >gl_fd[i].msg_len+4)
	 	  	 	  		 	  {
	 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf+4+gl_fd[i].msg_len,gl_fd[i].data_len-4-gl_fd[i].msg_len);
	 	  	 	  		 	  	 gl_fd[i].data_len= gl_fd[i].data_len- 4-gl_fd[i].msg_len;
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
	    	 	  }
	    	 	}
	    	 	else
	    	 	  dcs_log(0,0,"fd[%d] not used",i);
cri_loop:	    	 	
    	 	;
        }while(ret >0 && (i++) <4);
      }
    }

 
  for ( i=0; i < 4;i++)
  {
  	 if ( gl_fd[i].used )
  	 {
  	 	 close(gl_fd[i].sockfd);
  	 	 gl_fd[i].used=0;
  	 }
  }
}
