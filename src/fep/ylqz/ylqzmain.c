#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <curl/curl.h>
#include <openssl/md5.h>

#include "ibdcs.h"
#include "tmcibtms.h"
#include "dbstruct.h"
#include "../db/inc/glbdb.h"
#include "CMDI.h"
#include "iso8583.h"
#include "bc.h"

typedef struct
{
  char *writePtr;
  int sizeLeft;
  int writeSize;
}WriteThis;

void md5(char *md5Str, char *md);
void ExpendApp(ISO_data *p_iso);
int curl_post(char *postStr, char *urlStr, WriteThis *pooh);
void TransactionQuery(ISO_data *p_iso);
void TransactionQueryReply(char *retStr, int retSize, ISO_data *p_iso);
int get_var(const char *var, const char *buf, int buf_len,	char *value, int value_len);

typedef struct
{
	char merAbbr[100 +1];				//商户名称
	char merId[25 + 1];                 //商户ID
	char cardCvn2[3 + 1];               //CVN2
	char cardExpire[4 + 1];            	//卡有效期
	char credentialNumber[19 + 1];     	//证件号码
	char credentialType[2 + 1];        	//证件类型
	char phoneNumber[12 + 1];          	//电话号码
	char credentialName[30 + 1];       	//持卡人姓名
	char cardNumber[19 + 1];           	//卡号
	char merAbstract[8 + 1];           	//商户摘要
	char orderNumber[32 + 1];           //订单号
	char orderTime[14 + 1];             //交易时间
	char transType[2 + 1];              //交易类型	
	char cardPasswd[174 + 1];			//密码信息
 	char orderAmount[12 + 1];			//交易金额
 	char exchangeDate[4 + 1];     		//兑换日期
	char exchangeRate[8 + 1];     		//兑换汇率
	char orderCurrency[3 + 1];    		//订单货币代码
	char qid[21 + 1];             		//银联交易流水号
	char respCode[2 + 1];         		//返回代码
	char respMsg[1024 + 1];       		//返回消息
	char respTime[14 + 1];        		//响应时间
	char settleAmount[12 + 1];    		//清算金额
	char settleCurrency[3 + 1];   		//清算货币代码
	char settleDate[4 + 1];       		//清算日期
	char traceNumber[6 + 1];      		//系统跟踪号
	char traceTime[10 + 1];       		//系统跟踪时间
	char cardType[2 + 1];
}trans_log_def;


int gs_myFid    = -1, gs_appSvrFid = -1;
char g_iSrcFoldName[30+1] ;
trans_log_def BcTransLog;


char BackEndUrl[100 + 1];  			//回调IP地址
char ExpendUrl[100 + 1];			//银联交易地址
char QyeryUrl[100 + 1];				//银联查询地址
char TransTimeout[10 + 1];			//超时时间
char SignaturePwd[8 + 1];			//签名密码
char SignatureMD5[32 + 1];			//签名密码MD5;
char KeyIndex[5 + 1];				//密钥索引
char InstIdCode[8 + 1];				//机构代码

static void appSrvExit(int signo)
{
    if ( signo > 0 )
        dcs_log(0,0,"<FILE:%s,LINE:%d>catch a signal %d\n",__FILE__,__LINE__,signo);
    if ( signo !=0 && signo != SIGTERM && signo != SIGSEGV )
        return;

    dcs_log(0,0,"<FILE:%s,LINE:%d>AppServer terminated.\n",__FILE__,__LINE__);
    DasEnd( COMMIT );
    shm_detach((char *)g_pcBcdaShmPtr);
    exit(signo);
}

static int OpenLog(char *IDENT)
{
    char logfile[256];

    if ( u_fabricatefile("log/ylqzapp.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

static size_t write_callback(void *buffer, size_t size, size_t nmemb, void *userp)
{
	WriteThis *pooh = (WriteThis *)userp;
	int writeSize;
	if(size*nmemb < 1)  return 0;
	writeSize =  pooh->sizeLeft > size * nmemb ? size * nmemb : pooh->sizeLeft ;
  	memcpy(pooh->writePtr, buffer, writeSize);
  	pooh->sizeLeft -= writeSize;
  	pooh->writePtr += writeSize;
  	pooh->writeSize += writeSize;
	return size*nmemb;        
}

/*处理终端渠道上送交易报文*/
int process( char* srcBuf,int  iFid, int iLen )
{
	ISO_data iso;
	char buff[1024];
	char msgtype[5],process[7];
	int len;
    /*解包*/
    memset(&iso, 0, sizeof(iso));
    if ( 0!= BC_SetIsoMode(&iso, "JAVA") )
    {
        dcs_log(srcBuf, iLen, "<FILE:%s,LINE:%d><BC_UnPack>BC_SetIsoMode failure",__FILE__,__LINE__);
        return -1;
    }    
    if ( 0 > BC_UnPack_Data( &iso, srcBuf, iLen) )
    {
        dcs_log(srcBuf,iLen,"<FILE:%s,LINE:%d>Can not unpacket Terminal Msg!datalen=%d",__FILE__,__LINE__,iLen);
        return;
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d>g_iSrcFoldName[%s]",__FILE__,__LINE__,g_iSrcFoldName);

    memset(msgtype,0,sizeof(msgtype));
    memset(process,0,sizeof(process));
    PrintISO( &iso,"收到报文",0);
   	if ( (len = getbit(&iso,  0, buff)) > 0 )
    {
        memcpy(msgtype, buff, MIN(len,  4));
    }
    if ( (len = getbit(&iso,  3, buff)) > 0 )
    {
        memcpy(process, buff, MIN(len,  6));
    }

	if(memcmp(process, "810001", 6) == 0) //借记卡
	{
		if(memcmp(msgtype, "0820", 4) == 0) //交易申请
		{
			ExpendApp(&iso);
		}
		else if(memcmp(msgtype, "0620", 4) == 0)//交易查询
		{
			TransactionQuery(&iso);			
		}
		else
	    	dcs_log(0,0, "<FILE:%s,LINE:%d>Msgtype[%s] error!",__FILE__,__LINE__,msgtype);
	}
	else
	{
    	dcs_log(0,0, "<FILE:%s,LINE:%d>Process[%s] error!",__FILE__,__LINE__,process);
	}	
}

int getval(char *val, char *valValue, char *str, int size)
{
	char *p;
	if(memcmp(str, val, strlen(val)) == 0)
	{
		for(; *str != 0 && *str != '='; str++);
		if(*str == 0) return 0;
		str++;
		for(; *str !=0 && *str == ' '; str++);
		for(p = str; *p !=0 && *p != ';'; p++);
		*p=0;
		memcpy(valValue, str, MIN(p-str, size-1));
		return 1;
	}
	return 0;
}

static int LoadConfig()
{
	char cfgfile[256], caBuffer[1024];
	int flag;
	FILE *fd;
	strcpy(caBuffer, "config/ylqz.conf");
	if(u_fabricatefile(caBuffer, cfgfile, sizeof(cfgfile)) < 0)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, caBuffer);
		return -1;
	}
	fd = fopen(cfgfile, "r");	
	if(fd == NULL)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, cfgfile);
	}
	memset(caBuffer,0,sizeof(caBuffer));
	flag = 0;
	for(;!feof(fd);)
	{
		if(fgets(caBuffer, 1024, fd) == NULL) break;
		if(caBuffer[0] == '#') continue;					
		if(getval("backEndUrl",	BackEndUrl, caBuffer, sizeof(BackEndUrl)))
			flag |= 1;
		else if(getval("ExpendUrl", ExpendUrl, caBuffer, sizeof(ExpendUrl)))
			flag |= 2; 
		else if(getval("QyeryUrl", QyeryUrl, caBuffer, sizeof(QyeryUrl)))
			flag |= 4;
		else if(getval("transTimeout", TransTimeout, caBuffer, sizeof(TransTimeout)))
			flag |= 8;
		else if(getval("SignaturePwd", SignaturePwd, caBuffer, sizeof(SignaturePwd)))
		{
			memset(SignatureMD5, 0, sizeof(SignatureMD5));
			md5(SignaturePwd, SignatureMD5);
			flag |= 0x10;
		}
		else if(getval("KeyIndex", KeyIndex, caBuffer, sizeof(KeyIndex)))
		{
			flag |= 0x20;
		}
		else if(getval("InstIdCode", InstIdCode, caBuffer, sizeof(InstIdCode)))
		{
			flag |= 0x40;
		}
	}		
	fclose(fd);	
	if(flag != 0x7f)
	{
		if(!(flag&1))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[backEndUrl] failed!\n",__FILE__,__LINE__);			
		else if(!(flag&2))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[ExpendUrl] failed!\n",__FILE__,__LINE__);				
		else if(!(flag&4))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[QyeryUrl] failed!\n",__FILE__,__LINE__);			
		else if(!(flag&8))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[transTimeout] failed!\n",__FILE__,__LINE__);	
		else if(!(flag&0x10))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[SignaturePwd] failed!\n",__FILE__,__LINE__);	
		else if(!(flag&0x20))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[KeyIndex] failed!\n",__FILE__,__LINE__);	
       	else if(!(flag&0x40))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[InstIdCode] failed!\n",__FILE__,__LINE__);	
       	else
       		dcs_log(0,0,"<FILE:%s,LINE:%d>read config para failed![%02x]", __FILE__, __LINE__, flag);	
        return -1;
	}
    dcs_debug(0, 0, "<FILE:%s,LINE:%d>\nbackEndUrl[%s]\nExpendUrl[%s]\nQyeryUrl[%s]\nTransTimeout[%s]\nSignaturePwd[%s]\nKeyIndex[%s]\nInstIdCode[%s]\n", __FILE__, __LINE__, BackEndUrl, ExpendUrl, QyeryUrl, TransTimeout, SignaturePwd, KeyIndex, InstIdCode);
	return 1;
}

static int DoLoop()
{
    //char   caBuffer[4096];
    char *caBuffer = (char *)g_Rcv_Buffer;
    int iRead, fromFid;
    for ( ;; )
    {
        g_ForkFlag = 0;
        //read from my folder in blocking mode
        memset(caBuffer, 0, sizeof(caBuffer));
        iRead = fold_read( gs_myFid, &fromFid, g_Rcv_Buffer, sizeof(g_Rcv_Buffer)-8, 1);
        if ( iRead < 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d>fold_read() failed:%d\n",__FILE__,__LINE__,(errno));
            break;
        }
		if(memcmp(g_Rcv_Buffer, "HTTP", 4) == 0)//http异步返回结果
		{
			ExpendAppReply(g_Rcv_Buffer+4, iRead-4, 1, NULL);
		}
		else
		{
	        g_RcvBufferSize = iRead;
	        g_iSrcFoldId = fromFid;
	        dcs_debug(0,0, "<FILE:%s,LINE:%d>srcfolder[%d]!",__FILE__,__LINE__,g_iSrcFoldId);	
	        fold_get_name(g_iSrcFoldId, g_iSrcFoldName, 30);
	        g_iSrcFoldName[30] = 0;
	
	        //folder msg
	        memset(&g_folder_msg,0,sizeof(g_folder_msg));
	        memcpy(g_folder_msg.packet_type, "JAVA", 4);
	        g_folder_msg.srcFoldid = g_iSrcFoldId;
	        g_folder_msg.OutFoldType = 0;
	        memset(g_srcPacketType,0, sizeof(g_srcPacketType) );
	        memcpy(g_srcPacketType, "JAVA", 4);
	        dcs_debug(caBuffer, iRead, "recv data len=%d(from foldid:[%d],foldName[%s],packet_type:[%s])", iRead,g_iSrcFoldId, g_iSrcFoldName, g_srcPacketType);
	
	        //交易信息
	        process(caBuffer,fromFid,iRead);
	    }
    }//for(;;)

    return -1;
}

static int CreateMyFolder()
{
    if ( fold_initsys() < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d>cannot attach to folder system!", __FILE__, __LINE__);
        return -1;
    }
    
    gs_myFid = fold_create_folder("YLQZ");
    if ( gs_myFid < 0 )
        gs_myFid = fold_locate_folder("YLQZ");
    if ( gs_myFid < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>cannot create folder '%s':%d\n", __FILE__, __LINE__, "YLQZ", (errno) );
        return -1;
    }

    dcs_log(0,0, "<FILE:%s,LINE:%d>folder myFid=%d\n",__FILE__,__LINE__, gs_myFid);

    if ( fold_get_maxmsg(gs_myFid) <2 )
        fold_set_maxmsg(gs_myFid, 20) ;

    return 0;
}


int main(int argc, char *argv[])
{
    catch_all_signals(appSrvExit);
    if ( 0 > OpenLog(argv[0]))
    {
        BC_SendExeception("04","<FILE:%s,LINE:%d>打开日志失败，ylqzapp进程启动失败，退出");
        appSrvExit(0);
    }
    dcs_log(0,0, "<FILE:%s,LINE:%d>ylqzapp Servers is starting up...\n",__FILE__,__LINE__);
    
    u_daemonize(NULL);
    
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",__FILE__,__LINE__ ,strerror(errno));
        appSrvExit(0);
    }
    
    g_pcBcdaShmPtr = (char *)shm_connect("BCDA",NULL);
    if ( g_pcBcdaShmPtr == NULL )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><appSrv> cannot connect shared memory 'BCDA'!",__FILE__,__LINE__);
        appSrvExit(0);
    }

    if ( IsoLoad8583config(&iso8583_JAVAconf[0],"iso8583_JAVA.conf") < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d> Load JAVA system interface iso8583_JAVAconf() failed:%s\n",__FILE__,__LINE__,strerror(errno));
        appSrvExit(0);
    }

    if ( CreateMyFolder() < 0 )
    {
        dcs_log(0, 0, "创建folder文件夹失败，ylqzapp进程启动失败，退出");
        appSrvExit(0);
    }
    
    gs_appSvrFid = fold_locate_folder("APPL");
    if ( gs_appSvrFid < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>cannot create folder '%s':%d\n",__FILE__,__LINE__, "APPL", (errno) );
        appSrvExit(0);
    }
   

    //do my main logic in a infinite loop    

    dcs_log(0,0,"*************************************************\n"
            "!!      YLQZ Application Servers startup completed. !!\n"
            "*************************************************\n");

    if ( 1 != DESInit() )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d> DESInit() failed\n",__FILE__,__LINE__);
        appSrvExit(0);
    }
    
    if(1 != LoadConfig())
    {
    	appSrvExit(0);
    }
    DoLoop();
    appSrvExit(0);
}

int curl_post(char *postStr, char *urlStr, WriteThis *pooh)
{
	CURL *curl;
	CURLcode res;
	FILE *headerfile;
	const char *pPassphrase = NULL;
	
	static const char *pCertFile = "client.pem";
	static const char *pCACertFile="ca.pem";
	const char *pKeyName;
	const char *pKeyType;

	pKeyName  = "client_key.pem";
	pKeyType  = "PEM";
	curl = curl_easy_init();
	if(curl) 
	{
		/* what call to write: */ 
		curl_easy_setopt(curl, CURLOPT_URL, urlStr);
		while(1)                    /* do some ugly short cut... */ 
    	{
			/* cert is stored PEM coded in file... */ 
			/* since PEM is default, we needn't set it for PEM */ 
//			curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
 
			/* set the cert for client authentication */ 
//			curl_easy_setopt(curl, CURLOPT_SSLCERT, pCertFile);
 
			/* if we use a key stored in a crypto engine, 
			 we must set the key type to "ENG" */ 
//			curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, pKeyType);
 
			/* set the private key (file or ID in engine) */ 
//			curl_easy_setopt(curl, CURLOPT_SSLKEY, pKeyName);
			
			/* set the file with the certs vaildating the server */ 
//			curl_easy_setopt(curl, CURLOPT_CAINFO, "imm.pem");
 
			/* disconnect if we can't validate server's cert */ 
//			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
//			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, pooh);		
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postStr);
		
			dcs_debug(0, 0, "<FILE:%s,LINE:%d>PostStr = %s",  __FILE__, __LINE__, postStr);
			res = curl_easy_perform(curl);
		
			if(res != 0)
			{
        		dcs_log(0,0,"<FILE:%s,LINE:%d> url_easy_perform return %d\n", __FILE__, __LINE__, res);
			}
			break;                 
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return res;
}
