#include <stdio.h>
#include <string.h>

#define  ISE_PROCS_CFGFILE   "isdsrver.conf"
#define  ISE_PROCS_CFGFILE2  "isdproc.conf"
#define  MAX_PROCS           128
#define SPACE                ' '

struct procinfo
{
    char  procName[64];   //name of process
    short procInstances;  //number configured  
    short procAlive;      //number of running process 
};

static  struct procinfo g_procTbl[MAX_PROCS];
static  int    g_iProcs = -1;
char *RPAD(char *src, char fillchar, int width);

///////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    char tmpName[L_tmpnam], cmdLine[256],  curUsrName[L_cuserid];
    FILE  *fp;
    
    memset(g_procTbl,    0, sizeof(g_procTbl));
    if( (g_iProcs = LoadConfiguredProcs()) < 0)
        exit(1);

    //get login name
    memset(curUsrName, 0, sizeof(curUsrName));
    cuserid(curUsrName);
    
    //dump all processes into a temp file
    memset(tmpName, 0, sizeof(tmpName));
    tmpnam(tmpName); 

    memset(cmdLine, 0, sizeof(cmdLine));
    sprintf(cmdLine, "ps -ef | grep  %s > %s", curUsrName, tmpName);
    system(cmdLine);
    
    //analyse file
    fp = fopen(tmpName, "r");
    if(fp == NULL)
    {
        fprintf(stderr,"operation on temp file failed.\n");
        //unlink(tmpName);
        exit(1);    
    }
    
    AnalyzeFile(fp);
    
    fclose(fp);
    unlink(tmpName);
    exit(0);
}

///////////////////////////////////////////////////////////////////

int LoadConfiguredProcs()
{  
    char  caFileName[256], *pcIseHome;
    FILE  *fp;
    char  caLine[256], *ptr;
    char  *pSubSys, *pProcName, *pMax, *pMin;
    int   i;

    //get full path name of the configuration file
    if(NULL == (pcIseHome = (char *)getenv("ISEHOME")))
    {
        fprintf(stderr, "cannot get environment variable 'ISEHOME'!\n");
        return -1;
    }
    else
    {  
        memset(caFileName, 0, sizeof(caFileName) );
        sprintf(caFileName, "%s/etc/tbl/%s",pcIseHome,ISE_PROCS_CFGFILE);
    }
  
    //open the file
    if( (fp = fopen(caFileName, "r+")) == NULL)
    {
        fprintf(stderr, "cannot open configure file '%s'!\n");
        return -1;
    }
    
    for(i=0; i <  MAX_PROCS;)
    {
        memset(caLine, 0, sizeof(caLine));
        if(NULL == fgets(caLine, sizeof(caLine), fp))
            break;    //EOF
        
        for(ptr = caLine; *ptr; ptr++)
            if( !isspace(*ptr) ) break;
        if(*ptr == 0)  //empty line
            continue;    
        if(*ptr == '#')  //comment line
            continue;    
        
        pSubSys  = strtok(caLine, " \t");
        pProcName= strtok(NULL,   " \t");            
        pMax     = strtok(NULL,   " \t");            
        pMin     = strtok(NULL,   " \t");                    
        
        if(pSubSys  && pProcName && pMax &&  pMin)
        {
            sprintf(g_procTbl[i].procName,"%s", pProcName);
            g_procTbl[i].procInstances = atoi(pMin);
            i++;
        }
    }

    fclose(fp);

    //open another configure file
    memset(caFileName, 0, sizeof(caFileName) );
    sprintf(caFileName, "%s/etc/tbl/%s",pcIseHome,ISE_PROCS_CFGFILE2);
    if( (fp = fopen(caFileName, "r+")) == NULL)
        return i;

    for(; i <  MAX_PROCS;)
    {
        memset(caLine, 0, sizeof(caLine));
        if(NULL == fgets(caLine, sizeof(caLine), fp))
            break;    //EOF
        
        for(ptr = caLine; *ptr; ptr++)
            if( !isspace(*ptr) ) break;
        if(*ptr == 0)  //empty line
            continue;    
        if(*ptr == '#')  //comment line
            continue;    
        
        pProcName= strtok(caLine,   " \t\n");            
        if( !pProcName) continue;

        sprintf(g_procTbl[i].procName,"%s", pProcName);
        g_procTbl[i].procInstances = 0;
        i++;
    }//for
 
    fclose(fp);
    return i;
}

///////////////////////////////////////////////////////////////////

int AnalyzeFile(FILE *fp)
{
    char  caLine[256];
    char  tmpName[L_tmpnam],tmpName1[L_tmpnam];
    char  *ptrField[16];
    FILE  *fpTmp, *fpTmp1;
    int   i, j;

    memset(tmpName, 0, sizeof(tmpName));
    tmpnam(tmpName); 
    fpTmp = fopen(tmpName, "w+");
    if(fpTmp == NULL)
    {
        fprintf(stderr, "operation failed on temp file!\n");
        return -1;    
    }
        
    //find out all configured proecess
    for(;;)
    {
        memset(caLine, 0, sizeof(caLine));
        if(NULL == fgets(caLine, sizeof(caLine), fp))
            break;    //EOF
            
        if(!IsConfiguredProc(caLine))    
            continue;

        ptrField[0] = strtok(caLine, " \t");    
        for(j=1; j < 9; j++)
            ptrField[j] = strtok(NULL, " \t");
        ptrField[9] = strtok(NULL, "\t\n");
            
        fprintf(fpTmp, "%s", RPAD(ptrField[9],SPACE, 20));  //command line
        fprintf(fpTmp, "%s", RPAD(ptrField[1],SPACE, 8));   //pid
        fprintf(fpTmp, "%s", RPAD(ptrField[2],SPACE, 8));   //ppid
        fprintf(fpTmp, "%s", RPAD(ptrField[6],SPACE, 12));  //start time
        fprintf(fpTmp, "%s", RPAD(ptrField[8],SPACE, 8));   //running elapse
        fprintf(fpTmp, "\n");
    }//for

    fclose(fpTmp);
    
    //print process info
    memset(tmpName1, 0, sizeof(tmpName1));
    tmpnam(tmpName1); 

    memset(caLine, 0, sizeof(caLine));                    
    sprintf(caLine, "sort  %s > %s", tmpName, tmpName1);
    system(caLine);
    unlink(tmpName);

    memset(tmpName, 0, sizeof(tmpName));
    tmpnam(tmpName); 
    fpTmp = fopen(tmpName, "w+");
    if(fpTmp == NULL)
    {
        fprintf(stderr, "operation failed on temp file!\n");
        return -1;
    }
    
    fpTmp1 = fopen(tmpName1, "r");
    if(fpTmp1 == NULL)
    {
        fprintf(stderr, "operation failed on temp file!\n");
        return -1;
    }

    for(i=0; ; i++)
    {
        memset(caLine, 0, sizeof(caLine));
        if(NULL == fgets(caLine, sizeof(caLine), fpTmp1))
            break;    //EOF
            
        fprintf(fpTmp, "[%..3d]  %s",i+1, caLine);
    }
    fclose(fpTmp1);
    fclose(fpTmp);
    unlink(tmpName1);

    printTitle();
    printLine();

    memset(caLine, 0, sizeof(caLine));                    
    if(i > 24)
         sprintf(caLine, "cat  %s | more", tmpName);
    else
         sprintf(caLine, "cat  %s", tmpName);
    system(caLine);
    unlink(tmpName);

    printLine();
    fprintf(stdout, "statistics:\n");
    printStat();

    return 0;
}

///////////////////////////////////////////////////////////////////

int IsConfiguredProc(char *psLine)
{
    int i;
    
    for(i=0; i <  g_iProcs; i++)
    {
        if(strstr(psLine, g_procTbl[i].procName))    
        {
            g_procTbl[i].procAlive ++;
            return 1;
        }
    }

    return 0;
}    

///////////////////////////////////////////////////////////////////

static char __fillBuf[128];

char *RPAD(char *src, char fillchar, int width)
{
    int len, n;
    char *p;

    memset(__fillBuf, 0, sizeof(__fillBuf));

    for(len=0; src[len]; len ++)
       __fillBuf[len] = src[len]; 
    if(len >= width)
        return __fillBuf;

    n = width - len;
    for(p = __fillBuf; *p; p++)
        ;
    for(; n >0; n --,p++)
        *p = fillchar;

    *p = 0;

    return __fillBuf;
}

///////////////////////////////////////////////////////////////////

int printTitle()
{
    //print the title
    fprintf(stdout, "       ");
    fprintf(stdout, "%s", RPAD("ProcessName",SPACE, 20));  //command line
    fprintf(stdout, "%s", RPAD("PID",        SPACE, 8));   //pid
    fprintf(stdout, "%s", RPAD("PPID",       SPACE, 8));   //ppid
    fprintf(stdout, "%s", RPAD("StartTime",  SPACE, 12));  //start time
    fprintf(stdout, "%s", RPAD("RunElapse",  SPACE, 8));   //running elapse
    fprintf(stdout, "\n"); 
}

int printLine()
{
    fprintf(stdout, "%s", RPAD("-",  '-', 62));
    fprintf(stdout, "\n"); 
    fflush(stdout);
}

int printStat()
{
    int i, delta;
    int isOkay = 1;

#if 0
    int sumRun = 0, sumCfg = 0;

    for(i=0; i < g_iProcs; i++)
    {
        sumCfg += g_procTbl[i].procInstances;
        sumRun += g_procTbl[i].procAlive;
    }

    fprintf(stdout, "%d processes predefined, %d processes running.\n",
                     sumCfg, sumRun);
#endif

    for(i=0; i < g_iProcs; i++)
    {
        delta =  g_procTbl[i].procInstances - g_procTbl[i].procAlive;
        if(delta > 0)
        {
            fprintf(stdout, "warning: %3d %s died.\n",
                             delta,
                             RPAD(g_procTbl[i].procName, SPACE, 10));
            isOkay = 0;
        }
    }

    if(isOkay)
        fprintf(stdout, "info: all processes running ok.\n");
    fflush(stdout);
}
