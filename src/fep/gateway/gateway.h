struct FD_REC{
	int sockfd;
	time_t  timeout;
	time_t  last_time;
	time_t  begin_time;
	int ndx;
	char ip[17]; /*工作链路IP */
	int  port;   /*工作链路端口号*/
	char type; /* 0客户端侦听链路，1 服务端工作链路 2客户端工作链路*/
	char status; /* 0 准备状态 1就绪状态 2秘钥交换状态 3数据传输状态 4数据接收完毕状态*/
	char used;
	char len_type; /* 0 四字节asc码长度，1两字节hex长度，2 xml协议1 3xml协议2 4http协议长度， 5https协议长度*/
	int len_len;
	int data_len;
	int msg_len;
//	int remote_sockfd;
  char key[17];
	char remark[40];
	char buf[8192];
};
struct PARA {
	char svr_ip[32];  //应用服务端IP
	char cfgfile[256];
	int  svr_port;    //应用服务端端口号
	int  loc_port;    //本地客户端侦听端口号
	int  timeout;
	char used;       //使用标志
	int  type;      //   0 四字节asc码长度，1两字节hex长度，2 xml协议1 3xml协议2 4http协议长度， 5https协议长度
};

void add_to_set(int fd, fd_set *set, int *max_fd) ;