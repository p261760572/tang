#include <stdlib.h>
#include "secuSrv.h"
#include "dccdcs.h"
#include "folder.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)

#define DCS_SEC_NAME "effi_sec.conf"

#define DES_TYPE 0x01
#define SM4_TYPE 0x22

static int sgSndQid=-1 ,sgRcvQid=-1;
int SendFailMessage ( int id);
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

int DESDecodeTrack( char *return_code,char *key_index,char *unit,char *indata,int iLen,char *outdata)
{
	char Buffer[1024];
	
	int	nLen ;

	if (return_code==NULL || outdata==NULL )
	{
		dcs_log(0,0,"in DESDecodeTrack:: output parameter is NULL!");
		memcpy(return_code,"95",2);	
		return (-1);
	}

	memset(Buffer,'\0',sizeof(Buffer));
	memcpy(Buffer+2,"S1",2);
	nLen=4;
	memcpy(Buffer+nLen,"0",1);
	nLen++;
	memcpy(Buffer+nLen,key_index,3);
	nLen=nLen+3;
	memcpy(Buffer+nLen,"1",1);
	nLen++;
	memcpy(Buffer+nLen,unit,16);
	nLen=nLen+16;
	sprintf(Buffer+nLen,"%04d",iLen);
	nLen=nLen+4;
	memcpy(Buffer+nLen,indata,iLen);
	nLen=nLen+iLen;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"����ŵ���Ϣ ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"����ŵ���Ϣʧ��!");
		    return -1;
  }
	bcd_to_asc(outdata,Buffer+2,(nLen-2)*2,0);
	memcpy(return_code,Buffer,2);
//	dcs_debug(0,0," des track ��������ɹ� len��%d",nLen-2);
	return (nLen-2)*2;
}


int DESDestory()
{
	
}

/***
Function 

  ����:���ն˴�����Ϣ����


**/



int SendFailMessage ( int id)
{
	int fid;
	char caBuf[20];
/*  if (fold_initsys() < 0)
  {
          dcs_log(0,0," cannot attach to FOLDER !");

          exit(0);
  }
*/
	fid = fold_locate_folder("SECSRV");
	if  ( fid <0)
	{
			dcs_log(0,0,"can not locate folder [SECSRV]");
				return 0;
	}
			
	sprintf(caBuf,"STAR%02d",id);		
 	fold_write(fid ,-1,caBuf,6);		
 
 	return 1;
	
}

/***   56�� ���ܻ������� ***/

//��ȡ������Կ
int DESGETKY(char *return_code, char *key_data, char *key_index)
{
	
	char Buffer[1024];
	
	int	nLen ;

	if (return_code==NULL || key_data==NULL )
	{
		dcs_log(0,0,"in DESGETKY:: output parameter is NULL!");
		memcpy(return_code,"95",2);	
		return (-1);
	}

	memset(Buffer,'\0',sizeof(Buffer));
	memcpy(Buffer+2,"1A",2);
	nLen=4;
	memcpy(Buffer+4,key_index,3);
	nLen=nLen+3;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"����������Կ ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"����������Կʧ��!");
		    return -1;
  }
	memcpy(key_data,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}

int DESMACOP(char *return_code, char *mac_value, char *key_index, char *key_code, int length, char *data)
{
	
	char	Buffer[1024];
	int	i,s;

	if (return_code==NULL || mac_value==NULL)
	{
		dcs_log(0,0,"in DESMACOP:: output parameter is NULL!");
		memcpy(return_code,"95",2);
		return (-1);
	}

	
	if (length<=0 || length>1024)
	{
		dcs_log(0,0,"in DESMACOP:: mac data length [%d] error!",length);
		memcpy(return_code,"15",2);
		return (-1);
	}

	memset(Buffer,0,sizeof(Buffer));
	memcpy(Buffer+2,"80",2);
	i=4;
	memcpy(Buffer+i,key_index,3);
	i= i+3;
	memcpy(Buffer+i,key_code,16);
	i=i+16;
	s=length%8;
	if(s)
	{ 
		 
		 memset(data+length,0,(8-s));
		 length=length+(8-s);
  }
	sprintf(Buffer+i,"%03d",length);
	i=i+3;
	memcpy(Buffer+i,data,length);
	i= i+length;
	Buffer[0]=(i-2)/256;
	Buffer[1]=(i-2)%256;
//	dcs_debug(data,length,"����mac data len=%d",length);
	if ( 0>CommandProc(Buffer,i,Buffer,&i, DES_TYPE))
	{
		  dcs_log(0,0,"����MACʧ��!");
		  return -1;
	}
	if (memcmp(Buffer,"00",2)!=0)
	{
				memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,i,"����MACʧ��!������[%2.2s]",Buffer);
		    return -1;
  }
	memcpy(mac_value,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}


int DESPINTR(char *return_code, char *out_data, char *key_index1, char *key_code1,
             char *key_index2, char *key_code2, char *in_data, char *pan1, char *pan2)
{

  char Buffer[1024];
	int	nLen,i;


	if (return_code==NULL || out_data==NULL)
	{
		dcs_log(0,0,"in DESPINTR:: output parameter is NULL!");
		memcpy(return_code,"95",2);
		return (-1);
	}

	
	memset(Buffer,0,sizeof(Buffer));
	memcpy(Buffer+2,"69",2);
	i=4;
	memcpy(Buffer+i,key_index1,3);
	i=i+3;
	memcpy(Buffer+i,key_code1,16);
	i=i+16;
	memcpy(Buffer+i,key_index2,3);
	i=i+3;
	memcpy(Buffer+i,key_code2,16);
	i=i+16;
//	memcpy(Buffer+i,in_data,16);
    bcd_to_asc(Buffer+i,in_data,16,0);
	i=i+16;
	memcpy(Buffer+i,pan1,16);
	i=i+16;
	memcpy(Buffer+i,pan2,16);
	i=i+16;
	
	Buffer[0]=(i-2)/256;
	Buffer[1]=(i-2)%256;
  if ( 0>CommandProc(Buffer,i,Buffer,&i, DES_TYPE))
	{
		  dcs_log(0,0,"PIN ת�� ͨ��ʧ��!");
		  return -1;
	}
	if (memcmp(Buffer,"00",2)!=0)
	{
		dcs_log(0,0,"pin ת������ʧ�� response:*%2.2s*",Buffer);
		
		memcpy(return_code,Buffer,2);
		dcs_log(0,0,"index1[%3.3s]pinkey1[%16.16s]\n index2[%3.3s]pinkey2[%16.16s]\n pindata[%16.16s]\n pan1[%16.16s]pan2[%16.16s]",
		        key_index1,key_code1,key_index2,key_code2,in_data,pan1,pan2);
		return (-1);
	}

			
	asc_to_bcd(out_data,Buffer+2,16,0);
	memcpy(return_code,Buffer,2);
	return (0);
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
  *outLen=*outLen -sizeof(long)-1-sizeof(time_t)-4;
  dcs_debug(msg.msgtext+1+sizeof(time_t)+2 , *outLen , "recv msg from queue succ!");
  memcpy(outBuf, msg.msgtext + 1+sizeof(time_t)+4, *outLen );
  
  return 1;
}

/*
 * �����������
 *
 */
void SetLenField(char *buf, int len){
    *(buf) = len / 256;
    *(buf + 1) = len % 256;
}

/*
 * ����MAC
 *
 * ע��:��ɢ�����Ѿ�д��,����disTimes=3,��ɢ�������ⲿ����,
 *     ��ɢ���ӳ���Ӧ����disTimes * 16
 *
 */
int DESCalcTermMac(char *mackey_idx, char *key, char *srcBuf, int iLen, char *cMac, int sjflag){
    char cmd[2048] = {'\0'};
    char data[2048] = {"\0"};
    char result[2048] = {"\0"};

    int  idx = 0, i;
    int  resultLen = 0;
    const int macLen = 8; /* Mac����*/
    const int disTimes = 2; /* ��ɢ����*/
    const int disDataLen = 16*disTimes; /* ��ɢ���ݳ���*/
    
    char tmpbuf[512] = {'\0'};
    char randomData[17] = {'\0'};
    char samcarno[17] = {'\0'};
 		char discunt[33]; /*��ɢ����*/
 		
    dcs_log(0, 0, "DESCalcTermMac");
    
    asc_to_bcd((unsigned char *)randomData, (unsigned char *)key, 16, 0); 
    asc_to_bcd((unsigned char *)samcarno, (unsigned char *)key+16, 16, 0);		
				
		for(i=0; i<8; i++ )
		{
			tmpbuf[i]=randomData[i]^samcarno[i];
		}
		
		memset(discunt,0,sizeof(discunt));/*liuqing 2007/12/22/ �������PSAM���ŵ��������Ϊ��һ����ɢ����*/
		bcd_to_asc((unsigned char *)discunt, (unsigned char *)tmpbuf, 16);
		memcpy(discunt+16,key+16,16);
		
    /* ת��ΪASCII��ʽ*/
    bcd_to_asc((unsigned char *)data, (unsigned char *)srcBuf, iLen * 2);
    /*�������*/
    idx += 2; /* ��Ϣ����+2*/
    memcpy(cmd + idx, "95", 2); /* ����/2/A/ֵ"95" */
    idx += 2;
    *(cmd + idx) = '3'; /* MAC���ݲ�λ/1/N/1:��0X80 00 00 .... 2:ǿ�ƺ�0X80 00 00.... 3:��0X00 00 ...*/
    idx += 1;
    *(cmd + idx) = '2'; /* ������Կ����/1/N/1:ֱ�ӷ�ɢ�õ�(3DES��Կ);2:��ɢ��ȡ��8�ֽ�(DES��Կ)*/
    idx += 1;
    memcpy(cmd + idx, mackey_idx, 3); /* ������Կ����/3/N/��������Կ������*/
    idx += 3;
    *(cmd + idx) = disTimes + '0'; /* ��ɢ����/1/N/��ɢ���ӵ�����(���5��)*/
    idx += 1;
    memcpy(cmd + idx, discunt, disDataLen); /* ��ɢ����/16*N/H/��ɢ����*/
    idx += disDataLen;
    snprintf(cmd + idx, 5, "%04d", iLen); /* MAC���ݳ���/4/N/����8�ֽڳ�ʼ����(�ⲿ����)*/
    idx += 4;
    memcpy(cmd + idx, data, iLen * 2); /* MAC����/N*2/H*/
    idx += iLen * 2;

    SetLenField(cmd, idx - 2);

    /* ִ������*/
    if ( 0 > CommandProc(cmd, idx, result, &resultLen, DES_TYPE)) {
        dcs_log(0, 0, "����MACʧ��!");
        return -1;
    }
    /* �жϽ��*/
    if (memcmp(result, "00", 2)!=0) {
        dcs_log(0, 0, "����MACʧ��!������[%2.2s]", result);
        return -1;
	}
    asc_to_bcd((unsigned char *)cMac, (unsigned char *)result + 2, macLen * 2, 0); /* MACת��ΪBCD*/

    return 0;
}

/*
 * For TEST ADD 09/05
 *
 */
int DESTrackCoding(char *keyIndex, char *key, char *buf, int len, int * outlen){
    char cmd[2048] = {'\0'};
    char data[2048] = {'\0'};
    char result[2048] = {'\0'};

    int idx = 0,i;
    int resultLen = 0;
    const int disTimes = 2; /* ��ɢ����*/
    const int disDataLen = 16*disTimes;  /* ��ɢ���ݳ���*/

		char tmpbuf[512] = {'\0'};
    char randomData[17] = {'\0'};
    char samcarno[17] = {'\0'};
 		char discunt[33]; /*��ɢ����*/
 		
    dcs_log(0, 0, "DESTrackCoding");
    
		asc_to_bcd((unsigned char *)randomData, (unsigned char *)key, 16, 0); 
    asc_to_bcd((unsigned char *)samcarno, (unsigned char *)key+16, 16, 0);		
				
		for(i=0; i<8; i++ )
		{
			tmpbuf[i]=randomData[i]^samcarno[i];
		}
		
		memset(discunt,0,sizeof(discunt));/*liuqing 2007/12/22/ �������PSAM���ŵ��������Ϊ��һ����ɢ����*/
		bcd_to_asc((unsigned char *)discunt, (unsigned char *)tmpbuf, 16);
		memcpy(discunt+16,key+32,16);
		
    bcd_to_asc((unsigned char *)data, (unsigned char *)buf, len * 2, 0); /* ת��ΪASC��ʽ*/
    //�������
    idx += 2;
    memcpy(cmd + idx, "94", 2); /* ����/2/A/ֵ"94" */
    idx += 2;
    *(cmd + idx) = '2'; /* ��/���ܱ�־/1/N/1:����;2:����*/
    idx += 1;
    *(cmd + idx) = '1'; /* ��/�����㷨/1/N/1:ECB;2:CBC*/
    idx += 1;
    *(cmd + idx) = '2'; /* ������Կ����/1/N/1:ֱ�ӷ�ɢ�õ�(3DES��Կ);2:��ɢ��ȡ��8�ֽ�(DES��Կ)*/
    idx += 1;
    memcpy(cmd + idx, keyIndex, 3); /* ������Կ����/3/N/��������Կ������*/
    idx += 3;
    *(cmd + idx) = disTimes + '0'; /* ��ɢ����/1/N/��ɢ���ӵ�����(���5��)*/
    idx += 1;
    memcpy(cmd + idx, discunt, disDataLen); /* ��ɢ����/16*N/H/��ɢ����*/
    idx += disDataLen;
    snprintf(cmd + idx, 5, "%04d", len); /* �������ݿ鳤��/4/N/����Ϊ8�ı���*/
    idx += 4;
    memcpy(cmd + idx, data, len * 2);
    idx += len * 2;
    
    SetLenField(cmd, idx - 2);

    /* ִ������*/
    if ( 0 > CommandProc(cmd, idx, result, &resultLen, DES_TYPE)) {
        dcs_log(0, 0, "���ܴŵ�ʧ��!");
        return -1;
    }
    /* �жϽ��*/
    if (memcmp(result, "00", 2)!=0) {
        dcs_log(0, 0, "���ܴŵ�ʧ��!������[%2.2s]", result);
        return -1;
	}
    memcpy(buf, result + 2, resultLen - 2); /* 2Ϊ������ĳ���*/
    *outlen = resultLen - 2;
    
    return 0;
}

/*
 * For TEST ADD 09/05
 *
 */
int DESPinConver(char *keyidx, char *key, char *pinBuf, char *pinkey_idx, char *pinkey, char *pan, char *buf){
    char cmd[2048] = {'\0'};
    char data[2048] = {'\0'};
    char result[2048] = {'\0'};

    int idx = 0,i;
    int resultLen = 0;
    const int disTimes = 2; /* ��ɢ����*/
    const int disDataLen = 16*disTimes; /* ��ɢ���ݳ���*/
    const int pinKeyLen = 16; /* pin key����*/
    const int pinLen = 16;
    const int panLen = 16; /* pan����*/
    
    char tmpbuf[512] = {'\0'};
    char randomData[17] = {'\0'};
    char samcarno[17] = {'\0'};
 		char discunt[33]; /*��ɢ����*/
 		
    dcs_log(0, 0, "DESPinConver");
    
    asc_to_bcd((unsigned char *)randomData, (unsigned char *)key, 16, 0); 
    asc_to_bcd((unsigned char *)samcarno, (unsigned char *)key+16, 16, 0);		
				
		for(i=0; i<8; i++ )
		{
			tmpbuf[i]=randomData[i]^samcarno[i];
		}
		
		memset(discunt,0,sizeof(discunt));/*liuqing 2007/12/22/ �������PSAM���ŵ��������Ϊ��һ����ɢ����*/
		bcd_to_asc((unsigned char *)discunt, (unsigned char *)tmpbuf, 16);
		memcpy(discunt+16,key+32,16);
		
	bcd_to_asc((unsigned char *)data, (unsigned char *)pinBuf, pinLen, 0); /* ת��ΪASC��ʽ*/
    //�������
    idx += 2; /* ��Ϣ����+2*/
    memcpy(cmd + idx, "96", 2); /* ����/2/A/ֵ"96" */
    idx += 2;
    *(cmd + idx) = '1'; /* ������Կ����/1/N/1:ֱ�ӷ�ɢ�õ�(3DES��Կ);2:��ɢ��ȡ��8�ֽ�(DES��Կ)*/
    idx += 1;
    memcpy(cmd + idx, keyidx, 3); /* ������Կ����1/3/N/Դ��������Կ������*/
    idx += 3;
    memcpy(cmd + idx, pinkey_idx, 3); /* ������Կ����2/3/N/Ŀ����������Կ������*/
    idx += 3;
    *(cmd + idx) = disTimes + '0'; /* ��ɢ����/1/N/��ɢ���ӵ�����(���5��)*/
    idx += 1;
    memcpy(cmd + idx, discunt, disDataLen); /* ��ɢ����/16*N/H/��ɢ����*/
    idx += disDataLen;
    *(cmd + idx) = 'X'; /* Ŀ��PIK��־/1/A/��ѡ:X���޴���λ,64bit;Y,128bit;Z,192bit*/
	idx += 1;
	memcpy(cmd + idx, pinkey, pinKeyLen); /* Ŀ��PIK/16/32/48/H/������Կ����2����*/
	idx += pinKeyLen;
	memcpy(cmd + idx, data, pinLen); /*  PIN����/16/H/����PIN����(ANSI9.8PIN��ʽ)*/
	idx += pinLen;
	memcpy(cmd + idx, pan, panLen); /*������Ϣ/16/H/������Ϣ(�˺�)*/
	idx += panLen;

    SetLenField(cmd, idx - 2);
    /* ִ������*/
    if ( 0 > CommandProc(cmd, idx, result, &resultLen, DES_TYPE)) {
        dcs_log(0, 0, "���ܴŵ�ʧ��!");
        return -1;
    }
    /* �жϽ��*/
    if (memcmp(result, "00", 2)!=0) {
        dcs_log(0, 0, "���ܴŵ�ʧ��!������[%2.2s]", result);
        return -1;
	}
/*    asc_to_bcd((unsigned char *)buf, (unsigned char *)result + 2, resultLen - 2, 0);  2Ϊ������ĳ���*/
    memcpy(buf, result + 2, resultLen - 2);

    return 0;
}

/*
 * For TEST ADD 09/05
 *
 */
int Sj_3DES_crypt(char * p1, char * key, int i1, char *buf, int *len){
    dcs_log(0, 0, "Sj_3DES_crypt");
    return 0;
}

/*
 * ȡ�����
 *
 */
int GetRandomData(char *randomData) {
    sprintf(randomData, "%08X%08X", rand(), rand());
    return 0;
}



//�����ն�����ԿTMK��K0/K1��
//����	�����ն�����Կ�����������ĺͼ��ֵ���ظ�������
//˵��	���ɵ��ն�����Կ������У�顣
//��ڲ���
//sek_index:SEK����,S��4λ����
//tek_index:TEK����,T��4λ����
//���ڲ���
//return_code:������
//sek_tmk_data:TMK����1,��SEK����
//tek_tmk_data:TMK����2,��TEK����
int DESGETTMK(char *return_code, char *sek_index, char *tek_index, char *sek_tmk_data, char *tek_tmk_data)
{
	char Buffer[1024];
	char CheckValue[17];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(CheckValue,'\0',sizeof(CheckValue));
	nLen=2;
	memcpy(Buffer+nLen,"K0",2);nLen=nLen+2;
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	memcpy(Buffer+nLen,tek_index,5);nLen=nLen+5;
	memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"�����ն�����ԿTMK ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"�����ն�����ԿTMKʧ��!");
		    return -1;
  }
	memcpy(sek_tmk_data,Buffer+2,16);
	memcpy(tek_tmk_data,Buffer+2+16,16);
	memcpy(CheckValue,Buffer+2+16+16,16);
	memcpy(return_code,Buffer,2);
	return (1);
}

//�����ն�����ԿTMK_128��K0/K1��
//����	�����ն�����Կ�����������ĺͼ��ֵ���ظ�������
//˵��	���ɵ��ն�����Կ������У�顣
//��ڲ���
//sek_index:SEK����,S��4λ����
//tek_index:TEK����,T��4λ����
//���ڲ���
//return_code:������
//sek_tmk_data:TMK����1,��SEK����
//tek_tmk_data:TMK����2,��TEK����
int DESGETTMK2(char *return_code, char *sek_index, char *tek_index, char *sek_tmk_data, char *tek_tmk_data)
{
	char Buffer[1024];
	char CheckValue[17];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(CheckValue,'\0',sizeof(CheckValue));
	nLen=2;
	memcpy(Buffer+nLen,"K0",2);nLen=nLen+2;
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	memcpy(Buffer+nLen,tek_index,5);nLen=nLen+5;
	memcpy(Buffer+nLen,"Y",1);nLen=nLen+1;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"�����ն�����ԿTMK ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"�����ն�����ԿTMKʧ��!");
		    return -1;
  }
	memcpy(sek_tmk_data,Buffer+2,32);
	memcpy(tek_tmk_data,Buffer+2+32,32);
	memcpy(CheckValue,Buffer+2+32+32,16);
	memcpy(return_code,Buffer,2);
	return (1);
}

//����������ԿPIK/MAK��K2/K3��
//����	����������ԿPIK/MAK�����������ĺͼ��ֵ���ظ�������
//˵��	�������ɵ�������Կ��PIK��MAK����Ҫͬʱ�������������ģ�����һ����TMK��������ͨ���������ķ�ʽ��ǩ��Ӧ����Ϣ�д����նˣ���һ����SEK���ܺ󣬽����ı��������ݿ��С�
//��ڲ���
//sek_index1:SEK1����,���ڽ���TMK
//sek_index2:SEK2����,���ڼ���PIK/MAK
//tmk:TMK����,��SEK1����
//���ڲ���
//return_code:������
//sek_pikmak_data:PIK/MAK����1,��SEK2����
//tmk_pikmak_data:PIK/MAK����2,��TMK����
int DESTMKGETPIKMAK(char *return_code, char *sek_index1, char *sek_index2, char *tmk, char *sek_pikmak_data, char *tmk_pikmak_data) 
{
	char Buffer[1024];
	char CheckValue[1024];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(CheckValue,'\0',sizeof(CheckValue));
	nLen=2;
	memcpy(Buffer+nLen,"K2",2);nLen=nLen+2;
	memcpy(Buffer+nLen,sek_index1,5);nLen=nLen+5;
	memcpy(Buffer+nLen,sek_index2,5);nLen=nLen+5;
	memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
	memcpy(Buffer+nLen,tmk,16);nLen=nLen+16;
	memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"�����ն�����ԿTMK ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"�����ն�����ԿTMKʧ��!");
		    return -1;
  }
	memcpy(sek_pikmak_data,Buffer+2,16);
	memcpy(tmk_pikmak_data,Buffer+2+16,16);
	memcpy(CheckValue,Buffer+2+16+16,16);
	memcpy(return_code,Buffer,2);
	return (1);
}
//����������ԿPIK/MAK��K2/K3��
//����	����������ԿPIK/MAK�����������ĺͼ��ֵ���ظ�������
//˵��	�������ɵ�������Կ��PIK��MAK����Ҫͬʱ�������������ģ�����һ����TMK��������ͨ���������ķ�ʽ��ǩ��Ӧ����Ϣ�д����նˣ���һ����SEK���ܺ󣬽����ı��������ݿ��С�
//��ڲ���
//sek_index1:SEK1����,���ڽ���TMK
//sek_index2:SEK2����,���ڼ���PIK/MAK
//tmk:TMK����,��SEK1����
//���ڲ���
//return_code:������
//sek_pikmak_data:PIK/MAK����1,��SEK2����
//tmk_pikmak_data:PIK/MAK����2,��TMK����
int DESTMKGETPIKMAK2(char *return_code, char *sek_index1, char *sek_index2, char *tmk, char *sek_pikmak_data, char *tmk_pikmak_data, char *CheckValue, char *TMkOp, char *PIKMAKOp) 
{
	char Buffer[1024];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(CheckValue,'\0',sizeof(CheckValue));
	nLen=2;
	memcpy(Buffer+nLen,"K2",2);nLen=nLen+2;
	memcpy(Buffer+nLen,sek_index1,5);nLen=nLen+5;
	memcpy(Buffer+nLen,sek_index2,5);nLen=nLen+5;
	memcpy(Buffer+nLen,TMkOp,1);nLen=nLen+1;
	if (TMkOp[0] == 'Y')
	{
		memcpy(Buffer+nLen,tmk,32);nLen=nLen+32;
	}
	else if (TMkOp[0] == 'X')
	{
		memcpy(Buffer+nLen,tmk,16);nLen=nLen+16;
	}
	else if (TMkOp[0] == 'Z')
	{
		memcpy(Buffer+nLen,tmk,48);nLen=nLen+48;
	}
	memcpy(Buffer+nLen,PIKMAKOp,1);nLen=nLen+1;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"�����ն�����ԿTMK ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"�����ն�����ԿTMKʧ��!");
		    return -1;
  }
	if (PIKMAKOp[0] == 'Y')
	{
		memcpy(sek_pikmak_data,Buffer+2,32);
		memcpy(tmk_pikmak_data,Buffer+2+32,32);
		memcpy(CheckValue,Buffer+2+32+32,16);
	}
	else if (PIKMAKOp[0] == 'X')
	{
		memcpy(sek_pikmak_data,Buffer+2,16);
		memcpy(tmk_pikmak_data,Buffer+2+16,16);
		memcpy(CheckValue,Buffer+2+16+16,16);
	}
	else if (PIKMAKOp[0] == 'Z')
	{
		memcpy(sek_pikmak_data,Buffer+2,48);
		memcpy(tmk_pikmak_data,Buffer+2+48,48);
		memcpy(CheckValue,Buffer+2+48+48,16);
	}
	memcpy(return_code,Buffer,2);
	return (1);
}

//PIN BLOCKת����P0/P1��
//����	��ԴPIN������ԴPIK���ܣ�����PIN��ʽת����Ȼ����Ŀ��PIK���������
//˵��	��PinBlock��ʽΪ02ʱ��PIN����Ϊ4~6λ����PinBlock��ʽΪ04ʱ��PIN����Ϊ6λ������PinBlock��ʽPIN��������4λ���12λ����ָ����PinBlock��ʽΪ01ʱ��Ҫ������16λ�ʺţ�Ŀ����Ϊ�˰���ĳЩ�����PinBlock��ʽ���ϲ�Ӧ����Ҫ�����Լ���Ҫ����֯����HSMֱ���ô����ʺ�����PIN������γ�PinBlock��
//��ڲ���
//sek_index1:ԴSEK����
//sek_index2:Ŀ��SEK����
//pik1:ԴPIK��Կ����,��ԴSEK����
//pik2:Ŀ��PIK��Կ����,��Ŀ��SEK����
//in_pin_data:ԴPIN PinBlock����,��ԴPIK����
//���ڲ���
//return_code:������
//out_pin_dat:Ŀ��PinBlock����,��Ŀ��PIK����
int DESTRANSPIN(char *return_code, char *sek_index1, char *sek_index2, char *pik1, char *pik2, char *in_pin_data, char *out_pin_dat)
{
	char Buffer[1024];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	nLen=2;
	memcpy(Buffer+nLen,"P0",2);nLen=nLen+2;
	memcpy(Buffer+nLen,sek_index1,5);nLen=nLen+5;
	memcpy(Buffer+nLen,sek_index2,5);nLen=nLen+5;
	memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
	memcpy(Buffer+nLen,pik1,16);nLen=nLen+16;
	memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
	memcpy(Buffer+nLen,pik2,16);nLen=nLen+16;
	memcpy(Buffer+nLen,"06",2);nLen=nLen+2;
	memcpy(Buffer+nLen,"06",2);nLen=nLen+2;
	memcpy(Buffer+nLen,in_pin_data,16);nLen=nLen+16;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
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
	memcpy(out_pin_dat,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}
//PIN BLOCKת����P0/P1��
//����	��ԴPIN������ԴPIK���ܣ�����PIN��ʽת����Ȼ����Ŀ��PIK���������
//˵��	��PinBlock��ʽΪ02ʱ��PIN����Ϊ4~6λ����PinBlock��ʽΪ04ʱ��PIN����Ϊ6λ������PinBlock��ʽPIN��������4λ���12λ����ָ����PinBlock��ʽΪ01ʱ��Ҫ������16λ�ʺţ�Ŀ����Ϊ�˰���ĳЩ�����PinBlock��ʽ���ϲ�Ӧ����Ҫ�����Լ���Ҫ����֯����HSMֱ���ô����ʺ�����PIN������γ�PinBlock��
//��ڲ���
//sek_index1:ԴSEK����
//sek_index2:Ŀ��SEK����
//pik1:ԴPIK��Կ����,��ԴSEK����
//pik2:Ŀ��PIK��Կ����,��Ŀ��SEK����
//in_pin_data:ԴPIN PinBlock����,��ԴPIK����
//���ڲ���
//return_code:������
//out_pin_dat:Ŀ��PinBlock����,��Ŀ��PIK����
int DESTRANSPIN2(char *return_code, char *sek_index1, char *sek_index2, char *pik1, char *pik2, char *in_pin_data, char *in_pan_data, char *out_pin_dat, char *out_pan_data)
{
	char Buffer[1024];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	nLen=2;
	memcpy(Buffer+nLen,"P0",2);nLen=nLen+2;
	memcpy(Buffer+nLen,sek_index1,5);nLen=nLen+5;
	memcpy(Buffer+nLen,sek_index2,5);nLen=nLen+5;
	memcpy(Buffer+nLen,"Y",1);nLen=nLen+1;
	memcpy(Buffer+nLen,pik1,32);nLen=nLen+32;
	memcpy(Buffer+nLen,"Y",1);nLen=nLen+1;
	memcpy(Buffer+nLen,pik2,32);nLen=nLen+32;
	memcpy(Buffer+nLen,"01",2);nLen=nLen+2;
	memcpy(Buffer+nLen,"01",2);nLen=nLen+2;
	memcpy(Buffer+nLen,in_pin_data,16);nLen=nLen+16;
	memcpy(Buffer+nLen,in_pan_data,16);nLen=nLen+16;
	memcpy(Buffer+nLen,out_pan_data,16);nLen=nLen+16;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
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
	memcpy(out_pin_dat,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}


//MACУ��
//��ڲ���
//sek_index:SEK����
//mak:MAK��Կ����,��SEK����
//length:���ݳ���
//data:����
//���ڲ���
//return_code:������
//mac_value:MACֵ
//����ֵ 1�ɹ��� -1ʧ��
int DESCPOSMAC2(char *return_code, char *sek_index, char *mak, int length, char *data, char *mac_value)
{
	char xBuf[9],xAscBuf[17],sendBuf[50];
	int nLen, i, j;
	if(length%8 != 0) 
	{
		memcpy(return_code, "24", 2);
		return -1;
	}
	memset(xBuf, 0, sizeof(xBuf));
	for(i = 0; i < length;)
	{
		for(j = 0; j < 8; j++)
		{
			xBuf[i] ^= data[i++];
		}
	}
	bcd_to_asc(xAscBuf, xBuf, 16);
	memset(sendBuf, 0, sizeof(sendBuf));
	nLen = 2;
	memcpy(sendBuf + nLen, "M02", 3); nLen += 3;
	memcpy(sendBuf + nLen, sek_index, 5); nLen += 5;
	memcpy(sendBuf + nLen, "X", 1); nLen += 1;
	memcpy(sendBuf + nLen, mak, 16); nLen += 16;
	memcpy(sendBuf + nLen, "0008", 4); nLen += 4;
	memcpy(sendBuf + nLen, xAscBuf, 8); nLen += 8;
	sendBuf[0]=(nLen-2)/256;
	sendBuf[1]=(nLen-2)%256;
	if ( 0>CommandProc(sendBuf, nLen, sendBuf,&nLen, DES_TYPE))
	{
		dcs_log(0, 0, "MAC���� ͨ��ʧ��!");
		memcpy(return_code, "91", 2);
		return -1;
	}
	if ( memcmp(sendBuf,"00",2)!=0)
	{
		memcpy(return_code, sendBuf,2);
		dcs_log(sendBuf, nLen, "MAC����ʧ��!");
		return -1;
  	}
	for(i = 0; i < 8; i++)
	{
		xAscBuf[8 + i] ^= sendBuf[2 + i];
	}
	memset(sendBuf, 0, sizeof(sendBuf));
	nLen = 2;
	memcpy(sendBuf + nLen, "M02", 3); nLen += 3;
	memcpy(sendBuf + nLen, sek_index, 5); nLen += 5;
	memcpy(sendBuf + nLen, "X", 1); nLen += 1;
	memcpy(sendBuf + nLen, mak, 16); nLen += 16;
	memcpy(sendBuf + nLen, "0008", 4); nLen += 4;
	memcpy(sendBuf + nLen, xAscBuf+8, 8); nLen += 8;
	sendBuf[0]=(nLen-2)/256;
	sendBuf[1]=(nLen-2)%256;
	if ( 0>CommandProc(sendBuf, nLen, sendBuf,&nLen, DES_TYPE))
	{
		dcs_log(0, 0, "MAC���� ͨ��ʧ��!");
		memcpy(return_code, "91", 2);
		return -1;
	}
	if ( memcmp(sendBuf,"00",2)!=0)
	{
		memcpy(return_code, sendBuf, 2);
		dcs_log(sendBuf, nLen, "MAC����ʧ��!");
		return -1;
  	}
	bcd_to_asc(mac_value, sendBuf+2, 8);
	memcpy(return_code, "00", 2);
	return 1;
}

//MAC���㣨M0/M1��
//��ڲ���
//sek_index:SEK����
//mak:MAK��Կ����,��SEK����
//length:���ݳ���
//data:����
//���ڲ���
//return_code:������
//mac_value:MACֵ
//����ֵ 1�ɹ��� -1ʧ��
int DESCALCMAC(char *return_code, char *sek_index, char *mak, int length, char *data, char *mac_value)
{
	char Buffer[2048];
	char datalen[5];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(datalen,'\0',sizeof(datalen));
	nLen=2;
	memcpy(Buffer+nLen,"M0",2);nLen=nLen+2;
	memcpy(Buffer+nLen,"2",1);nLen=nLen+1;
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	rtrim(mak);
	if( strlen(mak)== 16)
	{
		 memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
		 memcpy(Buffer+nLen,mak,16);nLen=nLen+16;
	}
	else if( strlen(mak)== 32)
	{
		memcpy(Buffer+nLen,"Y",1);nLen=nLen+1;
		memcpy(Buffer+nLen,mak,32);nLen=nLen+32;
	}
	else
	{
	  	dcs_log(0,0,"MAC key length error [%d-%s]!",strlen(mak), mak);
		return -1;
	}
	sprintf(datalen,"%04d",length);
	memcpy(Buffer+nLen,datalen,4);nLen=nLen+4;
	memcpy(Buffer+nLen,data,length);nLen=nLen+length;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
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
	memcpy(mac_value,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}


int DES3CALCMAC(char *return_code, char *sek_index, char *mak, int length, char *data, char *mac_value)
{
	char Buffer[2048];
	char datalen[5];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(datalen,'\0',sizeof(datalen));
	nLen=2;
	memcpy(Buffer+nLen,"M0",2);nLen=nLen+2;
	memcpy(Buffer+nLen,"2",1);nLen=nLen+1;
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	memcpy(Buffer+nLen,"Y",1);nLen=nLen+1;
	memcpy(Buffer+nLen,mak,32);nLen=nLen+32;
	sprintf(datalen,"%04d",length);
	memcpy(Buffer+nLen,datalen,4);nLen=nLen+4;
	memcpy(Buffer+nLen,data,length);nLen=nLen+length;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"MAC����ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"MAC����ʧ��!");
		    return -1;
  }
	memcpy(mac_value,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}

//��֤MAC��M2/M3��
//��ڲ���
//sek_index:SEK����
//mak:MAK��Կ����,��SEK����
//mac_value:MACֵ
//length:���ݳ���
//data:����
//���ڲ���
//return_code:������
int DESCHECKMAC(char *return_code, char *sek_index, char *mak, char *mac_value, int length, char *data)
{
	char Buffer[1024];
	char datalen[5];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(datalen,'\0',sizeof(datalen));
	nLen=2;
	memcpy(Buffer+nLen,"M2",2);nLen=nLen+2;
	memcpy(Buffer+nLen,"2",1);nLen=nLen+1;
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
	memcpy(Buffer+nLen,mak,16);nLen=nLen+16;
	memcpy(Buffer+nLen,mac_value,8);nLen=nLen+8;
	sprintf(datalen,"%04d",length);
	memcpy(Buffer+nLen,datalen,4);nLen=nLen+4;
	memcpy(Buffer+nLen,data,length);nLen=nLen+length;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"��֤MAC ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"��֤MACʧ��!");
		    return -1;
  }
	
	memcpy(return_code,Buffer,2);
	return (1);
}

//SEK��TEK֮��ת��TMK��KE/KF��
//����	����SEK���ܵ�TMKת��Ϊ��TEK���ܣ����߽���TEK���ܵ�TMKת��Ϊ��SEK���ܡ�
//��ڲ���
//sek_index:SEK��Կ����
//tek_index:TEK��Կ����
//transbz:ת����־(0����SEK��TEK����,1����TEK��SEK����)
//in_tmk:ԴTMK��Կ����
//���ڲ���
//return_code:������
//out_tmk:Ŀ��TMK��Կ����
int DESTRANSTMK(char *return_code, char *sek_index, char *tek_index, char *transbz, char *in_tmk, char *out_tmk)
{
	char Buffer[1024];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	nLen=2;
	memcpy(Buffer+nLen,"KE",2);nLen=nLen+2;
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	memcpy(Buffer+nLen,tek_index,5);nLen=nLen+5;
	memcpy(Buffer+nLen,transbz,1);nLen=nLen+1;
	memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
	memcpy(Buffer+nLen,in_tmk,16);nLen=nLen+16;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"TMK ת�� ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"TMK ת��ʧ��!");
		    return -1;
  }
	memcpy(out_tmk,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}

//SEK��TMK֮��ת��PIK/MAK��KI/KJ��
//����	����SEK���ܵ�PIK/MAKת��Ϊ��TMK���ܣ����߽���TMK���ܵ�PIK/MAKת��Ϊ��SEK���ܡ�
//��ڲ���
//sek_index1:SEK��Կ����,���ڽ���PIK/MAK
//sek_index2:���ڽ���TMK
//tmk:TMK��Կ����
//transbz:ת����־(0����SEK��TMK����,1����TMK��SEK����)
//in_pikmak:ԴPIK/MAK��Կ����
//���ڲ���
//return_code:������
//out_pikmak:Ŀ��PIK/MAK��Կ����
int DESTRANSPIKMAK(char *return_code, char *sek_index1, char *sek_index2, char *tmk, char *transbz, char *in_pikmak, char *out_pikmak)
{
	char Buffer[1024];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	nLen=2;
	memcpy(Buffer+nLen,"KI",2);nLen=nLen+2;
	memcpy(Buffer+nLen,sek_index1,5);nLen=nLen+5;
	memcpy(Buffer+nLen,sek_index2,5);nLen=nLen+5;
	memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
	memcpy(Buffer+nLen,tmk,16);nLen=nLen+16;
	memcpy(Buffer+nLen,transbz,1);nLen=nLen+1;
	memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
	memcpy(Buffer+nLen,in_pikmak,16);nLen=nLen+16;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"PIKMAK ת�� ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"PIKMAK ת��ʧ��!");
		    return -1;
  }
	memcpy(out_pikmak,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}

//�ŵ����ܣ�E1/E2��
//��ڲ���
//sek_index:SEK����
//tdk:tdk��Կ����,��SEK����
//length:���ݳ���
//data:�ŵ���������
//���ڲ���
//return_code:������
//int out_length:���ݳ���
//out_data:�ŵ���������
int DecTrackPrg(char *return_code, char *sek_index, char *tdk, int length, char *data, int *out_length, char *out_data)
{
	char Buffer[1024];
	char datalen[5];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(datalen,'\0',sizeof(datalen));
	nLen=2;
	memcpy(Buffer+nLen,"E1",2);nLen=nLen+2;
	memcpy(Buffer+nLen,"1",1);nLen=nLen+1;//�ӽ��ܱ�־(0:����,1:����)
	memcpy(Buffer+nLen,"0",1);nLen=nLen+1;//�ӽ����㷨(0:ECB,1:CBC)
//	memcpy(Buffer+nLen,"S",1);nLen=nLen+1;//��Կ����
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	rtrim(tdk);
	if( strlen(tdk)== 16)
	{
		 memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
		 memcpy(Buffer+nLen,tdk,16);nLen=nLen+16;
	}
	else if( strlen(tdk)== 32)
	{
		memcpy(Buffer+nLen,"Y",1);nLen=nLen+1;
		memcpy(Buffer+nLen,tdk,32);nLen=nLen+32;
	}
	else
	{
	  	dcs_log(0,0,"key length error [%d-%s]!",strlen(tdk), tdk);
		return -1;
	}
	sprintf(datalen,"%04d",length);
	memcpy(Buffer+nLen,datalen,4);nLen=nLen+4;
	memcpy(Buffer+nLen,data,length);nLen=nLen+length;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
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
//�ŵ����ܣ�E1/E2��
//��ڲ���
//sek_index:SEK����
//tdk:tdk��Կ����,��SEK����
//length:���ݳ���
//data:�ŵ���������
//���ڲ���
//return_code:������
//int out_length:���ݳ���
//out_data:�ŵ���������
int EncTrackPrg(char *return_code, char *sek_index, char *tdk, int length, char *data, int *out_length, char *out_data)
{
	char Buffer[1024];
	char datalen[5];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(datalen,'\0',sizeof(datalen));
	nLen=2;
	memcpy(Buffer+nLen,"E1",2);nLen=nLen+2;
	memcpy(Buffer+nLen,"0",1);nLen=nLen+1;//�ӽ��ܱ�־(0:����,1:����)
	memcpy(Buffer+nLen,"0",1);nLen=nLen+1;//�ӽ����㷨(0:ECB,1:CBC)
//	memcpy(Buffer+nLen,"S",1);nLen=nLen+1;//��Կ����
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	rtrim(tdk);
	if( strlen(tdk)== 16)
	{
		 memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
		 memcpy(Buffer+nLen,tdk,16);nLen=nLen+16;
	}
	else if( strlen(tdk)== 32)
	{
		memcpy(Buffer+nLen,"Y",1);nLen=nLen+1;
		memcpy(Buffer+nLen,tdk,32);nLen=nLen+32;
	}
	else
	{
	  	dcs_log(0,0,"key length error [%d-%s]!",strlen(tdk), tdk);
		return -1;
	}
	sprintf(datalen,"%04d",length);
	memcpy(Buffer+nLen,datalen,4);nLen=nLen+4;
	memcpy(Buffer+nLen,data,length);nLen=nLen+length;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
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
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"����������Կʧ��!");
		    return -1;
  }
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

int RSALOADKEY(char *return_code, char *key_data,int key_len ,int key_index)
{
	char Buffer[2048];
	
	int	nLen ;

	if (return_code==NULL || key_data==NULL )
	{
		dcs_log(0,0,"in RSALOADKEY::  parameter is NULL!");
		memcpy(return_code,"95",2);	
		return (-1);
	}
	if ( key_len !=0 )
  if ( key_len <320 || key_len >4096 )
  {
		dcs_log(0,0,"in RSALOADKEY:: key len is err!");
		memcpy(return_code,"95",2);	
		return (-1);
	}	
	if ( key_index >20 || key_index <0 )
	{
		dcs_log(0,0,"in RSALOADKEY:: key index is err!");
		memcpy(return_code,"95",2);	
		return (-1);
	}
	
	memset(Buffer,'\0',sizeof(Buffer));
	memcpy(Buffer+2,"35",2);
	nLen=4;
	sprintf(Buffer+nLen,"%02d",key_index);
	nLen=nLen+2;
	sprintf(Buffer+nLen,"%04d",key_len);
	nLen=nLen+4;
	memcpy(Buffer+nLen,key_data,key_len);
	nLen=nLen+key_len;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"���빤����Կ ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"���빤����Կʧ��!");
		    return -1;
  }
  memcpy(return_code,Buffer,2);
	return (1);
	
}

int RSAIMPKEY(char *return_code, char *key_data,int *key_len ,int key_index)
{
	char Buffer[2048],tmp[10];
	
	int	nLen ;

	if (return_code==NULL || key_data==NULL || key_len == NULL)
	{
		dcs_log(0,0,"in RSAIMPKEY::  parameter is NULL!");
		memcpy(return_code,"95",2);	
		return (-1);
	}
	if ( key_index >20 || key_index <0 )
	{
		dcs_log(0,0,"in RSAIMPKEY:: key index is err!");
		memcpy(return_code,"95",2);	
		return (-1);
	}
	
	memset(Buffer,'\0',sizeof(Buffer));
	memcpy(Buffer+2,"36",2);
	nLen=4;
	sprintf(Buffer+nLen,"%02d",key_index);
	nLen=nLen+2;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"���빤����Կ ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"���빤����Կʧ��!");
		    return -1;
  }
  memset(tmp,0,sizeof(tmp));
  memcpy(tmp,Buffer+2,4);
  *key_len=atoi(tmp);
  if ( *key_len>0 && *key_len <=4096)
  	memcpy(key_data,Buffer+2+4,*key_len);
  else return -1;
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
	if(flag == 0)
	{
		memcpy(Buffer+nLen,"QK2", 3);nLen=nLen+3;
		memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
		memcpy(Buffer+nLen,tek_index,5);nLen=nLen+5;
	}
	else
	{
		memcpy(Buffer+nLen,"K0",2);nLen=nLen+2;
		memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
		memcpy(Buffer+nLen,tek_index,5);nLen=nLen+5;
		if(flag == 2)
			memcpy(Buffer+nLen,"Y",1);
		else if(flag == 3)
			memcpy(Buffer+nLen,"Z",1);
		else
			memcpy(Buffer+nLen,"X",1);
		nLen=nLen+1;
	}	
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, SM4_TYPE))
	{
		  dcs_log(0,0,"�����ն�����ԿTMK ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"�����ն�����ԿTMKʧ��!");
		    return -1;
	}
	if(flag == 0 || flag == 2)
	{
		memcpy(sek_tmk_data, Buffer+2, 32);
		memcpy(tek_tmk_data, Buffer+2+32, 32);
		if(flag) 
			memcpy(chk_tmk_data, Buffer+2+32+32, 16);
		else
			memcpy(chk_tmk_data, Buffer+2+32+32, 32);
	}
	else if(flag == 3)
	{
		memcpy(sek_tmk_data, Buffer+2, 48);
		memcpy(tek_tmk_data, Buffer+2+48, 48);
		memcpy(chk_tmk_data, Buffer+2+48+48, 16);
	}	
	else
	{
		memcpy(sek_tmk_data, Buffer+2, 16);
		memcpy(tek_tmk_data, Buffer+2+16, 16);
		memcpy(chk_tmk_data, Buffer+2+16+16, 16);
	}
	memcpy(return_code,Buffer,2);
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
int GET_PIKorMAK(char *return_code, char *sek_index1, char *sek_index2, char *tmk, unsigned char tmk_flag, unsigned char pm_flag, char *sek_pikmak_data, char *tmk_pikmak_data, char *CheckValue) 
{
	char Buffer[1024];
	
	int	nLen ;

	memset(Buffer, 0, sizeof(Buffer));
	nLen=2;
	if(tmk_flag == 0)
	{
		memcpy(Buffer+nLen, "QT1", 3);nLen=nLen+3;
		memcpy(Buffer+nLen, sek_index1,5);nLen=nLen+5;
		memcpy(Buffer+nLen, sek_index2,5);nLen=nLen+5;
		memcpy(Buffer+nLen, tmk, 32);nLen=nLen+32;
	}
	else
	{
		memcpy(Buffer+nLen,"K2",2);nLen=nLen+2;
		memcpy(Buffer+nLen,sek_index1,5);nLen=nLen+5;
		memcpy(Buffer+nLen,sek_index2,5);nLen=nLen+5;
		switch(tmk_flag)
		{
			case 2:
				memcpy(Buffer+nLen, "Y", 1);nLen=nLen+1;
				memcpy(Buffer+nLen,tmk,32);nLen=nLen+32;
				break;				
			case 3:
				memcpy(Buffer+nLen,tmk,48);nLen=nLen+48;
				memcpy(Buffer+nLen, "Z", 1);nLen=nLen+1;
				break;
			default:
				memcpy(Buffer+nLen, "X", 1);nLen=nLen+1;
				memcpy(Buffer+nLen,tmk,16);nLen=nLen+16;
				break;
				
		}
		if(pm_flag == 2 || pm_flag == 3)
		{
			Buffer[nLen] = 'Y';
			Buffer[nLen] += pm_flag -2;
		}
		else
			memcpy(Buffer+nLen, "X", 1);
		nLen=nLen+1;
	}	
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, SM4_TYPE))
	{
		  dcs_log(0,0,"�����ն�����ԿTMK ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
		memcpy(return_code,Buffer,2);
		dcs_log(Buffer,nLen,"�����ն�����ԿTMKʧ��!");
		return -1;
    }
	if(tmk_flag == 0)
	{
		memcpy(sek_pikmak_data,Buffer+2,32);
		memcpy(tmk_pikmak_data,Buffer+2+32,32);
		memcpy(CheckValue,Buffer+2+32+32,32);
	}
	else
	{
	    switch(pm_flag)
	   	{
	   		case 2:
				memcpy(sek_pikmak_data,Buffer+2,32);
				memcpy(tmk_pikmak_data,Buffer+2+32,32);
				memcpy(CheckValue,Buffer+2+32+32,16);
				break;
			case 3:
				memcpy(sek_pikmak_data,Buffer+2,48);
				memcpy(tmk_pikmak_data,Buffer+2+48,48);
				memcpy(CheckValue,Buffer+2+48+48,16);
				break;
			default:
				memcpy(sek_pikmak_data,Buffer+2,16);
				memcpy(tmk_pikmak_data,Buffer+2+16,16);
				memcpy(CheckValue,Buffer+2+16+16,16);
				break;
		}
	}
	memcpy(return_code,Buffer,2);
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

int _DESCALCMAC(char *return_code, char *sek_index, char *mak, int length, char *data, char *mac_value)
{
	char Buffer[2048];
	char datalen[5];
	
	int	nLen ;

	memset(Buffer,'\0',sizeof(Buffer));
	memset(datalen,'\0',sizeof(datalen));
	nLen=2;
	memcpy(Buffer+nLen,"M0",2);nLen=nLen+2;
	memcpy(Buffer+nLen,"2",1);nLen=nLen+1;
	memcpy(Buffer+nLen,sek_index,5);nLen=nLen+5;
	rtrim(mak);
	if( strlen(mak)== 16)
	{
		 memcpy(Buffer+nLen,"X",1);nLen=nLen+1;
		 memcpy(Buffer+nLen,mak,16);nLen=nLen+16;
	}
	else if( strlen(mak)== 32)
	{
		memcpy(Buffer+nLen,"Y",1);nLen=nLen+1;
		memcpy(Buffer+nLen,mak,32);nLen=nLen+32;
	}
	else
	{
	  	dcs_log(0,0,"MAC key length error [%d-%s]!",strlen(mak), mak);
		return -1;
	}
	sprintf(datalen,"%04d",length);
	memcpy(Buffer+nLen,datalen,4);nLen=nLen+4;
	memcpy(Buffer+nLen,data,length);nLen=nLen+length;
	Buffer[0]=(nLen-2)/256;
	Buffer[1]=(nLen-2)%256;
	
	if ( 0>CommandProc(Buffer,nLen,Buffer,&nLen, DES_TYPE))
	{
		  dcs_log(0,0,"MAC����ͨ��ʧ��!");
		  return -1;
	}
	if ( memcmp(Buffer,"00",2)!=0)
	{
			  memcpy(return_code,Buffer,2);
			  dcs_log(Buffer,nLen,"MAC����ʧ��!");
		    return -1;
  }
	memcpy(mac_value,Buffer+2,16);
	memcpy(return_code,Buffer,2);
	return (1);
}
