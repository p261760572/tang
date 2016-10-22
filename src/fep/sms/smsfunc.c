#include<stdio.h>
#include<string.h>
#include<time.h>
#include <curl/curl.h>
#include <openssl/md5.h>

#include "iso8583.h"
#include "bc.h"
#include "msg_send.h"
#include "msg_rcv.h"

typedef struct
{
  char *writePtr;
  int sizeLeft;
  int writeSize;
}WriteThis;

typedef struct
{
	char key_1[32+1];
	char key_2[32+1];
	char key_3[32+1];
	time_t t;
	char chk[4];
}key_def;

char SendMsgUrl[256 + 1];
char SrcSysUser[50 + 1];
char SrcSysPwd[50 + 1];
char SrcSysPwdMd5[32 + 1];
char KeyIndex[5 + 1];
char InstIdCode[8 + 1];
char Ext[10 + 1];
key_def KeyData;					//签到数据

int gs_myFid = -1, gs_appSvrFid = -1;


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

void bin2str(char *to, const unsigned char *p, size_t len)
{
	const char	*hex = "0123456789abcdef";

	for (;len--; p++) {
		*to++ = hex[p[0] >> 4];
		*to++ = hex[p[0] & 0x0f];
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

int curl_post(char *postStr, char *urlStr, WriteThis *pooh)
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, urlStr);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, pooh);		
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postStr);
		res = curl_easy_perform(curl);
		dcs_debug(0, 0, "<FILE:%s,LINE:%d>PostStr = %s",  __FILE__, __LINE__, postStr);
		if(res != 0)
		{
     		dcs_log(0,0,"<FILE:%s,LINE:%d> url_easy_perform return %d\n", __FILE__, __LINE__, res);
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return res;
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

void addYear(char *dStr)
{
	char locDate[10], year[5];
	glb_GetLocalTimeString(locDate, 9, "%Y%m%d");
	if(memcmp(locDate+4, "01", 2) == 0 && memcmp(dStr+4, "12", 2) == 0)
	{
		locDate[5] = 0;
		sprintf(year, "%04d", atoi(locDate) - 1);
		memcpy(dStr, year, 4);
	}
	else if(memcmp(locDate+4, "12", 2) == 0 && memcmp(dStr+4, "01", 2) == 0)
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

int returnSendMsgMesssage(char *resp_code, ISO_data *p_iso)
{
	setbit_t(p_iso, 0, "0730", 4);
	setbit_t(p_iso, 28, resp_code, 6);	
	sendIso(p_iso, 1);
	return 1;
}


int readMsg(char *svcBuff)
{
	char buff[2048];
	char msgBuff[1024];
	char msgUT8[1024], msgStr[1024];
	int ret, mRet, uL;
	char *p, *p1;
	msg_rcv_def 	msgRcvInf;
	p = getXMLVal(svcBuff, "MOs", buff, sizeof(buff), &ret);
	if(0 > ret)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Get MOs Fail!\nSDKSvcCont = %s\n", __FILE__, __LINE__, svcBuff);
		return -1;
	}
	p = buff;
	while(1)
	{
		p = getXMLVal(p, "MO", msgBuff, sizeof(msgBuff), &mRet);
		if(0 > mRet)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get MO Fail!\nMOs = %s\n", __FILE__, __LINE__, p);
			return -1;
		}
		memset(&msgRcvInf, 0, sizeof(msg_rcv_def));
		p1 = getXMLVal(msgBuff, "MDN", msgRcvInf.phone, sizeof(msgRcvInf.phone), &ret);
		if(0 > ret)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get MDN Fail!\nmsgBuff = %s\n", __FILE__, __LINE__, msgBuff);
			return -1;
		}
		p1 = getXMLVal(msgBuff, "SendTime", msgRcvInf.send_time, sizeof(msgRcvInf.send_time), &ret);
		if(0 > ret)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get SendTime Fail!\nmsgBuff = %s\n", __FILE__, __LINE__, msgBuff);
			return -1;
		}
		p1 = getXMLVal(msgBuff, "SendContent", msgRcvInf.msg_txt, sizeof(msgRcvInf.msg_txt), &ret);
		if(0 > ret)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get SendContent Fail!\nmsgBuff = %s\n", __FILE__, __LINE__, msgBuff);
			return -1;
		}
		p1 = getXMLVal(msgBuff, "Ext", msgRcvInf.ext, sizeof(msgRcvInf.ext), &ret);
		if(0 > ret)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get Ext Fail!ret = %d\nmsgBuff = %s\n", __FILE__, __LINE__, ret, msgBuff);
			return -1;
		}
		memset(msgStr, 0, sizeof(msgStr));
		memset(msgUT8, 0, sizeof(msgUT8));
		ret = _shttpd_url_decode(msgRcvInf.msg_txt, strlen(msgRcvInf.msg_txt), msgStr, sizeof(msgStr));
		uL = DecodeBase64(msgStr, msgUT8);
		ret = ToGBK(msgUT8, uL, msgStr, sizeof(msgStr));
		if(0 >= ret)
		{
			dcs_log(msgUT8, uL, "<FILE:%s,LINE:%d>GBK转码失败,ret=%d", __FILE__, __LINE__, ret);
		}
		else
		{
			dcs_debug(0, 0, "<FILE:%s,LINE:%d>收到短消息:[%s]", __FILE__, __LINE__, msgStr);
		}
		MsgRcvOpr(GLB_DB_INSERT1, &msgRcvInf, &ret);
		if(ret != 0)
		{
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>插入接收数据库失败！sqlcode=%d, ", __FILE__, __LINE__, ret); 
			return 1;
		}
		if(mRet == 0) return 0;
	}
}


int readMsgStat(char *svcBuff)
{
	char buff[2048];
	char report[1024];
	char ext[10 + 1];
	msg_send_def msgSendInf;	
	int ret, rRet;
	char *p, *p1;
	p = getXMLVal(svcBuff, "Reports", buff, sizeof(buff), &ret);
	if(0 > ret)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Get Reports Fail!\nSDKSvcCont = %s\n", __FILE__, __LINE__, svcBuff);
		return -1;
	}
	p = buff;
	while(1)
	{
		p = getXMLVal(p, "Report", report, sizeof(report), &rRet);
		if(0 > rRet)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get Report Fail!\nReports = %s\n", __FILE__, __LINE__, p);
			return -1;
		}
		memset(&msgSendInf, 0, sizeof(msg_send_def));
		p1 = getXMLVal(report, "MsgID", msgSendInf.msg_id, sizeof(msgSendInf.msg_id), &ret);
		if(0 > ret)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get MsgID Fail!\nReport = %s\n", __FILE__, __LINE__, report);
			return -1;
		}
		p1 = getXMLVal(report, "MDN", msgSendInf.phone, sizeof(msgSendInf.phone), &ret);
		if(0 > ret)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get MDN Fail!\nReport = %s\n", __FILE__, __LINE__, report);
			return -1;
		}
		p1 = getXMLVal(report, "ReportStatus", msgSendInf.send_flag, sizeof(msgSendInf.send_flag), &ret);
		if(0 > ret)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get ReportStatus Fail!\nReport = %s\n", __FILE__, __LINE__, report);
			return -1;
		}
		p1 = getXMLVal(report, "ReceiveDate", msgSendInf.rcv_time, sizeof(msgSendInf.rcv_time), &ret);
		if(0 > ret)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get ReceiveDate Fail!\nReport = %s\n", __FILE__, __LINE__, report);
			return -1;
		}
		p1 = getXMLVal(report, "Ext", ext, sizeof(ext), &ret);
		if(0 > ret)
		{
			dcs_log(0, 0, "<FILE:%s,LINE:%d>Get Ext Fail!\nReport = %s\n", __FILE__, __LINE__, report);
			return -1;
		}
    	MsgSendOpr(GLB_DB_UPDATE1, &msgSendInf, &ret);
		if(ret != 0)
		{
    		dcs_log(0, 0, "<FILE:%s,LINE:%d>修改发送数据库失败！sqlcode=%d", __FILE__, __LINE__, ret); 
			return 1;
		}		
		if(rRet == 0) return 0;
	}
}

int xmlRead(char *readBuff)
{
	char sDKContractRoot[2048];
	char buff[2048];
	char serviceCode[8 + 1];
	char srcSysPWD[32 + 1], srcSysUser[50];
	int ret;
	char *p;
	p = getXMLVal(readBuff, "SDKContractRoot", sDKContractRoot, sizeof(sDKContractRoot), &ret);
	if(0 > ret)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Get SDKContractRoot Fail!\nreadBuff = %s\n", __FILE__, __LINE__, readBuff);
		return -1;
	}
	p = getXMLVal(sDKContractRoot, "SDKTcpCont", buff, sizeof(buff), &ret);
	if(0 > ret)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Get SDKTcpCont Fail!\nsDKContractRoot = %s\n", __FILE__, __LINE__, sDKContractRoot);
		return -1;
	}
	p = getXMLVal(buff, "ServiceCode", serviceCode, sizeof(serviceCode), &ret);
	if(0 > ret)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Get ServiceCode Fail!\nsDKContractRoot = %s\n", __FILE__, __LINE__, buff);
		return -1;
	}
	p = getXMLVal(buff, "SrcSysUser", srcSysUser, sizeof(srcSysUser), &ret);
	if(0 > ret)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Get SrcSysUser Fail!\nsDKContractRoot = %s\n", __FILE__, __LINE__, buff);
		return -1;
	}
	p = getXMLVal(buff, "SrcSysPWD", srcSysPWD, sizeof(srcSysPWD), &ret);
	if(0 > ret)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Get SrcSysPWD Fail!\nsDKContractRoot = %s\n", __FILE__, __LINE__, buff);
		return -1;
	}
	if(strcmp(srcSysUser, SrcSysUser))
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>用户校验失败,SrcSysUser[%s][%s]", __FILE__, __LINE__, srcSysUser, SrcSysUser);
		return -4;
	}
	if(strcmp(srcSysPWD, SrcSysPwdMd5))
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>用户校验失败,SrcSysPWD[%s][%s]", __FILE__, __LINE__, srcSysPWD, SrcSysPwdMd5);
		return -5;
	}
	p = getXMLVal(sDKContractRoot, "SDKSvcCont", buff, sizeof(buff), &ret);
	if(0 > ret)
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>Get SDKSvcCont Fail!\nSDKContractRoot = %s\n", __FILE__, __LINE__, sDKContractRoot);
		return -1;
	}
	if(memcmp(serviceCode, "SVC00002", 8) == 0) //状态报告
	{
		return readMsgStat(buff);
	}
	else if(memcmp(serviceCode, "SVC00003", 8) == 0)//短信上行
	{
		return readMsg(buff);
	}
	else 
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>serviceCode=%s\n", __FILE__, __LINE__, serviceCode);
		return -2;
	}
	return -3;
}

int SendMsg(ISO_data *p_iso)
{
	char buff[2048], retBuff[2048];
	char transactionID[28 + 1];
	char content[1024];
	char msg[1024];
	char msgUTF8[1024];
	char *p;
	char keyIndex[3 + 1];
	struct  tm *tm;  
	msg_send_def msgSendInf;	
	WriteThis pooh;
	int ret, i, j, len;
	time_t  t;
	time(&t);
	tm = localtime(&t);	
	memset(content, 0, sizeof(content));
	memset(&msgSendInf, 0, sizeof(msg_send_def));
	//校验接收机构密钥
	if( 0 > ReadKeyData())
	{
		return returnSendMsgMesssage("N00096", p_iso);				
	}
	memset(keyIndex, 0, sizeof(keyIndex));
	memcpy(keyIndex, KeyIndex+2, 3);
	if ( 0 != BC_TermCheckMac(keyIndex, KeyData.key_1, g_Rcv_Buffer, g_RcvBufferSize))
	{
		dcs_log(0, 0, "<FILE:%s,LINE:%d>接收机构MAC校验未通过", __FILE__, __LINE__); 
		return returnSendMsgMesssage("N000A0", p_iso);				
	}
   	if((len = getbit(p_iso,  5, buff)) > 0)
    {	
    	memcpy(msgSendInf.phone, buff, MIN(16, len));
    	rtrim(msgSendInf.phone);
    }
    else
		return returnSendMsgMesssage("N00096", p_iso);				
   	if((len = getbit(p_iso,  11, buff)) > 0)
    	memcpy(msgSendInf.sys_tra_no, buff, MIN(6, len));
    else
		return returnSendMsgMesssage("N00096", p_iso);				
   	if((len = getbit(p_iso,  13, buff)) > 0)
   	{
    	memcpy(msgSendInf.loc_trans_dt+4, buff, MIN(4, len));
    	addYear(msgSendInf.loc_trans_dt);
    }
	else 
		return returnSendMsgMesssage("N00096", p_iso);				
	if((len = getbit(p_iso, 42, buff)) > 0)
	{
		memcpy(msgSendInf.mchnt_cd, buff, MIN(20, len));
		rtrim(msgSendInf.mchnt_cd);
	}
	else
		return returnSendMsgMesssage("N00096", p_iso);				
	if((len = getbit(p_iso, 48, msg)) > 0)
	{
		code_convert("GBK", "UTF-8", msg, strlen(msg), msgUTF8, 1024);
		ret = EncodeBase64(msgUTF8, buff, strlen(msgUTF8));
	}
	else
		return returnSendMsgMesssage("N00096", p_iso);				
	memcpy(msgSendInf.msg_txt, buff, MIN(ret, sizeof(msgSendInf.msg_txt)-1));
	sprintf(msgSendInf.send_time, "%04d%02d%02d%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    MsgSendOpr(GLB_DB_INSERT1, &msgSendInf, &i);
	if(i == -3114 || i == -3135)
	{
		dcs_log(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);
		if ( DasConectDB()==0 )
		{
			MsgSendOpr(GLB_DB_INSERT1, &msgSendInf, &i);
		}    
	}
    if(i == SQL_ERD_UNIKEY)
    {	 
    	if(msgSendInf.send_flag[0] != '2')
    	{
    		dcs_debug(0, 0, "<FILE:%s,LINE:%d>sqlcode=[%d],mchnt_cd[%s],loc_trans_dt[%ss],sys_tra_no[%s], phone[%s]", __FILE__, __LINE__, 
    			i, msgSendInf.mchnt_cd, msgSendInf.mchnt_cd, msgSendInf.loc_trans_dt, msgSendInf.sys_tra_no, msgSendInf.phone);
			return returnSendMsgMesssage("N00000", p_iso);				
		}
	}
    else if(i!=0)
    {
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>插入发送数据库失败！sqlcode=%d", __FILE__, __LINE__, i); 
		return returnSendMsgMesssage("N00096", p_iso);				
    }
	memset(content, 0, sizeof(content));
	for(i=0, j=0; i < ret; i++, j++)
	{	
		if(buff[i]=='/')
		{
			content[j++] = '%';
			content[j++] = '2';
			content[j] = 'F';
		}
		else if(buff[i]=='+')
		{
			content[j++] = '%';
			content[j++] = '2';
			content[j] = 'B';
		}
		else if(buff[i]=='=')
		{
			content[j++] = '%';
			content[j++] = '3';
			content[j] = 'D';
		}
		else
			content[j] = buff[i];
	}	
	memset(transactionID, 0, sizeof(transactionID));
	sprintf(transactionID, "%04d%02d%02d%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	memset(transactionID+14, '0', 14);
	memcpy(transactionID+14, msgSendInf.phone, min_strlen(msgSendInf.phone, 14));
	sprintf(buff,"data="
	"<SDKContractRoot>\n"
		"\t<SDKTcpCont>\n"
			"\t\t<ServiceCode>SVC00004</ServiceCode>\n"
			"\t\t<TransactionID>%s</TransactionID>\n"
			"\t\t<SrcOrgID>1000050001</SrcOrgID>\n"
			"\t\t<SrcSysUser>%s</SrcSysUser>\n"
			"\t\t<SrcSysPWD>%s</SrcSysPWD>\n"
			"\t\t<ReqTime>%s</ReqTime>\n"
		"\t</SDKTcpCont>\n"
		"\t<SDKSvcCont>\n"
			"\t\t<Phones>\n"
				"\t\t\t<Phone>%s</Phone>\n"
			"\t\t</Phones>\n"
			"\t\t<Ext>%s</Ext>\n"
			"\t\t<Sendtime>0000-00-00/00</Sendtime>\n"
			"\t\t<Content>%s</Content>\n"
		"\t</SDKSvcCont>\n"
	"</SDKContractRoot>\n",
	transactionID, SrcSysUser, SrcSysPwdMd5, msgSendInf.send_time, msgSendInf.phone, Ext, content);
	memset(retBuff, 0, sizeof(retBuff));
	pooh.writePtr = retBuff;
	pooh.sizeLeft = sizeof(retBuff);
	pooh.writeSize = 0;	
	ret = curl_post(buff, SendMsgUrl, &pooh);
    if(ret ==  0)
    {
    	dcs_debug(0, 0, "<FILE:%s,LINE:%d>retBuff=%s", __FILE__, __LINE__, buff);
    	while(1)
    	{
			p = getXMLVal(retBuff, "SDKContractRoot", buff, sizeof(buff), &ret);
			if(0 > ret)
			{
				dcs_log(0, 0, "<FILE:%s,LINE:%d>Get SDKContractRoot Fail!\nretBuff = %s\n", __FILE__, __LINE__, retBuff);
				ret = -1;
				break;
			}
			p = getXMLVal(buff, "SDKSvcCont", retBuff, sizeof(retBuff), &ret);
			if(0 > ret)
			{
				dcs_log(0, 0, "<FILE:%s,LINE:%d>Get SDKSvcCont Fail!\nSDKContractRoot = %s\n", __FILE__, __LINE__, buff);
				ret = -1;
				break;
			}
			p = getXMLVal(retBuff, "ResultCode", buff, sizeof(buff), &ret);
			if(0 > ret)
			{
				dcs_log(0, 0, "<FILE:%s,LINE:%d>Get ResultCode Fail!\nSDKSvcCont = %s\n", __FILE__, __LINE__, retBuff);
				ret = -1;
				break;
			}
			else
			{
				if(memcmp("00000000", buff, 8))
				{
					dcs_log(0, 0, "<FILE:%s,LINE:%d>Send Fail,ResultCode[%s]", __FILE__, __LINE__, buff);
					ret = -1;
					break;
				}	
				else
				{
			    	sprintf(msgSendInf.send_flag, "1");
				}
			}
			p = getXMLVal(retBuff, "MsgID", buff, sizeof(buff), &ret);
			if(0 > ret)
			{
				dcs_log(0, 0, "<FILE:%s,LINE:%d>Get MsgID Fail!\nSDKSvcCont = %s\n", __FILE__, __LINE__, retBuff);
				ret = -1;
				break;
			}
			else
			{
				memcpy(msgSendInf.msg_id, buff, MIN(strlen(buff), sizeof(msgSendInf.msg_id) - 1));
			}
			break;
		}
    }
    if(ret != 0)
    {
    	sprintf(msgSendInf.msg_id, "Flase_%s", transactionID);
    	sprintf(msgSendInf.send_flag, "2");
    }
    MsgSendOpr(GLB_DB_UPDATE2, &msgSendInf, &i);
    if(i!=0)
    {
    	dcs_log(0, 0, "<FILE:%s,LINE:%d>更新数据库失败！sqlcode=%d", __FILE__, __LINE__, i); 
    }
    if(ret == 0)
    {
		return returnSendMsgMesssage("N00000", p_iso);				
    }
    else
    {
		return returnSendMsgMesssage("N00096", p_iso);				
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


int LoadConfig()
{
	char cfgfile[256], caBuffer[1024];
	int flag;
	FILE *fd;
	strcpy(caBuffer, "config/sms.conf");
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
	memset(SendMsgUrl  , 0, sizeof(SendMsgUrl  )); 
	memset(SrcSysUser  , 0, sizeof(SrcSysUser  )); 
	memset(SrcSysPwd   , 0, sizeof(SrcSysPwd   )); 
	memset(SrcSysPwdMd5, 0, sizeof(SrcSysPwdMd5)); 
	memset(KeyIndex    , 0, sizeof(KeyIndex    )); 
	memset(InstIdCode  , 0, sizeof(InstIdCode  )); 
	memset(Ext         , 0, sizeof(Ext         )); 
	flag = 0;
	for(;!feof(fd);)
	{
		if(fgets(caBuffer, 1024, fd) == NULL) break;
		if(caBuffer[0] == '#') continue;					
		if(getval("SendMsgUrl", SendMsgUrl, caBuffer, sizeof(SendMsgUrl)))
			flag |= 2; 
		else if(getval("SrcSysUser", SrcSysUser, caBuffer, sizeof(SrcSysUser)))
			flag |= 4;
		else if(getval("SrcSysPwd", SrcSysPwd, caBuffer, sizeof(SrcSysPwd)))
		{
			md5(SrcSysPwd, SrcSysPwdMd5);
			flag |= 8;
		}
		else if(getval("KeyIndex", KeyIndex, caBuffer, sizeof(KeyIndex)))
		{
			flag |= 0x10;
		}
		else if(getval("InstIdCode", InstIdCode, caBuffer, sizeof(InstIdCode)))
		{
			flag |= 0x20;
		}
		else if(getval("Ext", InstIdCode, caBuffer, sizeof(InstIdCode)))
		{
			flag |= 0x40;
		}
	}		
	fclose(fd);	
	if(flag != 0x3E)
	{
		if(!(flag&2))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[SendMsgUrl] failed!\n",__FILE__,__LINE__);				
		else if(!(flag&4))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[SrcSysUser] failed!\n",__FILE__,__LINE__);			
		else if(!(flag&8))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[SrcSysPwd] failed!\n",__FILE__,__LINE__);	
		else if(!(flag&0x10))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[KeyIndex] failed!\n",__FILE__,__LINE__);	
       	else if(!(flag&0x20))
            dcs_log(0,0,"<FILE:%s,LINE:%d>read config para[InstIdCode] failed!\n",__FILE__,__LINE__);	
       	else
       		dcs_log(0,0,"<FILE:%s,LINE:%d>read config para failed![%02x]", __FILE__, __LINE__, flag);	
        return -1;
	}
    dcs_debug(0, 0, "<FILE:%s,LINE:%d>\nSendMsgUrl[%s]\nSrcSysUser[%s]\n"
    				"SrcSysPwd[%s][%s]\nKeyIndex[%s]\nInstIdCode[%s]\nExt[%s]\n", 
    				__FILE__, __LINE__, SendMsgUrl, SrcSysUser, SrcSysPwd, SrcSysPwdMd5, KeyIndex, InstIdCode, Ext);
	return 1;
}

int ReadKeyData()
{
	char cfgfile[256], caBuffer[1024];
	FILE *fd;
	char *p, chk[4];
	int i;
	sprintf(caBuffer, "key/SMS%s.key", InstIdCode);
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
	sprintf(caBuffer, "key/SMS%s.key", InstIdCode);
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
	setbit_t(iso, 28, resp_code, 6);
	setbit_t(iso, 33, InstIdCode, 8);
	if(0 == memcmp(resp_code, "N00000", 2))
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
        ReturnSignMessage("N00096", p_iso);
        return -1;
    }
    strcpy(KeyData.key_2, KeyData.key_1);
    strcpy(KeyData.key_3, KeyData.key_1);
    if(0> WriteKeyData())
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>DESGETKY failure.",__FILE__,__LINE__);
        ReturnSignMessage("N00096", p_iso);
        return -1;
    }
    ReturnSignMessage("N00000", p_iso);
}
