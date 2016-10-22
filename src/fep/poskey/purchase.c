#include "ibdcs.h"
#include <time.h>

void purchase(ISO_data *iso,int iFid,char *caMacIndex,char *caMacKey,char *caPinIndex,char *caPinKey)
{
	 char caPan[30],caIcData[100],caMac1[9],caRandom[5],caInputMode[4],caPin[9],caRtnCode[3],caAmount[13];
   char caPin1[13],caType[4],caShopId[9],caPosId[5],caTraceNo[7],caExpireDate[9],caBeginDate[9],caPwdFlag[2];
   char caTmpBuf[1024],caCryRtnCode[3],caMac[17];
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
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,22,(unsigned char *)caInputMode))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (memcmp(caInputMode,"051",3)==0 )
   {
   	  if ( 0>=getbit(iso,52,(unsigned char *)caPin))
   	  {	
   	  	ErrResponseForPOS(iso,"30",iFid);
   			return;
   		}
   }
   else if (memcmp(caInputMode,"052",3)!=0 )
 	 {
   	  	dcs_log(0,0,"输入方式错误,[%s]",caInputMode);
   	  	ErrResponseForPOS(iso,"30",iFid);
   			return;
   }
   	
   if (0>= getbit(iso,48,(unsigned char *)caIcData))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,4,(unsigned char *)caAmount))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,11,(unsigned char *)caTraceNo))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,41,(unsigned char *)caPosId))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,42,(unsigned char *)caShopId))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
 
   bcd_to_asc((unsigned char *)caBeginDate,(unsigned char *)caIcData+11,8,0);
   bcd_to_asc((unsigned char *)caExpireDate,(unsigned char *)caIcData+15,8,0);
  i =-1;
  i= WriteDb(iso,"01");
  if ( i== -1)
  {	
  	ErrResponseForPOS(iso,"94",iFid);
		return;
  }
  else if ( i <0)
  {
  	ErrResponseForPOS(iso,"96",iFid);
		return;
  }
   
  	
	/* step 2 检查消费需要在 主账户还是具体子账户扣款 */
	if ( 0>get_shopinfo(caShopId,caType,caPwdFlag))
	{
		UpdateDb(caShopId,caPosId, caTraceNo,"96",caPan,0,'0');
		ErrResponseForPOS(iso,"96",iFid);
		return;
	}
	if (0< getbit(iso,18,(unsigned char *)caTmpBuf))
  {
         memcpy(caType,caTmpBuf+1,3);
         caType[3]=0x00;  
  }
	/* step 3 检查账户状态 ,启用日期是否已过，卡片是否过期,资金是否充足,若不充足，检查是否可以透支，透支金额是否超过限制*/
	if ( memcmp(caPwdFlag,"1",1)==0)
		 i= CheckPanInfo(caPan,caType,caAmount,caExpireDate,caBeginDate,'1',caPin,caRtnCode) ;
	else
		 i= CheckPanInfo(caPan,caType,caAmount,caExpireDate,caBeginDate,'0',NULL,caRtnCode) ;
	
	if (  i == -1 )
	{
		UpdateDb(caShopId,caPosId, caTraceNo,"14",caPan,0,'0');
		ErrResponseForPOS(iso,"14",iFid);
		return;
	}
	else if ( i ==-2)
	{
		UpdateDb(caShopId,caPosId, caTraceNo,"96",caPan,0,'0');
		ErrResponseForPOS(iso,"96",iFid);
		return;
	}
	if ( memcmp(caRtnCode,"00",2)!=0)
	{
		UpdateDb(caShopId,caPosId, caTraceNo,caRtnCode,caPan,0,'0');
		ErrResponseForPOS(iso,caRtnCode,iFid);
		return;
	}
	/* step 5 扣减金额，获取记帐流水号，并记帐务流水，*/
  i=0;
  i = getseqno();	
  if ( i <=0) 
  {
  		UpdateDb(caShopId,caPosId, caTraceNo,"96",caPan,0,'0');
  		ErrResponseForPOS(iso,"96",iFid);
   		return;
  }
	/* step 6 记交易流水  */
	if ( 0 >=WriteAccountDb("01",caPan,caType,caAmount,'C',i,"000")) /*hj "000000")) */
	{
		UpdateDb(caShopId,caPosId, caTraceNo,"96",caPan,0,'0');
		ErrResponseForPOS(iso,"96",iFid);
		return;
	}
	UpdateDb(caShopId,caPosId, caTraceNo,"00",caPan,i,'1');
	/* step 7 打包应答终端 */
	 time(&t);
	 tm = localtime(&t);
	sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	setbit (iso,0,(unsigned char *)"0210",4);
	
	setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
	setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
	setbit (iso,39,(unsigned char *)"00",2);
	setbit (iso,48,(unsigned char *)"",0);
	setbit (iso,22,(unsigned char *)"",0);
	setbit (iso,14,(unsigned char *)"",0);
	setbit (iso,52,(unsigned char *)"",0);
	setbit(iso,64,(unsigned char *)"00000000",8);
	sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
	 dcs_debug(caTmpBuf+8+5,iLen-8," calc mac data");
	 DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
	 asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
	 memcpy(caTmpBuf+iLen+5,caMac,8);
	 PrintISO( iso,"应答报文",0);
	 if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
   {
        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
        return ;
   }
   txn_monitor ("01",iso);
	 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
}
