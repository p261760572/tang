#include "ibdcs.h"
#include <time.h>

void check(ISO_data *iso,int iFid,char *caMacIndex,char *caMacKey,char *caPinIndex,char *caPinKey) /* ×ªÕË    */
{
	 char caPan[30],caSeqNo[7],caRtnCode[3];
   char caShopId[9],caPosId[5];
   char caDate[5],caTime[7],caEndDate[5];
   char caTmpBuf[1024],caCryRtnCode[3],caMac[17];
   int nAppSeqNo,i,iLen;
	 struct  tm *tm;   time_t  t; 
	 
   memset(caPan ,0,sizeof(caPan));
   memset(caSeqNo,0,sizeof(caSeqNo));
   memset(caTime,0,sizeof(caTime));
   memset(caDate,0,sizeof(caDate));
   memset(caEndDate,0,sizeof(caEndDate));
   memset(caPosId,0,sizeof(caPosId));
   memset(caShopId,0,sizeof(caShopId));
  
   
   if (0>= getbit(iso,2,(unsigned char *)caPan))
   {
   		//ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,11,(unsigned char *)caSeqNo))
   {
   		//ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,12,(unsigned char *)caTime))
   {
   		//ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,13,(unsigned char *)caDate))
   {
   		//ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,14,(unsigned char *)caEndDate))
   {
   		//ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   
   if (0>= getbit(iso,41,(unsigned char *)caPosId))
   {
   		//ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   if (0>= getbit(iso,42,(unsigned char *)caShopId))
   {
   		//ErrResponseForPOS(iso,"30",iFid);
   		return;
   }
   
   dcs_log(caPan,sizeof(caPan),"[02Óò]caPan=[%s]\n",caPan);
   dcs_log(caSeqNo,sizeof(caSeqNo),"[11Óò]caSeqNo=[%s]\n",caSeqNo);
   dcs_log(caTime,sizeof(caTime),"[12Óò]caTime=[%s]\n",caTime);
   dcs_log(caDate,sizeof(caDate),"[13Óò]caDate=[%s]\n",caDate);
   dcs_log(caEndDate,sizeof(caEndDate),"[14Óò]caEndDate=[%s]\n",caEndDate);
   dcs_log(caPosId,sizeof(caPosId),"[41Óò]caPosId=[%s]\n",caPosId);
   dcs_log(caShopId,sizeof(caShopId),"[42Óò]caShopId=[%s]\n",caShopId);
   
  WriteCheckDb(caPan,caPosId,caSeqNo,caDate,caTime);
 
	time(&t);
	tm = localtime(&t);
	sprintf(caTmpBuf,"%04d%02d%02d%02d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	setbit (iso,0,(unsigned char *)"0851",4);
	setbit (iso,2,(unsigned char *)caPan,20);
	setbit (iso,11,(unsigned char *)caSeqNo,6);
	
//	setbit (iso,12,(unsigned char *)caTime,6);
//	setbit (iso,13,(unsigned char *)caDate,4);
	setbit (iso,12,(unsigned char *)caTmpBuf+8,6);
	setbit (iso,13,(unsigned char *)caTmpBuf+4,4);
	
	setbit (iso,14,(unsigned char *)caEndDate,4);
	setbit (iso,39,(unsigned char *)"00",2);
	setbit (iso,41,(unsigned char *)caPosId,4);
	setbit (iso,42,(unsigned char *)caShopId,8);
	sprintf(caTmpBuf,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	iLen=isotostr((unsigned char *)caTmpBuf+8+5,iso);
	dcs_debug(caTmpBuf+8+5,iLen-8," calc mac data");
	DESMACOP(caCryRtnCode, caMac, caMacIndex,caMacKey,iLen-8,caTmpBuf+8+5);
	asc_to_bcd((unsigned char *)caMac,(unsigned char *)caMac,16,0);
	memcpy(caTmpBuf+iLen+5,caMac,8);
	PrintISO( iso,"Ó¦´ð±¨ÎÄ",0);
	if( 0 > fold_write(iFid,-1,caTmpBuf,iLen+8+5) )
  {
        dcs_log(caTmpBuf,iLen+8+5,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
        return ;
  }
	dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
}
