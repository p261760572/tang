#include <folder.h>
#include <pwd.h>

///////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    char tmpName[L_tmpnam], cmdLine[256], logname[32];
    int  fd;

#if 0
    if(argc < 2)    
    {
        fprintf(stderr,"Usage: %s username.\n",argv[0]);
        exit(1);    
    }
#endif
    
    GetUserName(logname);

    //dump all ipc objects into a temp file
    memset(tmpName, 0, sizeof(tmpName));
    tmpnam(tmpName); 
    
    memset(cmdLine, 0, sizeof(cmdLine));
    sprintf(cmdLine, "ipcs | grep  %s > %s", logname, tmpName);
    
    system(cmdLine);   
    
    //analyse file
    fd = conf_open(tmpName);
    if(fd < 0)
    {
        fprintf(stderr,"operation on temp file failed.\n");
        exit(1);    
    }
    
    print_msgq(fd);
    print_shm(fd);    
    print_sem(fd);    
    
    conf_close(fd);
    unlink(tmpName);
    exit(0);
}

int print_msgq(int fd)
{
    char caBuf[512], name[16];
    char *pID, *pKey, *pMod, *p;
    long key;
    int  ret, nmsgbytes, nmsgcount, nID;
        
    fprintf(stdout, "[ message queues ]\n");     
           fprintf(stdout, "%s  %-6s %-10s %s            %4s  %4s\n", 
                            " ","ID  " ,"Name", "Mode","Msg#","Bytes#");     
    fprintf(stdout, "=================================================\n");
                          
   //shared memory
   for(ret = conf_get_first_string(fd, "q",caBuf);
       ret == 0;
       ret = conf_get_next_string(fd, "q",caBuf))
   {
       pID   = strtok(caBuf, " ");   nID = atoi(pID);
       pKey  = strtok(NULL,  " ");   
       pMod  = strtok(NULL,  " ");            
         
       memset(name, 0, sizeof(name));
       key = strtol(pKey, NULL, 16);
         
       ipc_namefromkey(key,name);
       for(p = name; *p; p++)
       {
            if(isprint(*p)) continue;
            strcpy(name, pKey+2);break;
       } 

       queue_get_info(nID, &nmsgcount, &nmsgbytes);

       fprintf(stdout, "%s  %-6s %-10s %s  %4d  %4d\n", 
                        "q", pID, name, pMod, nmsgcount,nmsgbytes);           
    }//for     
    
    fprintf(stdout,"\n");
}

int print_shm(int fd)
{
    char caBuf[512], name[16];
    char *pID, *pKey, *pMod, *p;
    long key;
    int  ret, nshmsize, nID;
        
    fprintf(stdout, "[ shared memory ]\n");     
           fprintf(stdout, "%s  %-6s %-10s %s            %4s\n", 
                            " ","ID  " ,"Name", "Mode","Size");     
    fprintf(stdout, "=================================================\n");
                          
   //shared memory
   for(ret = conf_get_first_string(fd, "m",caBuf);
       ret == 0;
       ret = conf_get_next_string(fd, "m",caBuf))
   {
       pID   = strtok(caBuf, " ");   nID = atoi(pID);
       pKey  = strtok(NULL,  " ");   
       pMod  = strtok(NULL,  " ");            
         
       memset(name, 0, sizeof(name));
       key = strtol(pKey, NULL, 16);
         
       ipc_namefromkey(key,name);
       for(p = name; *p; p++)
       {
            if(isprint(*p)) continue;
            strcpy(name, pKey+2);break;
       } 

       shm_get_info(nID, &nshmsize);

       fprintf(stdout, "%s  %-6s %-10s %s     %-d\n", 
                        "m", pID, name, pMod, nshmsize);           
    }//for     
    
    fprintf(stdout,"\n");    
}

int print_sem(int fd)
{
    char caBuf[512], name[16];
    char *pID, *pKey, *pMod, *p;
    long key;
    int  ret, nsems, nID;
        
    fprintf(stdout, "[ semaphores ]\n");     
           fprintf(stdout, "%s  %-6s %-10s %s            %4s\n", 
                            " ","ID  " ,"Name", "Mode","Sems");     
    fprintf(stdout, "=================================================\n");
                          
   //shared memory
   for(ret = conf_get_first_string(fd, "s",caBuf);
       ret == 0;
       ret = conf_get_next_string(fd, "s",caBuf))
   {
       pID   = strtok(caBuf, " ");   nID = atoi(pID);
       pKey  = strtok(NULL,  " ");   
       pMod  = strtok(NULL,  " ");            
         
       memset(name, 0, sizeof(name));
       key = strtol(pKey, NULL, 16);
         
       ipc_namefromkey(key,name);
       for(p = name; *p; p++)
       {
            if(isprint(*p)) continue;
            strcpy(name, pKey+2);break;
       } 

       sem_get_info(nID, &nsems);

       fprintf(stdout, "%s  %-6s %-10s %s     %-d\n", 
                        "s", pID, name, pMod, nsems);           
    }//for     
    
    fprintf(stdout,"\n");    
}

int GetUserName(char *strUsrName)
{
    int uid;
    struct passwd *pwd_ptr;

    uid = getuid();

    pwd_ptr = getpwuid(uid);
    if(pwd_ptr == NULL)
    {
        cuserid(strUsrName);
        return 0;
    }

    strcpy(strUsrName,pwd_ptr->pw_name);
    return 0;
}
