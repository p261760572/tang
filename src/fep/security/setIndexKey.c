#include  "dccdcs.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

static char mkey1[32]="S44458376811702B";
static char mkey2[32]="R98142232651462J";
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)
struct termios stored_settings;
int GenMKey(char *key1,char *key2,char *key3)
{
		char   tmpkey1[32],tmpkey2[32],buf[32],buf1[32];
  	int    iFd=-1,i;  
  	
  	
  	//�趨commlnk�����ļ��ľ���·��������
  	
  	if( key1==NULL || key2==NULL ||key3==NULL) return 0;

   	memset(tmpkey1, 0, sizeof(tmpkey1) );
  	memset(tmpkey2, 0, sizeof(tmpkey2) );
  	
  	for ( i =0 ;i<16;i++)
  	{
  		   tmpkey1[i]=mkey1[i]-i+(i%3);
  		   tmpkey2[i]=mkey2[i]-i+(i%7);
  		   tmpkey1[i]=((unsigned char)tmpkey1[i])^((unsigned char)tmpkey2[i]);
  		   tmpkey2[i]=((unsigned char)key1[i])^((unsigned char)key2[i]);
  	}
  	
  	
  	memcpy(buf,tmpkey2,8);
  	
    TripleDes(tmpkey1,buf,buf1 );
    memcpy(buf,tmpkey2+8,8);
    memcpy(key3,buf1,8);
    TripleDes(tmpkey1,buf,buf1 );
    memcpy(key3+8,buf1,8);
   	return 1;
}

int set_disp_mode(int fd,int option)
{
   int err;
   struct termios term;
   if(tcgetattr(fd,&term)==-1){
     perror("Cannot get the attribution of the terminal");
     return 1;
   }
   if(option)
       term.c_lflag|=ECHOFLAGS;
   else
       term.c_lflag &=~ECHOFLAGS;
   err=tcsetattr(fd,TCSAFLUSH,&term);
   if(err==-1 && err==EINTR){
       perror("Cannot set the attribution of the terminal");
       return 1;
   }
   return 0;
}
void set_keypress(void)                               //�����ն�ΪRAWģʽ�����رջ���

{
    struct termios new_settings;

    tcgetattr(0,&stored_settings);

    new_settings = stored_settings;

    /* Disable canonical mode, and set buffer size to 1 byte */
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= (~ECHO);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;

    tcsetattr(0,TCSANOW,&new_settings);
    return;
}

void reset_keypress(void)                                //�ָ��ն�����

{
    tcsetattr(0,TCSANOW,&stored_settings);
    return;
}

 
int getpasswd(char *disp,char* passwd, int size)
{
   int c;
   int n = 0;
  
   printf("\n%s:",disp);
  
   do{
//   	set_disp_mode(STDIN_FILENO,0);
      set_keypress();
      c=getchar();
      reset_keypress();
//    set_disp_mode(STDIN_FILENO,1);
      fprintf(stderr,"*");
      if (c != '\n'|c!='\r'){
         passwd[n++] = c;
      }
   }while(c != '\n' && c !='\r' && n < (size - 1));
   passwd[n] = '\0';
   return n;
}
int main()
{
   char passwd1[33],passwd2[33],buf[33];
   int n;
    
   while(1)
   {
   	 if ( n == 1) return;
	   n=getpasswd("\n������ɷ�һ",passwd1, 33);
	   if ( n <32) 
	   {
	   	  if ( n == 1) return;
	   	  printf("\n����ĳ��Ȳ���:%d", n);
	   	  continue;
	   }
	   n=getpasswd("\n��������һ�γɷ�һ",buf, 33);
	   if ( n <32) 
	   {
	   	  if ( n == 1) return;
	   	  printf("\n����ĳ��Ȳ���:%d", n);
	   	  continue;
	   }
	   if ( memcmp(passwd1,buf,32) !=0 )
	   {
	   	  printf("\n�ɷ�һ�������벻һ�£�����������!");
	   	  continue;
	   }
	   
	   n=0;
	   while(1)
	   {
		   if ( n == 1) return;
		   n=getpasswd("\n������ɷݶ�",passwd2, 33);
		   if ( n <32) 
		   {
		   	  if ( n == 1) return;
		   	  printf("\n����ĳ��Ȳ���:%d", n);
		   	  continue;
		   }
		   n=getpasswd("\n��������һ�γɷݶ�",buf, 33);
		   if ( n <32) 
		   {
		   	 if ( n == 1) return;
		   	  printf("\n����ĳ��Ȳ���:%d", n);
		   	  continue;
		   }
		   if ( memcmp(passwd2,buf,32) !=0 )
		   {
		   	  printf("\n�ɷݶ��������벻һ�£�����������!");
		   	  continue;
		   }
		   break;
	   }
	   break;
   }
   asc_to_bcd(passwd1,passwd1,32,0);
   asc_to_bcd(passwd2,passwd2,32,0);
   n=GenMKey(passwd1,passwd2,passwd1);
   if( !n)
   {  
   	    printf("\n ��Կ����ʧ��!\n");
   	    return;
   } 
   memset(passwd2,0,sizeof(passwd2));
   bcd_to_asc(passwd2,passwd1,32,0);
   printf("\n������������:%s\n", passwd2);
   printf("�밴���������! ...\n");
 
  
   getchar();
   return 0;
}