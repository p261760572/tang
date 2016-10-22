#include <stdio.h>
#include <string.h> 
#include <curl/curl.h>
#include <openssl/md5.h>

#include "iso8583.h"
#include "bc.h"

typedef struct
{
	char merAbbr[100 +1];				//商户名称
	char merId[25 + 1];                 //商户ID
	char cardCvn2[3 + 1];               //CVN2
	char cardExpire[4 + 1];            	//卡有效期
	char credentialNumber[19 + 1];    	//证件号码
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
 
extern trans_log_def BcTransLog;
extern char BackEndUrl[100 + 1];  			//回调IP地址
extern char TransTimeout[10 + 1];			//超时时间
extern char SignatureMD5[32 +1];
extern char ExpendUrl[100 + 1];				//银联交易地址
extern char QyeryUrl[100 + 1];				//银联查询地址
extern char KeyIndex[5 + 1];				//密钥索引
extern char InstIdCode[8 + 1];				//机构代码
extern int gs_myFid, gs_appSvrFid;

typedef struct {
  char *writePtr;
  int sizeLeft;
  int writeSize;
}WriteThis;


void ExpendAppReply(char *retStr, int retSize, int flag, ISO_data *p_iso);
void md5(char *md5Str, char *md);
void ExpendApp(ISO_data *p_iso);
int curl_post(char *postStr, char *urlStr, WriteThis *pooh);
void TransactionQuery(ISO_data *p_iso);
void TransactionQueryReply(char *retStr, int retSize, ISO_data *p_iso);
int get_var(const char *var, const char *buf, int buf_len,	char *value, int value_len);
int decryptInfo(char *cvv, char *vdate, char *encrypt_info);
void addYear(char *dStr);
void ReturnPayMessage(char *resp_code, ISO_data *iso);
void SendPayResult(char *resp_code, ISO_data *iso);

void bin2str(char *to, const unsigned char *p, size_t len)
{
	const char	*hex = "0123456789abcdef";

	for (;len--; p++) {
		*to++ = hex[p[0] >> 4];
		*to++ = hex[p[0] & 0x0f];
	}
}

int str2bin(unsigned char *to , char *p)
{
	int flag, j;	
	char ch;
	for (flag = 0, j = 0; *p; p++)
	{
		if (*p >= 'a' && *p <= 'f')
			ch = *p - 'a' + 10 ;
		else if ( *p >= 'A' && *p <= 'F')
			ch = *p - 'A' + 10 ;
		else if ( *p >= '0' && *p <='9')
			ch = *p - '0' ;
		else
			ch = 0;
		if(flag)
		{
			to[j] <<= 4;
			to[j++] |= ch;
			flag = 0;
		}
		else
		{
			flag = 1;
			to[j] = ch;
		}
	}
    return j;
}

//消费申请
void ExpendApp(ISO_data *p_iso)
{
	char md5Str[2048];
	char postStr[2048],buff[1024];
	WriteThis pooh;
	int i,j,len;
	char md[33];
	trans_log_def *p_BcTransLog = &BcTransLog;
	memset(p_BcTransLog, 0, sizeof(BcTransLog));
    if ((len = getbit(p_iso, 2, buff)) > 0)
    {
        memcpy(p_BcTransLog->cardNumber, buff, MIN(len, 19));
    }
	if ((len = getbit(p_iso, 4, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderAmount, buff, MIN(len, 12));
	}
	if ((len = getbit(p_iso, 5, buff)) > 0)
	{
		memcpy(p_BcTransLog->phoneNumber, buff, MIN(len, 11));
	}
	if ((len = getbit(p_iso, 11, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderNumber, "000000", 6);
		memcpy(p_BcTransLog->orderNumber, buff, MIN(len, 6));
	}
	if ((len = getbit(p_iso, 12, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderTime + 8, buff, MIN(len, 6));
	}
	if ((len = getbit(p_iso, 13, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderTime + 4, buff, MIN(len, 4));
	}
	addYear(p_BcTransLog->orderTime);
	if ((len = getbit(p_iso, 42, buff)) > 0)
	{
		memcpy(p_BcTransLog->merId, buff, MIN(len, 25));
		rtrim(p_BcTransLog->merId);
	}
	if ((len = getbit(p_iso, 43, buff)) > 0)
	{
		memcpy(p_BcTransLog->merAbbr, buff, MIN(len, 100));
	}
	if ((len = getbit(p_iso, 45, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderNumber + 6, buff, MIN(len, 19));
	}
	if ((len = getbit(p_iso, 49, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderCurrency, buff, MIN(len, 3));
	}
	if ((len = getbit(p_iso, 60, buff)) > 0)
	{
		if(memcmp(buff, "00", 2) == 0 && len==174)
		{
			strcpy(p_BcTransLog->cardType, "00");
			memcpy(p_BcTransLog->cardPasswd, buff + 2, 174);
		}
		else if(memcmp(buff, "01", 2) == 0)
		{
			strcpy(p_BcTransLog->cardType, "01");
			if(decryptInfo(p_BcTransLog->cardCvn2,md, buff+2) < 0)
			{
				return ReturnPayMessage("96", p_iso);	
			}
			memcpy(p_BcTransLog->cardExpire, md+2, 2);
			memcpy(p_BcTransLog->cardExpire+2, md, 2);
		}
		else //返回报文格错误
		{
			dcs_log(0, 0, "Filed 60[%s] err!", buff);
			return ReturnPayMessage("96", p_iso);	
		}
		
	}
	if ((len = getbit(p_iso, 61, buff)) > 0)
	{
		memcpy(p_BcTransLog->credentialNumber, buff, MIN(len, 19));
	}
	if ((len = getbit(p_iso, 62, buff)) > 0)
	{
		memcpy(p_BcTransLog->credentialName, buff, MIN(len, 30));
	}
	if ((len = getbit(p_iso, 63, buff)) > 0)
	{
		memcpy(p_BcTransLog->credentialType, buff, MIN(len, 2));
	}
	memcpy(p_BcTransLog->transType, "01", 2);
	//*********************
	memcpy(p_BcTransLog->phoneNumber, "13888888888", 11);
	//*********************
	if(memcmp(p_BcTransLog->cardType, "01", 2) == 0) //信用卡
	{
		sprintf(md5Str, "acqCode=&backEndUrl=%s&charset=UTF-8&commodityDiscount=&commodityName=&commodityQuantity=&commodityUnitPrice=&commodityUrl=&customerIp=127.0.0.1&"
				"customerName=%s&defaultBankNumber=&defaultPayType=&frontEndUrl=&merAbbr=%s&merCode=&merId=%s&"
				"merReserved={cardCvn2=%s&cardExpire=%s&phoneNumber=%s&cardNumber=%s}"
				"&orderAmount=%s&orderCurrency=%s&orderNumber=%s&orderTime=%s&origQid=&transTimeout=%s&transType=%s&transferFee=&version=1.0.0&%s", 
			BackEndUrl, p_BcTransLog->credentialName, p_BcTransLog->merAbbr, p_BcTransLog->merId, 
			p_BcTransLog->cardCvn2, p_BcTransLog->cardExpire, p_BcTransLog->phoneNumber, p_BcTransLog->cardNumber, 
			p_BcTransLog->orderAmount, p_BcTransLog->orderCurrency, p_BcTransLog->orderNumber, p_BcTransLog->orderTime, TransTimeout, p_BcTransLog->transType, SignatureMD5);
	}
	else
	{
		sprintf(md5Str, "acqCode=&backEndUrl=%s&charset=UTF-8&commodityDiscount=&commodityName=&commodityQuantity=&commodityUnitPrice=&commodityUrl=&customerIp=127.0.0.1&"
						"customerName=%s&defaultBankNumber=&defaultPayType=&frontEndUrl=&merAbbr=%s&merCode=&merId=%s&"
						"merReserved={phoneNumber=%s&cardNumber=%s&cardPasswd=%s}"
						"&orderAmount=%s&orderCurrency=%s&orderNumber=%s&orderTime=%s&origQid=&transTimeout=%s&transType=%s&transferFee=&version=1.0.0&%s", 
				BackEndUrl, p_BcTransLog->credentialName, p_BcTransLog->merAbbr, p_BcTransLog->merId, 
				p_BcTransLog->phoneNumber, p_BcTransLog->cardNumber, p_BcTransLog->cardPasswd, 
				p_BcTransLog->orderAmount, p_BcTransLog->orderCurrency, p_BcTransLog->orderNumber, p_BcTransLog->orderTime, TransTimeout, p_BcTransLog->transType, SignatureMD5);
	}
	memset(md, 0, sizeof(md));
	md5(md5Str, md);
	if(memcmp(p_BcTransLog->cardType, "01", 2) == 0) //信用卡
	{
		sprintf(postStr, "acqCode=&backEndUrl=%s&charset=UTF-8&commodityDiscount=&commodityName=&commodityQuantity=&commodityUnitPrice=&commodityUrl=&customerIp=127.0.0.1&"
						"customerName=%s&defaultBankNumber=&defaultPayType=&frontEndUrl=&merAbbr=%s&merCode=&merId=%s&"
						"merReserved={cardCvn2=%s%%26cardExpire=%s%%26phoneNumber=%s%%26cardNumber=%s}"
						"&orderAmount=%s&orderCurrency=156&orderNumber=%s&orderTime=%s&origQid=&signature=%s&signMethod=md5&transTimeout=%s&transType=%s&transferFee=&version=1.0.0", 
				BackEndUrl, p_BcTransLog->credentialName, p_BcTransLog->merAbbr, p_BcTransLog->merId, 
				p_BcTransLog->cardCvn2, p_BcTransLog->cardExpire, p_BcTransLog->phoneNumber, p_BcTransLog->cardNumber, 
				p_BcTransLog->orderAmount, p_BcTransLog->orderNumber, p_BcTransLog->orderTime, md, TransTimeout, p_BcTransLog->transType);
	}
	else
	{
		memset(buff, 0, sizeof(buff));
		for(i=0, j=0; p_BcTransLog->cardPasswd[i]; i++, j++)
		{	
			if(p_BcTransLog->cardPasswd[i]=='/')
			{
				buff[j++] = '%';
				buff[j++] = '2';
				buff[j] = 'F';
			}
			else if(p_BcTransLog->cardPasswd[i]=='+')
			{
				buff[j++] = '%';
				buff[j++] = '2';
				buff[j] = 'B';
			}
			else if(p_BcTransLog->cardPasswd[i]=='=')
			{
				buff[j++] = '%';
				buff[j++] = '3';
				buff[j] = 'D';
			}
			else
				buff[j] = p_BcTransLog->cardPasswd[i];
		}
		buff[j] = 0;
		sprintf(postStr, "acqCode=&backEndUrl=%s&charset=UTF-8&commodityDiscount=&commodityName=&commodityQuantity=&commodityUnitPrice=&commodityUrl=&customerIp=127.0.0.1&"
						"customerName=%s&defaultBankNumber=&defaultPayType=&frontEndUrl=&merAbbr=%s&merCode=&merId=%s&"
						"merReserved={phoneNumber=%s%%26cardNumber=%s%%26cardPasswd=%s}"
						"&orderAmount=%s&orderCurrency=156&orderNumber=%s&orderTime=%s&origQid=&signature=%s&signMethod=md5&transTimeout=%s&transType=%s&transferFee=&version=1.0.0", 
				BackEndUrl, p_BcTransLog->credentialName, p_BcTransLog->merAbbr, p_BcTransLog->merId, 
				p_BcTransLog->phoneNumber, p_BcTransLog->cardNumber, buff, 
				p_BcTransLog->orderAmount, p_BcTransLog->orderNumber, p_BcTransLog->orderTime, md, TransTimeout, p_BcTransLog->transType);
	}
	memset(md5Str, 0, sizeof(md5Str));
	pooh.writePtr = md5Str;
	pooh.sizeLeft = sizeof(md5Str);
	pooh.writeSize = 0;	
	if(curl_post(postStr, ExpendUrl, &pooh)==0)
	{
		i = _shttpd_url_decode(md5Str, pooh.writeSize, postStr, sizeof(postStr));
		ExpendAppReply(postStr, i, 0, p_iso);
	}
	else
	{
		ReturnPayMessage("91", p_iso);
	}
}

//消费申请返回
//flag 0: 消费申请同步返回  1：消费申请异步返回

void ExpendAppReply(char *retStr, int retSize, int flag, ISO_data *p_iso)
{
	char charset[21];         //字符集
	char signMethod[11];      //签名方法
	char signature[33];       //签名信息
	char cupReserved[1025];   //系统保留域
	char version[9];          //版本号
	char md5Str[2048];
	char md[33];
	ISO_data iso;
	trans_log_def *p_BcTransLog = &BcTransLog;
	memset(charset       , 0, sizeof(charset       ));
	memset(cupReserved   , 0, sizeof(cupReserved   ));
	memset(signMethod    , 0, sizeof(signMethod    ));
	memset(signature     , 0, sizeof(signature     ));
	memset(version       , 0, sizeof(version       ));
	get_var("charset"       , retStr, retSize, charset       , sizeof(charset       ));
	get_var("cupReserved"   , retStr, retSize, cupReserved   , sizeof(cupReserved   ));
	get_var("exchangeDate"  , retStr, retSize, p_BcTransLog->exchangeDate  , sizeof(p_BcTransLog->exchangeDate  ));
	get_var("exchangeRate"  , retStr, retSize, p_BcTransLog->exchangeRate  , sizeof(p_BcTransLog->exchangeRate  ));
	get_var("merAbbr"       , retStr, retSize, p_BcTransLog->merAbbr       , sizeof(p_BcTransLog->merAbbr       ));
	get_var("merId"         , retStr, retSize, p_BcTransLog->merId         , sizeof(p_BcTransLog->merId         ));
	get_var("orderAmount"   , retStr, retSize, p_BcTransLog->orderAmount   , sizeof(p_BcTransLog->orderAmount   ));
	get_var("orderCurrency" , retStr, retSize, p_BcTransLog->orderCurrency , sizeof(p_BcTransLog->orderCurrency ));
	get_var("orderNumber"   , retStr, retSize, p_BcTransLog->orderNumber   , sizeof(p_BcTransLog->orderNumber   ));
	get_var("qid"           , retStr, retSize, p_BcTransLog->qid           , sizeof(p_BcTransLog->qid           ));
	get_var("respCode"      , retStr, retSize, p_BcTransLog->respCode      , sizeof(p_BcTransLog->respCode      ));
	get_var("respMsg"       , retStr, retSize, p_BcTransLog->respMsg       , sizeof(p_BcTransLog->respMsg       ));
	get_var("respTime"      , retStr, retSize, p_BcTransLog->respTime      , sizeof(p_BcTransLog->respTime      ));
	get_var("settleAmount"  , retStr, retSize, p_BcTransLog->settleAmount  , sizeof(p_BcTransLog->settleAmount  ));
	get_var("settleCurrency", retStr, retSize, p_BcTransLog->settleCurrency, sizeof(p_BcTransLog->settleCurrency));
	get_var("settleDate"    , retStr, retSize, p_BcTransLog->settleDate    , sizeof(p_BcTransLog->settleDate    ));
	get_var("signMethod"    , retStr, retSize, signMethod    , sizeof(signMethod    ));
	get_var("signature"     , retStr, retSize, signature     , sizeof(signature     ));
	get_var("traceNumber"   , retStr, retSize, p_BcTransLog->traceNumber   , sizeof(p_BcTransLog->traceNumber   ));
	get_var("traceTime"     , retStr, retSize, p_BcTransLog->traceTime     , sizeof(p_BcTransLog->traceTime     ));
	get_var("transType"     , retStr, retSize, p_BcTransLog->transType     , sizeof(p_BcTransLog->transType     ));
	get_var("version"       , retStr, retSize, version       , sizeof(version       ));
	sprintf(md5Str, "charset=%s&cupReserved=%s&exchangeDate=%s&exchangeRate=%s&merAbbr=%s&merId=%s&orderAmount=%s&orderCurrency=%s&orderNumber=%s&qid=%s&respCode=%s&respMsg=%s&respTime=%s&settleAmount=%s&settleCurrency=%s&settleDate=%s&traceNumber=%s&traceTime=%s&transType=%s&version=%s&%s", 
				charset, cupReserved, p_BcTransLog->exchangeDate, p_BcTransLog->exchangeRate, p_BcTransLog->merAbbr, p_BcTransLog->merId, p_BcTransLog->orderAmount, p_BcTransLog->orderCurrency, p_BcTransLog->orderNumber, p_BcTransLog->qid, p_BcTransLog->respCode, p_BcTransLog->respMsg, p_BcTransLog->respTime, p_BcTransLog->settleAmount, p_BcTransLog->settleCurrency, p_BcTransLog->settleDate, p_BcTransLog->traceNumber, p_BcTransLog->traceTime, p_BcTransLog->transType, version, SignatureMD5);
	memset(md, 0, sizeof(md));
	md5(md5Str, md);
	dcs_debug(0, 0, "<FILE:%s,LINE:%d>retStr=%s", __FILE__, __LINE__, retStr);
	if(memcmp(md, signature, 32)==0)
	{
		if(flag == 1)
		{
			p_iso = &iso;
			memset(p_iso, 0, sizeof(ISO_data));
			clearbit(p_iso);
			if(0 != BC_SetIsoMode(p_iso, "JAVA"))
			{
            	dcs_log(0,0,"<FILE:%s,LINE:%d>设置报文类型[JAVA]失败", __FILE__, __LINE__);
			}			
		}
		if(p_BcTransLog->qid[0] != 0)
		{
			setbit_t(p_iso, 45, p_BcTransLog->qid, 21);
		}
		else
		{
			setbit_t(p_iso, 45, p_BcTransLog->orderNumber + 6, 19);
		}
		if(flag == 1)
		{
			SendPayResult(p_BcTransLog->respCode, p_iso);
		} 
		else
		{
			ReturnPayMessage(p_BcTransLog->respCode, p_iso);		
		}
	}
	else
	{
		if(flag == 0)
		{
			ReturnPayMessage("A0", p_iso);
		}
		dcs_log(0, 0, "<FILE:%s,LINE:%d>sign error!\nmdStr=%s\nmd=%s, signMethod=%s\n", __FILE__, __LINE__, md5Str, md, signature);
	}
	return;
}

void TransactionQuery(ISO_data *p_iso)
{
	char md[33];
	char md5Str[1024],postStr[1024];
	int dst_len,len;
	char buff[2048];
	WriteThis pooh;
	trans_log_def *p_BcTransLog = &BcTransLog;
	memset(p_BcTransLog, 0, sizeof(BcTransLog));
	if ((len = getbit(p_iso, 11, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderNumber, "000000", 6);
		memcpy(p_BcTransLog->orderNumber, buff, MIN(len, 6));
	}
	if ((len = getbit(p_iso, 12, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderTime + 8, buff, MIN(len, 6));
	}
	if ((len = getbit(p_iso, 13, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderTime + 4, buff, MIN(len, 4));
	}
	addYear(p_BcTransLog->orderTime);
	if ((len = getbit(p_iso, 42, buff)) > 0)
	{
		memcpy(p_BcTransLog->merId, buff, MIN(len, 25));
		rtrim(p_BcTransLog->merId);
	}
	if ((len = getbit(p_iso, 45, buff)) > 0)
	{
		memcpy(p_BcTransLog->orderNumber + 6, buff, MIN(len, 19));
	}
	sprintf(md5Str, "charset=UTF-8&merId=%s&merReserved=&orderNumber=%s&orderTime=%s&transType=01&version=1.0.0&%s",
			p_BcTransLog->merId, p_BcTransLog->orderNumber, p_BcTransLog->orderTime, SignatureMD5);
	memset(md, 0, sizeof(md));	
	md5(md5Str, md);
	sprintf(postStr, "charset=UTF-8&merId=%s&merReserved=&orderNumber=%s&orderTime=%s&transType=01&version=1.0.0&signMethod=MD5&signature=%s",
			p_BcTransLog->merId, p_BcTransLog->orderNumber, p_BcTransLog->orderTime, md);
	memset(md5Str, 0, sizeof(md5Str));
	pooh.writePtr = md5Str;
	pooh.sizeLeft = sizeof(md5Str);
	pooh.writeSize = 0;	
	if(curl_post(postStr, QyeryUrl, &pooh)==0)
	{
		dst_len = _shttpd_url_decode(md5Str, pooh.writeSize, postStr, sizeof(postStr));
		TransactionQueryReply(postStr, dst_len, p_iso);
	}
}

void md5(char *md5Str, char *md)
{
 	char md_bin[17];
  	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, md5Str, strlen(md5Str));
	memset(md_bin, 0, sizeof(md_bin));
	MD5_Final(md_bin, &ctx);
	bin2str(md, md_bin, 16);
 	dcs_debug(0, 0, "<FILE:%s,LINE:%d>md5=%s\nmd5Str=%s", __FILE__, __LINE__, md, md5Str);
}

void TransactionQueryReply(char *retStr, int retSize, ISO_data *p_iso)
{
	char charset[21];         //字符集
	char cupReserved[1025];   //系统保留域
	char signMethod[11];      //签名方法
	char signature[33];       //签名信息
	char version[9];          //版本号
	char md5Str[2048];
	char queryResult[3];
	char md[33];
	trans_log_def *p_BcTransLog = &BcTransLog;
	memset(charset       , 0, sizeof(charset       ));  	
	memset(cupReserved   , 0, sizeof(cupReserved   ));
	memset(signMethod    , 0, sizeof(signMethod    ));  
	memset(signature     , 0, sizeof(signature     ));  
	memset(version       , 0, sizeof(version       ));  
	memset(queryResult   , 0, sizeof(queryResult   ));
	get_var("charset"       , retStr, retSize, charset       , sizeof(charset       ));    
	get_var("cupReserved"   , retStr, retSize, cupReserved   , sizeof(cupReserved   ));    
	get_var("exchangeDate"  , retStr, retSize, p_BcTransLog->exchangeDate  , sizeof(p_BcTransLog->exchangeDate  ));    
	get_var("exchangeRate"  , retStr, retSize, p_BcTransLog->exchangeRate  , sizeof(p_BcTransLog->exchangeRate  ));    
	get_var("qid"           , retStr, retSize, p_BcTransLog->qid           , sizeof(p_BcTransLog->qid           ));    
	get_var("queryResult"   , retStr, retSize, queryResult   , sizeof(queryResult   ));    
	get_var("respCode"      , retStr, retSize, p_BcTransLog->respCode      , sizeof(p_BcTransLog->respCode      ));    
	get_var("respTime"      , retStr, retSize, p_BcTransLog->respTime      , sizeof(p_BcTransLog->respTime      ));    
	get_var("settleAmount"  , retStr, retSize, p_BcTransLog->settleAmount  , sizeof(p_BcTransLog->settleAmount  ));    
	get_var("settleCurrency", retStr, retSize, p_BcTransLog->settleCurrency, sizeof(p_BcTransLog->settleCurrency));    
	get_var("settleDate"    , retStr, retSize, p_BcTransLog->settleDate    , sizeof(p_BcTransLog->settleDate    ));    
	get_var("signMethod"    , retStr, retSize, signMethod    , sizeof(signMethod    ));    
	get_var("signature"     , retStr, retSize, signature     , sizeof(signature     ));    
	get_var("traceNumber"   , retStr, retSize, p_BcTransLog->traceNumber   , sizeof(p_BcTransLog->traceNumber   ));    
	get_var("traceTime"     , retStr, retSize, p_BcTransLog->traceTime     , sizeof(p_BcTransLog->traceTime     ));    
	get_var("version"       , retStr, retSize, version       , sizeof(version       ));    
	sprintf(md5Str, "charset=%s&cupReserved=%s&exchangeDate=%s&exchangeRate=%s&qid=%s&queryResult=%s&respCode=%s&respTime=%s&settleAmount=%s&settleCurrency=%s&settleDate=%s&traceNumber=%s&traceTime=%s&version=%s&%s",
				charset, cupReserved, p_BcTransLog->exchangeDate, p_BcTransLog->exchangeRate, p_BcTransLog->qid, queryResult, p_BcTransLog->respCode, p_BcTransLog->respTime, p_BcTransLog->settleAmount, p_BcTransLog->settleCurrency, p_BcTransLog->settleDate, p_BcTransLog->traceNumber, p_BcTransLog->traceTime, &version, SignatureMD5);
	memset(md, 0, sizeof(md));
	md5(md5Str, md);
	dcs_debug(0, 0, "<FILE:%s,LINE:%d>retStr=%s", __FILE__, __LINE__, retStr);
	if(memcmp(md, signature, 32)==0)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>respCode=%s", __FILE__, __LINE__, p_BcTransLog->respCode);
		switch(queryResult[0])
		{
			case '0':
			case '1':
				SendPayResult(p_BcTransLog->respCode, p_iso);
				break;
			case '3':
				SendPayResult("84", p_iso);
				break;
			case '2':
				break;
			default:
				SendPayResult("96", p_iso);
				dcs_log(0, 0, "<FILE:%s,LINE:%d>queryResult=%s", __FILE__, __LINE__, queryResult);
				break;
		}
	}
	else
	{
		SendPayResult("A0", p_iso);
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>sign error!\nmdStr=%s\nmd=%s, signMethod=%s\n", __FILE__, __LINE__, md5Str, md, signature);
	}
	return;	
}


int get_var(const char *var, const char *buf, int buf_len,	char *value, int value_len)
{
	const char	*p, *e, *s;
	size_t		var_len;
	var_len = strlen(var);
	e = buf + buf_len;		/* End of QUERY_STRING buffer	*/

	/* buf is "var1=val1&var2=val2...". Find variable first */
	for (p = buf; p + var_len < e; p++)
		if ((p == buf || p[-1] == '&') &&
		    p[var_len] == '=' &&
		    !_shttpd_strncasecmp(var, p, var_len)) {

			/* Point 'p' to var value, 's' to the end of value */
			p += var_len + 1;	
			if( *p == '{' )
			{
				if ((s = memchr(p, '}', e - p)) == NULL)
					s = e;
				else
					s++;
			}
			else
			{
				if ((s = memchr(p, '&', e - p)) == NULL)
					s = e;
			}
			/* URL-decode value. Return result length */
			return (_shttpd_url_decode(p, s - p, value, value_len));
		}

	return (-1);
}

//解密信用卡信息
int decryptInfo(char *p_cvv, char *p_vdate, char *encrypt_info)
{
	ti_bc_ins_inf_def tiBcInsInf;	
	char e_vdate[17], e_cvv[17], key[17], cvv[17], vdate[17];
	char resp_code[10];
	int len;
	memset(e_vdate, 0, sizeof(e_vdate));
	memset(vdate, 0, sizeof(vdate));
	memset(key, 0, sizeof(key));
	memcpy(key, encrypt_info, 16);
	asc_to_bcd(e_vdate, encrypt_info+16, 16, 0);
	if(  1 != DecTrackPrg(resp_code, KeyIndex, key, 8, e_vdate,   &len, vdate))
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>解密vdate失败！resp_code[%s],KeyIndex[%s],key[%s]", __FILE__, __LINE__, resp_code, KeyIndex, key);
		return -1;
	}
	memset(e_cvv, 0, sizeof(e_cvv));
	memset(cvv, 0, sizeof(cvv));
	asc_to_bcd(e_cvv, encrypt_info+32, 16, 0);
	if(  1 != DecTrackPrg(resp_code, KeyIndex, key, 8, e_cvv,   &len, cvv))
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>解密cvv失败！resp_code[%s],KeyIndex[%s],key[%s]", __FILE__, __LINE__, resp_code, KeyIndex, key);
		return -1;
	}	
	if(strlen(cvv) !=3 )
	{
		dcs_log(&cvv, 8, "<FILE:%s,LINE:%d>解密cvv不正确！", __FILE__, __LINE__);
		return -1;
	}
	if(strlen(vdate) !=4 )
	{
		dcs_log(&vdate, 8, "<FILE:%s,LINE:%d>解密有效期不正确！", __FILE__, __LINE__);
		return -1;
	}
	strcpy(p_cvv, cvv);
	strcpy(p_vdate, vdate);
	return 1;
}


void addYear(char *dStr)
{
	char locDate[10], year[5];
	glb_GetLocalTimeString(locDate, 9, "%Y%m%d");
	if(memcmp(locDate+4, "01", 2)==0 && memcmp(dStr+4, "12", 2))
	{
		locDate[5] = 0;
		sprintf(year, "04d", atoi(locDate) - 1);
		memcpy(dStr, year, 4);
	}
	else if(memcmp(locDate+4, "12", 2)==0 && memcmp(dStr+4, "01", 2))
	{
		locDate[5] = 0;
		sprintf(year, "04d", atoi(locDate) + 1);
		memcpy(dStr, year, 4);
	}
	else
	{
		memcpy(dStr, locDate, 4); 
	}
}

void ReturnPayMessage(char *resp_code, ISO_data *iso)
{
	setbit_t(iso, 0, "0830", 4);
	setbit_t(iso, 33, InstIdCode, 8);
	setbit_t(iso, 39, resp_code, 2);
	setbit_t(iso, 64, "00000000", 8);
	setbitoff(iso, 5);
	setbitoff(iso, 48);
	setbitoff(iso, 60);
	setbitoff(iso, 61);
	setbitoff(iso, 62);
	setbitoff(iso, 63);   
	sendIso(iso);
}

void SendPayResult(char *resp_code, ISO_data *iso)
{
	trans_log_def *p_BcTransLog = &BcTransLog;
	setbit_t(iso, 0, "0840", 4);
	setbit_t(iso, 3, "810001", 6);
	setbit_t(iso, 4, p_BcTransLog->settleAmount, 12);
	setbit_t(iso, 11, p_BcTransLog->orderNumber, 6);
	setbit_t(iso, 12, p_BcTransLog->respTime + 8, 6); 
	setbit_t(iso, 13, p_BcTransLog->respTime + 4, 4);
	dcs_debug(0, 0, "<FILE:%s,LINE:%d>respCode=%s", __FILE__, __LINE__, resp_code);
	if(memcmp(resp_code, "00", 2) == 0)
	{
		setbit_t(iso, 15, p_BcTransLog->settleDate, 4);
		setbit_t(iso, 37, p_BcTransLog->traceNumber, 6);
	}	
	setbit_t(iso, 33, InstIdCode, 8);
	setbit_t(iso, 39, resp_code, 2);
	setbit_t(iso, 42, p_BcTransLog->merId, 15);	
	setbit_t(iso, 45, p_BcTransLog->qid, 21);	
	setbit_t(iso, 49, "156", 3);
	setbit_t(iso, 64, "00000000", 8);
	sendIso(iso);
}

int sendIso(ISO_data *iso)
{
	char isoStr[2048];
	int iRet;
    PrintISO( iso,"发送报文",0);  
    memcpy(isoStr, "JAVA", 4);
    iRet = isotostr(isoStr + 4, iso);
    if ( iRet < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>isotostr failure!(return:%d)",__FILE__,__LINE__, iRet);
        BC_TRACE_END( -1 );
    }
    iRet = fold_write(gs_appSvrFid, gs_myFid, isoStr, iRet + 4);
    if ( iRet < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d>Failure to write msg to folder[%d]!",__FILE__,__LINE__, gs_appSvrFid);
        return -1;
    } else
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d>Success to write msg to folder[%d]",__FILE__,__LINE__, gs_appSvrFid);
         return 0;
    }
}
