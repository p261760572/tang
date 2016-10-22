#ifndef __EFFI_SECU_SRV_H__
#define __EFFI_SECU_SRV_H__
#define MAX_BUF_LEN 2048
struct sec_link_info
{
    short  iRole;     //该link在连接建立过程中的角色 0 加密工作线路 1 监控侦听线路 2监控工作线路 
    short  iStatus;   //该link当前 的状态 0 链路停止状态 1链路在建状态 2 链路就绪状态 3数据发送状态 4数据接收状态
    short  iWorkFlag; //0 空闲状态 1工作状态
    short  ip_num;    // 对应加密机ip所在的缓冲区指针
    short  expected_len; //期望数据的长度
    short  reality_len; //实际数据的长度
    int    iSocketId;
    short   iTimeOut;    //超时时间
    time_t  build_time;  // 端口创建时间
    time_t  work_begin_time; // 工作开始时间
    long    server_count; //链路服务次数
    unsigned char  data_key[20]; // 数据的关键字
    unsigned char  data_buf[MAX_BUF_LEN]; // 接收到的数据
};

struct sec_machine_info
{
	  char   remoteIp[20];//期望的远端的IP地址
    int    remotePort;   //期望的远端的端口号
    short  link_num;    //需要建立的链路数
    short  max_link_num; //最大能建立的链路数，为0则不进行判断
    short  undone_link_num; // 未完成建立的链路数
    short  timeout;
    unsigned char encType;//加密机类型   为0时为服务监控端口，remotePort为监控端口
};

struct MSG_STRU
{
	
	  long iPid;
	  time_t send_time;
	  unsigned char type;
	  char msg[1];
};

struct EFFI_QUEUE
{
	 int max_size; // 缓冲区大小
	 int free_size; // 剩余空间
	 short message_num;  //缓冲区存在的消息数
   long buf_head;  //数据所在缓冲区的头
   long buf_tail;	 //数据所在缓冲区的位
   char *buf;
}; 
struct EFFI_SEC_SRV
{
	char  queue_rcv_name[30];
	char  queue_snd_name[30];
	short work_links; //工作连接数
	short max_link;  //系统支持的最大链路数
	short used_link;  //已经使用到的链路数
	short curr_link; //当前链路所在指针，系统启动时在0位置，最大在use_link位置
	short max_machine;//加密机ip地址数量（或加密机台数）
	short used_machine;//已使用的台数
	int   build_count;//建链次数
	long  send_qid;
	long  recv_qid;
	long  count;     // 系统从启动开始累计为加密服务工作过的次数
	long  accept_cnt; // 系统受理次数
	long  refuse_cnt; // 拒绝次数
	long  abnormal_cnt; //异常次数
	long  discard_cnt; //丢弃次数
	pthread_mutex_t mutex;
	struct EFFI_QUEUE  data_queue;
	struct sec_machine_info *machine_info; //加密机通信ip地址等信息
	struct sec_link_info *link; // 每条链路信息存储的缓冲区
};
#endif
