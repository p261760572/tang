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
extern fd_set read_set,write_set,error_set;
extern SOCK_REC gl_fd[4];
extern int stop;
extern SYS_ARA g_para;
int gl_sock=-1;
void do_loop()
{
	
  fd_set vread_set,vwrite_set,verror_set;
  struct timeval tv;
  
  int max_fd ,i ,sock,loc_flg=0,ret,error,len;
  int s,flags,l,vmax_fd=0;
  unsigned char buffer[8192],tmp[256];
 
  loop_init:
  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&error_set);
  stop =0;
  max_fd=0;
    //创建数据请求端侦听端口
    
	  sock=tcp_open_server(NULL, g_para.loc_port);
	  if( sock >0 )
    {
    	add_to_set(sock, &read_set, &max_fd);
    	add_to_set(sock, &error_set, &max_fd);
      gl_fd[0].type=0;
      gl_fd[0].used=1;
      gl_fd[0].data_len=0;
      gl_fd[0].msg_len=0;
      gl_fd[0].sockfd=sock;
      gl_fd[0].status=1;
      dcs_log(0,0,"open local port succ ![%d][%d]\n",g_para.loc_port,sock);
    }
    else
    {
    	 dcs_log(0,0,"open local port fail ![%d][%s]",g_para.loc_port,strerror(errno));
    	 _srv_exit(0);
    		
    }
    dcs_debug(0,0,"connect ip=%s,port=%d ",g_para.old_ip,g_para.old_port);
    sock=tcp_connet_server(g_para.old_ip,g_para.old_port,0);
	  if ( sock < 0)
	  {
       dcs_log(0,0,"old_ip的连接[%s]%s:%d",strerror(errno),g_para.old_ip,g_para.old_port);
	  }
	  else
	  {
	  	  dcs_debug(0,0,"connect ip=%s,port=%d sock=%d ",g_para.old_ip,g_para.old_port,sock);
	  	  add_to_set(sock, &write_set, &max_fd);
	  	  add_to_set(sock, &error_set, &max_fd);
        gl_fd[2].type = 2; //服务端工作链路
        gl_fd[2].used=1;
        gl_fd[2].sockfd=sock;
        gl_fd[0].status=0;
	  }
	  dcs_debug(0,0,"connect ip=%s,port=%d ",g_para.new_ip,g_para.new_port);
	  sock=tcp_connet_server(g_para.new_ip,g_para.new_port,0);
	  if ( sock < 0)
	  {
       dcs_log(0,0,"new_ip的连接[%s]%s:%d",strerror(errno),g_para.new_ip,g_para.new_port);
	  }
	  else
	  {
	  	  dcs_debug(0,0,"connect ip=%s,port=%d sock=%d ",g_para.new_ip,g_para.new_port,sock);
	  	  add_to_set(sock, &write_set, &max_fd);
	  	  add_to_set(sock, &error_set, &max_fd);
	  	  gl_fd[3].sockfd=sock;
        gl_fd[3].type = 2; //服务端工作链路
        gl_fd[3].used=1;
        gl_fd[0].status=0;
	  }
	  dcs_debug(0,0,"begin entry select while!");
    while(1)
    { 
       
       if ( stop ) break;
    
    	tv.tv_sec = 1;
      tv.tv_usec = 0;
      vread_set=read_set;
      vwrite_set=write_set;
      verror_set=error_set;
//      vmax_fd=0;
//      if ( vmax_fd <max_fd) max_fd=vmax_fd;
    	ret=select(max_fd + 1, &vread_set, &vwrite_set, &verror_set, &tv);
//    	fprintf(stderr," recv select event ret=%d !\n",ret);
//      dcs_debug(0,0," recv select event ret=%d !",ret);
      if ( ret < 0) 
    	{
	    	 dcs_log(0,0,"select error![%s],max_fd=%d",strerror(errno),max_fd);
	    	 if( errno != EINTR && errno != EAGAIN)
	    	 {
	    	 	 for( i=0; i<4;i++)
	    	 	 {
	    	 	 	  if(gl_fd[i].used) close(gl_fd[i].sockfd);
	    	 	 }
	    	 	 goto loop_init;
	    	 }
      }
      else if ( ret == 0)
      {
           if( !gl_fd[2].used )
           {
           	 sock=tcp_connet_server(g_para.old_ip,g_para.old_port,0);
					  if ( sock < 0)
					  {
				       dcs_log(0,0,"old_ip的连接[%s]%s:%d",strerror(errno),g_para.old_ip,g_para.old_port);
					  }
					  else
					  {
					  	  dcs_debug(0,0,"connect ip=%s,port=%d sock=%d ",g_para.old_ip,g_para.old_port,sock);
					  	  add_to_set(sock, &write_set, &max_fd);
					  	  add_to_set(sock, &error_set, &max_fd);
				        gl_fd[2].type = 2; //服务端工作链路
				        gl_fd[2].used=1;
				        gl_fd[2].sockfd=sock;
				        gl_fd[0].status=0;
					  }
           }
           if( !gl_fd[3].used)
           {
           	  sock=tcp_connet_server(g_para.new_ip,g_para.new_port,0);
						  if ( sock < 0)
						  {
					       dcs_log(0,0,"new_ip的连接[%s]%s:%d",strerror(errno),g_para.new_ip,g_para.new_port);
						  }
						  else
						  {
						  	  dcs_debug(0,0,"connect ip=%s,port=%d sock=%d ",g_para.new_ip,g_para.new_port,sock);
						  	  add_to_set(sock, &write_set, &max_fd);
						  	  add_to_set(sock, &error_set, &max_fd);
						  	  gl_fd[3].sockfd=sock;
					        gl_fd[3].type = 2; //服务端工作链路
					        gl_fd[3].used=1;
					        gl_fd[0].status=0;
						  }
           }
      } 
      else 
      {
        i=0;
        do{
	        dcs_debug(0,0,"select ret=%d",ret);
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
	 	  	 	  	     FD_CLR(gl_fd[i].sockfd,&error_set);
	 	  	 	  	     if( i==	2 )	  	 	         
	 	  	 	         		dcs_log(0,0,"  connect server socket error!addr=[%s]:[%d][%s]",g_para.old_ip,g_para.old_port,strerror(errno));	
	 	  	 	         else
	 	  	 	         		dcs_log(0,0,"  connect server socket error!addr=[%s]:[%d][%s]",g_para.new_ip,g_para.new_port,strerror(errno));	
	 	  	 	         
                }
                else
                {
                	  add_to_set(gl_fd[i].sockfd,&read_set,&max_fd);
                	  gl_fd[i].status=1;
                	  gl_fd[i].data_len=0;
                    gl_fd[i].msg_len=0;
	
                }
	              
	    	  	}
	    	  	else if(FD_ISSET(gl_fd[i].sockfd, &vread_set))
	    	 	  {
	    	 	  	 ret--;
	    	 	  	 dcs_debug(0,0,"sockfd=[%d],type=[%d]!\n",gl_fd[i].sockfd,gl_fd[i].type);
	    	 	  	 if (gl_fd[i].type ==2 ) //从应用服务器收到的交易
	    	 	  	 { 
	 	
	                if( i == 2)
	                	dcs_debug(0,0,"recv old sys ip[%s]:[%d]",g_para.old_ip,g_para.old_port);
	                else
	                	dcs_debug(0,0,"recv new sys ip[%s]:[%d]",g_para.new_ip,g_para.new_port);
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
	    	 	  	 	    FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	  	dcs_log(0,0,"appsrv recv data error!sockfd=[%d][%s]",gl_fd[i].sockfd,strerror(errno));	  
	    	 	  	 	  }
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	  int t_flag=1;
	    	 	  	 	  	  dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"recv appsrv data len=%d\n",len);
	    	 	  	 	  	  gl_fd[i].data_len=gl_fd[i].data_len+len;
	    	 	  	 	  	  while(t_flag)
	    	 	  	 	  	  {
			    	 	  	 	 	    if ( gl_fd[i].data_len >2)
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  
	    	 	  	 	  		 	  	gl_fd[i].msg_len= (unsigned char)gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1];
	    	 	  	 	  		 	  	if( gl_fd[i].msg_len > 2048 )
	    	 	  	 	  		 	  	{
	    	 	  	 	  		 	  		 dcs_log(gl_fd[i].buf,gl_fd[i].data_len,"error data");
	    	 	  	 	  		 	  		 gl_fd[i].msg_len=0;
	    	 	  	 	  		 	  		 gl_fd[i].data_len=0;
	    	 	  	 	  		 	  		 gl_fd[i].status=1;
	    	 	  	 	  		 	  	}
	    	 	  	 	  		 	  	if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
						    	 	  	 	  
	    	 	  	 	  		 	  }
				    	 	  	 	    if (gl_fd[i].status==4)
				    	 	  	 	    {
	                            // 发送数据
	                            
	                          if( gl_fd[1].used && gl_fd[1].status )
	                          {
	                          	 len=write(gl_fd[1].sockfd,gl_fd[i].buf,gl_fd[i].msg_len+2);
	                          	 if( len <=0 )
	                          	 {
	                          	 	  gl_fd[1].used=0;
	                          	 	  gl_fd[1].status=0;
	                          	 	  close(gl_fd[1].sockfd);
	                          	 	  FD_CLR(gl_fd[1].sockfd,&read_set);
	    	 	  	 	                  FD_CLR(gl_fd[1].sockfd,&error_set);
	    	 	  	 	                  dcs_log(0,0,"nac network abnormal ,send data fail[%s]",strerror(errno));
	                          	 }
	                          	 else if( len < gl_fd[i].msg_len+2 )
	                          	 {
	                          	 	  gl_fd[1].used=0;
	                          	 	  gl_fd[1].status=0;
	                          	 	  close(gl_fd[1].sockfd);
	                          	 	  FD_CLR(gl_fd[1].sockfd,&read_set);
	    	 	  	 	                  FD_CLR(gl_fd[1].sockfd,&error_set);
	    	 	  	 	                  dcs_log(0,0,"nac overflow ,send data len=[%d],msg len=%d",len,gl_fd[i].msg_len+2);
	                          	 }
	                          }
	                          else
	                          {
	                          	 dcs_log(gl_fd[i].buf,gl_fd[i].msg_len+2,"nac network not ready, data discard!");
	                          }
				    	 	  	 	  	  if ( gl_fd[i].data_len >gl_fd[i].msg_len+2)
		    	 	  	 	  		 	  {
		    	 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf+2+gl_fd[i].msg_len,gl_fd[i].data_len-2-gl_fd[i].msg_len);
		    	 	  	 	  		 	  	 gl_fd[i].data_len= gl_fd[i].data_len- 2-gl_fd[i].msg_len;
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
	    	 	  	 else if (gl_fd[i].type == 0 )   //nac网控器侦听到新的连接
	    	 	  	 {
	    	 	  	 	 //新长连接
	                dcs_log(0,0,"<accept new begin>from loc_port=[%d]!\n",g_para.loc_port);
	    	 	  	 	  int client_addr,client_port;
	    	 	  	 	  sock=tcp_accept_client(gl_fd[i].sockfd,&client_addr, &client_port);
	    	 	  	 	 	if ( sock <0 )
	    	 	  	 	 	{    
	    	 	  	 	 		 dcs_log(0,0,"accpet client new socket fail ![%s]",strerror(errno));

	    	 	  	 	  }
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	struct in_addr in;
	                  if( gl_fd[1].used && gl_fd[1].sockfd > 0) close(gl_fd[1].sockfd);
	    	 	  	 	  	add_to_set(sock, &read_set, &max_fd);
	    	 	  	 	  	add_to_set(sock, &error_set, &max_fd);
						        gl_fd[1].sockfd=sock;
						        gl_fd[1].type=1;
						        gl_fd[1].used= 1;
			        	   	gl_fd[1].status= 1;
			        	   	gl_fd[1].data_len=0;
                    gl_fd[1].msg_len=0;
			        	   	in.s_addr = client_addr;
						        dcs_debug(0,0," new nac link connected! ip=[%s],port=%d,sock=%d\n",inet_ntoa(in),sock,client_port);
	
					        }
	    	 	  	 }
	    	 	  	 else //从终端收到的交易
	    	 	  	 {
	                int t_flag=1;
	    	 	  	 	  dcs_debug(0,0,"recving from nac data sock=[%d],i=%d,data_len=%d ... \n",gl_fd[i].sockfd,i,gl_fd[i].data_len);
	    	 	  	 	  len = recv(gl_fd[i].sockfd,gl_fd[i].buf+gl_fd[i].data_len,sizeof(gl_fd[i].buf)-gl_fd[i].data_len,0);
	    	 	  	 	  if ( len <=0 )
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop;
	    	 	  	 	  	dcs_log(0,0,"recv server data error![%s]",strerror(errno));
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
		    	 	  	 	  FD_CLR(gl_fd[i].sockfd,&error_set);
		    	 	  	 	  close(gl_fd[i].sockfd);
		    	 	  	 	  gl_fd[i].used=0x00;
		    	 	  	 	  gl_fd[i].status=0x00;
                    goto cri_loop;
	    	 	  	 		}
	    	 	  	 		dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"recv data len=%d \n",len);
	    	 	  	 		gl_fd[i].data_len= gl_fd[i].data_len+len;
		    	 	  	 	while(t_flag)
		    	 	  	 	{
		    	 	  	 	  if (gl_fd[i].data_len >2)
	 	  	 	  		 	  {
	 	  	 	  		 	  	gl_fd[i].msg_len= (unsigned char)gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1];
	 	  	 	  		 	  	if( gl_fd[i].msg_len > 2048 )
	 	  	 	  		 	  	{
	 	  	 	  		 	  		 dcs_log(gl_fd[i].buf,gl_fd[i].data_len,"error data");
	 	  	 	  		 	  		 gl_fd[i].msg_len=0;
	 	  	 	  		 	  		 gl_fd[i].data_len=0;
	 	  	 	  		 	  		 gl_fd[i].status=1;
	 	  	 	  		 	  	}
	 	  	 	  		 	  	if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
	 	  	 	  		 	  }
		    	 	  	 	  if ( gl_fd[i].status ==  4)
		    	 	  	 	  {
	                    int forward=0,off_set=0;
	                    unsigned char prg_ver[5];
		   	 	  	 	  	  
		   	 	  	 	  	  off_set=2+5;
	                    if( memcmp(gl_fd[i].buf+off_set,"\x4C\x52\x49\x00\x1C", 5)==0 )
	                    	off_set=off_set+33;  //网控器增加的电话
	                    
	                    off_set=off_set+1;     //报文标志
	                    off_set=off_set+2;      //报文长度
	                    off_set=off_set+4;      //交易同步随机数
	                    off_set=off_set+1;      //报文同步序号
	                    off_set=off_set+2;      //报文内容长度
	                    off_set=off_set+1;      //报文类型
	                    off_set=off_set+1;      //结束标志
	                    memcpy(prg_ver,gl_fd[i].buf+off_set,2);
	                    off_set=off_set+2;
	                    if( gl_fd[i].data_len <=off_set) goto cri_loop;
	                      
	                    prg_ver[2]=0x00;
	                    forward=(prg_ver[0]>>4)*1000+(prg_ver[0]&0x0f)*100+ (prg_ver[1]>>4)*10+(prg_ver[1]&0x0f);
	                    dcs_debug(prg_ver,2,"prog ver=%d",forward);
	                    if( forward <2014) //老系统
		 	 	  	 	  	 	  {
		 	 	  	 	  	 	  	if(gl_fd[2].used && gl_fd[2].status)
		 	 	  	 	  	 	  		len = write(gl_fd[2].sockfd,gl_fd[i].buf,gl_fd[i].msg_len+2);
		 	 	  	 	  	 	  	else
		 	 	  	 	  	 	  		len =0;
		 	 	  	 	  	 	  }
		 	 	  	 	  	 	  else  // 新系统
		 	 	  	 	  	 	  {
		 	 	  	 	  	 	  	if(gl_fd[3].used && gl_fd[3].status)
		 	 	  	 	  	 	  		len = write(gl_fd[3].sockfd,gl_fd[i].buf,gl_fd[i].msg_len+2);
		 	 	  	 	  	 	  	else len=0;
		 	 	  	 	  	 	  }
			    	 	  	 	  if ( len <=0 ) 	  	 	     	 	  	 	  	
			     	 	  	 	  {
			     	 	  	 	  		  close(gl_fd[i].sockfd);	
			     	 	  	 	  		  gl_fd[2].used=0x00;
					    	 	  	 	    gl_fd[2].data_len=0;
					    	 	  	 	    gl_fd[2].status=0;     	 	  	 	  		  
					    	 	  	 	  	
					    	 	  	 	  	FD_CLR(gl_fd[2].sockfd,&read_set);
					    	 	  	 	  	FD_CLR(gl_fd[2].sockfd,&error_set);
	                          if( len == 0)
	                          	dcs_log(0,0," write server not ready!");
	                          else
			     	 	  	 	  		  	dcs_log(0,0," write server data error![%s]",strerror(errno));
			     	 	  	 	  }
			    	 	  	 	  else
			    	 	  	 	  {
					    	 	  	 	 	dcs_debug(gl_fd[i].buf,len ,"\nsend server data len=%d\n",len);  	
			    	 	  	 	  }
			    	 	  	 	}
			    	 	  	 	if ( gl_fd[i].data_len >gl_fd[i].msg_len+2)
	 	  	 	  		 	  {
	 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf+2+gl_fd[i].msg_len,gl_fd[i].data_len-2-gl_fd[i].msg_len);
	 	  	 	  		 	  	 gl_fd[i].data_len= gl_fd[i].data_len- 2-gl_fd[i].msg_len;
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