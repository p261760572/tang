#include "ibdcs.h"
#include <time.h>

void batch_proc(ISO_data *iso,int iFid,char *caMacIndex,char *caMacKey,char *caPinIndex,char *caPinKey)
{
	 char caPan[30],caIcData[100],caRtnCode[3],caAmount[13],caAccount[10];
   char caShopId[9],caPosId[5],caTraceNo[7],caDate[9],caTime[7];
   char caTmpBuf[1024],caCryRtnCode[3],caMac[17],caTac[9];
   int i,n,iLen;
	 struct  tm *tm;   time_t  t; 
	 //hj begin
	 char caPurse[2];
	 //end
	 
   memset(caPan ,0,sizeof(caPan));
   memset(caIcData,0,sizeof(caIcData));
   memset(caAccount ,0,sizeof(caAccount));
   
   //hj begin
   memset(caPurse,0,sizeof(caPurse));
   //end
      
   if (0>= getbit(iso,2,(unsigned char *)caPan))
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
/*   
   if (0>= getbit(iso,12,(unsigned char *)caTime))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
    if (0>= getbit(iso,13,(unsigned char *)caDate))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
*/
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
   if (0>= getbit(iso,58,(unsigned char *)caIcData))
   {
   		ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   n= (unsigned char)caIcData[26]*256+(unsigned char)caIcData[27];
   bcd_to_asc((unsigned char *)caTac,(unsigned char *)caIcData+51,8,0);
   memset(caTmpBuf,0,sizeof(caTmpBuf));
   bcd_to_asc((unsigned char *)caTmpBuf,(unsigned char *)caIcData+20,62,0);
   memset(caTime,0,sizeof(caTime));
   memset(caDate,0,sizeof(caDate));
   bcd_to_asc((unsigned char *)caTime,(unsigned char *)caIcData+47,6,0);
   bcd_to_asc((unsigned char *)caDate,(unsigned char *)caIcData+43,8,0);
   
   //hj begin
   memcpy(caPurse,caIcData+54,1);
   dcs_log(0,0,"钱包标志:Purse=%s\n", caPurse);
   //end
      
   //begin
   /*GetAccount(caPan,caAccount);*/
   //end
   i=WriteOfflineDb(caPan,caAmount,caTraceNo,caDate,caTime,caShopId,caPosId,n,caTac,caTmpBuf,caPurse);
   
   if ( i == -1)
   {
   	  ErrResponseForPOS(iso,"94",iFid);
   	  return;
   }
   else if ( i <0)
   {
   		ErrResponseForPOS(iso,"96",iFid);
   	  return;
   }
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
	setbit (iso,58,(unsigned char *)"",0);
	setbit (iso,48,(unsigned char *)"",0);
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
   txn_monitor ("99",iso);
	 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
}
