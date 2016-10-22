#include <ibdcs.h>

int tm_init_queue(int nevents)
{
    struct eventitem  *evarray;
    int    i;

    if( !g_pTimeoutTbl || nevents <= 0 )
    {
        errno = EINVAL;
        return NULL;
    }

    // initialize control block
    //
    g_pTimeoutTbl->eq_magic    = EVENT_QUEUE_MAGIC;
    g_pTimeoutTbl->eq_nevents  = nevents;
    g_pTimeoutTbl->eq_nfrees   = nevents;
    g_pTimeoutTbl->eq_freehead = 0;
    g_pTimeoutTbl->eq_freetail = nevents - 1;
    g_pTimeoutTbl->eq_usedhead = -1;
    g_pTimeoutTbl->eq_usedtail = -1;

    evarray = g_pTimeoutTbl->eq_evarray;
    for(i=0; i < nevents; i++)
    {
        evarray[i].ei_id   = i;
        evarray[i].ei_next = i+1;
        evarray[i].ei_prev = i-1;
    }

    evarray[0].ei_prev = -1;
    evarray[nevents-1].ei_next = -1;
    return 0;
}
