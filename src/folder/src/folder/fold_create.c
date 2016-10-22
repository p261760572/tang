#include <folder.h>

int  fold_create_folder(const char *folder_name)
{
    //Description:  this function tries to create a normal folder with the given 
    //              name
    //Argument:     fold_name   name of folder to be create
    //Return value: if successful, this function will return the Id of the newly
    //              created folder;
    //              if failed,-1 will be returnd and  errno specifies detailed
    //              failure cause:
    //              FOLD_ENOSYS   the caller not attached to Folder system
    //              FOLD_EINVAL   invalid argument in function call
    //              FOLD_ENOFOLD  no free entry for creating new folder
    //              FOLD_EEXIST   the folder 'folder_name' already exists
    
    struct qrypacket  pkt;    
    
    if(fold_isattach() < 0)
    {
        errno = FOLD_ENOSYS;
        return -1;    
    }
    
    //make sure argument valid
    if(!folder_name || !folder_name[0])
    {
        errno =FOLD_EINVAL;
        return -1;    
    }
    
    //package and send request
    memset(&pkt, 0, sizeof(struct qrypacket));
    pkt.fold_command = QRYPKT_CMD_CREATE;
    pkt.fold_flags   = QRYPKT_FLG_NORMAL;
    strncpy(pkt.fold_name,folder_name,
            min(MAX_FOLDNAME_SIZE,strlen(folder_name)));
    
    if(0 > fold_send_request(&pkt))
        return -1;
        
    //wait the response and return   
    return fold_wait_answer(&pkt);
}

////////////////////////////////////////////////////////////////////////

int fold_create_anonymous()
{
    //Description:  this function tries to create a anonymous folder
    //Argument:     No argument.
    //Return value: if successful, this function will return the Id of the newly
    //              created folder;
    //              if failed,-1 will be returnd and  errno specifies detailed
    //              failure cause:
    //              FOLD_ENOSYS   the caller not attached to Folder system
    //              FOLD_ENOFOLD  no free entry for creating new folder
    
    struct qrypacket  pkt;    
    
    if(fold_isattach() < 0)
    {
        errno = FOLD_ENOSYS;
        return -1;    
    }
    
    //package and send request
    memset(&pkt, 0, sizeof(struct qrypacket));
    pkt.fold_command = QRYPKT_CMD_CREATE;
    pkt.fold_flags   = QRYPKT_FLG_ANONYMOUS;
    
    if(0 > fold_send_request(&pkt))
        return -1;
        
    //wait the response and return
    return fold_wait_answer(&pkt);
}

////////////////////////////////////////////////////////////////////////

int fold_delete_folder(int folder_id)
{
    //Description:  this function tries to delete a folder with given Id
    //Argument:     folder_id    Id of folder to be deleted
    //Return value: if successful, this function will return 0;
    //              if failed,-1 will be returnd and  errno specifies detailed
    //              failure cause:
    //              FOLD_ENOSYS   the caller not attached to Folder system
    //              FOLD_EINVAL   invalid argument in function call
    //              FOLD_ENOENT   the specified folder not exist
    //Note:         this function just decreases the reference count of the given 
    //              folder and the folder is removed only if its ref. count becomes 0 

    struct qrypacket  pkt;    
    
    if(fold_isattach() < 0)
    {
        errno = FOLD_ENOSYS;
        return -1;    
    }
    
    if(fold_check_Id(folder_id) < 0)
    {
        errno = FOLD_ENOENT;
        return -1;    
    }
    
    //package and send request
    memset(&pkt, 0, sizeof(struct qrypacket));
    pkt.fold_command = QRYPKT_CMD_DELETE;
    pkt.fold_flags   = QRYPKT_FLG_DELBYREF;
    sprintf(pkt.fold_name,"%d",folder_id);
        
    if(0 > fold_send_request(&pkt))
        return -1;
        
    //wait the response and return   
    return fold_wait_answer(&pkt);
}
 
////////////////////////////////////////////////////////////////////////
   
int fold_destroy_folder(int folder_id)
{
    //Description:  this function tries to destroy a folder with given Id
    //Argument:     folder_id    Id of folder to be deleted
    //Return value: if successful, this function will return 0;
    //              if failed,-1 will be returnd and  errno specifies detailed
    //              failure cause:
    //              FOLD_ENOSYS   the caller not attached to Folder system
    //              FOLD_EINVAL   invalid argument in function call
    //              FOLD_ENOENT   the specified folder not exist
    struct qrypacket  pkt;    
    
    if(fold_isattach() < 0)
    {
        errno = FOLD_ENOSYS;
        return -1;    
    }
    
    if(fold_check_Id(folder_id) < 0)
    {
        errno = FOLD_ENOENT;
        return -1;    
    }
    
    //package and send request
    memset(&pkt, 0, sizeof(struct qrypacket));
    pkt.fold_command = QRYPKT_CMD_DELETE;
    pkt.fold_flags   = QRYPKT_FLG_DELBYFORCE;
    sprintf(pkt.fold_name,"%d",folder_id);
        
    if(0 > fold_send_request(&pkt))
        return -1;
        
    //wait the response and return
    return fold_wait_answer(&pkt);
}
