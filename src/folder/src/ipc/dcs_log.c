#include "ibdcs.h"

static int  gs_fd  = -1;
static char gs_ident[64] ="        \0";
static char gs_logfile[256] = {0,0,0,0};
#ifndef __G_ILOGFLAG__
#define __G_ILOGFLAG__
	int g_iLogFlag = 0;
#endif
static void _dcs_format(char *, int ,const char *, va_list);
static void _dcs_dump(char *, int , char *, int );

#define ISPRINT(c)  (isprint(c) && isascii(c))
#define ISCHN(c,c1)  ((unsigned char)c>0x80 && (unsigned char)c1>0x80)
//#define ISPRINT(c)  ( c >0x80 || (isprint(c) && isascii(c))
#define MAX_BAK_LOG_SIZE   1024*1024*30
int dcs_log_open(const char * logfile, char *ident)
{
    if(ident && ident[0])
    {
        char *p = gs_ident;
        for(; p < gs_ident + sizeof(gs_ident) -1;p++,ident++)
        {
            *p = *ident;
            if(*p == 0) break;
        }
    }

    if(gs_fd < 0)
        gs_fd = open(logfile,O_WRONLY|O_APPEND|O_CREAT,0666);

    if(gs_fd >= 0)
    {
        memset(gs_logfile, 0, sizeof(gs_logfile));   
        strcpy(gs_logfile, logfile);
        close( gs_fd );
        gs_fd = -1;
        return 0;
    }
    
    return -1;
}

void dcs_log_close()
{
    close(gs_fd); 
    gs_fd = -1;
    memset(gs_logfile, 0, sizeof(gs_logfile));   
}

int dcs_set_logfd(int fd)
{
    if(fd < 0)
        return -1;

   if(gs_fd >= 0)
   {
        close(gs_fd);
        memset(gs_logfile, 0, sizeof(gs_logfile));   
   }

   gs_fd = fd;
   return 0;
}

void dcs_log(void *ptrbytes, int nbytes,const char * message,...)
{
    char    buffer[1024 * 8],*ptr;
    va_list ap;
    int     max_size,iRc,iRetry;
    long iFileSize;
    char    *pbytes = (char *)ptrbytes;
    char caLogBakFileName[180];
    gs_fd = open( gs_logfile,O_WRONLY|O_APPEND|O_CREAT,0666);
    if( gs_fd < 0 )
        return;
    if ((iFileSize = lseek(gs_fd,0,SEEK_END)) ==-1) {
      printf("dcs_log.c: fatal error occured !!! error_no is %d\n",errno);
      printf("dcs_log: %s CANNOT JUMP TO END OF FILE !!!\n",gs_logfile);
      close(gs_fd);
      return ;
    }
     iRetry=0;
    while (lockf(gs_fd,F_LOCK,0) == -1) {
      usleep(10000);
      if ( (iRetry ++) > 40 ) {
        printf("dcs_log.c: fatal error occured !!! error_no is %d\n",errno);
        printf("dcs_log:CANNOT LOCK ERROR_LOG %s !!!\n",gs_logfile);
        close(gs_fd);
        return;
      }
    }
    va_start(ap, message);
    memset(buffer, 0, sizeof(buffer));
    _dcs_format(buffer, sizeof(buffer)-2,message,ap);
    va_end(ap);

    max_size = sizeof(buffer) - 2 - strlen(buffer);
    ptr      = buffer + strlen(buffer);

    if(pbytes && nbytes)
        _dcs_dump(ptr, max_size, pbytes,nbytes);

    //write the logging message into file
    ptr      = buffer + strlen(buffer);
    *ptr ='\n';
    write(gs_fd, buffer, strlen(buffer));
    close( gs_fd );
    gs_fd = -1;
    if ( iFileSize >= MAX_BAK_LOG_SIZE ) 
    {
    		  memset(caLogBakFileName, 0, 180);
      		strcpy(caLogBakFileName, gs_logfile);
      		strcat(caLogBakFileName,(char *) ".bak");
					
    		  if ( ( iRc = rename(gs_logfile, caLogBakFileName) ) < 0 ) 
    		  {
        			printf("dcs_log.c: fatal error occured !!! iRc= %d error_no is %d\n" ,iRc,errno);
        			printf("dcs_log: %s CANNOT BE RENAMED !!!\n",gs_logfile);
      		}
    }
    return;
}

void dcs_debug(void *ptrbytes, int nbytes,const char * message,...)
{
    char    buffer[1024 * 8],*ptr;
    va_list ap;
    int     max_size,iRc,iRetry;
    long iFileSize;
    char    *pbytes = (char *)ptrbytes;
    char caLogBakFileName[180];

    if(g_pIbdcsCfg == NULL || g_pIbdcsCfg->iRunMode == 0) //debug mode
    {
    	  ;
    }   
    else
    {  //product mode
     		if ( g_iLogFlag == 1 )
    					 ;
    		else
    		     return ;
    }
        
    gs_fd = open( gs_logfile,O_WRONLY|O_APPEND|O_CREAT,0666);
    if( gs_fd < 0 )
        return;
		if ((iFileSize = lseek(gs_fd,0,SEEK_END)) ==-1) {
      printf("dcs_log.c: fatal error occured !!! error_no is %d\n",errno);
      printf("dcs_log: %s CANNOT JUMP TO END OF FILE !!!\n",gs_logfile);
      close(gs_fd);
      return ;
    }
     iRetry=0;
    while (lockf(gs_fd,F_LOCK,0) == -1) {
      usleep(10000);
      if ( (iRetry ++) > 40 ) {
        printf("dcs_log.c: fatal error occured !!! error_no is %d\n",errno);
        printf("dcs_log:CANNOT LOCK ERROR_LOG %s !!!\n",gs_logfile);
        close(gs_fd);
        return;
      }
    }
    va_start(ap, message);
    memset(buffer, 0, sizeof(buffer));
    _dcs_format(buffer, sizeof(buffer)-2,message,ap);
    va_end(ap);

    max_size = sizeof(buffer) - 2 - strlen(buffer);
    ptr      = buffer + strlen(buffer);

    if(pbytes && nbytes)
        _dcs_dump(ptr, max_size, pbytes,nbytes);

    //write the logging message into file
    ptr      = buffer + strlen(buffer);
    *ptr ='\n';
    write(gs_fd, buffer, strlen(buffer));
    close( gs_fd );
    gs_fd = -1;
    if ( iFileSize >= MAX_BAK_LOG_SIZE ) 
    {
    		  memset(caLogBakFileName, 0, 180);
      		strcpy(caLogBakFileName, gs_logfile);
      		strcat(caLogBakFileName,(char *) ".bak");
					
    		  if ( ( iRc = rename(gs_logfile, caLogBakFileName) ) < 0 ) 
    		  {
        			printf("dcs_log.c: fatal error occured !!! iRc= %d error_no is %d\n" ,iRc,errno);
        			printf("dcs_log: %s CANNOT BE RENAMED !!!\n",gs_logfile);
      		}
    }
    return;
}
void dcs_debug_disp(char cDumpDev, void *ptrbytes, int nbytes,const char * message,...)
{
    char    buffer[1024 * 8],*ptr;
    va_list ap;
    int     max_size,iRc,iRetry;
    long iFileSize;
    char    *pbytes = (char *)ptrbytes;
    char caLogBakFileName[180];

   if(g_pIbdcsCfg == NULL || g_pIbdcsCfg->iRunMode == 0) //debug mode
    {
    	  ;
    }   
    else
    {  //product mode
     		if ( g_iLogFlag == 1 )
    					 ;
    		else
    		     return ;
    }
    if( cDumpDev ==0 )    
   			 gs_fd = open(ttyname(1),O_WRONLY);
   	else
   			 gs_fd = open( "/dev/console",O_WRONLY);
    if( gs_fd < 0 )
        return;
	
    va_start(ap, message);
    memset(buffer, 0, sizeof(buffer));
    _dcs_format(buffer, sizeof(buffer)-2,message,ap);
    va_end(ap);

    max_size = sizeof(buffer) - 2 - strlen(buffer);
    ptr      = buffer + strlen(buffer);

    if(pbytes && nbytes)
        _dcs_dump(ptr, max_size, pbytes,nbytes);

    //write the logging message into file
    ptr      = buffer + strlen(buffer);
    *ptr ='\n';
    write(gs_fd, buffer, strlen(buffer));
    close( gs_fd );
    gs_fd = -1;
   
    return;
}
void dcs_log_disp(char cDumpDev, void *ptrbytes, int nbytes,const char * message,...)
{
    char    buffer[1024 * 8],*ptr;
    va_list ap;
    int     max_size,iRc,iRetry;
    long iFileSize;
    char    *pbytes = (char *)ptrbytes;
    char caLogBakFileName[180];

    if( cDumpDev ==0 )    
   			 gs_fd = open(ttyname(1),O_WRONLY);
   	else
   			 gs_fd = open( "/dev/console",O_WRONLY);
    if( gs_fd < 0 )
        return;
	
    va_start(ap, message);
    memset(buffer, 0, sizeof(buffer));
    _dcs_format(buffer, sizeof(buffer)-2,message,ap);
    va_end(ap);

    max_size = sizeof(buffer) - 2 - strlen(buffer);
    ptr      = buffer + strlen(buffer);

    if(pbytes && nbytes)
        _dcs_dump(ptr, max_size, pbytes,nbytes);

    //write the logging message into file
    ptr      = buffer + strlen(buffer);
    *ptr ='\n';
    write(gs_fd, buffer, strlen(buffer));
    close( gs_fd );
    gs_fd = -1;
   
    return;
}
void _dcs_format(char *ptr, int max_size,const char *message, va_list ap)
{
    struct  tm *tm;   time_t  t;

    //format the message
    time(&t);
    tm = localtime(&t);

    sprintf(ptr,"%4d/%02d/%02d %02d:%02d:%02d %s(%.6d) \n",
            tm->tm_year+1900,
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec,
            gs_ident,
            getpid());
    ptr      += strlen(ptr);
    max_size -= strlen(ptr);

    //print the true logging message
    vsnprintf(ptr, max_size,message, ap);

    ptr += strlen(ptr);
    if( *(ptr - 1) != '\n')
        *ptr = '\n';
}

void _dcs_dump(char *ptr, int max_size, char *pbytes, int nbytes)
{
    int  r,i;
    unsigned char chr;

    int round = nbytes / 16;

    for(r=0; max_size > 7 && pbytes && r < round; r++)
    {
        //dump a line
        sprintf(ptr, "%.4Xh: ", r * 16);
        max_size -= strlen(ptr); ptr += strlen(ptr);

        for(i=0; max_size > 0 && i < 16; i++)
        {
            chr = pbytes[r * 16 + i];
            sprintf(ptr, "%.2X ", chr & 0xFF);
            max_size -= strlen(ptr); ptr += strlen(ptr);
        }

        if(max_size < 2) break;
        sprintf(ptr, "%s", "; "); ptr += 2; max_size -= 2;

        for(i=0; max_size > 0 && i < 16; i++)
        {
            chr = pbytes[r * 16 + i] & 0xFF;
            *ptr = ISPRINT(chr) ? chr : '.';
            ptr ++; max_size --;
        }

        if(max_size < 2) goto lblReturn;

        *ptr = '\n'; ptr++; max_size --;
    }//for each round

    if(max_size > 7 && pbytes && (nbytes % 16) )
    {
        //dump the last line
        sprintf(ptr, "%.4Xh: ", r * 16);
        max_size -= strlen(ptr); ptr += strlen(ptr);

        for(i=0; max_size > 0 && i < nbytes % 16; i++)
        {
            chr = pbytes[r * 16 + i];
            sprintf(ptr, "%.2X ", chr & 0xFF);
            max_size -= strlen(ptr); ptr += strlen(ptr);
        }

        for(chr=' '; max_size > 0 && i < 16; i++)
        {
            sprintf(ptr, "%c%c ", chr & 0xFF, chr & 0xFF);
            max_size -= strlen(ptr); ptr += strlen(ptr);
        }

        if(max_size < 2) goto lblReturn;

        sprintf(ptr, "%s", "; ");
        ptr += 2; max_size -= 2;

        for(i=0; max_size > 0 && i < (nbytes % 16); i++)
        {
            chr = pbytes[r * 16 + i] & 0xFF;
            *ptr = ISPRINT(chr) ? chr : '.';
            ptr ++;  max_size --;
        }
    }//last line

lblReturn:
    ptr += strlen(ptr);
    *ptr = '\n';
}

