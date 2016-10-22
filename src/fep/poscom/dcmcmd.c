#define _MainProg

//filename:  dcmcmd.c
//           main()

#include "ibdcs.h"
#include "dccdcs.h"
#include <folder.h>
static   struct trmcom *g_pTrmCtrl = NULL;
static   struct trmgrpinfo * g_pTrmLink =NULL;
static   struct trminfo  *g_pTrmInfo;
static   int            g_iPosSrvFoldId = -1;

static   void   DcmCmdExit(int signo);

//forward declaration
int ListTrm   (int arg_c, char *arg_v[]);
int KillTrm   (int arg_c, char *arg_v[]);
int dumpBcda  (int arg_c, char *arg_v[]);
int list      (int arg_c, char *arg_v[]);

int StopLink  (int arg_c, char *arg_v[]);
int StartLink (int arg_c, char *arg_v[]);
int AddLink   (int arg_c, char *arg_v[]);
int ModifyLink   (int arg_c, char *arg_v[]);
int DeleteLink   (int arg_c, char *arg_v[]);

int main(int argc, char *argv[])
{
  catch_all_signals(DcmCmdExit);

  //attach to folder system
  if(fold_initsys() < 0)
  {
    fprintf(stderr,"cannot attach to FOLDER system!\n");
    exit(1);
  }

  //read all configuration

  cmd_init("poscmd>>",DcmCmdExit);
  cmd_add("list",list,"list -- list all group base info " );
  cmd_add("listtrm",ListTrm, "listtrm id--list a group status of terminals");
  cmd_add("killtrm",KillTrm, "killtrm--kill TrmSvr");
  	cmd_add("stop",    StopLink,  "stop id--stop a link");
  	cmd_add("start",   StartLink, "star id--start a link");
  	cmd_add("add",   AddLink, "add id--add a link");
  	cmd_add("update",   ModifyLink, "update id--update a link");
  	cmd_add("delete",   DeleteLink, "delete id--modify a link");

  //loop and interpret all command lines
  cmd_shell(argc,argv);
  DcmCmdExit(0);
}//main()

static void DcmCmdExit(int signo)
{
  //decrease references
  fold_delete_folder(g_iPosSrvFoldId);
  fprintf(stderr," catch signo =%d",signo);
  exit(signo);
}


int ListTrm   (int arg_c, char *arg_v[])
{
  char *strStat, *strPeerIp, *strConnWay;
  int   intPort, i;
  struct in_addr in;

  if ( arg_c <2)
  {
  	 fprintf(stderr,"Input Invalid parameter!\n");
  	 return 0;
  }
  if(g_pTrmCtrl == NULL)
  {
    //connect the shared memory TRMCOM,so we can dump its content
    g_pTrmCtrl = (struct trmcom *)shm_connect(DCS_POSCOM_NAME, NULL);
    if(g_pTrmCtrl == NULL)
    {
      fprintf(stderr,"TrmSvr not active!\n");
      return 0;
    }
  }
  for( i =0 ; i<g_pTrmCtrl->iTrmCnt; i++)
  {
  	  if ( g_pTrmCtrl->linkArray[i].iGid == atoi( arg_v[1]) )
  	       break;
  }
  if ( i >=g_pTrmCtrl->iTrmCnt)
  {
  		fprintf(stderr,"Can not found this grp id:%s\n",arg_v[1]);
  		return 0;
  }
  g_pTrmInfo = &g_pTrmCtrl->linkArray[i];
  if(g_pTrmLink == NULL)
  {
    g_pTrmLink = (struct trmgrpinfo *) shm_attach(g_pTrmInfo->iShmId);
    if(g_pTrmLink == NULL)
    {
      fprintf(stderr,"Can not connect link SHM  :%s shm_id=%d!\n",arg_v[1],g_pTrmInfo->iShmId);
      return 0;
    }
  }
  //dump status of all terminals
  fprintf(stderr,"Ord     FdName      GroupID Status       pidComm   LocalPort    RemoteAddr    \n");
  fprintf(stderr,"-------------------------------------------------------------------------------\n");

  for(i=0; i<g_pTrmInfo->minterm; i++)
  {
     switch(g_pTrmLink[i].iStatus)
     {
       case DCS_STAT_LISTENING: strStat = "listening  "; break;
       case DCS_STAT_CONNECTED: strStat = "connected  "; break;
       case DCS_STAT_READNET:   strStat = "readnet    "; break;
       case DCS_STAT_WRITENET:  strStat = "writenet   "; break;
       case DCS_STAT_WRITEFOLD:	strStat = "sendtoappl "; break;
       case DCS_STAT_READFOLD:  strStat = "waitappl   "; break;       
       case DCS_STAT_WAITREQ:   strStat = "waitnetreq "; break;
       case DCS_STAT_SENDREQ:   strStat = "sndrequest "; break;
       case DCS_STAT_WAITRESP:  strStat = "waitrespons"; break;
       default:                 strStat = "unkown     "; break;
     }
     if (strlen (g_pTrmLink[i].sRemoteIp)<2)
           strcpy(g_pTrmLink[i].sRemoteIp,"0.0.0.0");
     fprintf(stderr,"%.3d  %15.15s  %.3d   %s  %.6d   local:%d   %s:%d\n",
             i,
             g_pTrmLink[i].caFoldName,
             g_pTrmInfo->iGid,
             strStat,
             max(g_pTrmLink[i].pidComm,0),
             g_pTrmInfo->iLocalPort,
             g_pTrmLink[i].sRemoteIp,
             g_pTrmLink[i].iRemotePort);
  }//for
  shm_detach((char *)g_pTrmLink);
  shm_detach((char *)g_pTrmCtrl);
  g_pTrmCtrl=NULL;
  g_pTrmLink =NULL;
  return 0;
}

int list   (int arg_c, char *arg_v[])
{
  char  strConnWay[20],strStatus[20];
  int   intPort, i;
  struct in_addr in;

  
  if(g_pTrmCtrl == NULL)
  {
    //connect the shared memory TRMCOM,so we can dump its content
    g_pTrmCtrl = (struct trmcom *)shm_connect(DCS_POSCOM_NAME, NULL);
    if(g_pTrmCtrl == NULL)
    {
      fprintf(stderr,"TrmSvr not active!\n");
      return 0;
    }
  }
 
  //dump status of all terminals
  fprintf(stderr,"Ord     FdName      GroupID    Role   status  pidComm   LocalPort         RemoteAddr    \n");
  fprintf(stderr,"-----------------------------------------------------------------------------------------\n");

  for(i=0; i<g_pTrmCtrl->iTrmCnt; i++)
  {
    if (!( g_pTrmCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
               continue;
    if( g_pTrmCtrl->linkArray[i].iRole == DCS_ROLE_PASSIVE)
         strcpy(strConnWay,"passive");
    else
         strcpy(strConnWay,"active ");
    if ( g_pTrmCtrl->linkArray[i].iStatus == DCS_STAT_STOPPED )
           strcpy( strStatus,"stoped");
    else
    			strcpy(strStatus,"Runing");
     fprintf(stderr,"%.3d  %15.15s  %.3d   %s  %s  %.6d   local:%4d  %s:%d\n",
             i,
             g_pTrmCtrl->linkArray[i].caFoldName,
             g_pTrmCtrl->linkArray[i].iGid,
             strConnWay,
             strStatus,
             g_pTrmCtrl->linkArray[i].pidComm,
             g_pTrmCtrl->linkArray[i].iLocalPort,
             g_pTrmCtrl->linkArray[i].sRemoteIp,
             g_pTrmCtrl->linkArray[i].iRemotePort);
  }//for
  shm_detach((char *)g_pTrmCtrl);
  g_pTrmCtrl=NULL;
  return 0;
}

int KillTrm   (int arg_c, char *arg_v[])
{
  struct trmcmd Cmd;
  int    i, find = -1;

   if (g_iPosSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iPosSrvFoldId = fold_locate_folder(DCS_POSSRV_FOLD_NAME);
    		if (g_iPosSrvFoldId < 0)
    		{
      			fprintf(stderr,"PosSrv not active!\n");
      			return 0;
    		}
  	}

  //prepare the command and send it to TrmSrv
  sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
  Cmd.iCommand = DCS_CMD_EXIT;

  if(fold_write(g_iPosSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    fprintf(stderr,"request to TrmSrv failed:%d\n",(errno));

  return 0;
}




int StopLink  (int arg_c, char *arg_v[])
{
 		struct trmcmd Cmd;
  	int    i, find = -1;


   if ( arg_c <2)
  {
  	 fprintf(stderr,"Input Invalid parameter!\n");
  	 return 0;
  }
  if(g_pTrmCtrl == NULL)
  {
    //connect the shared memory TRMCOM,so we can dump its content
    g_pTrmCtrl = (struct trmcom *)shm_connect(DCS_POSCOM_NAME, NULL);
    if(g_pTrmCtrl == NULL)
    {
      fprintf(stderr,"TrmSvr not active!\n");
      return 0;
    }
  }
  for( i =0 ; i<g_pTrmCtrl->iTrmCnt; i++)
  {
  	  if ( g_pTrmCtrl->linkArray[i].iGid == atoi( arg_v[1]) )
  	       break;
  }
  if ( i >=g_pTrmCtrl->iTrmCnt)
  {
  		fprintf(stderr,"Can not found this grp id:%s\n",arg_v[1]);
  		return 0;
  }
  if (g_iPosSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iPosSrvFoldId = fold_locate_folder(DCS_POSSRV_FOLD_NAME);
    		if (g_iPosSrvFoldId < 0)
    		{
      			fprintf(stderr,"PosSrv not active!\n");
      			return 0;
    		}
  	}
  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
   	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_STOP;
   
  	if (fold_write(g_iPosSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to PosSrv failed,errno=%d\n",errno);

  	return 0;
}

int StartLink (int arg_c, char *arg_v[])
{
  		struct trmcmd Cmd;
  	int    i, find = -1;


   if ( arg_c <2)
  {
  	 fprintf(stderr,"Input Invalid parameter!\n");
  	 return 0;
  }
  if(g_pTrmCtrl == NULL)
  {
    //connect the shared memory TRMCOM,so we can dump its content
    g_pTrmCtrl = (struct trmcom *)shm_connect(DCS_POSCOM_NAME, NULL);
    if(g_pTrmCtrl == NULL)
    {
      fprintf(stderr,"TrmSvr not active!\n");
      return 0;
    }
  }
  for( i =0 ; i<g_pTrmCtrl->iTrmCnt; i++)
  {
  	  if ( g_pTrmCtrl->linkArray[i].iGid == atoi( arg_v[1]) )
  	       break;
  }
  if ( i >=g_pTrmCtrl->iTrmCnt)
  {
  		fprintf(stderr,"Can not found this grp id:%s\n",arg_v[1]);
  		return 0;
  }
  if (g_iPosSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iPosSrvFoldId = fold_locate_folder(DCS_POSSRV_FOLD_NAME);
    		if (g_iPosSrvFoldId < 0)
    		{
      			fprintf(stderr,"PosSrv not active!\n");
      			return 0;
    		}
  	}
  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
   	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_START;
   
  	if (fold_write(g_iPosSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to PosSrv failed,errno=%d\n",errno);

  	return 0;
}

int ModifyLink (int arg_c, char *arg_v[])
{
  		struct trmcmd Cmd;
  	int    i, find = -1;


   if ( arg_c <2)
  {
  	 fprintf(stderr,"Input Invalid parameter!\n");
  	 return 0;
  }
  if(g_pTrmCtrl == NULL)
  {
    //connect the shared memory TRMCOM,so we can dump its content
    g_pTrmCtrl = (struct trmcom *)shm_connect(DCS_POSCOM_NAME, NULL);
    if(g_pTrmCtrl == NULL)
    {
      fprintf(stderr,"TrmSvr not active!\n");
      return 0;
    }
  }
  for( i =0 ; i<g_pTrmCtrl->iTrmCnt; i++)
  {
  	  if ( g_pTrmCtrl->linkArray[i].iGid == atoi( arg_v[1]) )
  	       break;
  }
  if ( i >=g_pTrmCtrl->iTrmCnt)
  {
  		fprintf(stderr,"Can not found this grp id:%s\n",arg_v[1]);
  		return 0;
  }
  if (g_iPosSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iPosSrvFoldId = fold_locate_folder(DCS_POSSRV_FOLD_NAME);
    		if (g_iPosSrvFoldId < 0)
    		{
      			fprintf(stderr,"PosSrv not active!\n");
      			return 0;
    		}
  	}
  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
   	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_MODIFY;
   
  	if (fold_write(g_iPosSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to PosSrv failed,errno=%d\n",errno);

  	return 0;
}

int DeleteLink (int arg_c, char *arg_v[])
{
  		struct trmcmd Cmd;
  	int    i, find = -1;


   if ( arg_c <2)
  {
  	 fprintf(stderr,"Input Invalid parameter!\n");
  	 return 0;
  }
  if(g_pTrmCtrl == NULL)
  {
    //connect the shared memory TRMCOM,so we can dump its content
    g_pTrmCtrl = (struct trmcom *)shm_connect(DCS_POSCOM_NAME, NULL);
    if(g_pTrmCtrl == NULL)
    {
      fprintf(stderr,"TrmSvr not active!\n");
      return 0;
    }
  }
  for( i =0 ; i<g_pTrmCtrl->iTrmCnt; i++)
  {
  	  if ( g_pTrmCtrl->linkArray[i].iGid == atoi( arg_v[1]) )
  	       break;
  }
  if ( i >=g_pTrmCtrl->iTrmCnt)
  {
  		fprintf(stderr,"Can not found this grp id:%s\n",arg_v[1]);
  		return 0;
  }
  if (g_iPosSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iPosSrvFoldId = fold_locate_folder(DCS_POSSRV_FOLD_NAME);
    		if (g_iPosSrvFoldId < 0)
    		{
      			fprintf(stderr,"PosSrv not active!\n");
      			return 0;
    		}
  	}
  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
   	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_DEL;
   
  	if (fold_write(g_iPosSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to PosSrv failed,errno=%d\n",errno);

  	return 0;
}

int AddLink (int arg_c, char *arg_v[])
{
  		struct trmcmd Cmd;
  
   if ( arg_c <2)
  {
  	 fprintf(stderr,"Input Invalid parameter!\n");
  	 return 0;
  }
 
 if (g_iPosSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iPosSrvFoldId = fold_locate_folder(DCS_POSSRV_FOLD_NAME);
    		if (g_iPosSrvFoldId < 0)
    		{
      			fprintf(stderr,"PosSrv not active!\n");
      			return 0;
    		}
  	}
  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
   	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_ADD;
   
  	if (fold_write(g_iPosSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to PosSrv failed,errno=%d\n",errno);

  	return 0;
}
