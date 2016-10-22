#ifndef __EFFI_SECU_SRV_H__
#define __EFFI_SECU_SRV_H__
#define MAX_BUF_LEN 2048
struct sec_link_info
{
    short  iRole;     //��link�����ӽ��������еĽ�ɫ 0 ���ܹ�����· 1 ���������· 2��ع�����· 
    short  iStatus;   //��link��ǰ ��״̬ 0 ��·ֹͣ״̬ 1��·�ڽ�״̬ 2 ��·����״̬ 3���ݷ���״̬ 4���ݽ���״̬
    short  iWorkFlag; //0 ����״̬ 1����״̬
    short  ip_num;    // ��Ӧ���ܻ�ip���ڵĻ�����ָ��
    short  expected_len; //�������ݵĳ���
    short  reality_len; //ʵ�����ݵĳ���
    int    iSocketId;
    short   iTimeOut;    //��ʱʱ��
    time_t  build_time;  // �˿ڴ���ʱ��
    time_t  work_begin_time; // ������ʼʱ��
    long    server_count; //��·�������
    unsigned char  data_key[20]; // ���ݵĹؼ���
    unsigned char  data_buf[MAX_BUF_LEN]; // ���յ�������
};

struct sec_machine_info
{
	  char   remoteIp[20];//������Զ�˵�IP��ַ
    int    remotePort;   //������Զ�˵Ķ˿ں�
    short  link_num;    //��Ҫ��������·��
    short  max_link_num; //����ܽ�������·����Ϊ0�򲻽����ж�
    short  undone_link_num; // δ��ɽ�������·��
    short  timeout;
    unsigned char encType;//���ܻ�����   Ϊ0ʱΪ�����ض˿ڣ�remotePortΪ��ض˿�
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
	 int max_size; // ��������С
	 int free_size; // ʣ��ռ�
	 short message_num;  //���������ڵ���Ϣ��
   long buf_head;  //�������ڻ�������ͷ
   long buf_tail;	 //�������ڻ�������λ
   char *buf;
}; 
struct EFFI_SEC_SRV
{
	char  queue_rcv_name[30];
	char  queue_snd_name[30];
	short work_links; //����������
	short max_link;  //ϵͳ֧�ֵ������·��
	short used_link;  //�Ѿ�ʹ�õ�����·��
	short curr_link; //��ǰ��·����ָ�룬ϵͳ����ʱ��0λ�ã������use_linkλ��
	short max_machine;//���ܻ�ip��ַ����������ܻ�̨����
	short used_machine;//��ʹ�õ�̨��
	int   build_count;//��������
	long  send_qid;
	long  recv_qid;
	long  count;     // ϵͳ��������ʼ�ۼ�Ϊ���ܷ��������Ĵ���
	long  accept_cnt; // ϵͳ�������
	long  refuse_cnt; // �ܾ�����
	long  abnormal_cnt; //�쳣����
	long  discard_cnt; //��������
	pthread_mutex_t mutex;
	struct EFFI_QUEUE  data_queue;
	struct sec_machine_info *machine_info; //���ܻ�ͨ��ip��ַ����Ϣ
	struct sec_link_info *link; // ÿ����·��Ϣ�洢�Ļ�����
};
#endif
