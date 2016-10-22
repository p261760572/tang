#include <ibdcs.h>

static evkey_t g_dummy_evkey = EVKEY_INITIALIZER;

static int _tm_enqueue(int fid, long expire, int action,
                                evkey_t keyval, void *databuf, int datalen)
{
    struct eventitem *evptr;

    //try to allocate a free slot in event queue to put this event
    evptr = tm_alloc_entry();
    if( evptr == NULL )
    {
        dcs_log(0,0, "no free event slot in queue.\n");
        return -1;
    }

    evptr->ei_timein = time(NULL);
    evptr->ei_expire = expire;
    evptr->ei_action = action;
    evptr->ei_fid    = fid;
    evptr->ei_key    = keyval;
    evptr->ei_bufid  = -1;

    if( databuf && datalen >= 0)
    {
        // try to allocate cluster to hold data
        evptr->ei_bufid = mbuf_alloc(&gl_shmobj, datalen);
        if(evptr->ei_bufid < 0)
        {
            tm_free_entry( evptr );
            errno = FOLD_ENOMBUF;
            return -1;
        }

        //put the data
        if(0 > mbuf_putdata(&gl_shmobj,evptr->ei_bufid,databuf, datalen) )
        {
            mbuf_free(&gl_shmobj,evptr->ei_bufid);
            tm_free_entry( evptr );
            return -1;
        }
    }

    tm_insert_entry( evptr );
    return evptr->ei_id;
}

static int _tm_dequeue(int evid)
{
    struct eventitem *evptr;
    struct eventqueue *evq = g_pTimeoutTbl;
    if( !is_valid_eventid(evq, evid) )
    {
        errno = EINVAL;
        return -1;
    }
    evptr = &evq->eq_evarray[evid];
    if( evptr->ei_bufid >= 0 )
        mbuf_free(&gl_shmobj,evptr->ei_bufid);
    tm_remove_entry( evptr );
    tm_free_entry( evptr );
    return 0;
}

int tm_set_timer(int fid, long interval)
{
    return _tm_enqueue(fid,interval,EV_ACT_TIMER,g_dummy_evkey,0,0);
}

int tm_unset_timer(int timerid)
{
    return _tm_dequeue( timerid );
}

int tm_enqueue_txn(int fid, long expire, evkey_t keyval, void* data, int bytes)
{
    return _tm_enqueue(fid,expire,EV_ACT_TXN,keyval,data,bytes);
}

int tm_dequeue_txn( int txnid)
{
    return _tm_dequeue( txnid );
}

int tm_locate_txn_bykey(evkey_t  key)
{
    struct eventitem *evptr;
    struct eventqueue *evq = g_pTimeoutTbl;
    int    id;

    tm_lock_usedlist();

    for( id=evq->eq_usedhead; is_valid_eventid(evq, id); )
    {
        if(evkey_equal( evq->eq_evarray[ id ].ei_key ,key) )
        {
            tm_unlock_usedlist();
            return id;
        }
        id = evq->eq_evarray[ id ].ei_next;
    }

    tm_unlock_usedlist();
    return -1;
}

int tm_get_keyvalue(int txnid, evkey_t* outkey)
{
    struct eventitem *evptr;
    struct eventqueue *evq = g_pTimeoutTbl;

    if( !is_valid_eventid(evq, txnid) || !outkey )
    {
        errno = EINVAL;
        return -1;
    }

    evptr = &evq->eq_evarray[txnid];
    if( !is_valid_event(evq, evptr) ||
        !(evptr->ei_flags & EV_FLG_USED) )
    {
        errno = EINVAL;
        return -1;
    }

    tm_lock_usedlist();
    *outkey = evptr->ei_key;
    tm_unlock_usedlist();

    return 0;
}

int tm_get_txndata(int txnid, void* outbuf, int bufsize)
{
    struct eventitem *evptr;
    struct eventqueue *evq = g_pTimeoutTbl;
    int    nread;

    if( !is_valid_eventid(evq, txnid) )
    {
        errno = EINVAL;
        return -1;
    }

    evptr = &evq->eq_evarray[txnid];
    if( !is_valid_event(evq, evptr) ||
        !(evptr->ei_flags & EV_FLG_USED) )
    {
        errno = EINVAL;
        return -1;
    }

    if( evptr->ei_bufid < 0 )
        return -1;

    //pick the data
    if(bufsize > 0 && outbuf != NULL)
    {
        tm_lock_usedlist();
        nread=mbuf_getdata(&gl_shmobj, evptr->ei_bufid, outbuf, bufsize);
        tm_unlock_usedlist();
    }
    else
        nread = 0;

    return nread;
}
