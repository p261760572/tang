#include  "dccdcs.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <sys/syscall.h> 
#define gettid() syscall(__NR_gettid)

void main()
{
	unsigned char buf[512];
	struct MSG_STRU
 {
	
	  long iPid;
	  time_t send_time;
	  unsigned char type;
	  char msg[1];
 };
 struct MSG_STRU Msg,*pMsg;
	time_t t;
	long ppid,r_qid,s_qid;
   int len,i,x,l,f;	
	 	s_qid= queue_connect("1000")	;
	  if ( s_qid <0)
	  {	
	  		fprintf(stderr,"Can not connect send message queue !");
	  		return; 
	  }
	  r_qid= queue_connect("1001")	;
	  if ( r_qid <0)
	  {	
	  		fprintf(stderr,"Can not connect send message queue !");
	  		return; 
	  }
	  ppid=gettid();
	  t= time(NULL);
	  memcpy(buf,(char *)&ppid,sizeof(ppid));
	  memcpy(buf+sizeof(long),(unsigned char *)&t,sizeof(t));
	  memcpy(buf+sizeof(long)+sizeof(t),"\x01",1);
//	 	memcpy(buf+sizeof(long)+1+sizeof(t),"\x00\x07K0S0001",9);
    Msg.iPid=ppid;
    Msg.send_time=t;
    Msg.type=0x01;
   fprintf(stderr,"\nbuf=");
   for( i=0; i<sizeof(long)+1+sizeof(t);i++) fprintf(stderr,"%02x",buf[i]);
   fprintf(stderr,"\nmsg=");
   for( i=0; i<sizeof(Msg);i++) fprintf(stderr,"%02x",((unsigned char *)&Msg)[i]);
   fprintf(stderr,"\n");
   pMsg =(struct MSG_STRU *)buf;
   for( i=0; i<sizeof(struct MSG_STRU);i++) fprintf(stderr,"%02x",((unsigned char *)pMsg)[i]);
   fprintf(stderr,"\n");
   
   fprintf(stderr,"send_time=%d t=%d\n",pMsg->send_time,t);
//return ;
	 fprintf(stderr,"begin time=%d sizeof(t)=%d pid=%d sizepf(msg)=%d\n",t,sizeof(time_t),ppid,sizeof(struct MSG_STRU)); 
	 for ( x=0,l=0 ; x< 10000; x++) {
	 
	  t= time(NULL);
	  memcpy(buf,(char *)&ppid,sizeof(ppid));
	  memcpy(buf+sizeof(long),"\x01",1);
	  memcpy(buf+sizeof(long)+1,(unsigned char *)&t,sizeof(t));
	  
	  
	  memcpy(buf+sizeof(long)+1+sizeof(t),"\x00\xA0K0S0001",9);
	  if( x==0){
	  fprintf(stderr,"\nl=%d buf=",l);
	          for( i=0 ; i< 26 ;i++)
		        fprintf(stderr,"%02x",(unsigned char)buf[i]);
	          fprintf(stderr,"\n");
	  }
/*	  
	  pMsg =(struct MSG_STRU *)buf;
   for( i=0; i<sizeof(struct MSG_STRU);i++) fprintf(stderr,"%02x",((unsigned char *)pMsg)[i]);
   fprintf(stderr,"\n");
   
   fprintf(stderr,"send_time=%d t=%d\n",pMsg->send_time,t);
*/	
	  for( i=0;i<100;i++) 
    if( 0>(len=queue_send(s_qid,buf,sizeof(long)+1+sizeof(t)+162,1)))
	  {	
	  	fprintf(stderr,"send fail! x=%d err[%s]\n",x,strerror(errno));
	  	return;
	  }
//	  else fprintf(stderr,"send i=%d\n",i);
    for( i=0;i<100;i++)
    {
		  if( 0>(len=queue_recv(r_qid,buf,sizeof(buf),0)))
		  {
		  	fprintf(stderr,"recv fail! i=%d err[%s]\n",i,strerror(errno));
		  	break;
		  }
		  else 
		  {	   
	          l++;
/*	          fprintf(stderr,"\nl=%d buf=",l);
		          for( f=0 ; f< len ;f++)
			        fprintf(stderr,"%02x",(unsigned char)buf[f]);
		          fprintf(stderr,"\n");
*/	     
	 //fprintf(stderr,"l=%d \n",l);
	    }
    }
	 }
	  t= time(NULL);
	 fprintf(stderr,"end time=%d l=%d\n",t,l); 
       
}
