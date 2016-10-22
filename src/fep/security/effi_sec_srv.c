 #define _MainProg

// 程序名称：secuSrv.c
// 功能描述：
//          安全管理系统服务通讯程序
// 函数清单：
//           main()

	
#include  "dccdcs.h"
#include  "effi_sec.h"
#include <pthread.h>
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

#define DCS_SEC_NAME "effi_sec.conf"

static  struct EFFI_SEC_SRV *g_srv = NULL;

// Functions declaration
void SecSrvExit(int signo);
int SecSrvOpenLog();
void  net_to_queue(struct EFFI_SEC_SRV *srv);
void  queue_to_net(struct EFFI_SEC_SRV *srv);
void comm_build_proc(struct EFFI_SEC_SRV * srv, int index,fd_set *wset,fd_set *rset,fd_set *eset,int *max_fd);
void comm_read_proc(struct EFFI_SEC_SRV * srv, int index,fd_set *wset,fd_set *rset,fd_set *eset,int *max_fd);
void comm_error_proc(struct EFFI_SEC_SRV * srv, int index,fd_set *wset,fd_set *rset,fd_set *eset,int *max_fd);
void reconnection(struct EFFI_SEC_SRV * srv,fd_set *wset,fd_set *eset,int *max_fd );
int response_busy_error(struct EFFI_SEC_SRV * srv,unsigned char *buf, int len);
int add_machine(struct EFFI_SEC_SRV *srv,fd_set *wset,int *max_fd, char *ip,int port,int max_link,int min_link,int timeout,int type ,unsigned char *buf);
int del_machine(struct EFFI_SEC_SRV *srv,fd_set *wset,fd_set *rset,int *max_fd,int index,unsigned char *buf);
int del_link(struct EFFI_SEC_SRV *srv,fd_set *wset,fd_set *rset,int *max_fd,int index,unsigned char *buf );
int add_link(struct EFFI_SEC_SRV *srv,fd_set *wset,int *max_fd,int index,unsigned char *buf);

int query_links(struct EFFI_SEC_SRV *srv,int index,unsigned char *buf,int size);
int query_srv_info(struct EFFI_SEC_SRV *srv,unsigned char *buf,int size);
int query_machine(struct EFFI_SEC_SRV *srv,unsigned char *buf,int size);
int response_abnormal_error(struct EFFI_SEC_SRV * srv,unsigned char *buf, int size_len);
int response_discard_error(struct EFFI_SEC_SRV * srv,unsigned char *buf, int size_len);
static int sec_srv_stop=0;
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
    int   sock,flags;
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

   flags=fcntl(sock,F_GETFL);
   fcntl(sock,F_SETFL,flags|O_NDELAY);
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
 //   if(connect(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0)
 //       goto lblError;
    connect(sock,(struct sockaddr *)&sin,sizeof(sin));
    return sock;

lblError:
    if(sock >= 0)
        close(sock);
    return -1;
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

void add_to_set(int fd, fd_set *set, int *max_fd) {
  FD_SET(fd, set);
  if (fd >  *max_fd) {
    *max_fd = (int) fd;
  }
}
int main(int argc, char *argv[])
{
	int i;
struct EFFI_SEC_SRV srv;	
	
  pthread_t sthread;
  catch_all_signals(SecSrvExit);


    memset(&srv,0,sizeof(srv));
  	//prepare the log stuff
  	if (0 > SecSrvOpenLog())
    		exit(0);
   
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",strerror(errno),__FILE__,__LINE__);
        exit(0);
    }

   for ( i=1; i<argc ;i++)
       if(memcmp("-v",argv[i],2)==0 )
			 {
			 	  
				 	  fprintf(stderr,"\n Encryption server version 2.0-%s %s release \n",__DATE__,__TIME__);
				 	  return;
			 }
    g_srv=&srv;
    if (!load_config(&srv))
  	{
  		  dcs_log(0,0, "<%s> Failure to load config !",__FUNCTION__ );
//  		  fprintf(stderr,"<%s> Failure to load secsrvlnk.ini !",__FUNCTION__ );
    		goto lblExit;
  	}	
    dcs_debug(0,0,"load_config succ!");
//    fprintf(stderr,"load_config succ!");
  	srv.recv_qid = _queue_create(srv.queue_rcv_name);
		if ( srv.recv_qid <0)
	  	srv.recv_qid= _queue_connect(srv.queue_rcv_name)	;
	  if ( srv.recv_qid <0)
	  {	
	  		dcs_log(0,0,"Can not connect recv message queue !");
	  		goto lblExit; 
	  }
  	srv.send_qid = _queue_create(srv.queue_snd_name);
		if ( srv.send_qid <0)
	  	srv.send_qid= _queue_connect(srv.queue_snd_name)	;
	  if ( srv.send_qid <0)
	  {	
	  		dcs_log(0,0,"Can not connect send message queue !");
	  		goto lblExit; 
	  }
//	  dcs_debug(0,0,"<%s> system queue ready!",__FUNCTION__);
	  if(0> effi_init_queue(&srv.data_queue,1024*128))
	  {
	  		dcs_log(0,0,"Can not create effi data queue!");
	  		goto lblExit;
	  }
		if ( pthread_create(&sthread, NULL,( void * (*)(void *))net_to_queue,&srv) !=0)
		{
				dcs_log(0,0, "Can not create thread ");
				goto lblExit;
		}
    queue_to_net(&srv);

lblExit:

  	SecSrvExit(0);
}//main()

/*
  函数名称：MoniTcpipExit
  参数说明：
           signo：捕获的信号量
  返 回 值：
           无
  功能描述：
           当捕获的信号为SIGTERM时，该函数释放资源后终止当前进程；否则不做任何处理
*/

void SecSrvExit(int signo)
{
	int i;
  	dcs_debug(0,0,"<%s> catch a signal %d !",__FUNCTION__, signo);

  
  	//parent process
  	switch (signo)
  	{
    	case SIGUSR2: //monimnt want me to stop
     		break;

    	case 0:
    	case SIGTERM:
      		break;

    	case SIGCHLD:
					return;
      case 13:
      	  return;
    	default:
      		
	  	      break;
			 
		}
		if( g_srv )
		{
	    if( g_srv->send_qid >=0)  queue_delete(g_srv->send_qid);
	    if( g_srv->recv_qid >=0)  queue_delete(g_srv->recv_qid);
	    if( g_srv->data_queue.buf != NULL ) free(g_srv->data_queue.buf);
	    if( g_srv->machine_info != NULL ) free( g_srv->machine_info);
	    if( g_srv->link != NULL )
	    {
	    	for(i=0 ; i< g_srv->used_link; i++) close(g_srv->link[i].iSocketId);
	    	free( g_srv->link );
	    }
    }
  	exit(signo);
}

/*
  函数名称：SecSrvOpenLog
  参数说明：
           无
  返 回 值：
           0：成功
           -1：失败
  功能描述：
           1.获得系统主目录,将之放在全局变量g_pcIseHome
           2.读取系统配置文件system.conf
           3.设置日志文件和日志方式
*/

int SecSrvOpenLog()
{
  	char   caLogName[256];
    char logfile[400];
  
    memset(caLogName,0,sizeof(caLogName));
    strcpy(caLogName,"log/encrypt.log");
    if(u_fabricatefile(caLogName,logfile,sizeof(logfile)) < 0)
    {
            return -1;
    }
    dcs_log_open(logfile, "Encrypt");

 	return 0;
}

int get_free_link(struct EFFI_SEC_SRV *srv, unsigned char type )
{
	int i =0;
	if ( srv ==  NULL ) return -1;
	i=srv->curr_link;
//	fprintf(stderr,"<%s>curr  link =%d used_link=%d",__FUNCTION__,i,srv->used_link);
	while(1)
	{
		if( !srv->link[srv->curr_link].iWorkFlag && //空闲状态
		   srv->link[srv->curr_link].iStatus==2 &&   //就绪状态
		   (unsigned char)(srv->machine_info[srv->link[srv->curr_link].ip_num].encType &0xE0 )  == (unsigned char)(type & 0xE0) && // 符合预期的加密机指令所在服务机
			 (unsigned char)(srv->machine_info[srv->link[srv->curr_link].ip_num].encType &0x1F )  >=(unsigned char)(type & 0x1F) &&
			  srv->link[srv->curr_link].iRole == 0 ) //业务操作者类型
	  { srv->link[srv->curr_link].iWorkFlag = 1; return srv->curr_link;} 
		
		srv->curr_link++;
		if( srv->curr_link == i) return -1;
		if( srv->curr_link == srv->used_link )
		{	
			 srv->curr_link = 0;
			 if( i == 0) break;
		}
	}	
	return -1;
}
void queue_to_net(struct EFFI_SEC_SRV *srv)
{
	
//	struct MSG_STRU *pMsg;
	char caBuf[4096];
	int link_index,len,sock;
	time_t t;
	 dcs_debug(0,0,"<%s> begin",__FUNCTION__);

	while(1)
	{
		  if(sec_srv_stop) break;
		  memset(caBuf,0,30);
		  len = queue_recv( srv->recv_qid, caBuf, sizeof(caBuf), 0);
			if ( len <0)
			{			
				dcs_log(0,0,"<%s>Recive Message fail!",__FUNCTION__);
			  break;		
			}
//			fprintf(stderr,"\n rcv msg len=%d\n",len);
			dcs_debug(caBuf,len,"<%s> rcv msg len=%d",__FUNCTION__,len);
			srv->accept_cnt++;
			if( len < sizeof(long)+sizeof(time_t)+1+4)
			{
				dcs_log(caBuf,len,"<%s>recv msg Too short ,discard! ",__FUNCTION__);
				srv->discard_cnt++;
				response_discard_error(srv,caBuf,30);
				continue; // 判断为过时消息，丢弃！
			}
			memcpy(&t,caBuf+9,sizeof(time_t));
			if( time(NULL)- t > 8)
			{
				dcs_log(0,0,"<%s>recv expired data ,discard! ",__FUNCTION__);
				srv->discard_cnt++;
				response_discard_error(srv,caBuf,len);
				continue; // 判断为过时消息，丢弃！
			}
		
			if( srv->work_links <=0) {dcs_log(0,0,"<%s>work_links <=0",__FUNCTION__); response_busy_error(srv,caBuf,len); continue;}// 返回系统忙
			
			
//			dcs_debug(0,0,"<%s> pre lock",__FUNCTION__);
			pthread_mutex_lock(&srv->mutex);
//			dcs_debug(0,0,"<%s> lock succ",__FUNCTION__);
			link_index=get_free_link(srv ,caBuf[sizeof(long)]);
//			dcs_debug(0,0,"link_index=%d",link_index);
			if( link_index <0) //获取空闲链路
			{
				if( !effi_put_queue(&srv->data_queue, caBuf ,len)) //判断缓冲队列是否已满
				{
					// 返回系统忙
					pthread_mutex_unlock(&srv->mutex);
//					dcs_debug(0,0,"<%s> unlock succ",__FUNCTION__);
					dcs_log(0,0,"<%s> effi_put_queue fail !",__FUNCTION__);
					response_busy_error(srv,caBuf,len);
					continue;
				}
//				put_cnt++;
//				dcs_debug(0,0,"<%s>effi_put_queue ,send_time=%d",__FUNCTION__,t);
				pthread_mutex_unlock(&srv->mutex);
//				dcs_debug(0,0,"<%s> unlock succ",__FUNCTION__);
				continue;
			}	
			//存储数据关键字
			memcpy( srv->link[link_index].data_key,caBuf,sizeof(long)+1+sizeof(time_t)+2);
			sock=srv->link[link_index].iSocketId;
			srv->link[link_index].work_begin_time = time(NULL);
			srv->link[link_index].iStatus= 3;
//			pthread_mutex_unlock(&srv->mutex);
//			dcs_debug(0,0,"<%s> unlock succ",__FUNCTION__);
			//发送数据	
			while(1)
			{
				if( (len= send(sock,caBuf+sizeof(long)+1+sizeof(time_t),len -sizeof(long)-1-sizeof(time_t),0))<=0)
				{
					if (errno == EINTR || errno == EAGAIN) continue;
					dcs_log(0,0,"<%s> send fail ! errno=%d,%s",__FUNCTION__,errno,strerror(errno));
					response_abnormal_error(srv, caBuf ,30 );
					break;
				}
//				pthread_mutex_lock(&srv->mutex);
//				dcs_debug(0,0,"<%s> lock succ",__FUNCTION__);
//				srv->count++;
//	      srv->link[link_index].server_count++;
//				dcs_debug(0,0,"<%s> unlock succ",__FUNCTION__);
				break;
		 }
		 pthread_mutex_unlock(&srv->mutex);	
//		 dcs_debug(0,0,"<%s>send ok!",__FUNCTION__);			  
	}
}


void  net_to_queue(struct EFFI_SEC_SRV *srv)
{
	fd_set read_set,write_set,error_set,vread_set,vwrite_set,verror_set;
	struct timeval tv;
	unsigned char buffer[8192],tmp[256];
	int max_fd,ret,i,j,k,sock,len, slot_free;
	time_t t;
	FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&error_set);
  max_fd=0;
 
  dcs_debug(0,0,"<%s> begin",__FUNCTION__); 
  for( i=0; i< srv->used_machine;i++)
  {
  	 
  	 if( srv->machine_info[i].encType == 0)
  	 {
/*  	 			 fprintf(stderr,"<%s>type=%d,ip=%s,port=[%d],timeout=%d,min_link=%d,max_link=%d \n",__FUNCTION__,
                 srv->machine_info[i].encType,
                 srv->machine_info[i].remoteIp,
                 srv->machine_info[i].remotePort,
                 srv->machine_info[i].timeout,
                 srv->machine_info[i].link_num,
                 srv->machine_info[i].max_link_num
                 );
*/
   	 			 sock = tcp_open_server( NULL, srv->machine_info[i].remotePort);
  	 			 if(sock < 0){ fprintf(stderr,"open server fail !sock=%d err[%d][%s]\n",sock,errno,strerror(errno)); goto lError;}
 // 	 			 fprintf(stderr,"open server sock=%d\n",sock);
  	 			 add_to_set(sock, &read_set, &max_fd);
  	 			 srv->link[srv->used_link].iRole = 1;     //该link在连接建立过程中的角色 0 加密工作线路 1 监控侦听线路 2监控工作线路 
           srv->link[srv->used_link].iStatus = 2;   //该link当前 的状态 0 链路停止状态 1链路在建状态 2 链路就绪状态 3数据发送状态 4数据结束状态
           srv->link[srv->used_link].iWorkFlag = 0; //0 空闲状态 1工作状态
           srv->link[srv->used_link].ip_num  = i;
           srv->link[srv->used_link].iSocketId = sock;
           srv->link[srv->used_link].iTimeOut = 0;    //超时时间
           srv->link[srv->used_link].build_time = time(NULL);  // 端口创建时间
           srv->link[srv->used_link].server_count = 0 ; //链路服务次数
           srv->link[srv->used_link].expected_len =0; //期望数据的长度
           srv->link[srv->used_link].reality_len =0 ; //实际数据的长度
           srv->used_link++;
           srv->machine_info[i].undone_link_num=0;
           srv->machine_info[i].link_num=0;
           continue;
  	 }
  	 for( j=0; j<srv->machine_info[i].link_num;j++)
  	 {
  	 			 sock = tcp_connet_server( srv->machine_info[i].remoteIp, srv->machine_info[i].remotePort,0);
  	 			 if(sock < 0) goto lError;
/*  	 			 	fprintf(stderr,"<%s>type=%d,ip=%s,port=[%d],timeout=%d,min_link=%d,max_link=%d \n",__FUNCTION__,
                 srv->machine_info[i].encType,
                 srv->machine_info[i].remoteIp,
                 srv->machine_info[i].remotePort,
                 srv->machine_info[i].timeout,
                 srv->machine_info[i].link_num,
                 srv->machine_info[i].max_link_num
                 );
*/
  	 			 add_to_set(sock, &write_set, &max_fd);
  	 			 srv->link[srv->used_link].iRole = 0;     //该link在连接建立过程中的角色 0 加密工作线路 1 监控侦听线路 2监控工作线路 
           srv->link[srv->used_link].iStatus = 1;   //该link当前 的状态 0 链路停止状态 1链路在建状态 2 链路就绪状态 3数据发送状态 4数据结束状态
           srv->link[srv->used_link].iWorkFlag = 0; //0 空闲状态 1工作状态
           srv->link[srv->used_link].ip_num  = i;
           srv->link[srv->used_link].iSocketId = sock;
           srv->link[srv->used_link].iTimeOut = srv->machine_info[i].timeout;    //超时时间
           srv->link[srv->used_link].build_time = time(NULL);  // 端口创建时间
           srv->link[srv->used_link].server_count = 0 ; //链路服务次数
           srv->link[srv->used_link].expected_len =0; //期望数据的长度
           srv->link[srv->used_link].reality_len =0; //实际数据的长度
           srv->build_count++;
           srv->used_link++;
//           srv->machine_info[i].link_num++;
           srv->machine_info[i].undone_link_num--;  // 需要重连计数器调整  	 		
  	 }
  }
  while(1)
  {
    if(sec_srv_stop) break;
  	tv.tv_sec = 0;
    tv.tv_usec = 200000;
    vread_set=read_set;
    vwrite_set=write_set;
    verror_set=error_set;
    ret= select(max_fd + 1, &vread_set, &vwrite_set, &verror_set, &tv);
 	  if ( ret <0 )
 	  {
 	  		fprintf(stderr,"<%s> select error[%s]",__FUNCTION__,strerror(errno));
 	  		continue;
 	  }
 	  else if( ret == 0)
 	  {
 	  	
 	  	reconnection(srv,&write_set,&error_set,&max_fd);
 	  	//判断各工作链路的超时处理
 	  	t=time(NULL);
 	  	pthread_mutex_lock(&srv->mutex);
 	  	slot_free =0;
 	  	for( i=0; i<srv->used_link; i++)
 	  	{
 	  		 if( srv->link[i].iRole == 0 && srv->link[i].iWorkFlag  ) //加密工作链路
 	  		 {
 	  		 	  if( t > srv->link[i].work_begin_time + srv->link[i].iTimeOut )
 	  		 	  {
 	  		 	  	close( srv->link[i].iSocketId);
 	  		 	  	FD_CLR(srv->link[i].iSocketId,&read_set);
 	  		 	  	FD_CLR(srv->link[i].iSocketId,&write_set);
 	  		 	  	FD_CLR(srv->link[i].iSocketId,&error_set);
 	  		 	  	if(srv->link[i].iSocketId == max_fd) max_fd--;
 	  		 	  	dcs_log(0,0,"<%s>  time out socket id=[%d],begin_time=%d,timeout=%d ",__FUNCTION__,srv->link[i].iSocketId,srv->link[i].work_begin_time , srv->link[i].iTimeOut);
 	  		 	  	response_queue_error(srv,i,"FF");
 	  		 	  	link_minus_op(srv,i);
 	  		 	  	srv->work_links--;
 	  		 	  	
 	  		 	  }
 	  		 }
 	  		 else if( srv->link[i].iRole == 2 && srv->link[i].iTimeOut >0) // 监控工作链路
 	  		 {
 	  		 	  if( t > srv->link[i].work_begin_time + srv->link[i].iTimeOut )
 	  		 	  {
 	  		 	  	close( srv->link[i].iSocketId);
 	  		 	  	FD_CLR(srv->link[i].iSocketId,&read_set);
 	  		 	  	FD_CLR(srv->link[i].iSocketId,&write_set);
 	  		 	  	FD_CLR(srv->link[i].iSocketId,&error_set);
 	  		 	  	if(srv->link[i].iSocketId == max_fd) max_fd--;
 	  		 	  	dcs_log(0,0,"<%s> monitor link close for timeout",__FUNCTION__);
 	  		 	  	link_minus_op(srv,i);
 	  		 	  }
 	  		 }
 	  		 if( !slot_free)
 	  		 {	if( srv->link[i].iRole == 0 && !srv->link[i].iWorkFlag && srv->link[i].iStatus == 2) slot_free =1;}
 	  	}

 	  	
//	  	 dcs_debug(0,0,"<%s> data_queue num=%d",__FUNCTION__,srv->data_queue.message_num);
 	  	if( slot_free )
 	  		len=effi_get_queue(&srv->data_queue, buffer ,sizeof(buffer));
 	  	else 
 	  		len = effi_uget_queue(&srv->data_queue, buffer ,sizeof(buffer));
 	  	pthread_mutex_unlock(&srv->mutex);
 	  	
 	  	if( len >0 )
			{
				
				int index;
//				dcs_debug(0,0,"<%s> effi_get_queue proc begin",__FUNCTION__);
				memcpy(&t,buffer+9,sizeof(time_t));;
//			  get_cnt++;
			  dcs_debug(0,0,"<%s> len=[%d] slot_free=%d",__FUNCTION__,len ,slot_free);
				if( time(NULL) > t +2 ) // 消息在data队列内超时
				{

					dcs_debug(buffer,len,"<%s> effi_get_queue proc time out",__FUNCTION__);
					if( !slot_free)
					{
							pthread_mutex_lock(&srv->mutex);
							len=effi_get_queue(&srv->data_queue, buffer ,sizeof(buffer));
							pthread_mutex_unlock(&srv->mutex);
					}
					response_busy_error(srv,buffer,len);
					
					continue;
				}
				if( !slot_free ) continue;
				pthread_mutex_lock(&srv->mutex);  
			  index = get_free_link(srv ,buffer[sizeof(long)]);
			  pthread_mutex_unlock(&srv->mutex);
			  if ( index <0 )
			  {
			  	dcs_debug(buffer,len,"<%s> effi_get_queue proc get_free_link fail type=%d",__FUNCTION__,buffer[sizeof(long)]);
			  	response_busy_error(srv,buffer,len);
			  	continue;
			  }
				srv->link[index].work_begin_time = time(NULL); 			
				memcpy(srv->link[index].data_key,buffer,sizeof(long)+1+sizeof(time_t)+2);
				srv->link[index].reality_len= len -sizeof(long)-1-sizeof(time_t);
				memcpy(srv->link[index].data_buf,buffer+sizeof(long)+1+sizeof(time_t),len -sizeof(long)-1-sizeof(time_t));
				srv->link[index].iStatus = 3; //数据发送状态
				while(1)
				{	 
//					 dcs_debug(0,0,"<%s>accept count=%d",__FUNCTION__,srv->accept_cnt);
					 len =  send(srv->link[index].iSocketId,buffer+sizeof(long)+1+sizeof(time_t),srv->link[index].reality_len,0);
					 if( len <=0 )
					 {
					 		if (errno == EINTR || errno == EAGAIN) continue;
					 		dcs_debug(0,0,"<%s> send fail ! [%s]",__FUNCTION__,strerror(errno));
					 		close( srv->link[index].iSocketId );
					 		if( srv->link[index].iSocketId == max_fd ) max_fd--;
					 		FD_CLR( srv->link[index].iSocketId,&read_set);
					 		FD_CLR(srv->link[index].iSocketId,&write_set);
					 		FD_CLR(srv->link[index].iSocketId,&error_set);
					 	  response_abnormal_error(srv, buffer ,30 );
					 		pthread_mutex_lock(&srv->mutex);
					 		link_minus_op(srv, index);
					 		pthread_mutex_unlock(&srv->mutex);
							srv->work_links--;
							
							break;
					 }
					 srv->link[index].iStatus = 4;
					 srv->link[index].reality_len = 0;
			     srv->link[index].expected_len = 0;
					 break;
				}
				
				dcs_debug(0,0,"<%s> effi_get_queue proc end",__FUNCTION__);
			}

 	  	continue;
 	  }
 //	  dcs_debug(0,0,"<%s> ret =%d",__FUNCTION__,ret);
 	  for(i=0; ret>0 && i <srv->used_link;i++)
 	  {
 	  	if( FD_ISSET(srv->link[i].iSocketId,&vwrite_set))
 	  	{
 	  		comm_build_proc(srv, i,&write_set,&read_set,&error_set,&max_fd);
 	  		ret--;
 	  	}
 	  	else if( FD_ISSET(srv->link[i].iSocketId,&vread_set))
 	  	{
 	  		comm_read_proc(srv, i,&write_set,&read_set,&error_set,&max_fd);
// 	  		dcs_debug(0,0,"comm_read_proc end");
 	  		ret--;
 	  	}
 	  	else if( FD_ISSET(srv->link[i].iSocketId,&verror_set))
 	  	{
 	  		comm_error_proc(srv, i,&write_set,&read_set,&error_set,&max_fd);
 	  		ret--;
 	  	}
 	  }
  }
lError:
  for( i=0; i<srv->used_link ; i++) close(srv->link[i].iSocketId);
  
  return;
}

int load_config (struct EFFI_SEC_SRV *srv)
{
		char   caFileName[256],caBuffer[512];
  	int    iFd=-1,iCommCnt,iRc,i;  
  	char *pEncType,*pAddr,*pPort,*pMaxNum,*pMinNum,*pTime;
	if( srv ==  NULL ) return 0;
	
  	
  	//设定commlnk定义文件的绝对路径，并打开
  	
  	sprintf(caFileName, "%s/config/%s",getenv("ICS_HOME"),DCS_SEC_NAME);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log( 0,0, "<%s> open <%s>fail ! ",__FUNCTION__,caFileName);
				return 0;
  	}
    srv->max_machine=0;
    srv->used_machine=0;
    if ( 0 >conf_get_first_number(iFd, "comm.max",&iCommCnt) || iCommCnt < 0 )
    {
       dcs_log(0,0, "<%s> load configuration item 'comm.max' failed!",__FUNCTION__);
       conf_close(iFd);
       return (0);
    }
    memset(caBuffer,0,sizeof(caBuffer));
    if( 0 != conf_get_first_string(iFd, "queue.recv",caBuffer))
    	 strcpy(srv->queue_rcv_name,"1000");
    else
    	 strcpy(srv->queue_rcv_name,caBuffer);
    memset(caBuffer,0,sizeof(caBuffer));
    if( 0 != conf_get_first_string(iFd, "queue.send",caBuffer))
    	 strcpy(srv->queue_snd_name,"1001");
    else
    	 strcpy(srv->queue_snd_name,caBuffer);
    if( iCommCnt >100) srv->max_machine=100;
    else  srv->max_machine=iCommCnt;
    if((srv->machine_info =(struct sec_machine_info *) malloc(sizeof(struct sec_machine_info) * srv->max_machine))== NULL )
    {	
    	 dcs_log(0,0, "<%s> malloc max machine memory cache failed!",__FUNCTION__);
       conf_close(iFd);
       return (0);
    }
    memset(srv->machine_info,0,sizeof(struct sec_machine_info) * srv->max_machine);
    for( iRc = conf_get_first_string(iFd, "comm",caBuffer),i=0;\
         iRc == 0 && i<srv->max_machine;\
         iRc = conf_get_next_string(iFd, "comm",caBuffer),i++)
  	{

          pEncType    = strtok(caBuffer," \t\r");  //机器类型
          pAddr       = strtok(NULL   ," :\t\r");  //地址串
          pPort       = strtok(NULL   ," \t\r");  //端口号  
          pTime        = strtok(NULL   ," \t\r\n");//超时检测时间
          pMaxNum     = strtok(NULL," \t\r"); //通讯链路最大数
  				pMinNum    = strtok(NULL," \t\r"); //通讯链路最小数
          if (pEncType==NULL || pAddr==NULL || pTime==NULL || pMaxNum==NULL || pMinNum==NULL ||
               pPort==NULL  )
          {
                  //invalid address:skip this line
               i--;
               dcs_log(0,0,"<%s> [%d] is invalid !",__FUNCTION__,i);
               continue;
          }
          srv->machine_info[i].encType =atoi(pEncType);  
          strcpy(srv->machine_info[i].remoteIp,pAddr);//期望的远端的IP地址
          srv->machine_info[i].remotePort=atoi(pPort);   //期望的远端的端口号
          srv->machine_info[i].timeout=atoi(pTime);
          srv->machine_info[i].link_num=atoi(pMinNum);    //需要建立的链路数
          srv->machine_info[i].undone_link_num = atoi(pMinNum); //未完成创建的链路数
          srv->machine_info[i].max_link_num=atoi(pMaxNum); //最大能建立的链路数
          srv->max_link += srv->machine_info[i].max_link_num;
 //         srv->used_link += srv->machine_info[i].link_num   //系统启动时需要建的链路
          srv->used_machine++;
/*         fprintf(stderr,"\ntype=%d,ip=%s,port=%d,timeout=%d,min_link=%d,max_link=%d \n",
                 srv->machine_info[i].encType,
                 srv->machine_info[i].remoteIp,
                 srv->machine_info[i].remotePort,
                 srv->machine_info[i].timeout,
                 srv->machine_info[i].link_num,
                 srv->machine_info[i].max_link_num
                 );*/
  	}
  	
  	conf_close(iFd);
  	
  	if( srv->max_link < srv->max_machine *10 ) srv->max_link =  srv->max_machine *10;
  	if((srv->link = malloc(sizeof(struct sec_link_info) * srv->max_link))== NULL )
    {	
    	 dcs_log(0,0, "<%s> malloc max machine memory cache failed!",__FUNCTION__);
       conf_close(iFd);
       return (0);
    }
    memset(srv->link,0,sizeof(struct sec_link_info) * srv->max_link);
//    srv->mutex = PTHREAD_MUTEX_INITIALIZER ;
    pthread_mutex_init(&srv->mutex,NULL);
//    fprintf(stderr,"<%s> end q_rcv_name=[%s],q_snd_name=[%s]\n",__FUNCTION__,srv->queue_rcv_name,srv->queue_snd_name);
	return 1;
}

void comm_build_proc(struct EFFI_SEC_SRV * srv, int index,fd_set *wset,fd_set *rset,fd_set *eset,int *max_fd)
{
	int len,error,machine_index;
	char buffer[4096];
	if( srv == NULL ) return;
	if(index >= srv->used_link || index <0 ) return;
//	dcs_debug(0,0,"<%s> begin",__FUNCTION__);	
	len=sizeof(error);
  getsockopt(srv->link[index].iSocketId,SOL_SOCKET,SO_ERROR,&error,&len) ;
     
  FD_CLR(srv->link[index].iSocketId,wset);
  if( 0 != error )
  { 
     close(srv->link[index].iSocketId);
     FD_CLR(srv->link[index].iSocketId,wset);
     FD_CLR(srv->link[index].iSocketId,rset);
     if( srv->link[index].iSocketId == *max_fd ) *max_fd--;
     machine_index= srv->link[index].ip_num;
     srv->machine_info[srv->link[index].ip_num].undone_link_num++;  // 需要重连计数器调整
     if( index != srv->used_link -1) srv->link[index]=srv->link[srv->used_link-1];
     srv->used_link--;
//     dcs_debug(0,0,"used_link=%d index=%d",srv->used_link,index);
//     dcs_log(0,0,"<%s>connect server socket error!addr=[%s]:[%d][%s]",__FUNCTION__,srv->machine_info[machine_index].remoteIp,srv->machine_info[machine_index].remotePort,strerror(errno));	
     
  }
  else
  {
  	
  	 add_to_set(srv->link[index].iSocketId,rset,max_fd);

  	 srv->work_links++;
//  	  dcs_debug(0,0,"<%s> data_queue num=%d",__FUNCTION__,srv->data_queue.message_num);

  	 	pthread_mutex_lock(&srv->mutex);
  	 len = effi_get_queue(&srv->data_queue, buffer ,sizeof(buffer));
  	 pthread_mutex_unlock(&srv->mutex);

  	if(len  >0 )
		{
      time_t t;
			
			memcpy(&t,buffer+9,sizeof(time_t));
			if( time(NULL) >t +8 ) // 消息在data队列内超时
			{
				response_busy_error(srv, buffer, 30 );
				srv->link[index].iWorkFlag=0;
				srv->link[index].reality_len = 0;
				srv->link[index].expected_len = 0;
				srv->link[index].iStatus=2; //就绪状态
//				srv->abnormal_cnt++;
				dcs_log(0,0,"<%s>effi_get_queue msg time out! send_time=%d , now_time=%d",__FUNCTION__,t,time(NULL));
				return;
			}
			srv->link[index].iWorkFlag=1;
			srv->link[index].work_begin_time = time(NULL); 
			
			memcpy(srv->link[index].data_key,buffer,sizeof(long)+1+sizeof(time_t)+2);
			srv->link[index].reality_len= len -sizeof(long)-1-sizeof(time_t);
			srv->link[index].iStatus = 3; //数据发送状态
			while(1)
			{	 

				 len =  send(srv->link[index].iSocketId,buffer+sizeof(long)+1+sizeof(time_t),srv->link[index].reality_len,0);
				 if( len <=0 )
				 {
				 		if (errno == EINTR || errno == EAGAIN) continue;
				 		dcs_debug(0,0,"<%s> send fail  err=[%s]",__FUNCTION__,strerror(errno));
				 		close( srv->link[index].iSocketId );
				 	  response_abnormal_error(srv, buffer ,30 );
				 		pthread_mutex_lock(&srv->mutex);
				 		link_minus_op(srv, index);
				 		pthread_mutex_unlock(&srv->mutex);
						srv->work_links--;
						
						break;
				 }
				 srv->link[index].iStatus = 4;
//				 srv->count++;
//				 srv->link[index].server_count++;
				 break;
			}
			srv->link[index].reality_len = 0;
			srv->link[index].expected_len = 0;
		}
		
  	srv->link[index].iStatus=2; //就绪状态
  }
}
void comm_read_proc(struct EFFI_SEC_SRV * srv, int index,fd_set *wset,fd_set *rset,fd_set *eset,int *max_fd)
{
	int sock,len,client_addr,client_port,size_len,i;
	unsigned char buffer[4096] ,*str;
//	dcs_debug(0,0,"<%s> begin",__FUNCTION__);	
	if( srv == NULL ) return;
	if(index >= srv->used_link || index <0 ) return;
	
	if( srv->link[index].iRole == 1) //监控侦听线路
	{
		 sock=tcp_accept_client(srv->link[index].iSocketId,&client_addr,&client_port);
		 if ( sock <0 )
		 {
		 	 dcs_log(0,0,"<%s> tcp_accept_client fail! port=[%d] cause is [%s]",__FUNCTION__,srv->link[index].iSocketId,strerror(errno));
		 	 return;
		 }
		 if( srv->used_link >= srv->max_link || srv->machine_info[srv->link[index].ip_num].link_num >= srv->machine_info[srv->link[index].ip_num].max_link_num )
		 {
		 	  send(sock,"310 monitor link slot full !\n",29,0); 
		 	  close(sock);
		 	  dcs_log(0,0,"<%s> all link full( max_link[%d] used_link[%d]) or machine link overflow[max_link[%d] curr_link[%d]] ",
		 	          __FUNCTION__,srv->max_link,srv->used_link,srv->machine_info[srv->link[index].ip_num].max_link_num,srv->machine_info[srv->link[index].ip_num].link_num);
		 	  return;
		 }
		
		 if( link_add_op(srv, index,sock)){ add_to_set(sock, rset, max_fd);}
		 else { send(sock,"310 system link slot full !",27,0); close(sock);}

	}
	else if( srv->link[index].iRole == 2) //监控工作线路
	{
		//
		
		sock=srv->link[index].iSocketId;
		str= buffer;
		len= recv(sock,str,sizeof(buffer),0);
		if( len < 0)
		{
			if (errno == EINTR || errno == EAGAIN) { dcs_debug(0,0,"(errno == EINTR || errno == EAGAIN) errno=%d",errno); return; }
			close(sock);
			FD_CLR(sock,rset);
			FD_CLR(sock,wset);
			FD_CLR(sock,eset);
			if( sock == *max_fd ) *max_fd--;
			pthread_mutex_lock(&srv->mutex);
			link_minus_op(srv, index);
			pthread_mutex_unlock(&srv->mutex);
		}else if ( len == 0)
		{
			close(sock);
			FD_CLR(sock,rset);
			FD_CLR(sock,wset);
			FD_CLR(sock,eset);
			if( sock == *max_fd ) *max_fd--;
			pthread_mutex_lock(&srv->mutex);
			link_minus_op(srv, index);
			pthread_mutex_unlock(&srv->mutex);
		}
		else
		{
			char *pcommand;
			// 增加 1台加密机
			// 删除 1台加密机
			// 增加某台加密机的连接数
			// 减少某台加密机的连接数
      // 查询 加密机连接基本信息
      // 查询 系统 工作统计信息
      // 查询 某台加密机的 各链路 当前状态	
      // help		
      srv->link[index].work_begin_time = time(NULL);
      str[len]=0x00;
      pcommand =  strtok(str," \t\r\n");
      if( pcommand  ==  NULL ) return;
//      fprintf(stderr,"command=[%s]\n",pcommand);
      for(i=0;pcommand[i];i++)  pcommand[i]=toupper(pcommand[i]);
      if( strcmp(pcommand,"ADDM")==0)
      {
      	char *ip, *port,*max_link,*min_link,*timeout,*type;
      	ip=strtok(NULL," \t\r\n");
      	port=strtok(NULL," \t\r\n");
      	max_link=strtok(NULL," \t\r\n");
      	min_link=strtok(NULL," \t\r\n");
      	timeout =strtok(NULL," \t\r\n");
      	type =strtok(NULL," \t\r\n");
      	if( ip == NULL || port == NULL || max_link == NULL || 
      		min_link == NULL || timeout == NULL || type == NULL )
      	{	 
      		len= sprintf(buffer,"310 command format error! ip or port or max_link or min_link or timeout or type  is NULL! \n");
      		send(sock,buffer,len,0);
      		return;
        }
      	len =  add_machine(srv,wset,max_fd,ip,atoi(port),atoi(max_link),atoi(min_link),atoi(timeout),atoi(type),buffer);
      	if( len >0)
      	{
      		send(sock,buffer,len,0);
      	}
      }else if( strcmp(pcommand,"DELM")==0) {
      	char *num;
      	num=strtok(NULL," \t\r\n");
      	if( num == NULL)
      	{
      		
      			 len=sprintf(buffer,"310 command format error! machine index is NULL!\n");
      			 send(sock,buffer,len,0);
      			 return;
      	}
      	else
      	{
	      	len =  del_machine(srv,wset,rset,max_fd,atoi(num),buffer);
	      	if( len >0)	send(sock,buffer,len,0);
        }
      	
      }else if( strcmp(pcommand,"ADDL")==0) {
      	char *num;
      	num=strtok(NULL," \t\r\n");
      	
      	if( num == NULL )
      	{
      		   len=sprintf(buffer,"310 command format error! machine index is NULL!\n");
      			 send(sock,buffer,len,0);
      			 return;
      	}
      	else {
      		len =  add_link(srv,wset,max_fd,atoi(num),buffer);
      	  if( len >0)	send(sock,buffer,len,0);
      	}
      	
      	
      }else if( strcmp(pcommand,"DELL")==0) {
      	char *num;
      	num=strtok(NULL," \t\r\n");
      	if( num == NULL )
      	{
      		 len=sprintf(buffer,"310 command format error! machine index is NULL!\n");
      		 send(sock,buffer,len,0);
      		 return;
      	}
      	else
      	{
	      	len =  del_link(srv,wset,rset,max_fd,atoi(num),buffer);
	      	if( len >0)	send(sock,buffer,len,0);
        }
      	
      }else if( strcmp(pcommand,"MINF")==0) {
	      	len =  query_machine(srv,buffer,sizeof(buffer));
	      	if( len >0)	send(sock,buffer,len,0);      	
      }else if( strcmp(pcommand,"SINF")==0) {
      	  
      	  len =  query_srv_info(srv,buffer,sizeof(buffer));
	      	if( len >0)	send(sock,buffer,len,0); 
      }else if( strcmp(pcommand,"LINF")==0) {
      	  char *num;
      	  num =  strtok(NULL," \t\n");
      	  if( num == NULL )
      	  {
      	  	 len=sprintf(buffer,"310 command format error! machine index is NULL!\n");
      			 send(sock,buffer,len,0);
      			 return;
      	  }
      	  else {
	      	  len =  query_links(srv,atoi(num),buffer,sizeof(buffer));
		      	if( len >0)	send(sock,buffer,len,0);
          } 
      }else if( strcmp(pcommand,"HELP")==0) {
      	len =  sprintf(buffer,
      	                "addm[增加加密机]            delm[删除加密机]\n"  \
      	                "addl[加密机增加一条链路]    dell[加密机删除一条链路]\n" \
                        "minf[查看加密机基本信息]    linf[查看加密机链路信息]\n" \
                        "sinf[查看系统信息]          quit[退出系统]\n");
	      if( len >0)	send(sock,buffer,len,0); 
      }else if( strcmp(pcommand,"QUIT")==0) {
      	close(sock);
				FD_CLR(sock,rset);
				FD_CLR(sock,wset);
				FD_CLR(sock,eset);
				if( sock == *max_fd ) *max_fd--;
				pthread_mutex_lock(&srv->mutex);
				link_minus_op(srv, index);
				pthread_mutex_unlock(&srv->mutex);
      }
      else{
      	send(sock,"320 Unknown command!\n",21,0);
      	// 无法识别的指令
      }
      	
		}
	}
	else if( srv->link[index].iRole == 0) // 加密工作线路
	{
		sock=srv->link[index].iSocketId;
		if( srv->link[index].iWorkFlag)
		{
			srv->link[index].iStatus = 4; //接收状态
		}
		str= srv->link[index].data_buf+srv->link[index].reality_len;
		size_len = MAX_BUF_LEN-srv->link[index].reality_len;
		if( srv->link[index].reality_len !=0 )
			dcs_debug(0,0,"<%s>,index=%d, reality_len=%d",__FUNCTION__,index,srv->link[index].reality_len);
//		dcs_debug(0,0,"<%s> enc working line size_len =%d",__FUNCTION__,size_len);
		if( size_len <=0)
		{
				dcs_log(srv->link[index].data_buf,srv->link[index].reality_len,"<%s>system occur bug ,size_len[%d]",__FUNCTION__,size_len);
				return;
		}
	  
		len= recv(sock,str,size_len,0);
		if( len < 0)
		{
			if (errno == EINTR || errno == EAGAIN) return;
			close(sock);
			FD_CLR(sock,rset);
			FD_CLR(sock,wset);
			FD_CLR(sock,eset);
			if( sock == *max_fd ) *max_fd--;
			srv->work_links--;
//			if( srv->link[index].iWorkFlag)
			dcs_log(0,0,"<%s> wokr line <0 enter response_queue_error",__FUNCTION__);
			if( srv->link[index].iWorkFlag)	response_queue_error(srv,index,"FF");
//			fprintf(stderr,"work link close! <0 \n");
//      dcs_log(0,0,"<%s>work link close! <0 \n",__FUNCTION__);
			pthread_mutex_lock(&srv->mutex);
			link_minus_op(srv, index);
			pthread_mutex_unlock(&srv->mutex);
			dcs_log(0,0,"<%s>work link close! <0 \n",__FUNCTION__);
			
		}
		else if ( len == 0)
		{
			close(sock);
			FD_CLR(sock,rset);
			FD_CLR(sock,wset);
			FD_CLR(sock,eset);
			if( sock == *max_fd ) *max_fd--;
			srv->work_links--;
//			if( srv->link[index].iWorkFlag)
			dcs_log(0,0,"<%s> work line == 0 enter response_queue_error",__FUNCTION__);
			if( srv->link[index].iWorkFlag)	 response_queue_error(srv,index,"FF");
//			fprintf(stderr,"work link close! ==0 \n");
			pthread_mutex_lock(&srv->mutex);
			link_minus_op(srv, index);
			pthread_mutex_unlock(&srv->mutex);
			dcs_log(0,0,"<%s>work link close! ==0 \n",__FUNCTION__);
		}
		else
		{
			dcs_debug(str,len,"<%s>rcv len=%d",__FUNCTION__,len);
			srv->link[index].reality_len  += len ; 
			if( srv->link[index].expected_len == 0)
			{
				 if( srv->link[index].reality_len >=2 ) 
				 	srv->link[index].expected_len = srv->link[index].data_buf[0]*256+ srv->link[index].data_buf[1]+2;
			}
			if( srv->link[index].expected_len > MAX_BUF_LEN )
			{
					close(sock);
					FD_CLR(sock,rset);
					FD_CLR(sock,wset);
					FD_CLR(sock,eset);
					if( sock == *max_fd ) *max_fd--;
					dcs_log(0,0,"<%s> expected_len=[%d] overflow ,expected_len > MAX_BUF_LEN ",__FUNCTION__,srv->link[index].expected_len);
					response_queue_error(srv,index,"FF");
					pthread_mutex_lock(&srv->mutex);
					link_minus_op(srv, index);
					pthread_mutex_lock(&srv->mutex);
					srv->work_links--;
//					dcs_debug(0,0,"<%s> srv->link[index].expected_len > MAX_BUF_LEN enter response_queue_error",__FUNCTION__);
					
			}
			else if( srv->link[index].expected_len>0 &&
				  srv->link[index].expected_len <= srv->link[index].reality_len )
			{
				//返回正常数据
				time_t send_time;
				len =  sizeof(long)+1;
				memcpy(buffer,srv->link[index].data_key,len);
				send_time =  time(NULL);
				memcpy(buffer+len,&send_time,sizeof(time_t));
				len += sizeof(time_t);
				memcpy(buffer+len, srv->link[index].data_buf,srv->link[index].expected_len);
				len += srv->link[index].expected_len;
				if( 0>queue_send( srv->send_qid, buffer, len,1)) dcs_log(0,0,"<%s> queue send fail! [%s]",__FUNCTION__,strerror(errno));	
        srv->count++;
			  srv->link[index].server_count++;
//        sleep(30);
//        dcs_debug(0,0,"<%s> data_queue num=%d",__FUNCTION__,srv->data_queue.message_num);
        pthread_mutex_lock(&srv->mutex);
        len = effi_get_queue(&srv->data_queue, buffer ,sizeof(buffer));
        pthread_mutex_unlock(&srv->mutex);
				if( len >0 )
				{
				
					 time_t t;
//						pMsg= (struct MSG_STRU *)buffer;
						memcpy(&t,buffer+9,sizeof(time_t));
//						dcs_debug(0,0,"<%s> save time=%d, now time=%d ",__FUNCTION__,t,send_time);
						if( send_time > (t +8) ) // 消息在data队列内超时
						{
//							memcpy(srv->link[index].data_key,buffer,sizeof(long)+1+sizeof(time_t)+2);
							dcs_debug(0,0,"<%s> msg timeout return recv time=%d,now time=%d",__FUNCTION__,t,send_time);
							response_busy_error(srv, buffer , len );
							srv->link[index].iWorkFlag=0;
							srv->link[index].reality_len = 0;
							srv->link[index].expected_len = 0;
							srv->link[index].iStatus=2; //就绪状态
						}
					 srv->link[index].work_begin_time = time(NULL);
    			 memcpy(srv->link[index].data_key,buffer,30);
    			 srv->link[index].reality_len= len -sizeof(long)-1-sizeof(time_t);
     			
    			 srv->link[index].iStatus = 3; //数据发送状态
    			 while(1)
    			 {	 
//	    			 dcs_debug(0,0,"<%s>accept count=%d",__FUNCTION__,srv->accept_cnt);
	    			 len =  send(srv->link[index].iSocketId,buffer+sizeof(long)+1+sizeof(time_t),srv->link[index].reality_len,0);
	    			 if( len <=0 )
	    			 {
	    			 		if (errno == EINTR || errno == EAGAIN) continue;
	    			 		dcs_log(0,0,"<%s> send fail [%s]",__FUNCTION__,strerror(errno));
	    			 		close( srv->link[index].iSocketId );
	    			 		FD_CLR(srv->link[index].iSocketId,rset);
	    			 		FD_CLR(srv->link[index].iSocketId,wset);
	    			 		FD_CLR(srv->link[index].iSocketId,eset);
					      if( srv->link[index].iSocketId == *max_fd ) *max_fd--;	
								response_abnormal_error(srv,buffer,30);
	    			 		pthread_mutex_lock(&srv->mutex);
	    			 		link_minus_op(srv, index);
	    			 		pthread_mutex_unlock(&srv->mutex);
								srv->work_links--;
								break;
	    			 }
//	    			 srv->count++;
//	    			 srv->link[index].server_count++;
	    			 srv->link[index].expected_len=0;
						 srv->link[index].reality_len=0;
	    			 break;
    			 }
				}
				else
			{
						srv->link[index].iStatus=2;
						srv->link[index].iWorkFlag=0;	
						srv->link[index].expected_len=0;
						srv->link[index].reality_len=0;
			  }
			}
	  }
	}
	else //异常
	{
		 dcs_log(0,0,"<%s>system occur bug ,unknown role[%d]",__FUNCTION__,srv->link[index].iRole);
	}
}
void comm_error_proc(struct EFFI_SEC_SRV * srv, int index,fd_set *wset,fd_set *rset,fd_set *eset,int *max_fd)
{
	dcs_debug(0,0,"<%s> begin",__FUNCTION__);
	if( srv == NULL ) return;
	if(index >= srv->used_link || index <0 ) return;
	
	if ( srv ->link[index].iRole == 0 ) srv->work_links--;
	close(srv->link[index].iSocketId);
	FD_CLR(srv->link[index].iSocketId,wset);
	FD_CLR(srv->link[index].iSocketId,rset);
	FD_CLR(srv->link[index].iSocketId,eset);
}

void reconnection(struct EFFI_SEC_SRV * srv,fd_set *wset,fd_set *eset,int *max_fd )
{
	int i,j,sock;
	
//	dcs_debug(0,0,"<%s> begin",__FUNCTION__);
	for( i=0; i< srv->used_machine;i++)
	{
		if( !srv->machine_info[i].encType ) continue;
		j= srv->machine_info[i].undone_link_num;
		for(;j>0; j--)
		{
			if( srv->used_link >= srv->max_link )
			{
				dcs_log(0,0,"<%s> used_link overflow ! max_link=[%d],used_link=[%d] ",__FUNCTION__,srv->max_link,srv->used_link);
				return;
			}
			sock = tcp_connet_server( srv->machine_info[i].remoteIp, srv->machine_info[i].remotePort,0);
			if(sock < 0)
			{
				dcs_log(0,0,"<%s> ip=[%s]port=[%d] cause is[%s]",__FUNCTION__,srv->machine_info[i].remoteIp, srv->machine_info[i].remotePort,strerror(errno));
				break;
			}
			add_to_set(sock, wset, max_fd);
			srv->link[srv->used_link].iRole = 0;     //该link在连接建立过程中的角色 0 加密工作线路 1 监控侦听线路 2监控工作线路 
			srv->link[srv->used_link].iStatus = 1;   //该link当前 的状态 0 链路停止状态 1链路在建状态 2 链路就绪状态 3数据发送状态 4数据结束状态
			srv->link[srv->used_link].iWorkFlag = 0; //0 空闲状态 1工作状态
			srv->link[srv->used_link].ip_num  = i;
			srv->link[srv->used_link].iSocketId = sock;
			srv->link[srv->used_link].iTimeOut = srv->machine_info[i].timeout;    //超时时间
			srv->link[srv->used_link].build_time = time(NULL);  // 端口创建时间
			srv->link[srv->used_link].server_count = 0 ; //链路服务次数
			srv->link[srv->used_link].expected_len =0; //期望数据的长度
      srv->link[srv->used_link].reality_len = 0; //实际数据的长度
			srv->build_count++;
			srv->used_link++;
			srv->machine_info[i].undone_link_num--;  // 需要重连计数器调整
		}
	}
}

int link_add_op (struct EFFI_SEC_SRV * srv, int index,int sock)
{
  if( srv ==  NULL || index<0 || index >=srv->used_link || sock <0 ) return 0;
  pthread_mutex_lock(&srv->mutex);	
	if( srv->link[index].iRole == 1 )	{srv->link[srv->used_link].iRole = 2;	srv->link[srv->used_link].work_begin_time= time(NULL); }     //该link在连接建立过程中的角色 0 加密工作线路 1 监控侦听线路 2监控工作线路 
	else srv->link[srv->used_link].iRole = 0 ;
	if( srv->link[index].iRole == 0 ) srv->link[srv->used_link].iStatus = 1;
	else srv->link[srv->used_link].iStatus = 2;   //该link当前 的状态 0 链路停止状态 1链路在建状态 2 链路就绪状态 3数据发送状态 4数据结束状态
	srv->link[srv->used_link].iWorkFlag = 0; //0 空闲状态 1工作状态
	srv->link[srv->used_link].ip_num  = srv->link[index].ip_num;
	srv->link[srv->used_link].iSocketId = sock;
	srv->link[srv->used_link].iTimeOut = srv->machine_info[srv->link[index].ip_num].timeout;    //超时时间
	srv->link[srv->used_link].build_time = time(NULL);  // 端口创建时间
	srv->link[srv->used_link].server_count = 0 ; //链路服务次数
	srv->link[srv->used_link].expected_len =0; //期望数据的长度
	srv->link[srv->used_link].reality_len =0; //实际数据的长度
	if( srv->link[index].iRole == 1 )	srv->machine_info[srv->link[index].ip_num].link_num++;
//	else srv->machine_info[i].undone_link_num--;
//			srv->build_count++;
	srv->used_link++;
	pthread_mutex_unlock(&srv->mutex);	
	return 1;
}

int link_minus_op(struct EFFI_SEC_SRV * srv, int index )
{
	if( srv ==  NULL || index<0 || index >=srv->used_link) return 0;
	dcs_debug(0,0,"<%s> begin role=%d ,ip_num=%d",__FUNCTION__,srv->link[index].iRole, srv->machine_info[srv->link[index].ip_num].link_num);
//	pthread_mutex_lock(&srv->mutex);
	
	if( srv->link[index].iRole == 0 ) srv->machine_info[srv->link[index].ip_num].undone_link_num++;
	else if( srv->link[index].iRole == 2 ) srv->machine_info[srv->link[index].ip_num].link_num--;
	dcs_debug(0,0,"<%s> role=%d ,ip_num=%d",__FUNCTION__,srv->link[index].iRole, srv->machine_info[srv->link[index].ip_num].link_num);
	if( index != srv->used_link -1 )
		srv->link[index]= srv->link[srv->used_link -1];
	srv->used_link--;
	if( srv->curr_link == srv->used_link) srv->curr_link--; 
//	pthread_mutex_unlock(&srv->mutex);		
	dcs_debug(0,0,"<%s> end",__FUNCTION__);

	return 1;
}

int response_queue_error(struct EFFI_SEC_SRV * srv, int index , char * err_code )
{
	unsigned char buf[256];
	int len;
	time_t t;
	dcs_debug(0,0,"<%s> begin ", __FUNCTION__);
	
	if( srv ==  NULL || index<0 || index >=srv->used_link)
	{
		 dcs_log(0,0,"<%s> para error ! index=%d used_link=%d",__FUNCTION__,index,srv->used_link );
		 return 0;
	}
	if( srv->link[index].iRole == 0)	srv->abnormal_cnt++;
	memcpy(buf,srv->link[index].data_key,sizeof(long)+1);
	len = sizeof(long)+1;
	t= time(NULL);
	memcpy( buf+len,&t,sizeof(t));
	len += sizeof(t);
	memcpy(buf+len,"\x00\x04",2);
	len +=2;
	memcpy(buf+len,srv->link[index].data_key+len,2);
	len +=2;
	memcpy(buf+len,err_code,2);
	len +=2;
	if( 0>queue_send(srv->send_qid,buf,len,1)) dcs_log(0,0,"<%s> queue send fail! [%s]",__FUNCTION__,strerror(errno));	
	else dcs_debug(buf,len,"<%s> queue_send ok!",__FUNCTION__);
	dcs_debug(0,0,"<%s> end ", __FUNCTION__);
	return 1;
}

int response_busy_error(struct EFFI_SEC_SRV * srv,unsigned char *buf, int size_len)
{
	time_t t;
	int len;
	dcs_debug(0,0,"<%s> begin " ,__FUNCTION__);
	
//	if( srv  == NULL || buf == NULL || size_len <sizeof(long)+1+sizeof(time_t)+4) return 0;
	srv->refuse_cnt++;
	t= time(NULL);
	len = sizeof(long)+1;	
	memcpy( buf+len, &t,sizeof(time_t));
	len += sizeof(time_t);
	memcpy(buf+len,"\x00\x04",2);
	len += 4;
	memcpy(buf+len,"FE",2);
	len += 2;
//	dcs_debug(buf,len,"<%s> begin send " ,__FUNCTION__);
	if( 0>queue_send(srv->send_qid,buf,len,1))
		dcs_log(0,0,"<%s> queue send fail! [%s]",__FUNCTION__,strerror(errno));	
//	else dcs_debug(buf,len,"<%s> queue_send ok!",__FUNCTION__);
	dcs_debug(0,0,"<%s> end ", __FUNCTION__);
	return 1;
}
int response_discard_error(struct EFFI_SEC_SRV * srv,unsigned char *buf, int size_len)
{
	time_t t;
	int len;
	dcs_debug(0,0,"<%s> begin " ,__FUNCTION__);
	
//	if( srv  == NULL || buf == NULL || size_len <sizeof(long)+1+sizeof(time_t)+4) return 0;
	t= time(NULL);
	len = sizeof(long)+1;	
	memcpy( buf+len, &t,sizeof(time_t));
	len += sizeof(time_t);
	memcpy(buf+len,"\x00\x04",2);
	len += 4;
	memcpy(buf+len,"FD",2);
	len += 2;
//	dcs_debug(buf,len,"<%s> begin send " ,__FUNCTION__);
	if( 0>queue_send(srv->send_qid,buf,len,1))
		dcs_log(0,0,"<%s> queue send fail! [%s]",__FUNCTION__,strerror(errno));	
//	else dcs_debug(buf,len,"<%s> queue_send ok!",__FUNCTION__);
	dcs_debug(0,0,"<%s> end ", __FUNCTION__);
	return 1;
}
int response_abnormal_error(struct EFFI_SEC_SRV * srv,unsigned char *buf, int size_len)
{
	time_t t;
	int len;
	dcs_debug(0,0,"<%s> begin " ,__FUNCTION__);
	srv->abnormal_cnt++;
	if( srv  == NULL || buf == NULL || size_len <sizeof(long)+1+sizeof(time_t)+4) return 0;
	
	t= time(NULL);
	len = sizeof(long)+1;	
	memcpy( buf+len, &t,sizeof(time_t));
	len += sizeof(time_t);
	memcpy(buf+len,"\x00\x04",2);
	len += 4;
	memcpy(buf+len,"FF",2);
	len += 2;
	if( 0>queue_send(srv->send_qid,buf,len,1)) 
		dcs_log(0,0,"<%s> queue send fail! [%s]",__FUNCTION__,strerror(errno));	
	dcs_debug(0,0,"<%s> end ", __FUNCTION__);
	return 1;
}
int add_machine(struct EFFI_SEC_SRV *srv,fd_set *wset,int *max_fd, char *ip,int port,int max_link,int min_link,int timeout,int type ,unsigned char *buf)
{
	int i,flag,sock;
	if( buf == NULL || wset == NULL || max_fd==NULL || ip==NULL) return 0;
	dcs_debug(0,0,"<%s> begin min_link=%d,ip=[%s],port=%d",__FUNCTION__,min_link,ip,port);
	if( srv->used_machine == srv ->max_machine )
	{
		strcpy( buf,"330 system not fount free machine slot !");
		return strlen(buf);
	}
	if( max_link < min_link )
	{
		return sprintf( buf,"310  ADDM command  max link[%d] is less than min link[%d] !",max_link,min_link);
	}
	if( max_link > srv->max_link - srv->used_link )
	{
		strcpy( buf,"330 system free slot not enough ,please modify max_link!");
		return strlen(buf);
	}
	
	for ( i=0 ; i < min_link; i++)
	{
		sock =  tcp_connet_server(ip,port,0);
		
		if( i == 0 && sock >0 )
		{
			strcpy(srv->machine_info[srv->used_machine].remoteIp,ip);
			srv->machine_info[srv->used_machine].remotePort = port; 
			srv->machine_info[srv->used_machine].link_num = min_link;
			srv->machine_info[srv->used_machine].max_link_num = max_link;
			srv->machine_info[srv->used_machine].undone_link_num = min_link;
			srv->machine_info[srv->used_machine].timeout = timeout;       
			srv->machine_info[srv->used_machine].encType = type;
			srv->used_machine++;

		}
		else if( i == 0 )
		{
			return sprintf(buf,"system abnormal,surmise	ip or port is wrong[%s][%d]",ip,port);
		}
		
		if ( sock <0 ) break;
		add_to_set(sock, wset, max_fd);
		pthread_mutex_lock(&srv->mutex);
		srv->link[srv->used_link].iRole = 0;     //该link在连接建立过程中的角色 0 加密工作线路 1 监控侦听线路 2监控工作线路 
		srv->link[srv->used_link].iStatus = 1;   //该link当前 的状态 0 链路停止状态 1链路在建状态 2 链路就绪状态 3数据发送状态 4数据结束状态
		srv->link[srv->used_link].iWorkFlag = 0; //0 空闲状态 1工作状态
		srv->link[srv->used_link].ip_num  = srv->used_machine -1;
		srv->link[srv->used_link].iSocketId = sock;
		srv->link[srv->used_link].iTimeOut = timeout;    //超时时间
		srv->link[srv->used_link].build_time = time(NULL);  // 端口创建时间
		srv->link[srv->used_link].server_count = 0 ; //链路服务次数
		srv->link[srv->used_link].expected_len = 0; //期望数据的长度
    srv->link[srv->used_link].reality_len = 0; //实际数据的长度
		srv->build_count++;
		srv->used_link++;
		srv->machine_info[srv->used_machine -1].undone_link_num--;  // 需要重连计数器调整
		pthread_mutex_unlock(&srv->mutex);
	}
	if ( i > 0) return sprintf(buf,"200 addm link cnt=[%d]\n",i);
	dcs_debug(0,0,"<%s> end ", __FUNCTION__);
	return sprintf(buf,"210  not add machine and link!\n ");
	   
}

int del_machine(struct EFFI_SEC_SRV *srv,fd_set *wset,fd_set *rset,int *max_fd,int index,unsigned char *buf)
{
	int i,j;
	dcs_debug(0,0,"<%s> begin ", __FUNCTION__);
	if( srv == NULL || wset == NULL || rset == NULL || max_fd == NULL || buf == NULL ) return 0;
		
	if( index <0 || index >=srv->used_machine) return sprintf(buf,"310 machine num[%d] error!",index);
	pthread_mutex_lock(&srv->mutex);
	for ( i=0,j=0; i <srv->used_link; i++)
	{
		if( srv->link[i].ip_num != index) continue;
		if( srv->link[i].iWorkFlag ) continue;
		close( srv->link[i].iSocketId);
		FD_CLR(srv->link[i].iSocketId,rset);
		FD_CLR(srv->link[i].iSocketId,wset);
		if( srv->link[i].iSocketId == *max_fd) *max_fd--;
		if( srv->link[i].iStatus == 1) FD_CLR(srv->link[i].iSocketId,wset);
		else FD_CLR(srv->link[i].iSocketId,rset);
		
		srv->machine_info[index].link_num--;
		if( i != srv->used_link -1)
		{
				srv->link[i] = srv->link[srv->used_link -1];
				i--;
		}
		srv->used_link--;
		j++;
	}
	pthread_mutex_unlock(&srv->mutex);
//	if( j == 0) return sprintf(buf,"210 not del machine link");
  if( j == 0 ) srv->machine_info[index].link_num = 0;
	if( srv->machine_info[index].link_num == 0)
	{
		if( index != srv->used_machine -1) 
		{
			 for ( i=0 ; i< srv->used_link; i++)
			  if( srv->link[i].ip_num ==  srv->used_machine -1) srv->link[i].ip_num  = index;
			 srv->machine_info[index]= srv->machine_info[srv->used_machine -1];
			 
		}	
		srv->used_machine --;
	}
	dcs_debug(0,0,"<%s> end ", __FUNCTION__);
	return sprintf(buf,"200 del machine link num[%d]\n",j);
}

int add_link(struct EFFI_SEC_SRV *srv,fd_set *wset,int *max_fd,int index,unsigned char *buf)
{
	int sock;
	dcs_debug(0,0,"<%s> begin ", __FUNCTION__);
	if( srv == NULL || wset == NULL || max_fd == NULL || buf == NULL ) return 0;
	if( index <0 || index >= srv->used_machine) return sprintf(buf,"310 ADDL command machine index[%d] Limit exceeded!\n ",index);
	
	if( srv->machine_info[index].link_num >= srv->machine_info[index].max_link_num )
		return sprintf(buf,"310 ADDL command  link full ! max_link[%d]\n",srv->machine_info[index].max_link_num);	
	
		sock =  tcp_connet_server(srv->machine_info[index].remoteIp,srv->machine_info[index].remotePort,0);
		
	
		if( sock <= 0 )
		{
			return sprintf(buf,"300 system abnormal,surmise	ip or port is wrong[%s][%d] err[%s]\n",srv->machine_info[index].remoteIp,srv->machine_info[index].remotePort,strerror(errno));
		}
		add_to_set(sock, wset, max_fd);
		pthread_mutex_lock(&srv->mutex);
		srv->link[srv->used_link].iRole = 0;     //该link在连接建立过程中的角色 0 加密工作线路 1 监控侦听线路 2监控工作线路 
		srv->link[srv->used_link].iStatus = 1;   //该link当前 的状态 0 链路停止状态 1链路在建状态 2 链路就绪状态 3数据发送状态 4数据结束状态
		srv->link[srv->used_link].iWorkFlag = 0; //0 空闲状态 1工作状态
		srv->link[srv->used_link].ip_num  = index;
		srv->link[srv->used_link].iSocketId = sock;
		srv->link[srv->used_link].iTimeOut = srv->machine_info[index].timeout;    //超时时间
		srv->link[srv->used_link].build_time = time(NULL);  // 端口创建时间
		srv->link[srv->used_link].server_count = 0 ; //链路服务次数
		srv->link[srv->used_link].expected_len = 0; //期望数据的长度
    srv->link[srv->used_link].reality_len = 0; //实际数据的长度
		srv->build_count++;
		srv->used_link++;	
		srv->machine_info[index].link_num++;  // 需要重连计数器调整
		pthread_mutex_unlock(&srv->mutex);
		dcs_debug(0,0,"<%s> end ", __FUNCTION__);
		return sprintf(buf,"200 ADDL execute succ!\n");
}

int del_link(struct EFFI_SEC_SRV *srv,fd_set *wset,fd_set *rset,int *max_fd,int index,unsigned char *buf )
{
	int i;
	if( srv == NULL || wset == NULL || wset == NULL || max_fd == NULL || buf == NULL ) return 0;
	if( index <0 || index >= srv->used_machine) return sprintf(buf,"310 DELL command machine index[%d] Limit exceeded!\n",index);
	
		
	for ( i=0; i< srv->used_link; i++)
	{
		 if( srv->link[i].ip_num !=index) continue;
		 if( srv->link[i].iWorkFlag ) continue;
		 pthread_mutex_lock(&srv->mutex);
		 if( srv->link[i].iWorkFlag ) {pthread_mutex_unlock(&srv->mutex); break; }
		 close(srv->link[i].iSocketId);
		 FD_CLR(srv->link[i].iSocketId,rset);
		 FD_CLR(srv->link[i].iSocketId,wset);
		 if( srv->link[i].iSocketId == *max_fd ) *max_fd--;
		 if( srv->link[i].iSocketId == *max_fd) *max_fd--;
		 if( srv->link[i].iStatus == 1 ) FD_CLR(srv->link[i].iSocketId,wset);
		 else FD_CLR(srv->link[i].iSocketId,rset);
		 srv->machine_info[index].link_num--;
		 if( i != srv->used_link -1) srv->link[i] =  srv->link[srv->used_link -1];
		 
		 srv->used_link--;
		 pthread_mutex_unlock(&srv->mutex);
		 return sprintf(buf,"200 DELL command  succ!\n");
	}
	return sprintf(buf,"210 DELL command not found may del link !\n");	
}

int query_machine(struct EFFI_SEC_SRV *srv,unsigned char *buf,int size)
{
	int i,n;
	if( srv == NULL || buf == NULL ) return 0;
		
	for( i=0,n=0;i < srv->used_machine; i++)
  {
  	 n += snprintf(buf+n,size-n,"[%d] Ip=[%s] Port=[%d] min_link=[%d] max_link=[%d] undone_link=[%d] timeout=[%d] type=[%d]\n",i ,
  	  
    	srv->machine_info[i].remoteIp,  //期望的远端的IP地址
    	srv->machine_info[i].remotePort,   //期望的远端的端口号
    	srv->machine_info[i].link_num,    //需要建立的链路数
    	srv->machine_info[i].max_link_num, //最大能建立的链路数，为0则不进行判断
    	srv->machine_info[i].undone_link_num, // 未完成建立的链路数
    	srv->machine_info[i].timeout,
    	srv->machine_info[i].encType);
  }
	return n;
}
int query_srv_info(struct EFFI_SEC_SRV *srv,unsigned char *buf, int size)
{
	if( srv == NULL || buf == NULL ) return 0;
	
	return snprintf(buf,size,"200 snd queue name[%s] \n" \
	                   "rcv queue name[%s]\n" \
                     "max_link=[%d]\n"     \
	                   "used_link=[%d]\n"    \
	                   "work_links=[%d]\n"   \
									   "curr_link=[%d]\n"   \
									   "max_machine=[%d]\n"  \
									   "used_machine=[%d]\n" \
									   "link_build_count=[%d]\n" \
									   "send_qid =%d\n"  \
									   "recv_qid =%d\n"  \
									   "work count=[%d]\n"  \
									   "accept count=%d\n"    \
									   "refuse count=%d\n"    \
									   "abnormal count=%d\n"    \
									   "discard count=%d\n"    \
										 "data_queue msg=[%d]\n",
									srv->queue_snd_name,
									srv->queue_rcv_name,
									srv->max_link,
									srv->used_link,
									srv->work_links,
									srv->curr_link,
									srv->max_machine,
									srv->used_machine,
									srv->build_count,
									srv->send_qid,
									srv->recv_qid,
									srv->count,
									srv->accept_cnt,
									srv->refuse_cnt,
									srv->abnormal_cnt,
									srv->discard_cnt,
									srv->data_queue.message_num);
}

int query_links(struct EFFI_SEC_SRV *srv,int index,unsigned char *buf, int size)
{
	int i,n,j;
	if( srv == NULL || buf == NULL ) return 0;
	
	if( index <0 || index >= srv->used_machine) return snprintf(buf,size,"LINF command machine index[%d] Limit exceeded!\n",index);
dcs_debug(0,0,"<%s> begin index=%d used_link=%d link_num=%d",__FUNCTION__,index,srv->used_link,srv->machine_info[index].link_num);
  for( i=0,n=0,j=0; i< srv->used_link && j < srv->machine_info[index].link_num;i++)
    if( srv->link[i].ip_num ==  index )
    {
	    n +=snprintf(buf+n,size-n,"[%d]slot=[%d] Role=%d Status=%d WorkFlag=%d SocketId=%d TimeOut=%d build_time=%d server_count=%d\n",j,i,
	  						srv->link[i].iRole,     //该link在连接建立过程中的角色 0 加密工作线路 1 监控侦听线路 2监控工作线路 
    						srv->link[i].iStatus,   //该link当前 的状态 0 链路停止状态 1链路在建状态 2 链路就绪状态 3数据发送状态 4数据结束状态
    						srv->link[i].iWorkFlag, //0 空闲状态 1工作状态
    						srv->link[i].iSocketId,
    						srv->link[i].iTimeOut,    //超时时间
    						srv->link[i].build_time,  // 端口创建时间
    						srv->link[i].server_count 
	             );
	    j++;
    	
    }
    dcs_debug(0,0,"<%s> end i=%d,j=%d",__FUNCTION__,i,j);
  if( n == 0) n=sprintf(buf,"210 This machine not found working links!\n");
  return n;
}