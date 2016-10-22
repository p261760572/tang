#include <util_func.h>


//----------------------------------------------------------------------

int  u_fabricatefile(const char *file_name,char *out_buffer,int nsize)
{
    // this function fabricate a full path file name
    // based on the environment variable 'ICS_HOME'
    // and parameter 'file_name'.
    // 
    // if the fabrication is ok,the return valaue is zero
    // and the resulted file name is placed in 'out_buffer';
    // otherwise a -1 will return and the content in 'out_buffer'
    // is undefined.
    
    char *ptr=NULL;
    char *save_ptr;
    
    ptr = getenv("ICS_HOME");
    if(ptr == NULL)
        return -1;
    
    if(out_buffer == NULL)
        return -1;

    //copy the path derived from environment variable    
    save_ptr = out_buffer;
    for(nsize >0; *ptr; nsize--)    
    {
        *out_buffer = *ptr;    
        out_buffer ++;
        ptr ++;
    }
    
    // insert a '/' if necessary
    if(nsize > 0 && (save_ptr < out_buffer && *(out_buffer-1) != '/'))
    {
        *out_buffer = '/';
        out_buffer ++;
        nsize --;    
    }
    
    //copy the file name
    for(nsize >0; file_name && *file_name; nsize--)    
    {
        *out_buffer = *file_name;    
        out_buffer ++;
        file_name ++;
    }
    
    // append NULL character
    if(nsize > 0)
        *out_buffer = 0;
        
    return 0;    
}

//----------------------------------------------------------------------

int  u_daemonize(const char *directory)
{
    // this function makes the caller into a 
    // deamon. if the daemonization successful,the return
    // value is 0, else the caller is terminated
    //    
    // parameters:
    // directory    the working directory of daemon
    
    int  i;
    
    if(0 != fork())
        exit(0);   //parent terminate
    
    //1st child continue        
    setpgrp();
    
    if(0 != fork())
        exit(0);   //1st child terminate
        
    //2nd child continue...
    
    if(directory)    
        chdir(directory);  //change working directory
    umask(0);    //clear file mode creation mask
    
    close(0); close(1); close(2);
    return 0;
}

//-----------------------------------------------------------

#define _isalpha(c)   (('a' <= (c) && (c) <= 'z') ||  \
                       ('A' <= (c) && (c) <= 'Z'))
#define LOWER_AZ(c)   (( 'a' <= (c) && (c) <= 'z') ? (c) : ((c)-'A'+'a'))
#define LOWER(c)      ( _isalpha(c) ? (LOWER_AZ(c)) : (c))

int   u_stricmp(char *s1,char *s2)
{
   for(; *s1 && *s2; s1++,s2++)
      if(LOWER(*s1) == LOWER(*s2)) 
         continue;
      else
         return LOWER(*s1)-LOWER(*s2);

   return LOWER(*s1)-LOWER(*s2);
}

//-----------------------------------------------------------

int u_sleep_us(int nusecs)
{
  struct timeval tval;

  tval.tv_sec  = nusecs / 1000000;
  tval.tv_usec = nusecs % 1000000;
  select(0,NULL,NULL,NULL,&tval);

  return 0;
}


char *LPAD(char *src, char fillchar, int width)
{
    int len, n;
    char *p;

    for(len=0; src[len]; len ++)
        ;

    if(len >= width)
        return src;

    n = width - len;
    memmove(src+n, src,len+1);

    for(p = src; n >0; n --,p++)
        *p = fillchar;

    return src;
}

char *RPAD(char *src, char fillchar, int width)
{
    int len, n;
    char *p;

    for(len=0; src[len]; len ++)
        ;
    if(len >= width)
        return src;

    n = width - len;
    for(p = src; *p; p++)
        ;
    for(; n >0; n --,p++)
        *p = fillchar;

    *p = 0;

    return src;
}

int u_filesize(char *filename)
{
    struct stat sbuf;

    if(stat(filename, &sbuf) < 0)
        return -1;

    return sbuf.st_size;
}

