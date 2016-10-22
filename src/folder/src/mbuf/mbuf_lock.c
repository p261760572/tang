#include <mbuf_func.h>

int mbuf_lock(const SHMOBJ *shmobj)
{
    int semid;
    
    semid = shmobj->shm_pShmCtrl->shm_sem;
    for(;;)
    {
        if(0 > sem_lock(semid,0,TRUE))
        {
            if(errno == EINTR) continue;
            else  //lethal erro
              return -1;    
        }
        else
            return  0;
    }
}

//----------------------------------------------------

int mbuf_unlock(const SHMOBJ *shmobj)
{
    int semid;
    
    semid = shmobj->shm_pShmCtrl->shm_sem;
    for(;;)
    {
        if(0 > sem_unlock(semid,0,TRUE))
        {
            if(errno == EINTR) continue;
            else  //lethal erro
              return -1;    
        }
        else
            return  0;
    }
}
