#include "ibdcs.h"
#include "tmcibtms.h"
#include "dccdcs.h"
#include "bc.h"
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

char *g_pcBcdaShmPtr;
int g_iSrcFoldId = -1;
char g_iSrcFoldName[30+1] ;
char g_srcPacketType[5];
char g_Rcv_Buffer[4096] ;
int g_RcvBufferSize ;
int g_FromFolderId = -1;
int g_ForkFlag = 0;
char g_rtIns_id_cd[9] ;
int g_TransLock[2] ={'0','\0'};
char *aj_mchnt_cd;

struct ISO_8583 iso8583_ZDXTconf[128];
struct ISO_8583 iso8583_AJXTconf[128];
struct ISO_8583 iso8583_GDYLconf[128];
struct ISO_8583 iso8583_JAVAconf[128];
struct ISO_8583 iso8583_CUPSconf[128];

//超时表结构
struct TimeOut_REC g_TimeOutRec;
MONI_STRUCT g_Moni_Data;

// 交易类型信息
ti_bc_trans_inf_def g_tiBcTransInf;

// 交易类型决定信息
ti_bc_trans_det_def g_tiBcTransDet;

// 终端信息表
ti_bc_term_inf_def g_tiBcTermInf;

// 商户决定信息
ti_bc_mchnt_det_def g_tiBcMchntDet;

// 商户信息
ti_bc_mchnt_inf_def g_tiBcMchntInf;

// 清算商户信息
ti_bc_qs_mchnt_inf_def g_tiBcQsMchntInf;

// 机构信息
ti_bc_ins_inf_def g_tiBcInsInf;

//账单信息表
tl_bc_bill_log_def g_tlBcBillLog;

//商户转换信息表
ti_bc_mchnt_term_charge_inf_def g_tiBcMchntTermChargeInf;
ti_bc_route_card_bin_def g_tiBcRouteCardBinList ;
ti_bc_white_list_def g_tiBcWhiteList ;
ti_bc_black_list_def g_tiBcBlackList ;
ti_bc_mid_day_limit_tims_def g_tiBcMidDayLimitTims;
ti_bc_card_bin_def g_tiBcCardBin;
ti_bc_mid_day_limit_tims_def g_tiBcMidDayLimitTims ;

Folder_Msg g_folder_msg;
BcTransMsg g_TransMsg;
BcTransMsg g_TransMsg_o; //orig 最新接收到的原始交易信息
BcMacMsg g_MacMsg;

void PrintISO( ISO_data *iso,char *caRemark,int flag)
{
    int i,s,j;
    char buffer[8192],tmpbuf[400],tmpbuf1[1024],*p;
    memset(buffer,0,sizeof(buffer));
    p=&buffer[0];
    if ( caRemark !=NULL )
    {
        sprintf(p,"%s\n",caRemark);
        p=p+strlen(caRemark)+1;
    }
    for ( i =0; i<=128 ;i++ )
    {
        s=getbit(iso,i,(unsigned char *)tmpbuf);
        if ( s >0 )
        {
            char *index="0123456789ABCDEF";
            if ( i== 64 || i==128 )
            {
                for ( j=s; j>=0; j-- )
                {
                    char c=tmpbuf[j];
                    tmpbuf[2*j] = index[(c>>4)&0xf] ;
                    tmpbuf[2*j+1] = index[c&0xf] ;
                }
                s*=2;
            }
            tmpbuf[s]=0;
            if ( (p+s-&buffer[0]) >8192 ) return;
            sprintf(tmpbuf1,"第[%03d]域 =[%03d][%s]\n",i,s,tmpbuf);
            strcpy(p,tmpbuf1);
            p=p+strlen(tmpbuf1);
        }
    }
    dcs_debug(0,0,buffer);
    return;
}

/*
int TermMsgUnpack( char * srcBuf, ISO_data *iso, int iLen)
{
    dcs_debug(0, 0, "<FILE:%s,LINE:%d>into TermMsgUnpack",__FILE__,__LINE__);

    memset(iso,0, sizeof(ISO_data));
    if ( iLen < 20 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d>public system Msg error",__FILE__,__LINE__);
        return -1;
    }
    iso->iso8583=&iso8583_TERMconf[0];    
    SetIsoHeardFlag(iso,1);       // 设置报文类型内容为不压缩方式 
    SetIsoFieldLengthFlag(iso,1); // 设置可变长报文域长度为不压缩方式 
    if ( strtoiso((unsigned char *)srcBuf,iso,iLen)<0 )
    {
        dcs_log(srcBuf, iLen,"<FILE:%s,LINE:%d>cant not analyse got message(%d bytes) from ter-->",__FILE__,__LINE__, iLen);
        return -1;            
    }
    return 1;
}
*/
//srcBuf 前4位为报文类型，解包后的数据存放在srcBuf+4的位置
int BC_UnPack( ISO_data *iso, char *srcBuf,  int iLen)
{
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>into BC_UnPack",__FILE__,__LINE__);
    memset(iso,0, sizeof(ISO_data));
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>start to call BC_SetIsoMode2!",__FILE__,__LINE__);
    if ( 0!= BC_SetIsoMode(iso, srcBuf) )
    {
        dcs_log(srcBuf, iLen, "<FILE:%s,LINE:%d><BC_UnPack>BC_SetIsoMode failure",__FILE__,__LINE__);
        return -1;
    }
    if ( iLen < 20 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BC_UnPack>public system Msg error",__FILE__,__LINE__);
        return -1;
    }
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>start to call strtoiso!",__FILE__,__LINE__);
    if ( strtoiso((unsigned char *)srcBuf+4, iso, iLen-4)<0 )
    {
        dcs_log(srcBuf, iLen,"<FILE:%s,LINE:%d><BC_UnPack>cant not analyse got message(%d bytes) from ter-->",__FILE__,__LINE__, iLen);
        return -1;            
    }
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>BC_UnPack complated!",__FILE__,__LINE__);
    return 1;
}

//srcBuf 前4位无报文类型，解包后的数据存放在srcBuf的位置
int BC_UnPack_Data( ISO_data *iso, char *srcBuf,  int iLen)
{
    if ( iLen < 20 )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d><BC_UnPack_Data>public system Msg error",__FILE__,__LINE__);
        return -1;
    }
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack_Data>start to call strtoiso!",__FILE__,__LINE__);
    if ( strtoiso((unsigned char *)srcBuf,iso,iLen)<0 )
    {
        dcs_log(srcBuf, iLen,"<FILE:%s,LINE:%d><BC_UnPack_Data>cant not analyse got message(%d bytes) from ter-->",__FILE__,__LINE__, iLen);
        return -1;            
    }
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BC_UnPack>BC_UnPack_Data complated!",__FILE__,__LINE__);
    return 1;
}

int BC_SetIsoMode(ISO_data *iso, char *mode_type)
{
	char mode[5];
	memset(mode, 0, sizeof(mode));
	memcpy(mode, mode_type, 4);
	
    if ( memcmp( mode,"GDYL",4) == 0 )  /*运营商报文*/
    {
        iso->iso8583=&iso8583_GDYLconf[0];
        iso->headlen=11; 
        iso->head_flag=1; 
		SetIsoHeardFlag(iso,0);   /** 设置报文类型内容为压缩方式 **/
        SetIsoFieldLengthFlag(iso,0); /** 设置可变长报文域长度为压缩方式 **/
    }else if ( memcmp( mode,"ZDXT",4) == 0 )  /*运营商报文*/
    {
        iso->iso8583=&iso8583_ZDXTconf[0];
        SetIsoHeardFlag(iso,1);   /** 设置报文类型内容为不压缩方式 **/
        SetIsoFieldLengthFlag(iso,1); /** 设置可变长报文域长度为不压缩方式 **/
    }else if ( memcmp( mode,"HFTX",4) == 0 )  /*运营商报文*/
    {
        iso->iso8583=&iso8583_ZDXTconf[0];
        SetIsoHeardFlag(iso,1);   /** 设置报文类型内容为不压缩方式 **/
        SetIsoFieldLengthFlag(iso,1); /** 设置可变长报文域长度为不压缩方式 **/
    }else if ( memcmp( mode,"AJXT",4) == 0 )  /*安捷报文*/
    {
        iso->iso8583=&iso8583_AJXTconf[0];
        SetIsoHeardFlag(iso,1);   /** 设置报文类型内容为不压缩方式 **/
        SetIsoFieldLengthFlag(iso,1); /** 设置可变长报文域长度为不压缩方式 **/
    }else if ( memcmp( mode,"JAVA",4) == 0 )  /*JAVA前置*/
    {
        iso->iso8583=&iso8583_JAVAconf[0];
        SetIsoHeardFlag(iso,1);   /** 设置报文类型内容为不压缩方式 **/
        SetIsoFieldLengthFlag(iso,1); /** 设置可变长报文域长度为不压缩方式 **/
    
    }else if ( memcmp( mode,"CUPS",4) == 0 )  /*POSP前置*/
    {
        iso->iso8583=&iso8583_CUPSconf[0];
        SetIsoHeardFlag(iso, 1);   /** 设置报文类型内容为不压缩方式 **/
        SetIsoFieldLengthFlag(iso, 1); /** 设置可变长报文域长度为不压缩方式 **/
    } else
    {
        /*dcs_debug(mode,iRead,"Unkown Packet,discard !");*/
        dcs_log(0,0, "<FILE:%s,LINE:%d>Unkown ISO packet mode(%s) !",__FILE__,__LINE__, mode);
        return -1;
    }
    dcs_log(0,0, "<FILE:%s,LINE:%d><BC_SetIsoMode>SetIsoMode(%s) !",__FILE__,__LINE__, mode);
    return 0;
}

int BC_GetPacket_Type(char *packet_type, char *trans_chnl)
{
    int sqlCode ;
    int iRet = -1;
    ti_bc_trans_chnl_inf_def ti;

    memset(&ti,0,sizeof(ti_bc_trans_chnl_inf_def));
    strcpy(ti.trans_chnl, trans_chnl);

    iRet = tiBcTransChnlInfOpr(GLB_DB_SELECT1, &ti, &sqlCode);
    if ( 0 != sqlCode ) return -1;
    strcpy(packet_type, ti.packety_type);
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
        dcs_debug(0,0,"<FILE:%s,LINE:%d>LocateBankByName invalid pcaName =%s",__FILE__,__LINE__,pcaName);
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
		if ((!memcmp(g_folder_msg.OutFoldName, "HFTX", 4)) ||
		    (!memcmp(g_folder_msg.OutFoldName, "GDYL", 4))) 
		{
	        	if ( IsReady( g_folder_msg.OutFoldName )<0 )
	        	{
	            		dcs_log(0,0,"<FILE:%s,LINE:%d><SendMessageToFolder> the foldName [%s] isn't online ",__FILE__,__LINE__,g_folder_msg.OutFoldName );
	            		strcpy(resp, "91");
	            		return -1;
	        	}
		}

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
    BC_SetIsoMode(iso,"TERM");
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
    iRet = isotostr(g_folder_msg.data, iso);
    if ( iRet < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessage>isotostr failure!(return:%d)",__FILE__,__LINE__, iRet);
        BC_TRACE_END( -1 );
    }
    g_folder_msg.datalen = iRet;
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BC_SendMessage>isotostr (return:%d)",__FILE__,__LINE__, iRet);

    PrintISO( iso,"发送报文",0);  

    // 终端加密
    if ( 0 == g_MacMsg.type )
    {
        //不加密
    }
    if ( 1 == g_MacMsg.type )
    {
        //des 加密
        memset(mac_out,0,32);
        iRet = BC_GenMac(g_MacMsg.mk_index,g_MacMsg.mk_key,g_folder_msg.data+iso->headlen,iRet-8-iso->headlen,mac_out);
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

/*
int BC_SendMessageToSrc2(ISO_data *iso,char *trans_chnl)
{
    g_folder_msg.OutFoldType = 0; //发送给源端
    //BC_SetMacKey(0,0); //暂时不加密
    return BC_SendMessage(iso, trans_chnl);
}
*/

int BC_CheckIsoDataField(ISO_data *p_isoData, char *field)
{
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BC_CheckIsoDataField> enter  BC_CheckIsoDataField para1:%d,para2:%s ! ",__FILE__,__LINE__, p_isoData,field);

    int value;
    char buff[1024]={0};
    char *p=NULL;
    if ( field != NULL )
        strcpy(buff,field);
    p =strtok(buff,",\0");
    while ( NULL != p )
    {
        value = atoi(p);
        if ( 0 == isbitset(p_isoData,value) )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BC_CheckIsoDataField> field #%d not set! ",__FILE__,__LINE__, value);
            return -1*value;
        }
        p = strtok(NULL,",");
    }
    return 0;   
}

/* read ISO bag to struct  */
int BC_CopyISOdata( ISO_data *p_isoData, BcTransMsg *p_BcTransMsg)
{
    char buff[4096];
    int len;

    memset(p_BcTransMsg, 0, sizeof(BcTransMsg));

    if ( (len = getbit(p_isoData,  0, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.msg_tp            , buff, MIN(len,  4));
    }
    if ( (len = getbit(p_isoData,  2, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.pri_acct_no       , buff, MIN(len, 19));
    }
    if ( (len = getbit(p_isoData,  3, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.proc_cd           , buff, MIN(len,  6));
    }
    if ( (len = getbit(p_isoData,  4, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.trans_at          , buff, MIN(len, 12));
    }
    if ( (len = getbit(p_isoData,  5, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.fld_5             , buff, MIN(len, 12));
    }
    if ( (len = getbit(p_isoData,  7, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.transmsn_dt_tm    , buff, MIN(len, 10));
    }
    if ( (len = getbit(p_isoData,  8, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.fld_8             , buff, MIN(len, 8 ));
    }
    if ( (len = getbit(p_isoData, 11, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.sys_tra_no        , buff, MIN(len,  6));
        memcpy(p_BcTransMsg->BcTransLog.center_sys_tra_no , buff, MIN(len,  6));
    }
    if ( (len = getbit(p_isoData, 12, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.loc_trans_tm      , buff, MIN(len,  6));
    }
    if ( (len = getbit(p_isoData, 13, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.loc_trans_dt      , buff, MIN(len,  4));
    } else
    {
        //如果没有#13，则默认设置为当前日期
        glb_GetLocalTimeString(p_BcTransMsg->BcTransLog.loc_trans_dt,5,"%m%d");
    }
    if ( (len = getbit(p_isoData, 15, buff)) > 0 )
    {
        //UnionGetSystemYear(p_BcTransMsg->BcTransLog.settle_dt);
        //glb_GetLocalTimeString(p_BcTransMsg->BcTransLog.settle_dt,4,"%Y");
        memcpy(p_BcTransMsg->BcTransLog.settle_dt, buff, MIN(len, 4)); 
    } else
    {
        //glb_GetLocalTimeString(p_BcTransMsg->BcTransLog.settle_dt,9,"%Y%m%d");
        strcpy(p_BcTransMsg->BcTransLog.settle_dt," ");
    }
    if ( (len = getbit(p_isoData, 18, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.mchnt_tp          , buff, MIN(len,  4));
    }
    if ( (len = getbit(p_isoData, 22, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.pos_entry_md_cd   , buff, MIN(len,  3));
    }
    if ( (len = getbit(p_isoData, 25, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.pos_cond_cd       , buff, MIN(len,  2));
    }
    if ( (len = getbit(p_isoData, 28, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.fld_28            , buff, MIN(len,  6));
    }
    if ( (len = getbit(p_isoData, 32, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.fld32_ins_id_cd   , buff, MIN(len, 11));
    }
    if ( (len = getbit(p_isoData, 33, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.fwd_ins_id_cd     , buff, MIN(len, 11));
    }
    if ( (len = getbit(p_isoData, 35, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.track_2_data   	 , buff, MIN(len, 37));
    }
    if ( (len = getbit(p_isoData, 36, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.track_3_data     , buff, MIN(len, 104));
    }
    if ( (len = getbit(p_isoData, 37, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.retri_ref_no      , buff, MIN(len, 12));
    }
    if ( (len = getbit(p_isoData, 38, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.auth_id_resp_cd   , buff, MIN(len,  6));
    }
    if ( (len = getbit(p_isoData, 39, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.resp_cd           , buff, MIN(len,  2));
    }
    if ( (len = getbit(p_isoData, 41, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.term_id           , buff, MIN(len,  8));
        memcpy(p_BcTransMsg->BcTransLog.center_term_id    , buff, MIN(len,  8));
    }
    if ( (len = getbit(p_isoData, 42, buff)) > 0 )
    {
    	  strcpy_safe(p_BcTransMsg->BcTransLog.mchnt_cd, buff, sizeof(p_BcTransMsg->BcTransLog.mchnt_cd)-1);
    	  strcpy_safe(p_BcTransMsg->BcTransLog.center_mchnt_cd, buff, sizeof(p_BcTransMsg->BcTransLog.center_mchnt_cd)-1);
    }
    if ( (len = getbit(p_isoData, 44, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.fld_44            , buff, MIN(len, 27));
    }
    if ( (len = getbit(p_isoData, 45, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.fld_45            , buff, MIN(len, 76));
    }
    if ( (len = getbit(p_isoData, 46, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.fld_46            , buff, MIN(len, 46));
    }
    if ( (len = getbit(p_isoData, 48, buff)) > 0 )
    {
        p_BcTransMsg->len_48 = len;
        memcpy(p_BcTransMsg->BcTransLog.fld_48            , buff, MIN(len,515));
    }
    if ( (len = getbit(p_isoData, 49, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.trans_curr_cd      , buff, MIN(len, 3));
    }
    if ( (len = getbit(p_isoData, 52, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.pin_data      	   , buff, MIN(len, 8));
    }
    if ( (len = getbit(p_isoData, 54, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.addtnl_amt         , buff, MIN(len, 40));
    }
    if ( (len = getbit(p_isoData, 60, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.encrypt_info       , buff, MIN(len, 32));
    }
    if ( (len = getbit(p_isoData, 61, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.fld_61            , buff, MIN(len, 18));
    	memcpy(p_BcTransMsg->BcTransLog.id_type	, "00", 2);
    }
    if ( (len = getbit(p_isoData, 62, buff)) > 0 )
    {
    	ToGBK(buff, len, p_BcTransMsg->BcTransLog.pname, 210);
    }
    if ( (len = getbit(p_isoData, 63, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.id_type              , buff, MIN(len, 2));
    }
    if ( (len = getbit(p_isoData, 90, buff)) > 0 )
    {
        memcpy(p_BcTransMsg->BcTransLog.originaDataElements  , buff, MIN(len, 42));
    }
    return 0;
}

int BC_InsertIntoDB(tl_bc_trans_log_def *p_BcTransLog,int *SqlCode)
{
    int iRet = tlBcTransLogOpr(GLB_DB_INSERT1, p_BcTransLog, SqlCode);
    if ( 0 != *SqlCode )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>入库失败\n",__FILE__,__LINE__,*SqlCode);
        //系统故障，报警
        BC_TRACE_END( -1 );
    }
    return 0;
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

int BC_ErrLog_Func(char *pcaDumpAddr,long lDumpLen, char *pcaDispMsg)
{ 
	dcs_log(pcaDumpAddr, lDumpLen, pcaDispMsg);
}

int BC_Debug_Func(char *pcaDumpAddr,long lDumpLen, char *pcaDispMsg)
{ 
	dcs_debug(pcaDumpAddr, lDumpLen, pcaDispMsg);
}

int BC_Search_ins_inf(char *resp_code, ti_bc_ins_inf_def *p_bc_ins_inf, char *ins_id_cd)
{
	int sqlCode=-1;
	strcpy_safe(p_bc_ins_inf->ins_id_cd, ins_id_cd, 8);
  tiBcInsInfOpr(GLB_DB_SELECT1, p_bc_ins_inf, &sqlCode);
  if ( SQL_ERD_NONE_RECORD == sqlCode )
  {
  	 //dcs_log(0,0,"<FILE:%s,LINE:%d>没有查找到机构%s的信息.\n",__FILE__,__LINE__, ins_id_cd );
     strcpy(resp_code, RESP_CODE_CUS016 );
  }else if( 0!= sqlCode){
  	 //dcs_log(0,0,"<FILE:%s,LINE:%d>没有查找到机构%s的信息.\n",__FILE__,__LINE__, ins_id_cd );
  	 strcpy(resp_code, RESP_CODE_CUS015 );
  }else
  {
  	strcpy(resp_code, "N00000" );
  }
  return sqlCode;
}

int CheckRouteInsValid(char *OutRespCode, char *ins_id_cd, char *pri_acct_no, char *card_attr)
{
	int sqlCode = -1;
	ti_bc_route_card_bin_def *tiBcRouteCardBin;
	tiBcRouteCardBin = &g_tiBcRouteCardBinList;
	memset( tiBcRouteCardBin, 0, sizeof(ti_bc_route_card_bin_def) );
	sprintf(tiBcRouteCardBin->card_bin, "%.2d", strlen(pri_acct_no));
	strcpy_safe(tiBcRouteCardBin->card_bin+2, pri_acct_no, sizeof(tiBcRouteCardBin->card_bin)-1-2);
	strcpy_safe(tiBcRouteCardBin->rcv_ins_id_cd, ins_id_cd, sizeof(tiBcRouteCardBin->rcv_ins_id_cd)-1);
	strcpy_safe(tiBcRouteCardBin->card_attr, card_attr, sizeof(tiBcRouteCardBin->card_attr)-1);
	tiBcRouteCardBinOpr(GLB_DB_SELECT1, tiBcRouteCardBin, &sqlCode);
    if( 0== sqlCode)
    {
    	if( '0' == *tiBcRouteCardBin->enable_flag)
    	{
    		strcpy(OutRespCode, RESP_CODE_CUS011); //该卡bin被禁用
    		return -1;
    	}
    	strcpy(OutRespCode, "N00000");
    	return 0;	
    }
    else
    {
    	dcs_log(0,0,"<FILE:%s,LINE:%d>在结算渠道卡bin表中查询卡bin(%s)信息出现异常(sqlCode:%d).\n",__FILE__,__LINE__,tiBcRouteCardBin->card_bin,sqlCode );	
			strcpy(OutRespCode, "N00096");
    	return -1;
    }
}

int IsCreditCard(char *CardAttr)
{
	if( 0 == *CardAttr) return 0;
	if( 0 == memcmp("02", CardAttr, 2) ||  0 == memcmp("03", CardAttr, 2 ) ){
		return 1;
	}
   return 0;
}

int BC_Find_Route_Ins(char *resp_cd, char *ins_id_cd, ti_bc_mchnt_inf_def *p_tiBcMchntInf, ti_bc_card_bin_def *p_tiBcCardBin, char *pri_acct_no)
{
    if( p_tiBcMchntInf )
    {
	    //检查该商户是否支持信用卡
	    if( IsCreditCard(p_tiBcCardBin->card_attr))
	    {
	    	if( '0' == *p_tiBcMchntInf->credit_enable_flag )
	    	{
	    		strcpy(resp_cd, RESP_CODE_CUS009);
	    		return -1;
	    	}
	    	memset(ins_id_cd, 0, sizeof(ins_id_cd));
	    	strcpy_safe(ins_id_cd, p_tiBcMchntInf->credit_rcv_ins_id_cd, 8);
	    }
	    else
	   	{
	    	memset(ins_id_cd, 0, sizeof(ins_id_cd));
	    	strcpy_safe(ins_id_cd, p_tiBcMchntInf->debit_rcv_ins_id_cd, 8);
	    }
	    //检查该商户是否配置了有效的路由信息
	    rtrim(ins_id_cd);
	    if( 0 != *ins_id_cd )
	    {
	    	if( 0 == CheckRouteInsValid(resp_cd, ins_id_cd, pri_acct_no,p_tiBcCardBin->card_attr))
	    	{
	    		strcpy(resp_cd, "N00000");
	    		return 0;
	    	}
	    	else if( '0' == *p_tiBcMchntInf->auto_route_flag) //检查商户是否支持自动路由
	    	{
	    		strcpy(resp_cd, RESP_CODE_CUS010);
	    		return -1;
			}
	    }
    }
    //检查默认的路由信息
    if( '1' == *p_tiBcCardBin->enable_flag )
    {
      	memset(ins_id_cd, 0, sizeof(ins_id_cd));
    	strcpy_safe(ins_id_cd, p_tiBcCardBin->fst_rcv_ins_id_cd, 8);
    }
    else if( '2' == *p_tiBcCardBin->enable_flag )
    {
     	memset(ins_id_cd, 0, sizeof(ins_id_cd));
	   	strcpy_safe(ins_id_cd, p_tiBcCardBin->snd_rcv_ins_id_cd, 8);
    }
    else
    {
		strcpy(resp_cd, RESP_CODE_CUS011);
    	return -1;
    }
  	if( 0 == CheckRouteInsValid(resp_cd, ins_id_cd, pri_acct_no,p_tiBcCardBin->card_attr))
  	{
      	strcpy(resp_cd, "N00000");
      	return 0;
    }
    else
    {
		strcpy(resp_cd, RESP_CODE_CUS011);
    	return -1;
    }
}

// 根据 1-商户信息配置；2-卡号
// 决定接收机构信息
int BC_Search_mchnt_inf(ti_bc_mchnt_inf_def *p_tiBcMchntInf, tl_bc_trans_log_def *p_BcTransLog)
{
    int sqlCode=-1;
    int iRet=-1;
    char mchnt_cd[20+1];

    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    strcpy_safe(mchnt_cd, p_BcTransLog->mchnt_cd, sizeof(mchnt_cd)-1);
    rtrim(mchnt_cd);

    if ( '\0' == *mchnt_cd )
    {
        strcpy(p_tiBcMchntDet->trans_chnl, p_BcTransLog->trans_chnl);
        strcpy(p_tiBcMchntDet->msg_tp, p_BcTransLog->msg_tp);
        strcpy(p_tiBcMchntDet->proc_cd, p_BcTransLog->proc_cd);
        strcpy(p_tiBcMchntDet->pos_cond_cd, p_BcTransLog->pos_cond_cd);
        iRet = tiBcMchntDetOpr(GLB_DB_SELECT1, p_tiBcMchntDet, &sqlCode);
        if ( 0 != sqlCode )
        {
            //没有找到匹配的商户信息
            dcs_log(0, 0,"<FILE:%s,LINE:%d><BC_Search_mchnt_inf>cant not find mchnt_cd.(trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,sqlcode=%d) ",
                    __FILE__,__LINE__,
                    p_tiBcMchntDet->trans_chnl,
                    p_tiBcMchntDet->msg_tp,
                    p_tiBcMchntDet->proc_cd,
                    p_tiBcMchntDet->pos_cond_cd,
                    sqlCode );
            return -1;
        }
        strcpy(p_tiBcMchntInf->mchnt_cd, p_tiBcMchntDet->mchnt_cd);
    } else
    {
        strcpy(p_tiBcMchntInf->mchnt_cd, mchnt_cd);
    }

    //start to search mchnt inf    
    iRet = tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到商户信息
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );
        return -1;
    }

    return 0;
}

// ///////////////////////////////////////
// 超时表操作
int BC_PacketTimeOut(struct TimeOut_REC *p_TimeOutRec, ISO_data *iso, int flag)
{
    int len;
    char buff[256];
    TimeOut_Key *tk = (TimeOut_Key *)p_TimeOutRec->caKey;
    memset(p_TimeOutRec, ' ', sizeof(struct TimeOut_REC));
    if ( (len = getbit(iso, 11, buff)) > 0 )
    {
        memcpy_safe(tk->sys_tra_no, buff, sizeof(tk->sys_tra_no));
    }
    if ( (len = getbit(iso, 41, buff)) > 0 )
    {
        memcpy_safe(tk->term_id, buff, sizeof(tk->term_id));
    }
    if(flag)
    {
		  if ( (len = getbit(iso, 42, buff)) > 0 )
	    {
	        memcpy_safe(tk->mchnt_cd, buff, sizeof(tk->mchnt_cd) );
	    }
	  }
	 	else
	 	{
		  if ( (len = getbit(iso, 46, buff)) > 0 )
	    {
	        memcpy_safe(tk->mchnt_cd, buff, sizeof(tk->mchnt_cd) );
	    }
	 	}
    tk->end = '\0';
    return 0;
}

int BC_PacketIVRTimeOut(struct TimeOut_REC *p_TimeOutRec, IVR_STRUCT* PIVR)
{
    int len;
    char buff[256];
    TimeOut_Key *tk = (TimeOut_Key *)p_TimeOutRec->caKey;
    memset(p_TimeOutRec, 0, sizeof(struct TimeOut_REC));
    memcpy_safe((char*)tk, PIVR->key, sizeof(tk->sys_tra_no) + sizeof(tk->term_id) + sizeof(tk->mchnt_cd) + 4);
    return 0;
}

int BC_DeleteTimeOut1(struct TimeOut_REC *p_rec)
{
    return DelTimeOutTBL(p_rec);
}

int BC_InsertTimeOut1(int iflags, int iRcvFolderId,int iOrgFolderId,int iMaxTimeout,ISO_data *iso, BcTransMsg *p_TransMsg)
{
    int len;
    char buff[256];

    TimeOut_Key *tk = (TimeOut_Key *)g_TimeOutRec.caKey;
    TimeOut_ReMark *trm = (TimeOut_ReMark *)g_TimeOutRec.caRemark;
    memset(&g_TimeOutRec, ' ', sizeof(struct TimeOut_REC));

    if ( (len = getbit(iso, 11, buff)) > 0 )
    {
        memcpy_safe(tk->sys_tra_no, buff,  sizeof(tk->sys_tra_no) );
    }
    if ( (len = getbit(iso, 41, buff)) > 0 )
    {
        memcpy_safe(tk->term_id, buff,  sizeof(tk->term_id) );
    }
    if ( (len = getbit(iso, 42, buff)) > 0 )
    {
        memcpy_safe(tk->mchnt_cd, buff,  sizeof(tk->mchnt_cd) );
    }
    tk->end = '\0';
    
    //msg_tp
    if ( (len = getbit(iso, 0, buff)) > 0 )
    {
        memcpy_safe(trm->msg_tp, buff,  sizeof(trm->msg_tp ) );
    }
    //proc_cd
    if ( (len = getbit(iso, 3, buff)) > 0 )
    {
        memcpy_safe(trm->proc_cd, buff,  sizeof(trm->proc_cd)  );
    }
    
    //loc_trans_dt
    if ( (len = getbit(iso, 13, buff)) > 0 )
    {
        memcpy_safe(trm->loc_trans_dt, buff,  sizeof(trm->loc_trans_dt ));
    }
    memcpy_safe(trm->sys_tra_no , p_TransMsg->BcTransLog.sys_tra_no,  sizeof(trm->sys_tra_no) );
    memcpy_safe(trm->term_id    , p_TransMsg->BcTransLog.term_id,     sizeof(trm->term_id)  );
    memcpy_safe(trm->mchnt_cd   , p_TransMsg->BcTransLog.mchnt_cd,    sizeof(trm->mchnt_cd)  );
    trm->end = '\0';

    g_TimeOutRec.iRcvFolderId = iRcvFolderId ;
    g_TimeOutRec.iOrgFolderId = iOrgFolderId ;
    g_TimeOutRec.iMaxTimeout = iMaxTimeout ;
    g_TimeOutRec.iFlags = iflags;
    g_TimeOutRec.iExpiredTime = 0;
    return InsTimeOutTBL(&g_TimeOutRec); 
}


int BC_InsertBillNotify(int iRcvFolderId,int iOrgFolderId,int iMaxTimeout,ISO_data *iso, BcTransMsg *p_TransMsg)
{
    int len;
    char buff[256];

    TimeOut_Key *tk = (TimeOut_Key *)g_TimeOutRec.caKey;
    TimeOut_ReMark *trm = (TimeOut_ReMark *)g_TimeOutRec.caRemark;
    memset(&g_TimeOutRec, ' ', sizeof(struct TimeOut_REC));

    if ( (len = getbit(iso, 11, buff)) > 0 )
    {
        memcpy_safe(tk->sys_tra_no, buff,  sizeof(tk->sys_tra_no) );
    }
    if ( (len = getbit(iso, 41, buff)) > 0 )
    {
        memcpy_safe(tk->term_id, buff,  sizeof(tk->term_id) );
    }
    if ( (len = getbit(iso, 42, buff)) > 0 )
    {
        memcpy_safe(tk->mchnt_cd, buff,  sizeof(tk->mchnt_cd) );
    }
    tk->end = '\0';
    //msg_tp
    if ( (len = getbit(iso, 0, buff)) > 0 )
    {
        memcpy_safe(trm->msg_tp, buff,  sizeof(trm->msg_tp ) );
    }
    //proc_cd
    if ( (len = getbit(iso, 3, buff)) > 0 )
    {
        memcpy_safe(trm->proc_cd, buff,  sizeof(trm->proc_cd)  );
    }
    //loc_trans_dt
    if ( (len = getbit(iso, 13, buff)) > 0 )
    {
        memcpy_safe(trm->loc_trans_dt, buff,  sizeof(trm->loc_trans_dt ));
    }
    memcpy_safe(trm->sys_tra_no , p_TransMsg->BcTransLog.sys_tra_no,  sizeof(trm->sys_tra_no) );
    memcpy_safe(trm->term_id    , p_TransMsg->BcTransLog.term_id,     sizeof(trm->term_id)  );
    memcpy_safe(trm->mchnt_cd   , p_TransMsg->BcTransLog.mchnt_cd,    sizeof(trm->mchnt_cd)  );
    trm->end = '\0';

    g_TimeOutRec.iRcvFolderId = iRcvFolderId ;
    g_TimeOutRec.iOrgFolderId = iOrgFolderId ;
    g_TimeOutRec.iMaxTimeout = iMaxTimeout ;
    g_TimeOutRec.iFlags = 0;
    g_TimeOutRec.iExpiredTime = 0;
    return InsTimeOutTBL_2(&g_TimeOutRec); 
}

int BC_InsertIVRTimeOut(int iflags, int iRcvFolderId,int iOrgFolderId,int iMaxTimeout, tl_bc_trans_log_def *p_BcTransLog)
{
    int len;
    char buff[256];

    TimeOut_Key *tk = (TimeOut_Key *)g_TimeOutRec.caKey;
    TimeOut_ReMark *trm = (TimeOut_ReMark *)g_TimeOutRec.caRemark;
    
    memset(&g_TimeOutRec, ' ', sizeof(struct TimeOut_REC));
    memcpy_safe(tk->sys_tra_no, p_BcTransLog->sys_tra_no,  sizeof(tk->sys_tra_no) );
    memcpy_safe(tk->term_id   , p_BcTransLog->term_id   ,  sizeof(tk->term_id) );
    memcpy_safe(tk->mchnt_cd  , p_BcTransLog->mchnt_cd  ,  sizeof(tk->mchnt_cd) );
    tk->end = '\0';
    
    //addition message
//    memcpy_safe(trm->msg_tp      , p_BcTransLog->msg_tp      ,  sizeof(trm->msg_tp ) );
    strcpy(trm->msg_tp, "9989");
    memcpy_safe(trm->proc_cd     , p_BcTransLog->proc_cd     ,  sizeof(trm->proc_cd)  );
    memcpy_safe(trm->loc_trans_dt, p_BcTransLog->loc_trans_dt,  sizeof(trm->loc_trans_dt ));
    memcpy_safe(trm->sys_tra_no  , p_BcTransLog->sys_tra_no  ,  sizeof(trm->sys_tra_no) );
    memcpy_safe(trm->term_id     , p_BcTransLog->term_id     ,  sizeof(trm->term_id)  );
    memcpy_safe(trm->mchnt_cd    , p_BcTransLog->mchnt_cd    ,  sizeof(trm->mchnt_cd)  );
    trm->end = '\0';

    g_TimeOutRec.iRcvFolderId = iRcvFolderId ;
    g_TimeOutRec.iOrgFolderId = iOrgFolderId ;
    g_TimeOutRec.iMaxTimeout = iMaxTimeout ;
    g_TimeOutRec.iFlags = iflags;
    g_TimeOutRec.iExpiredTime = 0;
    return InsTimeOutTBL(&g_TimeOutRec); 
}

int BC_DeleteBillNotify(struct TimeOut_REC *p_rec)
{
    return DelTimeOutTBL_2(p_rec);
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
/*
交易标志	N1（0请求，1应答）
发起方代码	AN1(0中心 1运营商 2扣款方 3终端 4查询运营商)
渠道方代码	AN8     #32
商户号	AN15        #42
终端号	AN8         #41
日期	N8          #12
时间	N6          #13
业务种类(18域)	AN4 #18
交易类型	AN4     #--
扣款帐号	LL..ANS #2
运营商帐号	LL..ANS #44
交易金额	ANS12   #4
流水号	AN6         #11
系统参考号	ANS12   #37
应答码	            #39
*/

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
int BC_SendMessageToAJ(ISO_data *iso, char *pRecvData, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    char resp[3];
    char mac_out[32];
    int iRet;
    char resp_code[10];
    if( 0!= BC_Search_ins_inf(resp_code, p_tiBcInsInf, "00945500"))
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>查询安捷机构信息失败!",__FILE__,__LINE__);
        BC_TRACE_END( -1 );
    }
    BC_SetRcvMacKey(p_tiBcInsInf);    	
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

//GB2312码转为UTF-8码
int GBKToUTF8(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{//"ISO8859-2"
    return code_convert("GBK","UTF-8",inbuf,inlen,outbuf,outlen);
}

//unicode 索引表示方式
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

//unicode 索引表示方式
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

//unicode 索引表示方式
int UTF8ToUnicode_2(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    char buff[4096+1];
    int iret = code_convert("UTF-8","UNICODE",inbuf,inlen,outbuf,outlen);
    
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
int ToGBK(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	int len = inlen>outlen?outlen:inlen;
    if ( 0 == memcmp(inbuf,"**",2) )
    {
    	//GBK
    	outlen = inlen-2>outlen?outlen:inlen-2;
    	memcpy(outbuf, inbuf+2, outlen);
 		return outlen; 	
    }
    else
    {
    	//utf-8
        return code_convert("UTF-8", "GBK", inbuf,inlen,outbuf,outlen);
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

void memcpy_safe(char *des, char *src, int len)
{     
  int i;
	for(i=0;i<len&&*(src+i)!=0;i++) *(des+i)=*(src+i);
}

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

//记录异常记录库
int BC_exp_log(char *rsn_code, char *rsn_desc, char *binarydata, int datasize)
{
	//入库
	int sqlCode;
	tl_bc_exp_log_def tlBcExpLog;
	strcpy(tlBcExpLog.rsn_code, rsn_code);
	strcpy(tlBcExpLog.rsn_desc, rsn_desc);
	bcd_to_asc(tlBcExpLog.data, binarydata, MIN(sizeof(tlBcExpLog.data),datasize),0);
	tlBcExpLogOpr(GLB_DB_INSERT1, &tlBcExpLog, &sqlCode);
	if ( 0 != sqlCode )
  {
  	return -1;
  }
  return 0;
}

//g_iSrcFoldName
//根据foldername 获取机构信息
int BC_GetInsInfFromFoldName(char *resp, ti_bc_ins_inf_def *p_tiBcInsInf, char *foldName)
{
	int sqlCode;
	memset(p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
	memcpy_safe(p_tiBcInsInf->line_nm, foldName, sizeof(p_tiBcInsInf->line_nm)-1);
	tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
  if ( 0 == sqlCode ){
  	strcpy(resp,"N00000");
  	return 0;
  }
  else if( SQL_ERD_NONE_RECORD ==  sqlCode){
  	strcpy(resp,"N00091");
  	return 1;
  }
  strcpy(resp,"N00096");
	return -1;
}

int BC_UnLockTrans(tl_bc_trans_log_def *p_BcTransLog)
{
	int sqlCode;
	if( '1' == *g_TransLock ){
	 strcpy(p_BcTransLog->flag_lock, "0");
     tlBcTransLogOpr(GLB_DB_UPDATE11, p_BcTransLog, &sqlCode); 
     *g_TransLock = 0;           
  }
  return 0;        
}

int BC_GetMchntTransInf(char *resp_code, ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf,  
    char *mchnt_tp, char *term_id, char *mchnt_cd, char* ins_id_cd)
{
	int sqlCode = 0 ;
	strcpy(p_tiBcMchntTermChargeInf->mchnt_tp,  mchnt_tp);
    strcpy(p_tiBcMchntTermChargeInf->term_id ,  term_id);
    strcpy(p_tiBcMchntTermChargeInf->mchnt_cd,   mchnt_cd);
    strcpy(p_tiBcMchntTermChargeInf->rcv_ins_id_cd,  ins_id_cd);
    tiBcMchntTermChargeInfOpr(GLB_DB_SELECT1,  p_tiBcMchntTermChargeInf, &sqlCode);
    if( SQL_ERD_NONE_RECORD == sqlCode ){
    	strcpy(resp_code, RESP_CODE_CUS017);
    }else if (0 != sqlCode ){
    	strcpy(resp_code, "N00096");
    }else {
    	strcpy(resp_code, "N00000");
    }
    return sqlCode; 
}

int BC_Search_Trans_det(
  ti_bc_trans_det_def *p_tiBcTransDet,
  char * trans_chnl   ,    
  char * msg_tp       ,        
  char * proc_cd      ,       
  char * pos_cond_cd  ,
  char * mchnt_tp     , 
  char * msg_tp_new   ,
  char * trans_flag   ,
  char * rcv_ins_id_cd 
  )
{
	    int sqlCode = 0;
	    memset(p_tiBcTransDet, 0, sizeof(ti_bc_trans_det_def) );
	    strcpy(p_tiBcTransDet->trans_chnl,    trans_chnl    ); 
	    strcpy(p_tiBcTransDet->msg_tp,        msg_tp        );
	    strcpy(p_tiBcTransDet->proc_cd,       proc_cd       );
	    strcpy(p_tiBcTransDet->pos_cond_cd,   pos_cond_cd   );
	    strcpy(p_tiBcTransDet->mchnt_tp,      mchnt_tp      );
	    strcpy(p_tiBcTransDet->msg_tp_new,    msg_tp_new    );
	    strcpy(p_tiBcTransDet->trans_flag,    trans_flag    );
	    strcpy(p_tiBcTransDet->rcv_ins_id_cd, rcv_ins_id_cd );
	    dcs_debug(0,0,"<FILE:%s,LINE:%d>查找交易的处理方式：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s]，rcv_ins_id_cd[%s].\n",__FILE__,__LINE__,p_tiBcTransDet->trans_chnl,p_tiBcTransDet->msg_tp,p_tiBcTransDet->proc_cd,p_tiBcTransDet->pos_cond_cd,p_tiBcTransDet->mchnt_tp,p_tiBcTransDet->msg_tp_new,p_tiBcTransDet->rcv_ins_id_cd);
	    tiBcTransDetOpr(GLB_DB_SELECT1, p_tiBcTransDet, &sqlCode);
	    if ( 0 != sqlCode )
	    {
	        BC_SendExeception("02","<FILE:%s,LINE:%d>未找到该交易的后续处理方式，请检查配置表ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,p_tiBcTransDet->trans_chnl,p_tiBcTransDet->msg_tp,p_tiBcTransDet->proc_cd,p_tiBcTransDet->pos_cond_cd,p_tiBcTransDet->mchnt_tp,p_tiBcTransDet->msg_tp_new);
	        //BC_SendMonitor_trans("0","3", p_BcTransLog,&g_tiBcTransInf);
	
	        //没有找到该交易的后续处理方式，数据库配置错误
	        //没有找到该交易的转换信息
	        dcs_log(0,0,"<FILE:%s,LINE:%d>未在表ti_bc_trans_det中找到该交易的处理方式：trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,p_tiBcTransDet->trans_chnl,p_tiBcTransDet->msg_tp,p_tiBcTransDet->proc_cd,p_tiBcTransDet->pos_cond_cd,p_tiBcTransDet->mchnt_tp,p_tiBcTransDet->msg_tp_new);
	        //BC_UnLockTrans(p_BcTransLog);  
	        
	        //返回
	        return -1;
	    }
	    return 0;
}
