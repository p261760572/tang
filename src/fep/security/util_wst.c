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


//huang jia add 20100114 奇校验函数，校验入口数据（1个字节），如果符合奇校验则返回此原数据，否则处理后返回符合奇校验的数据
int calcParity(unsigned char code)
{
	unsigned char i;
	unsigned char count = 0;

	for(i=0;i<8;i++)
	{
		if(code & (1<<i))		//直接统计1的个数；当然，统计0也一样，有奇数个0的时候必然有奇数个1
		{
			count++;
		}
	}
	
	if ( count %2 )	return 1;
	else	return -1;
		
//	if ( count %2 )	return code;
//	else	return code&0xfe^(code&0x01^0x01);//取得code的最低位并取反
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
//      dcs_debug(0,0,"s=%d",s);
      p= (unsigned char*)&s;
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
  	
  	
  	//设定commlnk定义文件的绝对路径，并打开
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
  	if(getenv("ICS_HOME")!=NULL)
  	{
	  	sprintf(caFileName, "%s/config/%s",getenv("ICS_HOME"),"mastkey.dat");
	    dcs_debug(0,0,"file=[%s]",caFileName);
    }
  	if ((iFd = conf_open(caFileName)) < 0)
  	{
    		dcs_log( 0,0, " open mastkey.dat fail ! ");
				return -1;
  	}
  	dcs_debug(0,0,"key Index =%s",Index);
    if ( 0 >conf_get_first_string(iFd, Index,buf ))
    {
            dcs_log(0,0, "load configuration item %s failed!",Index);
            conf_close(iFd);
            return (-1);
    }
  	dcs_debug(0,0,"buf =%s",buf);

    memcpy(key,buf,32);
  	conf_close(iFd);
    
  	return 1;
}


int DesGetKey(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char key[17],buf[17];
	
	memset(key,0,sizeof(key));
	dcs_debug(0,0,"entry DesGetKey function");
	GenrateKey((unsigned char*)key);
	
	//huang jia add 20100114 临时直接赋值,以后再做修改
	//7F37A17F7F23E934	可通过奇校验
	//C12F29575B6D3B3B	可通过奇校验
	//asc_to_bcd(key,"C12F29575B6D3B3B",16,0);//在此关闭直接赋值

	dcs_debug(key,8,"key data");
	
	//huang jia add 20100114 将key做des加密运算
	unsigned char caIndex[4],caMastKey[60],mkey[17];
	memset(caIndex,0,sizeof(caIndex));
	memset(caMastKey,0,sizeof(caMastKey));
	memset(mkey,0,sizeof(mkey));
	memcpy(caIndex,inBuf,3);
	if ( 0>GetMastKey((char *)caIndex,(char *)caMastKey))
	{
		  dcs_debug(0,0,"can not get mastkey[%s]",caIndex);
		  return -1;
	}
	dcs_debug(0,0,"caIndex=[%s],caMastKey=[%s]",caIndex,caMastKey);
	asc_to_bcd(caMastKey,caMastKey,16,0);
	DES((unsigned char *)caMastKey,(unsigned char *)key,(unsigned char *)mkey);
	memcpy(key,mkey,8);
	dcs_debug(caMastKey,8,"caMastKey data");
	dcs_debug(key,8,"mkey data");
	
	
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



int DesGetTmk(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char tmk[32],buf[32],type;
	int indx;
	unsigned char caSekIndex[6],caTekIndex[6],caSekKey[60],caTekKey[60],mkey1[17],mkey2[17];
	memset(tmk,0,sizeof(tmk));
	
	dcs_debug(0,0,"<%s> begin",__FUNCTION__);
	GenrateKey((unsigned char*)tmk);
	GenrateKey((unsigned char*)tmk+8);
	
	dcs_debug(tmk,16,"tmk data");
	
	memset(caSekIndex,0,sizeof(caSekIndex));
	memset(caTekIndex,0,sizeof(caTekIndex));
	memset(caSekKey,0,sizeof(caSekKey));
	memset(caTekKey,0,sizeof(caTekKey));
	memset(mkey1,0,sizeof(mkey1));
	memset(mkey2,0,sizeof(mkey2));
	indx=inBuf[0]*256+inBuf[1];
	sprintf(caSekIndex,"%04d",indx);
	indx=inBuf[2]*256+inBuf[3];
	sprintf(caTekIndex,"%04d",indx);
	type=inBuf[4];
	GetMastKey((char *)caSekIndex,(char *)caSekKey);
	GetMastKey((char *)caTekIndex,(char *)caTekKey);
	dcs_debug(0,0,"<%s> caSekIndex=[%s],caSekKey=[%s]",__FUNCTION__,caSekIndex,caSekKey);
	dcs_debug(0,0,"<%s>caTekIndex=[%s],caTekKey=[%s]",__FUNCTION__,caTekIndex,caTekKey);
	asc_to_bcd(caSekKey,caSekKey,32,0);
	asc_to_bcd(caTekKey,caTekKey,32,0);
	TripleDes((unsigned char *)caSekKey,(unsigned char *)tmk,(unsigned char *)mkey1 );
	TripleDes((unsigned char *)caSekKey,(unsigned char *)tmk+8,(unsigned char *)mkey1+8 );
//	DES((unsigned char *)caSekKey,(unsigned char *)tmk,(unsigned char *)mkey1);
//	DES((unsigned char *)caTekKey,(unsigned char *)tmk,(unsigned char *)mkey2);
	TripleDes((unsigned char *)caTekKey,(unsigned char *)tmk,(unsigned char *)mkey2 );
	TripleDes((unsigned char *)caTekKey,(unsigned char *)tmk+8,(unsigned char *)mkey2+8 );
	dcs_debug(caSekKey,16,"caSekKey data");
	dcs_debug(caTekKey,16,"caTekKey data");
	dcs_debug(mkey1,16,"tmk1 data");
	dcs_debug(mkey2,16,"tmk2 data");
	if(type == 0)
	{
		bcd_to_asc((unsigned char*)buf,(unsigned char*)mkey1,16,0);
		bcd_to_asc((unsigned char*)buf+16,(unsigned char*)mkey2,16,0);
	}
	else 	if(type == 1)
	{
		bcd_to_asc((unsigned char*)buf,(unsigned char*)mkey1,32,0);
		bcd_to_asc((unsigned char*)buf+32,(unsigned char*)mkey2,32,0);
  }
  *nOut=2;
	memcpy(outBuf+*nOut,"A",2);

	*nOut=*nOut+1;

	
//	memcpy(outBuf+*nOut,buf,64);
	if( type == 0 )
	{	
		memcpy(outBuf+*nOut,buf,32);
		*nOut=*nOut+32;
  }
	else
	{
			memcpy(outBuf+*nOut,buf,64);
			*nOut=*nOut+64;
	}
	TripleDes((unsigned char *)tmk,(unsigned char *)"\x00\x00\x00\x00\x00\x00\x00\x00",(unsigned char *)outBuf+*nOut );
	*nOut=*nOut+8;
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;
  
	return 1;
}

/*
功能	生成数据密钥PIK/MAK，并将其密文和检查值返回给主机。
说明	对于生成的数据密钥（PIK、MAK）需要同时各生成两对密文，其中一对用TMK加密用于通过联机报文方式在签到应答消息中传给终端，另一对用SEK加密后，将密文保存在POSP数据库中。
*/
int DesGetPikMak(char *inBuf,int nIn,char *outBuf,int *nOut)
{
	char PikMak[33],buf[100],op1,op2,op3;
	int n;
	memset(PikMak,0,sizeof(PikMak));
	
	dcs_debug(0,0,"<%s> bein",__FUNCTION__);

	
	unsigned char caSekIndex1[6],caSekIndex2[6],caSekKey1[60],caSekKey2[60],tmk[33],mkey[33],PikMak1[33],PikMak2[33];
	memset(caSekIndex1,0,sizeof(caSekIndex1));
	memset(caSekIndex2,0,sizeof(caSekIndex2));
	memset(caSekKey1,0,sizeof(caSekKey1));
	memset(caSekKey2,0,sizeof(caSekKey2));
	memset(tmk,0,sizeof(tmk));
	memset(mkey,0,sizeof(mkey));
	memset(PikMak1,0,sizeof(PikMak1));
	memset(PikMak2,0,sizeof(PikMak2));
	
	n=inBuf[0]*256+inBuf[1];
	sprintf(caSekIndex1,"%04d",n);
	n=inBuf[2]*256+inBuf[3];
	sprintf(caSekIndex2,"%04d",n);
	n=4;
	op1=inBuf[n];
	n++;
	op2=inBuf[n];
	n++;
	op3=inBuf[3];
	if( op3 ==0)
	{
		memcpy(tmk,inBuf+n,8);
		n=n+8;
  }
  else if( op3 ==1)
	{
		memcpy(tmk,inBuf+n,16);
		n=n+32;
  }else if( op3 ==2)
	{
		memcpy(tmk,inBuf+n,24);
		n=n+48;
  }
  
	GetMastKey((char *)caSekIndex1,(char *)caSekKey1);
	GetMastKey((char *)caSekIndex2,(char *)caSekKey2);
	dcs_debug(0,0,"caSekIndex1=[%s],caSekKey1=[%s]",caSekIndex1,caSekKey1);
	dcs_debug(0,0,"Tek Index=[%s],tekKey=[%s]",caSekIndex2,caSekKey2);
	if( op1==0)
	{
		asc_to_bcd(caSekKey1,caSekKey1,32,0);
		asc_to_bcd(caSekKey2,caSekKey2,32,0);
		asc_to_bcd(tmk,tmk,16,0);
		
		GenrateKey((unsigned char*)PikMak);
		dcs_debug(PikMak,8,"PikMak mw data");
  }
  else if( op1==1)
  {
  	asc_to_bcd(caSekKey1,caSekKey1,32,0);
		asc_to_bcd(caSekKey2,caSekKey2,32,0);
		asc_to_bcd(tmk,tmk,32,0);
		
		GenrateKey((unsigned char*)PikMak);
		GenrateKey((unsigned char*)PikMak+8);
		dcs_debug(PikMak,16,"PikMak mw data");
  }
	
  *nOut=2;
	memcpy(outBuf+*nOut,"A",1);

	*nOut=*nOut+1;
	
  if( op1==0)
  {
		asc_to_bcd(outBuf+*nOut,buf,32,0);
		*nOut=*nOut+16;
  }
  else if( op2==1)
  {
		asc_to_bcd(outBuf+*nOut,buf,64,0);
		*nOut=*nOut+32;
  }
  TripleDes((unsigned char *)PikMak,(unsigned char *)"\x00\x00\x00\x00\x00\x00\x00\x00",(unsigned char *)buf);
	dcs_debug(buf,8,"<%s> check values",__FUNCTION__);
  memcpy(outBuf+*nOut,buf,8);
	*nOut=*nOut+8;
  outBuf[0]=(*nOut-2)/256;
  outBuf[1]=(*nOut-2)%256;  
	return 1;
}

int DesToRsaKey (char *inBuf,int nIn,char *outBuf,int *nOut)
{
	
}