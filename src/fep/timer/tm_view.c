#include <ibdcs.h>

static char *timeAsString(long t, int flag)
{
    static char strTime[64];
    struct tm *tm;
        
    tm = localtime(&t);
    
    if( flag == 1)
        sprintf(strTime, "%.2d:%.2d:%.2d",tm->tm_hour,tm->tm_min,tm->tm_sec);
    else
        sprintf(strTime,"%4d/%02d/%02d %02d:%02d:%02d",
            tm->tm_year+1900,
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec);
    return strTime;
}

int tm_view_queue(FILE* fp)
{
    int    id, j=0;
    struct eventitem *eiptr;
    struct eventqueue *evq = g_pTimeoutTbl;
    time_t  t;
    
    if(!fp) fp = stdout;

    if( !is_valid_evqueue(evq) )
    {
        fprintf(fp, "timer queue corrupted!\n");
        return -1;
    }
    
    time(&t);
    fprintf(fp,"Ord     ID    InTime    Life(s)  Key             Type   RegFid  Expired\n");
    fprintf(fp,"-----------------------------------------------------------------------\n");

    for( id=evq->eq_usedhead;
         is_valid_eventid(evq, id);
         id = evq->eq_evarray[id].ei_next)
    {
        eiptr = &evq->eq_evarray[ id ];
        j++;
        
        fprintf(fp, "[%.4d]  %.4d  %8s  %-4d     %14s  %5s  %-6d  %d\n",
                    j,
                    eiptr->ei_id,
                    timeAsString(eiptr->ei_timein, 1),
                    eiptr->ei_expire,
                    eiptr->ei_key.k_val,
                    eiptr->ei_action == EV_ACT_TIMER ? "TIMER" : "TRANS",
                    eiptr->ei_fid,
                    eiptr->ei_flags & EV_FLG_NOTIFIED);
    }//for

    fprintf(fp,"-----------------------------------------------------------------------\n");
    fprintf(fp,"                                                    %s\n",timeAsString(t, 0));        
    fflush( fp );
    return 0;
}

int tm_view_tmqlink(FILE* fp)
{
    int    id, j=0;
    struct eventitem *eiptr;
    struct eventqueue *evq = g_pTimeoutTbl;
    
    if(!fp) fp = stdout;

    if( !is_valid_evqueue(evq) )
    {
        fprintf(fp, "timer queue corrupted!\n");
        return -1;
    }
    
    //free entry list
    fprintf(fp,"[free entry link]\n");
    for(j=0, id=evq->eq_freehead; id>=0; id = evq->eq_evarray[id].ei_next, j++)
    {
        eiptr = &evq->eq_evarray[ id ];
        
        if( id == evq->eq_freetail)
        {
            fprintf(fp,"%.4d\n",eiptr->ei_id);
            break;
        }
        else
            fprintf(fp,"%.4d-->",eiptr->ei_id);

        if(((j+1) % 11) == 0)
          fprintf(fp,"\n");
    }

    //free entry list
    fprintf(fp,"\n[used entry link]\n");
    for(j=0, id=evq->eq_usedhead; id>=0; id = evq->eq_evarray[id].ei_next)
    {
        eiptr = &evq->eq_evarray[ id ];
        j++;
        
        if( id == evq->eq_usedtail)
        {
            fprintf(fp,"%.4d\n",eiptr->ei_id);
            break;
        }
        else
            fprintf(fp,"%.4d-->",eiptr->ei_id);

        if(((j+1) % 11) == 0)
          fprintf(fp,"\n");
    }
    
    fprintf(fp,"\nUsage Stastics:\n");
    fprintf(fp,"Allocated Entry : %d\n", evq->eq_nevents );
    fprintf(fp,"Used Entry      : %d\n", j);
    fprintf(fp,"Free Entry      : %d\n", evq->eq_nfrees);
    fflush( fp );
    return 0;
}
