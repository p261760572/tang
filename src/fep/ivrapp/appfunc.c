#include "ibdcs.h"
#include "tmcibtms.h"
#include "dccdcs.h"
#include "bc.h"
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

MONI_STRUCT g_Moni_Data;
char   *g_pcBcdaShmPtr;
int g_iSrcFoldId = -1;
char g_iSrcFoldName[30+1] ;
char g_srcPacketType[5];
char g_Rcv_Buffer[4096] ;
int g_RcvBufferSize ;
int g_FromFolderId = -1;
int g_ForkFlag = 0;

struct ISO_8583 iso8583_ZDXTconf[128];
struct ISO_8583 iso8583_TERMconf[128];
struct ISO_8583 iso8583_MCHNconf[128];
struct ISO_8583 iso8583_POSBconf[128];
struct ISO_8583 iso8583_AJXTconf[128];

Folder_Msg g_folder_msg;
BcMacMsg g_MacMsg;

ti_bc_term_inf_def            g_tiBcTermInf;
ti_bc_ivr_opr_impdata_inf_def g_tiBcIvrOprImpdataInf;
ti_bc_ins_inf_def             g_tiBcInsInf ;
ti_bc_ivr_opr_cmd_inf_def     g_tiBcIvrOprCmdInf;
ti_bc_ivr_inf_key_inf_def     g_tiBcIvrInfKeyInf;

void strcpy_safe(char *des, char *src, int len)
{
  int i;
  if( 0 == src ) {
        if(des) *des = 0;
        return;
  }
        for(i=0;i<len&&*(src+i)!=0;i++) *(des+i)=*(src+i);
        *(des+i) = 0;
}

void	tiPackFailToAppl(char *srcBuf, char *caRspCode, char *caRspMsg, char *destBuf, int *piLen)
{
	int    iLen=0;

	memcpy( destBuf, srcBuf, 12);
	iLen = 12;
	memcpy( destBuf+iLen, "1", 1);
	iLen = iLen + 1;
	memcpy( destBuf+iLen, srcBuf+iLen, 46);
	iLen = iLen + 46;
	sprintf( destBuf+iLen, "<~~RSPCODE~~>%s<~~/~~><~~RSPMSG~~>%s<~~/~~>", caRspCode, caRspMsg);
	*piLen = strlen(destBuf);
}

void	tiPackFailToIvr(char *srcBuf, char *caRspCode, char *caRspMsg, char *destBuf, int *piLen)
{
	char   caTmp[100];
    	int    sqlCode;
	int    iLen=0,iRet;
    	char   mac_out[100];
    	ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ;

	iLen = 0;
	memcpy( destBuf+iLen, srcBuf+iLen, 1);
	iLen = iLen + 1;
	memcpy( destBuf+iLen, "1", 1);
	iLen = iLen + 1;
	memcpy( destBuf+iLen, srcBuf+iLen, 50);
	iLen = iLen + 50;
	memcpy( destBuf+iLen, "00220", 5);
	iLen = iLen + 5;
	sprintf( destBuf+iLen, "006%s", caRspCode);
	iLen = iLen + 9;

    	/* 计算MAC */
    	memset(&g_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
    	strcpy(p_tiBcInsInf->ins_id_cd, INS_IVR );
    	iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    	if ( 0 != sqlCode )
    	{
       		dcs_log(0,0,"<FILE:%s,LINE:%d><tiPackFailToAppl>tiBcInsInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
		return;
    	}

    	memset(mac_out,0,sizeof(mac_out));
    	iRet = BC_TermGenMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1,destBuf,iLen,mac_out);
    	if ( 0 == iRet )
    	{
    		memcpy(destBuf+iLen, mac_out, 8);
    		iLen = iLen + 8;
	}
	*piLen = iLen;
}

void	tiPackFailToIvrAddHangUp(char *srcBuf, char *caRspCode, char *caRspMsg, char *destBuf, int *piLen)
{
	char   caTmp[100];
    	int    sqlCode;
	int    iLen=0,iRet;
    	char   mac_out[100];
    	char   caBuf[1024];
    	ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ;

	iLen = 0;
	memcpy( destBuf+iLen, srcBuf+iLen, 1);
	iLen = iLen + 1;
	memcpy( destBuf+iLen, "1", 1);
	iLen = iLen + 1;
	memcpy( destBuf+iLen, srcBuf+iLen, 50);
	iLen = iLen + 50;
	memcpy( destBuf+iLen, "0042012", 7);
	iLen = iLen + 7;
	sprintf( destBuf+iLen, "006%s", caRspCode);
	iLen = iLen + 9;

    	/* 计算MAC */
    	memset(&g_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
    	strcpy(p_tiBcInsInf->ins_id_cd, INS_IVR );
    	iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    	if ( 0 != sqlCode )
    	{
       		dcs_log(0,0,"<FILE:%s,LINE:%d><tiPackFailToAppl>tiBcInsInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
		return;
    	}

    	memset(mac_out,0,sizeof(mac_out));
    	iRet = BC_TermGenMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1,destBuf,iLen,mac_out);
    	if ( 0 == iRet )
    	{
    		memcpy(destBuf+iLen, mac_out, 8);
    		iLen = iLen + 8;
	}
	*piLen = iLen;
}

//srcBuf 前4位为报文类型,紧后面是0005或者0004，后面是报文，解包原数据存放在srcBuf, 组包后数据放在destBuf中, APPL到IVR的报文转换
int BC_ApplConvToIvr( char *srcBuf,  int iSrcLen, char *destBuf, int *piLen, char *packkeyid)
{
    int    sqlCode;
    char   caOutput[4096];
    char   caInfo[4096];
    char   opr_cmd[4096];
    char   caOprList[4096];
    char   caTotalInfo[4096];
    char   caPacketInfo[4096];
    char   caRspCode[100];
    char   caRspMsg[4096];
    char   mac_out[100];
    char   caTmp[100];
    char   *caString;
    int    iLen,iInfoLen=0,iOprListlen=0;
    int    iRet;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ;
    ti_bc_ivr_opr_cmd_inf_def *p_tiBcIvrOprCmdInf= &g_tiBcIvrOprCmdInf;

    memset( opr_cmd, 0, sizeof(opr_cmd));
    dcs_debug(srcBuf, iSrcLen, "<FILE:%s,LINE:%d><BC_ApplConvToIvr>into BC_ApplConvToIvr[%s]",__FILE__,__LINE__,INS_IVR);
    memcpy(opr_cmd, srcBuf+8, 4);
    
    memset( caRspCode, 0, sizeof(caRspCode));
    if (!memcmp(srcBuf+12, "1", 1))   /* 回应 */
    {
        caString = (char *)strstr( (char *)srcBuf, "<~~RSPCODE~~>");
        if (caString)
        {
        	memcpy( caRspCode, caString+13, 6);
//		if ((strcmp(caRspCode, "N30301")) &&  /* 证件号码不符 */
//		    (strcmp(caRspCode, "N00065")) &&  /* CVN2错误 */
//		    (strcmp(caRspCode, "N00000")) &&  /* 正确 */
//		    (strcmp(caRspCode, "NA0000")) &&  /* 定单有效，输入密码 */
//		    (strcmp(caRspCode, "NA0001")) &&  /* 定单有效，输入信用卡重要信息 */
//		    (strcmp(caRspCode, "N00053")) &&  /* 有效期错误1 */ 
//		    (strcmp(caRspCode, "N10053")) &&  /* 有效期错误2 */ 
//		    (strcmp(caRspCode, "N20053")) &&  /* 有效期错误3 */ 
//		    (strcmp(caRspCode, "N0PSE1")) &&  /* 密码错误1 */ 
//		    (strcmp(caRspCode, "N0PSE2")) &&  /* 密码错误2 */ 
//		    (strcmp(caRspCode, "N0PSE3")) &&  /* 密码错误3 */ 
//		    (strcmp(caRspCode, "N00051")) &&  /* 账户可用余额不足，信用额度不足，取现额度超限 */
//		    (strcmp(caRspCode, "N00061")) &&  /* 超出金额限制 */
//		    (strcmp(caRspCode, "N00075")) &&  /* 密码输入错误次数超限 */
//		    (strcmp(caRspCode, "N00054")) &&  /* 您的卡已经过期，请使用其他卡支付 */
//		    (strcmp(caRspCode, "N00055")) &&  /* 你输入的密码错误, 交易失败 */
//		    (strcmp(caRspCode, "N00038"))     /* 密码错误次数超限，请与发卡方联系  */
//		    )    
    		memset(&g_tiBcIvrInfKeyInf, 0, sizeof(ti_bc_ivr_inf_key_inf_def));
    		memcpy(g_tiBcIvrInfKeyInf.opr_cmd, srcBuf+8, 4);
    		memcpy(g_tiBcIvrInfKeyInf.rspcode, caString+13, 6);
       		dcs_log(0,0,"<FILE:%s,LINE:%d><BC_ApplConvToIvr>tiBcIvrInfKeyInfOpr opr_cmd[%s]rspcode[%s].",__FILE__,__LINE__,g_tiBcIvrInfKeyInf.opr_cmd,g_tiBcIvrInfKeyInf.rspcode);
    		tiBcIvrInfKeyInfOpr(GLB_DB_SELECT1, &g_tiBcIvrInfKeyInf, &sqlCode);
    		if ( (-3114 == sqlCode) || (-3135 == sqlCode))
    		{
        		BC_SendExeception("03","数据库连接异常，自动重连数据库。");
        		dcs_debug(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);
   		 
        		if ( DasConectDB()==0 )
        		{
            			tiBcIvrInfKeyInfOpr(GLB_DB_SELECT1, &g_tiBcIvrInfKeyInf, &sqlCode);
        		}
    		}
    		if ( (0 != sqlCode) && (SQL_ERD_NONE_RECORD != sqlCode) )
    		{
       			dcs_log(0,0,"<FILE:%s,LINE:%d><BC_ApplConvToIvr>tiBcIvrInfKeyInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
			return -1;
    		}
		if (SQL_ERD_NONE_RECORD == sqlCode )
		{
        		memcpy( caRspCode, "N99999", 6);
    			memcpy(caString+13, "N99999", 6);
       			dcs_log(0,0,"<FILE:%s,LINE:%d>srcBuf[%s].",__FILE__,__LINE__,srcBuf);
		}
        }
	else
	{
        	dcs_log(0,0,"<FILE:%s,LINE:%d><BC_ApplConvToIvr>RSPCODE is not null.",__FILE__,__LINE__);
		return -1;
	}
    }
    else                              /* 请求 */
    {
        	memcpy( caRspCode, "*", 1);
    }

    iLen = 0;
    memset( caOutput, 0, sizeof(caOutput));
    if (!memcmp(srcBuf+12, "0", 1))   /* 请求 */
    {
       	memcpy( caOutput+iLen, "20", 2);
    }
    else                              /* 回应 */
    {
       	memcpy( caOutput+iLen, "11", 2);
    }
    iLen = iLen + 2;
    /*memcpy( caOutput+iLen, srcBuf+13, 8); gy test*/
    memcpy( caOutput+iLen, INS_IVR, 8); 
    iLen = iLen + 8;
    if (!memcmp(srcBuf+12, "0", 1))   /* 请求 */
    {
    	memcpy( caOutput+iLen, opr_cmd, 4);
    	memcpy( packkeyid, opr_cmd, 4);
    }
    else                              /* 回应 */
    {
    	memset(&g_tiBcIvrOprCmdInf, 0, sizeof(ti_bc_ivr_opr_cmd_inf_def));
    	memcpy(g_tiBcIvrOprCmdInf.dest_opr_cmd, srcBuf+8, 4);
	
    	tiBcIvrOprCmdInfOpr(GLB_DB_SELECT2, p_tiBcIvrOprCmdInf, &sqlCode);
    	if ( (-3114 == sqlCode) || (-3135 == sqlCode))
    	{
        	BC_SendExeception("03","数据库连接异常，自动重连数据库。");
        	dcs_debug(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);
   	 
        	if ( DasConectDB()==0 )
        	{
            		tiBcIvrOprCmdInfOpr(GLB_DB_SELECT2, p_tiBcIvrOprCmdInf, &sqlCode);	
        	}
    	}
    	if ( 0 != sqlCode )
    	{
       		dcs_log(0,0,"<FILE:%s,LINE:%d><BC_ApplConvToIvr>tiBcIvrOprCmdInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
		return -1;
    	}
    	memcpy( caOutput+iLen, g_tiBcIvrOprCmdInf.src_opr_cmd, 4);
    	memcpy( packkeyid, g_tiBcIvrOprCmdInf.src_opr_cmd, 4);
    }
    iLen = iLen + 4;
    memcpy( caOutput+iLen, srcBuf+21, 38);
    memcpy( packkeyid+4, srcBuf+21, 38);
    iLen = iLen + 38;
    memset( caInfo, 0, sizeof(caInfo));
    memset( caOprList, 0, sizeof(caOprList));
    memset( caPacketInfo, 0, sizeof(caPacketInfo));
    memcpy( caPacketInfo, srcBuf+59, iSrcLen - 59);
    iRet = tiBcApplConvToIvrOpr(caPacketInfo, opr_cmd, caRspCode, caOprList, caInfo, &sqlCode, &iOprListlen, &iInfoLen);
    dcs_debug(0,0,"<FILE:%s,LINE:%d>tiBcApplConvToIvrOpr sqlcode[%d]\n",__FILE__,__LINE__,sqlCode);
    if ( (-3114 == sqlCode) || (-3135 == sqlCode))
    {
       	BC_SendExeception("03","数据库连接异常，自动重连数据库。");
       	dcs_debug(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);
    
       	if ( DasConectDB()==0 )
       	{
    		iRet = tiBcApplConvToIvrOpr(caPacketInfo, opr_cmd, caRspCode, caOprList, caInfo, &sqlCode, &iOprListlen, &iInfoLen);
       	}
    }
    if (iRet)
    {
       	dcs_log(0,0,"<FILE:%s,LINE:%d><BC_ApplConvToIvr>tiBcIvrInfKeyInfConv failure[%d].",__FILE__,__LINE__,iRet);
    	if (!memcmp(srcBuf+12, "0", 1))   /* 请求 */
    	{
    		tiPackFailToAppl(srcBuf, "N99999", "转换报文错误", destBuf, piLen);
       		dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_ApplConvToIvr>tiBcIvrInfKeyInfConv failure.",__FILE__,__LINE__);
		return -2;
	}
	return -1;
    }
    sprintf(caOutput+iLen, "%03d", iOprListlen);
    iLen = iLen + 3;
    memcpy(caOutput+iLen, caOprList, iOprListlen);
    iLen = iLen + iOprListlen;
    sprintf(caOutput+iLen, "%03d", iInfoLen);
    iLen = iLen + 3;
    memcpy(caOutput+iLen, caInfo, iInfoLen);
    iLen = iLen + iInfoLen;
    
    /* 计算MAC */
    memset(&g_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
    strcpy(p_tiBcInsInf->ins_id_cd, INS_IVR );
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
       	dcs_log(0,0,"<FILE:%s,LINE:%d><BC_ApplConvToIvr>tiBcInsInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
    	if (!memcmp(srcBuf+12, "0", 1))   /* 请求 */
    	{
    		tiPackFailToAppl(srcBuf, "N99999", "查询ti_bc_ins_inf表错误", destBuf, piLen);
       		dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_ApplConvToIvr>tiBcInsInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
		return -2;
	}
	return -1;
    }

    memset(mac_out,0,sizeof(mac_out));
    iRet = BC_TermGenMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1,caOutput,iLen,mac_out);
    if ( 0 == iRet )
    {
    	memcpy(caOutput+iLen, mac_out, 8);
    	iLen = iLen + 8;
    } else
    {
        //加密失败
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_TermGenMac>加密调用失败.",__FILE__,__LINE__);
    	if (!memcmp(srcBuf+12, "0", 1))   /* 请求 */
    	{
    		tiPackFailToAppl(srcBuf, "N99999", "计算MAC失败", destBuf, piLen);
        	dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_TermGenMac>加密调用失败.",__FILE__,__LINE__);
		return -2;
	}
	return -1;
    }
    memcpy( destBuf, caOutput, iLen);
    *piLen = iLen;

    dcs_debug(destBuf, *piLen, "<FILE:%s,LINE:%d><BC_ApplConvToIvr>BC_ApplConvToIvr complated!",__FILE__,__LINE__);
    return 0;
}

//destBuf 前4位为报文类型,紧后面是0005或者0004，后面是报文，解包原数据存放在srcBuf, 组包后数据放在destBuf中, IVR到APPL的报文转换
int BC_IvrConvToAppl( char *srcBuf,  int iSrcLen, char *destBuf, int *piLen)
{
    int    sqlCode;
    ti_bc_ivr_opr_impdata_inf_def *p_tiBcIvrOprImpdataInf= &g_tiBcIvrOprImpdataInf;
    ti_bc_ivr_opr_cmd_inf_def *p_tiBcIvrOprCmdInf= &g_tiBcIvrOprCmdInf;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ;
    char   caOutput[4096];
    char   caInfo[4096];
    char   caTotalInfo[4096];
    char   caPacketInfo[4096];
    char   caMacData[4096];
    char   caRspCode[100];
    char   caTmp[4096];
    char   caTmpBuf[4096];
    char   caInTmp[4096];
    char   caOutTmp[4096];
    char   packkeyid[4096];
    char   *caString;
    int    i,iLen,iOutLen,iMacDataLen,iLength,iMessLen=0;
    int    iRet;
    char   ivrsrcBuf[4096];

    dcs_debug(srcBuf, iSrcLen, "<FILE:%s,LINE:%d><BC_IvrConvToAppl>into BC_IvrConvToAppl",__FILE__,__LINE__);

/* gy test
    memset( caOutput, 0, sizeof(caOutput));
    memcpy( caOutput, srcBuf, 2);
if (!memcmp(caOutput, "10", 2))
{
	sleep(60);
}
*/
/* gy test */
if (0)
{
    if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    {
	if (!memcmp(srcBuf+10, "OP05", 4))
	{
    		memset( ivrsrcBuf, 0, sizeof(ivrsrcBuf));
    		memset( packkeyid, 0, sizeof(packkeyid));
    		memset( caTmp, 0, sizeof(caTmp));
		memcpy( caTmp, srcBuf+14, 38);
		/*sprintf( ivrsrcBuf, "CMDI0005OP05188888888%s<~~BILLKEY~~>OP01001101        302430145119998     0427<~~/~~><~~RSPCODE~~>NA0000<~~/~~><~~MERNAME~~>高汇通<~~/~~><~~INFO~~>订购2011/04／21从北京-长沙19：01CA1103航班机票一张<~~/~~><~~AMT~~>000000550000<~~/~~>", caTmp);*/
		sprintf( ivrsrcBuf, "CMDI0005OP05188888888%s<~~RSPCODE~~>N99999<~~/~~>", caTmp);
		BC_ApplConvToIvr( ivrsrcBuf,  strlen(ivrsrcBuf), destBuf, piLen, packkeyid);
       		dcs_log(destBuf,*piLen,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>BC_ApplConvToIvr .",__FILE__,__LINE__);
		return -2;
	}
    }
}
/* */
    iLen = 0;
    memset( caOutput, 0, sizeof(caOutput));
    memcpy( caOutput, "CMDI0005", 8);
    if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    {
    	memset(&g_tiBcIvrOprCmdInf, 0, sizeof(ti_bc_ivr_opr_cmd_inf_def));
    	memcpy(g_tiBcIvrOprCmdInf.src_opr_cmd, srcBuf+10, 4);
	
    	tiBcIvrOprCmdInfOpr(GLB_DB_SELECT1, p_tiBcIvrOprCmdInf, &sqlCode);
    	if ( (-3114 == sqlCode) || (-3135 == sqlCode))
    	{
       		BC_SendExeception("03","数据库连接异常，自动重连数据库。");
       		dcs_debug(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);
   	 
       		if ( DasConectDB()==0 )
       		{
    			tiBcIvrOprCmdInfOpr(GLB_DB_SELECT1, p_tiBcIvrOprCmdInf, &sqlCode);
       		}
    	}
    	if ( 0 != sqlCode )
    	{
       		dcs_log(0,0,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiBcIvrOprCmdInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
   		if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    		{
    			tiPackFailToIvrAddHangUp(srcBuf, "N99999", "", destBuf, piLen);
       			dcs_log(destBuf,*piLen,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiBcIvrOprCmdInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
			return -2;
		}
		return -1;
    	}
    	memcpy( caOutput+8, g_tiBcIvrOprCmdInf.dest_opr_cmd, 4); /* 指令串 */
    }
    else
    {
    	memcpy(caOutput+8, srcBuf+10, 4);
    }
    iLen = iLen + 12;
    if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    {
        memcpy( caOutput+iLen, "0", 1);
    }
    else
    {
       	memcpy( caOutput+iLen, "1", 1);
    }
    iLen = iLen + 1;
    memcpy( caOutput+iLen, srcBuf+2, 8);
    iLen = iLen + 8;
    memcpy( caOutput+iLen, srcBuf+14, 38);
    iLen = iLen + 38;

    memset(&g_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
    strcpy(p_tiBcInsInf->ins_id_cd, INS_IVR );
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( (-3114 == sqlCode) || (-3135 == sqlCode))
    {
    	BC_SendExeception("03","数据库连接异常，自动重连数据库。");
        dcs_debug(0,0,"<FILE:%s,LINE:%d>数据库连接异常，自动重连数据库。\n",__FILE__,__LINE__);
   	 
       	if ( DasConectDB()==0 )
       	{
    		iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
       	}
    }
    if ( 0 != sqlCode )
    {
       	dcs_log(0,0,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiBcInsInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
    	if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    	{
    		tiPackFailToIvrAddHangUp(srcBuf, "N99999", "", destBuf, piLen);
       		dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiBcInsInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
		return -2;
	}
	return -1;
    }
	
    iMacDataLen = iSrcLen ;
    memset( caMacData, 0, sizeof(caMacData));
    memcpy( caMacData, srcBuf, iMacDataLen);
    /* 检查MAC*/
    if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, caMacData,iMacDataLen) )
    {
       	dcs_log(0,0,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>BC_TermCheckMac failure.",__FILE__,__LINE__);
	tiIvrSignIn();
    	if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    	{
    		tiPackFailToIvr(srcBuf, "N000A0", "", destBuf, piLen);
       		dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>BC_TermCheckMac failure.",__FILE__,__LINE__);
		return -2;
	}
	return -1;
    }
    memset( caTmp, 0, sizeof(caTmp));
    memcpy( caTmp, srcBuf+52, 3);
    iLength = atoi(caTmp);
    if (iLength == 0)
    {
    	if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    	{
    		tiPackFailToIvrAddHangUp(srcBuf, "N99999", "", destBuf, piLen);
       		dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>请求报文没有指令.",__FILE__,__LINE__);
		return -2;
	}
      	dcs_log(0, 0,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>回应报文没有指令.",__FILE__,__LINE__);
	return -1;
    }
    for (i=0; i<(iLength/2); i++)
    {
    	memset(&g_tiBcIvrOprImpdataInf, 0, sizeof(ti_bc_ivr_opr_impdata_inf_def));
	memcpy(g_tiBcIvrOprImpdataInf.opr_list, srcBuf+55+i*2, 2);

    	tiBcIvrOprImpdataInfOpr(GLB_DB_SELECT1, p_tiBcIvrOprImpdataInf, &sqlCode);
    	if ( 0 != sqlCode )
    	{
        	dcs_log(0,0,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiBcIvrOprImpdataInfOpr[%s] failure[sqlcode:%d].",__FILE__,__LINE__,g_tiBcIvrOprImpdataInf.opr_list,sqlCode);
    		if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    		{
    			tiPackFailToIvrAddHangUp(srcBuf, "N99999", "", destBuf, piLen);
        		dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiBcIvrOprImpdataInfOpr failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
			return -2;
		}
		return -1;
    	}
	rtrim(p_tiBcIvrOprImpdataInf->keyid);
	if (p_tiBcIvrOprImpdataInf->data_flag[0] == '0' )
	{
        	memcpy( caOutput+iLen, p_tiBcIvrOprImpdataInf->keyid, strlen(p_tiBcIvrOprImpdataInf->keyid));
		iLen = iLen + strlen(p_tiBcIvrOprImpdataInf->keyid);
    		memset( caInTmp, 0, sizeof(caInTmp)); /* 转换前数据 */
    		memset( caOutTmp, 0, sizeof(caOutTmp));  /* 转换后数据 */
       		memcpy( caInTmp, srcBuf+58+iLength+iMessLen, p_tiBcIvrOprImpdataInf->data_len);
		if (!strcmp(caInTmp, "N000A0"))  /* MAC 错误 */
		{
			tiIvrSignIn();
		}
    		iRet = tiConvtString(caInTmp, p_tiBcIvrOprImpdataInf->data_len, p_tiBcIvrOprImpdataInf->pin_flag, p_tiBcIvrOprImpdataInf->expand_flag, caOutTmp, &iOutLen);
    		if (iRet)
    		{
       			dcs_log(0,0,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiConvtString failure.",__FILE__,__LINE__);
    			if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    			{
    				tiPackFailToIvrAddHangUp(srcBuf, "N99999", "", destBuf, piLen);
        			dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiBcIvrOprImpdataInfOpr failure.",__FILE__,__LINE__);
				return -2;
			}
			return -1;
		}
        	memcpy( caOutput+iLen, caOutTmp, iOutLen);
		iMessLen = iMessLen + p_tiBcIvrOprImpdataInf->data_len;
		iLen = iLen + iOutLen;
        	memcpy( caOutput+iLen, "<~~/~~>", 7);
		iLen = iLen + 7;
	}
	else if (p_tiBcIvrOprImpdataInf->data_flag[0] == '1' )
	{
        	memcpy( caOutput+iLen, p_tiBcIvrOprImpdataInf->keyid, strlen(p_tiBcIvrOprImpdataInf->keyid));
		iLen = iLen + strlen(p_tiBcIvrOprImpdataInf->keyid);
    		memset( caTmp, 0, sizeof(caTmp));
    		memset( caTmpBuf, 0, sizeof(caTmpBuf));
    		memcpy( caTmp, srcBuf+58+iLength+iMessLen, 2);
		bcd_to_asc((unsigned char*)caTmpBuf,(unsigned char*)caTmp,4,0);
    		memset( caInTmp, 0, sizeof(caInTmp)); /* 转换前数据 */
    		memset( caOutTmp, 0, sizeof(caOutTmp));  /* 转换后数据 */
        	memcpy( caInTmp, srcBuf+58+iLength+iMessLen+2, atoi(caTmpBuf));
    		iRet = tiConvtString(caInTmp, atoi(caTmpBuf), p_tiBcIvrOprImpdataInf->pin_flag, p_tiBcIvrOprImpdataInf->expand_flag, caOutTmp, &iOutLen);
    		if (iRet)
    		{
       			dcs_log(0,0,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiConvtString failure.",__FILE__,__LINE__);
    			if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    			{
    				tiPackFailToIvrAddHangUp(srcBuf, "N99999", "", destBuf, piLen);
        			dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiBcIvrOprImpdataInfOpr failure.",__FILE__,__LINE__);
				return -2;
			}
			return -1;
		}
        	memcpy( caOutput+iLen, caOutTmp, iOutLen);
		iMessLen = iMessLen + 2 + atoi(caTmpBuf);
		iLen = iLen + iOutLen;
        	memcpy( caOutput+iLen, "<~~/~~>", 7);
		iLen = iLen + 7;
	}
	else if (p_tiBcIvrOprImpdataInf->data_flag[0] == '2' )
	{
        	memcpy( caOutput+iLen, p_tiBcIvrOprImpdataInf->keyid, strlen(p_tiBcIvrOprImpdataInf->keyid));
		iLen = iLen + strlen(p_tiBcIvrOprImpdataInf->keyid);
    		memset( caTmp, 0, sizeof(caTmp));
    		memset( caTmpBuf, 0, sizeof(caTmpBuf));
    		memcpy( caTmp, srcBuf+58+iLength+iMessLen, 1);
		bcd_to_asc((unsigned char*)caTmpBuf,(unsigned char*)caTmp,2,0);
    		memset( caInTmp, 0, sizeof(caInTmp)); /* 转换前数据 */
    		memset( caOutTmp, 0, sizeof(caOutTmp));  /* 转换后数据 */
        	memcpy( caInTmp, srcBuf+58+iLength+iMessLen+1, atoi(caTmpBuf));
    		iRet = tiConvtString(caInTmp, atoi(caTmpBuf), p_tiBcIvrOprImpdataInf->pin_flag, p_tiBcIvrOprImpdataInf->expand_flag, caOutTmp, &iOutLen);
    		if (iRet)
    		{
       			dcs_log(0,0,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiConvtString failure.",__FILE__,__LINE__);
    			if (!memcmp(srcBuf+1, "0", 1)) /* 请求 */
    			{
    				tiPackFailToIvrAddHangUp(srcBuf, "N99999", "", destBuf, piLen);
        			dcs_log(destBuf, *piLen,"<FILE:%s,LINE:%d><BC_IvrConvToAppl>tiBcIvrOprImpdataInfOpr failure.",__FILE__,__LINE__);
				return -2;
			}
			return -1;
		}
        	memcpy( caOutput+iLen, caOutTmp, iOutLen);
		iMessLen = iMessLen + 1 + atoi(caTmpBuf);
		iLen = iLen + iOutLen;
        	memcpy( caOutput+iLen, "<~~/~~>", 7);
		iLen = iLen + 7;
	}
    }

    memcpy( destBuf, caOutput, iLen);
    *piLen = iLen;

    dcs_debug(destBuf, *piLen, "<FILE:%s,LINE:%d><BC_IvrConvToAppl>BC_IvrConvToAppl complated!",__FILE__,__LINE__);
    return 0;
}

int tiConvtString(char *pcaInTmp, int idata_len, char *pin_flag, char *expand_flag, char *pcaOutTmp, int *iOutLen)
{
	int	iLen,iExpandLen;
	char   	caTmp[4096];
	char   	caTmpBuf[4096];
	char   	caInTmp[4096];
	char   	caOutTmp[4096];

   	memset( caTmp, 0, sizeof(caTmp));
    	memset( caTmpBuf, 0, sizeof(caTmpBuf));
   	memset( caInTmp, 0, sizeof(caInTmp)); 
    	memset( caOutTmp, 0, sizeof(caOutTmp));

	memcpy( caTmp, pcaInTmp, idata_len );
	if (pin_flag[0] == '1')
	{
		/* 转换PINBLOCK */
		memcpy( caTmpBuf, caTmp, idata_len );
		iLen = idata_len;
	}
	else
	{
		/* 不转换PINBLOCK */
		memcpy( caTmpBuf, caTmp, idata_len );
		iLen = idata_len;
	}

	memcpy( caInTmp, caTmpBuf, iLen );
	if (expand_flag[0] == '1')
	{
		/* 展开数据 */
		bcd_to_asc((unsigned char*)caOutTmp,(unsigned char*)caInTmp,iLen*2,0);
		iExpandLen = iLen * 2;
	}
	else
	{
		/* 不展开数据 */
		memcpy( caOutTmp, caInTmp, iLen );
		iExpandLen = iLen;
	}

	memcpy( pcaOutTmp, caOutTmp, iExpandLen );
	*iOutLen = iExpandLen;

	return 0;
}

void BC_StringAdder(char *value, int addvalue, char* outbuff,int outlen)
{
    char slen[10];
    sprintf(slen  , "%%.%dd", outlen);
    sprintf(outbuff, slen, atoi(value) + addvalue ); 
}

int BC_StringtoUpper(char *value, int len)
{
    int i;
    for ( i=0; i<len; i++ )
    {
        if ( value[i] >='a' && value[i] <='z' )
        {
            value[i] -= 'a' - 'A';
        }
    }
    return 0;
}

int IsReady(char *pcaName)
{
    int iBnkIdx;
    struct BCDAst *pBCDAst;

    pBCDAst = (struct BCDAst *)g_pcBcdaShmPtr;
    if ( !pBCDAst )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>通信机构共享内存区尚未连接就绪!",__FILE__,__LINE__);
        return -1;
    }

    iBnkIdx = BC_GetFolderIdByName(pcaName);
    if ( iBnkIdx < 0 )  //invalid bank code
    {
        dcs_debug(0,0,"<FILE:%s,LINE:%d>LocateBankByName invalid pcaName =[%s]",__FILE__,__LINE__,pcaName);
        return -1;
    }

    if ( pBCDAst->stBcda[iBnkIdx].cNetFlag <= 0 )
    {
        //无网络连接或没有发开始请求报文
        dcs_debug(0,0,"<FILE:%s,LINE:%d>cNetFlag is 0 net not connect [%s]",__FILE__,__LINE__,pcaName);
        return -1;
    }

    return 1;
}

//根据名称得到 folderid
int BC_GetFolderIdByName(char *pcaBankName)
{
    int    iBnkNum, i;
    struct BCDA *pBCDA;


    iBnkNum = ((struct BCDAst *)g_pcBcdaShmPtr)->iBankNum;
    pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;

    for ( i=0; i < iBnkNum; i++ )
    {
        if ( memcmp(pcaBankName, pBCDA[i].caBankName,strlen(pcaBankName)) == 0 )
            return(i);
    }

    return(-1);
}

//根据机构 folderid 得到名称
int LocateNameByFdid(int iFld,char *caBankName)
{
    struct BCDA *pBCDA;
    pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;
    memcpy(caBankName,pBCDA[iFld].caBankName,15);
    return(0);
}

int BC_SendMessageToFolder_direct( char *data, int len)
{
    int    iRc ;
    int    iDestFolderId = g_iSrcFoldId ;

    //send
    iRc = fold_write(iDestFolderId, -1, data, len);
    if ( iRc < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BC_SendMessageToFolder_direct>Failure to write msg to folder[%d]!",__FILE__,__LINE__,iDestFolderId);
        return -1;
    } else
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BC_SendMessageToFolder_direct>Success to write msg to folder[%d]",__FILE__,__LINE__,iDestFolderId );
        return 0;
    }
    return 0;
}

int BC_SendMessageToFolder( char *resp )
{
    int    iRc, nbytes;
    int    iDestFolderId = -1;
    char   caBankName[5];
    char   caBankCode[15];

    //memset(g_stIbTwa.stIbTba.caTxnMsg, '\0', 1500 );
    iRc = 0;
    strcpy(resp, "00");

    if ( g_folder_msg.OutFoldType != 0 )
    {
        //g_iHstSrvFoldId = fold_locate_folder("SECSRV");
        //iDestFolderId = BC_GetFolderIdByName( g_folder_msg.OutFoldName );
        iDestFolderId = fold_locate_folder( g_folder_msg.OutFoldName );
        if ( iDestFolderId < 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SendMessageToFolder>failure to LocateBankByName[%s] !",__FILE__,__LINE__,
                    g_folder_msg.OutFoldName);
            strcpy(resp, "91");
            return -1;
        } else
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SendMessageToFolder>call BC_GetFolderIdByName success[foldId:%d,foldName:%s] !",__FILE__,__LINE__,
                    iDestFolderId, g_folder_msg.OutFoldName);
        }
    } else
        // if msg_type is RESPONSE, reply it.
        iDestFolderId = g_iSrcFoldId;

    dcs_debug(0,0,"<FILE:%s,LINE:%d><SendMessageToFolder>target foldId = %d,size=%d",__FILE__,__LINE__, iDestFolderId, g_folder_msg.datalen);

    //send
    nbytes = g_folder_msg.datalen;
    if ( nbytes < 0 )
    {
        dcs_log( 0,0, "<FILE:%s,LINE:%d><SendMessageToFolder> Failure!",__FILE__,__LINE__);
        strcpy(resp, "96");
        return -1;
    } else if ( nbytes > 0 )
    {
        iRc = fold_write(iDestFolderId, -1, g_folder_msg.data, nbytes);
        if ( iRc < 0 )
        {
            dcs_log(0,0, "<FILE:%s,LINE:%d><SendMessageToFolder>Failure to write msg to folder[%d]!",__FILE__,__LINE__,iDestFolderId);
            strcpy(resp, "96");
            return -1;
        } else
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><SendMessageToFolder>Success to write msg to folder[%d]",__FILE__,__LINE__,iDestFolderId );
            strcpy(resp, "00");
            return 0;
        }
    }
    return 0;
}

//发送给终端
int BC_SendMessageToTerm(ISO_data *iso)
{
    char resp[3];
    char mac_out[32];
    int iRet;
    setbit_t(iso,64,"00000000", 8);
    iRet = isotostr(g_folder_msg.data,iso);
    if ( iRet < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToTerm>isotostr failure!(return:%d)",__FILE__,__LINE__, iRet);
        BC_TRACE_END( -1 );
    }
    g_folder_msg.datalen=iRet;
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToTerm>isotostr success.",__FILE__,__LINE__);

    // 终端加密
    if ( 0 == g_MacMsg.type )
    {
        dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToTerm>终端无需加密.",__FILE__,__LINE__);
        //不加密
    }
    if ( 1 == g_MacMsg.type )
    {
        dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToTerm>开始终端加密调用.",__FILE__,__LINE__);
        //des 加密
        memset(mac_out,0,32);
        iRet = BC_TermGenMac(g_MacMsg.mk_index,g_MacMsg.mk_key,g_folder_msg.data,iRet-8,mac_out);
        if ( 0 == iRet )
        {
            memcpy(g_folder_msg.data+g_folder_msg.datalen-8, mac_out, 8 );
        } else
        {
            //加密失败
        }
    }

    // 发送方向
    g_folder_msg.OutFoldType = 0; //暂时取原floderid
    //strcpy(g_folder_msg.OutFoldName,"TERM");
    //strcpy(g_folder_msg.OutFoldName,"TERM");

    // 发送

    iRet = BC_SendMessageToFolder(resp);
    if ( iRet < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToTerm>BC_SendMessageToFolder failure!(return:%d,resp:%s)",__FILE__,__LINE__, iRet,resp);
        BC_TRACE_END( -1 );
    }

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToTerm>BC_SendMessageToTerm success.",__FILE__,__LINE__);
    return 0;
}


//发送
int BC_SendMessage(ISO_data *iso,char *trans_chnl)
{
    int iRet = -1;
    char resp[3];
    char mac_out[32+1];

    if ( trans_chnl == NULL || *trans_chnl ==0 )
    {
        g_folder_msg.OutFoldType = 0;
    } else
    {
        g_folder_msg.OutFoldType = 1;
        strcpy(g_folder_msg.OutFoldName, trans_chnl);
        rtrim(g_folder_msg.OutFoldName);
    }

    memset(g_folder_msg.data,0 ,sizeof(g_folder_msg.data));
    iRet = isotostr(g_folder_msg.data,iso);
    if ( iRet < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessage>isotostr failure!(return:%d)",__FILE__,__LINE__, iRet);
        BC_TRACE_END( -1 );
    }
    g_folder_msg.datalen=iRet;
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessage>isotostr (return:%d)",__FILE__,__LINE__, iRet);

    // 终端加密
    if ( 0 == g_MacMsg.type )
    {
        //不加密
    }
    if ( 1 == g_MacMsg.type )
    {
        //des 加密
        memset(mac_out,0,32);
        iRet = BC_GenMac(g_MacMsg.mk_index,g_MacMsg.mk_key,g_folder_msg.data,iRet-8,mac_out);
        if ( 0 == iRet )
        {
            memcpy(g_folder_msg.data+g_folder_msg.datalen-8, mac_out, 8 );
        } else
        {
            //加密失败
        }
    }

    // 发送
    iRet = BC_SendMessageToFolder(resp);
    if ( iRet < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToSrc>BC_SendMessageToFolder failure!(return:%d,resp:%s)",__FILE__,__LINE__, iRet,resp);
        BC_TRACE_END( -1 );
    }
    return 0;
}

int BC_SendMessageToSrc(ISO_data *iso)
{
    g_folder_msg.OutFoldType = 0; //发送给源端
    return BC_SendMessage(iso, "");
}

int BC_TermCheckMac(char *key_index,char *key_data, char *Data,int length)
{
    int iRc;
    char mac_out[32],resp_cd[3];

    memset(mac_out, 0, sizeof(mac_out));
    memcpy(mac_out, Data+length-8, 8 );

    dcs_debug(0,0,"<FILE:%s,LINE:%d>start to call BC_TermCheckMac",__FILE__,__LINE__);
    iRc = BC_TermGenMac(key_index,key_data,Data,length-8,mac_out+8);
    if ( iRc >= 0 )
    {
        if ( memcmp(mac_out+8, mac_out,8)==0 )
            return 0;
        else
        {
            dcs_debug(mac_out, 16, "<FILE:%s,LINE:%d>MAC 校验失败，上传交易MAC+本系统MAC.",__FILE__,__LINE__);
            return -1;
        }

    } else
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>call BC_TermCheckMac failure.",__FILE__,__LINE__);
        return -1;
    }
    return 0;
}

int BC_GenMac(char *key_index,char *key_data,char *macData,int length,char *mac_out)
{
    char resp_cd[3];
    char caMac[32+1];
    int nLen,nFillBytes;

    nLen = length;
    memset(caMac, 0, sizeof(caMac));
    if ( nLen >= 1500/8 * 8 )
    {
        dcs_debug(0,0,"<FILE:%s,LINE:%d>enter BC_TermGenMac. --2",__FILE__,__LINE__);
        dcs_log(0,0,"<FILE:%s,LINE:%d>Calculate MACLen Error Len=%d",__FILE__,__LINE__,nLen);
        BC_TRACE_END( -1 );
    }
    if ( nLen % 8 )
    {
        //do filling
        nFillBytes = 8 - ( nLen % 8);
        memset(macData + nLen, 0, nFillBytes);
    } else
        nFillBytes = 0;

    nLen += nFillBytes;

    dcs_debug(0,0,"<FILE:%s,LINE:%d>start to call DESMACOP().",__FILE__,__LINE__);
    DESMACOP(resp_cd,caMac,key_index, key_data, nLen, macData);
    dcs_debug(0,0,"<FILE:%s,LINE:%d>end call DESMACOP.",__FILE__,__LINE__);

    if ( memcmp(resp_cd, "00", 2) != 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_TermGenMac>DESMACOP() failed,rtnCode=%s",__FILE__,__LINE__,resp_cd);
        BC_TRACE_END( -1 );
    }
    asc_to_bcd((unsigned char *) mac_out, (unsigned char *) caMac, 16,0);
    dcs_log(mac_out,8,"<FILE:%s,LINE:%d><BC_TermGenMac>DESMACOP()[%s]",__FILE__,__LINE__, caMac);
    return 0;
}

int BC_TermGenMac(char *key_index,char *key_data,char *macData,int length,char *mac_out)
{
    return BC_GenMac(key_index,key_data,macData,length,mac_out);
}

//设置接收方密钥索引及密钥
int BC_SetMacKey(char *key_index,char *key_data)
{
    if ( key_index!=0 )
    {
        memset(&g_MacMsg, 0, sizeof(BcMacMsg));
        g_MacMsg.type=1; // des 加密
        memcpy(g_MacMsg.mk_index,key_index,3);
        memcpy(g_MacMsg.mk_key,key_data,16);
        g_MacMsg.PFUNC_GEN_KEY = &BC_TermGenMac;
    } else
    {
        //memset(g_MacMsg, 0, sizeof(BcMacMsg));
        g_MacMsg.type=0; //  不加密
    }
}

int BC_SetRcvMacKey(ti_bc_ins_inf_def *p_tiBcInsInf)
{
    if ( '0' == *p_tiBcInsInf->key_type )
    {
        return BC_SetMacKey(0,0);
    } else if ( '1' == *p_tiBcInsInf->key_type )
    {
        return BC_SetMacKey(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1);
    }
    return -1;
}

//设置终端密钥索引及密钥
int BC_SetTermMacKey(char *key_index,char *key_data)
{
    return BC_SetMacKey(key_index,key_data);
}


//设置终端密钥索引及密钥
int BC_SetTermMacKey2(char * term_id)
{
    int sqlCode;
    ti_bc_term_inf_def *p_tiBcTermInf= &g_tiBcTermInf;

    //search term info
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SetTermMacKey2>start to call tiBcTermInfOpr for select.",__FILE__,__LINE__);
    memset( p_tiBcTermInf, 0, sizeof(ti_bc_term_inf_def) );
    strcpy(p_tiBcTermInf->term_id ,term_id);
    tiBcTermInfOpr(GLB_DB_SELECT1, p_tiBcTermInf, &sqlCode);

    if ( 0 != sqlCode )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SetTermMacKey2>tiIseTermInfOpr search term info failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
        return BC_SetMacKey(0,0);;
    }
    return BC_SetMacKey(p_tiBcTermInf->mkey_index,p_tiBcTermInf->mkey_1);
}

//设置运营方密钥索引及密钥
int BC_SetOPMacKey(char *key_index,char *key_data)
{
    return BC_SetMacKey(key_index,key_data);
}


int BC_ErrLog(int iErrCode, char *pcaDispMsg,char cDumpDev,char *pcaDumpAddr,long lDumpLen)
{
    if ( iErrCode>=1000 )
    {
        dcs_log(pcaDumpAddr, lDumpLen, pcaDispMsg);
    } else
    {
        dcs_debug(pcaDumpAddr, lDumpLen, pcaDispMsg);
    }
}

int BC_SendExeception(char *level //an2
                      , char *log, ...)
{
    int len;
    int iRet;
    char buff[64];
    va_list ap;

    //format:
    // head : [4][3]
    // char level  [2];
    // char datetime[10];
    // LLLVar [3][len]

    va_start(ap, log);
    vsnprintf(g_Moni_Data.data+15, sizeof(g_Moni_Data.data)-15,log,ap);
    va_end(ap);

    //data
    len = strlen(g_Moni_Data.data+15);
    sprintf(buff, "%.3d",  len);
    memcpy(g_Moni_Data.data+12,buff,3);

    //level
    sprintf(buff,"%.2s",  level);
    memcpy(g_Moni_Data.data, buff, 2);

    //datetime
    glb_GetLocalTimeString(buff,11,"%m%d%H%M%S");
    memcpy(g_Moni_Data.data+2, buff, 10);

    //bagtype -- 
    memcpy(g_Moni_Data.bagtype,"SYS",3);

    //baglen
    sprintf(buff,"%.4d",  18+len );
    memcpy(g_Moni_Data.baglen, buff, 4);

    int qid;
    if ( (qid=queue_connect("Monitor"))<0 )
        if ( (qid=queue_create("Monitor"))<0 )
        {
            dcs_log(0,0, "<FILE:%s,LINE:%d><BC_SendMonitor>create queue fail[Monitor]!",__FILE__,__LINE__ );
            return -1;
        }

        // Write Data To Folder
    iRet = queue_send(qid, (char *)&g_Moni_Data, len+22,1);
    if ( iRet < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BC_SendMonitor>Error Write Monitor queue[Monitor]!",__FILE__,__LINE__ );

        return -3;
    }
    return 0;
}
int min_strlen(char *src, int len)
{
    int l=0;
    while ( (l<len) && *(src+l)!=0 ) l++;
    return l;
}

int min_strstr(char *str1, char *str2)
{
    int l=0;
    while ( 0 == *(str1+l) || 0 == *(str2+l) ) l++;
    return l;
}

#define MEMCPY_THIS(DES,SRC,LEN) memcpy(DES,SRC,min_strlen(SRC,LEN));DES+=LEN;
int BC_SendMonitor_trans(char *transflag,char *sendflag,tl_bc_trans_log_def *p_BcTransLog, ti_bc_trans_inf_def *p_tiBcTransInf)
{
    //return BC_SendMonitor22(transflag,sendflag,p_BcTransLog);
    int len;
    int iRet;
    char buff[256];
    va_list ap;

    char *ptr = (char *)g_Moni_Data.data ;
    memset(g_Moni_Data.data,' ',sizeof(g_Moni_Data.data));
    *(g_Moni_Data.data + sizeof(g_Moni_Data.data) -1) = 0;

    //value
    MEMCPY_THIS(ptr,transflag,1);
    MEMCPY_THIS(ptr,sendflag,1);
    MEMCPY_THIS(ptr, p_BcTransLog->trans_chnl,8);
    MEMCPY_THIS(ptr, p_BcTransLog->mchnt_cd, 15);
    MEMCPY_THIS(ptr, p_BcTransLog->term_id, 8);

    //year
    //strcpy(buff,p_BcTransLog->settle_dt);
    //rtrim(buff);
    //if ( *buff == 0 )
    //{
        glb_GetLocalTimeString(buff,5,"%Y");
    //}
    MEMCPY_THIS(ptr, buff, 4);

    //loc_trans_dt
    strcpy(buff,p_BcTransLog->loc_trans_dt);
    rtrim(buff);
    if ( *buff == 0 )
    {
        glb_GetLocalTimeString(buff,5,"%m%d");
    }
    MEMCPY_THIS(ptr, buff, 4);

    //loc_trans_tm
    strcpy(buff,p_BcTransLog->loc_trans_tm);
    rtrim(buff);
    if ( *buff == 0 )
    {
        glb_GetLocalTimeString(buff,7,"%H%M%S");
    }
    MEMCPY_THIS(ptr, buff, 6);

    //mchnt_tp
    MEMCPY_THIS(ptr, p_BcTransLog->mchnt_tp, 4);

    //交易类型
    if ( p_tiBcTransInf )
    {
        MEMCPY_THIS(ptr, p_tiBcTransInf->TransType, 4);
    } else
    {
        MEMCPY_THIS(ptr, "    ", 4);
    }

    //账号 -- 变长
    strcpy(buff,p_BcTransLog->pri_acct_no);
    rtrim(buff);
    len = strlen(buff);
    if ( len<=0 )
    {
        MEMCPY_THIS(ptr,"00",2);
    } else
    {
        sprintf(buff+100,"%.2d", len);
        MEMCPY_THIS(ptr, buff+100, 2);
        MEMCPY_THIS(ptr, buff, len);
    }

    //运营商账号 -- 变长
    strcpy(buff+3,p_BcTransLog->fld_44);
    rtrim(buff+3);
    len = strlen(buff+3);
    if ( len<=0 )
    {
        MEMCPY_THIS(ptr,"00",2);
    } else
    {
        sprintf(buff,"%.2d", len);
        MEMCPY_THIS(ptr, buff, 2);
        MEMCPY_THIS(ptr, buff+3, len);
    }

    MEMCPY_THIS(ptr, p_BcTransLog->trans_at, 12);
    MEMCPY_THIS(ptr, p_BcTransLog->sys_tra_no, 6);
    MEMCPY_THIS(ptr, p_BcTransLog->retri_ref_no, 12);

    strcpy(buff, p_BcTransLog->resp_cd);
    rtrim(buff);
    if ( 0!= *buff )
    {
        MEMCPY_THIS(ptr,p_BcTransLog->resp_cd, 2);
    } else
    {
        strcpy(buff, p_BcTransLog->fld_28);
        rtrim(buff);
        MEMCPY_THIS(ptr,(*buff==0)?p_BcTransLog->fld_28:buff+4, 2);
    }    

    //head
    len = ptr-g_Moni_Data.data+3;
    sprintf( buff, "%.4d", len);
    memcpy(g_Moni_Data.bagtype,"LOG",3);
    memcpy(g_Moni_Data.baglen,buff,4);

    int qid;
    if ( (qid=queue_connect("monitor"))<0 )
        if ( (qid=queue_create("monitor"))<0 )
        {
            dcs_log(0,0, "<FILE:%s,LINE:%d><BC_SendMonitor>create queue fail[Monitor]!",__FILE__,__LINE__ );
            return -1;
        }

        // Write Data To Folder
    dcs_debug(&g_Moni_Data, len+4, "<FILE:%s,LINE:%d><BC_SendMonitor>start to call queue_send[Monitor].",__FILE__,__LINE__ );
    iRet = queue_send(qid, (char *)&g_Moni_Data, len+4,1);
    if ( iRet < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BC_SendMonitor>Error Write Monitor queue[Monitor]!",__FILE__,__LINE__ );

        return -3;
    }
    return 0;
}

int BC_SendMonitor_iso(char *transflag,char *sendflag,char *trans_chnl,ISO_data *iso, ti_bc_trans_inf_def *p_tiBcTransInf)
{
    int len;
    int iRet;
    char buff[64];
    va_list ap;

    char *ptr = (char *)g_Moni_Data.data ;
    memset(g_Moni_Data.data,' ',sizeof(g_Moni_Data.data));
    *(g_Moni_Data.data + sizeof(g_Moni_Data.data) -1) = 0;

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BC_SendMonitor>enter.",__FILE__,__LINE__ );

    //value
    MEMCPY_THIS(ptr,transflag,1);
    MEMCPY_THIS(ptr,sendflag,1);
    MEMCPY_THIS(ptr,trans_chnl,8);

    len = getbit(iso, 42, buff);
    MEMCPY_THIS(ptr, (len<=0?"":buff), 15);

    len = getbit(iso, 41, buff);
    MEMCPY_THIS(ptr, (len<=0?"":buff), 8);

    //year
    len = getbit(iso, 15, buff);
    if ( len<=0 )
    {
        glb_GetLocalTimeString(buff,9,"%Y");
    }
    MEMCPY_THIS(ptr, buff, 4);

    //loc_trans_dt
    len = getbit(iso, 13, buff);
    if ( len<=0 )
    {
        glb_GetLocalTimeString(buff,5,"%m%d");
    }
    MEMCPY_THIS(ptr, buff, 4);

    //loc_trans_tm
    len = getbit(iso, 12, buff);
    if ( len <=0 )
    {
        glb_GetLocalTimeString(buff,7,"%H%M%S");
    }
    MEMCPY_THIS(ptr, buff, 6);

    //mchnt_tp
    len = getbit(iso, 18, buff);
    MEMCPY_THIS(ptr, len<=0?"":buff, 4);

    //交易类型
    //MEMCPY_THIS(ptr, "   ", 4);
    if ( p_tiBcTransInf )
    {
        MEMCPY_THIS(ptr, p_tiBcTransInf->TransType, 4);
    } else
    {
        MEMCPY_THIS(ptr, "   ", 4);
    }

    //账号 -- 变长
    len = getbit(iso, 2, buff+3);
    if ( len<=0 )
    {
        MEMCPY_THIS(ptr,"00",2);
    } else
    {
        sprintf(buff,"%.2d", len);
        MEMCPY_THIS(ptr, buff, 2);
        MEMCPY_THIS(ptr, buff+3, len);
    }

    //运营商账号 -- 变长
    len = getbit(iso, 44, buff);
    if ( len<=0 )
    {
        MEMCPY_THIS(ptr,"00",2);
    } else
    {
        sprintf(buff,"%.2d", len);
        MEMCPY_THIS(ptr, buff, 2);
        MEMCPY_THIS(ptr, buff+3, len);
    }

    len = getbit(iso, 4, buff);
    MEMCPY_THIS(ptr, len<=0?"":buff, 12);

    len = getbit(iso, 11, buff);
    MEMCPY_THIS(ptr, len<=0?"":buff, 6);

    len = getbit(iso, 37, buff);
    MEMCPY_THIS(ptr, len<=0?"":buff, 12);

    len = getbit(iso, 39, buff);
    if ( len>0 )
    {
        MEMCPY_THIS(ptr, buff, 2);
    } else
    {
        len = getbit(iso, 28, buff);
        MEMCPY_THIS(ptr, len<=0?"":buff+4, 2);
    }

    //head
    len = ptr-g_Moni_Data.data+3;
    sprintf( buff, "%.4d", len );
    memcpy(g_Moni_Data.bagtype,"LOG",3);
    memcpy(g_Moni_Data.baglen,buff,4);

    dcs_debug(0,0, "<FILE:%s,LINE:%d><BC_SendMonitor>start to call queue_connect[Monitor].",__FILE__,__LINE__ );

    int qid;
    if ( (qid=queue_connect("monitor"))<0 )
        if ( (qid=queue_create("monitor"))<0 )
        {
            dcs_log(0,0, "<FILE:%s,LINE:%d><BC_SendMonitor>create queue fail[Monitor]!",__FILE__,__LINE__ );
            return -1;
        }

        // Write Data To Folder
    dcs_debug(&g_Moni_Data, len+4, "<FILE:%s,LINE:%d><BC_SendMonitor>start to call queue_send[Monitor].",__FILE__,__LINE__ );
    iRet = queue_send(qid, (char *)&g_Moni_Data, len+4,1);
    if ( iRet < 0 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BC_SendMonitor>Error Write Monitor queue[Monitor]!",__FILE__,__LINE__ );

        return -3;
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BC_SendMonitor>out.",__FILE__,__LINE__ );
    return 0;
}

int BC_SendMonitor(char *transflag,char *sendflag,char *trans_chnl,ISO_data *iso)
{
    return BC_SendMonitor_iso(transflag,sendflag,trans_chnl,iso,0);
}

int BC_SendMonitor2(char *transflag,char *sendflag,tl_bc_trans_log_def *p_BcTransLog)
{
    return BC_SendMonitor_trans(transflag,sendflag,p_BcTransLog,0);
}



/*向安捷平台发送信息，并等待接收返回信息*/
int TPSendToAJ(char *pSendData,int iMsgLen,char *ip,int port,int timeout,char *pRecvData)
{
    int     iConnSock,iRc;

    dcs_debug(0, 0,"<FILE:%s,LINE:%d><TPSendToAJ>start to connnect ip:[%s] port[%d]!",__FILE__,__LINE__,ip,port);
    iConnSock = tcp_connet_server(ip,port,0);
    if ( iConnSock < 0 )
    {
        dcs_log(0, 0,"<FILE:%s,LINE:%d><TPSendToAJ>connnect to ip:[%s] port[%d] failure! ",__FILE__,__LINE__,ip,port);
        return -1;
    }
    dcs_debug(pSendData, iMsgLen, "<FILE:%s,LINE:%d><TPSendToAJ>准备发送安捷平台数据! ip:[%s] port[%d] ! ",__FILE__,__LINE__,ip,port);
    /*发送信息*/
    iRc = write_msg_to_net(iConnSock, pSendData, iMsgLen,timeout*1000);
    if ( iRc < 0 )
    {
        dcs_log(0, 0,"<FILE:%s,LINE:%d><TPSendToAJ>向安捷平台发送数据失败! ip:[%s] port[%d] failure! ",__FILE__,__LINE__,ip,port);
        tcp_close_socket(iConnSock);
        return iRc;
    }

    dcs_debug(0, 0,"<FILE:%s,LINE:%d><TPSendToAJ>向安捷平台发送数据成功! ip:[%s] port[%d] ! ",__FILE__,__LINE__,ip,port);

    /*接收信息*/
    iRc = read_msg_from_net(iConnSock, pRecvData,1024,timeout*1000);
    if ( iRc < 0 )
    {
        dcs_log(0, 0,"<FILE:%s,LINE:%d><TPSendToAJ>接收安捷平台数据失败! ip:[%s] port[%d] failure! ",__FILE__,__LINE__,ip,port);
        tcp_close_socket(iConnSock);
        return iRc;         
    }
    dcs_debug(pRecvData, iRc, "<FILE:%s,LINE:%d><TPSendToAJ>接收安捷平台数据成功（len=%d）! ip:[%s] port[%d] ! ",__FILE__,__LINE__,iRc,ip,port);
    tcp_close_socket(iConnSock);
    return iRc;
}

int BC_TPSendToAJ(char *pSendData,int iMsgLen,char *pRecvData,ti_bc_ins_inf_def *p_tiBcInsInf)
{
    int port = atoi(p_tiBcInsInf->port);
    return TPSendToAJ(pSendData,iMsgLen,p_tiBcInsInf->ip_addr, port,30,pRecvData);
}

//发送给安捷
int BC_SendMessageToAJ(ISO_data *iso, char *pRecvData,ti_bc_ins_inf_def *p_tiBcInsInf)
{
    char resp[3];
    char mac_out[32];
    int iRet;

    memset(g_folder_msg.data,0,sizeof(g_folder_msg.data));
    iRet = isotostr(g_folder_msg.data,iso);
    if ( iRet < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>isotostr failure!(return:%d)",__FILE__,__LINE__, iRet);
        BC_TRACE_END( -1 );
    }
    g_folder_msg.datalen=iRet;
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>isotostr success.",__FILE__,__LINE__);

    // 终端加密
    if ( 0 == g_MacMsg.type )
    {
        dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>无需加密.",__FILE__,__LINE__);
        //不加密
    }
    if ( 1 == g_MacMsg.type )
    {
        dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>开始加密调用.",__FILE__,__LINE__);

        //des 加密
        memset(mac_out,0,32);
        iRet = BC_TermGenMac(g_MacMsg.mk_index,g_MacMsg.mk_key,g_folder_msg.data,iRet-8,mac_out);
        if ( 0 == iRet )
        {
            dcs_debug(mac_out, 8,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>加密调用成功.",__FILE__,__LINE__);
            memcpy(g_folder_msg.data+g_folder_msg.datalen-8, mac_out, 8 );
        } else
        {
            //加密失败
            dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>加密调用失败.",__FILE__,__LINE__);
            return -1;
        }
    }

    // 发送
    iRet = BC_TPSendToAJ(g_folder_msg.data,g_folder_msg.datalen, pRecvData,p_tiBcInsInf);
    if ( iRet < 0 )
    {
        dcs_log(p_tiBcInsInf,sizeof(ti_bc_ins_inf_def),"<FILE:%s,LINE:%d><BC_SendMessageToAJ>BC_TPSendToAJ(%s) failure!(return:%d)",__FILE__,__LINE__,p_tiBcInsInf->ins_id_cd,iRet);
        BC_TRACE_END( -1 );
    }

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>BC_TPSendToAJ success.(%s)",__FILE__,__LINE__,p_tiBcInsInf->ins_id_cd);
    return iRet;
}


int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
    iconv_t cd;
    int rc;
    char **pin = &inbuf;
    char **pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);
    int len_out = outlen;
    if ( cd==0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><code_convert>call iconv_open return failure.",__FILE__,__LINE__);
        return -1;
    }
    memset(outbuf,0,outlen);
    if ( (rc=iconv(cd,pin,(size_t*)&inlen,pout,(size_t*)&len_out))==-1 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><code_convert>call iconv return failure.",__FILE__,__LINE__);
        return -1;
    }
    iconv_close(cd);
    //dcs_log(pout,outlen,"<FILE:%s,LINE:%d><code_convert>success(outbuff:%d,*pout:%d,inlen:%d,len_out:%d).",__FILE__,__LINE__,&outbuf,*pout,inlen,len_out);
    return outlen-len_out;
}

//UNICODE码转为GB2312码
int UnicodeToGBK(char *inbuf,int inlen,char *outbuf,int outlen)
{
    return code_convert("UNICODE","GBK",inbuf,inlen,outbuf,outlen);
}

//GB2312码转为UNICODE码
int GBKToUnicode(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{//"ISO8859-2"
    return code_convert("GBK","UNICODE",inbuf,inlen,outbuf,outlen);
}

int UnicodeToGBK_2(char *inbuf,int inlen,char *outbuf,int outlen)
{
    int i=0 ;
    char buff[4096] ;
    for ( i=0; i< inlen/4 && i*4+3 < 4096; i++ )
    {
        buff[i*2] = ((inbuf[i*4+2]<<4)&0xf0) | (inbuf[i*4+3]&0xf);
        buff[i*2+1] = ((inbuf[i*4]<<4)&0xf0) | (inbuf[i*4+1]&0xf);
    }
    return code_convert("UNICODE","GBK",inbuf,inlen,outbuf,MIN(i*2,outlen));
}

int GBKToUnicode_2(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    char buff[4096+1];
    int iret = code_convert("GBK","UNICODE",inbuf,inlen,outbuf,outlen);
    if ( iret >0 )
    {
        int i=0;
        char *index="0123456789ABCDEF";
        for ( i=0; i<iret/2 && i*4+3 <4096 && i*2+1 < outlen; i++ )
        {
            buff[i*4] = index[((unsigned char)outbuf[i*2+1]>>4)&0xf];
            buff[i*4+1] = index[outbuf[i*2+1]&0xf];
            buff[i*4+2] = index[((unsigned char)outbuf[i*2]>>4)&0xf];
            buff[i*4+3] = index[outbuf[i*2]&0xf];
        }
        dcs_debug(buff, i*4, "<FILE:%s,LINE:%d>bcd_to_asc.(iLen:%d)",__FILE__,__LINE__, i*4);
        memcpy(outbuf, buff+4, i<=0?0:i*4-4);
        return i*4-4;
    }
    return iret;
}

int UTF8ToUnicode_2(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    char buff[4096+1];
    int iret = code_convert("UTF-8","UNICODE",inbuf,inlen,outbuf,outlen);
    /*
    if( 0 == memcmp(inbuf,"**",2) ){
        iret = code_convert("GBK","UNICODE",inbuf,inlen,outbuf,outlen);
    }else{
        iret = code_convert("UTF-8","UNICODE",inbuf,inlen,outbuf,outlen);
    }*/
    if ( iret >0 )
    {
        int i=0;
        char *index="0123456789ABCDEF";
        for ( i=0; i<iret/2 && i*4+3 <4096 && i*2+1 < outlen; i++ )
        {
            buff[i*4] = index[((unsigned char)outbuf[i*2+1]>>4)&0xf];
            buff[i*4+1] = index[outbuf[i*2+1]&0xf];
            buff[i*4+2] = index[((unsigned char)outbuf[i*2]>>4)&0xf];
            buff[i*4+3] = index[outbuf[i*2]&0xf];
        }
        dcs_debug(buff, i*4, "<FILE:%s,LINE:%d>bcd_to_asc.(iLen:%d)",__FILE__,__LINE__, i*4);
        memcpy(outbuf, buff+4, i<=0?0:i*4-4);
        return i*4-4;
    }
    return iret;
}

// 将当前字符集转换为UTF8
int ToUTF8(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    if ( 0 == memcmp(inbuf,"**",2) )
    {
        return code_convert("GBK","UTF-8",inbuf,inlen,outbuf,outlen);
    }
    return 0;
}

int BC_Fork_Enter( )
{
    g_ForkFlag = 0;

    return -1;
}

int BC_Fork_Exit(int iExitCode)
{
    if ( g_ForkFlag )
    {
        int sqlCode;
        ti_bc_app_fork_inf_def tiBcAppForkInf ;
        memset(&tiBcAppForkInf,0, sizeof(ti_bc_app_fork_inf_def));
        tiBcAppForkInf.app_id = 1000;
        tiBcAppForkInfOpr(GLB_DB_UPDATE3, &tiBcAppForkInf, &sqlCode);
        if ( 0!= sqlCode )
        {
            dcs_debug(0, 0, "<FILE:%s,LINE:%d>更新fork进程数目-1失败(当前数目:%d),注意可能造成fork计数不准确.",__FILE__,__LINE__,tiBcAppForkInf.fork_num);
            BC_SendExeception("02","警告：更新fork进程数目-1失败(当前数目:%d),注意可能造成fork计数不准确.",tiBcAppForkInf.fork_num);
        }
        dcs_debug(0,0,"<FILE:%s,LINE:%d>AppServer Auto Fork Process terminated.\n",__FILE__,__LINE__);

        DasEnd( COMMIT );
        exit(iExitCode);
    }
}


//GB2312码转为UTF-8码
int GBKToUTF8(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{//"ISO8859-2"
    return code_convert("GBK","UTF-8",inbuf,inlen,outbuf,outlen);
}

