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
#include <getopt.h>
extern gs_run_mode;
#ifndef  MAX_TASK
  #define MAX_TASK 50
#endif
	struct CFTPCommand
	{
		int m_nTokenID;
		char *m_pszName;
	};
	enum // Token ID's
	{
		TOK_USER, TOK_PASS, TOK_CWD, TOK_PWD, 
		TOK_PORT, TOK_PASV, TOK_TYPE, TOK_LIST,
		TOK_REST, TOK_CDUP, TOK_RETR, TOK_STOR,
		TOK_SIZE, TOK_DELE, TOK_RMD, TOK_MKD,
		TOK_RNFR, TOK_RNTO, TOK_ABOR, TOK_SYST, TOK_NOOP, 
		TOK_BYE, TOK_QUIT,TOK_HELP,TOK_MODE,TOK_ERROR,
	};
int get_order_list[]={TOK_USER,TOK_PASS,TOK_CWD,TOK_PWD,TOK_TYPE,TOK_PASV,TOK_RETR,TOK_ERROR};
int put_order_list[]={TOK_USER,TOK_PASS,TOK_CWD,TOK_PWD,TOK_TYPE,TOK_PASV,TOK_STOR,TOK_RNFR, TOK_RNTO,TOK_ERROR};
int used_task_list,last_task_ndx; // ��ʹ�õ������б���������,�ϴ�ʹ�õ����񻺳������� 
int g_listen_sock=0;
struct CFTPCommand commandList[] = 
	{
		{TOK_USER,	"USER"},
		{TOK_PASS,	"PASS"},
		{TOK_CWD,	"CWD"	},
		{TOK_PWD,	"PWD"	},
		{TOK_PORT,	"PORT"},
		{TOK_PASV,	"PASV"},
		{TOK_TYPE,	"TYPE"},
		{TOK_LIST,	"LIST"},
		{TOK_REST,	"REST"},
		{TOK_CDUP,	"CDUP"},
		{TOK_RETR,	"RETR"},
		{TOK_STOR,	"STOR"},
		{TOK_SIZE,	"SIZE"},
		{TOK_DELE,	"DELE"},
		{TOK_RMD,	"RMD"},
		{TOK_MKD,	"MKD"},
		{TOK_RNFR,	"RNFR"},
		{TOK_RNTO,	"RNTO"},
		{TOK_ABOR,	"ABOR"}, 
		{TOK_SYST,	"SYST"},
		{TOK_NOOP,	"NOOP"},
		{TOK_BYE,	"BYE"},
		{TOK_QUIT,	"QUIT"},
		{TOK_QUIT,	"HELP"},
		{TOK_MODE,	"MODE"},
		{TOK_ERROR,	""},
	};
struct FD_REC{
	int sockfd; // ��sock
	int sockfd_corr;  //���� sock
	int order_ndx;  // ָ����������ʶ��ǰsock�Ѿ�ִ�е�����ָ���ˣ���ִ�е�TOK_ERRORָ��ʱ����ʾ������ִ�н���
	int file_id;
	time_t  timeout;
	time_t  last_time;
	time_t  begin_time;
	int ndx;  //���������б�����
	char type; /* 0�ͻ���������·��1 ������������· 2���������· 3�ļ����ݴ�����·���� 4 �ļ����ݴ�������·*/
	char status; /* 0 ���ڽ��� 1��·���� 2 ׼���������� 3�ȴ���������Ӧ�� 4 ���������յ� 5�������ڷ��� 6�������ڽ��� 7�ȴ��յ����� 8�������ѷ���*/
	char used; //�жϸò�λ�Ƿ���ʹ��
	int command;  // ��Ϊ���������·ʱ�����ʹ�õ����
	int data_len;  //δ��ɷ������ݳ���
	int off_set;   //һ�����ڷ����ļ�ʱ������ƫ��λ�ã����ڷ�������ʱ�����淢�ͷ��ͻ������������
	char buf[8192];
};
struct FTP_TASK{ 
	 char cmd;      //"0"��������,"3"�ϴ�����
   char user_name[60];  //�û���
   char password[30];   //����
   char remote_path[200]; //Զ���ļ�����Ŀ¼
   char remote_file_name[100]; // Զ���ļ���
   char local_path[200]; //�����ļ���������Ŀ¼
   char local_name[100]; // ���ر�����ļ���,�յĻ�����Զ���ļ���һ��
   char host_name[100];  // Զ������ftp��ַ
   char used;           // 0x00 δʹ��,����ʹ��
   int  host_port;      // Զ������ftp �˿ں�
   char status;         // ����״̬ 0 ��ʼ״̬ 1���ݴ���׼��״̬ 2����״̬,3��ͣ״̬
   int  socket_cmd;	        // ����ʹ�õ�socket
   int  socket_data;    // �ļ�����socket
   int  socket_acq;     //��������scoket
   time_t accept_time;   // �������ʱ��
   time_t begin_time;   // �ļ����俪ʼʱ��
   time_t last_time;    // �ϴγ�������ʱ��
   int  timeout;        // �������ʱʱ��
   long size   ;         // �������ļ����ݴ�С
};
struct FD_REC gl_fd[FD_SETSIZE];
struct FTP_TASK gl_task[MAX_TASK];
int role=0,stop=0,crypt_flag=0;
static char logfile[256];
static int dont_daemonize =0;
fd_set vread_set,vwrite_set,verror_set;
int max_fd ;
int resp_err_msg(int sock,char cmd,struct FTP_TASK *task, int err_code,char *err_msg);
int delete_task(struct FTP_TASK *task,int flag, char *msg);
int query_task(int sock,struct FTP_TASK *task);
int resp_succ_msg( int sock,char cmd, struct FTP_TASK *task);
void process_timeout ();
void do_loop();
int connect_sock(int i ,int *max_fd);
fd_set read_set,write_set,error_set;
void _srv_exit( int signo);
 void add_to_set(int fd, fd_set *set, int *max_fd) {
  FD_SET(fd, set);
  if (fd >  *max_fd) {
    *max_fd = (int) fd;
  }
}

typedef void sigfunc_t(int);
sigfunc_t * catch_signal(int sig_no, sigfunc_t *sig_catcher)
{
    //
    //catch a specified signal with the given handler
    //
    
    struct sigaction act, oact;
    
    act.sa_handler = sig_catcher;
    act.sa_flags   = 0;

    //block no signals when this signal is being handled.        
    sigemptyset(&act.sa_mask);

    if(sig_no == SIGALRM || sig_no == SIGUSR1 || sig_no == SIGUSR2)
    {
        #ifdef SA_INTERRUPT        
            act.sa_flags |=  SA_INTERRUPT;    
        #endif        
    }
    else
    {
        #ifdef SA_INTERRUPT        
            act.sa_flags |=  SA_INTERRUPT;    
        #endif        

        //#ifdef SA_RESTART
          // act.sa_flags |=  SA_RESTART;
        //#endif    
    }
    
    if(sigaction(sig_no, &act, &oact) < 0)
        return (sigfunc_t *)0;
    return oact.sa_handler;    
}

//-------------------------------------------------

int catch_all_signals(sigfunc_t *sig_catcher)
{
    int signo;
    
    for(signo=1; signo < NSIG; signo++)
    {
        switch(signo)
        {
            case SIGHUP:             
            case SIGCONT:             
                 catch_signal(signo, SIG_IGN);
                 break;

            case SIGCHLD:
            default:
                 catch_signal(signo,sig_catcher);
                 break;     
        }//switch         
    }//for
    
    return 0;    
}
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
static void daemonize(void) {

	if (0 != fork()) exit(0);

	if (-1 == setsid()) exit(0);

	signal(SIGHUP, SIG_IGN);

	if (0 != fork()) exit(0);

	if (0 != chdir("/")) exit(0);
}
static void show_version (void) {

	fprintf(stderr,"version 1.0 -Build-Date: %s %s\n",__DATE__,__TIME__);
}
static void show_help (void) {

	fprintf(stderr,"-p []   Listen Port\n");
//	fprintf(stderr,"-D   daemonize\n");
	fprintf(stderr,"-v   display version \n");
	fprintf(stderr,"-h   display help \n");
}
void rtrim( char * str)
{
	int i;
	if ( str == NULL ) return ;
	i=strlen(str);
	for(;i>0;i--)
	   if( str[i-1]!=0x20) break;
	str[i]=0x00;
	return ;
}
int main (int argc, char *argv[] )
{
	int i;
	char tmp[32],tmp1[32],o;
   gs_run_mode =0;
  sprintf(logfile,"%s.log",argv[0]);
	dcs_log_open(logfile,NULL);
	catch_all_signals(_srv_exit);
	catch_signal(40,_srv_exit);
	catch_signal(13,SIG_IGN);
	memset(&gl_fd,0,sizeof(gl_fd));
	memset(&gl_task,0, sizeof(gl_task));
  while(-1 != (o = getopt(argc, argv, "p:hvt"))) {
		switch(o) {
		case 'p': if( atoi(optarg )>0 ) g_listen_sock=atoi(optarg);break;
//		case 'D':	dont_daemonize = 1; break;
		case 'v': show_version(); return 0;
		case 'h': show_help(); return 0;
		default:
			show_help();
			return -1;
		}
	}
//	if (dont_daemonize) daemonize();

	dcs_debug(0,0,"<%s> begin entery do_loop",__FUNCTION__);
	do_loop();
	_srv_exit(0);
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



void _srv_exit( int signo)
{
	int i;
	
	if ( signo == 40 )
	{
		  
			if (gs_run_mode)
			{
				dcs_log(0,0,"open log debug mode ! signo=[%d]",signo);
				gs_run_mode=0;
			}
			else
			{
				dcs_log(0,0,"close log debug mode ! signo=[%d]",signo);
				gs_run_mode=1;
			}
			return;
	}
	dcs_log(0,0,"system  exit! signo=[%d]",signo);
	if( signo== 15 )
	{
		 stop=1;
		 return;
	}
	 for ( i=0; i < FD_SETSIZE;i++)
   {
  	 if ( gl_fd[i].used )
  	 {
  	 	 close(gl_fd[i].sockfd);
  	 	 gl_fd[i].used=0;
  	 }
   }
	 exit(0);
}

void do_loop()
{
	
  
  struct timeval tv;
  
  int i ,sock,loc_flg=0,ret,error,len,sl_sock;
  int s,flags,l,vmax_fd=0;
  unsigned char buffer[8192],tmp[256];
 
  loop_init:
  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&error_set);
  stop =0;
  max_fd=0;
  
    sock=tcp_open_server(NULL,g_listen_sock);
	  if ( sock < 0)
	  {
       dcs_log(0,0,"<%s>open listen port  error[%d]:%s",__FUNCTION__,errno,strerror(errno));
       return;
	  }
	  else
	  {
	  	  dcs_debug(0,0,"connect ftpserver succ! sock=%d ",sock);
	  	  add_to_set(sock, &read_set, &max_fd);
	  	  add_to_set(sock, &error_set, &max_fd);
	  	  gl_fd[sock].sockfd=sock;
        gl_fd[sock].type = '0'; //����˹�����·
        gl_fd[sock].used=1;
	  }
	  
	  dcs_debug(0,0,"begin entry select while!");
    while(1)
    { 
       
       if ( stop ) break;
    
    	tv.tv_sec = 5;
      tv.tv_usec = 0;
      vread_set=read_set;
      vwrite_set=write_set;
      verror_set=error_set;
//      vmax_fd=0;
//      if ( vmax_fd <max_fd) max_fd=vmax_fd;
    	ret=select(max_fd + 1, &vread_set, &vwrite_set, &verror_set, &tv);
//      dcs_debug(0,0," recv select event ret=%d !",ret);
//      sleep(1);
      if ( ret < 0) 
    	{
	    	 dcs_log(0,0,"select error![%s],max_fd=%d",strerror(errno),max_fd);
	    
      }
      else if ( ret == 0)
      {
          process_list_command();
          process_timeout();
           
      } 
      else 
      {
        i=0;
        do{
//	        dcs_debug(0,0,"<%s>select ret =%d ,i=%d",__FUNCTION__,ret,i);

	    	  if ( gl_fd[i].used )
	    	  {
//	          dcs_debug(0,0,"sock=%d",i);
	          if(FD_ISSET(i, &vread_set))
	    	 	  {
	    	 	  	 ret--;
    	 	  	   dcs_debug(0,0,"enter read set proccess! type=[%02x]",gl_fd[i].type); 
    	 	  	   if( gl_fd[i].type == '0') // �����������󷽽���
    	 	  	   {
								process_type_0(i);
    	 	  	 	 }
    	 	  	   else if( gl_fd[i].type == '1') // ������������
    	 	  	   {
								process_type_1(i);
    	 	  	 	 }
    	 	  	 	 else if( gl_fd[i].type == '2')// ���������·
    	 	  	 	 {
                  process_type_2(i);
    	 	  	 	 }
    	 	  	 	 else if( gl_fd[i].type == '4') // �ļ����ݴ�����·
    	 	  	 	 {
    	 	  	 	 	 process_type_3(i);
    	 	  	 	 }
    	 	  	 	 else
    	 	  	 	 	 dcs_log(0,0,"<%s> can not match! type=[%02x]",__FUNCTION__,gl_fd[i].type);
	    	 	  }
	    	 	  else if(FD_ISSET(i, &vwrite_set))
	    	 	  {
	    	 	  	 ret--;
	    	 	  	 if( gl_fd[i].type == '3'||  gl_fd[i].type == '2')
	    	 	  	 	 process_sendtype_0( i);
    	 	  	   else if( gl_fd[i].type == '4')
	    	 	  	 	 process_sendtype_1(i);
	    	 	  	 else
	    	 	  	 	dcs_log(0,0,"<%s>can not recognition type=[%c],sock=%d",__FUNCTION__,gl_fd[i].type,i);
	    	 	  	
	    	 	  }
	    	 	  else if(FD_ISSET(i, &verror_set))
	    	 	  {
	    	 	  	ret--;
//	    	 	  	fprintf(stderr,"enter error set proccess!\n");
	    	 	  	dcs_log(0,0," select error sock i=%d",i);
	    	 	  }
	    	 	}
	   	 	  i++;

        }while(ret >0 );
        process_list_command();
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
/**
��Ϊ����˽��ܽ�����������socket����
**/
int process_type_0(int sock)
{
	int client_addr, client_port,new_sock;
	if( gl_fd[sock].type !='0') return -1;
	
	new_sock = tcp_accept_client(sock,&client_addr, &client_port);
	if( new_sock < 0 ) return 0;
	gl_fd[new_sock].used=1;
	gl_fd[new_sock].data_len=0;
	gl_fd[new_sock].timeout=60*60;
	gl_fd[new_sock].type= '1';
	gl_fd[new_sock].ndx=-1;
	gl_fd[new_sock].status='0';
	add_to_set(new_sock, &read_set, &max_fd);
	dcs_debug(0,0,"<%s> add new client accept sock[%d]!",__FUNCTION__,new_sock);
}

/**
�����������󣬲����������б�
**/

int process_type_1(int sock)
{
	 	int flag,len;
 	  char *p=NULL;
 	  struct FTP_TASK task;
 	  dcs_debug(0,0,"recving from sock=[%d],type=[%d]",sock,gl_fd[sock].type);
 	  len = read(sock,gl_fd[sock].buf,sizeof(gl_fd[sock].buf));
 	  if ( len <=0 )
 	  {
 	  	if (len <0)
 	  	if (errno == EINTR || errno == EAGAIN) return 0;
 	  	dcs_log(0,0,"recv server data error![%s]",strerror(errno));
 	  	FD_CLR(sock,&read_set);
	 	  FD_CLR(sock,&error_set);
	 	  close(sock);
	 	  gl_fd[sock].used=0x00;
      return -1;
 		}
 		dcs_log(gl_fd[sock].buf,len ,"recv input data len=%d \n",len);
 		gl_fd[sock].buf[len]=0x00;
 		memset(&task,0,sizeof(task));
	  for(p=strtok(gl_fd[sock].buf,"|"),flag=0; p;p=strtok(NULL,"|"))
	  {
	  	if( flag == 0 ) // �ͻ�������, 00 �����ļ����� 01��ѯ���� 02 ɾ������ 03�ϴ��ļ�����
	  	{
	  		if( strcmp(p,"00")== 0)	 task.cmd='0';
	  		else if( strcmp(p,"01")== 0)	task.cmd='1';
	  		else if( strcmp(p,"02")== 0)	task.cmd='2';
	  		else if( strcmp(p,"03")== 0)	task.cmd='3';
	  		else break;
	  		flag++;
	  	}
	  	else if( flag == 1 ) //Զ��������
	  	{
	  		strcpy(task.host_name,p);
	  		flag++;
	  	}
	  	else if( flag == 2 ) //Զ�������˿�
	  	{
	  		task.host_port=atoi(p);
	  		if( task.host_port <=0 ) break;
	  		flag++;
	  	}
	  	else if( flag == 3 ) // Զ��·��
	  	{
	  		strcpy(task.remote_path,p);
	  		flag++;
	  	}
	  	else if( flag == 4 ) //Զ���ļ���
	  	{
	  		strcpy(task.remote_file_name,p);
	  		flag++;
	  	}
	  	else if( flag == 5 ) // �û���
	  	{
	  		strcpy(task.user_name,p);
	  		flag++;
	  	}
	  	else if( flag == 6 ) // ����
	  	{
	  		strcpy(task.password,p);
	  		flag++;
	  	}
	  	else if( flag == 7 ) // �����ļ�·��
	    {
	    	strcpy(task.local_path,p);
	  		flag++;
	    }
	    else if( flag == 8 ) // ����ʱʱ��
	    {
	    	task.timeout= atol(p);
	    	if( task.timeout <=0) break;
	  		flag++;
	    }
	    else if( flag == 9 ) // �����ļ�����
	    {
	    	strcpy(task.local_name,p);
	  		flag++;
	    }
	    else break;
	  }
	  if( task.cmd=='0' && flag >7 || task.cmd=='3' && flag >8)
	  {
	  		if( 0< create_new_console_session(sock,&task))
	  				resp_succ_msg(sock,task.cmd,&task);
	  }
	  else if( task.cmd =='1' && flag >3) return query_task(sock,&task);
	  else if( task.cmd =='2' && flag >3) return delete_task(&task,1,"");
	  else return resp_err_msg(sock,task.cmd,&task,5,"can not parse message!");
}
/**
������������������ƻ���һ������ָ��
**/
int process_type_2(int sock)
{
 	  int len,ret,fd;
 	  char buf[1024];
 	  dcs_debug(0,0,"recving from sock=[%d],type=[%d]",sock,gl_fd[sock].type);
 	  len = recv(sock,gl_fd[sock].buf,sizeof(gl_fd[sock].buf),0);
 	  if ( len <=0 )
 	  {
 	  	if (len <0)
 	  	if (errno == EINTR || errno == EAGAIN) return 0;
 	  	if (len == 0)
 	  		dcs_log(0,0,"recv  ftp server data end![%s]",strerror(errno));
 	  	else
 	  		dcs_log(0,0,"recv  ftp server data error![%s]",strerror(errno));
 	  	FD_CLR(sock,&read_set);
	 	  FD_CLR(sock,&error_set);
	 	  close(sock);
	 	  gl_fd[sock].used=0x00;
      return -1;
 		}
 		gl_fd[sock].buf[len]=0x00;
 		dcs_log(gl_fd[sock].buf,len ,"recv ftp server data len=%d status=[%c]",len,gl_fd[sock].status);
// 		fprintf(stderr,"%s",gl_fd[sock].buf);
 		if( memcmp("220",gl_fd[sock].buf,3)==0 && gl_fd[sock].status=='0')
 		{
	 		if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  		gl_fd[sock].command=get_order_list[0];
	  	else
	  		gl_fd[sock].command=put_order_list[0];
	  	gl_fd[sock].order_ndx;
	  	gl_fd[sock].status='1';
	  	len=command_pkg(gl_fd[sock].ndx,gl_fd[sock].command,gl_fd[sock].buf, sizeof(gl_fd[sock].buf));
			if( len > 0 )
			{
					if(0<( ret=write(sock,gl_fd[sock].buf,len)))
						gl_fd[sock].status='3';	
					else
						dcs_log(0,0,"<%s> write socket fail![%s]",__FUNCTION__,strerror(errno));
					if( ret >0 )
						dcs_log(0,0,"<%s> send data=[%d][%s]",__FUNCTION__,ret,gl_fd[sock].buf);			
			}
			else
			{
				dcs_log(0,0,"<%s>command_pkg fail! command=[%d]",__FUNCTION__,gl_fd[sock].command);
				return -1;
			}
		  return 1;
  	}
   
 		if( gl_fd[sock].command == TOK_PASV ) // ��ftp������ṩ���ݴ���˿�
 		{
 			if( memcmp(gl_fd[sock].buf,"227",3)==0)
 			{
 				int sport,new_sock;
 				char *p;
 				p=strtok(gl_fd[sock].buf,",");
 				p=strtok(NULL,",");
 				p=strtok(NULL,",");
 				p=strtok(NULL,",");
 				p=strtok(NULL,",)");
 				fprintf(stderr,"first p=%s\n",p);
 				sport= atoi(p)*256;
 				p=strtok(NULL,",)");
 				fprintf(stderr,"second p=%s\n",p);
 				sport=sport+atoi(p);
 				fprintf(stderr,"sport=%d\n",sport);
 				
 				new_sock=tcp_connet_server(gl_task[gl_fd[sock].ndx].host_name,sport,0);
				
 				if( new_sock > 0)
 				{
 					gl_fd[new_sock].sockfd=new_sock;
 					gl_fd[new_sock].type ='3'; //���ݴ�����·
 					add_to_set(new_sock, &write_set, &max_fd);    	 	  	 					
	 	      add_to_set(new_sock,&error_set, &max_fd);
	 	      gl_fd[new_sock].used=1;
	 	      gl_fd[new_sock].status='0';
	 	      gl_fd[new_sock].data_len=0;
	 	      gl_fd[new_sock].off_set=0;
	 	      gl_fd[new_sock].timeout=20;
	 	      gl_fd[new_sock].begin_time=time(NULL);
	 	       gl_fd[new_sock].last_time=time(NULL);
	 	      dcs_debug(0,0,"data link pre connect succ! sock=%d remote_port=[%d]",new_sock,sport);
	 	      if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  				gl_fd[sock].command=get_order_list[++gl_fd[sock].order_ndx];
	  	  	else
	  				gl_fd[sock].command=put_order_list[++gl_fd[sock].order_ndx];
	  			gl_task[gl_fd[sock].ndx].socket_data=new_sock;
	  			len=sprintf(buf,"%s/%s",gl_task[gl_fd[sock].ndx].local_path,gl_task[gl_fd[sock].ndx].local_name);
					if(buf[len-1]==0x0d || buf[len-1]==0x0a) {buf[len-1]=0x00; len--;}
					if(buf[len-1]==0x0d || buf[len-1]==0x0a) {buf[len-1]=0x00; }
					strcat(buf,".tmp");
					if( gl_task[gl_fd[sock].ndx].cmd=='0') // ����
						fd= open(buf,O_WRONLY|O_CREAT|O_TRUNC,0666);
					else
						fd= open(buf,O_RDONLY); 
					if( fd > 0 )
						gl_fd[new_sock].file_id=fd;
					else // �����޷�����
					{
						return delete_task(&gl_task[gl_fd[sock].ndx],1,NULL);
					}
					
 				}
 				else
 				  fprintf(stderr,"data link connect fail!\n"); 
 					
 			}
 		}
 		else if( gl_fd[sock].command == TOK_USER )
 		{
 			if( memcmp("331",gl_fd[sock].buf,3)==0 && gl_fd[sock].status=='3') //�û�����
 			{
 				if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  			gl_fd[sock].command=get_order_list[++gl_fd[sock].order_ndx];
	  	  else
	  			gl_fd[sock].command=put_order_list[++gl_fd[sock].order_ndx];
 			}
 			else if( memcmp("230",gl_fd[sock].buf,3)==0) //�û��ѵ�½
 			{
 				gl_fd[sock].order_ndx++;
 				if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  			gl_fd[sock].command=get_order_list[++gl_fd[sock].order_ndx];
	  	  else
	  			gl_fd[sock].command=put_order_list[++gl_fd[sock].order_ndx];
	  		
 			}
 			else //�������������������޷���������������б�
 			{
 				FD_CLR(sock,&read_set);
		 	  FD_CLR(sock,&error_set);
		 	  close(sock);
		 	  gl_fd[sock].used=0x00;
		 	  return delete_task(&gl_task[gl_fd[sock].ndx],1,NULL);
     		
 			}
 			gl_fd[sock].status='2';
 			
 		}
 		else if( gl_fd[sock].command == TOK_PASS )
 		{
 			if( memcmp("530",gl_fd[sock].buf,3)==0) 
 			{
 				if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  			gl_fd[sock].command=get_order_list[--gl_fd[sock].order_ndx];
	  	  else
	  			gl_fd[sock].command=put_order_list[--gl_fd[sock].order_ndx];
 			}
 			else if( memcmp("230",gl_fd[sock].buf,3)==0) 
 			{
 				if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  			gl_fd[sock].command=get_order_list[++gl_fd[sock].order_ndx];
	  	  else
	  			gl_fd[sock].command=put_order_list[++gl_fd[sock].order_ndx];
 			}
 			else //�������������������޷���������������б�
 			{
 				FD_CLR(sock,&read_set);
		 	  FD_CLR(sock,&error_set);
		 	  close(sock);
		 	  gl_fd[sock].used=0x00;
		 	  return delete_task(&gl_task[gl_fd[sock].ndx],1,NULL);
     	}
 		}
 		else if( gl_fd[sock].command == TOK_CWD ) // �ı�Զ��������ǰĿ¼Ӧ����
 		{
 			if( memcmp("250",gl_fd[sock].buf,3)==0)
 			{
 				if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  			gl_fd[sock].command=get_order_list[++gl_fd[sock].order_ndx];
	  	  else
	  			gl_fd[sock].command=put_order_list[++gl_fd[sock].order_ndx];
 		  }
 		  else //�������������������޷���������������б�
 			{
 				FD_CLR(sock,&read_set);
		 	  FD_CLR(sock,&error_set);
		 	  close(sock);
		 	  gl_fd[sock].used=0x00;
		 	  return delete_task(&gl_task[gl_fd[sock].ndx],1,NULL);
 			}
 		}
 		else if( gl_fd[sock].command == TOK_TYPE ) // ����ģʽ ������
 		{
 			if( memcmp("200",gl_fd[sock].buf,3)==0)
 			{
 				if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  			gl_fd[sock].command=get_order_list[++gl_fd[sock].order_ndx];
	  	  else
	  			gl_fd[sock].command=put_order_list[++gl_fd[sock].order_ndx];
 		  }
 		  else //�������������������޷���������������б�
 			{
 				FD_CLR(sock,&read_set);
		 	  FD_CLR(sock,&error_set);
		 	  close(sock);
		 	  gl_fd[sock].used=0x00;
		 	  return delete_task(&gl_task[gl_fd[sock].ndx],1,NULL);
 			}
 		}
 		else if( gl_fd[sock].command == TOK_PWD ) // ����Զ����������Ŀ¼
 		{
 			if( memcmp("257",gl_fd[sock].buf,3)==0)
 			{
 				if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  			gl_fd[sock].command=get_order_list[++gl_fd[sock].order_ndx];
	  	  else
	  			gl_fd[sock].command=put_order_list[++gl_fd[sock].order_ndx];
 		  }
 		  else //�������������������޷���������������б�
 			{
 				FD_CLR(sock,&read_set);
		 	  FD_CLR(sock,&error_set);
		 	  close(sock);
		 	  gl_fd[sock].used=0x00;
		 	  return delete_task(&gl_task[gl_fd[sock].ndx],1,NULL);
 			}
 		}
 		else if( gl_fd[sock].command == TOK_RETR ) // get �ļ�
 		{
 			  /*
 			   110  �����������Ӧ��
             �ڴ�����£������Ǿ�ȷ�ģ����Ҳ��������ض���ʵ�ִ����������ȡ��
             MARK yyyy=mmmm
             ����yyyy���û����̵���������ǣ�mmmm�Ƿ������ϵ���Ӧ��ǣ���ע���Ǻ͡�=��֮��Ŀո񣩡�
         125  ���������Ѵ򿪣�����������
         150  �ļ�״̬û���⣬׼�����������ӡ�
         226  �ر��������ӣ�������ļ������ѳɹ���
         250  ������ļ������������У�����ɡ�
         421  ���񲻿��ã��رտ������ӡ�
              ���ĳ������֪�Լ������رգ��������������������Ӧ��
         425  �޷����������ӡ�
         426  ���ӹرգ�������ֹ��
         450  ������ļ������޷�ִ�У��ļ������ã����磬�ļ���æ����
         451  ����Ĳ�������ֹ�������з������ش���
         500  �﷨�����޷�ʶ�����
             �����а��������й���֮��Ĵ���
         501  �������Ԫ�����﷨����
         504  �����л�û��ʵ�ָò�����
         530  �޷���¼��
         550  ����Ĳ����޷�ִ�У��ļ������ã������Ҳ����ļ����޷���Ȩ����

 			  */
 			if( memcmp("226",gl_fd[sock].buf,3)==0) // �������
 			{
 				char tmp[256];
 				if( gl_task[gl_fd[sock].ndx].cmd =='0')
	  			gl_fd[sock].command=get_order_list[++gl_fd[sock].order_ndx];
	  	  else
	  			gl_fd[sock].command=put_order_list[++gl_fd[sock].order_ndx];
	  		// ���ļ���������ɾ�������б���Ӧ��������
	  		close(gl_fd[gl_task[gl_fd[sock].ndx].socket_data].file_id);
	  		len=sprintf(buf,"%s/%s",gl_task[gl_fd[sock].ndx].local_path,gl_task[gl_fd[sock].ndx].local_name);
				if(buf[len-1]==0x0d || buf[len-1]==0x0a) {buf[len-1]=0x00; len--;}
				if(buf[len-1]==0x0d || buf[len-1]==0x0a) {buf[len-1]=0x00; }
				strcat(buf,".tmp");
				strcpy(tmp,buf);
				tmp[strlen(tmp)-4]=0x00;
	  		rename(buf, tmp);
 		  }
 		  else //�������������������޷���������������б�
 			{
 				FD_CLR(sock,&read_set);
		 	  FD_CLR(sock,&error_set);
		 	  close(sock);
		 	  if( gl_task[gl_fd[sock].ndx].socket_data >0 )
		 	  {
		 	  	FD_CLR(gl_task[gl_fd[sock].ndx].socket_data,&read_set);
		 	    FD_CLR(gl_task[gl_fd[sock].ndx].socket_data,&error_set);
		 	    close(gl_task[gl_fd[sock].ndx].socket_data);
		 	    gl_fd[gl_task[gl_fd[sock].ndx].socket_data].used=0x00;
		 	  }
		 	  dcs_log(0,0,"<%s> TOK_RETR error! [%s] ",__FUNCTION__,gl_fd[sock].buf);
		 	  gl_fd[sock].used=0x00;
		 	  delete_task(&gl_task[gl_fd[sock].ndx],1,NULL);
     		return -1;
 			}
 		}
 		else if( gl_fd[sock].command == TOK_MKD )//�ڷ���������Ŀ¼
 		{
 			/*
  		   257  ��PATHNAME���Ѵ�����
         421  ���񲻿��ã��رտ������ӡ�
              ���ĳ������֪�Լ������رգ��������������������Ӧ��
         500  �﷨�����޷�ʶ����������а��������й���֮��Ĵ���
         501  �������Ԫ�����﷨����
         502  ���û�б�ʵ�֡�
         530  �޷���¼��
         550  ����Ĳ����޷�ִ�У��ļ������ã������Ҳ����ļ����޷���Ȩ����

 			*/
 		}
 		else if( gl_fd[sock].command == TOK_RNFR )//�������ļ����������̵�ǰһ�롣ָ��Ҫ���������ļ��ľ�·�����ļ���
 		{
 			/*
 			RNFR������������һ��ָ����·�����ļ�����"Rename to"���RNTO����
         ����ֵ���£������ʾ�ɹ�����
         ����Ӧ����ʾ�ɹ�
         350  ������ļ������ڵȴ�����һ������Ϣ��
         421  ���񲻿��ã��رտ������ӡ�
             ���ĳ������֪�Լ������رգ��������������������Ӧ��
         450  ������ļ������޷�ִ�У��ļ������ã������ļ���æ����
         500  �﷨�����޷�ʶ����������а��������й���֮��Ĵ���
         501  �������Ԫ�����﷨����
         502  ���û�б�ʵ�֡�
         530  �޷���¼��
         550  ����Ĳ����޷�ִ�У��ļ������ã������Ҳ����ļ����޷���Ȩ����
 			*/
 		}
 		else if( gl_fd[sock].command == TOK_RNTO )//�������ļ����������̵ĺ�һ�롣ָ��Ҫ���������ļ�����·�����ļ�����
 		{
 			/*
 			  250  ������ļ������������У�����ɡ�
         421  ���񲻿��ã��رտ������ӡ�
          ���ĳ������֪�Լ������رգ��������������������Ӧ��
         500  �﷨�����޷�ʶ����������а��������й���֮��Ĵ���
         501  �������Ԫ�����﷨����
         502  ���û�б�ʵ�֡�
         503  �����˳�򲻶ԡ�
         530  �޷���¼��
         532  �ļ��Ĵ洢��Ҫ�ʺš�
         553  ����Ĳ����޷�ִ�У���������ļ�����
			*/
 		}
 		else if( gl_fd[sock].command == TOK_STOR )//�����ļ���������
 		{
 			/*
 			ע�ͣ�Store�����÷���������ͨ���������Ӵ�����������ݣ��������ݴ�Ϊ����վ���ϵ��ļ��������·������ָ�����ļ��Ѿ��ڷ���վ���ϴ��ڣ�����ļ������ݽ����������������������������·������ָ�����ļ���δ���ڣ�������һ�����ļ���
     ����ֵ���£�ֵ���ʾ�ɹ�����
         110  �����������Ӧ��
           �ڴ�����£��ı��Ǿ�ȷ�ģ����Ҳ������������ʵ�ִ����������ȡ��
           MARK yyyy=mmmm
           ����yyyy���û����̵���������ǣ�mmmm�Ƿ������ϵ���Ӧ���ǣ���ע���Ǻ͡�=��֮��Ŀո񣩡�
        125  ���������Ѵ򿪣�����������
        150  �ļ�״̬һ��������׼�����������ӡ�
        226  �ر��������ӣ�������ļ������ѳɹ���
        250  ������ļ������������У�����ɡ�
        421  ���񲻿��ã��رտ������ӡ�
          ���ĳ������֪�Լ������رգ��������������������Ӧ��
         425  �޷����������ӡ�
         426  ���ӹرգ�������ֹ��
         450  ������ļ������޷�ִ�У��ļ������ã������ļ���æ��
         451  ����Ĳ�������ֹ�������з������ش���
         452  ����Ĳ����޷�ִ�У�ϵͳ�Ĵ洢�ռ䲻�㡣
         500  �﷨�����޷�ʶ����������а��������й���֮��Ĵ���
         501  �������Ԫ�����﷨����
         504  �����л�û��ʵ�ָò�����
         530  �޷���¼��
         532  �ļ��Ĵ洢��Ҫ�ʺš�
         550  ����Ĳ����޷�ִ�У��ļ������ã������Ҳ����ļ����޷���Ȩ����
         551  ����Ĳ�������ֹ��δ֪��ҳ���͡�
         552  ������ļ�����������ֹ�������˷���Ĵ洢��Ԫ���Ե�ǰĿ¼�����ݼ����ԣ���
         553  ����Ĳ����޷�ִ�У���������ļ�����

 			*/
 			if( memcmp("125",gl_fd[sock].buf,3)==0 ||
 				  memcmp("150",gl_fd[sock].buf,3)==0 
 				 )
 			{
	 			if( gl_fd[gl_task[gl_fd[sock].ndx].socket_data].status =='1')
	 			{
	 					add_to_set(gl_task[gl_fd[sock].ndx].socket_data, &write_set, &max_fd);
	 					gl_fd[gl_task[gl_fd[sock].ndx].socket_data].status ='2';
	 			}
 			} 
 		}
 		else
 	  {
 	  	dcs_log(0,0,"<%s> �޷�������ָ��! command=[%d]",__FUNCTION__,gl_fd[sock].command);
 	  	return -1 ;
 	  }
 		len=command_pkg(gl_fd[sock].ndx,gl_fd[sock].command,gl_fd[sock].buf, sizeof(gl_fd[sock].buf));
		if( len > 0 )
		{
				if(0<( ret=write(sock,gl_fd[sock].buf,len)))
					gl_fd[sock].status='3';	
				else
					dcs_log(0,0,"<%s> write socket fail![%s]",__FUNCTION__,strerror(errno));
				if( ret >0 )
					dcs_log(0,0,"<%s> send data=[%d][%s]",__FUNCTION__,ret,gl_fd[sock].buf);			
		}
		else
		{
			dcs_log(0,0,"<%s>command_pkg fail! command=[%d]",__FUNCTION__,gl_fd[sock].command);
			return -1;
		}
	 return 1;
}
/**
���ڽ��մ�����������,�������ļ�
**/
int process_type_3( int sock)
{
	int ret,len;
	  dcs_log(0,0,"<%s>enter data recv link type=[%c] sock=[%d]",__FUNCTION__,gl_fd[sock].type,sock);
 	  len = read(sock,gl_fd[sock].buf,sizeof(gl_fd[sock].buf));
 	  if ( len <=0 )
 	  {
 	  	if (len <0)
 	  	if (errno == EINTR || errno == EAGAIN) ;
 	  	
      if ( len == 0) //�ļ����ս���
 	  	{
  	  		//�Կ�����·������
  	  		gl_task[gl_fd[sock].ndx].status = 4;
  	  		// �������б����Ƴ�����
  	  		dcs_log(0,0,"recv  ftp server data end![%s]",strerror(errno));
  	  		// ���������������
 	  	}
 	  	else
 	  	{
 	  		  gl_task[gl_fd[sock].ndx].socket_data=-1;
 	  		  
 	  		  //����ftp server �ϵ������ļ�
 	  		  gl_fd[gl_task[gl_fd[sock].ndx].socket_cmd].command=TOK_REST;
 	  		  dcs_log(0,0,"recv  ftp server data error![%s]",strerror(errno));
 	  		 // �ر��ļ����
 	  		 // ���������·�ش������ļ��������Ƿ���ʵ�ֶϵ�����
 	  	}
      FD_CLR(sock,&read_set);
	 	  FD_CLR(sock,&error_set);
	 	  close(sock);
	 	  gl_fd[sock].used=0x00;
     
      return 0;
 		}
 		gl_fd[sock].last_time=time(NULL);
 		gl_fd[sock].buf[len]=0x00;
 		dcs_debug(gl_fd[sock].buf,len ,"<%s>recv ftp server data len=%d \n",__FUNCTION__,len);
 		ret=write( gl_fd[sock].file_id,gl_fd[sock].buf,len);
 		if( ret != len)
 		{
 			//д�ļ������쳣������
 		}
 		
//	fprintf(stderr,"%s\n",gl_fd[i].buf);
		    	 	  	 		
}

/**
���ڶԽ���socket�ľ������������߶Խ���ʧ�ܵ�socket������
**/
int process_sendtype_0(int sock)
{
	int len,i,error;
//	fprintf(stderr,"<%s>enter write set proccess! sock=[%d]",__FUNCTION__,sock);
	len=sizeof(error);
	getsockopt(sock,SOL_SOCKET,SO_ERROR,&error,&len) ;
	if( 0 != error )  { 
     dcs_log(0,0,"  connect server socket error!addr=[%s]:[%d][%s]",
         gl_task[i].host_name,gl_task[i].host_port,strerror(errno));	
     close(sock);
     gl_fd[sock].used=0x00;
     gl_fd[sock].data_len=0;
     if( gl_fd[sock].file_id >0 ) close(gl_fd[sock].file_id);
     FD_CLR(sock,&error_set);	 
     i=gl_fd[sock].ndx;	  	 	         
     return 0;   
  }  

  if( gl_fd[sock].type=='3'  ) //���ݴ�����·����
  {
  	 gl_fd[sock].status='1'; //����״̬
  	 gl_fd[sock].command=-1;
  	 gl_fd[sock].data_len=0;
  	 gl_fd[sock].timeout=60;
  	 // �������������ټ��д״̬
 // 	 if( gl_task[gl_fd[sock].ndx].cmd='0' )	 FD_CLR(sock,&write_set);
     FD_CLR(sock,&write_set);
  	 gl_fd[sock].type='4';
  	 gl_fd[sock].begin_time=time(NULL);
  	 gl_fd[sock].last_time=time(NULL);
  	 dcs_debug(0,0,"<%s> data link socket[%d] is ready  ",__FUNCTION__,sock); 	 
  }
  else if( gl_fd[sock].type=='2') //���������·����
  {
//  	gl_fd[sock].status='1'; 
/*  	if( gl_task[gl_fd[sock].ndx].cmd =='0')
  		gl_fd[sock].command=get_order_list[0];
  	else
  		gl_fd[sock].command=put_order_list[0];
*/
  	gl_fd[sock].order_ndx=0;	
  	gl_fd[sock].data_len=0;
  	gl_fd[sock].timeout=20;
  	gl_fd[sock].status='0';
  	gl_task[gl_fd[sock].ndx].socket_cmd=sock;
  	FD_CLR(sock,&write_set);
  }
  add_to_set(sock, &read_set, &max_fd);
  dcs_debug(0,0,"<%s> scoket[%d]link is ready!",__FUNCTION__,sock);
  return 1;
}
/**
�����ļ��ķ��͵�socket������ʱ����������

���ڷ���������FTPָ��ϴ��ļ���������
**/
int process_sendtype_1(int sock)
{
	int len;
	char buf[1024];
	dcs_debug(0,0,"<%s> begin",__FUNCTION__);
	 if( gl_fd[sock].type=='4' && gl_task[gl_fd[sock].ndx].cmd=='3')  // �ϴ��ļ�����
	 {
	 	  if( gl_fd[sock].data_len >0)
	 	  {
	 	  	    
	 	  	    len = write( sock,gl_fd[sock].buf+gl_fd[sock].off_set,gl_fd[sock].data_len); 
	 	  	 	  if(  len < 0) // ������д����ʧ��
	 	  	 	  {
	 	  	 	  	  close(sock);
	 	  	 	  	  close(gl_fd[sock].file_id);
	 	  	 	  	  dcs_log(0,0,"<%s,%d> send data error[%s]",__FUNCTION__,__LINE__,strerror(errno));
	 	  	 	  }
	 	  	 	  else if( gl_fd[sock].data_len == len )
	 	  	 	  {
	 	  	 	  		 gl_fd[sock].data_len = 0;
	 	  	 	  		 gl_fd[sock].off_set = 0;
	 	  	 	  }
	 	  	 	  else  
	 	  	 	  {	
	 	  	 	  	gl_fd[sock].data_len -= len;
	 	  	 	  	gl_fd[sock].off_set += len;
	 	  	 	  }
	 	  	 	  gl_task[gl_fd[sock].ndx].size  += len;
	 	  }
	 	  else if( gl_fd[sock].file_id  >0 )
	 	  {
	 	  	 len= read(gl_fd[sock].file_id,gl_fd[sock].buf,sizeof(gl_fd[sock].buf));
	 	  	 if( len <0 ) // ���ļ�����
	 	  	 {
	 	  	 	 close(gl_fd[sock].file_id);
	 	  	 }
	 	  	 else if ( len == 0 ) // ���ļ�����
	 	  	 {
	 	  	 		close(gl_fd[sock].file_id);
	 	  	 		FD_CLR(sock,&write_set);
	 	  	 		// ���ÿ�����·����һָ�ͨ��������·����
	 	  	 }
	 	  	 else
	 	  	 {
	 	  	 	  if( len < sizeof(gl_fd[sock].buf)) // ���ļ�����
	 	  	 	  {
	 	  	 	  	close(gl_fd[sock].file_id);
	 	  	 	  }
	 	  	 	  // �����緢���ļ�data
	 	  	 	  gl_fd[sock].data_len=len;
	 	  	 	  gl_fd[sock].off_set = write( sock,gl_fd[sock].buf,len); 
	 	  	 	  if(  gl_fd[sock].off_set < 0) // ������д����ʧ��
	 	  	 	  {
	 	  	 	  	close(sock);
	 	  	 	  	close(gl_fd[sock].file_id);
	 	  	 	  }
	 	  	 	  else if( gl_fd[sock].off_set == len )
	 	  	 	  {
	 	  	 	  		 gl_fd[sock].data_len = 0;
	 	  	 	  		 gl_fd[sock].off_set = 0;
	 	  	 	  }
	 	  	 	  else  gl_fd[sock].data_len -= gl_fd[sock].off_set;
	 	  	 	  gl_task[gl_fd[sock].ndx].size  += gl_fd[sock].off_set;
	 	  	 }
	 	  }
	 }
	 else if( gl_fd[sock].type=='2') // ���������· ,��Ҫ������һָ��
	 {
	 	 if( gl_fd[sock].status =='4') // ���ʹ�õ��������Ѿ��յ��ҳɹ�
	 	 {
	 	 	  if( gl_task[gl_fd[sock].ndx].cmd =='0')
	 	 	  	gl_fd[sock].command=get_order_list[gl_fd[sock].ndx++];
	 	 	  else
	 	 	  	gl_fd[sock].command=put_order_list[gl_fd[sock].ndx++];
	 	 	 //�Է��͵ĸ����������������
	 	 	 len = command_pkg(gl_fd[sock].ndx,gl_fd[sock].command,buf, sizeof(buf));
	 	 	 //����ָ��
	 	 	 if ( len > 0)
	 	 	 {
	 	 	 	  len= write(sock,buf,len);
	 	 	 	  if ( len <=0)
	 	 	 	  {
	 	 	 	  	 dcs_log(0,0,"<%s> send first command fail! [%s]" ,__FUNCTION__,strerror(errno));
	 	 	 	  	 close(sock);
	 	 	 	  	 gl_fd[sock].used=0x00;
	 	 	 	  	 FD_CLR(sock,&write_set);
	 	 	 	  	 FD_CLR(sock,&read_set);
	 	 	 	  	 FD_CLR(sock,&error_set);
	 	 	 	  	 return -1;
	 	 	 	  }
	 	 	 }
	 	 }
	  
	 	 // ���write set ���
	 	 FD_CLR(sock,&write_set);
	 }
	 return 1;
}

/**
��������б����һ�����Ƿ���Ҫ��������
**/

int process_list_command()
{
	
}
/**
�Գ�ʱsocket������
**/
void process_timeout ()
{
	return;
}
int create_new_console_session(int sock,struct FTP_TASK *task)
{
	char buf[1024];
	int len,fd;
	if( task  ==  NULL ) return -1;
	if( used_task_list == MAX_TASK )
	{
		 // Ӧ������,ϵͳ����æ�����Ժ�����
		 len = snprintf(buf,sizeof(buf),"01|%s|%s|%s|ϵͳ��æ,���Ժ�����!",
		       task->host_name,task->remote_path,task->remote_file_name);
		 if( 0>= write(sock,buf,len))
		 {
		 		dcs_log(0,0,"����Ӧ�����������ʧ��![%s|%s|%s]",task->host_name,task->remote_path,task->remote_file_name);
		 		close(sock);
		 		FD_CLR(sock,&read_set);
	 	    FD_CLR(sock,&error_set);
	 	    gl_fd[sock].used=0x00;
	 	    return -1;
		 }
		 return 0;
	}
	// ���������������ĺϷ������ļ��ķ���Ȩ��
	len=sprintf(buf,"%s/%s",task->local_path,task->local_name);
	if(buf[len-1]==0x0d || buf[len-1]==0x0a) {buf[len-1]=0x00; len--;}
	if(buf[len-1]==0x0d || buf[len-1]==0x0a) {buf[len-1]=0x00; }
	strcat(buf,".tmp");
	if( task->cmd=='0') // ����
		fd= open(buf,O_WRONLY|O_CREAT|O_TRUNC,0666);
	else
		fd= open(buf,O_RDONLY);
	if( fd <0)
	{
		
		dcs_log(0,0,"<%s>cmd=[%c] [%s]|%s ",__FUNCTION__,task->cmd,buf,strerror(errno));
		len = snprintf(buf,sizeof(buf),"01|%s|%s|%s|ϵͳ�������ܲ���ȷ![%s]",
		       task->host_name,task->remote_path,task->remote_file_name,strerror(errno));
		 if( 0>= write(sock,buf,len))
		 {
		 		dcs_log(0,0,"����Ӧ�����������ʧ��![%s|%s|%s]",task->host_name,task->remote_path,task->remote_file_name);
		 		close(sock);
		 		FD_CLR(sock,&read_set);
	 	    FD_CLR(sock,&error_set);
	 	    gl_fd[sock].used=0x00;
	 	    return -1;
		 }
		return 0;
	}
	else close(fd);
	// �������񻺴�������λ��
	if( gl_task[last_task_ndx].used)
	{
		len=last_task_ndx;
		for( last_task_ndx++; last_task_ndx!=len ; last_task_ndx++)
	  {
	  	 if( last_task_ndx == MAX_TASK ) last_task_ndx =0;
	  	 if( !gl_task[last_task_ndx].used) break;
	  }
	  if( last_task_ndx == len )
	  {
	  	 len = snprintf(buf,sizeof(buf),"01|%s|%s|%s|<%s>ϵͳ�쳣,����ϵ������Ա!",
			       task->host_name,task->remote_path,task->remote_file_name,__FUNCTION__);
			 if( 0>= write(sock,buf,len))
			 {
			 		dcs_log(0,0,"<%s>����Ӧ�����������ʧ��![%s|%s|%s]",__FUNCTION__,task->host_name,task->remote_path,task->remote_file_name);
			 		close(sock);
			 		FD_CLR(sock,&read_set);
		 	    FD_CLR(sock,&error_set);
		 	    gl_fd[sock].used=0x00;
		 	    return -1;
			 }
			 return 0;
	  }
  }
  gl_task[last_task_ndx].cmd = task->cmd;
  strcpy( gl_task[last_task_ndx].user_name,task->user_name ); 
  strcpy( gl_task[last_task_ndx].password,task->password );
  strcpy( gl_task[last_task_ndx].remote_path,task->remote_path );
  strcpy( gl_task[last_task_ndx].remote_file_name,task->remote_file_name );
  strcpy( gl_task[last_task_ndx].local_path,task->local_path );
  strcpy( gl_task[last_task_ndx].local_name,task->local_name );
  strcpy( gl_task[last_task_ndx].host_name,task->host_name );
  gl_task[last_task_ndx].used = 0x31;
  gl_task[last_task_ndx].host_port = task->host_port;
  gl_task[last_task_ndx].status = 0x30;
  gl_task[last_task_ndx].socket_cmd = -1;
  gl_task[last_task_ndx].socket_data = -1;
  gl_task[last_task_ndx].accept_time = time(NULL);
  gl_task[last_task_ndx].begin_time = -1;
  gl_task[last_task_ndx].last_time = -1;
  gl_task[last_task_ndx].timeout = task->timeout;
  gl_task[last_task_ndx].size = 0;
  startover_task(&gl_task[last_task_ndx]);
  dcs_debug(0,0,"<%s>add task succ! ",__FUNCTION__);
  return 1;
}
/**
�����ϴ������ص�ftp ����
**/
int startover_task(struct FTP_TASK *task)
{
	int sock;
	
	if ( task  ==  NULL )
	{	
		 dcs_log(0,0,"<%s> task is null!",__FUNCTION__);
		 return -1;
	}
	if( task->cmd !='0' && task->cmd !='3')
	{	 
		dcs_log(0,0,"<%s>cmd=[%c]",__FUNCTION__,task->cmd);
		return -1;
  }
	//�첽����
	sock=tcp_connet_server(task->host_name,task->host_port,0);
	if( sock < 0)
	{
		dcs_log(0,0,"<%s> tcp_connect_server error![%s] ",__FUNCTION__,strerror(errno));
		return -1;
	}
	gl_fd[sock].used=0x31;
	gl_fd[sock].status='0';
	gl_fd[sock].begin_time=time(NULL);
	gl_fd[sock].last_time=time(NULL);
	gl_fd[sock].timeout=20; //����ftp���������·��Ĭ�ϳ�ʱʱ��
	gl_fd[sock].type='2';
	gl_fd[sock].data_len=0;
	add_to_set(sock, &write_set, &max_fd);
	dcs_debug(0,0,"<%s> add new control link host[%s],port[%d] sock=%d",__FUNCTION__,task->host_name,task->host_port,sock);
}

int command_pkg(int task_ndx,int command,char *buf, int size)
{
	int i,len;
	char *p;
	struct FTP_TASK *task;
	
	task =  &gl_task[task_ndx];
	for ( i=0; commandList[i].m_pszName;i++)
	 	 if( commandList[i].m_nTokenID == command ) { p=commandList[i].m_pszName; break;}
  switch ( command)
 {
  	 case TOK_USER:
  	 	len=snprintf(buf,size,"%s %s\r\n",p,task->user_name);
  	 	break;
  	 case TOK_PASS:
  	 	len=snprintf(buf,size,"%s %s\r\n",p,task->password);
  	 	break;
  	 case TOK_CWD:
  	 	rtrim(task->remote_path);
  	 	len=snprintf(buf,size,"%s %s\r\n",p,task->remote_path);
  	 	break;
  	 case TOK_PWD:
  	 	len=snprintf(buf,size,"%s\r\n",p);
  	 	break;
  	case TOK_TYPE:
  	 	len=snprintf(buf,size,"%s I\r\n",p);
  	 	break;
  	case TOK_PASV:
  	 	len=snprintf(buf,size,"%s I\r\n",p);
  	 	break;
  	 case TOK_RETR:
  	 	len=snprintf(buf,size,"%s %s\r\n",p,task->remote_file_name);
  	 	break;
  	 case TOK_MKD:
  	   len=snprintf(buf,size,"%s %s\r\n",p,task->remote_path);
  	 case TOK_STOR:
  	 	len=snprintf(buf,size,"%s %s.tmp\r\n",p,task->remote_file_name);
  	 	break;
  	 case TOK_RNFR:
  	  len=snprintf(buf,size,"%s %s.tmp\r\n",p,task->remote_file_name);
  	 	break;
  	 case TOK_RNTO:
  	  len=snprintf(buf,size,"%s %s\r\n",p,task->remote_file_name);
  	 	break;
  	 case TOK_ERROR:
  	 default:
  	 	dcs_log(0,0,"<%s> can not parse order! p=[%s]",__FUNCTION__,p);
  	   len=0;
 }
	 	 return len;
}

int  resp_succ_msg( int sock,char cmd, struct FTP_TASK *task)
{
	;
}

int query_task(int sock,struct FTP_TASK *task)
{
	return 1;
}

int  delete_task(struct FTP_TASK *task,int flag/* �Ƿ���Ӧ��ֻ���������*/,char *msg)
{
	char buf[1024],err_code[3];
	int len,sock;
	sock= task->socket_acq;
	if( flag && sock >0 )
	{
		if( msg == NULL ) strcpy( err_code,"01");
		else { memcpy(err_code,msg,2); err_code[2]=0x00;}
		len=snprintf(buf,sizeof(buf),"%02d|%s|%s|%s|%s",err_code,
		       task->host_name,task->remote_path,task->remote_file_name,msg);
		write(sock,buf,len);
	}
	for( len=0 ; len < MAX_TASK ; len++)
  {
  	if( strcmp(task->remote_path,gl_task[len].remote_path)==0 &&
  		  strcmp(task->remote_file_name,gl_task[len].remote_file_name)==0 &&
  		  strcmp(task->host_name,gl_task[len].host_name)==0 )
  		  break;
  }
  if( len != MAX_TASK)
  {
  	used_task_list++;
  	gl_task[len].used=0x00; 
  }
	return 1;
}

int resp_err_msg(int sock,char cmd,struct FTP_TASK *task, int err_code,char *err_msg)
{
	char buf[1024];
	int len;
	len=snprintf(buf,sizeof(buf),"%02d|%s|%s|%s|%s",err_code,
		       task->host_name,task->remote_path,task->remote_file_name,err_msg);
	
	return write(sock,buf,len);
}