#define _MainProg

//filename:  dcmcmd.c
//           main()

#include  "dccglobl.h"
#include  "dccpgdef.h"
#include  "dccdcs.h"
#include  "secuSrv.h"

static struct SECSRVCFG *g_pHstCtrl = NULL;
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

  	cmd_init("seccmd>>",DcmCmdExit);
  	cmd_add("list", ListHst,   "list [linkName]--list status of link");
  	cmd_add("stop",    StopLink,  "stop linkName [no]--stop a link");
  	cmd_add("start",   StartLink, "star linkName [no]--start a link");
  	cmd_add("add",   AddLink, "Add linkName [no]--add a link");
  	cmd_add("update",   ModifyLink, "update linkName no --modify a link");
  	cmd_add("delete",   DeleteLink, "delete linkName no--modify a link");
//  	cmd_add("killhst", KillHst, "killhst--kill HstSvr");

  	cmd_shell(argc,argv);
  	DcmCmdExit(0);
}//main()

static void DcmCmdExit(int signo)
{
  	int i;
  
  	//decrease references
//  	fold_delete_folder(g_iHstSrvFoldId);
  	
  	exit(signo);
}

int ListHst(int arg_c, char *arg_v[])
{
  	int  i, find = -1, j, nLow, nUpper;
  	char *strDesc, *strRole, *strPeerIp;
  	int  intPort;
  	char strLocalAddr[32];

  	
    		//connect the shared memory HSTCOM,so we can dump its content
		g_pHstCtrl = (struct SECSRVCFG *)shm_connect("TCSECSRV", NULL);
		if (g_pHstCtrl == NULL)
		{
  			fprintf(stderr,"HstSvr not active!\n");
  			return 0;
		}
  	
	
			//dump status of the sepecifed link(s)
			find = -1;
			
  		for (i=0; i <= g_pHstCtrl->maxLink-1; i++)
  		{
    			if (!(g_pHstCtrl->secLink[i].lFlags & DCS_FLAG_USED))
      			continue;
    		 			
      			
             if ( find <0)
             {
             		fprintf(stderr,"序号 类型   链路状态  工作状态 加密机IP：PORT         超时时间\n");
								fprintf(stderr,"===============================================================\n");
             }
             find =i;
			    						
			    		switch (g_pHstCtrl->secLink[i].iStatus)
			    		{
			      		case DCS_STAT_DISCONNECTED:
			        		strDesc="discon ";
			        		break;
			
			      		case DCS_STAT_CONNECTED:
			        		strDesc="conncted";
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
			
			    	if (g_pHstCtrl->secLink[i].iStatus == DCS_STAT_CONNECTED)
			      			fprintf(stderr,"[%.3d] %3.3s    %8.8s    %d     %s:%d    %d\n",
			          			g_pHstCtrl->secLink[i].iNum,
			          			g_pHstCtrl->secLink[i].cEncType,
			          			strDesc,
			          			g_pHstCtrl->secLink[i].iWorkFlag,
			          			g_pHstCtrl->secLink[i].caRemoteIp,
			          			g_pHstCtrl->secLink[i].iRemotePort,
			          			g_pHstCtrl->secLink[i].iTimeOut);
			    	else
			      			fprintf(stderr,"[%.3d] %3.3s    %8.8s    %d                      %d\n",
			          			g_pHstCtrl->secLink[i].iNum,
			          			g_pHstCtrl->secLink[i].cEncType,
			          			strDesc,
			          			g_pHstCtrl->secLink[i].iWorkFlag,
			          			g_pHstCtrl->secLink[i].iTimeOut);
			  	
	
  		}
      shm_detach((char *)g_pHstCtrl);
  		if (find < 0)
  		{
    			fprintf(stderr,"Link '%s' not exists!\n",arg_v[1]);
    			return 0;
  		}
  		return 0;
}

int StopLink  (int arg_c, char *arg_v[])
{
  	char Cmd[20];
  	int    i, find = -1;

 	
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder("SECSRV");
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"SecSrv not active!\n");
      			return 0;
    		}
  	}
  	sprintf(Cmd,"STOP%2.2s",arg_v[1]);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,6) < 6)
    		fprintf(stderr,"request to SecSrv failed,errno=%d\n",errno);
  	return 0;
}

int StartLink (int arg_c, char *arg_v[])
{
  	char Cmd[20];
  
	
		memset(&Cmd,0,sizeof(Cmd));
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder("SECSRV");
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"SecSrv not active!\n");
      			return 0;
    		}
  	}
	  sprintf(Cmd,"STAR%2.2s",arg_v[1]);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,6) < 6)
    		fprintf(stderr,"request to SecSrv failed,errno=%d\n",errno);
  


  	return 0;
}

int ModifyLink (int arg_c, char *arg_v[])
{
  		char Cmd[20];
  	int    i, find = -1;

	memset(&Cmd,0,sizeof(Cmd));
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder("SECSRV");
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"SecSrv not active!\n");
      			return 0;
    		}
  	}
	sprintf(Cmd,"MODI%2.2s",arg_v[1]);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,6) < 6)
    		fprintf(stderr,"request to SecSrv failed,errno=%d\n",errno);
  


  
  	return 0;
}

int DeleteLink (int arg_c, char *arg_v[])
{
	char Cmd[20];
  	int    i, find = -1;

	memset(&Cmd,0,sizeof(Cmd));
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder("SECSRV");
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"SecSrv not active!\n");
      			return 0;
    		}
  	}
	sprintf(Cmd,"DELE%2.2s",arg_v[1]);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,6) < 6)
    		fprintf(stderr,"request to SecSrv failed,errno=%d\n",errno);
  


  	return 0;
}

int AddLink (int arg_c, char *arg_v[])
{
	char Cmd[20];
  	int    i, find = -1;
	
	memset(&Cmd,0,sizeof(Cmd));
  	if (g_iHstSrvFoldId < 0)
  	{
    		//loate the folder of HstSrv
    		g_iHstSrvFoldId = fold_locate_folder("SECSRV");
    		if (g_iHstSrvFoldId < 0)
    		{
      			fprintf(stderr,"SecSrv not active!\n");
      			return 0;
    		}
  	}
	 sprintf(Cmd,"ADDE%2.2s",arg_v[1]);
  	if (fold_write(g_iHstSrvFoldId,-1,&Cmd,6) < 6)
    		fprintf(stderr,"request to SecSrv failed,errno=%d\n",errno);
  

  	return 0;
}



