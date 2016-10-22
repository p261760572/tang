#include <stdlib.h>
#include "folder.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)

#define DCS_SEC_NAME "effi_sec.conf"

#define DES_TYPE 0x01
#define SM4_TYPE 0x22

static int sgSndQid=-1 ,sgRcvQid=-1;
int CommandProc(char *inBuf,int inLen,char *outBuf,int *outLen, unsigned char type);
int Init()
{ 
	char sgSndID[40 + 1], sgRcvID[40 + 1];
  	char caBuffer[256], *p;
	int iFd=-1;  
	if(sgSndQid < 0 || sgRcvQid < 0)
	{
	  	//�趨commlnk�����ļ��ľ���·��������
	  	memset(caBuffer, 0, sizeof(caBuffer) );
	  	sprintf(caBuffer, "%s/config/%s", getenv("ICS_HOME"), DCS_SEC_NAME);
	  	if ((iFd = conf_open(caBuffer)) < 0)
	  	{
			dcs_log( 0,0, "<Init> open %s fail ! ", caBuffer);
			return -1;
	  	}
        //��ȡ������'queue.send'
	  	memset(caBuffer, 0, sizeof(caBuffer) );
	  	if(0 == conf_get_first_string(iFd, "queue.recv",caBuffer))
	  	{
	        p = strtok(caBuffer, " \t\r");
		  	if(p == NULL)
		  	{
				close(iFd);
				dcs_log( 0,0, "<DESInit>queue.recv[%s] error! ", caBuffer);
				close(iFd);
				return -1;
		  	}
		  	strcpy(sgRcvID, p); 
	  	}
	  	else
	  	{
			dcs_log( 0,0, "<Init>conf_get_first_string queue.recv fail ! ");
			strcpy(sgRcvID, "1000");
			
  		}
	  	memset(caBuffer, 0, sizeof(caBuffer) );
	  	if(0 == conf_get_first_string(iFd, "queue.send",caBuffer))
	  	{
	        p = strtok(caBuffer, " \t\r");
		  	if(p == NULL)
		  	{
				close(iFd);
				dcs_log( 0,0, "<Init>queue.send[%s] error! ", caBuffer);
				return -1;
		  	}
		  	strcpy(sgSndID, p); 
	  	}
	  	else
	  	{
			dcs_log( 0,0, "<Init>conf_get_first_string queue.send fail ! ");
			strcpy(sgSndID, "1001");
	  	}
	  	conf_close(iFd);
		if( sgSndQid <0)
	  		sgSndQid = _queue_connect(sgRcvID);
		if ( sgSndQid <0)
		{
			dcs_log(0,0,"connect msg queue fail[%s],error %d-%s!", sgRcvID, errno, strerror(errno));
			return -1;
		}
		if( sgRcvQid <0)
			sgRcvQid = _queue_connect(sgSndID);
		if ( sgRcvQid <0)
		{
			dcs_log(0,0,"connect msg queue fail[%s],error %d-%s!", sgSndID, errno, strerror(errno));
			return -1;
		}	
	}
	return 1;
}

struct secmsgstru  
{  
  long msgtype;  
  char msgtext[2048];   
};  
int CommandProc(char *inBuf,int inLen,char *outBuf,int *outLen, unsigned char type)
{
	long iPid;
	int i;
	time_t send_time;
  struct secmsgstru msg;
//	char caTmpBuf[4096];
	if ( 0 > Init() )
	{
		dcs_log(0,0," init system fail");
		return -1;
	}
	memset(&msg,0,sizeof(msg));
	iPid = gettid();
	msg.msgtype= iPid;
	send_time = time(NULL);
	i = sizeof(long);
//	memcpy(caTmpBuf, &iPid, i);
  msg.msgtext[0]=type;
//	caTmpBuf[i++] = type;
	i++;
	memcpy(msg.msgtext + 1, &send_time, sizeof(time_t));
	i += sizeof(time_t);
	memcpy(msg.msgtext + 1+sizeof(time_t), inBuf, inLen);
	msg.msgtext[sizeof(time_t)+inLen+1]=0x00;
	i +=inLen;
 	if (0 >queue_send( sgSndQid ,&msg, i,0))
  {
	  	sgSndQid=-1;
		if ( 0 > Init() )
		{
			dcs_log(0,0," init system fail");
			return -1;
		}
	 	if (0 > queue_send( sgSndQid ,&msg, i,0))
	 	{
		  	dcs_log(0,0,"send msg to queue fail!");
		  	return -1;
		}
  }
  dcs_debug(msg.msgtext, i-sizeof(long), "send msg to queue succ!");
//	i=0;
//  caTmpBuf[sizeof(long)]=0x00;
	if ( 0 >(*outLen=queue_recv(sgRcvQid, &msg, sizeof(msg)-sizeof(long),0)))
	{
		sgRcvQid=-1;
		dcs_log(0,0,"recv msg from msg queue fail!");
		return -1;
	}
  
//	i += 4;
  *outLen=*outLen -sizeof(long)-1-sizeof(time_t)-2;
  dcs_debug(msg.msgtext+1+sizeof(time_t)+2 , *outLen , "recv msg from queue succ!");
  memcpy(outBuf, msg.msgtext + 1+sizeof(time_t)+2, *outLen );
  
  return 1;
}



/*
 * ȡ�����
 *
 */
int GetRandomData(char *randomData) {
    sprintf(randomData, "%08X%08X", rand(), rand());
    return 0;
}


/* RSA �㷨������Կ�� */
int RSAGETKY(char *return_code, char *priv_key_data,int *priv_len,char *pub_key_data,int *pub_len,
              int key_index,int key_len)
{
	char Buffer[2048];
	char tmp[10];
	int	nLen ;

	if (return_code==NULL || priv_key_data==NULL || pub_key_data==NULL)
	{
		dcs_log(0,0,"in RSAGETKY::  parameter is NULL!");
		memcpy(return_code,"95",2);	
		return (-1);
	}
  if ( key_len <320 || key_len >4096 )
  {
		dcs_log(0,0,"in RSAGETKY:: key len is err!");
		memcpy(return_code,"95",2);	
		return (-1);
	}	
	if ( key_len %8 )
	{
		dcs_log(0,0,"in RSAGETKY:: key len is err!");
		memcpy(return_code,"95",2);	
		return (-1);
	}
	if ( key_index >20 ) key_index=99;
	memset(Buffer,'\0',sizeof(Buffer));
	memcpy(Buffer+2,"34",2);
	nLen=4;
	sprintf(Buffer+nLen,"%04d",key_len);
	nLen=nLen+4;
	sprintf(Buffer+nLen,"%02d",key_index);
	nLen=nLen+2;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"����������Կ ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"A",1)!=0)
	{
			  sprintf(return_code,"%02x",Buffer[1]);
//			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"����������Կʧ��!");
		    return -1;
  }
  else memcpy(return_code,"00",2);
  memset(tmp,0,sizeof(tmp));
  memcpy(tmp,Buffer+2,4);
  *priv_len= atoi(tmp);
  if (*priv_len>0 && *priv_len <=4096)
  	memcpy(priv_key_data,Buffer+2+4,*priv_len);
  *pub_len=nLen-*priv_len-6;
	memcpy(pub_key_data,Buffer+2+4+*priv_len,*pub_len);
	memcpy(return_code,Buffer,2);
	return (1);
}




int RSATODES_PIN(char *return_code, const char *key_data,int key_len ,
                 int rsa_index,char *des_index,const char *rsa_pan,const char *des_pan,
                 const char *rsa_mdata,int rsa_datalen,char *des_mdata)
{
	char Buffer[2048];
	int	nLen,i;

	if (return_code==NULL || des_mdata==NULL)
	{
		dcs_log(0,0,"in RSATODES_PIN:: output parameter is NULL!");
		memcpy(return_code,"95",2);
		return (-1);
	}
  if (key_data==NULL || rsa_pan==NULL || des_pan == NULL ||
  	  rsa_mdata == NULL)
	{
		dcs_log(0,0,"in RSATODES_PIN:: input parameter is NULL!");
		memcpy(return_code,"95",2);
		return (-1);
	}
	printf("\n packet begin!\n");
	memset(Buffer,0,sizeof(Buffer));
	memcpy(Buffer+2,"43",2);
	i=4;
	sprintf(Buffer+i,"%02d",rsa_index);
	i=i+2;
	memcpy(Buffer+i, des_index, 3);
	i=i+3;
	if ( key_len==16 ) 
		memcpy(Buffer+i,"X",1);
	else if ( key_len==32 )
		memcpy(Buffer+i,"Y",1); 
	else if ( key_len==48 )
		memcpy(Buffer+i,"Z",1); 
	else
	{
		dcs_log(0,0,"in RSATODES_PIN::  parameter is err!key_len[%d]",key_len);
		memcpy(return_code,"95",2);
		return (-1);
	}
	printf("\n packet key_len type!\n");
	i=i+1;
	memcpy(Buffer+i,key_data,key_len);
	i=i+key_len;
	memcpy(Buffer+i,"1",1);
	i=i+1;
	memcpy(Buffer+i,rsa_pan,16);
	i=i+16;
	memcpy(Buffer+i,des_pan,16);
	i=i+16;
//	printf("\n packet des_pan[%s] !\n",des_pan);
//	printf("\n packet rsa_mdata len=[%d]\n",strlen(rsa_mdata));
	memcpy(Buffer+i,rsa_mdata,rsa_datalen);
	i=i+rsa_datalen;
//     printf("\n packet rsa_mdata !\n");
	Buffer[0]=(i-2)/256;
	Buffer[1]=(i-2)%256;
	printf("\n packet end!\n");
  if ( 0>CommandProc(Buffer,i,Buffer,&i, DES_TYPE))
	{
		  dcs_log(0,0,"in RSATODES_PIN:PIN ת�� ͨ��ʧ��!");
		  return -1;
	}
	if (memcmp(Buffer,"00",2)!=0)
	{
		dcs_log(0,0,"in RSATODES_PIN:pin ת������ʧ�� response:*%2.2s*",Buffer);
		
		memcpy(return_code,Buffer,2);
		return (-1);
	}

			
	asc_to_bcd(des_mdata,Buffer+2,16,0);
	memcpy(return_code,Buffer,2);
	return (1);
}

int DESTORAS_PIN(char *return_code, char *des_key_data,int des_key_len ,
                 char *rsa_key_data, int rsa_key_len,
                 char *des_index,char *rsa_pan,char *des_pan,
                 char *des_mdata,char *rsa_mdata,int *rsa_datalen)
{
	char Buffer[2048],tmp[10];
	int	nLen,i;

	if (return_code==NULL || des_mdata==NULL)
	{
		dcs_log(0,0,"in RSATODES_PIN:: output parameter is NULL!");
		memcpy(return_code,"95",2);
		return (-1);
	}
  if (des_key_data==NULL || rsa_pan==NULL || des_pan == NULL || 
  	  rsa_key_data==NULL || rsa_mdata == NULL )
	{
		dcs_log(0,0,"in RSATODES_PIN:: input parameter is NULL!");
		memcpy(return_code,"95",2);
		return (-1);
	}
	
	memset(Buffer,0,sizeof(Buffer));
	memcpy(Buffer+2,"47",2);
	i=4;
	sprintf(Buffer+i,"%04d",rsa_key_len);
	i=i+4;
	memcpy(Buffer+i,rsa_key_data,rsa_key_len);
	i=i+rsa_key_len;
	memcpy(Buffer+i, des_index, 3);
	i=i+3;
	if ( des_key_len==16 ) 
		memcpy(Buffer+i,"X",1);
	else if ( des_key_len==32 )
		memcpy(Buffer+i,"Y",1); 
	else if ( des_key_len==48 )
		memcpy(Buffer+i,"Z",1); 
	else
	{
		dcs_log(0,0,"in RSATODES_PIN::  parameter is err!des_key_len[%d]",des_key_len);
		memcpy(return_code,"95",2);
		return (-1);
	}
	i=i+1;
	memcpy(Buffer+i,des_key_data,des_key_len);
	i=i+des_key_len;
	memcpy(Buffer+i,"1",1);
	i=i+1;

	memcpy(Buffer+i,des_pan,16);
	i=i+16;
	memcpy(Buffer+i,rsa_pan,16);
	i=i+16;
	memcpy(Buffer+i,des_mdata,16);
	i=i+16;

	Buffer[0]=(i-2)/256;
	Buffer[1]=(i-2)%256;
  if ( 0>CommandProc(Buffer,i,Buffer,&i, DES_TYPE))
	{
		  dcs_log(0,0,"in RSATODES_PIN:PIN ת�� ͨ��ʧ��!");
		  return -1;
	}
	if (memcmp(Buffer,"00",2)!=0)
	{
		dcs_log(0,0,"in RSATODES_PIN:pin ת������ʧ�� response:*%2.2s*",Buffer);
		
		memcpy(return_code,Buffer,2);
		return (-1);
	}
  memset(tmp,0,sizeof(tmp));
	memcpy(tmp,Buffer+2,4);
	*rsa_datalen=atoi(tmp);
	memcpy(rsa_mdata,Buffer+2+4,*rsa_datalen);
	memcpy(return_code,Buffer,2);
	return (1);
}


//�����ն�����ԿTMK DES(K0/K1������(QK/QL)
//����	�����ն�����Կ�����������ĺͼ��ֵ���ظ�������
//˵��	���ɵ��ն�����Կ������У�顣
//��ڲ���
//sek_index:SEK����,S��4λ����
//tek_index:TEK����,T��4λ����
//flag 0������ 1:DES 64 2:DES 128 3 DES 192
//���ڲ���
//return_code:������
//sek_tmk_data:TMK����1,��SEK����
//tek_tmk_data:TMK����2,��TEK����
//chk_tmk_data:TMK����ֵ
int GET_TMK(char *return_code, char *sek_index, char *tek_index, unsigned char flag, char *sek_tmk_data, char *tek_tmk_data, char *chk_tmk_data)
{
	char Buffer[1024];
	int	nLen ;
	memset(Buffer, 0, sizeof(Buffer));
	nLen=2;
	
	{
		memcpy(Buffer+nLen,"\x04\x20",2);nLen=nLen+2;
		Buffer[nLen++]=atoi(sek_index)/256;
		Buffer[nLen++]=atoi(sek_index)%256;
//		memcpy(Buffer+nLen,sek_index,2);nLen=nLen+2;
//		memcpy(Buffer+nLen,tek_index,2);nLen=nLen+2;
		Buffer[nLen++]=atoi(tek_index)/256;
		Buffer[nLen++]=atoi(tek_index)%256;
		if(flag == 2)
			Buffer[nLen++]='1';
		else if(flag == 3)
			Buffer[nLen++]='2';
		else
			Buffer[nLen++]='0';
	}	
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, SM4_TYPE))
	{
		  dcs_log(0,0,"�����ն�����ԿTMK ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"A",1)!=0)
	{
			  sprintf(return_code,"%02x",Buffer[1]);
			  dcs_log(Buffer,nLen,"�����ն�����ԿTMKʧ��!");
		    return -1;
	}
	if(flag == 0 )
	{
		memcpy(sek_tmk_data, Buffer+1, 8);
		memcpy(tek_tmk_data, Buffer+1+8, 8);
		memcpy(chk_tmk_data, Buffer+1+8+8, 8);
	}
	else if(flag == 2)
	{
		memcpy(sek_tmk_data, Buffer+1, 16);
		memcpy(tek_tmk_data, Buffer+1+16, 16);
		memcpy(chk_tmk_data, Buffer+1+16+16, 8);
	}
	else if(flag == 3)
	{
		memcpy(sek_tmk_data, Buffer+1, 24);
		memcpy(tek_tmk_data, Buffer+2+24, 24);
		memcpy(chk_tmk_data, Buffer+2+24+24, 16);
	}	
	
	memcpy(return_code,"00",2);
	return (1);
}


//����������ԿPIK/MAK DES(K2/K3������(QT/QU)
//����	����������ԿPIK/MAK�����������ĺͼ��ֵ���ظ�������
//˵��	�������ɵ�������Կ��PIK��MAK����Ҫͬʱ�������������ģ�����һ����TMK��������ͨ���������ķ�ʽ��ǩ��Ӧ����Ϣ�д����նˣ���һ����SEK���ܺ󣬽����ı��������ݿ��С�
//��ڲ���
//sek_index1:SEK1����,���ڽ���TMK
//sek_index2:SEK2����,���ڼ���PIK/MAK
//tmk:TMK����,��SEK1����
//tmk_flag 0:���� 1:DES 64 2:DES 128 3 DES 192 
//pm_flag 0:���� 1:DES 64 2:DES 128 3 DES 192 [tmkΪ����ʱpkdmakֻ��Ϊ����,tmkΪDESʱpikmakֻ��ΪDES]
//���ڲ���
//return_code:������
//sek_pikmak_data:PIK/MAK����1,��SEK2����
//tmk_pikmak_data:PIK/MAK����2,��TMK����
//CheckValue:У��ֵ
int GET_WORK_KEY(char *return_code, char *sek_index1, char *sek_index2, char *tmk, unsigned char tmk_flag, unsigned char pm_flag, char *sek_pikmak_data, char *tmk_pikmak_data, char *CheckValue) 
{
	char Buffer[1024],tmp[64];
	
	int	nLen ;

	memset(Buffer, 0, sizeof(Buffer));
	nLen=2;
	
	{
		memcpy(Buffer+nLen,"\x04\x21",2);nLen=nLen+2;
//		memcpy(Buffer+nLen,sek_index1,5);nLen=nLen+5;
    Buffer[nLen++]=atoi(sek_index1)/256;
		Buffer[nLen++]=atoi(sek_index1)%256;
//		memcpy(Buffer+nLen,sek_index2,5);nLen=nLen+5;
    Buffer[nLen++]=atoi(sek_index2)/256;
		Buffer[nLen++]=atoi(sek_index2)%256;

		switch(pm_flag) //������Կ�ĳ���
		{
			case 2:
				Buffer[nLen++]=0x01;
				Buffer[nLen++]=0x01;
				break;				
			case 3:
				Buffer[nLen++]=0x02;
				Buffer[nLen++]=0x02;
				break;
			default:
				Buffer[nLen++]=0x00;
				Buffer[nLen++]=0x00;
				break;
				
		}
		if(tmk_flag == 0 ) //�ն�����Կ����
		{
			Buffer[nLen++] = 0x00;
			asc_to_bcd(tmp,tmk,16,0);
			memcpy(Buffer+nLen,tmp,8);
		}
		else if(tmk_flag == 1 ) //�ն�����Կ����
		{
			Buffer[nLen++] = 0x01;
			asc_to_bcd(tmp,tmk,32,0);
			memcpy(Buffer+nLen,tmp,16);
		}
		else if(tmk_flag == 2 ) //�ն�����Կ����
		{
			Buffer[nLen++] = 0x02;
			asc_to_bcd(tmp,tmk,48,0);
			memcpy(Buffer+nLen,tmp,24);
		}
	}	
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, SM4_TYPE))
	{
		  dcs_log(0,0,"�����ն�����ԿTMK ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"A",1)!=0)
	{
		  sprintf(return_code,"%02x",Buffer[1]);
			dcs_log(Buffer,nLen,"�����ն�����ԿTMKʧ��!");
			return -1;
  }
	if(tmk_flag == 0)
	{
		bcd_to_asc(sek_pikmak_data,Buffer+1,16,0);
		bcd_to_asc(tmk_pikmak_data,Buffer+1+8,16,0);
		bcd_to_asc(CheckValue,Buffer+1+8+8,16,0);
	}
	else if(tmk_flag == 1)
	{
	  bcd_to_asc(sek_pikmak_data,Buffer+1,32,0);
		bcd_to_asc(tmk_pikmak_data,Buffer+1+16,32,0);
		bcd_to_asc(CheckValue,Buffer+1+16+16,16,0);
	}
	else if(tmk_flag == 2)
	{
	  bcd_to_asc(sek_pikmak_data,Buffer+1,48,0);
		bcd_to_asc(tmk_pikmak_data,Buffer+1+24,48,0);
		bcd_to_asc(CheckValue,Buffer+1+24+24,16,0);
	}
	memcpy(return_code,"00",2);
	return 1;
}

//MAC���� DES��M0/M1������(Q2/Q3)
//��ڲ���
//sek_index:SEK����
//mak:MAK��Կ����,��SEK����
//flag 0������ 1:DES 64 2:DES 128 3 DES 192
//length:���ݳ���
//data:����
//���ڲ���
//return_code:������
//mac_value:MACֵ
//����ֵ 1�ɹ��� -1ʧ��
int CALCMAC(char *return_code, char *sek_index, char *mak, unsigned char flag, int length, char *data, char *mac_value)
{
	char Buffer[2048];
	char datalen[5];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(datalen,'\0',sizeof(datalen));
	nLen=2;
	if(flag == 0)
	{
		memcpy(Buffer+nLen,"Q21", 3);nLen=nLen+3;
	}
	else
	{
		memcpy(Buffer+nLen,"M02", 3);nLen=nLen+3;
	}
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	switch(flag)
	{
		case 0:
			memcpy(Buffer+nLen,mak, 32);nLen=nLen+32;
			Buffer[nLen++] = '1';
			break;
		case 2:
			Buffer[nLen++] = 'Y';
			memcpy(Buffer+nLen,mak, 32);nLen=nLen+32;
			break;
		case 3:
			Buffer[nLen++] = 'Z';
			memcpy(Buffer+nLen,mak,48);nLen=nLen+48;
			break;
		default:
			Buffer[nLen++] = 'X';
			memcpy(Buffer+nLen,mak,16);nLen=nLen+16;
			break;
	}
	sprintf(datalen,"%04d",length);
	memcpy(Buffer+nLen,datalen,4);nLen=nLen+4;
	memcpy(Buffer+nLen,data,length);nLen=nLen+length;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, SM4_TYPE))
	{
		  dcs_log(0,0,"MAC���� ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"MAC����ʧ��!");
		    return -1;
  }
	if(flag == 0)
		memcpy(mac_value,Buffer+2, 32);
	else
		memcpy(mac_value,Buffer+2, 16);
	memcpy(return_code,Buffer,2);
	return 1;
}

//PIN BLOCKת����P0/P1��
//����	��ԴPIN������ԴPIK���ܣ�����PIN��ʽת����Ȼ����Ŀ��PIK���������
//˵��	��PinBlock��ʽΪ02ʱ��PIN����Ϊ4~6λ����PinBlock��ʽΪ04ʱ��PIN����Ϊ6λ������PinBlock��ʽPIN��������4λ���12λ����ָ����PinBlock��ʽΪ01ʱ��Ҫ������16λ�ʺţ�Ŀ����Ϊ�˰���ĳЩ�����PinBlock��ʽ���ϲ�Ӧ����Ҫ�����Լ���Ҫ����֯����HSMֱ���ô����ʺ�����PIN������γ�PinBlock��
//��ڲ���
//sek_index1:ԴSEK����
//sek_index2:Ŀ��SEK����
//pik1:ԴPIK��Կ����,��ԴSEK����
//pik2:Ŀ��PIK��Կ����,��Ŀ��SEK����
//flag1:Դ������ܷ�ʽ 0:���� 1:DES 64 2: DES 128 3: DES 192 
//flag2:Ŀ��������ܷ�ʽ 0:���� 1:DES 64 2: DES 128 3: DES 192 
//in_pin_data:ԴPIN PinBlock����,��ԴPIK����
//���ڲ���
//return_code:������
//out_pin_dat:Ŀ��PinBlock����,��Ŀ��PIK����
int TRANS_PIN(char *return_code, char *sek_index1, char *sek_index2, char *pik1, char *pik2, unsigned char flag1, unsigned char flag2, char *in_pin_data, char *in_pan_data, char *out_pin_dat, char *out_pan_data)
{
	char Buffer[1024];
	int	nLen, pkLen;
	memset(Buffer,'\0',sizeof(Buffer));
	nLen=2;
	memcpy(Buffer+nLen,"Q0",2);nLen=nLen+2;
	Buffer[nLen++] = flag1 == 0 ? '2' : '1';
	Buffer[nLen++] = flag2 == 0 ? '2' : '1';
	switch(flag1)
	{
		case 0:
			pkLen = 32;
			break;
		case 2:
			Buffer[nLen++] = '2';
			pkLen = 32;
			break;
		case 3:
			Buffer[nLen++] = '3';
			pkLen = 48;
			break;
		default:
			Buffer[nLen++] = '1';
			pkLen = 16;
			break;
	}
	memcpy(Buffer+nLen,sek_index1,5);nLen=nLen+5;
	memcpy(Buffer+nLen, pik1, pkLen); nLen += pkLen;
	Buffer[nLen++] = '0';
	switch(flag2)
	{
		case 0:
			pkLen = 32;
			break;
		case 2:
			pkLen = 32;
			Buffer[nLen++] = '2';
			break;
		case 3:
			pkLen = 48;
			Buffer[nLen++] = '3';
			break;
		default:
			Buffer[nLen++] = '1';
			pkLen = 16;
			break;
	}	
	memcpy(Buffer+nLen,sek_index2,5);nLen=nLen+5;
	memcpy(Buffer+nLen, pik2, pkLen); nLen += pkLen;
	Buffer[nLen++] = '0';
	memcpy(Buffer+nLen,"01",2);nLen=nLen+2;
	memcpy(Buffer+nLen,"01",2);nLen=nLen+2;
	if(flag1 == 0)
	{
		memcpy(Buffer+nLen,in_pin_data, 32);nLen += 32;
	}	
	else
	{
		memcpy(Buffer+nLen,in_pin_data,16);nLen=nLen+16;
	}
	memcpy(Buffer+nLen, in_pan_data + 4, 12);nLen += 12;
	memcpy(Buffer+nLen, out_pan_data + 4, 12);nLen += 12;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, SM4_TYPE))
	{
		  dcs_log(0,0,"PIN BLOCKת�� ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
		  memcpy(return_code,Buffer,2);
		  dcs_log(Buffer,nLen,"PIN BLOCKת��ʧ��!");
	   	  return -1;
  	}
  	if(flag2 == 0)
		memcpy(out_pin_dat,Buffer+2,32);
  	else
		memcpy(out_pin_dat,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}

//�ŵ����� DES��E1/E2�� SM��QM/QN)
//��ڲ���
//sek_index:SEK����
//tdk:tdk��Կ����,��SEK����
//flag 0:���� 1:DES 64 2: DES 128 3: DES 192
//length:���ݳ���
//data:�ŵ���������
//���ڲ���
//return_code:������
//int out_length:���ݳ���
//out_data:�ŵ���������
int Track_Decryption(char *return_code, char *sek_index, char *tdk, unsigned char flag, int length, char *data, int *out_length, char *out_data)
{
	char Buffer[1024];
	char datalen[5];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(datalen,'\0',sizeof(datalen));
	nLen=2;
	
   dcs_log(0,0,"flag = %d", flag);
	if(flag == 0)
	{
		memcpy(Buffer+nLen,"QM",2);nLen=nLen+2;
		memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
		memcpy(Buffer+nLen,tdk, 32); nLen += 32;
		Buffer[nLen++] = '1';
	}
	else
	{
		memcpy(Buffer+nLen,"E1",2);nLen=nLen+2;
		memcpy(Buffer+nLen,"1",1);nLen=nLen+1;//�ӽ��ܱ�־(0:����,1:����)
		memcpy(Buffer+nLen,"0",1);nLen=nLen+1;//�ӽ����㷨(0:ECB,1:CBC)
		memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
		switch(flag)
		{
			case 2:
				Buffer[nLen++] = 'Y';
				memcpy(Buffer+nLen,tdk, 32);nLen=nLen+32;
				break;
			case 3:
				Buffer[nLen++] = 'Z';
				memcpy(Buffer+nLen,tdk, 48);nLen=nLen+48;
				break;
			default:
				Buffer[nLen++] = 'X';
				memcpy(Buffer+nLen,tdk,16);nLen=nLen+16;
				break;
		}
	}
	sprintf(datalen,"%04d",length);
	memcpy(Buffer+nLen,datalen,4);nLen=nLen+4;
	memcpy(Buffer+nLen,data,length);nLen=nLen+length;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, SM4_TYPE))
	{
		  dcs_log(0,0,"�ŵ����� ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"�ŵ����� ʧ��!");
		    return -1;
	}
	memcpy(return_code,Buffer,2);
	memcpy(datalen,Buffer+2,4);
	*out_length=atoi(datalen);
	memcpy(out_data,Buffer+2+4,atoi(datalen));
	return (1);
}
/*
DES_TO_RSA_KEY : 
��Կԭ����DES�㷨���ܱ�����ͨ���˺���ת��ΪRSA�㷨���ܲ�������ܽ����

*/

int DES_TO_RSA_KEY(char *return_code, char *sek_index, char *key, int rsa_length, char *rsa_key, int *out_length, char *out_data)
{
  char buffer[1024];
  int	nLen ;
  
  nLen=2;
  //ָ��
  memcpy(buffer+2,"\xc0\x49",2); nLen +=2;
  //��Կ����
  buffer[nLen++]=0x13;
  //KEY���ܱ�־
  buffer[nLen++]=0x03;
  //����Կ����
  buffer[nLen++]=atoi(sek_index)/256;
  buffer[nLen++]=atoi(sek_index)%256;
  //key ����
  buffer[nLen++]=16;
  //key
  asc_to_bcd(buffer+nLen,key,32,0);
  nLen +=16;
  //��Կ����(���ⲿ����)
  buffer[nLen++]=0xff;
  buffer[nLen++]=0xff;
  //��䷽ʽ RSA_NO_PADDING
  buffer[nLen++]=0x03;
  //���ʿ���
  nLen +=8;
  //��Կ���뷽ʽ;
  buffer[nLen++]=0x01;
  //��Կ����
  buffer[nLen++]=rsa_length/256;
  buffer[nLen++]=rsa_length%256;
  //��Կ
  memcpy(buffer+nLen,rsa_key,rsa_length);
  buffer[0]=(nLen-2)/256;
	buffer[1]=(nLen-2)%256;
	if ( 0>CommandProc(buffer,nLen,buffer,&nLen, SM4_TYPE))
	{
		  dcs_log(0,0,"�����ն�����ԿTMK ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(buffer,"A",1)!=0)
	{
		  sprintf(return_code,"%02x",buffer[1+8]);
			dcs_log(buffer,nLen,"�����ն�����ԿTMKʧ��!");
			return -1;
  }
  *out_length=buffer[1+8]*256+buffer[1+8+1];
  memcpy(out_data,buffer+1+8+2,*out_length);
  memcpy(return_code,"00",2);
  return 1;	
}

/*
DES_TO_MD5��
 ��Կԭ����DES�㷨������ͨ���˺�����������������ݺϲ���MD5�㷨�������MD5���
*/
int DES_TO_MD5(char *return_code, char *sek_index, char *key, int in_length, char *in_data, int *out_length, char *out_data)
{
	
}
