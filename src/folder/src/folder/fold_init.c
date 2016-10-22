#include <folder.h>

int fold_initsys()
{
    //Description:  this function attaches the calling process to Folder 
    //              system,so the caller can exchange info with other processes 
    //              with Folder mechanism.
    //Argument:     No arguments.
    //Return value: if successful, this function returns 0, otherwise a -1
    //              will be returned and errno is set to FOLD_ENOSYS.
    
    char  *ptr = NULL;
    
    //if alreadey attached,just return
    shm_detach((char *)gl_shmobj.shm_pShmCtrl);
    shm_detach((char *)gl_pFldCtrl);
    gl_pFldCtrl=NULL;
    
    if(fold_isattach() == 0)
        return 0;
        
    //try connecting to buffer pool of Folder system
    memset(&gl_shmobj,0,sizeof(gl_shmobj));
    if(mbuf_connect(&gl_shmobj,BUFFER_POOL_SHM_NAME) < 0)
        goto failure;
        
    //try connecting to folder list of Folder system        
    ptr = (char *)shm_connect(FOLDER_LIST_SHM_NAME,NULL);
    if(ptr==NULL)
        goto failure;
    
    //get all pointers
    gl_pFldCtrl  = (FOLDCTRLBLK *)ptr;
    gl_pFldArray = (FOLDERENTRY *)(ptr + sizeof(FOLDCTRLBLK));           
         
    return 0;       

failure:
    errno = FOLD_ENOSYS;
    return -1;
}

////////////////////////////////////////////////////////////////////////

int fold_isattach()
{
    //Description:  this function determines whether the calling process has attached
    //              to Folder system or not yet.
    //Argument:     No arguments.
    //Return value: if the caller attached to Folder,this function returns 0, 
    //              otherwise -1
    
    if(gl_pFldCtrl==NULL)
        return -1;
    if(gl_pFldCtrl->shm_magic != FOLD_MAGIC)
        return -1;

    return 0;
}

////////////////////////////////////////////////////////////////////////

int fold_check_Id(int folder_id)
{
    if(folder_id < 0 || folder_id >= gl_pFldCtrl->fold_nalloc)
        return -1;
    if(!(gl_pFldArray[folder_id].fold_flags & FOLD_FLAG_USED))
        return -1;
    return 0;            
}

////////////////////////////////////////////////////////////////////////

int fold_sys_active()
{
    //check the server is active or not
    if(fold_isattach() == 0)
      return kill(gl_pFldCtrl->foldsvr_pid,0);
    else
      return -1;
}

////////////////////////////////////////////////////////////////////////

int fold_sys_tickcount()
{
    //get the tick count of folder system

    int ret;

    if(fold_isattach() <  0)
      return -1;

   //lock the whole folder list and fetch the tick count
   fold_lock_folderlist();
   ret = gl_pFldCtrl->fold_systickcount ++;
   fold_unlock_folderlist();

   return ret;
}
