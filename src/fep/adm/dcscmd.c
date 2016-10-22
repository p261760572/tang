#include "ibdcs.h"

struct _ItemTbl
{
    char *strItem;
    int  idItem;
}
gs_ItemTbl[]=
{
    {"RunMode",         RUN_MODE},
    {"T1",              TIMEOUT1},
    {"T2",              TIMEOUT2},
    {"T3",              TIMEOUT3},
    {NULL,              -1}
};


//forward declaration
static void dcsCmdExit(int signo);
static int ListSysStat (int arg_c, char *arg_v[]);
static int ListSysConf (int arg_c, char *arg_v[]);
static int SetVal(int arg_c, char *arg_v[]);
static int StopMoni(int arg_c, char *arg_v[]);
static int ViewTmQEntry(int arg_c, char *arg_v[]);
static int Viewtmqusage(int arg_c, char *arg_v[]);
static int delete_tmq_entry( int arg_c, char *arg_v[] );

int main(int argc, char *argv[])
{
  int i;
  char prompt[1024];

  catch_all_signals(dcsCmdExit);

  if(dcs_connect_shm() < 0)
  {
    printf("cannot connect to SHM!\n");
    exit(1);
  }

  cmd_init("srvcmd>>",dcsCmdExit);
 // cmd_add("sysstat", ListSysStat,   "sysstat--print current status of IBDCS");
  cmd_add("sysconf", ListSysConf,   "sysconf--print configue info of IBDCS");
  cmd_add("stop", StopMoni,      "stop--terminate the monitor of IBDCS");
//  cmd_add("listtmq", ViewTmQEntry, "listtmq--view timer queue entries");
//  cmd_add("tmqusage", Viewtmqusage,  "tmqusage--view timer queue linkage");
//  cmd_add("rmtmq", delete_tmq_entry,  "rmtmq--romove a timer queue entry by id or key");
      
  memset(prompt, 0, sizeof(prompt));
  sprintf(prompt,"set item val--modify system status or config item,\n"
                 "where item is one of:\n\n");
  for(i=0; gs_ItemTbl[i].strItem;i++)
  {
    sprintf(prompt+strlen(prompt),"%-13s ",gs_ItemTbl[i].strItem);
    if((i+1) % 5 == 0)
        sprintf(prompt+strlen(prompt),"\n");
  }
  cmd_add("set",SetVal,prompt);

  //loop and interpret all command lines
  cmd_shell(argc,argv);
  dcsCmdExit(0);
}

static void dcsCmdExit(int signo)
{
    exit(signo);
}
/*
static int ListSysStat (int arg_c, char *arg_v[])
{
    dcs_dump_sysstat(stdout);
    return 0;
}
*/
static int ListSysConf (int arg_c, char *arg_v[])
{
     dcs_dump_sysconf(stdout);
     return 0;
}

static int SetVal(int arg_c, char *arg_v[])
{
    int i;

    if(arg_c < 3)
        return -1;

    for(i=0; gs_ItemTbl[i].strItem; i++)
    {
        if(u_stricmp(gs_ItemTbl[i].strItem, arg_v[1])!=0)
            continue;
        else
            break;
    }

    if(gs_ItemTbl[i].strItem == NULL)
    {
        printf("illegal item %s\n",arg_v[1]);
        return 0;
    }

    if(TIMEOUT1 <= gs_ItemTbl[i].idItem && gs_ItemTbl[i].idItem<=  RUN_MODE)
    {
        //system configure item
        dcs_set_sysconf(gs_ItemTbl[i].idItem,atoi(arg_v[2]));
        return 0;
    }

    return -1;
}

static int StopMoni(int arg_c, char *arg_v[])
{
    
    
//    execl("foldUtil","stop", (char*)0);

    system("foldUtil stop && killall timescanner && killall notiscanner"); 
    kill( g_pIbdcsShm->is_MoniPid,SIGTERM);
    
    return 0;
}
/*
static int ViewTmQEntry(int arg_c, char *arg_v[])
{
     tm_view_queue(stdout);
     return 0;
}

static int Viewtmqusage(int arg_c, char *arg_v[])
{
    tm_view_tmqlink( stdout );
}

static int delete_tmq_entry( int arg_c, char *arg_v[] )
{
    int     evid;
    evkey_t key;
    char outbuf[2048];        
    //first we assume the arg_v[1] is event id
    evid = atoi( arg_v[1] );
    if( 0 == tm_dequeue_txn( evid ) )
        return 0;
    
    //second we assume the arg_[1] is event key data
    evkey_makekey(&key, arg_v[1], strlen(arg_v[1]) );
    
    evid = tm_locate_txn_bykey( key );
    if( evid < 0)
    {
        printf("no such entry.\n");    
        return 0;
    }
    tm_get_txndata(evid, outbuf, sizeof(outbuf)); 
    tm_dequeue_txn( evid ); 
    return 0;
}

*/