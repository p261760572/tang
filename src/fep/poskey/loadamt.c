#include "ibdcs.h"
#include <time.h>

void loadAmt(ISO_data *iso,int iFid,char *caMacIndex,char *caMacKey,char *caPinIndex,char *caPinKey) /* 圈存    */
{
	 char caPan[30],caIcData[100],caIcData1[100],caMac1[9],caRandom[5],caInputMode[4],caPin[9],caRtnCode[3],caAmount[13];
   char caPin1[13],caType[4],caShopId[9],caPosId[5],caTraceNo[7],caExpireDate[9],caBeginDate[9],caPwdFlag[2];
   char caTmpBuf[1024],caCryRtnCode[3],caMac[17],caInitData[100],data[200];
   int nAppSeqNo,i,iLen,length;
	 struct  tm *tm;   time_t  t; 
	 //hj begin
	 char caPurse[2];
	 //end
	 
   memset(caPan ,0,sizeof(caPan));
   memset(caIcData,0,sizeof(caIcData));
   memset(caMac1,0,sizeof(caMac1));
   memset(caRandom,0,sizeof(caRandom));
   memset(caExpireDate,0,sizeof(caExpireDate));
   memset(caBeginDate,0,sizeof(caBeginDate));
   
   //hj begin
   memset(caPurse,0,sizeof(caPurse));
   //end
  
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
   if (0>= getbit(iso,58,(unsigned char *)caIcData1))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   bcd_to_asc((unsigned char *)caBeginDate,(unsigned char *)caIcData+11,8,0);
   bcd_to_asc((unsigned char *)caExpireDate,(unsigned char *)caIcData+15,8,0);
   //hj begin
   memcpy(caPurse,caIcData+19,1);
   dcs_log(0,0,"卡信息:BeginDate=%s,ExpireDate=%s,Purse=%s\n", caBeginDate,caExpireDate,caPurse);
   //end
   i=WriteDb(iso,"04");
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
	  memcpy(caInitData,caIcData1+10,14);
	  memcpy(caInitData+14,caIcData1+26,2);
	  memcpy(caInitData+16,"\x80\x00",2);
	  memcpy(data,caIcData1+28,15);
	  
   if ( 0 >=DESICMAC(caCryRtnCode, caMac, "009",caInitData, 15, data))
   {
   		UpdateDb(caShopId,caPosId, caTraceNo,"96",caPan,0,'0');
   		ErrResponseForPOS(iso,"96",iFid);
   		return;;
   }
   if ( memcmp(caCryRtnCode,"00",2)!=0)
   {
   		UpdateDb(caShopId,caPosId, caTraceNo,"99",caPan,0,'0');
   		ErrResponseForPOS(iso,"99",iFid);
   		return;;
   }
   if ( memcmp(caMac,caIcData1+43,4)!=0)
   {
   		dcs_log(caMac,4,"本地计算的mac1!");
   		UpdateDb(caShopId,caPosId, caTraceNo,"A0",caPan,0,'0');
   		ErrResponseForPOS(iso,"A0",iFid);
   		return;;
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
  else 
  	 strcpy(caType,"000");
  	 
  //hj begin 检查钱包和子账户是否有对应关系
   if (0>= CmpPurse(caPurse,caType))
   {
   		ErrResponseForPOS(iso,"96",iFid);
   		return;
   }
  //end
    
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
	time(&t);
	tm = localtime(&t);
	sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	asc_to_bcd((unsigned char *)caTmpBuf,(unsigned char *)caTmpBuf,14,0);
  asc_to_bcd((unsigned char *)data,(unsigned char *)caAmount+4,8,0);
  data[0]=(atol(caAmount)&(0xff000000))>>24;
  data[1]=(atol(caAmount)&(0xff0000))>>16;
  data[2]=(atol(caAmount)&(0xff00))>>8;
  data[3]=(atol(caAmount)&(0xff));
  memcpy(data+4,caIcData1+36,7);
  memcpy(data+11,caTmpBuf,7);
  if ( 0 >=DESICMAC(caCryRtnCode, caMac, "009",caInitData, 18, data))
  {
   		UpdateDb(caShopId,caPosId, caTraceNo,"96",caPan,0,'0');
   		ErrResponseForPOS(iso,"96",iFid);
   		return;;
  }
  if ( memcmp(caCryRtnCode,"00",2)!=0)
  {
   		dcs_log(0,0,"计算mac2错误!");
   		UpdateDb(caShopId,caPosId, caTraceNo,"99",caPan,0,'0');
   		ErrResponseForPOS(iso,"99",iFid);
   		return;;
  }
  memcpy(caTmpBuf+7,caMac,4);
  setbit(iso,58,(unsigned char*)caTmpBuf,11);
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
	if ( 0 >=WriteAccountDb("04",caPan,caType,caAmount,'C',i,"000")) /*hj "000000")) */
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
   txn_monitor ("04",iso);
	 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
  /* step 1 检查mac1 是否有效 */
	
	/* step 2 检查主账户是否有效 ，启用日期是否已过，卡片是否过期*/
	
	/* step 3 检查转出账户资金是否充足 */
	
	/* step 4 计算mac2 */
	
	/* step 6 扣减主账户金额，获取记帐流水号，并记帐务流水，*/
		
	/* step 8 记交易流水  */
	
	/* step 9 打包应答终端 */
  
}
