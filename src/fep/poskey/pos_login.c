#include "ibdcs.h"
#include <time.h>

void pos_login(ISO_data *iso,char *caPinIndex,char *caMacIndex, int iFid)
{
 char caMacKey[33],caPinKey[33],caCryRtnCode[3],caTmpBuf[1024];
 char caShopId[16],caPosId[7],caMac[17],TraceNo[7];       
 struct  tm *tm;   time_t  t;       
 int iLen,i=0;
 
     memset(caMacKey,0,sizeof(caMacKey));
 		 memset(caPinKey,0,sizeof(caPinKey));
 		 memset(caShopId,0,sizeof(caShopId));
 		 memset(caPosId,0,sizeof(caPosId));
 		 
 		 getbit(iso,11,(unsigned char*)TraceNo);
 		 getbit(iso,41,(unsigned char*)caPosId);
 		 getbit(iso,42,(unsigned char*)caShopId);
 		 i=WriteDb(iso,"00");
 		 if ( i == -1)
 		 {
// 		 	 ErrResponseForPOS(iso,"94",iFid);
        ;
 		 }
 		 else if ( i == -2 )
 		 {
 		 	  ErrResponseForPOS(iso,"96",iFid);
 		 	  return;
 		 }
 		 if ( 0>DESGETKY(caCryRtnCode, caMacKey, caMacIndex))
 		 {
 		 	  UpdateDb(caShopId,caPosId, TraceNo,"96","",0,'0');
 		 	  ErrResponseForPOS(iso,"96",iFid);
 		 	  return;
 		 }
 		 else if(0> DESGETKY(caCryRtnCode, caPinKey, caPinIndex))
 		 {
 		 	  UpdateDb(caShopId,caPosId, TraceNo,"96","",0,'0');
 		 	  ErrResponseForPOS(iso,"96",iFid);
 		 	  return;
 		 }
 		 else
 		 {
 		 		 
 		 		if (0> UpdateTerminalPara(caShopId,caPosId,caMacKey,caPinKey))
	   		{
	   		 	  dcs_log(0,0,"更新自助终端参数失败!商户号=%s,终端号=%s,caMacKey=[%s],caPinKey=[%s]",
	   		 	                caShopId,caPosId,caMacKey,caPinKey);
	   		 	  UpdateDb(caShopId,caPosId, TraceNo,"96","",0,0);
	   		 	  ErrResponseForPOS(iso,"96",iFid);
	   		 		return ;
	   		}
	   	dcs_debug(0,0,"shop=%s,pos=%s,no=%s",caShopId,caPosId, TraceNo);
	   	 UpdateDb(caShopId,caPosId, TraceNo,"00","",0,'0');
       setbit(iso,39,(unsigned char *)"00",2);
   		 memset(caTmpBuf,0,sizeof(caTmpBuf));
   		 memcpy(caTmpBuf,caMacKey,16);
   		 memcpy(caTmpBuf+16,caPinKey,16);
   		 setbit(iso,48,(unsigned char *)caTmpBuf,32);

 		 }
 		 setbit(iso,64,(unsigned char *)"00000000",8);
 		 setbit(iso,43,(unsigned char *)"0",0);
 		 setbit(iso,0,(unsigned char *)"0810",4);
 		 memset(caTmpBuf,0,sizeof(caTmpBuf));
 		 time(&t);
		 tm = localtime(&t);
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
	   txn_monitor ("00",iso);
 		 dcs_debug(caTmpBuf,iLen+8+5,"return terminal data!len=%d",iLen+8+5);
}
