#ifndef __P_SW__
#define  __P_SW__
typedef struct {
	char ip[16]; 
	int  port;   
	int  new_loc_port;   
	int  old_loc_port;
} SYS_ARA;

typedef struct {
	int sockfd;
	int type;
	int used;
	int status;
	int data_len;
	int msg_len;
	time_t t;
	char buf[8192];
}SOCK_REC;

typedef struct {
	
	int cnt;
	char keys[1024][30];
	unsigned char  socks[1024];
	time_t  t[1024]; 
	
}key_rec;
void add_to_set(int fd, fd_set *set, int *max_fd) ;
#endif