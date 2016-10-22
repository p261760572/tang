#include <folder.h>

static char name[64];
static int  fid = -1;
static int  ndelay;

void eachoExit(int signo)
{
    printf("catch a signal %d and exit.\n",signo);
    //if(fid >= 0)
       // fold_delete_folder(fid);
    exit(1);
}

int main(int argc, char * argv[])
{
    if(0 > fold_initsys())
    {
        printf("cannot attach to folder system!\n");
        exit(1);    
    }

    catch_all_signals(eachoExit);

    if(argc < 3)
    {
        printf("usage: %s fname delay_ms\n",argv[0]);
        exit(0);
    }

    sprintf(name,"%s",argv[1]);
    ndelay = atoi(argv[2]) * 1000;

    //u_daemonize(NULL);
    do_reader();

    exit(0);
}//main

int do_reader()
{
   int         i, nread, nwrite;
   char        buf[256 + 1024];
   int         org;

   //create my channel
   fid = fold_create_folder(name);
   if(fid < 0)
       fid = fold_locate_folder(name);
   if(fid < 0)
   {
     printf("cannot create folder with name '%s':%s\n",
                  name,ise_strerror(errno));
     exit(1);
   }
   else
     printf("eacho fold id = %d\n",fid);

for(i=1;;i++)
{
   memset(buf,0,sizeof(buf));
   nread = fold_read(fid,&org,buf,sizeof(buf),TRUE);
   if(nread < 0)
   {
       printf("fold_read() error,no=%d\n",errno);
       break;
   }

   printf("[%.6d]R %.4dbytes-->%s\n",i,nread,buf);

   u_sleep_us(ndelay);

   //eacho what received
   nwrite = fold_write(org,-1,buf,nread);
}

}
