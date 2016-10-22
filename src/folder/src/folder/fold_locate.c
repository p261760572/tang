#include <folder.h>

int  fold_locate_folder(const char *folder_name)
{
    //Description:  this function tries to locate a folder by the given 
    //              name; or get the Id of folder by its name
    //Argument:     fold_name   name of folder to locate
    //Return value: if successful, this function will return the Id of the given
    //              folder;
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
    
    //make sure argument valid
    if(!folder_name || !folder_name[0])
    {
        errno =FOLD_EINVAL;
        return -1;    
    }
    
    //package and send request
    memset(&pkt, 0, sizeof(struct qrypacket));
    pkt.fold_command = QRYPKT_CMD_LOCATE;
    pkt.fold_flags  |= 0;
    strncpy(pkt.fold_name,folder_name,
            min(MAX_FOLDNAME_SIZE,strlen(folder_name)));
    
    if(0 > fold_send_request(&pkt))
        return -1;
        
    //wait the response and return
    return fold_wait_answer(&pkt);
}

////////////////////////////////////////////////////////////////////////

int  fold_get_name(int folder_id, char *folder_name,int nsize)
{
    // get the name of a folder from its Id
    
    int n;
    
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
    
    if(folder_name == 0 || nsize < 1)
    {
        errno = FOLD_EINVAL;
        return -1;    
    }
    
    n = min(MAX_FOLDNAME_SIZE,nsize-1);
    memcpy(folder_name,gl_pFldArray[folder_id].fold_name,n);
    *(folder_name + n) = 0;
    return 0;
}
