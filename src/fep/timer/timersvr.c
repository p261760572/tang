#include "ibdcs.h"
#include <errno.h>
//forward declaration
static void timerSvrExit(int signo);

static int SendTimedoutNotification(struct eventitem *ev);
static int DoLoop();
static int OpenLog(char *IDENT);

int main(int argc, char *argv[])
{
    //prepare the logging stuff
    if(0 > OpenLog(argv[0]))
        exit(1);

    dcs_log(0,0, "TimerServer is starting up...\n");

    catch_all_signals(timerSvrExit);

    //attach to SHM of IBDCS
    if(dcs_connect_shm() < 0)
    {
        dcs_log(0,0,"dcs_connect_shm() failed:%s\n",ise_strerror(errno));
        printf("dcs_connect_shm() failed:%s\n",ise_strerror(errno));
        exit(1);
    }
/*
    if(g_pIbdcsShm->is_MoniPid != getppid())
    {
        dcs_log(0,0,"invalid invokation of '%s'\n",argv[0]);
        printf("invalid invokation of '%s'\n",argv[0]);
        exit(1);
    }
*/
    if(fold_initsys() < 0)
    {
        dcs_log(0,0, "cannot attach to folder system!");
        return -1;
    }

    dcs_log(0,0,"*************************************************\n"
                "!!        TimerServer startup completed.       !!\n"
                "*************************************************\n");

    DoLoop();

    timerSvrExit(0);
}//main()

static void timerSvrExit(int signo)
{
    if(signo > 0)
        dcs_debug(0,0,"catch a signal %d\n",signo);

    if(signo !=0 && signo != SIGTERM )
        return;

    dcs_log(0,0,"TimerServer terminated.\n");
//    dcs_log_close();
    exit(signo);
}

static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/timersvr.log"
    if(u_fabricatefile("log/timersvr.log",logfile,sizeof(logfile)) < 0)
        return -1;

    return dcs_log_open(logfile, IDENT);
}

static int DoLoop()
{
    struct eventqueue *evq = g_pTimeoutTbl;
    int    id,eid;
    time_t now, expiredTime;
    
    for(;;)
    {
//        if( getppid() == 1 ) //my parent gone
 //           break;

//        sleep(1);
        usleep(500000);
        now = time(NULL);
        eid = -1;
        tm_lock_usedlist();

        for( id=evq->eq_usedhead; 
             is_valid_eventid(evq, id); 
             id = evq->eq_evarray[ id ].ei_next)
        {
            expiredTime = evq->eq_evarray[ id ].ei_timein + 
                         evq->eq_evarray[ id ].ei_expire;
            if( expiredTime > now )
                continue;
                
            if( evq->eq_evarray[ id ].ei_action == EV_ACT_TXN )    
            {
                if( evq->eq_evarray[ id ].ei_flags & EV_FLG_NOTIFIED )    
            	{
			dcs_debug(0,0,"time out ei_flags = %d,slot=%d" ,evq->eq_evarray[id].ei_flags,id);
//                    tm_dequeue_txn(id);
                    eid=id;
                    continue;
							}
            }
           
            //send a timed out notification to registerer
            SendTimedoutNotification( &evq->eq_evarray[ id ] );
            
            evq->eq_evarray[ id ].ei_flags |= EV_FLG_NOTIFIED;
            
            if( evq->eq_evarray[ id ].ei_action == EV_ACT_TIMER )
            {
                //reset timer
                evq->eq_evarray[ id ].ei_timein = time(NULL);
            }
        }//for(;;)

        tm_unlock_usedlist();
     if ( eid >=0)
       tm_dequeue_txn(eid);
    
    }//for
}

static int SendTimedoutNotification(struct eventitem *ev)
{
    struct IBDCSPacket pkt;
    char caBuf[100]; 
    if ( ev ->ei_action ==EV_ACT_TXN )
   	 pkt.pkt_cmd   = PKT_CMD_TMOUT;
    else
   	 pkt.pkt_cmd   = PKT_CMD_TIMER;
    pkt.pkt_val   = ev->ei_id;
    pkt.pkt_bytes = 0;
    memset(caBuf,0,sizeof(caBuf));
    memcpy(caBuf,"TIME",4);
    memcpy(caBuf+4,&pkt,sizeof(pkt)); 
    dcs_debug(0,0,"time out fid = %d" ,ev->ei_fid);
    if(0 > fold_write( ev->ei_fid, -1, caBuf, sizeof(pkt)+4) )
    {
        dcs_log(0,0, "fold_write() failed: %s\n", ise_strerror(errno) );
        return -1;    
    }
    
    return 0;
}
