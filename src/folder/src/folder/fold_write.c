#include <folder.h>

static int fold_send_msgpacket(int ,int , int ,int, int);

////////////////////////////////////////////////////////////////////////

int fold_write(int dest_folderId,int org_folderId,void *user_data,int nbytes)
{
    //Description:  this function writes the application data into a destined
    //              folder
    //Argument:     dest_folderId   folder to write to
    //              org_folderId    the orginator's folder, -1 denotes no 
    //                              matter whatever orginator is
    //              user_data       a buffer containing application data
    //              nbytes          bytes number of data contained in user_data
    //Return value: if successful, this function returns the real bytes 
    //              number written to destination folder;
    //              if failed,-1 will be returnd and  errno specifies detailed
    //              failure cause:
    //              FOLD_ENOSYS   the caller not attached to Folder system
    //              FOLD_EINVAL   invalid argument in function call
    //              FOLD_ENOENT   specified folder not existing
    //              FOLD_EMSGOVFLW  message overflow
    //              FOLD_ENOMBUF  not enough buffer in shared memory hold data
    //              other valude  error code generated by Unix
    
    int    cluster_id = -1;
    int    nwritten   = 0;
    int    ret, nmaxmsg;
    
    // make sure all arguments valid
    if(fold_isattach() < 0)
    {
        errno = FOLD_ENOSYS;
        return -1;    
    }
     
    if((fold_check_Id(dest_folderId)) < 0 || 
       (org_folderId >= 0 && fold_check_Id(org_folderId) < 0))
    {
        errno = FOLD_ENOENT;
        return -1;    
    }
    
    nmaxmsg =  gl_pFldArray[dest_folderId].fold_nmaxmsg;
    if((nmaxmsg > 0 && gl_pFldArray[dest_folderId].fold_nmsg >= nmaxmsg)|| 100 < gl_pFldArray[dest_folderId].fold_nmsg)
    {
        //too much message in specified folder
        errno = FOLD_EMSGOVFLW;  
        return -1;
    }
    
    if(user_data && nbytes > 0)
    {
        // try to allocate cluster to hold data
        cluster_id = mbuf_alloc(&gl_shmobj,nbytes);   
        if(cluster_id < 0)
        {
            errno = FOLD_ENOMBUF;
            return -1;
        }
        
        //put the data
        nwritten=mbuf_putdata(&gl_shmobj,cluster_id,user_data,nbytes);
        if(nwritten < 0)
        {
            mbuf_free(&gl_shmobj,cluster_id);
            return -1;
        }
    }
    
    ret = fold_send_msgpacket(dest_folderId,org_folderId,
                                            cluster_id,FOLD_MSG_DATA,0);
    if(ret < 0) //sending failed
    {
        if(cluster_id>=0)
            mbuf_free(&gl_shmobj,cluster_id);
        return -1;
    }
    
    return nwritten;
}

////////////////////////////////////////////////////////////////////////

int fold_probe_request(int dest,int org)
{
    // make sure all arguments valid
    if(fold_isattach() < 0)
    {
        errno = FOLD_ENOSYS;
        return -1;    
    }
     
    if((fold_check_Id(dest)) < 0 || (org >= 0 && fold_check_Id(org) < 0))
    {
        errno = FOLD_ENOENT;
        return -1;    
    }
    
    return fold_send_msgpacket(dest,org,-1,FOLD_MSG_PROBE_REQ,0);    
}

////////////////////////////////////////////////////////////////////////

int fold_probe_ack(int dest,int org)
{
    // make sure all arguments valid
    if(fold_isattach() < 0)
    {
        errno = FOLD_ENOSYS;
        return -1;    
    }
     
    if((fold_check_Id(dest)) < 0 || (org >= 0 && fold_check_Id(org) < 0))
    {
        errno = FOLD_ENOENT;
        return -1;    
    }
    
    return fold_send_msgpacket(dest,org,-1,FOLD_MSG_PROBE_ACK,0);    
}

////////////////////////////////////////////////////////////////////////

int fold_send_msgpacket(int dest,int org, int cluster_id,int command, int flags)
{
    struct msgpacket  pkt, *pPkt;
    int    ret;
        
    pPkt = &pkt;
    pPkt->msg_dest       = dest;
    pPkt->msg_org        = org;
    pPkt->msg_clusterid  = cluster_id;
    pPkt->msg_command    = command;
    pPkt->msg_flags      = flags;
    pPkt->msg_tickcount  = fold_sys_tickcount();
    
    pPkt->msg_address = gl_pFldArray[dest].fold_address;
    ret = queue_send(gl_pFldCtrl->msg_viaqid, pPkt, 
                   sizeof(struct msgpacket)-sizeof(long),FALSE);
    if(ret >= 0)
    {
        fold_lock_folder(dest);
        gl_pFldArray[dest].fold_nmsg ++;
        fold_unlock_folder(dest);        
        
        if(gl_pFldArray[dest].fold_flags & FOLD_FLAG_NEEDSIG)
            kill(gl_pFldArray[dest].fold_pid,SIGUSR1);    
    }
    return ret;
}
