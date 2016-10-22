#include <ipc_func.h>

// message queue functions,all of which are 
// prefixed by 'queue_':
// (1) int queue_create(const char *name);
// (2) int queue_connect(const char *name);
// (3) int queue_delete(int qid);
// (4) int queue_send(int qid, char *msg, int size, int nowait);
// (5) int queue_recv(int qid, char *buf, int size, int nowait);
// internal helper:
/* (1)*/ 
int queue_getqueue(const char *name, int createflg);
int _queue_getqueue(const char *name, int createflg);
//
//
//import from other module:
/*(1)*/ extern int ipc_makekey(const char *name);

static int  g_readq_interruptable = 0;

/*---------------------------------------------------------------------------*/
int _queue_create(const char *name)
{
    int qid;

    //call queue_getqueue() to create the block 
    qid = _queue_getqueue(name, 1);
    return(qid);
}

/*---------------------------------------------------------------------------*/

int _queue_connect(const char *name)
{
    return(_queue_getqueue(name, 0));
}

int queue_create(const char *name)
{
    int qid;

    //call queue_getqueue() to create the block 
    qid = queue_getqueue(name, 1);
    return(qid);
}

/*---------------------------------------------------------------------------*/

int queue_connect(const char *name)
{
    return(queue_getqueue(name, 0));
}

/*---------------------------------------------------------------------------*/

int queue_delete(int qid)
{
    return(msgctl(qid, IPC_RMID, NULL));
}

/*---------------------------------------------------------------------------*/

int queue_send(int qid, void *buf, int size,int nowait)
{
    // the layout of 'buf' should be as following:
    // (buf->msgtype          , buf->mtext[])
    // (4_bytes_length_msgtype, 'size'_bytes_application_data),where 
    // 'size' is the inputed argument
     
    int	flag;
    int	ret;

    if(nowait)
        flag = IPC_NOWAIT;
    else
        flag = 0;

    for(;;)
    {
        if((ret = msgsnd(qid, buf,size, flag)) == -1)
        {
	        if(errno == EINTR)
	            continue;
	        else
	            break;
	    }       
        else
	        break;
    }//for

    return(ret);
}

/*---------------------------------------------------------------------------*/

int queue_recv(int qid, void *buf, int size, int nowait)
{
    // the layout of 'buf' should be as following:
    // (buf->msgtype          , buf->mtext[])
    // (4_bytes_length_msgtype, 'size'_bytes_application_buffer),where 
    // 'size' is the inputed argument.
    // when function returns successfully, all data received will be
    // placed into buf->mtext[], and the true message type will be
    // placed into buf->mtype.
    
    int	  flags, ret;
    long  type;
    
    if(nowait)
        flags = IPC_NOWAIT;
    else
        flags = 0;
    
    for(;;)
    {
        type = *(int *)buf;
        ret = msgrcv(qid, buf, size, type, flags);
        if(ret < 0)
        {
            if(errno == EINTR)
            {
                if(g_readq_interruptable)
                    return -1;
                else
                    continue;
            }
            else  //fatal error
                return -1;
        }
        else
            return ret;
    }    
}

/*---------------------------------------------------------------------------*/

int queue_getqueue(const char *name, int createflg)
{
    int	    qid;
    long	flags;
    key_t  key;

    if(createflg)
        flags = IPC_CREAT | IPC_EXCL;
    else
        flags = 0;
	
    flags |= 0666;
		
    //create the new entry
    key = ipc_makekey(name);
    qid = msgget(key, flags);
    return(qid);
}

int _queue_getqueue(const char *name, int createflg)
{
    int	    qid;
    long	flags;
    key_t  key;

    if(createflg)
        flags = IPC_CREAT | IPC_EXCL;
    else
        flags = 0;
	
    flags |= 0666;
		
    //create the new entry
    key = atoi(name);
    qid = msgget(key, flags);
    return(qid);
}

/*---------------------------------------------------------------------------*/

int queue_get_info(int qid, int *nmsgcount, int *nmsgbytes)
{
    struct msqid_ds  info;

    if(0 > msgctl(qid, IPC_STAT, &info))
        return -1;

    if(nmsgcount)
        *nmsgcount = info.msg_cbytes;
    if(nmsgbytes)
        *nmsgbytes = info.msg_qnum;

    return 0;
}

/*---------------------------------------------------------------------------*/

int queue_readq_interruptable(int flag)
{
    if(flag)
        g_readq_interruptable = 1;
    else
        g_readq_interruptable = 0;
}
