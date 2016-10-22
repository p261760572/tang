#include <folder.h>

//forward declaration
int do_mstat  (int arg_c, char *arg_v[]);
int do_minfo  (int arg_c, char *arg_v[]);
int do_mlink  (int arg_c, char *arg_v[]);
int do_list   (int arg_c, char *arg_v[]);
int do_ctrlblk(int arg_c, char *arg_v[]);
int do_probe  (int arg_c, char *arg_v[]);
int do_create (int arg_c, char *arg_v[]);
int do_delete (int arg_c, char *arg_v[]);
int do_purge (int arg_c, char *arg_v[]);
int do_stop_sys (int arg_c, char *arg_v[]);

void foldUtilExit(int signo);

////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    //attach to server
    if(0 > fold_initsys())
    {
        printf("cannot attach to folder system!\n");
        exit(1);    
    }

    cmd_init("foldcmd>>", foldUtilExit);
    cmd_add("mstat", do_mstat, "print the statistics of buffer allocation");
    cmd_add("minfo", do_minfo, "print the control block of shared memory");
    cmd_add("mlink", do_mlink, "print free header link and buffer\n"
                               " link in shared memory");
    cmd_add("list",  do_list,  "list all folders");

    cmd_add("ctrlblk",do_ctrlblk,"view the control block of folders");
    cmd_add("create", do_create, "create name:create folder by given name");
    cmd_add("delete", do_delete, "delete fid:delete folder with the given ID");
    cmd_add("purge", do_purge,   "remove obsolete message from folder");

    cmd_add("stop", do_stop_sys, "stop : terminate the folder server");

    cmd_add("probe",do_probe, "probe folderId timeout--determine if someone"
                              " listening at the given folder");

    //loop and interpret all command lines
    catch_signal(SIGCHLD,SIG_IGN);
    cmd_shell(argc,argv);
    foldUtilExit(0);
}

////////////////////////////////////////////////////////

void foldUtilExit(int signo)
{
    if(signo > 0)
    {
        char msg[64];
        sprintf(msg,"catch a signal %d!\n",signo);
        write(1,msg,strlen(msg));
    }

    exit(signo);
}

////////////////////////////////////////////////////////

int do_mstat(int arg_c, char *arg_v[])
{
    mbuf_view_statistics(&gl_shmobj,stdout);
    return 0;
}

int do_minfo(int arg_c, char *arg_v[])
{
    mbuf_view_ctrlblk(&gl_shmobj,stdout);
    return 0;
}

int do_mlink(int arg_c, char *arg_v[])
{
    mbuf_view_link(&gl_shmobj,stdout);
    return 0;
}

int do_list(int arg_c, char *arg_v[])
{
    fold_list_entry(stdout);
    return 0;
}

int do_ctrlblk(int arg_c, char *arg_v[])
{
    fold_view_ctrlblk(stdout);
    return 0;
}

int do_probe(int arg_c, char *arg_v[])
{
    int  nready, fd[2];
    struct timeval  tmval;
    int  fid, org, chld_pid, tmout;
    char *ptr;
    fd_set rset;

    if(arg_c < 3)  return -1;

    //get the targeted folder
    fid = get_folder_Id(arg_v[1]);
    if(fid < 0)
        return -1;

    //create a temp folder to do this business
    org = fold_create_anonymous();
    if(org < 0)
    {
        printf("cannot create folder any more.\n");
        fold_delete_folder(fid);
        return 0;    
    } 
    
    tmout = atol(arg_v[2]);
    if(tmout <= 2)
        tmout = 2;
    
    if(0 > pipe(fd))
    {
        printf("cannot create pipe().\n");
        fold_delete_folder(fid);
        return 0;    
    }

    if((chld_pid=fork()) < 0)
    {
        printf("cannot fork().\n");
        goto lblreturn;
    }

    if(chld_pid== 0)
    {
        //child send the probe request and wait the ack
        fold_probe_request(fid,org);
        fold_read(org, NULL, NULL, 0, TRUE);
        write(fd[1], "", 1);
        exit(0);
    }

    close(fd[1]);
    FD_ZERO(&rset); FD_SET(fd[0],&rset);
    tmval.tv_sec  = tmout;
    tmval.tv_usec = 0;

    for(;;)
    {
        nready = select(fd[0]+1,&rset,NULL,NULL,&tmval);
        if(nready < 0)
        {
            if(errno == EINTR)  //select again
                continue;
            goto lblreturn;
        }

        if(nready == 0)  //timeout
        {
            kill(chld_pid, SIGKILL);
            fold_read(fid, NULL, NULL, 0, FALSE); //purge the probe request
            fold_read(org, NULL, NULL, 0, FALSE); //purge the probe ACK
            printf("probe timed out.\n");
            goto lblreturn;
        }

        printf("specified folder active.\n");   
        goto lblreturn;
    }//for

lblreturn:
    fold_destroy_folder(org);
    fold_delete_folder(fid);
    close(fd[0]); close(fd[1]);
    return 0;
}

int do_create(int arg_c, char *arg_v[])
{
    if(arg_c < 2)
        return -1;

    if( 0 >fold_create_folder(arg_v[1]))
        printf("cannot create folder,errno=%d\n",errno);
    return 0;
}

int do_delete(int arg_c, char *arg_v[])
{
    int fid, ret;
    char *ptr;

    if(arg_c < 2)
        return -1;

    if((fid = get_folder_Id(arg_v[1])) < 0)
        return -1;

    ret = fold_destroy_folder(fid);
    if(ret < 0)
    {
        printf("cannot delete the given folder,errno=%d\n",errno);
        return 0;
    }

    return 0;
}

int do_stop_sys(int arg_c, char *arg_v[])
{
    kill(gl_pFldCtrl->foldsvr_pid, 15);
    return 0;
}

int get_folder_Id(char *name)
{
    char  *ptr; int fid;

    for(ptr = name; *ptr; ptr++)
    {
        if('0' <= *ptr && *ptr <= '9')
            continue;
        else
            break;
    }

    if(*ptr==0) //all numeric char,maybe a folder ID
    {
        fid = atoi(name);
        if(fold_check_Id(fid) < 0)
            fid = fold_locate_folder(name);
    }
    else
        fid = fold_locate_folder(name);

    if(fold_check_Id(fid) < 0)
    {
        printf("no such folder identifier.\n");
        return -1;
    }

    return fid;
}

int do_purge (int arg_c, char *arg_v[])
{
    int fid, ret;
    char *ptr;

    if(arg_c < 2)
        return -1;

    if((fid = get_folder_Id(arg_v[1])) < 0)
        return -1;

    ret = fold_purge_msg(fid);

    return 0;
}

