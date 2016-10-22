#include <ibdcs.h>

struct eventitem *tm_alloc_entry()
{
    // pick from free list

    struct eventitem *eiptr;
    struct eventqueue *evq = g_pTimeoutTbl; 
    
    if( !is_valid_evqueue(evq) )
    {
        errno = EINVAL;
        return NULL;
    }

    tm_lock_freelist(evq);

    if( evq->eq_freehead < 0)
    {
        tm_unlock_freelist(evq);
        return NULL;
    }

    // allocate form head to tail
    //
    eiptr = &evq->eq_evarray[evq->eq_freehead];
    evq->eq_freehead = eiptr->ei_next;
    if(evq->eq_freehead < 0 )
        evq->eq_freetail = -1;  //become empty
    else
        evq->eq_evarray[evq->eq_freehead].ei_prev = -1;
    evq->eq_nfrees --;
    tm_unlock_freelist(evq);

    eiptr->ei_next = -1;
    eiptr->ei_prev = -1;
    eiptr->ei_flags |= EV_FLG_USED;
    return eiptr;
}

int tm_free_entry( struct eventitem *ev)
{
    // return to free list

    struct eventitem *eiptr;
    struct eventqueue *evq = g_pTimeoutTbl;

    if( !is_valid_evqueue(evq) ||
        !is_valid_event(evq, ev) ||
        !(ev->ei_flags & EV_FLG_USED) )
    {
        errno = EINVAL;
        return -1;
    }

    ev->ei_next = -1;
    ev->ei_prev = -1;
    ev->ei_flags &= ~EV_FLG_USED;

    tm_lock_freelist(evq);

    // free at tail
    //
    if( evq->eq_freetail < 0 )
    {
        evq->eq_freehead = ev->ei_id;
        evq->eq_freetail = ev->ei_id;
    }    
    else
    {
        eiptr = &evq->eq_evarray[evq->eq_freetail];
        eiptr->ei_next = ev->ei_id;
        ev->ei_prev = eiptr->ei_id;
        evq->eq_freetail = ev->ei_id;
    }
    evq->eq_nfrees ++;

    tm_unlock_freelist(evq);
         
    return 0;
}

int tm_insert_entry(struct eventitem *ev)
{
    // insert into used list

    struct eventitem *eiptr;
    struct eventqueue *evq = g_pTimeoutTbl;    

    if( !is_valid_evqueue(evq) ||
        !is_valid_event(evq, ev) ||
        !(ev->ei_flags & EV_FLG_USED) )
    {
        errno = EINVAL;
        return -1;
    }

    ev->ei_prev = ev->ei_next = -1;
    
    tm_lock_usedlist(evq);

    if( evq->eq_usedhead < 0 )
        evq->eq_usedhead = evq->eq_usedtail = ev->ei_id;
    else
    {
        eiptr = &evq->eq_evarray[evq->eq_usedtail];
        eiptr->ei_next = ev->ei_id;
        ev->ei_prev = eiptr->ei_id;
        evq->eq_usedtail = ev->ei_id;
    }

    tm_unlock_usedlist(evq);
    return 0;
}

int tm_remove_entry(struct eventitem *ev)
{
    // pick off the item from used list

    struct eventqueue *evq = g_pTimeoutTbl;

    if( !is_valid_evqueue(evq) ||
        !is_valid_event(evq, ev) ||
        !(ev->ei_flags & EV_FLG_USED) )
    {
        errno = EINVAL;
        return -1;
    }

    tm_lock_usedlist(evq);
    
    //pick this entry off from timer queue table
    //
    if( evq->eq_usedhead == ev->ei_id ) //at head
    {
        evq->eq_usedhead = evq->eq_evarray[ev->ei_id].ei_next;
        if( is_valid_eventid(evq, evq->eq_usedhead) )
            evq->eq_evarray[evq->eq_usedhead].ei_prev = -1;
    }
    else if( evq->eq_usedtail == ev->ei_id ) //at tail
    {
        evq->eq_usedtail = evq->eq_evarray[ev->ei_id].ei_prev;
        if( is_valid_eventid(evq, evq->eq_usedtail) )
            evq->eq_evarray[evq->eq_usedtail].ei_next = -1;
    }
    else
    {
        struct eventitem *curp, *next, *prev;

        curp = ev;
        prev = &evq->eq_evarray[ curp->ei_prev ];
        next = &evq->eq_evarray[ curp->ei_next ];
        prev->ei_next = next->ei_id;
        next->ei_prev = prev->ei_id;
    }

    if( evq->eq_usedtail < 0 )
        evq->eq_usedhead = -1;
    if( evq->eq_usedhead < 0 )
        evq->eq_usedtail = -1;

    tm_unlock_usedlist(evq);
    ev->ei_next = ev->ei_prev = -1;
    return 0;
}
