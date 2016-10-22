#include "ibdcs.h"
#include <time.h>

void rollback_purchase(ISO_data *iso,int iFid,char *caMacIndex,char *caMacKey,char *caPinIndex,char *caPinKey)
{
  	
   char caPan[30],caIcData[100],caMac1[9],caRandom[5],caInputMode[4],caPin[9],caRtnCode[3],caAmount[13];
   char caPin1[13],caType[4],caShopId[9],caPosId[5],caTraceNo[7],caExpireDate[9],caBeginDate[9],caPwdFlag[2];
   char caTmpBuf[1024],caCryRtnCode[3],caMac[17],caDate[8];
   int nAppSeqNo,i,iLen;
	 struct  tm *tm;   time_t  t; 
	 
   memset(caPan ,0,sizeof(caPan));
   memset(caIcData,0,sizeof(caIcData));
   memset(caMac1,0,sizeof(caMac1));
   memset(caRandom,0,sizeof(caRandom));
   memset(caExpireDate,0,sizeof(caExpireDate));
   memset(caBeginDate,0,sizeof(caBeginDate));
  
   dcs_debug(0,0,"�������ѳ�������ģ��!");
   if (0>= getbit(iso,2,(unsigned char *)caPan))
   {
   		dcs_log(0,0,"2 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,4,(unsigned char *)caAmount))
   {
   		dcs_log(0,0,"4 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,11,(unsigned char *)caTraceNo))
   {
   		dcs_log(0,0,"4 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,41,(unsigned char *)caPosId))
   {
   		dcs_log(0,0,"4 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,42,(unsigned char *)caShopId))
   {
   		dcs_log(0,0,"4 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
	dcs_debug(0,0,"׼�����ԭʼ����״̬!");
	if ( 0>=CheckLogStatus(caShopId,caPosId,caTraceNo,caPan,caAmount,NULL,caRtnCode))
	{
			ErrResponseForPOS(iso,"96",iFid);
   		return;
	}
	dcs_debug(0,0,"ԭʼ����״̬������!");
	if ( memcmp(caRtnCode,"00",2 )!=0)
	{
		time(&t);
		tm = localtime(&t);
		sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		setbit (iso,0,(unsigned char *)"0410",4);
		
		setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
		setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
		setbit (iso,39,(unsigned char *)"00",2);
		setbit (iso,48,(unsigned char *)"",0);
		setbit (iso,52,(unsigned char *)"",0);
		setbit(iso,64,(unsigned char *)"00000000",8);
		sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
		 dcs_debug(caTmpBuf,iLen," calc mac data");
		 DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
		 asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
		 memcpy(caTmpBuf+iLen+5,caMac,8);
		 PrintISO( iso,"Ӧ����<����δ��ԭʼ����������>",0);
		 if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
	   {
	        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
	        return ;
	   }
	   txn_monitor ("10",iso );
		 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
		 return;
	}
	
	/* step 2 ���������Ҫ�� ���˻����Ǿ������˻��ۿ� */
	time(&t);
	tm = localtime(&t);
	sprintf(caDate,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
  if ( 0>=get_oldtype(caShopId,caPosId,caTraceNo,caDate,caType))
  {
 			ErrResponseForPOS(iso,"96",iFid);
			return;
  }

	/* step 5 �ۼ�����ȡ������ˮ�ţ�����������ˮ��*/
  i=0;
  i = getseqno();	
  if ( i <=0) 
  {
  		ErrResponseForPOS(iso,"96",iFid);
   		return;
  }
	/* step 6 �ǽ�����ˮ  */
	if ( 0 >=WriteAccountDbInVoid("10",caPan,caType,caAmount,'D',i,"000" /*hj "000000" */ ,caShopId,caPosId, caTraceNo))
	{
		 ErrResponseForPOS(iso,"96",iFid);
		 return;
	}
	
	/* step 7 ���Ӧ���ն� */
	time(&t);
	tm = localtime(&t);
	sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	setbit (iso,0,(unsigned char *)"0410",4);
	setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
	setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
	setbit (iso,39,(unsigned char *)"00",2);
	setbit (iso,48,(unsigned char *)"",0);
	setbit (iso,52,(unsigned char *)"",0);
	setbit(iso,64,(unsigned char *)"00000000",8);
	sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
	dcs_debug(caTmpBuf,iLen," calc mac data");
	DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
	asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
	memcpy(caTmpBuf+iLen+5,caMac,8);
	PrintISO( iso,"Ӧ����",0);
	if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
  {
        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
        return ;
  }
  txn_monitor ("10",iso);
	dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);

}
void  rollback_refund(ISO_data *iso,int iFid,char *caMacIndex,char *caMacKey,char *caPinIndex,char *caPinKey)
{

 char caPan[30],caIcData[100],caMac1[9],caRandom[5],caInputMode[4],caPin[9],caRtnCode[3],caAmount[13];
   char caPin1[13],caType[4],caShopId[9],caPosId[5],caOldTraceNo[7],caTraceNo[7],caExpireDate[9],caBeginDate[9],caPwdFlag[2];
   char caTmpBuf[1024],caCryRtnCode[3],caMac[17],caDate[8];
   int nAppSeqNo,i,iLen;
	 struct  tm *tm;   time_t  t; 
	 
   memset(caPan ,0,sizeof(caPan));
   memset(caIcData,0,sizeof(caIcData));
   memset(caMac1,0,sizeof(caMac1));
   memset(caRandom,0,sizeof(caRandom));
   memset(caExpireDate,0,sizeof(caExpireDate));
   memset(caBeginDate,0,sizeof(caBeginDate));
  
   dcs_debug(0,0,"���볷����������ģ��!");
   if (0>= getbit(iso,2,(unsigned char *)caPan))
   {
   		dcs_log(0,0,"2 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,4,(unsigned char *)caAmount))
   {
   		dcs_log(0,0,"4 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,11,(unsigned char *)caTraceNo))
   {
   		dcs_log(0,0,"11 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,41,(unsigned char *)caPosId))
   {
   		dcs_log(0,0,"41 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,42,(unsigned char *)caShopId))
   {
   		dcs_log(0,0,"42 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
	 if (0>= getbit(iso,63,(unsigned char *)caTmpBuf))
   {
   		dcs_log(0,0,"63 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   memset( caOldTraceNo,0,sizeof(caOldTraceNo));
   memcpy(caOldTraceNo,caTmpBuf,6);
	if ( 0>=CheckLogStatus(caShopId,caPosId,caTraceNo,caPan,caAmount,NULL,caRtnCode))
	{
			dcs_log(0,0,"����״̬���ʧ��!");
			ErrResponseForPOS(iso,"96",iFid);
   		return;
	}
	if ( memcmp(caRtnCode,"00",2 )!=0)
	{
		time(&t);
		tm = localtime(&t);
		sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		setbit (iso,0,(unsigned char *)"0410",4);
		
		setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
		setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
		setbit (iso,39,(unsigned char *)"00",2);
		setbit (iso,48,(unsigned char *)"",0);
		setbit (iso,52,(unsigned char *)"",0);
		setbit(iso,64,(unsigned char *)"00000000",8);
		sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
		 dcs_debug(caTmpBuf,iLen," calc mac data");
		 DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
		 asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
		 memcpy(caTmpBuf+iLen+5,caMac,8);
		 PrintISO( iso,"Ӧ����<����δ��ԭʼ����������>",0);
		 if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
	   {
	        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
	        return ;
	   }
	   txn_monitor ("30",iso);
		 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
		 return;
	}
	if ( 0>=CheckLogStatus2(caShopId,caPosId,caOldTraceNo,caPan,caAmount,NULL,caRtnCode))
	{
			dcs_log(0,0,"����״̬���ʧ��!");
			ErrResponseForPOS(iso,"96",iFid);
   		return;
	}
	if ( memcmp(caRtnCode,"00",2 )!=0)
	{
		time(&t);
		tm = localtime(&t);
		sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		setbit (iso,0,(unsigned char *)"0410",4);
		
		setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
		setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
		setbit (iso,39,(unsigned char *)"00",2);
		setbit (iso,48,(unsigned char *)"",0);
		setbit (iso,52,(unsigned char *)"",0);
		setbit(iso,64,(unsigned char *)"00000000",8);
		sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
		 dcs_debug(caTmpBuf,iLen," calc mac data");
		 DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
		 asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
		 memcpy(caTmpBuf+iLen+5,caMac,8);
		 PrintISO( iso,"Ӧ����<����δ��ԭʼ����������>",0);
		 if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
	   {
	        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
	        return ;
	   }
	   txn_monitor ("POSC",'1' ,iso,'1' );
		 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
		 return;
	}
	/* step 2 ����� ���˻����Ǿ������˻����� */
	
	time(&t);
	tm = localtime(&t);
	sprintf(caDate,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
  if ( 0>=get_oldtype(caShopId,caPosId,caTraceNo,caDate,caType))
  {
 			ErrResponseForPOS(iso,"96",iFid);
			return;
  }
	/* step 5 �ۼ�����ȡ������ˮ�ţ�����������ˮ��*/
  i=0;
  i = getseqno();	
  if ( i <=0) 
  {
  		dcs_log(0,0,"��ȡ������ˮ��ʧ��!");
  		ErrResponseForPOS(iso,"96",iFid);
   		return;
  }
	/* step 6 �ǽ�����ˮ  */
	if ( 0 >=WriteAccountDbInVoid("30",caPan,caType,caAmount,'C',i,"000" /*hj "000000" */ ,caShopId,caPosId, caOldTraceNo))
	{
		 ErrResponseForPOS(iso,"96",iFid);
		 return;
	}
	
	/* step 7 ���Ӧ���ն� */
	time(&t);
	tm = localtime(&t);
	sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	setbit (iso,0,(unsigned char *)"0410",4);
	setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
	setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
	setbit (iso,39,(unsigned char *)"00",2);
	setbit (iso,48,(unsigned char *)"",0);
	setbit (iso,52,(unsigned char *)"",0);
	setbit(iso,64,(unsigned char *)"00000000",8);
	sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
	dcs_debug(caTmpBuf,iLen," calc mac data");
	DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
	asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
	memcpy(caTmpBuf+iLen+5,caMac,8);
	PrintISO( iso,"Ӧ����",0);
	if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
  {
        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
        return ;
  }
  txn_monitor ("30",iso);
	dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
}

void rollback_load(ISO_data *iso,int iFid ,char *caMacIndex,char *caMacKey,char *caPinIndex,char *caPinKey)
{
  	
	 char caPan[30],caIcData[100],caMac1[9],caRandom[5],caInputMode[4],caPin[9],caRtnCode[3],caAmount[13];
   char caPin1[13],caType[4],caShopId[9],caPosId[5],caTraceNo[7],caExpireDate[9],caBeginDate[9],caPwdFlag[2];
   char caTmpBuf[1024],caCryRtnCode[3],caMac[17],caDate[8];
   int nAppSeqNo,i,iLen;
	 struct  tm *tm;   time_t  t; 
	 
   memset(caPan ,0,sizeof(caPan));
   memset(caIcData,0,sizeof(caIcData));
   memset(caMac1,0,sizeof(caMac1));
   memset(caRandom,0,sizeof(caRandom));
   memset(caExpireDate,0,sizeof(caExpireDate));
   memset(caBeginDate,0,sizeof(caBeginDate));
  
   
   if (0>= getbit(iso,2,(unsigned char *)caPan))
   {
   		dcs_log(0,0,"2 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,4,(unsigned char *)caAmount))
   {
   		dcs_log(0,0,"4 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,11,(unsigned char *)caTraceNo))
   {
   		dcs_log(0,0,"11 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,41,(unsigned char *)caPosId))
   {
   		dcs_log(0,0,"41 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,42,(unsigned char *)caShopId))
   {
   		dcs_log(0,0,"42 �򲻴���!");
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
	
	if ( 0>=CheckLogStatus(caShopId,caPosId,caTraceNo,caPan,caAmount,NULL,caRtnCode))
	{
			ErrResponseForPOS(iso,"96",iFid);
   		return;
	}
	if ( memcmp(caRtnCode,"00",2 )!=0)
	{
		time(&t);
		tm = localtime(&t);
		sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		setbit (iso,0,(unsigned char *)"0410",4);
		
		setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
		setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
		setbit (iso,39,(unsigned char *)"00",2);
		setbit (iso,48,(unsigned char *)"",0);
		setbit (iso,52,(unsigned char *)"",0);
		setbit(iso,64,(unsigned char *)"00000000",8);
		sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
		 dcs_debug(caTmpBuf,iLen," calc mac data");
		 DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
		 asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
		 memcpy(caTmpBuf+iLen+5,caMac,8);
		 PrintISO( iso,"Ӧ����<����δ��ԭʼ����������>",0);
		 if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
	   {
	        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
	        return ;
	   }
	   txn_monitor ("40",iso);
		 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
		 return;
	}
	
	time(&t);
	tm = localtime(&t);
	sprintf(caDate,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	if ( 0>=get_oldtype(caShopId,caPosId,caTraceNo,caDate,caType))
  {
 			ErrResponseForPOS(iso,"96",iFid);
			return;
  }
	/* step 5 �ۼ�����ȡ������ˮ�ţ�����������ˮ��*/
  i=0;
  i = getseqno();	
  if ( i <=0) 
  {
  		ErrResponseForPOS(iso,"96",iFid);
   		return;
  }
	/* step 6 �ǽ�����ˮ  */
	if ( 0 >=WriteAccountDbInVoid("40",caPan,caType,caAmount,'D',i,"000" /*hj "000000" */ ,caShopId,caPosId, caTraceNo))
	{
		 ErrResponseForPOS(iso,"96",iFid);
		 return;
	}
	
	/* step 7 ���Ӧ���ն� */
	time(&t);
	tm = localtime(&t);
	sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	setbit (iso,0,(unsigned char *)"0410",4);
	setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
	setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
	setbit (iso,39,(unsigned char *)"00",2);
	setbit (iso,48,(unsigned char *)"",0);
	setbit (iso,52,(unsigned char *)"",0);
	setbit(iso,64,(unsigned char *)"00000000",8);
	sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
	dcs_debug(caTmpBuf,iLen," calc mac data");
	DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
	asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
	memcpy(caTmpBuf+iLen+5,caMac,8);
	PrintISO( iso,"Ӧ����",0);
	if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
  {
        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
        return ;
  }
  txn_monitor ("40",iso);
	dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);

}
