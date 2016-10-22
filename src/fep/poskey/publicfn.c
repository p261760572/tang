#include <time.h>
#include "ibdcs.h"
#include  "tmcibtms.h"
#include  "dccglobl.h"
extern int gs_myFid   ;
extern char   *g_pcBcdaShmPtr;
static int LocateBankByName(char *pcaBankName);
int GetCommID(char *caRouteName)
{
	char *p;
	p=caRouteName;
	while ( *p!=0x20 && *p) p++;
	*p=0x00;
	return fold_locate_folder(caRouteName);
}
int GetPan(char *src,char *caPan, int flag)
{
	int i=0,s;
	char tmp[30],*p;
	p=caPan;
	 if (src==NULL || p == NULL)
	 	   return -1;
	 while( *src && *src !='=')
	 {
	 	 if ( i >20) return -2;
	 	 *p = *src;
	 	  p++;
	 	  src++;
	 	  i++;
	 }
	 if (*src !='=')
	 	   return -2;
	 if( flag)
	 {
	 	 strcpy(tmp,caPan);
	 	 sprintf(caPan,"0000",4);
	 	 memcpy(caPan+4,tmp+strlen(tmp)-13,12);
	 	  
	 } 
	 return 1;
}


void  Trim( char *p)
{
	 while ( *p && *p !=0x20)  *p++;
	 if ( *p) *p=0x00;
	 return ;
}

void  sh2dh( char *p) // "|"号转换为","
{
	int len,i;
	len=strlen(p);
	for(i=0;i<len;i++)
		if( memcmp(p+i,"|",1) == 0 )	memcpy(p+i,",",1);
	return ;
}

int IsReady(char *pcaName)
{
    int iBnkIdx;
    struct BCDAst *pBCDAst;

    pBCDAst = (struct BCDAst *)g_pcBcdaShmPtr;
    if(!pBCDAst)
    {
    	dcs_log(0,0,"通信机构共享内存区尚未连接就绪!");
    	 return -1;
    }

    iBnkIdx = LocateBankByName(pcaName);
    if(iBnkIdx < 0)  //invalid bank code
    {
    	dcs_debug(0,0,"LocateBankByName invalid pcaName =%s",	pcaName);
        return -1;
    }

    if(pBCDAst->stBcda[iBnkIdx].cNetFlag <= 0)
    {
        //无网络连接或没有发开始请求报文
    		dcs_debug(0,0,"cNetFlag is 0 net not connect [%s]",pcaName);
        return -1;
    }
 
    return 1;
}

static int LocateBankByName(char *pcaBankName)
{
    int    iBnkNum, i;
    struct BCDA *pBCDA;


    iBnkNum = ((struct BCDAst *)g_pcBcdaShmPtr)->iBankNum;
    pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;

    for(i=0; i < iBnkNum; i++)
    {
        if(memcmp(pcaBankName, pBCDA[i].caBankName,strlen(pcaBankName)) == 0)
            return (i);
    }

    return (-1);
}

int IsTimeOut(char * pDay, char * pTime)
{
     struct  tm *tm;   time_t  t;
     char caDay[3], caHour[3],caMin[3],caSec[3];
     long t1,t2;	
   
 
     if ( strlen(pDay)<2 || strlen(pTime) <6)
     {
				dcs_log(0,0,"无效的输入参数!");
        return 1; 
     }
     memset(caDay,0,sizeof(caDay));
     memset(caHour,0,sizeof(caHour));
     memset(caMin,0,sizeof(caMin));
     memset(caSec,0,sizeof(caSec)); 
     memcpy(caDay,pDay,2);
     memcpy(caHour,pTime,2);
     memcpy(caMin,pTime+2,2);
     memcpy(caSec,pTime+4,2);

     time(&t);
     tm = localtime(&t);
     t1=tm->tm_hour*3600+tm->tm_min*60+tm->tm_sec;
     t2=atoi(caHour)*3600+atoi(caMin)*60+atoi(caSec);
     if(atoi(caDay)==tm->tm_mday)
     {
        if( t1 -t2 <20)
                return 0;
        else 
						return 1;
     }else 
     {
         if( t1 >=20 )
							return 1;
         else
							return 0;
     }
     
}

void PrintISO( ISO_data *iso,char *caRemark,int flag)
{
	int i,s;
	char buffer[8192],tmpbuf[400],tmpbuf1[512],*p;
	memset(buffer,0,sizeof(buffer));
	p=&buffer[0];
	if ( caRemark !=NULL )
	{
		sprintf(p,"%s\n",caRemark);
		p=p+strlen(caRemark)+1;
	}
  for ( i =0; i<=128 ;i++)
	{
		s=getbit(iso,i,(unsigned char *)tmpbuf);
		if ( (p+s-&buffer[0]) >8192 ) return;
		if ( s >0)
		{
				sprintf(tmpbuf1,"第[%03d]域 =[%03d][%s]\n",i,s,tmpbuf);
				strcpy(p,tmpbuf1);
				p=p+strlen(tmpbuf1);
		}
		
	}
	if ( flag)
		dcs_log(0,0,buffer);
	else
		dcs_debug(0,0,buffer);
	return;
}


/*
  type  通道类型
  

*/

int txn_monitor ( char *type,ISO_data *iso)
{
	int qid,iRc,n;
	struct msqid_ds MsgQid_Buf;
	struct  tm *tm;   time_t  t;
	char buffer[1024],*p,tmpbuf[100];
//	dcs_debug(0,0,"entry notify"); 

	qid=queue_connect("monitor");
	if ( qid <=0 )
	{
	 	dcs_log(0,0,"File:%s,Line:%d: Connect Moni Error ",__FILE__,__LINE__);
	  return -1;
	}
	iRc = msgctl(qid,IPC_STAT,&MsgQid_Buf);
	if(iRc < 0)
	{
		dcs_log(0,0,"File:%s,Line:%d: Get Stat Error,Error Code = %d",__FILE__,__LINE__,iRc);
		
		return -1;
	}
	if(MsgQid_Buf.msg_qnum > 40)
	{
		dcs_log(0,0,"File:%s,Line:%d: Send Moni Message limited  ",__FILE__,__LINE__);
	  return -1;
	}	
/*
交易类型(2字节) 00签订， 01消费 02 查询 03撤消 04圈存 05转帐 10消费冲正 30 撤消冲正 40 圈存冲正
卡号(2字节长度＋内容)
交易金额(12字节)
转入账户信息(2字节长度＋内容)
流水号[6]
商户号(8字节)
终端号(4字节)
应答码(2字节)
日期(8字节)
时间(6字节)
*/
  memset(buffer,0,sizeof(buffer));
  p=&buffer[0];
  memcpy(p,type,2);
  p=p+2;
  n=getbit(iso,2,(unsigned char*)tmpbuf);
  sprintf(p,"%02d",n);
  p=p+2;
  memcpy(p,tmpbuf,n);
  p=p+n;
  getbit(iso,4,(unsigned char*)p);
  p=p+12;
  
  n=getbit(iso,44,(unsigned char*)tmpbuf);
  sprintf(p,"%02d",n);
  p=p+2;
  memcpy(p,tmpbuf,n);
  p=p+n;
  
  getbit(iso,11,(unsigned char*)p);
  p=p+6;
  getbit(iso,42,(unsigned char*)p);
  p=p+8;
  getbit(iso,41,(unsigned char*)p);
  p=p+4;
  getbit(iso,39,(unsigned char*)p);
  p=p+2;
  
  time(&t);
  tm = localtime(&t);
  sprintf(p,"%02d%02d%02d%02d%02d",tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
  p=p+10;   
	queue_send( qid, buffer,p-buffer,0);
//	dcs_debug(buffer,p-buffer," finilsh send data len=[%d]",p-buffer);
	return 1;
}
