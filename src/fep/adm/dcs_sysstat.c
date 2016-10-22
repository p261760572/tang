#include "ibdcs.h"

static int dcs_lock_sysstat();
static int dcs_unlock_sysstat();


int dcs_lock_sysstat()
{
    if(sem_lock(g_pIbdcsShm->is_semid, 0, TRUE) < 0)
    {
        dcs_log(0,0,"dcs_lock_sysstat() failed:%s.\n",ise_strerror(errno));
        return -1;
    }

    return 0;
}

int dcs_unlock_sysstat()
{
    if(sem_unlock(g_pIbdcsShm->is_semid, 0, TRUE) < 0)
    {
        dcs_log(0,0,"dcs_unlock_sysstat() failed:%s.\n",ise_strerror(errno));
        return -1;
    }

    return 0;
}
