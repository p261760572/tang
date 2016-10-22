#define _MainProg

//filename:  dcmcmd.c
//           main()

#include  "dccglobl.h"
#include  "dccpgdef.h"
#include  "dccdcs.h"
#include  "tmcibtms.h"

static struct hstcom *g_pHstCtrl = NULL;
static int g_iHstSrvFoldId = -1;
static   void   DcmCmdExit(int signo);

//forward declaration
int ListHst  (int arg_c, char *arg_v[]);
int StopLink  (int arg_c, char *arg_v[]);
int StartLink (int arg_c, char *arg_v[]);
int AddLink   (int arg_c, char *arg_v[]);
int ModifyLink   (int arg_c, char *arg_v[]);
int DeleteLink   (int arg_c, char *arg_v[]);
int KillHst   (int arg_c, char *arg_v[]);

int dumpBcda  (int arg_c, char *arg_v[]);
int addBcda  (int arg_c, char *arg_v[]);
int updateBcda  (int arg_c, char *arg_v[]);
int delBcda  (int arg_c, char *arg_v[]);
int dumpTca   (int arg_c, char *arg_v[]);
int printIseStat  (int arg_c, char *arg_v[]);

int set (int arg_c, char *arg_v[]);
int opentrace(int arg_c,char *arg_v[]);
int closetrace(int arg_c,char *arg_v[]);

int IsDeBug()
{
	return 1;
}

int main(int argc, char *argv[])
{
	catch_all_signals(DcmCmdExit);

  	//attach to folder system
  	if (fold_initsys() < 0)
  	{
    		fprintf(stderr,"cannot attach to FOLDER system!\n");
    		exit(1);
  	}

  	//read all configuration
//  	ReadConfig();

  	cmd_init("dcmcmd>>",DcmCmdExit);
  	cmd_add("listhst", ListHst,   "listhst [linkName]--list status of link");
  	cmd_add("stop",    StopLink,  "stop linkName [no]--stop a link");
  	cmd_add("start",   StartLink, "star linkName [no]--start a link");
  	cmd_add("add",   AddLink, "Add linkName [no]--add a link");
  	cmd_add("update",   ModifyLink, "update linkName no --modify a link");
  	cmd_add("delete",   DeleteLink, "delete linkName no--modify a link");
  	cmd_add("killhst", KillHst, "killhst--kill HstSvr");

  	cmd_add("dumpbcda", dumpBcda, "dumpbcda--dump contents in BCDA");
  	cmd_add("addbcda", addBcda, "addBcda name --add a recode  BCDA");
  	cmd_add("updatebcda", updateBcda, "updateBcda name --update a recode in BCDA");
  	cmd_add("delbcda", delBcda, "delBcda name --delete a recode  in BCDA");
/*  	cmd_add("isestat",  printIseStat, "isestat--dump contents in SSA");
   	cmd_add("dumptca",  dumpTca,
  	"dumptca timer--view all active timer\n"
  	"dumptca pending--view all pending transactions\n"
  	"dumptca resp   --view all responsed transactions\n"
  	"dumptca timeout--view all timed out transactions\n"
  	"dumptca usage  --view usage of timing queue");
*/
  	cmd_add("opentrace",    opentrace,  "opentrace hostname--open  a host trace");
  	cmd_add("closetrace",    closetrace,  "closetrace hostname--close  a host trace");
/*
   	cmd_add("set",  set,
  	"set IseDebug 0/1--set ise log mode 0:run mode 1:test mode\n"
	"set SWTimeout times--set switch command timeout [sencond]\n"
	"set IBTimeout times--set financial transaction timeout [sencond]\n"
	"set IBOutTimeout times--set atm transfer out timeout [sencond]\n"
	"set IBInTimeout times--set atm transfer in timeout [sencond]\n");
 */ 	//loop and interpret all command lines
  	cmd_shell(argc,argv);
  	DcmCmdExit(0);
}//main()

static void DcmCmdExit(int signo)
{
  	int i;
  
  	//decrease references
  	fold_delete_folder(g_iHstSrvFoldId);
  	
  	exit(signo);
}

int ListHst(int arg_c, char *arg_v[])
{
  	int  i, find = -1, j, nLow, nUpper;
  	char *strDesc, *strRole, *strPeerIp;
  	int  intPort;
  	char strLocalAddr[32];

  	//if (g_pHstCtrl == NULL)		// Mary delete, 2001-7-13
  	{
    		//connect the shared memory HSTCOM,so we can dump its content
    		g_pHstCtrl = (struct hstcom *)shm_connect(DCS_HSTCOM_NAME, NULL);
    		if (g_pHstCtrl == NULL)
    		{
      			fprintf(stderr,"HstSvr not active!\n");
      			return 0;
    		}
  	}

  	if (arg_c >= 2)
  	{
  			//dump status of the sepecifed link(s)
  			find = -1;
  			
    		for (i=0; i <= g_pHstCtrl->iLinkCnt-1; i++)
    		{
      			if (!(g_pHstCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
        			continue;
      			if (u_stricmp(arg_v[1],g_pHstCtrl->linkArray[i].caFoldName)!=0)
        			continue;
        			
        			if (!(g_pHstCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      					continue;
               
               if ( find <0)
               {
               		fprintf(stderr,"Ord   FdName        commNO  AppFid   Role     Status  LocalAddr  RemoteAddr\n");
  								fprintf(stderr,"=============================================================================\n");
               }
               find =i;
				    		if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_PASSIVE)
				      			strRole = "passive";
				    		else if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_ACTIVE)
				      			strRole = "active ";
				      	else if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_SIMPLEXSVR)
				      			strRole ="simplexsvr";
				      	else if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_SIMPLEXCLI)
				      			strRole="simplexcli";
				
				    		switch (g_pHstCtrl->linkArray[i].iStatus)
				    		{
				      		case DCS_STAT_DISCONNECTED:
				        		strDesc="discon ";
				        		break;
				
				      		case DCS_STAT_CONNECTED:
				        		strDesc="connct ";
				        		break;
				
				      		case DCS_STAT_LISTENING:
				        		strDesc="listen ";
				        		break;
				
				      		case DCS_STAT_CALLING:
				        		strDesc="call   ";
				        		break;
				
				      		case DCS_STAT_STOPPED:
				        		strDesc="stpped ";
				       			break;
				     		default:
				        		strDesc="       ";
				        		break;
				    		}//switch
				
				    		if (g_pHstCtrl->linkArray[i].iStatus == DCS_STAT_CONNECTED)
				    		{
				      			if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_PASSIVE ||g_pHstCtrl->linkArray[i].iRole ==DCS_ROLE_SIMPLEXSVR )
				      			{
				        			struct in_addr in;
				        			in.s_addr = g_pHstCtrl->linkArray[i].iRemoteIp0;
				        			strPeerIp = inet_ntoa(in);
				        			intPort   = g_pHstCtrl->linkArray[i].sRemotePort0;
				      			}
				      			else
				      			{
				        			strPeerIp = g_pHstCtrl->linkArray[i].caRemoteIp;
				        			intPort   = g_pHstCtrl->linkArray[i].sRemotePort;
				      			}
				    		}
				
				    		if (1)
				    		{
				      			int pos;
				
				      			memset(strLocalAddr,0,sizeof(strLocalAddr));
				//      			sprintf(strLocalAddr,"%s:%d",g_pHstCtrl->linkArray[i].caLocalIp,
				      			sprintf(strLocalAddr,":%d",g_pHstCtrl->linkArray[i].sLocalPort);
				     			for (pos=7; pos>=0; pos--)
				        			if (strLocalAddr[pos] == 0)
				          				strLocalAddr[pos]=' ';
				      				else
				        				break;
				   
				    		}
				
				    		if (g_pHstCtrl->linkArray[i].iStatus == DCS_STAT_CONNECTED)
				      			fprintf(stderr,"[%.3d] %-14s %3.3s    %.3d %-10.10s %s%s %s:%d\n",
				          			j,
				          			g_pHstCtrl->linkArray[i].caFoldName,
				          			g_pHstCtrl->linkArray[i].commNO,
				          			g_pHstCtrl->linkArray[i].iApplFid,
				          			strRole,
				          			strDesc,
				          			strLocalAddr,
				          			strPeerIp,
				          			intPort);
				    		else
				      			fprintf(stderr,"[%.3d] %-14s %3.3s    %.3d %-10.10s %s%s\n",
				          			j,
				          			g_pHstCtrl->linkArray[i].caFoldName,
				          			g_pHstCtrl->linkArray[i].commNO,
				          			g_pHstCtrl->linkArray[i].iApplFid,
				          			strRole,
				          			strDesc,
				          			strLocalAddr);
				  	
  	
    		}
        shm_detach((char *)g_pHstCtrl);			// Mary add, 2001-7-13
    		if (find < 0)
    		{
      			fprintf(stderr,"Link '%s' not exists!\n",arg_v[1]);
      			return 0;
    		}
    		return 0;
  	}

  	if (find >=0)
    		nLow = nUpper = find;
  	else
  	{
    		nLow   = 0;
    		nUpper = g_pHstCtrl->iLinkCnt-1;
  	}

  	//dump status of the sepecifed link(s)
  	fprintf(stderr,"Ord   FdName        commNO  AppFid   Role     Status  LocalAddr  RemoteAddr\n");
  	fprintf(stderr,"=============================================================================\n");

  	for (j=1,i=nLow; i <=nUpper ; i++,j++)
  	{
    		if (!(g_pHstCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;

    		if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_PASSIVE)
      			strRole = "passive";
    		else if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_ACTIVE)
      			strRole = "active ";
      	else if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_SIMPLEXSVR)
      			strRole ="simplexsvr";
      	else if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_SIMPLEXCLI)
      			strRole="simplexcli";

    		switch (g_pHstCtrl->linkArray[i].iStatus)
    		{
      		case DCS_STAT_DISCONNECTED:
        		strDesc="discon ";
        		break;

      		case DCS_STAT_CONNECTED:
        		strDesc="connct ";
        		break;

      		case DCS_STAT_LISTENING:
        		strDesc="listen ";
        		break;

      		case DCS_STAT_CALLING:
        		strDesc="call   ";
        		break;

      		case DCS_STAT_STOPPED:
        		strDesc="stpped ";
       			break;
     		default:
        		strDesc="       ";
        		break;
    		}//switch

    		if (g_pHstCtrl->linkArray[i].iStatus == DCS_STAT_CONNECTED)
    		{
      			if (g_pHstCtrl->linkArray[i].iRole == DCS_ROLE_PASSIVE ||g_pHstCtrl->linkArray[i].iRole ==DCS_ROLE_SIMPLEXSVR )
      			{
        			struct in_addr in;
        			in.s_addr = g_pHstCtrl->linkArray[i].iRemoteIp0;
        			strPeerIp = inet_ntoa(in);
        			intPort   = g_pHstCtrl->linkArray[i].sRemotePort0;
      			}
      			else
      			{
        			strPeerIp = g_pHstCtrl->linkArray[i].caRemoteIp;
        			intPort   = g_pHstCtrl->linkArray[i].sRemotePort;
      			}
    		}

    		if (1)
    		{
      			int pos;

      			memset(strLocalAddr,0,sizeof(strLocalAddr));
//      			sprintf(strLocalAddr,"%s:%d",g_pHstCtrl->linkArray[i].caLocalIp,
      			sprintf(strLocalAddr,":%d",g_pHstCtrl->linkArray[i].sLocalPort);
     			for (pos=7; pos>=0; pos--)
        			if (strLocalAddr[pos] == 0)
          				strLocalAddr[pos]=' ';
      				else
        				break;
   
    		}

    		if (g_pHstCtrl->linkArray[i].iStatus == DCS_STAT_CONNECTED)
      			fprintf(stderr,"[%.3d] %-14s %3.3s    %.3d %-10.10s %s%s %s:%d\n",
          			j,
          			g_pHstCtrl->linkArray[i].caFoldName,
          			g_pHstCtrl->linkArray[i].commNO,
          			g_pHstCtrl->linkArray[i].iApplFid,
          			strRole,
          			strDesc,
          			strLocalAddr,
          			strPeerIp,
          			intPort);
    		else
      			fprintf(stderr,"[%.3d] %-14s %3.3s    %.3d %-10.10s %s%s\n",
          			j,
          			g_pHstCtrl->linkArray[i].caFoldName,
          			g_pHstCtrl->linkArray[i].commNO,
          			g_pHstCtrl->linkArray[i].iApplFid,
          			strRole,
          			strDesc,
          			strLocalAddr);
  	}//for
  	
  	shm_detach((char *)g_pHstCtrl);			// Mary add, 2001-7-13

  	return 0;
}

int StopLink  (int arg_c, char *arg_v[])
{
  	struct hstcmd Cmd;
  	int    i, find = -1;

	/* Mary add begin, 2001-6-19 */
	//if (g_pHstCtrl == NULL)		// Mary delete, 2001-7-13
  	{
    		//connect the shared memory HSTCOM,so we can dump its content
    		g_pHstCtrl = (struct hstcom *)shm_connect(DCS_HSTCOM_NAME, NULL);
    		if (g_pHstCtrl == NULL)
    		{
      			fprintf(stderr,"HstSvr not active!\n");
      			return 0;
    		}
  	}
	/* Mary add end, 2001-6-19 */
    memset(&Cmd,0,sizeof( struct hstcmd));
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder(DCS_HSTSRV_FOLD_NAME);
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"HstSrv not active!\n");
      			return 0;
    		}
  	}

  	if (arg_c < 2)
    		return -1;

  	for (i=0; i <= g_pHstCtrl->iLinkCnt-1; i++)
  	{
    		if (!(g_pHstCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (u_stricmp(arg_v[1],g_pHstCtrl->linkArray[i].caFoldName)!=0)
      			continue;
    		find = i;
    		break;
  	}

  	if (find < 0)
  	{
    		fprintf(stderr,"Link '%s' not exists!\n",arg_v[1]);
    		return 0;
  	}

  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
  	if (strlen(arg_v[1]) > 20)
   		arg_v[1][20]=0;
  	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_STOP;
    if ( arg_c >2)
            memcpy(Cmd.caMsgFlag,arg_v[2],3);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to HstSrv failed,errno=%d\n",errno);

	shm_detach((char *)g_pHstCtrl);			// Mary add, 2001-7-13

  	return 0;
}

int StartLink (int arg_c, char *arg_v[])
{
  	struct hstcmd Cmd;
  	int    i, find = -1;

	/* Mary add begin, 2001-6-19 */
	//if (g_pHstCtrl == NULL)		// Mary delete, 2001-7-13
  	{
    		//connect the shared memory HSTCOM,so we can dump its content
    		g_pHstCtrl = (struct hstcom *)shm_connect(DCS_HSTCOM_NAME, NULL);
    		if (g_pHstCtrl == NULL)
    		{
      			fprintf(stderr,"HstSvr not active!\n");
      			return 0;
    		}
  	}
	/* Mary add end, 2001-6-19 */
		memset(&Cmd,0,sizeof(struct hstcmd));
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder(DCS_HSTSRV_FOLD_NAME);
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"HstSrv not active!\n");
      			return 0;
    		}
  	}

  	if (arg_c < 2)
    		return -1;

  	for (i=0; i <= g_pHstCtrl->iLinkCnt-1; i++)
  	{
    		if (!(g_pHstCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (u_stricmp(arg_v[1],g_pHstCtrl->linkArray[i].caFoldName)!=0)
      			continue;
    		find = i;
    		break;
  	}

  	if (find < 0)
  	{
    		fprintf(stderr,"Link '%s' not exists!\n",arg_v[1]);
    		return 0;
  	}

  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
  	if (strlen(arg_v[1]) > 20)
    		arg_v[1][20]=0;
  	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_START;
 		if ( arg_c >2)
            memcpy(Cmd.caMsgFlag,arg_v[2],3);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to HstSrv failed,errno=%d\n",errno);

	shm_detach((char *)g_pHstCtrl);			// Mary add, 2001-7-13

  	return 0;
}

int ModifyLink (int arg_c, char *arg_v[])
{
  	struct hstcmd Cmd;
  	int    i, find = -1;

		g_pHstCtrl = (struct hstcom *)shm_connect(DCS_HSTCOM_NAME, NULL);
		if (g_pHstCtrl == NULL)
		{
  			fprintf(stderr,"HstSvr not active!\n");
  			return 0;
		}
		memset(&Cmd,0,sizeof(struct hstcmd));
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder(DCS_HSTSRV_FOLD_NAME);
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"HstSrv not active!\n");
      			return 0;
    		}
  	}

  	if (arg_c < 3)
    		return -1;

  	for (i=0; i <= g_pHstCtrl->iLinkCnt-1; i++)
  	{
    		if (!(g_pHstCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (u_stricmp(arg_v[1],g_pHstCtrl->linkArray[i].caFoldName)!=0)
      			continue;
    		find = i;
    		break;
  	}

  	if (find < 0)
  	{
    		fprintf(stderr,"Link '%s' not exists!\n",arg_v[1]);
    		return 0;
  	}

  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
  	if (strlen(arg_v[1]) > 20)
    		arg_v[1][20]=0;
  	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_MODIFY;
		if ( arg_c >2)
            memcpy(Cmd.caMsgFlag,arg_v[2],3);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to HstSrv failed,errno=%d\n",errno);

		shm_detach((char *)g_pHstCtrl);			// Mary add, 2001-7-13

  	return 0;
}

int DeleteLink (int arg_c, char *arg_v[])
{
  	struct hstcmd Cmd;
  	int    i, find = -1;

		g_pHstCtrl = (struct hstcom *)shm_connect(DCS_HSTCOM_NAME, NULL);
		if (g_pHstCtrl == NULL)
		{
  			fprintf(stderr,"HstSvr not active!\n");
  			return 0;
		}
		memset(&Cmd,0,sizeof(struct hstcmd));
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder(DCS_HSTSRV_FOLD_NAME);
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"HstSrv not active!\n");
      			return 0;
    		}
  	}

  	if (arg_c < 3)
    		return -1;

  	for (i=0; i <= g_pHstCtrl->iLinkCnt-1; i++)
  	{
    		if (!(g_pHstCtrl->linkArray[i].lFlags & DCS_FLAG_USED))
      			continue;
    		if (u_stricmp(arg_v[1],g_pHstCtrl->linkArray[i].caFoldName)!=0)
      			continue;
    		find = i;
    		break;
  	}

  	if (find < 0)
  	{
    		fprintf(stderr,"Link '%s' not exists!\n",arg_v[1]);
    		return 0;
  	}

  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
  	if (strlen(arg_v[1]) > 20)
    		arg_v[1][20]=0;
  	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_DEL;
		if ( arg_c >2)
            memcpy(Cmd.caMsgFlag,arg_v[2],3);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to HstSrv failed,errno=%d\n",errno);

		shm_detach((char *)g_pHstCtrl);			// Mary add, 2001-7-13

  	return 0;
}

int AddLink (int arg_c, char *arg_v[])
{
  	struct hstcmd Cmd;
  	int    i, find = -1;

		g_pHstCtrl = (struct hstcom *)shm_connect(DCS_HSTCOM_NAME, NULL);
		if (g_pHstCtrl == NULL)
		{
  			fprintf(stderr,"HstSvr not active!\n");
  			return 0;
		}
		memset(&Cmd,0,sizeof(struct hstcmd));
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder(DCS_HSTSRV_FOLD_NAME);
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"HstSrv not active!\n");
      			return 0;
    		}
  	}

  	if (arg_c < 2)
    		return -1;

  	for (i=0; i <= g_pHstCtrl->iLinkCnt-1; i++)
  	{
    		if ( g_pHstCtrl->linkArray[i].lFlags & DCS_FLAG_USED )
      			continue;
    		find = i;
    		break;
  	}

  	if (find < 0)
  	{
    		fprintf(stderr," not free slot add Link '%s'!\n",arg_v[1]);
    		return 0;
  	}

  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
  	if (strlen(arg_v[1]) > 20)
    		arg_v[1][20]=0;
  	sprintf(Cmd.caFoldName, "%s",arg_v[1]);
  	Cmd.iCommand = DCS_CMD_ADD;
		if ( arg_c >2)
            memcpy(Cmd.caMsgFlag,arg_v[2],3);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to HstSrv failed,errno=%d\n",errno);

		shm_detach((char *)g_pHstCtrl);			// Mary add, 2001-7-13

  	return 0;
}

int KillHst   (int arg_c, char *arg_v[])
{
  	struct hstcmd Cmd;
  	int    i, find = -1;

  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder(DCS_HSTSRV_FOLD_NAME);
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"HstSrv not active!\n");
      			return 0;
    		}
  	}

  	//prepare the command and send it to HstSrv
  	sprintf(Cmd.caMsgType,  "%.4d",DCS_COMMAND_LINE);
  	Cmd.iCommand = DCS_CMD_EXIT;

  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,sizeof(Cmd)) < sizeof(Cmd))
    		fprintf(stderr,"request to HstSrv failed:%d\n",(errno));

  	return 0;
}



int dumpBcda  (int arg_c, char *arg_v[])
{
    	DumpBcda();
    	return 0;
}
int addBcda  (int arg_c, char *arg_v[])
{
	    if ( arg_c <2)
	    {
	    	fprintf(stderr,"parameter error\n");
  			return 0;
  		}
    	AddBcda(arg_v[1]);
    	return 0;
}
int updateBcda  (int arg_c, char *arg_v[])
{
			if ( arg_c <2)
	    {
	    	fprintf(stderr,"parameter error\n");
  			return 0;
  		}
    	UpdateBcda(arg_v[1]);
    	return 0;
}
int delBcda  (int arg_c, char *arg_v[])
{
			if ( arg_c <2)
	    {
	    	fprintf(stderr,"parameter error\n");
  			return 0;
  		}
    	DelBcda(arg_v[1]);
    	return 0;
}
/*
int dumpTca  (int arg_c, char *arg_v[])
{
    	int  i;
    
    	if (arg_c == 1)
    	{
        	TcaDump("pending"); return 0;
    	}

    	if ( u_stricmp(arg_v[1], "pending") == 0 ||
        	u_stricmp(arg_v[1], "timer") == 0   ||
        	u_stricmp(arg_v[1], "resp") == 0    ||
        	u_stricmp(arg_v[1], "timeout") == 0 ||
        	u_stricmp(arg_v[1], "usage") == 0 )
    	{ TcaDump(arg_v[1]); return 0;}

    	return -1;
}

int printIseStat  (int arg_c, char *arg_v[])
{
    	dumpSSA();
    	return 0;
}

int set  (int arg_c, char *arg_v[])
{
	if(arg_c < 3)
		return -1;
	
	if( u_stricmp(arg_v[1],"IseDebug") == 0 ||
		u_stricmp(arg_v[1],"SWTimeout") == 0 ||
		u_stricmp(arg_v[1],"IBTimeout") == 0 ||
		u_stricmp(arg_v[1],"IBOutTimeout") == 0 ||
		u_stricmp(arg_v[1],"IBInTimeout") == 0)
		SetIse(arg_v[1],arg_v[2]);

	return 0;
	
}
*/
int opentrace(int arg_c,char *arg_v[])
{
	if(arg_c < 2)
		return -1;
	SetTraceFlag(arg_v[1],1);
	return 0;
}


int closetrace(int arg_c,char *arg_v[])
{
	if(arg_c < 2)
		return -1;
	SetTraceFlag(arg_v[1],0);
	return 0;
}

