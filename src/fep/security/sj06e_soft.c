#include  "dccdcs.h"
#include <getopt.h>
void SrvExit(int signo);
void DoService ( void *arg);
int SrvOpenLog();
int appProc(char *inBuf,int nIn,char *outBuf,int *nOut);
static void show_version (void);
static void show_help (void);
static int dont_daemonize =0;
extern int g_iLogFlag ;
#define MIN(a,b)  ((((a)-(b))<0)?(a):(b))
struct FD_REC{
	int sockfd;
	int data_len;
	char type;
	char flag;
	char data_stat;
	char buf[2048];
};
struct FD_REC gl_fd[FD_SETSIZE-1];
fd_set master_read_set;
pid_t child_pid;
static volatile sig_atomic_t srv_shutdown = 0;
static volatile sig_atomic_t srv_kill_stat = 0;
int child = 0,ListenPort=1188;
static void signal_handler(int sig) {
	dcs_log(0,0,"catch signo =%d",sig);
	switch (sig) {
	case SIGTERM:
	{
			 srv_shutdown = 1; 
			 if( !child ){
			 	 srv_kill_stat=1;
			 	 kill(child_pid,SIGHUP);
			 }
			 break;
	}
	case SIGINT:
	     srv_shutdown = 1;
	     break;
	case SIGHUP:  srv_shutdown = 1;break;
	case SIGCHLD:  break;
	default:
		break;
	}
}
static void daemonize(void) {

	if (0 != fork()) exit(0);

	if (-1 == setsid()) exit(0);

	signal(SIGHUP, SIG_IGN);

	if (0 != fork()) exit(0);

	if (0 != chdir("/")) exit(0);
}
static void add_to_set(int fd, fd_set *set, int *max_fd) {
  FD_SET(fd, set);
  if (fd >  *max_fd) {
    *max_fd = (int) fd;
  }
}
int main (int argc, char **argv) 
{
	
	int sock,ret,iCliAddr,iCliPort,len,n;
	int num_childs = 0;
	int max_fd ,vmax_fd;
	struct timeval tv ;
	char buffer[2047],o;
	fd_set v_set;
	size_t i;
 g_iLogFlag =1;
 max_fd=0;
 vmax_fd=0;
  memset(&gl_fd,0,sizeof sizeof(struct FD_REC)*(FD_SETSIZE-1));
  FD_ZERO(&master_read_set);
	
  if (0 > SrvOpenLog())
  {
			fprintf(stderr,"can not open log file !");
			return -1;
	}
	while(-1 != (o = getopt(argc, argv, "p:hvtD"))) {
		switch(o) {
		case 'p': if( atoi(optarg )>0 ) ListenPort=atoi(optarg);break;
		case 'D':	dont_daemonize = 1; break;
		case 'v': show_version(); return 0;
		case 'h': show_help(); return 0;
		case 't': GetMastKey("001",buffer); return 0;
		default:
			show_help();
			return -1;
		}
	}
	sock=tcp_open_server(NULL, ListenPort);
  if ( sock <0) 
  {
    	 dcs_log(0,0," tcp_open_server() failed:%s! port:%d",strerror(errno),ListenPort);
       return(0);
  }
  add_to_set(sock, &master_read_set, &max_fd);
  gl_fd[sock].sockfd=sock;
  gl_fd[sock].type=0x30;
  gl_fd[sock].flag=0x31;
	if (dont_daemonize) daemonize();
//  setpgid(0,0);
//  setuid(pwd->pw_uid);   
	signal(SIGPIPE, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	signal(SIGTERM, signal_handler);
	signal(SIGHUP,  signal_handler);
	signal(SIGCHLD, signal_handler);
	signal(SIGINT,  signal_handler);

	/* start watcher and workers */
	num_childs = 1;
	
		
		while (!child && !srv_shutdown ) {
			if (num_childs > 0) {
				dcs_log(0,0,"build wokeer!");
				switch ((child_pid=fork())) {
				case -1:
					return -1;
				case 0:
					child = 1;
					break;
				default:
					num_childs--;
					break;
				}
			} 
			else 
			{
				int status;

				if (-1 != wait(&status)) {
					/** 
					 * one of our workers went away 
					 */
					num_childs++;
				} else 
				{
					switch (errno) {
					case EINTR:
						/**
						 * if we receive a SIGHUP we have to close our logs ourself as we don't 
						 * have the mainloop who can help us here
						 */
							if (!srv_shutdown) {
								srv_shutdown = 1;
//								kill(child_pid, SIGHUP);
							}
  						break;
					default:
						break;
					}
				}
			}
		}

		/**
		 * for the parent this is the exit-point 
		 */
		if (!child) {
			/** 
			 * kill all children too 
			 */
			if (srv_shutdown) {
				if( !srv_kill_stat )
				  kill(child_pid, SIGTERM);
			}
//			for( i=0; i<=max_fd;i++)
 //       if( gl_fd[i].flag ==0x31) close(gl_fd[i].sockfd);
      dcs_log(0,0,"master system exit!");
			return 0;
		}
		
	/* libev backend overwrites our SIGCHLD handler and calls waitpid on SIGCHLD; we want our own SIGCHLD handling. */

//	signal(SIGCHLD, signal_handler );

catch_all_signals(signal_handler);
signal(SIGHUP,  signal_handler);
	/* main-loop */
	while (!srv_shutdown) {
		
		  FD_ZERO(&v_set);
    	tv.tv_sec = 1;
      tv.tv_usec = 0;
      v_set=master_read_set;
      
      vmax_fd=0;
      for ( i=max_fd; i>1 ;i--)
      {
      	if( gl_fd[i].flag)
      	{
      			 vmax_fd=i;
      			 break;
      	}
      }
      max_fd=vmax_fd;
    	ret=select(max_fd + 1, &v_set, NULL, NULL, &tv);
      if ( ret < 0) 
    	{

	    	 dcs_log(0,0,"select error! %s",strerror(errno));
      }
      else 
      {
 
        for ( i=0 ; i<= max_fd;i++)
    	  {
    	  	if ( gl_fd[i].flag == 0x31)
    	  	if(FD_ISSET(gl_fd[i].sockfd, &v_set))
    	  	{
    	  		 if( gl_fd[i].type == 0x30)
    	  		 {
    	  		 	int flags;
    	  		 	struct in_addr in;
    	  		 	 	sock = tcp_accept_client(gl_fd[i].sockfd,&iCliAddr, &iCliPort);
								if (sock < 0)
								{
					  			dcs_log(0,0," tcp_accept_client() failed: %s!",strerror(errno));
					  			break;
								}
								if ( sock >(FD_SETSIZE -2)){
								   
								   dcs_log(0,0,"sock overflow limited![%d]",sock);
								   close(sock);
									 break;
								}
								gl_fd[sock].sockfd=sock;
								gl_fd[sock].flag=0x31;
								gl_fd[sock].type=0x31;
								gl_fd[sock].data_stat=0;
								gl_fd[sock].data_len=0;
								flags=fcntl(sock,F_GETFL);
                fcntl(sock,F_SETFL,flags|O_NDELAY);
                in.s_addr = iCliAddr;
                add_to_set(sock, &master_read_set, &max_fd);
								dcs_debug(0,0, "accept socket succ socket=%d",sock,inet_ntoa(in),iCliPort);
    	  		 }
    	  		 else
    	  		 {
    	  		 	  len=read(gl_fd[i].sockfd,buffer,sizeof(buffer));
    	  		 	  if ( len >0 )
    	  		 	  {
     	  		 	  	  
     	  		 	  	  if ( gl_fd[i].data_stat==0)
     	  		 	  	     memcpy(gl_fd[i].buf,buffer,len);
     	  		 	  	  else
     	  		 	  	  	 memcpy(gl_fd[i].buf+gl_fd[i].data_len,buffer,MIN(len,sizeof(buffer)-gl_fd[i].data_len));
     	  		 	  	  gl_fd[i].data_len +=len;
     	  		 	  	  gl_fd[i].data_stat=1;
     	  		 	  	  dcs_debug(gl_fd[i].buf,gl_fd[i].data_len,"recv data len=%d,gl_fd[i].data_len=%d",len,gl_fd[i].data_len);
     	  		 	  	  if (gl_fd[i].data_len < 2 )
     	  		 	  	  {
     	  		 	  	  	   gl_fd[i].data_stat=1;
     	  		 	  	  	   break;
     	  		 	  	  }
     	  		 	  	  if( gl_fd[i].data_len >=2047 ||
     	  		 	  	  	  ((unsigned char)gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1])>=2047)
     	  		 	  	  {
     	  		 	  	  	
     	  		 	  	  	dcs_log(gl_fd[i].buf,2,"error data_len[%d]!",gl_fd[i].data_len);
     	  		 	  	  	gl_fd[i].data_len=0;
     	  		 	  	  	gl_fd[i].data_stat=0;
     	  		 	  	  	break;
     	  		 	  	  }
     	  		 	  	  
     	  		 	  	  
     	  		 	  	  if ( gl_fd[i].data_len >= ((unsigned char)gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1]))
     	  		 	  	  {
     	  		 	  	  	gl_fd[i].data_stat=2;
     	  		 	  	  }	
     	  		 	  	  else break;
     	  		 	  	  len=gl_fd[i].data_len;
     	  		 	  	  gl_fd[i].data_len=0;
     	  		 	  	  dcs_debug(gl_fd[i].buf,len,"recv data from socket len=%d ",gl_fd[i].buf[0]*256+(unsigned char)gl_fd[i].buf[1]);
								  	if (0>appProc(gl_fd[i].buf+2,gl_fd[i].data_len-2,buffer,&n))
								  		  break;
								  	i=write(gl_fd[i].sockfd, buffer,n);
								  	if ( i <=0)
								  	{
								  	 	   close(gl_fd[i].sockfd);
								  	 	   gl_fd[i].flag=0;
								  	 	   FD_CLR(gl_fd[i].sockfd,&master_read_set);
								  	 		 break;
								  	}
								  	dcs_debug(buffer,i,"send data to app len=%d",i);
								}
								else
								{
									 close(gl_fd[i].sockfd);
								   gl_fd[sock].flag=0;
								   FD_CLR(gl_fd[i].sockfd,&master_read_set);
								   dcs_log(0,0,"<%s> socket error! [%d][%s]",__FUNCTION__,errno,strerror(errno));
								}
	
             }
          }
       }
		
	   }
  }
  for( i=0; i<=max_fd;i++)
     if( gl_fd[i].flag ==0x31) close(gl_fd[i].sockfd);
	dcs_log(0,0,"worker system exit!");	
	return 0;
}

static void show_version (void) {

	fprintf(stderr,"version 1.0 -Build-Date: %s %s\n",__DATE__,__TIME__);
}
static void show_help (void) {

	fprintf(stderr,"-p []   Listen Port\n");
	fprintf(stderr,"-D   daemonize\n");
	fprintf(stderr,"-v   display version \n");
	fprintf(stderr,"-h   display help \n");
}

int SrvOpenLog()
{
  	char   caLogName[256];
    char logfile[400];
  
    memset(caLogName,0,sizeof(caLogName));
    strcpy(caLogName,"log/softjmj.log");
    if(u_fabricatefile(caLogName,logfile,sizeof(logfile)) < 0)
    {
            return -1;
    }
    dcs_log_open(logfile, "Encrypt");

 	return 0;
}

int appProc(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char order[3];
	memset(order,0,sizeof(order));
	
	if ( memcmp(inBuf,"94",2)==0) /*解密终端上送的磁道信息*/
		 return DecodeTrack(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"1A",2)==0) /*生成随机密钥*/
		 return DesGetKey(inBuf+2,nIn-2,outBuf,nOut);
		 
	//huang jia add 20100326 begin
	else if ( memcmp(inBuf,"K0",2)==0) /*生成终端主密钥TMK（K0/K1）*/
		 return DesGetTmk(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"K2",2)==0) /*生成数据密钥PIK/MAK（K2/K3）*/
		 return DesGetPikMak(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"M0",2)==0) /*MAC计算（M0/M1）*/
		 return DesGetMac(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"M2",2)==0) /*验证MAC（M2/M3）*/
		 return DesCheckMac(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"P0",2)==0) /*PIN BLOCK转换（P0/P1）*/
		 return DesTransPin(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"KE",2)==0) /*SEK和TEK之间转换TMK（KE/KF）*/
		 return DesTransTmk(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"KI",2)==0) /*SEK和TMK之间转换PIK/MAK（KI/KJ）*/
		 return DesTransPikMak(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"98",2)==0) /*解密终端上送的磁道信息*/
		 return DecTrack(inBuf+2,nIn-2,outBuf,nOut);		 
    else if ( memcmp(inBuf,"E1",2)==0) /*解密终端上送的磁道信息*/
		 return DecTrack2(inBuf+2,nIn-2,outBuf,nOut);
	//huang jia add 20100326 end
		 
	else if ( memcmp(inBuf,"95",2)==0) /*计算终端报文MAC*/
		 return DesMacOp(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"96",2)==0) /*转换终端上送的密钥密文*/
		 return DesPinTr(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"80",2)==0) /*计算银行间交易报文的MAC*/
		 return BNKDesMacOp(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"69",2)==0) /*银行间的密钥转换*/
		 return BNKDesPinTr(inBuf+2,nIn-2,outBuf,nOut);
	else if ( memcmp(inBuf,"01",2)==0)
		 return DesTest(inBuf+2,nIn-2,outBuf,nOut);
  else
  {
  	memcpy(order,inBuf,2);
  	dcs_log(0,0,"加密指令错误,order=%s",order);
  	return -1;
  }
}
