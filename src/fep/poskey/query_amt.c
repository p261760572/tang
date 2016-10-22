#include "ibdcs.h"
#include <time.h>

void query_amt(ISO_data *iso,int iFid,char *caMacIndex,char *caMacKey,char *caPinIndex,char *caPinKey)
{
   char caPan[30],caIcData[100],caRandom[5],caInputMode[4],caPin[9],caRtnCode[3],caAmount[14],caTmpBuf[1024];
   char caPin1[13],caExpireDate[9],caBeginDate[9],caPwdFlag[2],caType[4],caShopId[9],caPosId[5],caTraceNo[11],caCryRtnCode[3],caMac[17];
   int i,iLen;
    struct  tm *tm;   time_t  t; 
    
   memset(caPan ,0,sizeof(caPan));
   memset(caIcData,0,sizeof(caIcData));
  
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
   if (memcmp(caInputMode,"051",3)==0)
   {
   	  if ( 0>=getbit(iso,52,(unsigned char *)caPin))
   	  {	
   	  	ErrResponseForPOS(iso,"30",iFid);
   			return;
   		}
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
   if (0>= getbit(iso,48,(unsigned char *)caIcData))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   WriteDb(iso,"02");
   memset(caBeginDate,0,sizeof(caBeginDate));
   memset(caExpireDate,0,sizeof(caExpireDate));
   bcd_to_asc((unsigned char *)caBeginDate,(unsigned char *)caIcData+11,8,0);
   bcd_to_asc((unsigned char *)caExpireDate,(unsigned char *)caIcData+15,8,0);
   dcs_debug(0,0,"begin date=[%s],expire date=[%s]",caBeginDate,caExpireDate);
   
   if ( is_verify_pwd(caShopId))
   {
   	   iLen=get_pwd(caPan,'1',caPin1);
   }
   i=table_query_result('0',caPan,"000",caBeginDate,caExpireDate,caAmount,caRtnCode);
    if ( i  == -1 )
   {
   		   	UpdateDb(caShopId,caPosId, caTraceNo,"96",caPan,0,'0');
					ErrResponseForPOS(iso,"14",iFid);
					return;
   }
   else if ( i == -2)
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
   UpdateDb(caShopId,caPosId, caTraceNo,"00",caPan,0,'1');

	 time(&t);
	 tm = localtime(&t);
	sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	setbit (iso,0,(unsigned char *)"0210",4);
	
	setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
	setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
	setbit (iso,39,(unsigned char *)"00",2);
	setbit (iso,48,(unsigned char *)"",0);
	setbit (iso,52,(unsigned char *)"",0);
	setbit (iso, 54,(unsigned char *)caAmount,13);
	setbit(iso,64,(unsigned char *)"00000000",8);
	sprintf(caTmpBuf,"02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
	 dcs_debug(caTmpBuf,iLen," calc mac data");
	 DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
	 asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
	 memcpy(caTmpBuf+iLen+5,caMac,8);
	 PrintISO( iso,"应答报文",0);
	 if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
   {
        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
        return ;
   }
   txn_monitor ("02",iso);
	 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5); 
}
void query_subamt(ISO_data *iso,int iFid,char *caMacIndex,char *caMacKey,char *caPinIndex,char *caPinKey)
{
	
	 char caPan[30],caIcData[100],caMac1[9],caRandom[5],caInputMode[4],caPin[9],caRtnCode[3],caAmount[14],caTmpBuf[1024];
   char caPin1[13],caExpireDate[9],caBeginDate[9],caPwdFlag[2],caType[4],caShopId[9],caPosId[5],caTraceNo[11],caCryRtnCode[3],caMac[17];
   int i,iLen;
   struct  tm *tm;   time_t  t; 
    
   memset(caPan ,0,sizeof(caPan));
   memset(caIcData,0,sizeof(caIcData));
  
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
   if (memcmp(caInputMode,"051",3)==0)
   {
   	  if ( 0>=getbit(iso,52,(unsigned char *)caPin))
   	  {	
   	  	ErrResponseForPOS(iso,"30",iFid);
   			return;
   		}
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
   if (0>= getbit(iso,48,(unsigned char *)caIcData))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   i=WriteDb(iso,"02");
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
  if ( 0>get_shopinfo(caShopId,caType,caPwdFlag))
	{
		UpdateDb(caShopId,caPosId, caTraceNo,"96",caPan,0,'0');
		ErrResponseForPOS(iso,"96",iFid);
		return;
	}
	dcs_debug(0,0,"应用类型=[%s]",caType);
  if (0< getbit(iso,18,(unsigned char *)caTmpBuf))
  {
         memcpy(caType,caTmpBuf+1,3);
         caType[3]=0x00;  
  }
   memset(caBeginDate,0,sizeof(caBeginDate));
   memset(caExpireDate,0,sizeof(caExpireDate));
   bcd_to_asc((unsigned char *)caBeginDate,(unsigned char *)caIcData+11,8,0);
   bcd_to_asc((unsigned char *)caExpireDate,(unsigned char *)caIcData+15,8,0);
   dcs_debug(0,0,"begin date=[%s],expire date=[%s]",caBeginDate,caExpireDate);
   if ( is_verify_pwd(caShopId))
   {
   	   iLen=get_pwd(caPan,'1',caPin1);
   }
   i=table_query_result('1',caPan,caType,caBeginDate,caExpireDate,caAmount,caRtnCode);
   if ( i  == -1 )
   {
   		   	UpdateDb(caShopId,caPosId, caTraceNo,"96",caPan,0,'0');
					ErrResponseForPOS(iso,"14",iFid);
					return;
   }
   else if ( i == -2)
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
   UpdateDb(caShopId,caPosId, caTraceNo,"00",caPan,0,'1');

	 time(&t);
	 tm = localtime(&t);
	sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	setbit (iso,0,(unsigned char *)"0210",4);
	
	setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
	setbit ( iso,13,(unsigned char *)caTmpBuf+4,4);
	setbit (iso,39,(unsigned char *)"00",2);
	setbit (iso,48,(unsigned char *)"",0);
	setbit (iso,52,(unsigned char *)"",0);
	setbit (iso, 54,(unsigned char *)caAmount,13);
	setbit(iso,64,(unsigned char *)"00000000",8);
	sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
	 dcs_debug(caTmpBuf,iLen," calc mac data");
	 DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
	 asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
	 memcpy(caTmpBuf+iLen+5,caMac,8);
	 PrintISO( iso,"应答报文",0);
	 if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
   {
        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
        return ;
   }
   txn_monitor ("02",iso);
	 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5); 
}

