#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main ()
{

  char ss[]="ddd=ssss=kkk";

  unsigned char prg_ver[3];
  int forward;
 memcpy(prg_ver,"\x20\x14",2);
 prg_ver[2]=0x00;
 forward=(prg_ver[0]>>4)*1000+(prg_ver[0]&0x0f)*100+ (prg_ver[1]>>4)*10+(prg_ver[1]&0x0f);
 fprintf(stderr,"prg_ver[0]>>4*1000=%d\n",(prg_ver[0]>>4)*1000);
 fprintf(stderr,"forward=%d\n",forward);
/*
const char* s = "iios/12DDWDFF@122"; 
char buf[20]; 
sscanf( s, "%*[^/]/%[^@]", buf ); 
printf( "%s\n", buf ); */
return 0; 
}
