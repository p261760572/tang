#ifndef __NAC_SW__
#define  __NAC_SW__
typedef struct {
	char new_ip[16]; 
	char old_ip[16]; 
	int  new_port;   
	int  old_port;   
	int  loc_port;
} SYS_ARA;

typedef struct {
	int sockfd;
	int type;
	int used;
	int status;
	int data_len;
	int msg_len;
	char buf[8192];
}SOCK_REC;
void add_to_set(int fd, fd_set *set, int *max_fd) ;
#endif