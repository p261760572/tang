#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "effi_sec.h"

int effi_init_queue(struct EFFI_QUEUE *queue,int size)
{
	if( queue ==  NULL || size <10 || size > 1024*1024 ) return 0;
	
	queue->max_size=size; // 缓冲区大小
	queue->free_size=size;
	queue->message_num=0;  //缓冲区存在的消息数
  queue->buf_head=0;  //数据所在缓冲区的头
  queue->buf_tail=0;	 //数据所在缓冲区的位
  if( (queue->buf=malloc(size))== NULL) return 0;
	return 1;
}

int effi_put_queue(struct EFFI_QUEUE *queue, char * buf ,short size)
{
	if( queue == NULL || buf == NULL || size <=0) return 0;
	if( queue->free_size < (size+2) ) return 0;
	if( (queue->buf_tail < queue->buf_head) || // 队列尾在队列头的前面，中间为自由区
		( queue->buf_tail == queue->buf_head && (queue->max_size-queue->buf_tail)>(size+2))) //队列中无数据,且队列尾部至缓冲区尾部能完整存放当次数据
	{
//			fprintf(stderr,"// 队列尾在队列头的前面，中间为自由区 ;队列中无数据,且队列尾部至缓冲区尾部能完整存放当次数据\n");
			queue->buf[queue->buf_tail]=(unsigned char)(size/256);
		 	queue->buf[queue->buf_tail+1]=(unsigned char)(size%256);
			memcpy(queue->buf+(queue->buf_tail+2),buf,size);
			queue->buf_tail +=(size+2);
	}
	else //队列头和队列尾的外侧为自由区
	{
		 if( queue->max_size-queue->buf_tail >=(size+2) ) //队列中有数据,且队列尾部至缓冲区尾部能完整存放当次数据
		 {
//		 		 fprintf(stderr,"//队列中有数据,且队列尾部至缓冲区尾部能完整存放当次数据\n");
		 		 queue->buf[queue->buf_tail]=(unsigned char)(size/256);
		 	   queue->buf[queue->buf_tail+1]=(unsigned char)(size%256);
		 		 memcpy(queue->buf+(queue->buf_tail+2),buf,size);
		 		 if( queue->max_size-queue->buf_tail == size+2) queue->buf_tail=0;
		 		 else queue->buf_tail +=(size+2);
		 }
		 else //队列中无数据,且队列尾部至缓冲区尾部不能完整存放当次数据；或者队列中有数据，队列尾部至缓冲区尾部不能完整存放当次数据
		 {
		 	 if( queue->max_size-queue->buf_tail >1) //队列尾部空间能存放数据长度及以上的空间,但不能存放整个数据；
		 	 {
//		 	 	 fprintf(stderr,"//队列尾部空间能存放数据长度及以上的空间，但不能存放整个数据\n");
		 	 	 queue->buf[queue->buf_tail]=(unsigned char)(size/256);
		 	   queue->buf[queue->buf_tail+1]=(unsigned char)(size%256);
		 	   memcpy(queue->buf+queue->buf_tail+2,buf,queue->max_size - queue->buf_tail-2);
		 	   memcpy(queue->buf,buf+(queue->max_size - queue->buf_tail-2),size-(queue->max_size - queue->buf_tail-2));
		 	   queue->buf_tail= size-(queue->max_size - queue->buf_tail-2);
		 	 }
		 	 else //队列尾部空间已没有存放数据长度空间
		 	 {
//		 	 	 fprintf(stderr,"队列尾部空间已没有存放数据长度空间\n");
		 	 	 queue->buf[queue->buf_tail]=(unsigned char)(size/256);
		 	   queue->buf[0]=(unsigned char)(size%256);
		 	   memcpy(queue->buf+1,buf,size);
		 	   queue->buf_tail= size + 1;
		 	 }	
		 	 
		 }
	}
	queue->free_size -= (size+2);
	queue->message_num++;
	return 1;
}

int effi_get_queue(struct EFFI_QUEUE *queue, char * buf ,int size)
{
	int len;
	if( queue ==  NULL || buf == NULL ) return 0;
	if( queue->message_num <1) return 0;
	if( queue->buf_head <queue->max_size -1)
		len=((unsigned char)queue->buf[queue->buf_head])*256+(unsigned char)queue->buf[queue->buf_head+1];
	else
		len=((unsigned char)queue->buf[queue->buf_head])*256+(unsigned char)queue->buf[0];
  if( size < len ) return -1;
  
  if( queue->max_size - (queue->buf_head+2 ) >=len ) // 不需分段取数据
  {
  		memcpy(buf,queue->buf+queue->buf_head+2,len);
  		if( queue->max_size - (queue->buf_head+2 ) ==len ) queue->buf_head  = 0; // 本次的数据结束刚好为缓冲区尾部，下一个数据段需指向缓冲区头部
  		else queue->buf_head  += (2+len);
  }
  else if( queue->max_size - (queue->buf_head+2 ) >=0 ) { //需分段取数据
//  	fprintf(stderr,"<%s> 需分段取数据\n",__FUNCTION__);
   memcpy(buf,queue->buf+(queue->buf_head+2),queue->max_size- queue->buf_head-2 ); 
   memcpy(buf+(queue->max_size- queue->buf_head-2),queue->buf,len - (queue->max_size- queue->buf_head-2) ); 
   queue->buf_head = len - (queue->max_size- queue->buf_head-2);
  }
  else  //数据长度有一个字节位置位于缓冲区头部
  {
 // 	 fprintf(stderr,"<%s> 数据长度有一个字节位置位于缓冲区头部\n",__FUNCTION__);
  	 memcpy(buf,queue->buf+1,len);
  	 queue->buf_head = len +1;
  }
  queue->message_num--;
  queue->free_size +=(len+2);
  return len;
}


int effi_uget_queue(struct EFFI_QUEUE *queue, char * buf ,int size)
{
	int len;
	if( queue ==  NULL || buf == NULL ) return 0;
	if( queue->message_num <1) return 0;
	if( queue->buf_head <queue->max_size -1)
		len=((unsigned char)queue->buf[queue->buf_head])*256+(unsigned char)queue->buf[queue->buf_head+1];
	else
		len=((unsigned char)queue->buf[queue->buf_head])*256+(unsigned char)queue->buf[0];
  if( size < len ) return -1;
  
  if( queue->max_size - (queue->buf_head+2 ) >=len ) // 不需分段取数据
  {
  		memcpy(buf,queue->buf+queue->buf_head+2,len);
 // 		if( queue->max_size - (queue->buf_head+2 ) ==len ) queue->buf_head  = 0; // 本次的数据结束刚好为缓冲区尾部，下一个数据段需指向缓冲区头部
 // 		else queue->buf_head  += (2+len);
  }
  else if( queue->max_size - (queue->buf_head+2 ) >=0 ) { //需分段取数据
//  	fprintf(stderr,"<%s> 需分段取数据\n",__FUNCTION__);
   memcpy(buf,queue->buf+(queue->buf_head+2),queue->max_size- queue->buf_head-2 ); 
   memcpy(buf+(queue->max_size- queue->buf_head-2),queue->buf,len - (queue->max_size- queue->buf_head-2) ); 
//   queue->buf_head = len - (queue->max_size- queue->buf_head-2);
  }
  else  //数据长度有一个字节位置位于缓冲区头部
  {
 // 	 fprintf(stderr,"<%s> 数据长度有一个字节位置位于缓冲区头部\n",__FUNCTION__);
  	 memcpy(buf,queue->buf+1,len);
//  	 queue->buf_head = len +1;
  }
//  queue->message_num--;
//  queue->free_size +=(len+2);
  return len;
}

void effi_printf_queue(struct EFFI_QUEUE *queue)
{

	if ( queue == NULL ) return;
	fprintf(stderr,"\n/******************************/\n");
	fprintf(stderr," max_size=%d ",queue->max_size);
	fprintf(stderr,"free_size=%d ",queue->free_size);
	fprintf(stderr,"message_num=%d ",queue->message_num);
	fprintf(stderr,"head=%d ",queue->buf_head);
	fprintf(stderr,"tail=%d \n",queue->buf_tail);
	fprintf(stderr,"/******************************/\n");
}

void effi_destroy_queue(struct EFFI_QUEUE *queue)
{
	 if( queue == NULL ) return;
	 if( queue->buf == NULL ) return;
	 free(queue->buf);
}