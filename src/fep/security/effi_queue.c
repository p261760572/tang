#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "effi_sec.h"

int effi_init_queue(struct EFFI_QUEUE *queue,int size)
{
	if( queue ==  NULL || size <10 || size > 1024*1024 ) return 0;
	
	queue->max_size=size; // ��������С
	queue->free_size=size;
	queue->message_num=0;  //���������ڵ���Ϣ��
  queue->buf_head=0;  //�������ڻ�������ͷ
  queue->buf_tail=0;	 //�������ڻ�������λ
  if( (queue->buf=malloc(size))== NULL) return 0;
	return 1;
}

int effi_put_queue(struct EFFI_QUEUE *queue, char * buf ,short size)
{
	if( queue == NULL || buf == NULL || size <=0) return 0;
	if( queue->free_size < (size+2) ) return 0;
	if( (queue->buf_tail < queue->buf_head) || // ����β�ڶ���ͷ��ǰ�棬�м�Ϊ������
		( queue->buf_tail == queue->buf_head && (queue->max_size-queue->buf_tail)>(size+2))) //������������,�Ҷ���β����������β����������ŵ�������
	{
//			fprintf(stderr,"// ����β�ڶ���ͷ��ǰ�棬�м�Ϊ������ ;������������,�Ҷ���β����������β����������ŵ�������\n");
			queue->buf[queue->buf_tail]=(unsigned char)(size/256);
		 	queue->buf[queue->buf_tail+1]=(unsigned char)(size%256);
			memcpy(queue->buf+(queue->buf_tail+2),buf,size);
			queue->buf_tail +=(size+2);
	}
	else //����ͷ�Ͷ���β�����Ϊ������
	{
		 if( queue->max_size-queue->buf_tail >=(size+2) ) //������������,�Ҷ���β����������β����������ŵ�������
		 {
//		 		 fprintf(stderr,"//������������,�Ҷ���β����������β����������ŵ�������\n");
		 		 queue->buf[queue->buf_tail]=(unsigned char)(size/256);
		 	   queue->buf[queue->buf_tail+1]=(unsigned char)(size%256);
		 		 memcpy(queue->buf+(queue->buf_tail+2),buf,size);
		 		 if( queue->max_size-queue->buf_tail == size+2) queue->buf_tail=0;
		 		 else queue->buf_tail +=(size+2);
		 }
		 else //������������,�Ҷ���β����������β������������ŵ������ݣ����߶����������ݣ�����β����������β������������ŵ�������
		 {
		 	 if( queue->max_size-queue->buf_tail >1) //����β���ռ��ܴ�����ݳ��ȼ����ϵĿռ�,�����ܴ���������ݣ�
		 	 {
//		 	 	 fprintf(stderr,"//����β���ռ��ܴ�����ݳ��ȼ����ϵĿռ䣬�����ܴ����������\n");
		 	 	 queue->buf[queue->buf_tail]=(unsigned char)(size/256);
		 	   queue->buf[queue->buf_tail+1]=(unsigned char)(size%256);
		 	   memcpy(queue->buf+queue->buf_tail+2,buf,queue->max_size - queue->buf_tail-2);
		 	   memcpy(queue->buf,buf+(queue->max_size - queue->buf_tail-2),size-(queue->max_size - queue->buf_tail-2));
		 	   queue->buf_tail= size-(queue->max_size - queue->buf_tail-2);
		 	 }
		 	 else //����β���ռ���û�д�����ݳ��ȿռ�
		 	 {
//		 	 	 fprintf(stderr,"����β���ռ���û�д�����ݳ��ȿռ�\n");
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
  
  if( queue->max_size - (queue->buf_head+2 ) >=len ) // ����ֶ�ȡ����
  {
  		memcpy(buf,queue->buf+queue->buf_head+2,len);
  		if( queue->max_size - (queue->buf_head+2 ) ==len ) queue->buf_head  = 0; // ���ε����ݽ����պ�Ϊ������β������һ�����ݶ���ָ�򻺳���ͷ��
  		else queue->buf_head  += (2+len);
  }
  else if( queue->max_size - (queue->buf_head+2 ) >=0 ) { //��ֶ�ȡ����
//  	fprintf(stderr,"<%s> ��ֶ�ȡ����\n",__FUNCTION__);
   memcpy(buf,queue->buf+(queue->buf_head+2),queue->max_size- queue->buf_head-2 ); 
   memcpy(buf+(queue->max_size- queue->buf_head-2),queue->buf,len - (queue->max_size- queue->buf_head-2) ); 
   queue->buf_head = len - (queue->max_size- queue->buf_head-2);
  }
  else  //���ݳ�����һ���ֽ�λ��λ�ڻ�����ͷ��
  {
 // 	 fprintf(stderr,"<%s> ���ݳ�����һ���ֽ�λ��λ�ڻ�����ͷ��\n",__FUNCTION__);
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
  
  if( queue->max_size - (queue->buf_head+2 ) >=len ) // ����ֶ�ȡ����
  {
  		memcpy(buf,queue->buf+queue->buf_head+2,len);
 // 		if( queue->max_size - (queue->buf_head+2 ) ==len ) queue->buf_head  = 0; // ���ε����ݽ����պ�Ϊ������β������һ�����ݶ���ָ�򻺳���ͷ��
 // 		else queue->buf_head  += (2+len);
  }
  else if( queue->max_size - (queue->buf_head+2 ) >=0 ) { //��ֶ�ȡ����
//  	fprintf(stderr,"<%s> ��ֶ�ȡ����\n",__FUNCTION__);
   memcpy(buf,queue->buf+(queue->buf_head+2),queue->max_size- queue->buf_head-2 ); 
   memcpy(buf+(queue->max_size- queue->buf_head-2),queue->buf,len - (queue->max_size- queue->buf_head-2) ); 
//   queue->buf_head = len - (queue->max_size- queue->buf_head-2);
  }
  else  //���ݳ�����һ���ֽ�λ��λ�ڻ�����ͷ��
  {
 // 	 fprintf(stderr,"<%s> ���ݳ�����һ���ֽ�λ��λ�ڻ�����ͷ��\n",__FUNCTION__);
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