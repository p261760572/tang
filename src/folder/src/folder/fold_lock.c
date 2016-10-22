#include <folder.h>

int fold_lock_folderlist()
{
    return sem_lock(gl_pFldCtrl->sem_id,gl_pFldCtrl->sem_count-1,TRUE);    
}

////////////////////////////////////////////////////////////////////////

int fold_unlock_folderlist()
{
    return sem_unlock(gl_pFldCtrl->sem_id,gl_pFldCtrl->sem_count-1,TRUE);    
}

////////////////////////////////////////////////////////////////////////

int fold_lock_folder(int folder_id)
{
    int sem_which;
    
    if(folder_id < 0 || folder_id >= gl_pFldCtrl->fold_nalloc)
        return -1;
    
    if(folder_id < gl_pFldCtrl->sem_count-1)
        sem_which = folder_id;
    else 
        sem_which = gl_pFldCtrl->sem_count -1;
            
    return sem_lock(gl_pFldCtrl->sem_id,sem_which,TRUE);        
}

////////////////////////////////////////////////////////////////////////

int fold_unlock_folder(int folder_id)
{
    int sem_which;
    
    if(folder_id < 0 || folder_id >= gl_pFldCtrl->fold_nalloc)
        return -1;
    
    if(folder_id < gl_pFldCtrl->sem_count-1)
        sem_which = folder_id;
    else 
        sem_which = gl_pFldCtrl->sem_count -1;
            
    return sem_unlock(gl_pFldCtrl->sem_id,sem_which,TRUE);        
}
