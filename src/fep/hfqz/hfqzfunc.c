#include <stdio.h>
#include <string.h> 
#include <curl/curl.h>
#include <openssl/md5.h>

#include "iso8583.h"
#include "bc.h"
#define MAX(a,b) a>b?a:b
void ReturnQueryMessage(char *resp_code, ISO_data *iso);

typedef struct
{
	char merId[6 + 1];					//商户ID  汇付 6位
	char MerDate[8 + 1];				//交易日期 YYYYMMDD
	char SysDate[8 + 1];				//清算日期 YYYYMMDD
	char OrdId[16 + 1];					//订单号  汇付6-16位
	char SysSeqId[6 + 1];				//清算参考号	
	char TransAmt[13 + 1];				//交易金额 汇付以元为单位，必带两位小数
	char respCode[2 + 1];         		//返回代码
	char GateId[2 + 1];					//网关号
	char TransStat[1 + 1];				//异步返回交易状态
	char MerPriv[120 + 1];				//私有域
	char OrgOrdId[19 + 1];				//原订单号 汇付6-16位
	char OrgMerDate[8 + 1];				//交易日期 YYYYMMDD
}trans_log_def;

typedef struct
{
	char key_1[32+1];
	char key_2[32+1];
	char key_3[32+1];
	time_t t;
	char chk[4];
}key_def;
 
extern trans_log_def BcTransLog;
extern char BackEndUrl[100 + 1];  			//回调IP地址
extern char ExpendUrl[100 + 1];				//交易地址
extern char KeyIndex[5 + 1];				//密钥索引
extern char InstIdCode[8 + 1];				//机构代码
extern int gs_myFid, gs_appSvrFid;
extern key_def KeyData;					//签到数据

typedef struct {
  char *writePtr;
  int sizeLeft;
  int writeSize;
}WriteThis;

char *getXMLVal(char *buff, char *xmlStr, char *val, int valSize, int *ret);
void ExpendAppReply(char *retStr, int retSize, ISO_data *p_iso);
void md5(char *md5Str, char *md);
void ExpendApp(ISO_data *p_iso);
int curl_post(char *postStr, char *urlStr, WriteThis *pooh);
void TransactionQuery(ISO_data *p_iso);
void TransactionQueryReply(char *retStr, int retSize, ISO_data *p_iso);
int get_var(const char *var, const char *buf, int buf_len,	char *value, int value_len);
int ChangeInfo(char *encrypt_info, int encryptInfoLen, char *encMsg, char *merId);
void addYear(char *dStr);
void ReturnPayMessage(char *resp_code, ISO_data *iso);
void SendPayResult(char *resp_code);
void ChangeAmt(char *amt, int amtLen, char *inamt);


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
	char signStr[2048];
	char postStr[2048],buff[1024];
	WriteThis pooh;
	int i, j, len,ret;
	char md[33];
	char resp_code[7];
	char cardInfo[512];
	char keyIndex[3 + 1];
	trans_log_def *p_BcTransLog = &BcTransLog;
	memset(p_BcTransLog, 0, sizeof(BcTransLog));
	//check field
	if ( 0 != BC_CheckIsoDataField(p_iso, "4,11,12,13,42,48,63") )
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>校验接收报文0820失败！!", __FILE__, __LINE__); 
		ReturnPayMessage("96", p_iso);				
		return;
	}
	//校验接收机构密钥
	if( 0 > ReadKeyData())
	{
		ReturnPayMessage("96", p_iso);				
		return;
	}
	memset(keyIndex, 0, sizeof(keyIndex));
	memcpy(keyIndex, KeyIndex+2, 3);
	if ( 0 != BC_TermCheckMac(keyIndex, KeyData.key_1, g_Rcv_Buffer, g_RcvBufferSize))
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>接收机构MAC校验未通过", __FILE__, __LINE__); 
		ReturnPayMessage("SE", p_iso);				
		return;
	}
	if ((len = getbit(p_iso, 4, buff)) > 0)
	{
		buff[12]=0;
		sprintf(p_BcTransLog->TransAmt, "%.2f", 0.01 * atoi(buff));
	}
	memcpy(p_BcTransLog->OrdId, "000000", 6);
	if ((len = getbit(p_iso, 11, buff)) > 0)
	{
		memcpy(p_BcTransLog->OrdId, buff, MIN(len, 6));
	}
	if ((len = getbit(p_iso, 12, buff)) > 0)
	{
		memcpy(p_BcTransLog->OrdId + 6, buff, MIN(len, 6));
	}
	if ((len = getbit(p_iso, 13, buff)) > 0)
	{
		memcpy(p_BcTransLog->MerDate + 4, buff, MIN(len, 4));
	}
	addYear(p_BcTransLog->MerDate);
	if ((len = getbit(p_iso, 42, buff)) > 0)
	{
		memcpy(p_BcTransLog->merId, buff, MIN(len, 6));
		rtrim(p_BcTransLog->merId);
	}
	if ((len = getbit(p_iso, 48, buff)) > 0)
	{
		memset(cardInfo, 0, sizeof(cardInfo));
		if(0>ChangeInfo(buff, len, cardInfo,  p_BcTransLog->merId))
			return ReturnPayMessage("96", p_iso);				
	}
	if ((len = getbit(p_iso, 63, buff)) > 0)
	{
		memcpy(p_BcTransLog->GateId, buff, MIN(len, 2));
	}
	sprintf(signStr, "20%s%s%s%s%s%s%s%s" ,p_BcTransLog->merId, p_BcTransLog->OrdId,  p_BcTransLog->TransAmt,  p_BcTransLog->MerDate,  "P",  p_BcTransLog->GateId , "ChinaInPay" , BackEndUrl) ;
	sprintf(buff, "key/MerPrK%s.key", p_BcTransLog->merId);
	if(0 > u_fabricatefile(buff, postStr, sizeof(postStr)))
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, buff);
		return ReturnPayMessage("96", p_iso);				
	}
	ret = SignMsg(p_BcTransLog->merId, postStr, signStr, strlen(signStr), buff);
	if(ret !=0 )
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>SignMsg error!ret = [%d]", __FILE__, __LINE__, ret);
		ReturnPayMessage("96", p_iso);				
		return;
	}
	sprintf(postStr, "Version=20&MerId=%s&OrdId=%s&TransAmt=%s&MerDate=%s&TransType=P&GateId=%s&MerPriv=%s&RetUrl=%s&CardInfo=%s&ChkValue=%s",
		p_BcTransLog->merId, p_BcTransLog->OrdId,  p_BcTransLog->TransAmt,  p_BcTransLog->MerDate, p_BcTransLog->GateId, "ChinaInPay" , BackEndUrl, cardInfo, buff) ;
	memset(signStr, 0, sizeof(signStr));
	pooh.writePtr = signStr;
	pooh.sizeLeft = sizeof(signStr);
	pooh.writeSize = 0;	
	if(curl_post(postStr, ExpendUrl, &pooh)==0)
	{
		i = _shttpd_url_decode(signStr, pooh.writeSize, postStr, sizeof(postStr));
		ExpendAppReply(postStr, i, p_iso);
	}
	else
	{
		ReturnPayMessage("91", p_iso);
	}
}

//消费申请异步返回
void PayResultReply(char *retStr, int retSize)
{
	char resp_code[2 + 1];
	char transType[1 + 1];
	char transStat[1 + 1];
	char chkValue[256 +1];
	char signStr[2048],buff[2048];
	char keyFile[255];
	int ret;
	char *p;
	trans_log_def *p_BcTransLog = &BcTransLog;
	dcs_debug(0,0, "<FILE:%s,LINE:%d>retStr[%d]=%s!", __FILE__,__LINE__, retSize, retStr);		
	ret = get_var("MerId", retStr, retSize, p_BcTransLog->merId, sizeof(p_BcTransLog->merId));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取merId失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("OrdId", retStr, retSize, p_BcTransLog->OrdId, sizeof(p_BcTransLog->OrdId));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取ordId失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("TransAmt", retStr, retSize, p_BcTransLog->TransAmt, sizeof(p_BcTransLog->TransAmt));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取transAmt失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("TransType", retStr, retSize, transType, sizeof(transType));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取transType失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("TransStat", retStr, retSize, transStat, sizeof(transStat));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取transStat失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("MerPriv", retStr, retSize, p_BcTransLog->MerPriv, sizeof(p_BcTransLog->MerPriv));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取merPriv失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("GateId", retStr, retSize, p_BcTransLog->GateId, sizeof(p_BcTransLog->GateId));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取gateId失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("SysDate", retStr, retSize, p_BcTransLog->SysDate, sizeof(p_BcTransLog->SysDate));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取SysDate失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("SysSeqId", retStr, retSize, p_BcTransLog->SysSeqId, sizeof(p_BcTransLog->SysSeqId));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取SysSeqId失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("MerDate", retStr, retSize, p_BcTransLog->MerDate, sizeof(p_BcTransLog->MerDate));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取merDate失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	ret = get_var("ChkValue", retStr, retSize, chkValue, sizeof(chkValue));
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取chkValue失败\nretStr=%s", __FILE__, __LINE__, retStr);
		return;
	}
	sprintf(signStr, "%s%s%s%s%s%s%s%s%s%s", 
				p_BcTransLog->merId, p_BcTransLog->OrdId, p_BcTransLog->TransAmt, p_BcTransLog->MerDate, 
				transType, transStat, p_BcTransLog->GateId, p_BcTransLog->MerPriv, p_BcTransLog->SysDate, p_BcTransLog->SysSeqId);
	sprintf(buff, "key/PgPuK%s.key", p_BcTransLog->merId);
	if(0 > u_fabricatefile(buff, keyFile, sizeof(keyFile)))
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, buff);
    	SendPayResult("96");
	}
	ret = VeriSignMsg(keyFile, signStr, strlen(signStr), chkValue);
	if(ret != 0)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>VeriSignMsg Fail!ret = %d,keyFile=%s\nsignStr=%s", __FILE__, __LINE__, ret, keyFile, signStr);
		return SendPayResult("96");
	}
	else
	{
		switch(transStat[0])
		{
			case 'S':
				SendPayResult("00");
				break;
			case 'I':
				SendPayResult("98");
				break;
			case 'F':
				SendPayResult("F6");
				break;
			default:
				dcs_log(0, 0,"<FILE:%s,LINE:%d>transStat[%s]", __FILE__, __LINE__, transStat);
				SendPayResult("96");
				break;
		}
	}
}

//消费申请同步返回
void ExpendAppReply(char *retStr, int retSize, ISO_data *p_iso)
{
	char resp_code[2 + 1];
	char transType[1 + 1];
	char chkValue[256 +1];
	char signStr[2048],buff[2048];
	char keyFile[255];
	int ret;
	char *p;
	trans_log_def *p_BcTransLog = &BcTransLog;
	dcs_debug(0, 0, "retStr[%d]=%s", retSize, retStr);
	p = getXMLVal(retStr, "respCode", resp_code, sizeof(resp_code), &ret);
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取返回码失败\nretStr=%s", __FILE__, __LINE__, retStr);
		ReturnPayMessage("96", p_iso);
		return;
	}
	p = getXMLVal(retStr, "transType", transType, sizeof(transType), &ret);
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取transType失败\nretStr=%s", __FILE__, __LINE__, retStr);
		ReturnPayMessage("96", p_iso);
		return;
	}
	p = getXMLVal(retStr, "merId", p_BcTransLog->merId, sizeof(p_BcTransLog->merId), &ret);
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取merId失败\nretStr=%s", __FILE__, __LINE__, retStr);
		ReturnPayMessage("96", p_iso);
		return;
	}
	p = getXMLVal(retStr, "ordId", p_BcTransLog->OrdId, sizeof(p_BcTransLog->OrdId), &ret);
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取ordId失败\nretStr=%s", __FILE__, __LINE__, retStr);
		ReturnPayMessage("96", p_iso);
		return;
	}
	p = getXMLVal(retStr, "merDate", p_BcTransLog->MerDate, sizeof(p_BcTransLog->MerDate), &ret);
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取merDate失败\nretStr=%s", __FILE__, __LINE__, retStr);
		ReturnPayMessage("96", p_iso);
		return;
	}
	p = getXMLVal(retStr, "transAmt", p_BcTransLog->TransAmt, sizeof(p_BcTransLog->TransAmt), &ret);
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取transAmt失败\nretStr=%s", __FILE__, __LINE__, retStr);
		ReturnPayMessage("96", p_iso);
		return;
	}
	p = getXMLVal(retStr, "gateId", p_BcTransLog->GateId, sizeof(p_BcTransLog->GateId), &ret);
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取gateId失败\nretStr=%s", __FILE__, __LINE__, retStr);
		ReturnPayMessage("96", p_iso);
		return;
	}
	p = getXMLVal(retStr, "merPriv", p_BcTransLog->MerPriv, sizeof(p_BcTransLog->MerPriv), &ret);
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取merPriv失败\nretStr=%s", __FILE__, __LINE__, retStr);
		ReturnPayMessage("96", p_iso);
		return;
	}
	p = getXMLVal(retStr, "chkValue", chkValue, sizeof(chkValue), &ret);
	if(0 > ret)
	{
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>取chkValue失败\nretStr=%s", __FILE__, __LINE__, retStr);
		ReturnPayMessage("96", p_iso);
		return;
	}
	sprintf(signStr, "%s%s%s%s%s%s%s%s", 
				resp_code, transType, p_BcTransLog->merId, p_BcTransLog->OrdId, p_BcTransLog->MerDate, p_BcTransLog->TransAmt, p_BcTransLog->GateId, p_BcTransLog->MerPriv);
	sprintf(buff, "key/PgPuK%s.key", p_BcTransLog->merId);
	if(0 > u_fabricatefile(buff, keyFile, sizeof(keyFile)))
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, buff);
		return ReturnPayMessage("96", p_iso);				
	}
	memset(buff, 0, sizeof(buff));
	ret = VeriSignMsg(keyFile, signStr, strlen(signStr), chkValue);
	if(ret != 0)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>VeriSignMsg Fail!ret = %d,keyFile=%s\nsignStr=%s", __FILE__, __LINE__ , ret, keyFile, signStr);
		ReturnPayMessage("96", p_iso);
	}
	else
	{
		ReturnPayMessage(resp_code, p_iso);
	}
	return;
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

//转换信用卡信息
int ChangeInfo(char *encrypt_info, int encryptInfoLen, char *encMsg, char *merId)
{
	ti_bc_ins_inf_def tiBcInsInf;	
	char resp_code[10],info[1024];
	char keyFile[256], buff[256];
	int len, ret;
	memset(info, 0, sizeof(info));
	if(0 > ReadKeyData())
	{
		return -1;
	}
	if(  1 != DecTrackPrg(resp_code, KeyIndex, KeyData.key_2, encryptInfoLen, encrypt_info, &len, info))
	{
		return -1;
	}
	sprintf(buff, "key/PgPuK%s.key", merId);
	if(u_fabricatefile(buff, keyFile, sizeof(keyFile)) < 0)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, buff);
		return -1;
	}
	len = strlen(info);
	ret = EncMsg(keyFile, info, len, encMsg);
	if(ret != 0)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>EncMsg error!ret = [%d]\ninfo[%d][%s]\nkeyFile=%s", __FILE__, __LINE__, ret, len, info, keyFile);
		return -1;
	}
	return 1;
}

void addYear(char *dStr)
{
	char locDate[10], year[5];
	glb_GetLocalTimeString(locDate, 9, "%Y%m%d");
	if(memcmp(locDate+4, "01", 2)==0 && memcmp(dStr+4, "12", 2) == 0)
	{
		locDate[5] = 0;
		sprintf(year, "%04d", atoi(locDate) - 1);
		memcpy(dStr, year, 4);
	}
	else if(memcmp(locDate+4, "12", 2)==0 && memcmp(dStr+4, "01", 2) == 0)
	{
		locDate[5] = 0;
		sprintf(year, "%04d", atoi(locDate) + 1);
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
	setbitoff(iso, 8);
	setbitoff(iso, 48);
	setbitoff(iso, 63);   
	sendIso(iso, 1);
}

void SendPayResult(char *resp_code)
{
	ISO_data iso; 
	ISO_data *p_iso=&iso; 
	char buff[255];
	int i;
	trans_log_def *p_BcTransLog = &BcTransLog;
	memset(p_iso, 0, sizeof(ISO_data));
	if ( 0 != BC_SetIsoMode(p_iso, "JAVA"))
	{
        dcs_log(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>BC_SetIsoMode failure",__FILE__,__LINE__);
        return;
	}	
	setbit_t(p_iso, 0, "0870", 4); 
	setbit_t(p_iso, 3, "810001", 6);
	ChangeAmt(buff, 12, p_BcTransLog->TransAmt);
	setbit_t(p_iso, 4 , buff, 12);
	setbit_t(p_iso, 11, p_BcTransLog->OrdId, 6);
	setbit_t(p_iso, 12, p_BcTransLog->OrdId + 6, 6);
	setbit_t(p_iso, 13, p_BcTransLog->MerDate + 4, 4);
	setbit_t(p_iso, 15, p_BcTransLog->SysDate + 4, 4);
	setbit_t(p_iso, 33, InstIdCode, 8);
	setbit_t(p_iso, 37, p_BcTransLog->SysSeqId, 12);
	setbit_t(p_iso, 39, resp_code, 2);
	setbit_t(p_iso, 42, p_BcTransLog->merId, 6);
	setbit_t(p_iso, 64, "00000000", 8);
	sendIso(p_iso, 1);
}

int sendIso(ISO_data *iso, int flag)
{
	char isoStr[2048];
	int iRet;
	char keyIndex[3 + 1];
	char mac[8 + 1];
    PrintISO( iso,"发送报文",0);  
    memcpy(isoStr, "JAVA", 4);
    iRet = isotostr(isoStr + 4, iso);
    if ( iRet < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>isotostr failure!(return:%d)",__FILE__,__LINE__, iRet);
        return -1;
    }
    if(flag)
    {
	    if(0 > ReadKeyData())
	    {
	    	return -1;
	    }
	    memset(keyIndex, 0, sizeof(keyIndex));
	    memcpy(keyIndex, KeyIndex + 2, 3);
	    if(0 > BC_GenMac(keyIndex, KeyData.key_1, isoStr + 4, iRet-8, mac))
	    {
        	dcs_log(0,0,"<FILE:%s,LINE:%d>BC_GenMac failure!",__FILE__,__LINE__);
	    	return -1;
	    }
	    memcpy(isoStr + 4 + iRet - 8, mac, 8);
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

int ReadKeyData()
{
	char cfgfile[256], caBuffer[1024];
	FILE *fd;
	char *p, chk[4];
	int i;
	sprintf(caBuffer, "key/HF%s.key", InstIdCode);
	if(u_fabricatefile(caBuffer, cfgfile, sizeof(cfgfile)) < 0)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, caBuffer);
		return -1;
	}
	fd = fopen(cfgfile, "r");	
	if(fd == NULL)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, cfgfile);
    	return -1;
	}    
	if(fread(&KeyData, sizeof(KeyData), 1, fd)!=1)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Read file[%s] error!", __FILE__, __LINE__, cfgfile);
		fclose(fd);
    	return -1;
	}
    fclose(fd);
	memset(chk, 0, sizeof(chk));
	for(i=0, p = (char *)&KeyData; i< sizeof(KeyData) -4; i++, p++)
	{
		chk[i % 4] ^= *p;
	}
	if(memcmp(chk, KeyData.chk, 4))
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Check file[%s] error!", __FILE__, __LINE__, cfgfile);
    	return -1;
	}
	return 1;
}

int WriteKeyData()
{
	char cfgfile[256], caBuffer[1024];
	FILE *fd;
	unsigned char *p, chk[4];
	int i;
	sprintf(caBuffer, "key/HF%s.key", InstIdCode);
	if(u_fabricatefile(caBuffer, cfgfile, sizeof(cfgfile)) < 0)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, caBuffer);
		return -1;
	}
	fd = fopen(cfgfile, "w");	
	if(fd == NULL)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Open file[%s] error!", __FILE__, __LINE__, cfgfile);
    	return -1;
	}  
	time(&KeyData.t);  
	memset(chk, 0, sizeof(chk));
	for(i=0, p = (unsigned char *)&KeyData; i< sizeof(KeyData) -4; i++, p++)
	{
		chk[i % 4] ^= *p;
	}
	memcpy(KeyData.chk, chk, 4);
	if(fwrite(&KeyData, sizeof(KeyData), 1, fd)!=1)
	{
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>Write file[%s] error!", __FILE__, __LINE__, cfgfile);
    	fclose(fd);
    	return -1;
	}
	fclose(fd);
	return 1;
}

void ReturnSignMessage(char *resp_code, ISO_data *iso)
{
	char buff[32 + 1];
	clearbit(iso);
	setbit_t(iso, 0, "0530", 4);
	setbit_t(iso, 3, "510000", 6);
	setbit_t(iso, 33, InstIdCode, 8);
	setbit_t(iso, 39, resp_code, 2);
	if(0 == memcmp(resp_code, "00", 2))
	{
		memcpy(buff, KeyData.key_1, 16);
		memcpy(buff + 16, KeyData.key_2, 16);
		setbit_t(iso, 48, buff, 32);
	}
	sendIso(iso, 0);
}


int Sign(ISO_data *p_iso)
{
	char keyIndex[3 +1];
	char resp_cd[6 + 1];
	memset(keyIndex, 0, sizeof(keyIndex));
	memcpy(keyIndex, KeyIndex+2, 3);
    DESGETKY(resp_cd, KeyData.key_1, keyIndex);
    if ( 0 != memcmp( resp_cd, "00", 2) )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>DESGETKY failure.",__FILE__,__LINE__);
        ReturnSignMessage("96", p_iso);
        return -1;
    }
    DESGETKY(resp_cd, KeyData.key_2, keyIndex);
    if ( 0 != memcmp( resp_cd, "00", 2) )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>DESGETKY failure.",__FILE__,__LINE__);
        ReturnSignMessage("96", p_iso);
        return -1;
    }
    DESGETKY(resp_cd, KeyData.key_3, keyIndex);
    if ( 0 != memcmp( resp_cd, "00", 2) )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>DESGETKY failure.",__FILE__,__LINE__);
        ReturnSignMessage("96", p_iso);
        return -1;
    }
    if(0> WriteKeyData())
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>DESGETKY failure.",__FILE__,__LINE__);
        ReturnSignMessage("96", p_iso);
        return -1;
    }
    ReturnSignMessage("00", p_iso);
}

char *getXMLVal(char *buff, char *xmlStr, char *val, int valSize, int *ret)
{
	int i,j;
	char *p, *e, *ps;
	int xml_len;
	int flag;
	xml_len = strlen(xmlStr);
	e = buff + strlen(buff);
	for(p = buff, flag = 0; p + xml_len + 2 < e; p++)
	{
		if(*p == '<' && p[xml_len + 1] == '>')
		{
			if(memcmp(p+1, xmlStr, xml_len) == 0)
			{
				if(flag == 0)
				{
					flag = 1;
					p += xml_len + 1;
					ps = p + 1;
				}
				else if(flag == 2)
				{
					*ret = 1;
					return p;
				}
				else
				{
					*ret = -2;
					return NULL;
				}
			}
		}
		else if(*p == '<' && p[xml_len + 1] == '/' && p[xml_len + 2] == '>')
		{
			if(flag == 0)
			{
				memset(val, 0, valSize);
				flag = 2;
			}else if(flag == 2)
			{
				*ret = 1;
				return p;
			}
			else
			{
				*ret = -4;
				return NULL;
			}
		}
		else if(*p == '<' && p[1] == '/' && p[xml_len + 2] == '>')
		{
			if(memcmp(p+2, xmlStr, xml_len) == 0)
			{
				if(flag == 1)
				{
					memset(val, 0, valSize);
					memcpy(val, ps, MIN(p-ps, valSize-1));
					flag = 2;
				}
				else
				{
					*ret = -3;
					return NULL;
				}
			}
		}
	}
	if(flag == 2)
	{
		*ret = 0;
		return buff;
	}
	*ret = -1;
	return NULL;
}

void ChangeAmt(char *amt, int amtLen, char *inamt)
{
	int i, j, k;
	memset(amt, '0', amtLen);
	amt[amtLen] = 0;
    i = strlen(inamt);
    memcpy(amt + amtLen - 2, inamt + i -2, 2);
    j = MAX(0, amtLen + 1 - i);
    k = MAX(i - amtLen - 1, 0);
    memcpy(amt + j, inamt + k , MIN(amtLen - 2, i - 3));  
}
