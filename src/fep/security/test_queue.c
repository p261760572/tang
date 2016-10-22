#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "effi_sec.h"

int main()
{
	struct EFFI_QUEUE  q;
	int i=0,s;
	char buf[200];
 fprintf(stderr,"effi_init_queue return=%d",	effi_init_queue( &q,1024));
 effi_printf_queue(&q);
 
 
 fprintf(stderr,"begin time=%ld\n",time(NULL));
 for( i=0 ;i < 10; i++)
 {
	 {
	 	 effi_put_queue(&q,"12345678901234567890qwertyuiopasdfghjklzxcvbnm,                  ",7);
//	 	 effi_printf_queue(&q);
	
	 }
	 
}
for( i=0 ;i < 10; i++)
{
	 	 memset(buf,0,sizeof(buf));
	 	 if( (s=effi_get_queue(&q,buf,100)) <=0) fprintf(stderr," i=%d\n",i);
//	 	 buf[s]=0x00;
	 	 effi_printf_queue(&q);
	 	 fprintf(stderr,"buf=[%d][%d][%s]\n",i,s,buf);
	 	 
	 }
 fprintf(stderr,"end time=%ld\n",time(NULL));

	 	
/* for( i=0; i< 3;i++)
 {
 	 effi_put_queue(&q,buf,48);
 	 effi_printf_queue(&q);
 }
 fprintf(stderr,"get oper \n");
 for( i=0; i< 3;i++)
 {
 	 effi_get_queue(&q,buf,100);
 	 effi_printf_queue(&q);
 }
 */
 effi_destroy_queue(&q);
}