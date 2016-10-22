#include  <folder.h>

static int  gs_pid = -1;  //id of current process 
static int  gs_tid = -1;  //id of current thread 
static int  gs_qid = -1;  //id of message queue

//---------------------------------------------------------------------

int  log_open(const char * logfile)
{
	int fd = -1;

	fd = open(logfile,O_WRONLY|O_APPEND|O_CREAT,0666);
	return fd;
}

//---------------------------------------------------------------------

void  log_close(int fd)
{
	close(fd);
}

//---------------------------------------------------------------------

void  log_jotdown(int fd,const char * ident,const char * message, ...)
{
    char    buffer[4096], tmp_ident[11], *ptr;
    struct  tm *tm;
    time_t  t;
    va_list ap;
    int     i;
    static  int gs_pid = -1;
    
    if(fd < 0)
        return;
    
    if(gs_pid != getpid())
    	gs_pid = getpid();
      
    for(i=0; ident && i<sizeof(tmp_ident)-1;i++)
    {
        if(ident[i]==0)  break;
        tmp_ident[i] = ident[i];
    }
    for(;i<sizeof(tmp_ident)-1;i++)
        tmp_ident[i] = ' ';
    tmp_ident[i] = 0;

    //format the message
    time(&t);
    tm = localtime(&t);
    
    memset(buffer,0,sizeof(buffer));
    ptr = buffer;    
    sprintf(ptr,"%4d/%02d/%02d %02d:%02d:%02d %s(%.6d) ",
            tm->tm_year+1900,
            tm->tm_mon + 1, 
            tm->tm_mday, 
            tm->tm_hour, 
            tm->tm_min, 
            tm->tm_sec,
            tmp_ident,
            gs_pid);
    ptr += strlen(ptr);
            
    //print the true logging message
    va_start(ap, message);
    vsprintf(ptr, message, ap);
    va_end(ap);
    
    //write the logging message into file
    write(fd, buffer, strlen(buffer));
    return;
}

