#include <ibdcs.h>

int tm_lock_freelist()
{
    if(sem_lock(g_pIbdcsShm->is_semid, 1, TRUE) < 0)
    {
        dcs_log(0,0,"dcs_lock_sysstat() failed:%s.\n",ise_strerror(errno));
        return -1;
    }

    return 0;
}

int tm_unlock_freelist()
{
    if(sem_unlock(g_pIbdcsShm->is_semid, 1, TRUE) < 0)
    {
        dcs_log(0,0,"dcs_lock_sysstat() failed:%s.\n",ise_strerror(errno));
        return -1;
    }

    return 0;

}

int tm_lock_usedlist()
{
    if(sem_lock(g_pIbdcsShm->is_semid, 2, TRUE) < 0)
    {
        dcs_log(0,0,"dcs_lock_sysstat() failed:%s.\n",ise_strerror(errno));
        return -1;
    }

    return 0;

}

int tm_unlock_usedlist()
{
    if(sem_unlock(g_pIbdcsShm->is_semid, 2, TRUE) < 0)
    {
        dcs_log(0,0,"dcs_lock_sysstat() failed:%s.\n",ise_strerror(errno));
        return -1;
    }

    return 0;
}
