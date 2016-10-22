#ifndef __TIMER__H__
#define __TIMER__H__

#define  EV_K_LEN   72 

typedef struct 
{
    int  k_len;
    unsigned char  k_val[EV_K_LEN];
}evkey_t;

#define EVKEY_INITIALIZER  {0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} }

// each event item
//
struct eventitem
{
    long    ei_id;       //id of this event item
    long    ei_expire;   //when does message expire
    long    ei_timein;   //when did message arrive
    int     ei_flags;    //flags which can be set
    short   ei_action;   //what to do when msg expires
    int     ei_fid;      //which folder to send time out's to
    evkey_t ei_key;      //key by which sender knows message
    short   ei_bufid;    //which buffer cluster contains data of event
    
    short   ei_next;     //id of next event item
    short   ei_prev;     //id of previous event item
};

// defines action for event
//
#define EV_ACT_TXN      1   //transaction
#define EV_ACT_TIMER    3   //timer

// defines flags for event
//
#define EV_FLG_NOTIFIED  0x00000001 //owner of expired event notified already
#define EV_FLG_USED      0x00000008

struct eventqueue
{
    long eq_magic;
    long eq_nevents;   //how many event items alloacted in event queue
    long eq_nfrees;    //how many free event items available
    
    int  eq_freehead;  //head event item in free event list
    int  eq_freetail;  //tail event item in free event list
    int  eq_usedhead;  //head event item in used event list
    int  eq_usedtail;  //tail event item in used event list

    struct eventitem eq_evarray[1];
};

#define  EVENT_QUEUE_MAGIC    0x20001009

#define is_valid_evqueue( evq ) \
    ( (evq) && ((struct eventqueue *)evq)->eq_magic==EVENT_QUEUE_MAGIC )

#define is_valid_event(evq, ev) \
        ( (ev) && ((ev)->ei_id >=0) && ((ev)->ei_id < (evq)->eq_nevents) )

#define is_valid_eventid(evq, id) \
        ( ((id) >=0) && ((id) < (evq)->eq_nevents) )

#ifdef _cplusplus
extern "C" {
#endif

int tm_init_queue(int nevents);

struct eventitem *tm_alloc_entry();
int tm_free_entry(struct eventitem *ev);
int tm_insert_entry(struct eventitem *ev);
int tm_remove_entry(struct eventitem *ev);

int tm_lock_freelist();
int tm_unlock_freelist();
int tm_lock_usedlist();
int tm_unlock_usedlist();

int tm_set_timer(int fid, long interval);
int tm_unset_timer(int timerid);
int tm_enqueue_txn(int fid, long expire, evkey_t keyval, void* data, int bytes);
int tm_dequeue_txn( int txnid);
int tm_locate_txn_bykey(evkey_t  key);
int tm_get_txndata(int txnid, void* outbuf, int bufsize);
int tm_get_keyvalue(int txnid, evkey_t* outkey);

int evkey_makekey( evkey_t *outkey, void *keydata, int keylen );

int tm_view_queue(FILE* fp);
int tm_view_tmqlink(FILE* fp);

#ifdef _cplusplus
}
#endif

#endif  //__TIMER__H__

