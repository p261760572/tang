#include <stdlib.h>
#include <stdio.h>


int main()

{

 unsigned char buffer[4];

    memcpy(buffer,(unsigned char *)"\x00\x74",2); 
   
    printf( "\\x74 >>4=%d\n",(((buffer[1]>>4)&0x0f)*10) +(buffer[1]&0x0f));
    printf( "\\x74 >>4&0x0f=%d\n",buffer[1]&0x0f);

    printf("len=%d\n",(buffer[0]>>4)&0x0f*1000 +(buffer[0]&0x0f)*100+((buffer[1]>>4)&0x0f)*10+buffer[1]&0x0f);
   return 1;
}
