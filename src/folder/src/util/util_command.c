#include <util_func.h>

//
//internal structure no necessary exported to outer
//
typedef struct cmdEntry
{
    char*cmd;
    short     nid;
    char*prompt;
    cmdfunc_t *pfnFunc;

    struct cmdEntry  *cmdlist_next;
} CMDENTRY, *LPCMDENTRY;

typedef struct _CommandObj
{
    short    ncommands;
    CMDENTRY *cmdlist_hdr;
    CMDENTRY *cmdlist_til;
}  COMMANDOBJ;

static COMMANDOBJ  gl_cmdserver = {0};
static char        gs_prompt[128];
static char        gs_cmdline[512];
static char        gs_prev_cmd[512];

void impl_signal_catcher(int signo);
int  impl_help(int argc, char *argv[]);
int  impl_exit(int argc, char *argv[]);
int  impl_null(int argc, char *argv[]){}

////////////////////////////////////////////////////////////////////

int cmd_init(const char *prompt, sigfunc_t *pFn)
{
   if(!prompt) 
       return -1;
   if(gl_cmdserver.ncommands > 0)
       //already init()
       return -1;
   
   strcpy(gs_prompt, prompt);
   //add all redefined commands    
   memset(&gl_cmdserver, 0, sizeof(gl_cmdserver));          
   cmd_add("help", impl_help, "help [all | command]");
   cmd_add("exit", impl_exit, "exit command processor");
   cmd_add("q",    impl_exit, "exit command processor");
   cmd_add(".",    impl_null, "repeat last command");
   cmd_add("!shell", impl_null, "execute shell command");
      
   //install the signal handler
   if(pFn == NULL)
       pFn = impl_signal_catcher;
   catch_all_signals(pFn);
   return 0;
}

////////////////////////////////////////////////////////////////////

int cmd_add(char *command, cmdfunc_t *pfnHandler, char *prompt)
{
    CMDENTRY *pEntry = NULL;
        
    if(!command || !pfnHandler)
        return -1;
        
    pEntry = (CMDENTRY *)malloc(sizeof(CMDENTRY));        
    if(!pEntry)
        return -1;
    pEntry->nid    = gl_cmdserver.ncommands ++;
    pEntry->cmd    = (char *)strdup(command);
    pEntry->prompt = prompt ? (char *)strdup(prompt) : NULL;
    if(!pEntry->cmd || (!pEntry->prompt && prompt))
    {
        free(pEntry->cmd);
        free(pEntry->prompt);
        return -1;
    }
    pEntry->cmdlist_next = NULL;    
    pEntry->pfnFunc      = pfnHandler;
        
    //append this entry to list
    if(gl_cmdserver.cmdlist_hdr ==0)
    {
        gl_cmdserver.cmdlist_hdr =  pEntry;
        gl_cmdserver.cmdlist_til =  pEntry;
    }
    else
    {
       (gl_cmdserver.cmdlist_til)->cmdlist_next =  pEntry; 
       gl_cmdserver.cmdlist_til =  pEntry;   
    }
    
    return 0;
}

////////////////////////////////////////////////////////////////////

int cmd_shell(int argc, char *argv[])
{
    int   l_argc,s;
    char  *l_argv[256];
    
    if(argc > 1)
    {
        impl_exec_command(argc-1, argv+1);	
        return 0;	
    }
    s=0;
    //loop and process commands 
    for(;;)
    {
        l_argc = 0;
        impl_get_input(&l_argc, &l_argv);
        if(l_argc <= 0 || impl_exec_command(l_argc,l_argv)< 0 )
	{
	        printf("invalid command. use 'help'\n");
        	s++;
        	if( s >100)
        		break;
        }
        else s=0;
    }        
}

////////////////////////////////////////////////////////////////////

int impl_help(int argc, char *argv[])
{
    CMDENTRY *pEntry = NULL;
    int       i;
    
    pEntry = gl_cmdserver.cmdlist_hdr;
    
    if(argc <= 1)  	
    {
        for(i=0; pEntry != NULL;i++)
        {
            printf("%-10s",pEntry->cmd);
            if((i+1) % 6 == 0) 
                printf("\n");
            pEntry = pEntry->cmdlist_next;    
        }	
     
        if(i % 6) 
             printf("\n");
        return 0;
    }
   
    if(u_stricmp(argv[1],"all")==0)
    {
        for(; pEntry != NULL; pEntry = pEntry->cmdlist_next)
            printf("%s---%s\n",pEntry->cmd,
                               pEntry->prompt ? pEntry->prompt : "");
        return 0;
    }
   
    for(; pEntry != NULL; pEntry = pEntry->cmdlist_next)
    {
      if(0 != u_stricmp(argv[1],pEntry->cmd))
        continue;
         	
      printf("%s\n", pEntry->prompt ? pEntry->prompt : "");
      return 0;
    }
   
    printf("irrecognizable command '%s'\n",argv[1]);
    return 0;
}

////////////////////////////////////////////////////////////////////

int impl_exit(int argc, char *argv[])
{
    exit(0);
}

////////////////////////////////////////////////////////////////////

void impl_signal_catcher(int signo)
{
    printf("\n");
    exit(1);	
}

////////////////////////////////////////////////////////////////////

int impl_get_input(int *l_argc, char *l_argv[])
{
    char	*bp;
     
    for(;;)
    {
        printf("%s ",gs_prompt); 
        fflush(stdout);
      
        memset(gs_cmdline,0,sizeof(gs_cmdline));
        if(fgets(gs_cmdline, sizeof(gs_cmdline), stdin) == NULL)
        {
            *l_argc = 0;
	        return(-1);
	}    
      
        if(gs_cmdline[0] == '.' && gs_cmdline[1] == '\n') 
            strcpy(gs_cmdline,gs_prev_cmd);
        else if(gs_cmdline[0] != '\n')
            //save the previous command
            strcpy(gs_prev_cmd,gs_cmdline);
		
        bp = gs_cmdline;
        while(isspace(*bp))
	        ++bp;
        if(!*bp)  continue;
		
        *l_argc = impl_make_argv(bp, &l_argv);
        break;
    }//for
	
    return 0;	
}

////////////////////////////////////////////////////////////////////

int impl_make_argv(char *cmdline,char **l_argv[])
{
    char *p;
    int  i,rc;
  	
    while(isspace(*cmdline) && (*cmdline))
        cmdline ++ ;
    if(!(*cmdline))
        return 0;

    (*l_argv)[0] = cmdline;
    rc = 1;
  
    for(i=1,p = cmdline; *p ;)
    {
        while(!isspace(*p) && *p)
            p++;
        if(!*p)  return rc;
    
        *p = 0; p++;
        while(isspace(*p) && (*p))
        p++;
        if(!*p) return rc;
    
        rc ++;
        (*l_argv)[i] = p;
        i++;	
    }//for
  
    return rc;  
}

////////////////////////////////////////////////////////////////////

int impl_exec_command(int l_argc,char *l_argv[])
{
    CMDENTRY *pEntry = NULL;
    
    //see if this command is a unix shell instruct
    if(l_argv[0][0]=='!')
    {
        char *p, *q; int child, i;
        char cmd[512];

        //strip the leading '!'
        p = l_argv[0]; q = p+1;
        for(;;p++, q++)
        {
            *p = *q;
            if(*p == 0) break;
        }

        memset(cmd,0,sizeof(cmd));
        for(i=0; i<l_argc; i++)
            sprintf(cmd+strlen(cmd),"%s ", l_argv[i]);
        return system(cmd);
    }

    pEntry = gl_cmdserver.cmdlist_hdr;
    for(; pEntry != NULL; pEntry = pEntry->cmdlist_next)
    {
      if(0 != u_stricmp(l_argv[0],pEntry->cmd))
        continue;
         	
      return (*pEntry->pfnFunc)(l_argc,l_argv);
    }

    return -1;
}
