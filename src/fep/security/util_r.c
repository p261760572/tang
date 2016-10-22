#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include    <sys/time.h>    /* timeval{} for select() */
#include <string.h>

static char mkey1[32]="S44458376811702B";
static char mkey2[32]="R98142232651462J";

_3DES(char *key, char *inBuf, char *outBuf)
{
    char tKey[8 + 1], tBuf[8 + 1];
    memset(tKey, 0, sizeof(tKey));
    memcpy(tKey, key, 8);    
	_DES(tKey, inBuf, outBuf);
    memcpy(tKey, key + 8, 8);    
	DES(tKey, outBuf, tBuf);
    memcpy(tKey, key, 8);    
	_DES(tKey, tBuf, outBuf);
}


//huang jia add 20100114 ��У�麯����У��������ݣ�1���ֽڣ������������У���򷵻ش�ԭ���ݣ�������󷵻ط�����У�������
int calcParity(unsigned char code)
{
	unsigned char i;
	unsigned char count = 0;

	for(i=0;i<8;i++)
	{
		if(code & (1<<i))		//ֱ��ͳ��1�ĸ�������Ȼ��ͳ��0Ҳһ������������0��ʱ���Ȼ��������1
		{
			count++;
		}
	}
	
	if ( count %2 )	return 1;
	else	return -1;
		
//	if ( count %2 )	return code;
//	else	return code&0xfe^(code&0x01^0x01);//ȡ��code�����λ��ȡ��
} 

int GenrateKey(unsigned char *key)
{
    int i ;
    int s;
		struct timeval  nowtime;
    unsigned char buf[20],*p;
   
   if ( key == NULL)
   		return 0;
   
//  time(&t);
//  tm = localtime(&t);
  gettimeofday( &nowtime, 0 );
  memset(buf,0,sizeof(buf));
  for ( i=0 ; i<8;i++)
  {
      srand(((nowtime.tv_sec +nowtime.tv_usec)>>i)+i);
      s=rand();
      ////dcs_debug(0,0,"s=%d",s);
      p= (unsigned char*)&s;
      
//huang jia del 20100114 ɾ����У�����ݻ�ȡ��ʽ       
//      if ( (*p) %2 )
//        *key=*p;
//      else
//      	*key=(*p)+1;
//      key++;
//      if ( (*(p+1)) %2 )
//      	*key=*(p+1);
//      else 
//      	*key=(*(p+1)) +1;
//      key++;

			//huang jia add 20100114 ������У�����ݻ�ȡ��ʽ
//			*key=calcParity(*p);
//			key++;
//			*key=calcParity(*(p+1));
//			key++;
			if( calcParity(*p) )	*key=*p;
			else	*key=(*p)&0xfe^((*p)&0x01^0x01);
			key++;
			if( calcParity(*(p+1)) )	*key=*(p+1);
			else	*key=(*(p+1))&0xfe^((*(p+1))&0x01^0x01);
			key++;
			
  }
	return 1;
}
int GetMastKey(char *Index,char *key)
{
		char   caFileName[512],buf[64],tmpkey1[32],tmpkey2[32],buf1[33];
  	int    iFd=-1,i;  
  	
  	
  	//�趨commlnk�����ļ��ľ���·��������
  	memset(buf,0,sizeof(buf));
  	memset(caFileName, 0, sizeof(caFileName) );
  	memset(tmpkey1, 0, sizeof(tmpkey1) );
  	memset(tmpkey2, 0, sizeof(tmpkey2) );
  	for ( i =0 ;i<16;i++)
  	{
  		   tmpkey1[i]=mkey1[i]-i+(i%3);
  		   tmpkey2[i]=mkey2[i]-i+(i%7);
  		   tmpkey1[i]=((unsigned char)tmpkey1[i])^((unsigned char)tmpkey2[i]);
  	}
  	
  	sprintf(caFileName, "%s/config/%s",getenv("ICS_HOME"),"mastkey.dat");
    ////dcs_debug(0,0,"file=[%s]",caFileName);
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log( 0,0, " open mastkey.dat fail ! ");
				return -1;
  	}
    if ( 0 >conf_get_first_string(iFd, Index,buf ))
    {
            dcs_log(0,0, "load configuration item %s failed!",Index);
            conf_close(iFd);
            return (-1);
    }
    
 //   ////dcs_debug(tmpkey1,16,"mastkey key ");
    asc_to_bcd(tmpkey2,buf,32,0);
//    ////dcs_debug(tmpkey2,16,"key mtext=%s",buf);
    memcpy(buf,tmpkey2,8);
    _TripleDes(tmpkey1,buf,buf1 );
    memcpy(buf,tmpkey2+8,8);
    memcpy(key,buf1,8);
    _TripleDes(tmpkey1,buf,buf1 );
    memcpy(key+8,buf1,8);
    ////dcs_debug(key,16,"key disp");
    bcd_to_asc(buf1,key,32,0);
    memcpy(key,buf1,32);
  	conf_close(iFd);
    
  	return 1;
}

int DisperseKey(char *MKey, char *Disunit, char *SKey)
{
	unsigned char caMastKey[17],caSlaveKey[17],caTmpKey[17],caTmpBuf[512];
	unsigned char caDisUnit[9];
	int n;
	
	memset(caMastKey,0,sizeof(caMastKey));
	memset(caSlaveKey,0,sizeof(caSlaveKey));
	memset(caTmpKey,0,sizeof(caTmpKey));
	memset(caTmpBuf,0,sizeof(caTmpBuf));
	memset(caDisUnit,0,sizeof(caDisUnit));
	
	memcpy(caMastKey, MKey, 16);
	memcpy(caDisUnit, Disunit, 8);	
	
	////dcs_debug(Disunit,8,"��ɢ����");
	TripleDes(caMastKey,caDisUnit,caTmpKey );
  for ( n=0; n<8;n++)
  {
  	Disunit[n]=~Disunit[n];
	}
	
	TripleDes(caMastKey,Disunit,caTmpKey+8);
	memcpy(SKey,caTmpKey,16);
	
	////dcs_debug(SKey,16,"��ɢ��Կ");
	
	return 1;
	
}

int DecodeTrack(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char caTmpBuf[512];	
	char caMastKey[60], caTmpKey[17], caWorkKey[17], caTrackKey[17];
	char caEncryptFlg[2],caArithmeticFlg[2],caWorkKeyFlg[2],caBankKeyIndex[4],caDisTimes[2];
	char caDisUnit[80], caData[1024];
	char caText[9], caNextText[9];
	char caTmpMac[17], caMac[17];
	unsigned char caDataLen[5];
	
	int iEncryptFlg, iArithmeticFlg, iWorkKeyFlg, iDisTimes, iDataLen=0;
	int i=0,j=0,idx=0;
	
	//dcs_debug(0,0,"entry DecodeTrack function");
	
	memset(caMastKey,0,sizeof(caMastKey));
	memset(caTmpKey,0,sizeof(caTmpKey));
	memset(caWorkKey,0,sizeof(caWorkKey));
	memset(caTrackKey,0,sizeof(caTrackKey));	
	memset(caEncryptFlg,0,sizeof(caEncryptFlg));	
	memset(caArithmeticFlg,0,sizeof(caArithmeticFlg));
	memset(caWorkKeyFlg,0,sizeof(caWorkKeyFlg));
	memset(caBankKeyIndex,0,sizeof(caBankKeyIndex));
	memset(caDisTimes,0,sizeof(caDisTimes));
	memset(caDisUnit,0,sizeof(caDisUnit));
	memset(caDataLen,0,sizeof(caDataLen));
	memset(caData,0,sizeof(caData));
	memset(caText,0,sizeof(caText));
	memset(caNextText,0,sizeof(caNextText));
	memset(caTmpMac,0,sizeof(caTmpMac));
	memset(caMac,0,sizeof(caMac));
	
	/*��ȡ���ܲ���*/
	idx=0;
	memcpy(caEncryptFlg, inBuf, 1);/*��/���ܱ�־,1�����ܣ�2�����ܣ�*/
	iEncryptFlg=atoi(caEncryptFlg);
	idx+=1;
	
	memcpy(caArithmeticFlg, inBuf+idx, 1);/*��/�����㷨,1��ECB 2��CBC */
	iArithmeticFlg=atoi(caArithmeticFlg);
	idx+=1;
	
	memcpy(caWorkKeyFlg, inBuf+idx, 1);/*������Կ����,1��ֱ�ӷ�ɢ�õ���3DES��Կ��2����ɢ��ȡ��8�ֽڣ�DES��Կ��*/
	iWorkKeyFlg=atoi(caWorkKeyFlg);
	idx+=1;
	
	memcpy(caBankKeyIndex, inBuf+idx, 3);/*��������Կ������*/
	idx+=3;
	
	memcpy(caDisTimes, inBuf+idx, 1);/*��ɢ����*/
	iDisTimes=atoi(caDisTimes);
	idx+=1;
	
	memcpy(caDisUnit, inBuf+idx, iDisTimes*16 );/*��ɢ����*/
	idx+=iDisTimes*16;
	
	asc_to_bcd(caDataLen,inBuf+idx, 4);	/*���ݿ鳤��*/
	iDataLen=caDataLen[0]*256 + caDataLen[1]-8;/* 8:��ʼ��������*/
	iDataLen=iDataLen/2;
	idx+=4;
	
	asc_to_bcd(caData,inBuf+idx, iDataLen*2, 0 );/*���ݿ�*/
	
	/*У��ӽ��ܱ�־*/
	if ( iEncryptFlg!=2)
	{
		 dcs_log(0,0,"����ļӽ��ܱ�־�����ܴŵ������뽫�ӽ��ܱ�־��Ϊ2");
		 return -1;
  }
  /*У�����ݳ���*/
  if( 0 != iDataLen%8 || iDataLen <= 0 )
  {
  	dcs_log(0,0,"�ŵ����ݳ��ȱ�����ڵ���0��Ϊ8�ı���, len=%d", iDataLen );
		//return -1;//huang jia del 20100114 ��У�����ݳ���
  }
  
//dcs_debug(caData,iDataLen,"�������ݣ�len=%d", iDataLen );
  
  /*��ȡ����Կ*/
	if ( 0>GetMastKey((char *)caBankKeyIndex,(char *)caMastKey))
	{
		  //dcs_debug(0,0,"can not get mastkey[%s]",caBankKeyIndex);
		  return -1;
	}
	
	/*���㹤����Կ*/
	asc_to_bcd(caMastKey,caMastKey,32,0);
	for( i=0; i<iDisTimes; i++ )
	{		
		memset(caTmpKey,0,sizeof(caTmpKey));
		memset(caTmpBuf,0,sizeof(caTmpBuf));		
		asc_to_bcd(caTmpBuf,caDisUnit+(i*16),16);
		DisperseKey(caMastKey,caTmpBuf,caTmpKey );
		memcpy(caMastKey,caTmpKey,16);				
	}
	memcpy(caWorkKey,caTmpKey,16);	
	
//dcs_debug(caWorkKey,16,"������Կ��len=%d", strlen(caWorkKey));	

	/*ȡTrack��Կ*/
	switch( iWorkKeyFlg )
	{
		case 1:
			memcpy(caTrackKey,caWorkKey,16);
			//dcs_debug(caTrackKey,16,"Track��Կ��len=%d", strlen(caTrackKey));	
			break;
		case 2:
			memcpy(caTrackKey,caWorkKey,8);
			//dcs_debug(caTrackKey,8,"Track��Կ��len=%d", strlen(caTrackKey));	
			break;
		default:
			//dcs_debug(0,0,"caWorkKeyFlg=%s, iWorkKeyFlg=%d", caWorkKeyFlg, iWorkKeyFlg );
			return -1;
	}
	
	/*���ܴŵ���Ϣ*/
	memset(caTmpBuf,0,sizeof(caTmpBuf));	
	for(i=0; i< iDataLen/8; i++ )
	{
		memset(caText,0,sizeof(caText));
		memcpy(caText,caData+i*8, 8 );
////dcs_debug(caText,8,"�������ݿ�");
		_DES(caTrackKey,caText,caTmpBuf+i*8);	
////dcs_debug(caTmpBuf+i*8,8,"���ܽ�����ݿ�");	

	}

////dcs_debug(caTmpBuf,iDataLen,"���ܽ������");	

	/*��֯Ӧ����*/
	*nOut=2;
	memcpy(outBuf+*nOut,"95",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,"00",2);
	*nOut=*nOut+2;		
	bcd_to_asc(outBuf+*nOut,caTmpBuf,iDataLen*2);
	*nOut=*nOut+iDataLen*2;
	outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;

//dcs_debug(outBuf,6+iDataLen*2,"�ŵ�����Ӧ����");	
  
	return 1;
	
	
}
int DesGetKey(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char key[17],buf[17];
	
	memset(key,0,sizeof(key));
	//dcs_debug(0,0,"entry DesGetKey function");
	GenrateKey((unsigned char*)key);
	
	//huang jia add 20100114 ��ʱֱ�Ӹ�ֵ,�Ժ������޸�
	//7F37A17F7F23E934	��ͨ����У��
	//C12F29575B6D3B3B	��ͨ����У��
	//asc_to_bcd(key,"C12F29575B6D3B3B",16,0);//�ڴ˹ر�ֱ�Ӹ�ֵ

	//dcs_debug(key,8,"key data");
	
	//huang jia add 20100114 ��key��des��������
	unsigned char caIndex[4],caMastKey[60],mkey[17];
	memset(caIndex,0,sizeof(caIndex));
	memset(caMastKey,0,sizeof(caMastKey));
	memset(mkey,0,sizeof(mkey));
	memcpy(caIndex,inBuf,3);
	if ( 0>GetMastKey((char *)caIndex,(char *)caMastKey))
	{
		  //dcs_debug(0,0,"can not get mastkey[%s]",caIndex);
		  return -1;
	}
	//dcs_debug(0,0,"caIndex=[%s],caMastKey=[%s]",caIndex,caMastKey);
	asc_to_bcd(caMastKey,caMastKey,16,0);
	DES((unsigned char *)caMastKey,(unsigned char *)key,(unsigned char *)mkey);
	memcpy(key,mkey,8);
	//dcs_debug(caMastKey,8,"caMastKey data");
	//dcs_debug(key,8,"mkey data");
	
	
	bcd_to_asc((unsigned char*)buf,(unsigned char*)key,16,0);

  *nOut=2;
	memcpy(outBuf+*nOut,"1B",2);

	*nOut=*nOut+2;

	memcpy(outBuf+*nOut,"00",2);
	*nOut= *nOut+2;

	memcpy(outBuf+*nOut,buf,16);
	*nOut=*nOut+16;
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}
int DesMacOp(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char caTmpBuf[512];	
	char caMastKey[60], caTmpKey[17], caWorkKey[17], caMacKey[17];
	char caFillFlg[2],caWorkKeyFlg[2],caBankKeyIndex[4],caDisTimes[2];
	char caDisUnit[80],caData[1024];
	char caAppData[10], caText[9], caNextText[9];
	char caTmpMac[17], caMac[17];
	unsigned char  caDataLen[5];
	
	int iFillFlg, iWorkKeyFlg, iDisTimes, iDataLen;
	int i=0,j=0,n;
	
	//dcs_debug(0,0,"entry DesMacOp function");
	
	memset(caMastKey,0,sizeof(caMastKey));
	memset(caTmpKey,0,sizeof(caTmpKey));
	memset(caWorkKey,0,sizeof(caWorkKey));
	memset(caMacKey,0,sizeof(caMacKey));	
	memset(caFillFlg,0,sizeof(caFillFlg));	
	memset(caWorkKeyFlg,0,sizeof(caWorkKeyFlg));
	memset(caBankKeyIndex,0,sizeof(caBankKeyIndex));
	memset(caDisTimes,0,sizeof(caDisTimes));
	memset(caDisUnit,0,sizeof(caDisUnit));
	memset(caDataLen,0,sizeof(caDataLen));
	memset(caData,0,sizeof(caData));
	memset(caAppData,0,sizeof(caAppData));
	memset(caText,0,sizeof(caText));
	memset(caNextText,0,sizeof(caNextText));
	memset(caTmpMac,0,sizeof(caTmpMac));
	memset(caMac,0,sizeof(caMac));
	
	/*��ȡ���ܲ���*/
	memcpy(caFillFlg, inBuf, 1);
	iFillFlg=atoi(caFillFlg);
	memcpy(caWorkKeyFlg, inBuf+1, 1);
	iWorkKeyFlg=atoi(caWorkKeyFlg);
	memcpy(caBankKeyIndex, inBuf+1+1, 3);
	memcpy(caDisTimes, inBuf+1+1+3, 1);
	iDisTimes=atoi(caDisTimes);
	memcpy(caDisUnit, inBuf+1+1+3+1, iDisTimes*16 );
	memcpy(caDataLen,inBuf+1+1+3+1+iDisTimes*16, 4);
	iDataLen=atoi((char *)caDataLen);//huang jia modi ��caDataLenǰ�����(char*)����ȥ������ʱ���ֵľ���
////dcs_debug(0,0,"���ݳ���, caDataLen=%s",caDataLen);	
////dcs_debug(0,0,"���ݳ���,iDataLen=%d",iDataLen);
	asc_to_bcd(caData,inBuf+1+1+3+1+iDisTimes*16+4, iDataLen*2, 0 );
	memcpy(caAppData,"\x80\x00\x00\x00\x00\x00\x00\x00\x00", 9 );
	
	if( iDataLen <=0)
	{
		  //dcs_debug(0,0,"len <=0[%04d]",caDataLen);
			 return -1;
	}
	
	/*��ȡ����Կ*/
	if ( 0>GetMastKey((char *)caBankKeyIndex,(char *)caMastKey))
	{
		  //dcs_debug(0,0,"can not get mastkey[%s]",caBankKeyIndex);
		  return -1;
	}
	
	/*�����*/
	if ( iDataLen%8 )
	{
		  //dcs_debug(0,0,"len ��Ϊ8�ı���[%04d]",iDataLen);
			switch( iFillFlg )
			{
				case 1:
					memcpy(caData+iDataLen,caAppData,8-(iDataLen%8));
					break;
				case 2:
					//dcs_debug(0,0,"caFillFlg=%s, iFillFlg=%d", caFillFlg, iFillFlg );
					break;
				case 3:
					memcpy(caData+iDataLen,caAppData+1,8-(iDataLen%8));
					break;
				default:
					;
			}
	}
	else
	{
		if( 2==iFillFlg )
			memcpy(caData+iDataLen,caAppData,8);		
	}
	
    //dcs_debug(caData,((int)((iDataLen-1)/8)+1)*8,"�������ݣ�len=%d", ((int)((iDataLen-1)/8)+1)*8 );

	/*���㹤����Կ*/
	asc_to_bcd(caMastKey,caMastKey,32,0);
	for(i=0; i<iDisTimes; i++ )
	{		
		memset(caTmpKey,0,sizeof(caTmpKey));
		memset(caTmpBuf,0,sizeof(caTmpBuf));		
		asc_to_bcd(caTmpBuf,caDisUnit+(i*16),16);
		DisperseKey(caMastKey,caTmpBuf,caTmpKey );
		memcpy(caMastKey,caTmpKey,16);				
	}
	memcpy(caWorkKey,caTmpKey,16);
	
//dcs_debug(caWorkKey,16,"������Կ��len=%d", strlen(caWorkKey));	

	/*ȡMAC��Կ*/
	switch( iWorkKeyFlg )
	{
		case 1:
			memcpy(caMacKey,caWorkKey,16);
			break;
		case 2:
			memcpy(caMacKey,caWorkKey,8);
			break;
		default:
			//dcs_debug(0,0,"caWorkKeyFlg=%s, iWorkKeyFlg=%d", caWorkKeyFlg, iWorkKeyFlg );
			return -1;
	}
	
//dcs_debug(caMacKey,16,"MAC��Կ��len=%d", strlen(caMacKey));	
			
	/*����MAC*/
	memcpy(caText,caData, 8 );
	for(i=1; i<= ((int)((iDataLen-1)/8)+1); i++ )
	{	
////dcs_debug(caText,8,"����MAC�����ݿ�");	
		memset(caTmpMac,0,sizeof(caTmpMac));
		DES(caMacKey,caText,caTmpMac);
////dcs_debug(caTmpMac,8,"�м�MAC");			
		memcpy(caNextText,caData+i*8, 8 );
		for(j=0; j<8; j++)
		{
			caText[j]=caTmpMac[j]^caNextText[j];
		}
		
	}
	memcpy(caMac,caTmpMac,8);
	
	/*��֯Ӧ����*/
	*nOut=2;
	memcpy(outBuf+*nOut,"96",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,"00",2);
	*nOut=*nOut+2;		
	bcd_to_asc(outBuf+*nOut,caMac,16);
	*nOut=*nOut+16;
	outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;

//dcs_debug(outBuf,22,"MACӦ����");	
  
	return 1;
	
}

int DesPinTr(char *inBuf,int nIn,char *outBuf,int *nOut)
{
		
	char caTmpBuf[512], caAppData[17];	
	char caTermKeyIndex[4], caBankKeyIndex[4], caTermPinKey[33], caBankPinKey[33];
	char caTermMastKey[60], caBankMastKey[60],caWorkMastkey[60],caTmpPinKey[33];	
	char caWorkKeyFlg[2], caDesPikFlg[2], caDisTimes[2],caDisUnit[81];
	char caPan[17],caTermPin[17],caBankPin[17], caTmpPin[17];
	int iEncryptFlg, iArithmeticFlg, iWorkKeyFlg, iDisTimes, iDataLen=0;
	int i=0,n,idx=0;
	
	//dcs_debug(0,0,"entry DesPinTr function");
	memset(caTmpBuf,0,sizeof(caTmpBuf));
	memset(caTermKeyIndex,0,sizeof(caTermKeyIndex));
	memset(caBankKeyIndex,0,sizeof(caBankKeyIndex));
	memset(caTermPinKey,0,sizeof(caTermPinKey));
	memset(caBankPinKey,0,sizeof(caBankPinKey));
	memset(caTermMastKey,0,sizeof(caTermMastKey));
	memset(caBankMastKey,0,sizeof(caBankMastKey));
	memset(caWorkMastkey,0,sizeof(caWorkMastkey));
	memset(caTmpPinKey,0,sizeof(caTmpPinKey));
	memset(caWorkKeyFlg,0,sizeof(caWorkKeyFlg));
	memset(caDesPikFlg,0,sizeof(caDesPikFlg));
	memset(caDisTimes,0,sizeof(caDisTimes));
	memset(caDisUnit,0,sizeof(caDisUnit));
	memset(caPan,0,sizeof(caPan));
	memset(caTermPin,0,sizeof(caTermPin));
	memset(caBankPin,0,sizeof(caBankPin));
	memset(caTmpPin,0,sizeof(caTmpPin));
	memset(caAppData,0,sizeof(caAppData));
	
	/*��ȡ���ܲ���*/
	idx=0;	
	memcpy(caWorkKeyFlg, inBuf+idx, 1);/*Դ������Կ����,1��ֱ�ӷ�ɢ�õ���3DES��Կ��2����ɢ��ȡ��8�ֽڣ�DES��Կ��*/
	iWorkKeyFlg=atoi(caWorkKeyFlg);
	idx+=1;
	
	memcpy(caTermKeyIndex, inBuf+idx, 3);/*�ն�����Կ������*/
	idx+=3;
	
	memcpy(caBankKeyIndex, inBuf+idx, 3);/*��������Կ������*/
	idx+=3;
	
	memcpy(caDisTimes, inBuf+idx, 1);/*��ɢ����*/
	iDisTimes=atoi(caDisTimes);
	idx+=1;
	
	memcpy(caDisUnit, inBuf+idx, iDisTimes*16 );/*��ɢ����*/
	idx+=iDisTimes*16;
	
	memcpy(caDesPikFlg, inBuf+idx, 1);/*Ŀ��PIK��־*/	
	idx+=1;	
	
	/*ȡ������Կ����*/
	switch( caDesPikFlg[0] )
	{
		case 'X':
			asc_to_bcd(caBankPinKey, inBuf+idx, 16);/*64bit������Կ����*/	
			idx+=16;
			//dcs_debug(caBankPinKey,8,"������Կ����");					
			break;
		case 'Y':
			asc_to_bcd(caBankPinKey, inBuf+idx, 32);/*128bit������Կ����*/	
			idx+=32;				
			//dcs_debug(caBankPinKey,16,"������Կ����");	
			break;
		case 'Z':
			asc_to_bcd(caBankPinKey, inBuf+idx, 48);/*128bit������Կ����*/	
			idx+=48;	
			//dcs_debug(caBankPinKey,24,"������Կ����");				
			break;
		default:
			//dcs_debug(0,0,"�����Ŀ��PIK��־caDesPikFlg=%s", caDesPikFlg );
			return -1;
	}

	asc_to_bcd(caTermPin, inBuf+idx, 16);/*PIN ����*/	
	idx+=16;
//dcs_debug(caTermPin,8,"�ն�PIN����");

	memcpy(caAppData, inBuf+idx, 16);/*������Ϣ*/	
	idx+=16;
//dcs_debug(caAppData,16,"������Ϣ");	
	
	
  /*��ȡ�ն�����Կ*/
	if ( 0>GetMastKey((char *)caTermKeyIndex,(char *)caTermMastKey))
	{
		  //dcs_debug(0,0,"can not get terminal mastkey[%s]",caTermKeyIndex);
		  return -1;
	}
//dcs_debug(caTermMastKey,32,"�ն�����Կ��len=%d", strlen(caTermMastKey));
	
	/*��ȡ��������Կ*/
	if ( 0>GetMastKey((char *)caBankKeyIndex,(char *)caBankMastKey))
	{
		  //dcs_debug(0,0,"can not get bank mastkey[%s]",caBankKeyIndex);
		  return -1;
	}
//dcs_debug(caBankMastKey,32,"��������Կ��len=%d", strlen(caBankMastKey));	
	
	/*�����ն˹�����Կ*/
	asc_to_bcd(caTermMastKey,caTermMastKey,32,0);
	for(i=0; i<iDisTimes; i++ )
	{		
		memset(caTmpPinKey,0,sizeof(caTmpPinKey));
		memset(caTmpBuf,0,sizeof(caTmpBuf));		
		asc_to_bcd(caTmpBuf,caDisUnit+(i*16),16);
		DisperseKey(caTermMastKey,caTmpBuf,caTmpPinKey );
		memcpy(caTermMastKey,caTmpPinKey,16);				
	}	
//dcs_debug(caTmpPinKey,16,"������Կ��len=%d", strlen(caTmpPinKey));	

	/*ȡ�ն�PIN��Կ*/
	switch( iWorkKeyFlg )
	{
		case 1:
			memcpy(caTermPinKey,caTmpPinKey,16);
			//dcs_debug(caTermPinKey,16,"�ն�PIN��Կ��len=%d", strlen(caTermPinKey));	
			break;
		case 2:
			memcpy(caTermPinKey,caTmpPinKey,8);
			//dcs_debug(caTermPinKey,8,"�ն�PIN��Կ��len=%d", strlen(caTermPinKey));	
			break;
		default:
			//dcs_debug(0,0,"caWorkKeyFlg=%s, iWorkKeyFlg=%d", caWorkKeyFlg, iWorkKeyFlg );
			return -1;
	}
	
	/*�����ն�PIN����*/
	_TripleDes( caTermPinKey,caTermPin,caPan);
//dcs_debug(caPan,8,"�ն�PIN����");
	
	/*��������PIN��Կ*/
	if ( caDesPikFlg[0] == 'X')
	{
		asc_to_bcd(caBankMastKey,caBankMastKey,32,0);		
		memcpy(caWorkMastkey,caBankMastKey,8);
		_DES(caWorkMastkey,caBankPinKey,caBankPinKey);
//dcs_debug(caBankPinKey,8,"����PIN��Կ����,len=%d", strlen(caBankPinKey));

	}
	else
	{
		//dcs_debug(0,0,"��ʱ��û��ʵ�ָ���������Կ�ļ��ܷ�ʽ");
		return -1;
	}
	
	/*������PIN��Կ����PIN����*/
	DES(caBankPinKey, caPan, caTmpPin);
	bcd_to_asc(caBankPin, caTmpPin, 16);
//dcs_debug(caBankPin,16,"������PIN��Կ����PIN����");

	*nOut=2;
	memcpy(outBuf+*nOut,"97",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,"00",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,caBankPin,16);	
	*nOut=*nOut+16;
	outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
 //dcs_debug(outBuf,22,"��Կת��Ӧ����");
 
	return 1;
	
}

int BNKDesMacOp(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	unsigned char caIndex[4],caMacKey[33],caLen[4], caMastKey[60], caMastKeyB[16 + 1], caTmpKey[17],caMac[17],caTmpBuf[17];
	int i=0,n;
	//dcs_debug(0,0,"entry BNKDesMacOp function");
	memset(caIndex,0,sizeof(caIndex));
	memset(caMacKey,0,sizeof(caMacKey));
	memset(caLen,0,sizeof(caLen));
	memset(caTmpKey,0,sizeof(caTmpKey));
	memcpy(caIndex,inBuf,3);
	memcpy(caMacKey,inBuf+3,16);
	memcpy(caLen,inBuf+19,3);
	if( atoi( (char *)caLen ) <=0)
	{
		  //dcs_debug(0,0,"len <=0[%s]",caLen);
			 return -1;
	}
	if ( 0>GetMastKey((char *)caIndex,(char *)caMastKey))
	{
		  //dcs_debug(0,0,"can not get mastkey[%s]",caIndex);
		  return -1;
	}
	if ( atoi((char *)caLen)%8)
	{
		  //dcs_debug(0,0,"len ��Ϊ8�ı���[%s]",caLen);
			 return -1;
	}
	//dcs_debug(0, 0, "MastKey[%s]", caMastKey);
	asc_to_bcd(caMacKey,caMacKey,16,0);
	asc_to_bcd(caMastKeyB, caMastKey, 32, 0);
	_3DES(caMastKeyB, caMacKey, caTmpKey);
	//dcs_debug(caTmpKey,8,"mac key");
	memset(caTmpBuf,0,sizeof(caTmpBuf));
	for( i=0; i<atoi((char *)caLen)/8; i++)
	{
		for( n=0;n<8;n++)
		  caMac[n]=caTmpBuf[n]^inBuf[22+i*8+n];
		DES(caTmpKey,caMac,caTmpBuf);
//		//dcs_debug(caTmpBuf,8,"mac temp data");
	}	
	*nOut=2;
	memcpy(outBuf+*nOut,"81",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,"00",2);
	*nOut=*nOut+2;
	//dcs_debug(caTmpBuf,8,"mac data");
	bcd_to_asc(caMac,caTmpBuf,16);
	memcpy(outBuf+*nOut,caMac,16);
	*nOut=*nOut+16;
	outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}

int BNKDesPinTr(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	unsigned char caIndex1[4],caIndex2[4],caPinKey1[33],caPinKey2[33],caMastKey1[60],
	         caPin[17],caMastKey2[60],caTmpKey[17],caTmpBuf[17],
	         caPan1[17],caPan2[17],caTmpPin[17];
	int i=0,n;
	
	//dcs_debug(0,0,"entry BNKDesPinTr function");
	
	memset(caIndex1,0,sizeof(caIndex1));
	memset(caIndex2,0,sizeof(caIndex2));
	memset(caPinKey1,0,sizeof(caPinKey1));
	memset(caPinKey2,0,sizeof(caPinKey2));

	memset(caTmpKey,0,sizeof(caTmpKey));
	memcpy(caIndex1,inBuf,3);
	i=3;
	memcpy(caPinKey1,inBuf+i,16);
	i=i+16;
	memcpy(caIndex2,inBuf+i,3);
	i=i+3;
	memcpy(caPinKey2,inBuf+i,16);
	i=i+16;
	memcpy(caPin,inBuf+i,16);
	i=i+16;
	memcpy(caPan1,inBuf+i,16);
	i=i+16;
	memcpy(caPan2,inBuf+i,16);
	
	if ( 0>GetMastKey((char *)caIndex1,(char *)caMastKey1))
		  return -1;
	if ( 0>GetMastKey((char *)caIndex2,(char *)caMastKey2))
		  return -1;
	
	asc_to_bcd(caPinKey1,caPinKey1,16,0);
	asc_to_bcd(caPinKey2,caPinKey2,16,0);
	asc_to_bcd(caMastKey1,caMastKey1,32,0);
	asc_to_bcd(caMastKey2,caMastKey2,32,0);
	asc_to_bcd(caPin,caPin,16,0);
	asc_to_bcd(caPan1,caPan1,16,0);
	asc_to_bcd(caPan2,caPan2,16,0);
	_3DES(caMastKey1,caPinKey1,caTmpKey);
	//dcs_debug(caMastKey1, 16, "caMastKey1:");
	//dcs_debug(caTmpKey, 8, "Pin Key:");
	_DES(caTmpKey,caPin,caTmpPin);
	for( i=0 ; i<8 ; i++)
	  caTmpPin[i]=caTmpPin[i]^caPan1[i];
	//dcs_debug(caMastKey2, 16, "caMastKey2:");
	_3DES(caMastKey2,caPinKey2,caTmpKey);
	//dcs_debug(caTmpKey, 8, "Bank Pin Key:");
    //dcs_debug(caTmpPin,8,"pin data");
	for( i=0 ; i<8 ; i++)
	  caTmpPin[i]=caTmpPin[i]^caPan2[i];
	DES(caTmpKey,caTmpPin,caPin);
	bcd_to_asc(caTmpBuf,caPin,16,0);
	*nOut=2;
	memcpy(outBuf+*nOut,"6:",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,"00",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,caTmpBuf,16);	
	*nOut=*nOut+16;
	outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
	return 1;
}


int DesTest(char *inBuf,int nIn,char *outBuf,int nOut)
{
	return 1;
}


/*
��ĸ��д˵��
A��	�ɴ�ӡ�ַ�
B��	�����ַ���0x00-0xFF
H��	ʮ�������ַ���0���C��9������A���C��F��
N��	ʮ�����ַ���0��-��9��
n��	�ɱ䳤����

����	�����ն�����Կ�����������ĺͼ��ֵ���ظ�������
˵��	���ɵ��ն�����Կ������У�顣

��						����/����			����
ָ����Ϣ��ʽ
������				2A						K0
SEK����				1A��4N				S��4λ����
TEK����				1A��4N				T��4λ����
TMK��Կ����		1A						X��64��������
														Y��128 ��������
														Z��192 ��������

������Ϣ��ʽ
������				2A						K1
������				2A						00����ȷ
TMK����1			16/32/48H			��SEK����
TMK����2			16/32/48H			��TEK����
TMK��Կ���ֵ	16H						��TMK����64����0
*/
int DesGetTmk(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char tmk[17],buf[17];
	
	memset(tmk,0,sizeof(tmk));
	
	//dcs_debug(0,0,"entry DesGetTmk function");
	GenrateKey((unsigned char*)tmk);
	
	
	//7F37A17F7F23E934	��ͨ����У��
	//C12F29575B6D3B3B	��ͨ����У��
	//asc_to_bcd(tmk,"0123456789ABCDEF",16,0);//�ڴ˹ر�ֱ�Ӹ�ֵ
	

	//dcs_debug(tmk,8,"tmk data");
	
	
	unsigned char caSekIndex[6],caTekIndex[6],caSekKey[60],caTekKey[60],mkey1[17],mkey2[17];
	memset(caSekIndex,0,sizeof(caSekIndex));
	memset(caTekIndex,0,sizeof(caTekIndex));
	memset(caSekKey,0,sizeof(caSekKey));
	memset(caTekKey,0,sizeof(caTekKey));
	memset(mkey1,0,sizeof(mkey1));
	memset(mkey2,0,sizeof(mkey2));
	memcpy(caSekIndex,inBuf,5);
	memcpy(caTekIndex,inBuf+5,5);
	GetMastKey((char *)caSekIndex,(char *)caSekKey);
	GetMastKey((char *)caTekIndex,(char *)caTekKey);
	//dcs_debug("caSekIndex=[%s],caSekKey=[%s]",caSekIndex,caSekKey);
	//dcs_debug("caTekIndex=[%s],caTekKey=[%s]",caTekIndex,caTekKey);
	asc_to_bcd(caSekKey,caSekKey,16,0);
	asc_to_bcd(caTekKey,caTekKey,16,0);
	DES((unsigned char *)caSekKey,(unsigned char *)tmk,(unsigned char *)mkey1);
	DES((unsigned char *)caTekKey,(unsigned char *)tmk,(unsigned char *)mkey2);
	
	//dcs_debug(caSekKey,8,"caSekKey data");
	//dcs_debug(caTekKey,8,"caTekKey data");
	//dcs_debug(mkey1,8,"tmk1 data");
	//dcs_debug(mkey2,8,"tmk2 data");
		
	bcd_to_asc((unsigned char*)buf,(unsigned char*)mkey1,16,0);
	bcd_to_asc((unsigned char*)buf+16,(unsigned char*)mkey2,16,0);

  *nOut=2;
	memcpy(outBuf+*nOut,"K1",2);

	*nOut=*nOut+2;

	memcpy(outBuf+*nOut,"00",2);
	*nOut= *nOut+2;

	memcpy(outBuf+*nOut,buf,32);
	*nOut=*nOut+32;
	memcpy(outBuf+*nOut,"0000000000000000",16);
	*nOut=*nOut+16;
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}

/*
����	����������ԿPIK/MAK�����������ĺͼ��ֵ���ظ�������
˵��	�������ɵ�������Կ��PIK��MAK����Ҫͬʱ�������������ģ�����һ����TMK��������ͨ���������ķ�ʽ��ǩ��Ӧ����Ϣ�д����նˣ���һ����SEK���ܺ󣬽����ı�����POSP���ݿ��С�
*/
int DesGetPikMak(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char PikMak[33],buf[17],op1,op2;
	int n;
	memset(PikMak,0,sizeof(PikMak));
	
	//dcs_debug(0,0,"entry DesGetPikMak function");

	
	unsigned char caSekIndex1[6],caSekIndex2[6],caSekKey1[60],caSekKey2[60],tmk[33],mkey[33],PikMak1[33],PikMak2[33];
	memset(caSekIndex1,0,sizeof(caSekIndex1));
	memset(caSekIndex2,0,sizeof(caSekIndex2));
	memset(caSekKey1,0,sizeof(caSekKey1));
	memset(caSekKey2,0,sizeof(caSekKey2));
	memset(tmk,0,sizeof(tmk));
	memset(mkey,0,sizeof(mkey));
	memset(PikMak1,0,sizeof(PikMak1));
	memset(PikMak2,0,sizeof(PikMak2));
	memcpy(caSekIndex1,inBuf,5);
	memcpy(caSekIndex2,inBuf+5,5);
	n=10;
	op1=inBuf[n];
	n++;
	if( op1 =='X')
	{
		memcpy(tmk,inBuf+n,16);
		n=n+16;
  }
  else if( op1 =='Y')
	{
		memcpy(tmk,inBuf+n,32);
		n=n+32;
  }else if( op1 =='Z')
	{
		memcpy(tmk,inBuf+n,48);
		n=n+48;
  }
  op2=inBuf[n];
	GetMastKey((char *)caSekIndex1,(char *)caSekKey1);
	GetMastKey((char *)caSekIndex2,(char *)caSekKey2);
	//dcs_debug("caSekIndex1=[%s],caSekKey1=[%s]",caSekIndex1,caSekKey1);
	//dcs_debug("caSekIndex2=[%s],caSekKey2=[%s]",caSekIndex2,caSekKey2);
	if( op1=='X')
	{
		asc_to_bcd(caSekKey1,caSekKey1,32,0);
		asc_to_bcd(caSekKey2,caSekKey2,32,0);
		asc_to_bcd(tmk,tmk,16,0);
		
		GenrateKey((unsigned char*)PikMak);
		
		//7F37A17F7F23E934	��ͨ����У��
		//C12F29575B6D3B3B	��ͨ����У��
		//asc_to_bcd(PikMak,"0123456789ABCDEF",16,0);//�ڴ˹ر�ֱ�Ӹ�ֵ
	
		//dcs_debug(PikMak,8,"PikMak data");
  }
  else
  {
  	asc_to_bcd(caSekKey1,caSekKey1,32,0);
		asc_to_bcd(caSekKey2,caSekKey2,32,0);
		asc_to_bcd(tmk,tmk,32,0);
		
		GenrateKey((unsigned char*)PikMak);
		GenrateKey((unsigned char*)PikMak+8);
		//7F37A17F7F23E934	��ͨ����У��
		//C12F29575B6D3B3B	��ͨ����У��
		//asc_to_bcd(PikMak,"0123456789ABCDEF",16,0);//�ڴ˹ر�ֱ�Ӹ�ֵ
	
		//dcs_debug(PikMak,16,"PikMak data");
  }
	if( op2== 'X')
	{
		  _3DES((unsigned char *)caSekKey1,(unsigned char *)tmk,(unsigned char *)mkey);
		  _3DES((unsigned char *)caSekKey1,(unsigned char *)tmk+8,(unsigned char *)mkey+8);
		  TripleDes((unsigned char *)caSekKey2,(unsigned char *)PikMak,(unsigned char *)PikMak1 );
//		  DES((unsigned char *)caSekKey2,(unsigned char *)PikMak,(unsigned char *)PikMak1);
//			DES((unsigned char *)mkey,(unsigned char *)PikMak,(unsigned char *)PikMak2);
      TripleDes((unsigned char *)mkey,(unsigned char *)PikMak,(unsigned char *)PikMak2 );
			//dcs_debug(caSekKey1,16,"caSekKey1 data");
			//dcs_debug(caSekKey2,16,"caSekKey2 data");
			//dcs_debug(mkey,16,"tmk data");
			//dcs_debug(PikMak1,8,"PikMak1 data");
			//dcs_debug(PikMak2,8,"PikMak2 data");
				
			bcd_to_asc((unsigned char*)buf,(unsigned char*)PikMak1,16,0);
			bcd_to_asc((unsigned char*)buf+16,(unsigned char*)PikMak2,16,0);
	}
	else if( op2== 'Y')
	{
			_3DES((unsigned char *)caSekKey1,(unsigned char *)tmk,(unsigned char *)mkey);
		  _3DES((unsigned char *)caSekKey1,(unsigned char *)tmk+8,(unsigned char *)mkey+8);
		  TripleDes((unsigned char *)caSekKey2,(unsigned char *)PikMak,(unsigned char *)PikMak1 );
		  TripleDes((unsigned char *)caSekKey2,(unsigned char *)PikMak+8,(unsigned char *)PikMak1+8 );
//		  DES((unsigned char *)caSekKey2,(unsigned char *)PikMak,(unsigned char *)PikMak1);
//			DES((unsigned char *)mkey,(unsigned char *)PikMak,(unsigned char *)PikMak2);
      TripleDes((unsigned char *)mkey,(unsigned char *)PikMak+8,(unsigned char *)PikMak2+8 );
			//dcs_debug(caSekKey1,16,"caSekKey1 data");
			//dcs_debug(caSekKey2,16,"caSekKey2 data");
			//dcs_debug(mkey,16,"tmk data");
			//dcs_debug(PikMak1,16,"PikMak1 data");
			//dcs_debug(PikMak2,16,"PikMak2 data");
				
			bcd_to_asc((unsigned char*)buf,(unsigned char*)PikMak1,32,0);
			bcd_to_asc((unsigned char*)buf+32,(unsigned char*)PikMak2,32,0);
 }
  *nOut=2;
	memcpy(outBuf+*nOut,"K3",2);

	*nOut=*nOut+2;

	memcpy(outBuf+*nOut,"00",2);
	*nOut= *nOut+2;
  if( op2=='X')
  {
		memcpy(outBuf+*nOut,buf,16);
		*nOut=*nOut+16;
  }
  else if( op2=='Y')
  {
		memcpy(outBuf+*nOut,buf,32);
		*nOut=*nOut+32;
  }
	memcpy(outBuf+*nOut,"0000000000000000",16);
	*nOut=*nOut+16;
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}

/*
����	��ANSI X9.9 MAC�㷨��������MAC��
*/
int DesGetMac(char *inBuf,int nIn,char *outBuf,int *nOut)
{
        int i,n;
	char buf[17],caTmpBuf[1024],caMac[48];
	
	//dcs_debug(0,0,"entry DesGetMac function");
	
	unsigned char caSekIndex[6],caSekKey[60],PikMak[17],mkey[17],datalen[5],mdata[1024];
	memset(caSekIndex,0,sizeof(caSekIndex));
	memset(caSekKey,0,sizeof(caSekKey));
	memset(PikMak,0,sizeof(PikMak));
	memset(mkey,0,sizeof(mkey));
	memset(datalen,0,sizeof(datalen));
	memset(mdata,0,sizeof(mdata));
	
	memcpy(caSekIndex,inBuf+1,5);
	memcpy(PikMak,inBuf+1+5+1,16);
	memcpy(datalen,inBuf+1+5+1+16,4);
	memcpy(mdata,inBuf+1+5+1+16+4,atoi((char *)datalen));
	
	GetMastKey((char *)caSekIndex,(char *)caSekKey);
	
	//dcs_debug(0, 0, "caSekIndex=[%s],caSekKey=[%s]",caSekIndex,caSekKey);
	
	asc_to_bcd(caSekKey,caSekKey,16,0);
	asc_to_bcd(PikMak,PikMak,16,0);
	
	_DES((unsigned char *)caSekKey,(unsigned char *)PikMak,(unsigned char *)mkey);
	
	//dcs_debug(caSekKey,8,"caSekKey data");
	//dcs_debug(mkey,8,"mkey data");
	
	//dcs_debug(mdata,atoi((char *)datalen),"mdata data");
		
	if ( atoi((char *)datalen)%8)
	{
		  //dcs_debug(0,0,"datalen ��Ϊ8�ı���[%s]",datalen);
			 return -1;
	}
	
	memset(caTmpBuf,0,sizeof(caTmpBuf));
	memset(caMac,0,sizeof(caMac));
	for( i=0; i<atoi((char *)datalen)/8; i++)
	{
		for( n=0;n<8;n++){
		  caMac[n]=caTmpBuf[n]^mdata[i*8+n];
		}
		DES(mkey,caMac,caTmpBuf);
		////dcs_debug(caTmpBuf,8,"mac temp data");
	}
		
	bcd_to_asc((unsigned char*)buf,(unsigned char*)caTmpBuf,16,0);

  *nOut=2;
	memcpy(outBuf+*nOut,"M1",2);

	*nOut=*nOut+2;

	memcpy(outBuf+*nOut,"00",2);
	*nOut= *nOut+2;

	memcpy(outBuf+*nOut,buf,8);
	*nOut=*nOut+8;
	
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}


/*
����	��ANSI X9.9 MAC�㷨��������MAC�����������MAC���бȽϣ����رȽϽ����
*/
int DesCheckMac(char *inBuf,int nIn,char *outBuf,int *nOut)
{
        int i,n;
	char buf[17],caTmpBuf[1024],caMac[48],MacData[48];
	
	//dcs_debug(0,0,"entry DesCheckMac function");
	
	unsigned char caSekIndex[6],caSekKey[60],PikMak[17],mkey[17],datalen[5],mdata[1024];
	memset(caSekIndex,0,sizeof(caSekIndex));
	memset(caSekKey,0,sizeof(caSekKey));
	memset(PikMak,0,sizeof(PikMak));
	memset(mkey,0,sizeof(mkey));
	memset(datalen,0,sizeof(datalen));
	memset(mdata,0,sizeof(mdata));
	
	memcpy(caSekIndex,inBuf+1,5);
	memcpy(PikMak,inBuf+1+5+1,16);
	memcpy(MacData,inBuf+1+5+1+16,8);
	memcpy(datalen,inBuf+1+5+1+16+8,4);
	memcpy(mdata,inBuf+1+5+1+16+8+4,atoi((char *)datalen));
	
	GetMastKey((char *)caSekIndex,(char *)caSekKey);
	
	//dcs_debug("caSekIndex=[%s],caSekKey=[%s]",caSekIndex,caSekKey);
	
	asc_to_bcd(caSekKey,caSekKey,16,0);
	asc_to_bcd(PikMak,PikMak,16,0);
	
	_DES((unsigned char *)caSekKey,(unsigned char *)PikMak,(unsigned char *)mkey);
	
	//dcs_debug(caSekKey,8,"caSekKey data");
	//dcs_debug(mkey,8,"mkey data");
	
	//dcs_debug(mdata,atoi((char *)datalen),"mdata data");
		
	if ( atoi((char *)datalen)%8)
	{
		  //dcs_debug(0,0,"datalen ��Ϊ8�ı���[%s]",datalen);
			 return -1;
	}
	
	memset(caTmpBuf,0,sizeof(caTmpBuf));
	memset(caMac,0,sizeof(caMac));
	for( i=0; i<atoi((char *)datalen)/8; i++)
	{
		for( n=0;n<8;n++)
		  caMac[n]=caTmpBuf[n]^mdata[i*8+n];
		DES(mkey,caMac,caTmpBuf);
//		//dcs_debug(caTmpBuf,8,"mac temp data");
	}
		
	bcd_to_asc((unsigned char*)buf,(unsigned char*)caTmpBuf,16,0);
	
	//dcs_debug(MacData,8,"MacData");
	//dcs_debug(buf,8,"caMac");

  *nOut=2;
	memcpy(outBuf+*nOut,"M1",2);

	*nOut=*nOut+2;

	if( memcmp(buf,MacData,8)==0 )
		memcpy(outBuf+*nOut,"00",2);
	else	memcpy(outBuf+*nOut,"01",2);
	*nOut= *nOut+2;

  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}

/*
����	��ԴPIN������ԴPIK���ܣ�����PIN��ʽת����Ȼ����Ŀ��PIK���������
*/
int DesTransPin(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char buf[17];
	
	//dcs_debug(0,0,"entry DesTransPin function");
	
	unsigned char caSekIndex1[6],caSekKey1[60],caSekIndex2[6],caSekKey2[60];
	unsigned char SourcePinBlock[17],DestPinBlock[17],mPinkBlock[17];
	unsigned char PinKey1[17],mKey1[17],PinKey2[17],mKey2[17];
	
	memset(buf,0,sizeof(buf));
	memset(caSekIndex1,0,sizeof(caSekIndex1));
	memset(caSekIndex2,0,sizeof(caSekIndex2));
	memset(caSekKey1,0,sizeof(caSekKey1));
	memset(caSekKey2,0,sizeof(caSekKey2));
	memset(SourcePinBlock,0,sizeof(SourcePinBlock));
	memset(DestPinBlock,0,sizeof(DestPinBlock));
	memset(mPinkBlock,0,sizeof(mPinkBlock));
	memset(PinKey1,0,sizeof(PinKey1));
	memset(PinKey2,0,sizeof(PinKey2));
	memset(mKey1,0,sizeof(mKey1));
	memset(mKey2,0,sizeof(mKey2));
	
	int i=0;
	memcpy(caSekIndex1,inBuf+i,5);i=i+5;
	memcpy(caSekIndex2,inBuf+i,5);i=i+5;i=i+1;
	memcpy(PinKey1,inBuf+i,16);i=i+16;i=i+1;
	memcpy(PinKey2,inBuf+i,16);i=i+16;i=i+4;
	memcpy(SourcePinBlock,inBuf+i,16);i=i+16;
	
	GetMastKey((char *)caSekIndex1,(char *)caSekKey1);
	GetMastKey((char *)caSekIndex2,(char *)caSekKey2);
	
	//dcs_debug(0,0,"caSekIndex1=[%s],caSekKey1=[%s]",caSekIndex1,caSekKey1);
	//dcs_debug(0,0,"caSekIndex2=[%s],caSekKey2=[%s]",caSekIndex2,caSekKey2);
	//dcs_debug(0,0,"PinKey1=[%s],PinKey2=[%s]",PinKey1,PinKey2);
	//dcs_debug(0,0,"SourcePinBlock=[%s]",SourcePinBlock);
	
	asc_to_bcd(caSekKey1,caSekKey1,16,0);
	asc_to_bcd(caSekKey2,caSekKey2,16,0);
	asc_to_bcd(PinKey1,PinKey1,16,0);
	asc_to_bcd(PinKey2,PinKey2,16,0);
	asc_to_bcd(SourcePinBlock,SourcePinBlock,16,0);
	
	_DES((unsigned char *)caSekKey1,(unsigned char *)PinKey1,(unsigned char *)mKey1);
	_DES((unsigned char *)caSekKey2,(unsigned char *)PinKey2,(unsigned char *)mKey2);
	
	//dcs_debug(caSekKey1,8,"caSekKey1 data");
	//dcs_debug(PinKey1,8,"PinKey1 data");
	//dcs_debug(mKey1,8,"mKey1 data");
	//dcs_debug(caSekKey2,8,"caSekKey2 data");
	//dcs_debug(PinKey2,8,"PinKey2 data");
	//dcs_debug(mKey2,8,"mKey2 data");
	
	_DES((unsigned char *)mKey1,(unsigned char *)SourcePinBlock,(unsigned char *)mPinkBlock);
	
	DES((unsigned char *)mKey2,(unsigned char *)mPinkBlock,(unsigned char *)DestPinBlock);
	
	//dcs_debug(SourcePinBlock,8,"SourcePinBlock",SourcePinBlock);
	//dcs_debug(mPinkBlock,8,"mPinkBlock",mPinkBlock);
	//dcs_debug(DestPinBlock,8,"DestPinBlock",DestPinBlock);
		
	bcd_to_asc((unsigned char*)buf,(unsigned char*)DestPinBlock,16,0);
	

  *nOut=2;
	memcpy(outBuf+*nOut,"P1",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,"00",2);
	*nOut= *nOut+2;
	memcpy(outBuf+*nOut,buf,16);
	*nOut= *nOut+16;
	
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}


/*
����	����SEK���ܵ�TMKת��Ϊ��TEK���ܣ����߽���TEK���ܵ�TMKת��Ϊ��SEK���ܡ�
*/
int DesTransTmk(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char buf[17];
	
	//dcs_debug(0,0,"entry DesTransTmk function");
	
	unsigned char caSekIndex[6],caSekKey[60],caTekIndex[6],caTekKey[60];
	unsigned char SourceTmk[17],DestTmk[17],TmpTmk[17];
	unsigned char TransBz[2];
	
	memset(buf,0,sizeof(buf));
	memset(caSekIndex,0,sizeof(caSekIndex));
	memset(caTekIndex,0,sizeof(caTekIndex));
	memset(caSekKey,0,sizeof(caSekKey));
	memset(caTekKey,0,sizeof(caTekKey));
	memset(SourceTmk,0,sizeof(SourceTmk));
	memset(DestTmk,0,sizeof(DestTmk));
	memset(TmpTmk,0,sizeof(TmpTmk));
	memset(TransBz,0,sizeof(TransBz));
	
	int i=0;
	memcpy(caSekIndex,inBuf+i,5);i=i+5;
	memcpy(caTekIndex,inBuf+i,5);i=i+5;
	memcpy(TransBz,inBuf+i,1);i=i+1;i=i+1;
	memcpy(SourceTmk,inBuf+i,16);i=i+16;
	
	GetMastKey((char *)caSekIndex,(char *)caSekKey);
	GetMastKey((char *)caTekIndex,(char *)caTekKey);
	
	//dcs_debug(0,0,"caSekIndex=[%s],caSekKey=[%s]",caSekIndex,caSekKey);
	//dcs_debug(0,0,"caTekIndex=[%s],caTekKey=[%s]",caTekIndex,caTekKey);
	//dcs_debug(0,0,"SourceTmk=[%s]",SourceTmk);
	//dcs_debug(0,0,"TransBz=[%s]",TransBz);
	
	asc_to_bcd(caSekKey,caSekKey,16,0);
	asc_to_bcd(caTekKey,caTekKey,16,0);
	asc_to_bcd(SourceTmk,SourceTmk,16,0);
	
	if( memcmp ( TransBz,"0",1 ) == 0 )
	{
		//dcs_debug(0,0,"��SEK��TEK����");
		
		_DES((unsigned char *)caSekKey,(unsigned char *)SourceTmk,(unsigned char *)TmpTmk);
	
		//dcs_debug(caSekKey,8,"caSekKey data");
		//dcs_debug(SourceTmk,8,"SourceTmk data");
		//dcs_debug(TmpTmk,8,"TmpTmk data");
	
		DES((unsigned char *)caTekKey,(unsigned char *)TmpTmk,(unsigned char *)DestTmk);
	
		//dcs_debug(caTekKey,8,"caTekKey data");
		//dcs_debug(TmpTmk,8,"TmpTmk data");
		//dcs_debug(DestTmk,8,"DestTmk data");
		
		bcd_to_asc((unsigned char*)buf,(unsigned char*)DestTmk,16,0);
	}
	else
	{
		//dcs_debug(0,0,"��TEK��SEK����");
		
		//_DES((unsigned char *)caSekKey,(unsigned char *)SourceTmk,(unsigned char *)TmpTmk);
		_DES((unsigned char *)caTekKey,(unsigned char *)SourceTmk,(unsigned char *)TmpTmk);
	
		//dcs_debug(caSekKey,8,"caTekKey data");
		//dcs_debug(SourceTmk,8,"SourceTmk data");
		//dcs_debug(TmpTmk,8,"TmpTmk data");
	
		//DES((unsigned char *)caTekKey,(unsigned char *)TmpTmk,(unsigned char *)DestTmk);
		DES((unsigned char *)caSekKey,(unsigned char *)TmpTmk,(unsigned char *)DestTmk);
	
		//dcs_debug(caTekKey,8,"caSekKey data");
		//dcs_debug(TmpTmk,8,"TmpTmk data");
		//dcs_debug(DestTmk,8,"DestTmk data");
		
		bcd_to_asc((unsigned char*)buf,(unsigned char*)DestTmk,16,0);
	}
	

  *nOut=2;
	memcpy(outBuf+*nOut,"KF",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,"00",2);
	*nOut= *nOut+2;
	memcpy(outBuf+*nOut,buf,16);
	*nOut= *nOut+16;
	
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}

int DesTransPikMak(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char buf[17];
	
	//dcs_debug(0,0,"entry DesTransPikMak function");
	
	unsigned char caSekIndex1[6],caSekKey1[60],caSekIndex2[6],caSekKey2[60];
	unsigned char SourcePikMak[17],DestPikMak[17],TmpPikMak[17];
	unsigned char SourceTmk[17],DestTmk[17];
	unsigned char TransBz[2];
	
	
	memset(buf,0,sizeof(buf));
	memset(caSekIndex1,0,sizeof(caSekIndex1));
	memset(caSekIndex1,0,sizeof(caSekIndex1));
	memset(caSekKey1,0,sizeof(caSekKey1));
	memset(caSekKey2,0,sizeof(caSekKey2));
	memset(SourcePikMak,0,sizeof(SourcePikMak));
	memset(DestPikMak,0,sizeof(DestPikMak));
	memset(TmpPikMak,0,sizeof(TmpPikMak));
	memset(SourceTmk,0,sizeof(SourceTmk));
	memset(DestTmk,0,sizeof(DestTmk));
	memset(TransBz,0,sizeof(TransBz));
	
	int i=0;
	memcpy(caSekIndex1,inBuf+i,5);i=i+5;
	memcpy(caSekIndex2,inBuf+i,5);i=i+5;i=i+1;
	memcpy(SourceTmk,inBuf+i,16);i=i+16;
	memcpy(TransBz,inBuf+i,1);i=i+1;i=i+1;
	memcpy(SourcePikMak,inBuf+i,16);i=i+16;
	
	GetMastKey((char *)caSekIndex1,(char *)caSekKey1);
	GetMastKey((char *)caSekIndex2,(char *)caSekKey2);
	
	//dcs_debug(0,0,"caSekIndex1=[%s],caSekKey1=[%s]",caSekIndex1,caSekKey1);
	//dcs_debug(0,0,"caSekIndex2=[%s],caSekKey2=[%s]",caSekIndex2,caSekKey2);
	//dcs_debug(0,0,"Tmk=[%s]",SourceTmk);
	//dcs_debug(0,0,"TransBz=[%s]",TransBz);
	//dcs_debug(0,0,"SourcePikMak=[%s]",SourcePikMak);
	
	asc_to_bcd(caSekKey1,caSekKey1,16,0);
	asc_to_bcd(caSekKey2,caSekKey2,16,0);
	asc_to_bcd(SourceTmk,SourceTmk,16,0);
	asc_to_bcd(SourcePikMak,SourcePikMak,16,0);
	
	_DES((unsigned char *)caSekKey2,(unsigned char *)SourceTmk,(unsigned char *)DestTmk);
	
	//dcs_debug(caSekKey2,8,"caSekKey2 data");
	//dcs_debug(SourceTmk,8,"SourceTmk data");
	//dcs_debug(DestTmk,8,"DestTmk data");
	
	if( memcmp ( TransBz,"0",1 ) == 0 )
	{
		//dcs_debug(0,0,"��SEK��TMK����");
		
		_DES((unsigned char *)caSekKey1,(unsigned char *)SourcePikMak,(unsigned char *)TmpPikMak);
	
		//dcs_debug(caSekKey1,8,"caSekKey1 data",caSekKey1);
		//dcs_debug(SourcePikMak,8,"SourcePikMak data",SourcePikMak);
		//dcs_debug(TmpPikMak,8,"TmpPikMak data",TmpPikMak);
		
		DES((unsigned char *)DestTmk,(unsigned char *)TmpPikMak,(unsigned char *)DestPikMak);

		//dcs_debug(DestTmk,8,"DestTmk data",DestTmk);
		//dcs_debug(TmpPikMak,8,"TmpPikMak data",TmpPikMak);
		//dcs_debug(DestPikMak,8,"DestPikMak data",DestPikMak);
		
		bcd_to_asc((unsigned char*)buf,(unsigned char*)DestPikMak,16,0);
	}
	else
	{
		//dcs_debug(0,0,"��TMK��TEK����");

		_DES((unsigned char *)DestTmk,(unsigned char *)SourcePikMak,(unsigned char *)TmpPikMak);
	
		//dcs_debug(DestTmk,8,"DestTmk data",caSekKey1);
		//dcs_debug(SourcePikMak,8,"SourcePikMak data",SourcePikMak);
		//dcs_debug(TmpPikMak,8,"TmpPikMak data",TmpPikMak);
		
		DES((unsigned char *)caSekKey1,(unsigned char *)TmpPikMak,(unsigned char *)DestPikMak);

		//dcs_debug(caSekKey1,8,"caSekKey1 data",DestTmk);
		//dcs_debug(TmpPikMak,8,"TmpPikMak data",TmpPikMak);
		//dcs_debug(DestPikMak,8,"DestPikMak data",DestPikMak);
		
		bcd_to_asc((unsigned char*)buf,(unsigned char*)DestPikMak,16,0);
	}
	

  *nOut=2;
	memcpy(outBuf+*nOut,"KJ",2);
	*nOut=*nOut+2;
	memcpy(outBuf+*nOut,"00",2);
	*nOut= *nOut+2;
	memcpy(outBuf+*nOut,buf,16);
	*nOut= *nOut+16;
	
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;	
}

/*
����	�������Ĵŵ�����
*/
int DecTrack(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char buf[1024],caTmpBuf[1024];
	
	//dcs_debug(0,0,"entry DecTrack function");
	
	unsigned char caIndex[6],caKey[60],Tdk[17],mkey[17],datalen[5],Trackdata[1024];
	memset(caIndex,0,sizeof(caIndex));
	memset(caKey,0,sizeof(caKey));
	memset(Tdk,0,sizeof(Tdk));
	memset(mkey,0,sizeof(mkey));
	memset(datalen,0,sizeof(datalen));
	memset(Trackdata,0,sizeof(Trackdata));
	
	int i=0;
	memcpy(caIndex,inBuf+i,5);i=i+5;i=i+1;
	memcpy(Tdk,inBuf+i,16);i=i+16;
	memcpy(datalen,inBuf+i,4);i=i+4;
	memcpy(Trackdata,inBuf+i,atoi((char *)datalen));
	
	GetMastKey((char *)caIndex,(char *)caKey);
	
	//dcs_debug(0, 0, "caIndex=[%s],caKey=[%s]",caIndex,caKey);
	
	asc_to_bcd(caKey,caKey,16,0);
	asc_to_bcd(Tdk,Tdk,16,0);
	
	_DES((unsigned char *)caKey,(unsigned char *)Tdk,(unsigned char *)mkey);
	
	//dcs_debug(caKey,8,"caKey data");
	//dcs_debug(Tdk,8,"Tdk data");
	//dcs_debug(mkey,8,"mkey data");
	
	//dcs_debug(Trackdata,atoi((char *)datalen),"Trackdata data");
		
	if ( atoi((char *)datalen)%8)
	{
		  //dcs_debug(0,0,"datalen ��Ϊ8�ı���[%s]",datalen);
			return -1;
	}
	
	//memset(caTmpBuf,0,sizeof(caTmpBuf));
	
	for( i=0; i<atoi((char *)datalen)/8; i++)
	{
		//for( n=0;n<8;n++)
		  //caMac[n]=caTmpBuf[n]^mdata[i*8+n];
		_DES(mkey,Trackdata+i*8,buf+i*8);
		//bcd_to_asc((unsigned char*)buf+i*16,(unsigned char*)caTmpBuf+i*8,16,0);
		////dcs_debug(caTmpBuf,8,"mac temp data");
	}
	

  *nOut=2;
	memcpy(outBuf+*nOut,"99",2);

	*nOut=*nOut+2;

	memcpy(outBuf+*nOut,"00",2);
	*nOut= *nOut+2;

	sprintf((char *)datalen,"%04d",atoi((char *)datalen));
	memcpy(outBuf+*nOut,datalen,4);
	*nOut=*nOut+4;
	
	memcpy(outBuf+*nOut,buf,atoi((char *)datalen));
	*nOut=*nOut+atoi((char *)datalen);
	
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}



/*
����	�������Ĵŵ�����
*/
int DecTrack2(char *inBuf1,int nIn,char *outBuf,int *nOut)
{
	
	char buf[1024],caTmpBuf[1024];
	
	//dcs_debug(0,0,"entry DecTrack function");
	
	unsigned char caIndex[6],caKey[60],Tdk[17],mkey[17],datalen[5],Trackdata[1024];
	memset(caIndex,0,sizeof(caIndex));
	memset(caKey,0,sizeof(caKey));
	memset(Tdk,0,sizeof(Tdk));
	memset(mkey,0,sizeof(mkey));
	memset(datalen,0,sizeof(datalen));
	memset(Trackdata,0,sizeof(Trackdata));

	int i=0;
    char *inBuf = inBuf1 + 2 ;
	memcpy(caIndex,inBuf + i, 5);i=i+5;i=i+1;
	memcpy(Tdk,inBuf+i,16);i=i+16;
	memcpy(datalen,inBuf+i,4);i=i+4;
	memcpy(Trackdata,inBuf+i,atoi((char *)datalen));
	
	GetMastKey((char *)caIndex,(char *)caKey);
	
	//dcs_debug(0, 0, "caIndex=[%s],caKey=[%s]",caIndex,caKey);
	
	asc_to_bcd(caKey,caKey,16,0);
	asc_to_bcd(Tdk,Tdk,16,0);
	//dcs_debug(caKey,8,"mastkey data");
	//dcs_debug(Tdk,8,"Tdk data");
	_DES((unsigned char *)caKey,(unsigned char *)Tdk,(unsigned char *)mkey);
	
	
	//dcs_debug(mkey,8,"mkey data");
	
	//dcs_debug(Trackdata,atoi((char *)datalen),"Trackdata data");
		
	if ( atoi((char *)datalen)%8)
	{
		  //dcs_debug(0,0,"datalen ��Ϊ8�ı���[%s]",datalen);
			return -1;
	}
	
	//memset(caTmpBuf,0,sizeof(caTmpBuf));
	
	for( i=0; i<atoi((char *)datalen)/8; i++)
	{
		//for( n=0;n<8;n++)
		  //caMac[n]=caTmpBuf[n]^mdata[i*8+n];
      if(inBuf1[0]==0x30)

        	DES(mkey,Trackdata+i*8,buf+i*8);
      else
			_DES(mkey,Trackdata+i*8,buf+i*8);
		//bcd_to_asc((unsigned char*)buf+i*16,(unsigned char*)caTmpBuf+i*8,16,0);
		////dcs_debug(caTmpBuf,8,"mac temp data");
	}
	

  *nOut=2;
	memcpy(outBuf+*nOut,"E2",2);

	*nOut=*nOut+2;

	memcpy(outBuf+*nOut,"00",2);
	*nOut= *nOut+2;

	sprintf((char *)datalen,"%04d",atoi((char *)datalen));
	memcpy(outBuf+*nOut,datalen,4);
	*nOut=*nOut+4;
	
	memcpy(outBuf+*nOut,buf,atoi((char *)datalen));
	*nOut=*nOut+atoi((char *)datalen);
	
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}


