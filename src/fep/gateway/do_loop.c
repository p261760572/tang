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
extern int gl_sock[MAX_LINK],role,stop,crypt_flag,iso_mode,gs_xft,gs_tpdu;
extern struct PARA g_para[MAX_LINK];
#define PUBLIC_EXPONENT RSA_F4
void do_loop()
{
	
  fd_set vread_set,vwrite_set,verror_set;
  struct timeval tv;
  
  int max_fd ,vmax_fd,i ,srv_flg,sock,cli_flg,ret,error,len;
  int flags,l,s;
  unsigned char buffer[8192],tmp[32],rsa_n[513],ver_time[15];
  char * p;
  time_t rsa_time;
  struct  tm *tm;
//  BIGNUM *bnn, *bne, *bnd;  
  RSA *r;
  
  
//  bnn = BN_new();                             
//  bne = BN_new();                             
//  bnd = BN_new();
//  r = RSA_new();                         
//	r->n = bnn;                                 
//	r->e = bne;                                 
//	r->d = bnd; 
 if (crypt_flag)
 {
		r=RSA_generate_key(1024,RSA_F4,0,0);
		if ( r == NULL ) return;
		memset(rsa_n,0,sizeof(rsa_n));
		p=BN_bn2hex(r->n);
		if ( p != NULL )
		{
				strcpy(rsa_n,p);
				free(p);
				dcs_debug(0,0,"rsa-n[%d]=%s",strlen(rsa_n),rsa_n);
		}
		else
			dcs_debug(0,0,"rsa generate fail!");
		p=BN_bn2hex(r->d);
		if ( p != NULL )
		{
				dcs_debug(0,0,"rsa-d[%d]=%s",strlen(p),p);
				free(p);
		}
		else
			dcs_debug(0,0,"rsa generate fail!");
		rsa_time=time(NULL);
		tm=localtime(&rsa_time);
		sprintf(ver_time,"%04d%02d%02d%02d%02d%02d",tm->tm_year,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
 }
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
    // 创建与服务器后端应用通信连接
      dcs_debug(0,0,"connecting posp begin! [%s]:[%d]\n",g_para[0].svr_ip,g_para[0].svr_port);
      sock=tcp_connet_server(g_para[i].svr_ip, g_para[i].svr_port,0);
      if( sock >=0 )
      {
        add_to_set(sock, &write_set, &max_fd);
        add_to_set(sock, &error_set, &max_fd);
        gl_fd[sock].sockfd=sock;
        strncpy(gl_fd[sock].ip,g_para[i].svr_ip,sizeof(gl_fd[sock].ip)-1);
        gl_fd[sock].port=g_para[i].svr_port;
        gl_fd[sock].type = 1;
        gl_fd[sock].timeout =g_para[i].timeout;
        gl_fd[sock].used=0x30;
        gl_fd[sock].ndx=i;
        gl_fd[sock].len_type=g_para[i].type;
        if( gl_fd[sock].len_type==0) gl_fd[sock].len_len=4;
        else if( gl_fd[sock].len_type==1) gl_fd[sock].len_len=2;
        else if( gl_fd[sock].len_type==2) gl_fd[sock].len_len=8;
        else if( gl_fd[sock].len_type==3) gl_fd[sock].len_len=10;
        else if( gl_fd[sock].len_type==4 ||  gl_fd[sock].len_type==5 ) gl_fd[sock].len_len=0;
        gl_fd[sock].data_len=0;
	      gl_fd[sock].msg_len=0;
        gl_fd[sock].status=0;
        gl_sock[i]= sock;
        dcs_log(0,0,"connect svr_ip succ! [%s]:[%d][sock:%d]\n",gl_fd[sock].ip,gl_fd[sock].port,sock);
      }
      else
      	_srv_exit(0);

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
          gl_fd[sock].len_type=g_para[i].type;
          if( gl_fd[sock].len_type==0) gl_fd[sock].len_len=4;
	        else if( gl_fd[sock].len_type==1) gl_fd[sock].len_len=2;
	        else if( gl_fd[sock].len_type==2) gl_fd[sock].len_len=8;
	        else if( gl_fd[sock].len_type==3) gl_fd[sock].len_len=10;
	        else if( gl_fd[sock].len_type==4 ||  gl_fd[sock].len_type==5 ) gl_fd[sock].len_len=0;
//	        gl_fd[sock].remote_sockfd=gl_sock[i];
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
			    	 	  	 if ( gl_fd[i].type == 1)/* 服务端主动链接 */
	    	 	  	 	   {   
	    	 	  	 	   }
	    	 	  	 	   close(gl_fd[i].sockfd);
			    	 	  	 FD_CLR(gl_fd[i].sockfd,&read_set);
			    	 	  	 gl_fd[i].used=0x00;
			    	 	  	 perror("sockfd timeout!");
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
	  	 	  	 	  getsockopt(i,SOL_SOCKET,SO_ERROR,&error,&len) ;
	                 
                FD_CLR(i,&write_set);
                FD_CLR(i,&vwrite_set);
                if( 0 != error )
                { 
                   close(i);
	 	  	 	  	     gl_fd[i].used=0x00;	  
	 	  	 	  	     FD_CLR(gl_fd[i].sockfd,&error_set);	 	  	 	         
//	 	  	 	         dcs_log(0,0," app svr socket error!sockfd=[%d][%s]",gl_fd[i].sockfd,strerror(errno));	
	 	  	 	         if (connect_sock(i,&max_fd)<0)
							       	_srv_exit(0); 	  	 	         
                }
                else
                {
                	   add_to_set(gl_fd[i].sockfd,&read_set,&max_fd);
                		 gl_fd[i].status=1;
                		 gl_fd[i].data_len=0;
                		 gl_fd[i].msg_len=0;
                		 dcs_debug(0,0,"connected finish! sock=%d",i);
                }
	              
	    	  	}
	    	 	  else if(FD_ISSET(i, &vread_set)) /*对进入接收状态链路进行处理*/
	    	 	  {
	    	 	  		ret--;
//	    	 	  		dcs_debug(0,0,"sockfd=[%d],type=[%d]!\n",gl_fd[i].sockfd,gl_fd[i].type);
	    	 	  	 if (gl_fd[i].type == 1)//应用服务器收到的报文
	    	 	  	 { 
	    	 	  	 	  
	    	 	  	 	 
	    	 	  	 	  memset(buffer,0,sizeof(buffer));
	    	 	  	 	  len=read(gl_fd[i].sockfd,gl_fd[i].buf+gl_fd[i].data_len,sizeof(gl_fd[i].buf)-gl_fd[i].data_len);
	    	 	  	 	  if ( len <= 0)
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop;
	    	 	  	 	  	close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	  	gl_fd[i].sockfd=-1;
	    	 	  	 	  	if ( len <0)
	    	 	  	 	  	dcs_log(0,0,"posp recv data error![%s]",strerror(errno));
	    	 	  	 	  	if (connect_sock(i,&max_fd)<0)
							       	_srv_exit(0); 
	    	 	  	 	  }
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	int t_flag;
	    	 	  	 	  	gl_fd[i].last_time= time(NULL);
	    	 	  
			    	 	  	 	  dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"\nrecv posp data len=%d,data_len=%d,len_type=%d\n",len,gl_fd[i].data_len,gl_fd[i].len_type);
			    	 	  	 	  gl_fd[i].data_len= gl_fd[i].data_len+len;
			    	 	  	 	  t_flag=1;
	    	 	  	 	  	while(t_flag)
	    	 	  	 	  	{
	    	 	  	 	  		 	  if ( gl_fd[i].len_type==1 && gl_fd[i].data_len >2)
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  
	    	 	  	 	  		 	  	gl_fd[i].msg_len=(unsigned char)gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1];
	    	 	  	 	  		 	  	if ( gl_fd[i].msg_len>0 &&  gl_fd[i].msg_len<= gl_fd[i].data_len)
	    	 	  	 	  		 	  	{	 
	    	 	  	 	  		 	  		gl_fd[i].status=4;
							    	 	  	 	  if( crypt_flag)
							    	 	  	 	  {
//							    	 	  	 	  		dcs_debug(gl_fd[gl_fd[i].ndx].key,8,"entry send client crypt proc! key ");
                                  int k;
															    if( iso_mode )
	    	 	  	 	  		 	          		k=get_sockfd(gl_fd[i].buf+gl_fd[i].len_len ,gl_fd[i].msg_len,max_fd,i);
				    	 	  	 	  		 	    else
				    	 	  	 	  		 	    {
				    	 	  	 	  		 	    	 memcpy(tmp,gl_fd[i].buf+gl_fd[i].len_len+1,4);
				    	 	  	 	  		 	    	 tmp[4]=0x00;
				    	 	  	 	  		 	    	 k= atoi(tmp);
				    	 	  	 	  		 	    }
				    	 	  	 	  		 	    if( k < 0 || k >= FD_SETSIZE ) 
				    	 	  	 	  		 	    {
				    	 	  	 	  		 	    		gl_fd[i].msg_len=0;
							    	 	  	 		 				gl_fd[i].data_len=0;
							    	 	  	 		 				gl_fd[i].status=1;
							    	 	  	 		 				dcs_log(gl_fd[i].buf,gl_fd[i].msg_len,"无效报文！");
				    	 	  	 	  		 	    		goto cri_loop;
				    	 	  	 	  		 	    }
								    	 	  	 	  	l= gl_fd[i].msg_len+2+(8-(gl_fd[i].msg_len+2)%8);
								    	 	  	 	  	memset(buffer,0,l);
								    	 	  	 	  	for( s=0; s< l/8;s++)
								    	 	  	 	  	  DES((unsigned char*)gl_fd[k].key,(unsigned char*)gl_fd[i].buf+(s*8),buffer+(2+3+s*8));
								    	 	  	 	  	buffer[0]=(unsigned char)((l+3)/256);
								    	 	  	 	  	buffer[1]=(unsigned char)((l+3)%256);
								    	 	  	 	  	memcpy(buffer+2,"00",2);
								    	 	  	 	  	memcpy(buffer+4,"\x22",1);
								    	 	  	 	  	gl_fd[i].msg_len=l+3;
							    	 	  	 	  }
							    	 	  	 	}
							    	 	  	 	
							    	 	  	 	if ( gl_fd[i].msg_len >8170  )
							    	 	  	 	{
							    	 	  	 		 gl_fd[i].msg_len=0;
							    	 	  	 		 gl_fd[i].data_len=0;
							    	 	  	 		 gl_fd[i].status=1;
							    	 	  	 		 goto cri_loop;
							    	 	  	 	}
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else  if ( gl_fd[i].len_type==0 && gl_fd[i].data_len >4)
	    	 	  	 	  		 	  {
//	    	 	  	 	  		 	  	  dcs_debug(0,0,"entr len_type=0");
	    	 	  	 	  		 	  	   memcpy(buffer,gl_fd[i].buf,4);
	    	 	  	 	  		 	  	   buffer[4]=0x00;
	    	 	  	 	  		 	  	   gl_fd[i].msg_len= atoi(buffer);
	    	 	  	 	  		 	  	   if ( gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
//								    	 	  	 	  dcs_debug(0,0,"entr len_type=0 msg_len=%d,status=%d", gl_fd[i].msg_len,gl_fd[i].status);
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else if ( gl_fd[i].len_type==2 && gl_fd[i].data_len >8)
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  	 if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else if ( gl_fd[i].len_type==3 && gl_fd[i].data_len >10)
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  	 if ( gl_fd[i].msg_len>0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else
	    	 	  	 	  		 	  {
	    	 	  	 	  		 	  	 if (gl_fd[i].msg_len>0 &&  gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  if ( gl_fd[i].status==4)
	    	 	  	 	  		 	  {
//	    	 	  	 	  		 	    dcs_debug(0,0,"begin write to client data");
	    	 	  	 	  		 	    if( iso_mode )
	    	 	  	 	  		 	       s=get_sockfd(gl_fd[i].buf+gl_fd[i].len_len ,gl_fd[i].msg_len,max_fd,i);
	    	 	  	 	  		 	    else
	    	 	  	 	  		 	    {
	    	 	  	 	  		 	    	 memcpy(tmp,gl_fd[i].buf+gl_fd[i].len_len+1,4);
	    	 	  	 	  		 	    	 tmp[4]=0x00;
	    	 	  	 	  		 	    	 s= (unsigned char)tmp[2]*256+(unsigned char)tmp[3];
	    	 	  	 	  		 	    	 
	    	 	  	 	  		 	    	 if( s >FD_SETSIZE ||
	    	 	  	 	  		 	    	 	!gl_fd[s].used || 
	    	 	  	 	  		 	    	 	 ((unsigned char)gl_fd[s].key[0]*256+(unsigned char)gl_fd[s].key[1]) != ((unsigned char)tmp[0]*256+(unsigned char)tmp[1]))
	    	 	  	 	  		 	    	 {
		    	 	  	 	  		 	    	 	t_flag =0;
			    	 	  	 	  		 	  		gl_fd[i].msg_len=0;
			    	 	  	 	  		 	  		gl_fd[i].data_len=0;
			    	 	  	 	  		 	  		gl_fd[i].status=1;
			    	 	  	 	  		 	  		dcs_log(tmp,4,"<%s> src socket not match!",__FUNCTION__);
			    	 	  	 	  		 	  		break;
	    	 	  	 	  		 	    	 }
	    	 	  	 	  		 	    	 if (!gl_fd[s].used || 
	    	 	  	 	  		 	    	 	 ((unsigned char)gl_fd[s].key[0]*256+(unsigned char)gl_fd[s].key[1]) != ((unsigned char)tmp[0]*256+(unsigned char)tmp[1]))
	    	 	  	 	  		 	    	 {
	    	 	  	 	  		 	    	 	 
	    	 	  	 	  		 	    	 }
	    	 	  	 	  		 	    }
	    	 	  	 	  		 	    if ( crypt_flag && s >0)
	    	 	  	 	  		 	    	len = write(s,buffer,gl_fd[i].msg_len+gl_fd[i].len_len);
	    	 	  	 	  		 	    else if ( s >0)
	    	 	  	 	  		 	    {
	    	 	  	 	  		 	    	  if ( gl_fd[i].len_type == 1 && gl_fd[s].remark[0]==0x31)
	    	 	  	 	  		 	    	  {
	    	 	  	 	  		 	    	  	  memcpy( buffer+2,gl_fd[i].buf,gl_fd[i].msg_len+gl_fd[i].len_len);
	    	 	  	 	  		 	    	  	  buffer[0]=(gl_fd[i].msg_len+gl_fd[i].len_len)/256;
	    	 	  	 	  		 	    	  	  buffer[1]=(gl_fd[i].msg_len+gl_fd[i].len_len)%256;
	    	 	  	 	  		 	    	  	  len = write(s,buffer,gl_fd[i].msg_len+gl_fd[i].len_len+2);
	    	 	  	 	  		 	    	  }
	    	 	  	 	  		 	    		else
	    	 	  	 	  		 	    			len = write(s,gl_fd[i].buf,gl_fd[i].msg_len+gl_fd[i].len_len);
	    	 	  	 	  		 	    }
	    	 	  	 	  		 	    else
	    	 	  	 	  		 	       len=0;
		    	 	  	 	  		 	  if ( len <= 0)
		    	 	  	 	  		 	  {
		    	 	  	 	  		 	  	//发送数据失败
		    	 	  	 	  		 	  	 dcs_log(gl_fd[i].buf,gl_fd[i].msg_len+gl_fd[i].len_len,"posp recv data send client error![%s] sock id=%d",strerror(errno),s);
		    	 	  	 	  		 	  }
		    	 	  	 	  		 	  else
		    	 	  	 	  		 	  {
		    	 	  	 	  		 	  	if ( crypt_flag)
		    	 	  	 	  		 	  		  dcs_debug(buffer,gl_fd[i].msg_len+gl_fd[i].len_len,"posp1 recv data send client succ!remark[0]=%02x\n", gl_fd[s].remark[0]);
		    	 	  	 	  		 	    else
		    	 	  	 	  		 	    	  dcs_debug(gl_fd[i].buf,gl_fd[i].msg_len+gl_fd[i].len_len,"posp recv data send client succ!remark[0]=%02x\n", gl_fd[s].remark[0]);
		    	 	  	 	  		 	    
		    	 	  	 	  		 	  }
		    	 	  	 	  		 	  if ( gl_fd[i].data_len >=gl_fd[i].msg_len+gl_fd[i].len_len)
		    	 	  	 	  		 	  {
		    	 	  	 	  		 	  	 memcpy(buffer,gl_fd[i].buf+gl_fd[i].len_len+gl_fd[i].msg_len,gl_fd[i].data_len-gl_fd[i].len_len-gl_fd[i].msg_len);
		    	 	  	 	  		 	  	 gl_fd[i].data_len= gl_fd[i].data_len- gl_fd[i].len_len-gl_fd[i].msg_len;
		    	 	  	 	  		 	  	 memcpy(gl_fd[i].buf,buffer,gl_fd[i].data_len);
		    	 	  	 	  		 	  	 gl_fd[i].msg_len=0;
		    	 	  	 	  		 	  	 gl_fd[i].status=1;
		    	 	  	 	  		 	  }
		    	 	  	 	  		 	  else
		    	 	  	 	  		 	  {
		    	 	  	 	  		 	  		t_flag =0;
		    	 	  	 	  		 	  		gl_fd[i].msg_len=0;
		    	 	  	 	  		 	  		gl_fd[i].data_len=0;
		    	 	  	 	  		 	  		gl_fd[i].status=1;
		    	 	  	 	  		 	  }
/*		    	 	  	 	  		 	  
		    	 	  	 	  		 	  if ( s >0)
		    	 	  	 	  		 	  {
			    	 	  	 	  		 	  close(s);
			    	 	  	 	  		 	  gl_fd[s].used=0x00;
			    	 	  	 	  		 	  FD_CLR(s,&read_set);
			    	 	  	 	  		 	  FD_CLR(s,&error_set);
		    	 	  	 	  		 	  }
*/	    	 	  	 	  		 	  }
	    	 	  	 	  		 	  else 
	    	 	  	 	  		 	  {	
	    	 	  	 	  		 	  	 gl_fd[i].status=1;
	    	 	  	 	  		 	     t_flag =0;
	    	 	  	 	  		 	  }
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
//	    	 	  	 	 		 close(gl_fd[i].sockfd);
//	    	 	  	 	 		 gl_fd[i].used=0x00;
	//    	 	  	 	 		 cli_flg=0;
	    	 	  	 	  }
	    	 	  	 	 	else if ( sock >=FD_SETSIZE )
	    	 	  	 	 	{
	    	 	  	 	 	//超出最大连接限制
	    	 	  	 	 	   dcs_log(0,0,"client nums limited!\n");
	    	 	  	 	 		 close(sock);
	    	 	  	 	  } 
	    	 	  	 	  else
	    	 	  	 	  {
	    	 	  	 	  	struct in_addr in;
	    	 	  	 	  	in.s_addr = client_addr;
	    	 	  	 	  	add_to_set(sock, &read_set, &max_fd);
						        strcpy(gl_fd[sock].ip,inet_ntoa(in));
						        gl_fd[sock].sockfd=sock;
						        gl_fd[sock].type = 2;
						        gl_fd[sock].len_type=gl_fd[i].len_type;
						        gl_fd[sock].len_len=gl_fd[i].len_len;
						        gl_fd[sock].status=1;
						        gl_fd[sock].data_len=0;
						        gl_fd[sock].msg_len=0;
						        gl_fd[sock].ndx= gl_fd[i].ndx;
						        gl_fd[sock].timeout =90;
						        gl_fd[sock].last_time=time(NULL);
						        gl_fd[sock].used=0x30;
						        gl_fd[sock].port=client_port;						        
					          dcs_debug(0,0," new client connected!ndx=%d ip=%s ,port=%d",gl_fd[sock].ndx,gl_fd[sock].ip,gl_fd[sock].port);
					        }
	    	 	  	 }
	    	 	  	 else  //客户端收到的报文
	    	 	  	 {
	    	 	  	 	 
	    	 	  	 	  dcs_debug(0,0,"client recving data ... \n");
	
	    	 	  	 	  len = recv(gl_fd[i].sockfd,gl_fd[i].buf+gl_fd[i].data_len,sizeof(gl_fd[i].buf)-gl_fd[i].data_len,0);
	    	 	  	 	  if ( len <=0 )
	    	 	  	 	  {
	    	 	  	 	  	if (len <0)
	    	 	  	 	  	if (errno == EINTR || errno == EAGAIN) goto cri_loop;
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
	    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
	    	 	  	 	  	close(gl_fd[i].sockfd);
	    	 	  	 	  	gl_fd[i].used=0x00;
	    	 	  	 	  	dcs_debug(0,0,"recv client data error![%s]",strerror(errno));
	    	 	  	 	  	goto cri_loop;
	    	 	  	 	  }
	    	 	  	 	  gl_fd[i].last_time= time(NULL);
	    	 	  	 	  dcs_debug(gl_fd[i].buf+gl_fd[i].data_len,len ,"client recv data len=%d len_type=%d crypt_flag=%d status=%d\n",len,gl_fd[i].len_type,crypt_flag,gl_fd[i].status);
	    	 	  	 	  gl_fd[i].data_len=gl_fd[i].data_len+len;    	 	  	 	  
	    	 	  	 	 if( gl_fd[i].len_type == 1 && gl_fd[i].data_len >2)//两字节长度包处理
	    	 	  	 	 {
		    	 	  	 	 	dcs_debug(0,0,"entery len process ");
		    	 	  	 	 	gl_fd[i].msg_len= (unsigned char)gl_fd[i].buf[0]*256+ (unsigned char)gl_fd[i].buf[1];
		    	 	  	 	 	if ( gl_fd[i].msg_len >8170  )
		    	 	  	 	 	{
		    	 	  	 	 		FD_CLR(gl_fd[i].sockfd,&read_set);
		    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
		    	 	  	 	  	close(gl_fd[i].sockfd);
		    	 	  	 	  	gl_fd[i].used=0x00;
		    	 	  	 	  	dcs_log(0,0,"recv client len error!len=[%d]",gl_fd[i].msg_len);
		    	 	  	 	  	goto cri_loop;
		    	 	  	 	 	}
		    	 	  	 	 	dcs_debug(0,0,"len process end [succ]");
		    	 	  	 	 	if ( !crypt_flag)
	    	 	  	 	  	{   
	    	 	  	 	  		 if ( gl_fd[i].msg_len <= gl_fd[i].data_len && gl_fd[i].msg_len>0) gl_fd[i].status=4;
	    	 	  	 	  			dcs_debug(0,0," !crypt_flag proc");
	    	 	  	 	    }
	    	 	  	 	  	else
	    	 	  	 	  	{
    	 	  	 	  		 	dcs_debug(0,0,"entry crypt_flag  proc ");
    	 	  	 	  		  if ( gl_fd[i].status == 1 )
    	 	  	 	  			{				
    	 	  	 	  				 dcs_debug(0,0,"entry crypt_flag  status =1 ");
    	 	  	 	  				 if ( gl_fd[i].msg_len <= gl_fd[i].data_len)
    	 	  	 	  				 {
    	 	  	 	           	 dcs_debug(0,0,"entry public get proc %02x",(unsigned char)gl_fd[i].buf[2]);
    	 	  	 	  				 	  if( gl_fd[i].buf[2]==0x20 ) //获取公钥
    	 	  	 	  				 	  {
    	 	  	 	  				 	  	 memcpy(buffer+2,"00",2);
    	 	  	 	  				 	  	 l=2;
    	 	  	 	  				 	  	 buffer[l+2]=0x20;
    	 	  	 	  				 	  	 l=l+1;
//    	 	  	 	  				 	  	 dcs_debug(0,0,"entry public 1");
    	 	  	 	  				 	  	 memcpy(buffer+l+2,ver_time,14);
//    	 	  	 	  				 	  	 dcs_debug(0,0,"entry public 2");
    	 	  	 	  				 	  	 l=l+14;
    	 	  	 	  				 	  	 memcpy(buffer+l+2,rsa_n,strlen(rsa_n));
//    	 	  	 	  				 	  	  dcs_debug(0,0,"entry public 3");
    	 	  	 	  				 	  	 l=l+strlen(rsa_n);
    	 	  	 	  				 	  	 buffer[0]=(unsigned char)(l/256);
    	 	  	 	  				 	  	 buffer[1]=(unsigned char)(l%256);
//    	 	  	 	  				 	  	  dcs_debug(0,0,"entry public 4");
    	 	  	 	  				 	  	 len = write(gl_fd[i].sockfd,buffer,l+2);
    	 	  	 	  				 	  	 if ( len <=0 )
    	 	  	 	  				 	  	 {
    	 	  	 	  				 	  	 		FD_CLR(gl_fd[i].sockfd,&read_set);
								    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
								    	 	  	 	  	close(gl_fd[i].sockfd);
								    	 	  	 	  	gl_fd[i].used=0x00;
								    	 	  	 	  	dcs_log(0,0,"send client key fail ![%s]",strerror(errno));
  							    	 	  	 	  	goto cri_loop;
								    	 	  	 	  	
    	 	  	 	  				 	  	 }
//    	 	  	 	  				 	  	  dcs_debug(0,0,"entry public 5");
    	 	  	 	  				 	  	 gl_fd[i].msg_len=0;
    	 	  	 	  				 	  	 gl_fd[i].data_len=0;
    	 	  	 	  				 	  	 gl_fd[i].status=2;
    	 	  	 	  				 	  	 dcs_debug(buffer,len,"send client data len=[%d]",len);
    	 	  	 	  				 	  }
    	 	  	 	  				 	  else if( gl_fd[i].buf[2]==0x21 ) //随机数上传
    	 	  	 	  				 	  {
    	 	  	 	  				 	 			 	  	unsigned char tmp_key[17], rand_buf[128];
                                 l= gl_fd[i].msg_len -15; 
                                 if ( l != 128)
                                 {
                                 	    FD_CLR(gl_fd[i].sockfd,&read_set);
										    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
										    	 	  	 	  	close(gl_fd[i].sockfd);
										    	 	  	 	  	gl_fd[i].used=0x00;
															        dcs_log(0,0,"client key len error l=%d!\n",l);
															        goto cri_loop;
                                 }
                                 memcpy(rand_buf,gl_fd[i].buf+17,l);
    	 	  	 	  				 	  		 l = RSA_private_decrypt(128,rand_buf ,buffer, r, RSA_NO_PADDING);
															   if (l < 0)
															   {
															        FD_CLR(gl_fd[i].sockfd,&read_set);
										    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
										    	 	  	 	  	close(gl_fd[i].sockfd);
										    	 	  	 	  	gl_fd[i].used=0x00;
															        dcs_log(0,0,"Decrypt client key failed!\n");
															        goto cri_loop;
															   }
															   dcs_debug(buffer,l,"Decrypt client rand data l=%d",l);									   
															   memcpy(gl_fd[i].key,buffer+l-8,8);
															   //产生本地随机数
															   srand((unsigned)time(NULL));
															   
															   for ( l=0; l<8; l++)
															   {
															   	  buffer[l]=((unsigned char)rand()-l*l)%256;
															   }
															   dcs_debug(buffer,8,"本地产生的随机数");
															   DES(gl_fd[i].key,buffer,tmp_key);
															   
															   for( l=0;l<8;l++)
															   {
															   	 gl_fd[i].key[l]=(unsigned char)gl_fd[i].key[l]^(unsigned char)buffer[l];
															   }
															   memcpy(buffer+2,"00",2);
	    	 	  	 	  				 	  	 l=4;
	    	 	  	 	  				 	  	 buffer[l]=0x21;
	    	 	  	 	  				 	  	 l=l+1;		    	 	  	 	  				 	  	 
	    	 	  	 	  				 	  	 memcpy(buffer+l,tmp_key,8);
	    	 	  	 	  				 	  	 l=l+8;
	    	 	  	 	  				 	  	 DES((unsigned char *)gl_fd[i].key,(unsigned char *)"00000000",buffer+l);
	    	 	  	 	  				 	  	 l=l+4;
	    	 	  	 	  				 	  	 buffer[0]=(unsigned char)((l-2)/256);
	    	 	  	 	  				 	  	 buffer[1]=(unsigned char)((l-2)%256);
															   len = write(gl_fd[i].sockfd,buffer,l);
	    	 	  	 	  				 	  	 if ( len <=0 )
	    	 	  	 	  				 	  	 {
	    	 	  	 	  				 	  	 		FD_CLR(gl_fd[i].sockfd,&read_set);
									    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
									    	 	  	 	  	close(gl_fd[i].sockfd);
									    	 	  	 	  	gl_fd[i].used=0x00;
									    	 	  	 	  	dcs_log(0,0,"send client rand key fail![%s]",strerror(errno));
									    	 	  	 	  	goto cri_loop;
	    	 	  	 	  				 	  	 }
	    	 	  	 	  				 	  	 gl_fd[i].msg_len=0;
	    	 	  	 	  				 	  	 gl_fd[i].data_len=0;
	    	 	  	 	  				 	  	 gl_fd[i].status=3;
	    	 	  	 	  				 	  	 dcs_debug(buffer,len,"send client data len=[%d]",len);
    	 	  	 	  				 	  }
    	 	  	 	  				 }
    	 	  	 	  				 	  
    	 	  	 	  			}    	 	  	 	  	
	  	 	  	 	  			else if ( gl_fd[i].status == 2 && gl_fd[i].msg_len <= gl_fd[i].data_len)//密钥交换状态，等待随机数上传
	  	 	  	 	  			{
	  	 	  	 	  				unsigned char tmp_key[17];
	  	 	  	 	  				if( gl_fd[i].buf[2]!=0x21 )
	  	 	  	 	  				{
	  	 	  	 	  					FD_CLR(gl_fd[i].sockfd,&read_set);
				    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
				    	 	  	 	  	close(gl_fd[i].sockfd);
				    	 	  	 	  	gl_fd[i].used=0x00;
				    	 	  	 	  	goto cri_loop;
	  	 	  	 	  				}
	                       l= gl_fd[i].msg_len -15;  
	                       if ( l != 128)
                         {
                         	    FD_CLR(gl_fd[i].sockfd,&read_set);
						    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
						    	 	  	 	  	close(gl_fd[i].sockfd);
						    	 	  	 	  	gl_fd[i].used=0x00;
											        dcs_log(0,0,"client key len error l=%d!\n",l);
											        goto cri_loop;
                         }                      
	                       l = RSA_private_decrypt(128, (unsigned char *)(gl_fd[i].buf+17),buffer, r, RSA_NO_PADDING);
//		 	  				 	  		 l = RSA_private_decrypt(l, gl_fd[i].buf+17,buffer, r, RSA_PKCS1_PADDING);
											   if (l < 0)
											   {
											        FD_CLR(gl_fd[i].sockfd,&read_set);
						    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
						    	 	  	 	  	close(gl_fd[i].sockfd);
						    	 	  	 	  	gl_fd[i].used=0x00;
											        dcs_log(gl_fd[i].buf,gl_fd[i].data_len,"Decrypt client rand key failed!\n");
											        goto cri_loop;
											   }
											   dcs_debug(buffer,l,"recv client rand data:l=%d",l);
											   memcpy(gl_fd[i].key,buffer+l-8,8);
											   //产生本地随机数
											   srand((unsigned)time(NULL));
											   
											   for ( l=0; l<8; l++)
											   {
											   	  buffer[l]=(unsigned char)rand()%256;
											   }
											   dcs_debug(buffer,8,"本地产生的随机数");
											   DES((unsigned char *)gl_fd[i].key,(unsigned char *)buffer,tmp_key);
											   for( l=0;l<8;l++)
											   {
											   	 gl_fd[i].key[l]=(unsigned char)gl_fd[i].key[l]^(unsigned char)buffer[l];
											   }
											   memcpy(buffer+2,"00",2);
	  	 	  				 	  	 l=2;
	  	 	  				 	  	 buffer[l+2]=0x21;
	  	 	  				 	  	 l=l+1;		    	 	  	 	  				 	  	 
	  	 	  				 	  	 memcpy(buffer+l+2,tmp_key,8);
	  	 	  				 	  	 l=l+8;
	  	 	  				 	  	 DES(gl_fd[i].key,"00000000",buffer+l+2);
	    	 	  	 	  			 l=l+4;
	  	 	  				 	  	 buffer[0]=(unsigned char)(l/256);
	  	 	  				 	  	 buffer[1]=(unsigned char)(l%256);
											   len = write(gl_fd[i].sockfd,buffer,l+2);
	  	 	  				 	  	 if ( len <=0 )
	  	 	  				 	  	 {
	  	 	  				 	  	 		FD_CLR(gl_fd[i].sockfd,&read_set);
					    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
					    	 	  	 	  	close(gl_fd[i].sockfd);
					    	 	  	 	  	gl_fd[i].used=0x00;
					    	 	  	 	  	dcs_log(0,0,"send client rand key fail![%s]",strerror(errno));
					    	 	  	 	  	goto cri_loop;
	  	 	  				 	  	 }
	  	 	  				 	  	 gl_fd[i].msg_len=0;
	  	 	  				 	  	 gl_fd[i].data_len=0;
	  	 	  				 	  	 gl_fd[i].status=3;
	  	 	  				 	  	 dcs_debug(buffer,len,"send client data len=[%d]",len);
	  	 	  	 	  			}
		 	  	 	  			  else if ( gl_fd[i].status == 3 ) // 数据传输状态，解密数据
	  	 	  	 	  			{
	  	 	  	 	  				if ( gl_fd[i].msg_len <= gl_fd[i].data_len)
	  	 	  	 	  				{
//	  	 	  	 	  					dcs_debug(0,0,"entry crypt data status 3 ");
	  	 	  	 	  					if( memcmp(ver_time,gl_fd[i].buf+3,14)!=0)
	  	 	  	 	  					{
	  	 	  	 	  						  FD_CLR(gl_fd[i].sockfd,&read_set);
						    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
						    	 	  	 	  	close(gl_fd[i].sockfd);
						    	 	  	 	  	gl_fd[i].used=0x00;
						    	 	  	 	  	dcs_log(0,0,"ver error![%s] ",ver_time);
						    	 	  	 	  	goto cri_loop;
	  	 	  	 	  					}
	  	 	  	 	  					for (s=0; s<(gl_fd[i].msg_len-14-1)/8;s++)
	  	 	  	 	  					   _DES((unsigned char*)gl_fd[i].key,(unsigned char*)(gl_fd[i].buf+(2+1+14+s*8)),buffer+(s*8));
	  	 	  	 	  					dcs_debug(buffer,gl_fd[i].msg_len-14-1,"decode data,len=%d",gl_fd[i].msg_len-14-1);
	  	 	  	 	  					l=(unsigned char)buffer[0]*256+(unsigned char)buffer[1];
	  	 	  	 	  					if ( (l-2)>=gl_fd[i].msg_len || buffer[2]!=0x60)
	  	 	  	 	  					{
	  	 	  	 	  							FD_CLR(gl_fd[i].sockfd,&read_set);
						    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&error_set);
						    	 	  	 	  	close(gl_fd[i].sockfd);
						    	 	  	 	  	gl_fd[i].used=0x00;
						    	 	  	 	  	dcs_log(0,0,"Decrypt client data fail!");
						    	 	  	 	  	goto cri_loop;
	  	 	  	 	  					}
	  	 	  	 	  					gl_fd[i].msg_len=l;
	  	 	  	 	  					gl_fd[i].data_len=l+2;
	  	 	  	 	  					memcpy(gl_fd[i].buf,buffer,l+2);
	  	 	  	 	  					gl_fd[i].status=4;
	  	 	  	 	  				}
	  	 	  	 	  			}
	    	 	  	 	  	}	
	
	    	 	  	 	 }
	    	 	  	 	 else if( gl_fd[i].len_type == 0 && gl_fd[i].data_len >4)
	    	 	  	 	 {
	    	 	  	 	 	    
//	    	 	  	 	 	    dcs_debug(0,0,"entr len_type=0");
  	 	  		 	  	   memcpy(buffer,gl_fd[i].buf,4);
  	 	  		 	  	   buffer[4]=0x00;
  	 	  		 	  	   gl_fd[i].msg_len= atoi(buffer);
  	 	  		 	  	   if ( gl_fd[i].msg_len > 0 && gl_fd[i].msg_len <= gl_fd[i].data_len) gl_fd[i].status=4;
//			    	 	  	 	  dcs_debug(0,0,"entr len_type=0 msg_len=%d,data_len=%d,status=%d", gl_fd[i].msg_len,gl_fd[i].data_len,gl_fd[i].status);
		    	 	  	 	  
	    	 	  	 	 }
	    	 	  	 	 if( gl_fd[i].status ==  4)
	    	 	  	 	 {
	    	 	  	
		    	 	  	 	  if( gl_fd[i].len_type == 1 && gl_fd[i].buf[2] !=0x60 )
		    	 	  	 	  {
		    	 	  	 	  		gl_fd[i].remark[0]=0x31;//指示终端有两个包头长度
		    	 	  	 	  		if( iso_mode )
		    	 	  	 	  		  set_sockfd(gl_fd[i].buf+2,gl_fd[i].msg_len,i);
		    	 	  	 	  		else
		    	 	  	 	  		{
		    	 	  	 	  				memset(tmp,0,sizeof(tmp));
/*						    	 	  	 	  sprintf(tmp,"%04d",i);
						    	 	  	 	  memcpy(tmp+4,tmp+2,2);
						    	 	  	 	  memcpy(tmp+6,tmp,2);
						    	 	  	 	  memcpy(gl_fd[i].buf+2+2+1,tmp+4,4);
*/
														tmp[0]=(unsigned char )(i/256);
                            tmp[1]=(unsigned char )(i%256);
														gs_tpdu++;
														if( gs_tpdu >65530) gs_tpdu =0;
														tmp[2]=(unsigned char )(gs_tpdu/256);
                            tmp[3]=(unsigned char )(gs_tpdu%256);
//														memcpy(tmp+2,(unsigned char *)&gs_tpdu,2);
                            memcpy( gl_fd[i].key,tmp+2,2);
						    	 	  	 	  memcpy(gl_fd[i].buf+2+2+1,tmp,4);
		    	 	  	 	  		}	
		    	 	  	 	  }
		    	 	  	 	  else
			    	 	  	 	{
			    	 	  	 		gl_fd[i].remark[0]=0x30;//指示终端有一个包头长度	
			    	 	  	 	  if( iso_mode)
			    	 	  	 	  	set_sockfd(gl_fd[i].buf+gl_fd[i].len_len,gl_fd[i].msg_len,i);
			    	 	  	 	  else
	    	 	  	 	  		{
	    	 	  	 	  				memset(tmp,0,sizeof(tmp));
/*					    	 	  	 	  sprintf(tmp,"%04d",i);
					    	 	  	 	  memcpy(tmp+4,tmp+2,2);
					    	 	  	 	  memcpy(tmp+6,tmp,2);
					    	 	  	 	  memcpy(gl_fd[i].buf+gl_fd[i].len_len+1,tmp+4,4);
*/
//                          memcpy(tmp,(unsigned char *)&i,2);
                          tmp[0]=(unsigned char )(i/256);
                          tmp[1]=(unsigned char )(i%256);
													gs_tpdu++;
													if( gs_tpdu >65530) gs_tpdu =0;
//													memcpy( gl_fd[i].key,(unsigned char *)&gs_tpdu,2);
													tmp[2]=(unsigned char )(gs_tpdu/256);
                          tmp[3]=(unsigned char )(gs_tpdu%256);
                          memcpy( gl_fd[i].key,tmp+2,2);
//													memcpy(tmp+2,(unsigned char *)&gs_tpdu,2);
													dcs_debug(tmp,4,"<%s> gs_tpdu=[%d],i=[%d]",__FUNCTION__,gs_tpdu,i);
						    	 	  	 	memcpy(gl_fd[i].buf+gl_fd[i].len_len+1,tmp,4);
	    	 	  	 	  		}
	    	 	  	 	  		if( gs_xft)
	    	 	  	 	  		{
	    	 	  	 	  			unsigned char x_buf[2048];
	    	 	  	 	  			memcpy(x_buf,gl_fd[i].buf,gl_fd[i].data_len);
	    	 	  	 	  			gl_fd[i].buf[0]=(x_buf[0]*256+x_buf[1]+33)/256;
	    	 	  	 	  			gl_fd[i].buf[1]=(x_buf[0]*256+x_buf[1]+33)%256;
	    	 	  	 	  			memcpy(gl_fd[i].buf+2,x_buf+2,5);
	    	 	  	 	  			memcpy(gl_fd[i].buf+7,"\x4C\x52\x49\x00\x1C", 5);
	    	 	  	 	  			sprintf(gl_fd[i].buf+12,"%-28.28s",gl_fd[i].ip);
	    	 	  	 	  			memcpy(gl_fd[i].buf+40,x_buf+7,gl_fd[i].data_len-7);
	    	 	  	 	  			gl_fd[i].data_len +=33;
	    	 	  	 	  		}	
			    	 	  	 	}
			    	 	  	 	if( gl_fd[gl_sock[gl_fd[i].ndx]].status)
			    	 	  	 	{   	 	  	 	   
			    	 	  	 	  if( gl_sock[gl_fd[i].ndx] >0)
			    	 	  	 	  {
				    	 	  	 	  if( gl_fd[i].remark[0]== 0x31  )
				    	 	  	 	  	len = write(gl_sock[gl_fd[i].ndx],gl_fd[i].buf+2,gl_fd[i].data_len-2);
				    	 	  	 	  else
				    	 	  	 	  	len = write(gl_sock[gl_fd[i].ndx],gl_fd[i].buf,gl_fd[i].data_len);
			    	 	  	 	  }
			    	 	  	 	  else len=0;
		    	 	  	 	  }
		    	 	  	 	  else
		    	 	  	 	  	 len =0;
		    	 	  	 	  if ( len <=0 )
		    	 	  	 	  {
//		    	 	  	 	  	close(gl_sock[gl_fd[i].ndx]);
		    	 	  	 	  	close(gl_fd[i].sockfd);
		    	 	  	 	  	FD_CLR(gl_fd[i].sockfd,&read_set);
		    	 	  	 	    FD_CLR(gl_fd[i].sockfd,&error_set);
		    	 	  	 	    gl_fd[i].used=0x00;
//		    	 	  	 	  	gl_fd[gl_sock[gl_fd[i].ndx]].used=0x00;
//		    	 	  	 	  	FD_CLR(gl_sock[gl_fd[i].ndx],&read_set);
//		    	 	  	 	    FD_CLR(gl_sock[gl_fd[i].ndx],&error_set);
//		    	 	  	 	    gl_sock[gl_fd[i].ndx]= -1;
		    	 	  	 	  	dcs_log(0,0," write posp data error!soket=[%d][%s:%d]",gl_sock[gl_fd[i].ndx],gl_fd[gl_sock[gl_fd[i].ndx]].ip,gl_fd[gl_sock[gl_fd[i].ndx]].port);
		    	 	  	 	  	//需添加与POSP链接链路重连代码
//		    	 	  	 	  	if (connect_sock(gl_sock[gl_fd[i].ndx],&max_fd)<0)
//								       	_srv_exit(0); 	
                      goto cri_loop;
		    	 	  	 	  }
		    	 	  	 	  gl_fd[i].data_len=0;
		    	 	  	 	  gl_fd[i].msg_len=0;
		    	 	  	 	  if ( crypt_flag ) gl_fd[i].status =3;
		    	 	  	 	  else gl_fd[i].status=1;
		    	 	  	 	  if( gl_fd[i].remark[0]== 0x31  )
		    	 	  	 	    dcs_debug(gl_fd[i].buf+2,len ,"send posp data len=%d remark[0]=%02x",len,gl_fd[i].remark[0]);
		    	 	  	 	  else
		    	 	  	 	  	dcs_debug(gl_fd[i].buf,len ,"send posp data len=%d remark[0]=%02x",len,gl_fd[i].remark[0]);
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
	    	 	  	 	    //需添加与POSP链接链路重连代码
	    	 	  	 	    if( gl_fd[i].type ==1 )
	    	 	  	 	    {
		    	 	  	 	    if (connect_sock(i,&max_fd)<0)
								       	_srv_exit(0); 	
							      }
	    	 	  }
cri_loop:
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
