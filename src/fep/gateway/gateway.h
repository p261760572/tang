struct FD_REC{
	int sockfd;
	time_t  timeout;
	time_t  last_time;
	time_t  begin_time;
	int ndx;
	char ip[17]; /*������·IP */
	int  port;   /*������·�˿ں�*/
	char type; /* 0�ͻ���������·��1 ����˹�����· 2�ͻ��˹�����·*/
	char status; /* 0 ׼��״̬ 1����״̬ 2��Կ����״̬ 3���ݴ���״̬ 4���ݽ������״̬*/
	char used;
	char len_type; /* 0 ���ֽ�asc�볤�ȣ�1���ֽ�hex���ȣ�2 xmlЭ��1 3xmlЭ��2 4httpЭ�鳤�ȣ� 5httpsЭ�鳤��*/
	int len_len;
	int data_len;
	int msg_len;
//	int remote_sockfd;
  char key[17];
	char remark[40];
	char buf[8192];
};
struct PARA {
	char svr_ip[32];  //Ӧ�÷����IP
	char cfgfile[256];
	int  svr_port;    //Ӧ�÷���˶˿ں�
	int  loc_port;    //���ؿͻ��������˿ں�
	int  timeout;
	char used;       //ʹ�ñ�־
	int  type;      //   0 ���ֽ�asc�볤�ȣ�1���ֽ�hex���ȣ�2 xmlЭ��1 3xmlЭ��2 4httpЭ�鳤�ȣ� 5httpsЭ�鳤��
};

void add_to_set(int fd, fd_set *set, int *max_fd) ;