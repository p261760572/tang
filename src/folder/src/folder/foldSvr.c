#include <folder.h>

//
// the implementation of Folder Server(foldSvr).
// The foldSvr is a daemon managing all folder resources,such
// as creation,locating,deleting a folder on the demand from
// the requestor.
//

static int gs_nheader = 0; //header# in buffer pool
static int gs_nbuffer = 0; //buffer# in buffer pool
static int gs_nbufsize= 0; //size of each buffer
static int gs_nfolder = 0; //folder# in folder array

// stuff with respect to logging
static int gs_logfd = -1;
#define    LOG_FILE_NAME  "log/foldsvr.log"
#define    CONF_FILE_NAME "config/folder.conf"

static void foldSvrExit(int signo);
static char *make_temp_name(int pid);
static char *IDENT;

#define  SEMMSL    512

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    IDENT = argv[0];

    //bring ourself to background and install the 
    //omnipotent signal handler
     u_daemonize(NULL);
    catch_all_signals(foldSvrExit);
    catch_signal(SIGCHLD,SIG_IGN);
    
    //open the logging file
    open_log_file(); 
    
    //determine if a previous 'foldSvr' instance already running
    if(NULL != shm_connect(FOLDER_LIST_SHM_NAME,NULL))
    {
        log_jotdown(gs_logfd,IDENT,"'foldSvr' already running.\n");
        exit(1);
    }
    
    //load configuration about buffer pool
    if(0 > load_buffer_config())
    {
        log_jotdown(gs_logfd,IDENT,"cannot load configuration.\n");
        exit(1);    
    }
    
    //try creating buffer pool
    if(0 > create_buffer_pool())
    {
        log_jotdown(gs_logfd,IDENT,
                    "cannot create shared memory as buffer pool:%s\n",
                            strerror(errno));
        exit(1);    
    }
    
    //try creating folder list
    if(0 > create_folder_list())
    {
        log_jotdown(gs_logfd,IDENT,
                    "cannot create shared memory as folder list:%s\n",
                            strerror(errno));
        exit(1);
    }
    
    create_predefined_folders();

    //loop and service
    log_jotdown(gs_logfd,IDENT,"!!********startup completed.*********!!\n");    
    do_service();
    
    //terminate
    foldSvrExit(SIGTERM);
}//main

/////////////////////////////////////////////////////////////////////////////

static void foldSvrExit(int signo)
{
    if(signo != SIGTERM)  return;
    
    //notify all processes to exit
    if(gl_pFldCtrl)
    {
        int i;
        for(i=0; i<gl_pFldCtrl->fold_nalloc;i++)
        {
            if(!(gl_pFldArray[i].fold_flags & FOLD_FLAG_USED))
                continue;
            kill(gl_pFldArray[i].fold_pid,SIGTERM);
        }
    }

    //cleanup all resources before exiting
    cleanup_call_resources();
    if(gs_logfd >= 0)
        log_close(gs_logfd);        
    exit(signo);
}

/////////////////////////////////////////////////////////////////////////////

int cleanup_call_resources()
{
    //remove the buffer pool
    mbuf_delete(&gl_shmobj);
        
    //cleanup the folder list
    if(gl_pFldCtrl)
    {
        int shmid;

        shmid = gl_pFldCtrl->shm_id;
        if(gl_pFldCtrl->msg_reqqid >= 0)
            queue_delete(gl_pFldCtrl->msg_reqqid);
        if(gl_pFldCtrl->msg_viaqid >= 0)
            queue_delete(gl_pFldCtrl->msg_viaqid);
        if(gl_pFldCtrl->sem_id >= 0)
            sem_delete(gl_pFldCtrl->sem_id);
        shm_detach((char *)gl_pFldCtrl);
        shm_delete(shmid);
        
        gl_pFldCtrl = NULL;
    }
    
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

int open_log_file()
{
    char logfile[256];
    
    //assuming the log file is $ISEHOME/log/foldsvr.log
    if(0 > u_fabricatefile(LOG_FILE_NAME,logfile,sizeof(logfile)))
        return -1;

    gs_logfd = log_open(logfile);    
    return (gs_logfd >= 0) ? 0 : (-1);
}

/////////////////////////////////////////////////////////////////////////////

int load_buffer_config()
{
    char conf_file[256];
    int  fd;
    
    //assuming the configure file is $ISEHOME/etc/fold/folder.conf
    if(0 > u_fabricatefile(CONF_FILE_NAME,conf_file,sizeof(conf_file)))
        return -1;
        
    if((fd = conf_open(conf_file)) < 0)
        return -1;
    
    //read all items    
    if( 0 >conf_get_first_number(fd, "nMHdrs",   &gs_nheader))
        goto failure;
    if( 0 >conf_get_first_number(fd, "nMBufs",   &gs_nbuffer))
        goto failure;    
    if( 0 >conf_get_first_number(fd, "szMbuf",   &gs_nbufsize))
        goto failure;    
    if( 0 >conf_get_first_number(fd, "nFolders", &gs_nfolder))
        goto failure;    
    conf_close(fd);
    return 0;
    
failure:
    conf_close(fd);
    return -1;    
}

/////////////////////////////////////////////////////////////////////////////

int create_buffer_pool()
{
    memset(&gl_shmobj,0,sizeof(gl_shmobj));
    return mbuf_create(&gl_shmobj,BUFFER_POOL_SHM_NAME,
                       gs_nheader,gs_nbuffer,gs_nbufsize);
}

/////////////////////////////////////////////////////////////////////////////

int create_folder_list()
{
    long space; 
    int  shmid, i;
    char *ptr = NULL;
    char strName[32];
    int  base_address;

    //calculate the size of shared memory and adjust it
    //to multiple 4 bytes
    space = sizeof(FOLDCTRLBLK) +
            gs_nfolder * sizeof(FOLDERENTRY);
    space = ((space-sizeof(long))/sizeof(long) +1 )* sizeof(long);
	log_jotdown(gs_logfd,IDENT,"SHM size =%d.\n",space);
    //try creating shared memory as folder list
    ptr = (char *)shm_create(FOLDER_LIST_SHM_NAME,space,&shmid);
    if(ptr==NULL)
    {
	    log_jotdown(gs_logfd,IDENT,"create shm name %s fail size=%d.\n",FOLDER_LIST_SHM_NAME,space);
	    goto failure;
    }

    //initialize this memory block and get pointers
    memset(ptr, 0, space);
    gl_pFldCtrl  = (FOLDCTRLBLK *)ptr;
    gl_pFldArray = (FOLDERENTRY *)(ptr + sizeof(FOLDCTRLBLK));

    //try creating the messasge queue
    gl_pFldCtrl->msg_reqqid = queue_create(FOLDER_LIST_SHM_NAME);
    if(gl_pFldCtrl->msg_reqqid < 0)
    {
	    log_jotdown(gs_logfd,IDENT,"create queue name %s fail .\n",FOLDER_LIST_SHM_NAME);
	    goto failure;
    }	
    
    memset(strName,0,sizeof(strName));
    sprintf(strName, "%s",FOLDER_LIST_SHM_NAME);
    (*strName) ++;
    gl_pFldCtrl->msg_viaqid = queue_create(strName);
    if(gl_pFldCtrl->msg_viaqid < 0)
    {
	    log_jotdown(gs_logfd,IDENT,"create queue name %s fail .\n",FOLDER_LIST_SHM_NAME);
	    goto failure;
    }
        
    //try creating the semaphore
    if(1)
    {
        int nsem = min(SEMMSL-2, gs_nfolder) + 1;
        gl_pFldCtrl->sem_id = sem_create(FOLDER_LIST_SHM_NAME,nsem);
        if(gl_pFldCtrl->sem_id < 0)
    	{
	    log_jotdown(gs_logfd,IDENT,"create sem name %s fail num=%d.\n",FOLDER_LIST_SHM_NAME,nsem);
	    goto failure;
    	}
        gl_pFldCtrl->sem_count = nsem;
    }
        
    //initialize the control block
    gl_pFldCtrl->shm_id             = shmid;    
    gl_pFldCtrl->foldsvr_pid        = getpid();
    gl_pFldCtrl->shm_magic          = FOLD_MAGIC;
    gl_pFldCtrl->fold_nalloc        = gs_nfolder;
    gl_pFldCtrl->fold_nfree         = gs_nfolder;
    gl_pFldCtrl->fold_systickcount  = 1;

    //initialize the folder array
    base_address = getpid() + 1;
    for(i=0; i<gs_nfolder; i++)
        gl_pFldArray[i].fold_address = base_address ++;

    //creation ok,return to caller
    return 0;

failure:
    cleanup_call_resources();
    return -1;
}

/////////////////////////////////////////////////////////////////////////////

int create_predefined_folders()
{
    char conf_file[256],folder_name[MAX_FOLDNAME_SIZE+1];
    int  fd, ret;

    ret = fork();
    if(ret < -1)
        return -1;
    
    if(ret > 0)  //parent return
        return 0;
    
    //i am child, i will read each folder configuration from
    //file and ask my parent to create it for me. first i wait
    //a while so that my parent's startup has been completed.
    sleep(1);
        
    //assuming the configure file is $ISEHOME/etc/fold/folder.conf
    if(0 > u_fabricatefile(CONF_FILE_NAME,conf_file,sizeof(conf_file)))
        exit(1);
        
    if((fd = conf_open(conf_file)) < 0)
        exit(1);

    //read all items    
    ret = conf_get_first_string(fd, "folder.entry",folder_name);
    while(ret == 0)
    {
        fold_create_folder(folder_name);
        ret = conf_get_next_string(fd, "folder.entry",folder_name);
    }    
        
    conf_close(fd);
    exit(0);        
}

/////////////////////////////////////////////////////////////////////////////

int do_service()
{
    //main logic of foldSvr
    struct qrypacket packet, *pQry;
    int    qid,ret;

    qid = gl_pFldCtrl->msg_reqqid;
    pQry    = &packet;

    for(;;)
    {
        //blocking on waiting for request
        pQry->fold_msgtype =  gl_pFldCtrl->foldsvr_pid;
        ret = queue_recv(qid, pQry, 
                         sizeof(struct qrypacket)-sizeof(long),FALSE);
        if(ret < 0)  //fatal error
            return -1;

        //process the request
        switch(pQry->fold_command)
        {
            case QRYPKT_CMD_CREATE:
                 create_folder_impl(pQry);
                 break;

            case QRYPKT_CMD_LOCATE:
                 locate_folder_impl(pQry);
                 break;

            case QRYPKT_CMD_DELETE:
                 delete_folder_impl(pQry);
                 break;
            
            case QRYPKT_CMD_EXIT:
                 return 0;
                
            default:  //just ignore
                 break;
        }//switch
    }//for
}

/////////////////////////////////////////////////////////////////////////////

int create_folder_impl(struct qrypacket *pQry)
{
    char *name_ptr;
    int   i;
    
    //first check if there exists a folder with the same name
    if(pQry->fold_flags & QRYPKT_FLG_NORMAL)
        for(i=0; i<gl_pFldCtrl->fold_nalloc;i++)
        {
            if(!(gl_pFldArray[i].fold_flags & FOLD_FLAG_USED))
                continue;
            if(u_stricmp(pQry->fold_name, gl_pFldArray[i].fold_name) != 0)
                continue;
            
            //duplicate folder found    
            pQry->fold_result = -1;
            pQry->fold_errno  = FOLD_EEXIST;
            goto result;
        }
   
    //second check there is free entry to create a new folder
    if(gl_pFldCtrl->fold_nfree <= 0)     
    {
        //no free entry
        pQry->fold_result = -1;
        pQry->fold_errno  = FOLD_ENOFOLD;
        goto result;
    }
   
    //get the free position
    for(i=0; i<gl_pFldCtrl->fold_nalloc;i++)
    {
        if(gl_pFldArray[i].fold_flags & FOLD_FLAG_USED)
            continue;
        break;    
    }

    //initialize this newly allocate folder
    gl_pFldCtrl->fold_nfree --;
    gl_pFldArray[i].fold_flags |= FOLD_FLAG_USED;
    gl_pFldArray[i].fold_pid    = pQry->fold_pid;
    gl_pFldArray[i].fold_nref   = 1;
    gl_pFldArray[i].fold_type   = (pQry->fold_flags & QRYPKT_FLG_NORMAL) ? 
                                      FOLD_TYPE_NORMAL : FOLD_TYPE_ANONYMOUS;   
    if(gl_pFldArray[i].fold_type == FOLD_TYPE_NORMAL)
       name_ptr = pQry->fold_name;
    else    
       name_ptr = make_temp_name(pQry->fold_pid); 
    memcpy(gl_pFldArray[i].fold_name,name_ptr,
                  min(MAX_FOLDNAME_SIZE,strlen(name_ptr)));

    pQry->fold_result = i;
    pQry->fold_errno  = 0;
   
result:        
   send_answer_packet(pQry);
   return 0;
}

/////////////////////////////////////////////////////////////////////////////

int locate_folder_impl(struct qrypacket *pQry)
{
    int i;
    
    for(i=0; i<gl_pFldCtrl->fold_nalloc;i++)
    {
        if(!(gl_pFldArray[i].fold_flags & FOLD_FLAG_USED))
            continue;
        if(u_stricmp(pQry->fold_name, gl_pFldArray[i].fold_name) != 0)
            continue;
        
        //we find it            
        pQry->fold_result = i;
        pQry->fold_errno  = 0;
        gl_pFldArray[i].fold_nref ++;
        goto result;
    }

    //we not find it            
    pQry->fold_result = -1;
    pQry->fold_errno  = FOLD_ENOENT;
    
result:    
    send_answer_packet(pQry);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

int delete_folder_impl(struct qrypacket *pQry)
{
    int fid;
    
    fid = atoi(pQry->fold_name);
    if(fold_check_Id(fid) < 0)
    {
        pQry->fold_result = -1;
        pQry->fold_errno  = FOLD_ENOENT;
        goto result;
    }
    
    gl_pFldArray[fid].fold_nref --;
    if(gl_pFldArray[fid].fold_nref <= 0 || 
           pQry->fold_flags & QRYPKT_FLG_DELBYFORCE)
    {
        int save_addr;
        
        //physically remove this folder    
        save_addr = gl_pFldArray[fid].fold_address;
        memset(&gl_pFldArray[fid],0,sizeof(FOLDERENTRY));
        gl_pFldArray[fid].fold_address = save_addr;
        gl_pFldCtrl->fold_nfree ++;
    }

    pQry->fold_result = 0;
    pQry->fold_errno  = 0;
    
result:    
    send_answer_packet(pQry);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

int send_answer_packet(struct qrypacket *pQry)
{
    pQry->fold_msgtype =  pQry->fold_pid;
    return queue_send(gl_pFldCtrl->msg_reqqid,pQry,
                      sizeof(struct qrypacket)-sizeof(long),FALSE);    
}

/////////////////////////////////////////////////////////////////////////////

static char _tmpNameBuffer[MAX_FOLDNAME_SIZE+4];
static int  _tmpSeed = 1;

char *make_temp_name(int pid)
{
    memset(_tmpNameBuffer,0,sizeof(_tmpNameBuffer));
    sprintf(_tmpNameBuffer,"~tmp%.6d.%.3d",pid,_tmpSeed);
    _tmpSeed ++;

    return _tmpNameBuffer;
}
