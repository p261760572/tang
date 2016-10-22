#include "ibdcs.h"

//static int  gs_fd  = -1;
static char gs_ident[64] ="        \0";
static char gs_logfile[512] = {0,0,0,0};
static char gs_logfile_src[512] = {0,0,0,0};
static time_t gs_log_time;
#ifndef __G_ILOGFLAG__
#define __G_ILOGFLAG__
	int g_iLogFlag = 0;
#endif
static int _dcs_format(char *, int ,const char *, va_list);
static int _dcs_dump(char *, int , char *, int );

#define ISPRINT(c)  (isprint(c) && isascii(c))
#define ISCHN(c,c1)  ((unsigned char)c>0x80 && (unsigned char)c1>0x80)
//#define ISPRINT(c)  ( c >0x80 || (isprint(c) && isascii(c))
#define MAX_BAK_LOG_SIZE   1024*1024*30
int dcs_log_open(const char * logfile, char *ident)
{
int  gs_fd  = -1,n;
struct  tm *tm; 
    
    time(&gs_log_time);
    tm = localtime(&gs_log_time);
    
    if(ident && ident[0])
    {
        char *p = gs_ident;
        for(; p < gs_ident + sizeof(gs_ident) -1;p++,ident++)
        {
            *p = *ident;
            if(*p == 0) break;
        }
    }


    memset(gs_logfile, 0, sizeof(gs_logfile));   
    memcpy(gs_logfile,logfile,strlen(logfile));
    strcpy(gs_logfile_src,gs_logfile);
    sprintf(gs_logfile+strlen(gs_logfile),".%4d%02d%02d",tm->tm_year+1900,
            tm->tm_mon + 1,tm->tm_mday );
//    n=strlen(logfile)-4;
//    n +=sprintf(gs_logfile+n,"%d",getpid());
//    memcpy(gs_logfile+n,logfile+strlen(logfile)-4,4);

    if(gs_fd < 0)
        gs_fd = open(gs_logfile,O_WRONLY|O_APPEND|O_CREAT,0666);

    if(gs_fd >= 0)
    {
       
        close( gs_fd );
        gs_fd = -1;
        return 0;
    }
    
    return -1;
}
/*
void dcs_log_close()
{
    close(gs_fd); 
    gs_fd = -1;
    memset(gs_logfile, 0, sizeof(gs_logfile));   
}
*/
/*
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
*/
void dcs_log(void *ptrbytes, int nbytes,const char * message,...)
{
    char    buffer[1024 * 16],*ptr;
    va_list ap;
    int     max_size,iRc,iRetry,len,cnt,l_year,l_mon,l_day;
    long iFileSize;
    char    *pbytes = (char *)ptrbytes;
    char caLogBakFileName[512];
    int  gs_fd  = -1;
    struct  tm *tm1; time_t  t;
    va_start(ap, message);
//    memset(buffer, 0, sizeof(buffer));
//    buffer[0]=0x00;
    len=_dcs_format(buffer, sizeof(buffer)-4,message,ap);
    va_end(ap);

//    max_size = sizeof(buffer) - 4 - strlen(buffer);
    max_size = sizeof(buffer) - 4 - len;
    cnt=len;
    ptr      = buffer + cnt;

    if(pbytes && nbytes)
    {  
    	 len=_dcs_dump(ptr, max_size, pbytes,nbytes);
//       ptr      = buffer + strlen(buffer);
       cnt +=len;
       ptr      = buffer + cnt;
    }
    //write the logging message into file
   
    *ptr ='\n';
    *(++ptr)=0x00;
    cnt++;
    buffer[cnt]=0x00;
//    write(gs_fd, buffer, strlen(buffer));
    time(&t);
    tm1 = localtime(&t);
    l_year= tm1->tm_year;
    l_mon=tm1->tm_mon;
    l_day=tm1->tm_mday;
    tm1 = localtime(&gs_log_time);
    
//    fprintf(stderr," tm1=[%d] tm2=[%d]\n tm1=[%d] tm2=[%d]\ntm1=[%d] tm2=[%d]\n",
//        tm1->tm_year,l_year,tm1->tm_mon,l_mon,tm1->tm_mday,l_day);
    if( tm1->tm_year != l_year ||
        tm1->tm_mon  != l_mon  ||
        tm1->tm_mday != l_day )
    {    snprintf(gs_logfile+strlen(gs_logfile_src),sizeof(gs_logfile)-strlen(gs_logfile_src),
    	     ".%04d%02d%02d",l_year+1900,l_mon+1,l_day);
    	   gs_log_time=t;
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
      usleep(10);
      if ( (iRetry ++) > 40 ) {
        printf("dcs_log.c: fatal error occured !!! error_no is %d\n",errno);
        printf("dcs_log:CANNOT LOCK ERROR_LOG %s !!!\n",gs_logfile);
        close(gs_fd);
        return;
      }
    }
    write(gs_fd, buffer, cnt);
    close( gs_fd );
    gs_fd = -1;
    if ( iFileSize+cnt >= MAX_BAK_LOG_SIZE ) 
    {
//    		  memset(caLogBakFileName, 0, 180);
//      		strcpy(caLogBakFileName, gs_logfile);
//      		strcat(caLogBakFileName,(char *) ".bak");
          time(&t);
          tm1 = localtime(&t);
          snprintf(caLogBakFileName,sizeof(caLogBakFileName),"%s.%02d%02d",
             gs_logfile,tm1->tm_hour,tm1->tm_min);					
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
    char    buffer[1024 * 16],*ptr;
    va_list ap;
    int     max_size,iRc,iRetry,len,cnt,l_year,l_mon,l_day;
    long iFileSize;
    char    *pbytes = (char *)ptrbytes;
    char caLogBakFileName[512];
    int  gs_fd  = -1;
    struct  tm *tm1; time_t  t;
//    fprintf(stderr,"g_iLogFlag=%d\n",g_iLogFlag);
    if(g_pIbdcsCfg == NULL ) //debug mode
    {
//    	  fprintf(stderr,"g_pIbdcsCfg is null\n");
    	  if ( !g_iLogFlag == 1 )  return ;
    }   
    else
    {  //product mode
//     		fprintf(stderr,"g_pIbdcsCfg is not null\n");
     		if( g_pIbdcsCfg->iRunMode ) return;
     		
    }
        

    va_start(ap, message);
//    memset(buffer, 0, sizeof(buffer));
//    buffer[0]=0x00;
    len=_dcs_format(buffer, sizeof(buffer)-4,message,ap);
    va_end(ap);
    cnt=len;
//    max_size = sizeof(buffer) - 4 - strlen(buffer);
    max_size = sizeof(buffer) - 4 - cnt;
    ptr      = buffer + cnt;

    if(pbytes && nbytes)
    {   
    	  len=_dcs_dump(ptr, max_size, pbytes,nbytes);
//    		ptr      = buffer + strlen(buffer);
    		cnt +=len;
    		ptr      = buffer + cnt;
    		
    }	

    //write the logging message into file
    
    *ptr ='\n';
    *(++ptr)=0x00;
    cnt++;
    buffer[cnt]=0x00;
//    write(gs_fd, buffer, strlen(buffer));
    time(&t);
    tm1 = localtime(&t);
    l_year= tm1->tm_year;
    l_mon=tm1->tm_mon;
    l_day=tm1->tm_mday;
    tm1 = localtime(&gs_log_time);
    if( tm1->tm_year != l_year ||
        tm1->tm_mon  != l_mon  ||
        tm1->tm_mday != l_day )
    {    snprintf(gs_logfile+strlen(gs_logfile_src),sizeof(gs_logfile)-strlen(gs_logfile_src),
    	     ".%04d%02d%02d",l_year+1900,l_mon+1,l_day);
    	   gs_log_time=t;
    }
    gs_fd = open( gs_logfile,O_WRONLY|O_APPEND|O_CREAT,0666);
//    fprintf(stderr,"gs_fd=%d\n",gs_fd);
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
      usleep(10);
      if ( (iRetry ++) > 40 ) {
        printf("dcs_log.c: fatal error occured !!! error_no is %d\n",errno);
        printf("dcs_log:CANNOT LOCK ERROR_LOG %s !!!\n",gs_logfile);
        close(gs_fd);
        return;
      }
    }
    write(gs_fd, buffer, cnt);
    close( gs_fd );
    gs_fd = -1;
    if ( iFileSize+cnt >= MAX_BAK_LOG_SIZE ) 
    {
//    		  memset(caLogBakFileName, 0, 180);
          time(&t);
          tm1 = localtime(&t);
      		snprintf(caLogBakFileName,sizeof(caLogBakFileName),"%s.%02d%02d",
      		        gs_logfile,tm1->tm_hour,tm1->tm_min);
//      		strcat(caLogBakFileName,(char *) ".bak");
					
    		  if ( ( iRc = rename(gs_logfile, caLogBakFileName) ) < 0 ) 
    		  {
        			printf("dcs_log.c: fatal error occured !!! iRc= %d error_no is %d\n" ,iRc,errno);
        			printf("dcs_log: %s CANNOT BE RENAMED !!!\n",gs_logfile);
      		}
    }
    return;
}


int _dcs_format(char *ptr, int max_size,const char *message, va_list ap)
{
    struct  tm *tm;   time_t  t;
    int len,cnt;
    //format the message
    time(&t);
    tm = localtime(&t);
    cnt=max_size;
    len=snprintf(ptr,max_size,"%4d/%02d/%02d %02d:%02d:%02d %s(%.6d) \n",
            tm->tm_year+1900,
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec,
            gs_ident,
            getpid());
    
    max_size -= len;
    ptr      += len;
    //print the true logging message
    len=vsnprintf(ptr, max_size,message, ap);

    ptr += len; max_size -= len;
    if( *(ptr - 1) != '\n')
    {  *ptr = '\n'; max_size--;}
//    ptr++;
//    *ptr=0x00;
    return cnt -max_size;
}

int _dcs_dump(char *ptr, int max_size, char *pbytes, int nbytes)
{
    int  r,i,len,cnt;
    unsigned char chr;
 
    int round = nbytes / 16;
    cnt=max_size;
    for(r=0; max_size > 7 && pbytes && r < round; r++)
    {
        //dump a line
        len=snprintf(ptr,max_size, "%.4Xh: ", r * 16);
        max_size -= len; ptr += len;

        for(i=0; max_size > 3 && i < 16; i++)
        {
            chr = pbytes[r * 16 + i];
            len=snprintf(ptr, max_size, "%.2X ", chr & 0xFF);
            max_size -= len; ptr += len;
        }

        if(max_size < 2) break;
        len=snprintf(ptr, max_size, "%s", "; "); ptr += len; max_size -= len;

        for(i=0; max_size > 1 && i < 16; i++)
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
        len=snprintf(ptr, max_size, "%.4Xh: ", r * 16);
        max_size -= len; ptr += len;

        for(i=0; max_size > 3 && i < nbytes % 16; i++)
        {
            chr = pbytes[r * 16 + i];
            len=snprintf(ptr, max_size, "%.2X ", chr & 0xFF);
            max_size -= len; ptr += len;
        }

        for(chr=' '; max_size > 3 && i < 16; i++)
        {
            len=snprintf(ptr,max_size, "%c%c ", chr & 0xFF, chr & 0xFF);
            max_size -= len; ptr += len;
        }

        if(max_size < 2) goto lblReturn;

        len=snprintf(ptr,max_size,"%s", "; ");
        ptr += len; max_size -= len;

        for(i=0; max_size > 1 && i < (nbytes % 16); i++)
        {
            chr = pbytes[r * 16 + i] & 0xFF;
            *ptr = ISPRINT(chr) ? chr : '.';
            ptr ++;  max_size --;
        }
    }//last line

lblReturn:
    *ptr = '\n';
    ptr ++;  max_size --;
    *ptr=0x00;
    return cnt-max_size;
}

