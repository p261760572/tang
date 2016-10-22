#include <folder.h>


int fold_set_attr(int folder_id, int attr)
{
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

    gl_pFldArray[folder_id].fold_flags |= attr;

    if(gl_pFldArray[folder_id].fold_flags &  FOLD_FLAG_NEEDSIG)
        queue_readq_interruptable(1);

    return 0;
}

////////////////////////////////////////////////////////////////////////

int fold_unset_attr(int folder_id, int attr)
{
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

    gl_pFldArray[folder_id].fold_flags &= (~attr);

    if(gl_pFldArray[folder_id].fold_flags &  FOLD_FLAG_NEEDSIG)
        queue_readq_interruptable(1);
    else
        queue_readq_interruptable(0);

    return 0;
}

////////////////////////////////////////////////////////////////////////

int fold_set_maxmsg(int folder_id, int nmaxmsg)
{
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

    gl_pFldArray[folder_id].fold_nmaxmsg = nmaxmsg;
    return 0;
}

////////////////////////////////////////////////////////////////////////

int fold_get_maxmsg(int folder_id)
{
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

    return gl_pFldArray[folder_id].fold_nmaxmsg;
}

////////////////////////////////////////////////////////////////////////

int fold_count_messags(int folder_Id)
{
    //Description:  this function counts the message number in given folder
    //Argument:     folder_Id       folder to count
    //Return value: if successful, this function returns the messages 
    //              number contained 
    //              in folder 'folder_Id';
    //              otherwise,a -1 will be returnd and  errno specifies detailed
    //              failure cause:
    //              FOLD_ENOSYS   the caller not attached to Folder system
    //              FOLD_ENOENT   specified folder not existing
    
    if(fold_isattach() < 0)
    {
        errno = FOLD_ENOSYS;
        return -1;    
    }
     
    if(fold_check_Id(folder_Id) < 0)
    {
        errno = FOLD_ENOENT;
        return -1;    
    }

    return gl_pFldArray[folder_Id].fold_nmsg;    
}
