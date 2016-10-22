#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
int l;
unsigned char buffer[20];
  srand((unsigned)time(NULL));
  for ( l=0; l<8; l++)
   {
   	  buffer[l]=((unsigned char)rand()-l*l)%256;
         printf("%02x ",buffer[l]);
   }
   return 1;
}
