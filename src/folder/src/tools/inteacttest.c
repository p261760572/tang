#include <folder.h>

static char name[64];
static int  fid = -1;

void mytestExit(int signo)
{
    printf("catch a signal %d and exit.\n",signo);
    //if(fid >= 0)
       // fold_delete_folder(fid);
    exit(1);
}

int main(int argc, char * argv[])
{
    int  nchild;
    int  fid;

    if(0 > fold_initsys())
    {
        printf("cannot attach to folder system!\n");
        exit(1);    
    }

    catch_all_signals(mytestExit);

    if(argc < 4 || argv[1][0] != '-')
    {
        printf("usage: %s -r|-w fname nchild\n",argv[0]);
        exit(0);
    }

    sprintf(name,"%s",argv[2]);
    nchild = atoi(argv[3]);

    //u_daemonize(NULL);

    switch(argv[1][1]) 
    {
        case 'r':
        case 'R':
               for(;nchild>0;nchild--)
               {
                 //if(fork() ==0) 
                     do_reader();
               }
               break;

        case 'w':
        case 'W':
               for(;nchild>0;nchild--)
               {
                 //if(fork() ==0) 
                     do_writer();
               }
               break;

        default:
                printf("usage: %s -r|-w fname nchild\n",argv[0]);
                break;
    }//switch

    exit(0);
}//main

int do_reader()
{
   int         i, nread, tick;
   char        buf[256 + 1024];
   int         org;

   //create my channel
   fid = fold_create_folder(name);
   if(fid < 0)
       fid = fold_locate_folder(name);
   if(fid < 0)
   {
     printf("cannot create folder with name '%s',err=%d\n",name,errno);
     exit(1);
   }

   printf("any key to receive data...");
   fflush(stdin);getchar();

   for(i=1;;i++)
   {
       memset(buf,0,sizeof(buf));
       nread = fold_read(fid,&org,buf,sizeof(buf),TRUE);
       if(nread < 0)
       {
           printf("fold_read() error,no=%d\n",errno);
           break;
       }

       tick = fold_msg_tickcount();
       printf("[%.6d]R %.4dbytes tick=%6d-->%s\n",i,nread,tick,buf);
   }
}

do_writer()
{
   char  line[1024+128];
   int   nwritten,nbytes,i;
   char  buffer[1024+64];
   time_t   seed; char state[256];

   //locate the channel of reader
   fid = fold_locate_folder(name);
   if(fid < 0)
   {
     printf("cannot open folder of reader.\n");
     exit(1);
   }

   printf("any key to send data...");
   fflush(stdin);getchar();

   time(&seed);
   initstate(seed, state, 256); 

   for(i=1;;)
   {
        nbytes = random() % 1024;
        if(nbytes ==0) nbytes = 33;
        memset(line,i % 32 + '0',nbytes);
        line[56]=0;
       
        nwritten=fold_write(fid,-1,line,nbytes);
        if(nwritten < 0)
        {
           //printf("write to reader failure.errno=%d\n",GetLastError());
           if(errno == FOLD_ENOMBUF) //not enough memory
           {
              sleep(1);
              continue;
           }
           else
              break;
        }
        
        printf("[%.6d]W %.4dbytes-->%s\n",i,nwritten,line);
        sleep(1);
        i++;
   }
}
