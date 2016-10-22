#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

/*缴费请求*/
//#define SETBIT_T(ISO,INDEX,DATA,DATA_O,LEN) if( 0 != *(DATA))setbit_t(ISO, INDEX, (unsigned char *)DATA, LEN); else setbit_t(ISO, INDEX, (unsigned char *)DATA_O, LEN);
//#define SETBIT_N(ISO,INDEX,DATA,LEN) if( 0 != *(DATA))setbit_t(ISO, INDEX, (unsigned char *)DATA, LEN);

int BusZ00_sign_ylqz(ti_bc_trans_inf_def *p_tiBcTransInf, ti_bc_ins_inf_def *p_tiBcInsInf, tl_bc_trans_log_def *p_BcTransLog, ISO_data *iso)
{
	int sqlCode;
	if( 0 != memcmp("00", p_BcTransLog->resp_cd, 2) )
	{
    dcs_log(0, 0,"<FILE:%s,LINE:%d>JAVA前置签到失败(ret:%s).接收机构信息:[%s]", __FILE__, __LINE__, p_BcTransLog->fld_28, p_BcTransLog->fwd_ins_id_cd);
    BC_SendExeception("02","警告，JAVA前置签到失败(ret:%s).接收机构信息:[%s]", p_BcTransLog->fld_28, p_BcTransLog->fwd_ins_id_cd);
    return -1;
	}  
	
  ti_bc_ins_inf_def tiBcInsInf;
	memset((char *)&tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
  memcpy(tiBcInsInf.ins_id_cd, p_BcTransLog->fwd_ins_id_cd, 8);
  memcpy(tiBcInsInf.key_1, p_BcTransLog->fld_48, 16);
  memcpy(tiBcInsInf.key_2, p_BcTransLog->fld_48 + 16, 16);
  strcpy(tiBcInsInf.key_3, " ");
  tiBcInsInfOpr(GLB_DB_UPDATE2, &tiBcInsInf, &sqlCode);
  if ( 0!= sqlCode )
  {
      dcs_debug(0,0,"更新密钥信息失败[机构信息：[%s]返回mackey[%s],pinkey[%s]].\n", tiBcInsInf.ins_id_cd, tiBcInsInf.key_1, tiBcInsInf.key_2);
      return -1;
  }	
  dcs_debug(0,0,"向JAVA前置签到成功\n");
}

int BusZ00_sign_jv(ti_bc_trans_inf_def *p_tiBcTransInf, ti_bc_ins_inf_def *p_tiBcInsInf, tl_bc_trans_log_def *p_BcTransLog, ISO_data *iso)
{
	int sqlCode;
	if( 0 != memcmp("N00000", p_BcTransLog->fld_28, 6) )
	{
    dcs_log(0, 0,"<FILE:%s,LINE:%d>JAVA前置签到失败(ret:%s).接收机构信息:[%s]", __FILE__, __LINE__, p_BcTransLog->fld_28, p_BcTransLog->fwd_ins_id_cd);
    BC_SendExeception("02","警告，JAVA前置签到失败(ret:%s).接收机构信息:[%s]", p_BcTransLog->fld_28, p_BcTransLog->fwd_ins_id_cd);
    return -1;
	}  
	
  ti_bc_ins_inf_def tiBcInsInf;
	memset((char *)&tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
  memcpy(tiBcInsInf.ins_id_cd, p_BcTransLog->fwd_ins_id_cd, 8);
  memcpy(tiBcInsInf.key_1, p_BcTransLog->fld_48, 16);
  memcpy(tiBcInsInf.key_2, p_BcTransLog->fld_48 + 16, 16);
  strcpy(tiBcInsInf.key_3, " ");
  tiBcInsInfOpr(GLB_DB_UPDATE2, &tiBcInsInf, &sqlCode);
  if ( 0!= sqlCode )
  {
      dcs_debug(0,0,"更新密钥信息失败[机构信息：[%s]返回mackey[%s],pinkey[%s]].\n", tiBcInsInf.ins_id_cd, tiBcInsInf.key_1, tiBcInsInf.key_2);
      return -1;
  }	
  dcs_debug(0,0,"向JAVA前置签到成功\n");
}

int BusZ00_sign_gz(ti_bc_trans_inf_def *p_tiBcTransInf, ti_bc_ins_inf_def *p_tiBcInsInf, tl_bc_trans_log_def *p_BcTransLog, ISO_data *iso)
{
    int iRet = -1   ;
    int sqlCode     ;
    char resp_code[6+1];
    char buff[256] ;

    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
    
    
    if( 0!= memcmp("00", p_BcTransLog->resp_cd, 2) )
    {
    	//签到失败
      dcs_log(0, 0, "<FILE:%s,LINE:%d>签到失败(ret:%s).商户号:[%s],终端号:[%s] ",__FILE__, __LINE__, p_BcTransLog->resp_cd, p_BcTransLog->mchnt_cd, p_BcTransLog->term_id);
      BC_SendExeception("02","警告，签到失败(ret:%s).商户号:[%s],终端号:[%s] ", p_BcTransLog->resp_cd, p_BcTransLog->mchnt_cd, p_BcTransLog->term_id);
      return -1;
    }
    
	 // 获取中心商户信息
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcQsMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->term_id);
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_tiBcInsInf->ins_id_cd);
    tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到商户信息
        dcs_log(p_BcTransLog, sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd,sqlCode );
        BC_SendExeception("02","警告，签到应答交易获取商户信息失败.接收机构信息:[%s],终端号:[%s],商户号:[%s]",p_tiBcQsMchntInf->rcv_ins_id_cd,p_tiBcQsMchntInf->term_id,p_tiBcQsMchntInf->mchnt_cd);
        return -1;
    }
    
    //
    memset(buff, 0, sizeof(buff));
    if ( 0 >= (iRet = getbit(iso,  62, buff))  )
    {
        dcs_log(p_BcTransLog, sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d>获取签到应答交易第60域失败.\n",__FILE__,__LINE__ );
        BC_SendExeception("02","警告，获取签到应答交易第60域失败" );
        return -1;
    }//14E68F19654136F5F99DF1E7590C0F887B5934D61C2219EE
    
    strcpy_safe(p_tiBcQsMchntInf->mkey_1, buff+24, 16);
    strcpy_safe(p_tiBcQsMchntInf->mkey_2, buff, 16);
  
    tiBcQsMchntInfOpr(GLB_DB_UPDATE2, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到商户信息
        dcs_log(0, 0,"<FILE:%s,LINE:%d>更新清算商户密钥信息失败.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd, sqlCode );
        BC_SendExeception("02","警告，更新清算商户密钥信息失败.接收机构信息:[%s],终端号:[%s],商户号:[%s]",p_tiBcQsMchntInf->rcv_ins_id_cd,p_tiBcQsMchntInf->term_id,p_tiBcQsMchntInf->mchnt_cd);
        return -1;
    }
    
	  return 0;	
}


//安捷签到报文
int BusZ00_SignInReq(char *resp_code, ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_ins_inf_def *p_tiBcInsInf, ti_bc_qs_mchnt_inf_def *p_tiBcMchntInf)
{
    int iLen;
    int iRet;
    int sqlCode;
    char buff[2048];
    char packet_type[5];
    ISO_data iso;
    ISO_data *p_iso=&iso;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_ins_inf_def tiBcInsInf;
    //发送给终端应答
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>BC_SetIsoMode failure。",__FILE__,__LINE__);
        BC_TRACE_END( -1 );
    }

    setbit_t(p_iso, 0 , "0520", 4); 
    setbit_t(p_iso, 3 , "510000", 6);
    setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog->mchnt_cd, strlen(p_BcTransLog->mchnt_cd) );

    //发送监控信息
    BC_SendMonitor_iso("0","0",p_BcTransLog->trans_chnl,&iso,p_tiBcTransInf);

    //发送签到并更新数据表
    memset(buff, 0, sizeof(buff) );

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>start call BC_SendMessageToAJ 发送数据给安捷。",__FILE__,__LINE__);
    //转发给安捷    
    BC_SetMacKey(0,0);
    memset(buff, 0, sizeof(buff) );
    PrintISO( p_iso,"发送安捷商户签到请求报文",0);
    iLen = BC_SendMessageToAJ(p_iso, buff,p_tiBcInsInf);
    if ( iLen <= 0 )
    {
        //发送给安捷失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>send to rcv failure.",__FILE__,__LINE__);

        //更新库记录
        BC_SendExeception("03","警告，发送安捷商户签到信息失败.");
        BC_TRACE_END( -1 );
    }

    //解包
    //to iso
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0!= BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>BC_SetIsoMode failure。",__FILE__,__LINE__);
        BC_TRACE_END( -1 )
    }
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>start call BC_UnPack_Data 解包安捷数据。",__FILE__,__LINE__);
    iRet = BC_UnPack_Data( p_iso, buff,  iLen);
    PrintISO( p_iso,"收到安捷商户签到应答报文",0 );

    //发送给安捷失败
    dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>已经收到安捷商户签到回复报文.",__FILE__,__LINE__);

    if ( iRet >= 0 )
    {
        BC_SendMonitor_iso("1","2",p_BcTransLog->trans_chnl,p_iso,p_tiBcTransInf);
        dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>安捷商户签到回复报文解包成功.",__FILE__,__LINE__);

        //解包成功
        if ( (iRet = getbit(p_iso, 0, buff)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>get #0 failure。",__FILE__,__LINE__);
            strcpy(resp_code,"N00096");
            BC_TRACE_END( -1 );
        }
        if ( (iRet = getbit(p_iso, 3, buff+4)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>get #3 failure。",__FILE__,__LINE__);
            strcpy(resp_code,"N00030");
            BC_TRACE_END( -1 );
        }
        if ( (iRet = getbit(p_iso, 42, buff+20)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>get #42 failure。",__FILE__,__LINE__);
            strcpy(resp_code,"N00030");
            BC_TRACE_END( -1 );
        }

        //check
        if ( 0 != memcmp(buff, "0530510000", 10) 
             || 0 != memcmp(buff+20, p_BcTransLog->mchnt_cd,15)
           )
        {
            dcs_log(buff,50,"<FILE:%s,LINE:%d><BusZ00_SignInReq>check #0 #3 #42(%s) failure。",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
            strcpy(resp_code,"N00030");
            BC_TRACE_END( -1 );
        }

        //return code
        if ( (iRet = getbit(p_iso, 28, buff)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>get #28 failure。",__FILE__,__LINE__);
            strcpy(resp_code,"N00030");
            return 0;
        }

        memcpy(resp_code, buff, 6);
        resp_code[6] = 0;
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>return #28[%s]。",__FILE__,__LINE__, resp_code);
        if ( 0 != memcmp(resp_code, "N00000", 6) )
        {
            return 0;
        }

        //mac
        if ( (iRet = getbit(p_iso, 48, buff)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>get #48 failure。",__FILE__,__LINE__);
            BC_TRACE_END( -1 );
        }

        //update key
        memset((char *)&tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def));
        memcpy(tiBcInsInf.ins_id_cd, "00945500", 8);
        memcpy(tiBcInsInf.key_1, buff, 16);
        memcpy(tiBcInsInf.key_2, buff, 16);
        memcpy(tiBcInsInf.key_3, buff, 16);
        tiBcInsInfOpr(GLB_DB_UPDATE2, &tiBcInsInf, &sqlCode);
/*        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>start to call tiBcMchntInfOpr update key .",__FILE__,__LINE__);
        memcpy(p_tiBcMchntInf->mkey_3, buff, 16);
        *(p_tiBcMchntInf->mkey_3 + 16) = 0;
        iRet = tiBcQsMchntInfOpr(GLB_DB_UPDATE2, p_tiBcMchntInf, &sqlCode);*/
        if ( 0!= sqlCode )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>更新安捷密钥失败！",__FILE__,__LINE__);
            //更新密钥失败
            strcpy(resp_code,"N00096");
            return 0;
        }
        return 0;
    }
    BC_TRACE_END( -1 );
}


/************************************
 *
 *       账单系统签到处理 req
 *
************************************/
int BusZ0000(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet = -1;
    int sqlCode ;
    char resp_cd[6+1];
    strcpy(resp_cd,"96");
    char mac_out[32+1];
    char buff[256],temp[256];

    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf= &g_tiBcInsInf;
    ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf;

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0000>enter BusZ0000.",__FILE__,__LINE__);
    BC_SendMonitor_trans("0","3",p_BcTransLog,p_tiBcTransInf);

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0000>start to call BC_CheckIsoDataField.",__FILE__,__LINE__);
    if ( 0 != BC_CheckIsoDataField(iso, "3,33") )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ0000>doBus_CheckIsoDataField failure.",__FILE__,__LINE__);
        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"03",2 );
        setbit_t(iso,28,"N00003",6 );
        BC_SetTermMacKey(0,0);
        strcpy(p_BcTransLog->resp_cd,"03");
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        //return BC_SendMessageToSrc(iso);
        BC_SendMessageToSrc(iso) ;
        return -1;
        return 0;
    }

    //search ins inf
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0000>start to call tiBcInsInfOpr for select.",__FILE__,__LINE__,RPT_TO_LOG,0,0);
    memset( p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy(p_tiBcInsInf->ins_id_cd, g_TransMsg.BcTransLog.fwd_ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ0000>tiBcInsInfOpr search ins[%s] info failure[sqlcode:%d].",__FILE__,__LINE__,p_tiBcInsInf->ins_id_cd,sqlCode);
        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"91",2 );
        setbit_t(iso,28,"N00091",6 );
        BC_SetTermMacKey(0,0);
        strcpy(p_BcTransLog->resp_cd,"91");
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        //return BC_SendMessageToSrc(iso);
        BC_SendMessageToSrc(iso) ;
        return -1;
        return 0;
    }

    if ( '0' == *p_tiBcInsInf->openflag )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ0000>ins openflag is closed.",__FILE__,__LINE__);
        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"91",2 );
        setbit_t(iso,28,"N00091",6 );
        BC_SetTermMacKey(0,0);
        strcpy(p_BcTransLog->resp_cd,"91");
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        //return BC_SendMessageToSrc(iso);
        BC_SendMessageToSrc(iso) ;
        return -1;
        return 0;
    }

    //检查是否有商户代码，
    ///*
    if ( 0 == BC_CheckIsoDataField(iso, "42") )
    {
    	  // 确定接收机构代码
    	  iRet = BC_GetInsInfFromFoldName(resp_cd, &tiBcInsInf, g_iSrcFoldName);
    	  if( 0 != iRet ){
    	  	  dcs_log(0, 0,"<FILE:%s,LINE:%d><BusT22_send_consume_req>call BC_GetInsInfFromFoldName failure,cann't find ins inf.",__FILE__,__LINE__);
            BC_SendExeception("02","根据线路名称[%s]查询机构信息失败.", g_iSrcFoldName);    
    	  	  BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso,39,resp_cd+4,2 );
            setbit_t(iso,28,resp_cd,6 );
            //BC_SetTermMacKey(0,0);
            BC_SetRcvMacKey(p_tiBcInsInf);
            strcpy(p_BcTransLog->resp_cd,"96");
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            //return BC_SendMessageToSrc(iso);
            BC_SendMessageToSrc(iso) ;
            return -1;
            return 0;
    	  }
    	  strcpy(g_rtIns_id_cd, tiBcInsInf.ins_id_cd);
    	  
        //如果有，进行商户签到处理
        // search mchnt inf
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0000>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
        strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->mchnt_cd);
        strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->term_id);
        strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, g_rtIns_id_cd);        
        iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
        if ( 0 != sqlCode )
        {
            //没有找到商户信息
            dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0000>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );
            strcpy(buff, "N00003");
        } else
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0000>start to call BusZ00_SignInReq!",__FILE__,__LINE__);
            //商户签到
            iRet = BusZ00_SignInReq(buff, p_tiBcTransInf, &tiBcInsInf, p_tiBcQsMchntInf);
            if ( 0!=iRet )
            {
                dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>call BusZ00_SignInReq return failure.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd );
            }
        }
    } else
    {
        //机构签到
        //GET KEY
        dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0000>start to first call DESGETKY.",__FILE__,__LINE__);
        DESGETKY(resp_cd,p_tiBcInsInf->key_1,p_tiBcInsInf->key_index);
        if ( 0 != memcmp( resp_cd, "00", 2) )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ0000>DESGETKY failure.",__FILE__,__LINE__);
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso,39,"96",2 );
            setbit_t(iso,28,"N00096",6 );
            BC_SetTermMacKey(0,0);
            strcpy(p_BcTransLog->resp_cd,"96");
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            //return BC_SendMessageToSrc(iso);
            BC_SendMessageToSrc(iso) ;
            return -1;
            return 0;
        }

        dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0000>start to second call DESGETKY.",__FILE__,__LINE__);
        DESGETKY(resp_cd,p_tiBcInsInf->key_2,p_tiBcInsInf->key_index);
        if ( 0 != memcmp( resp_cd, "00", 2) )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ0000>DESGETKY failure.",__FILE__,__LINE__);
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso,39,"96",2 );
            setbit_t(iso,28,"N00096",6 );
            BC_SetTermMacKey(0,0);
            strcpy(p_BcTransLog->resp_cd,"96");
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            //return BC_SendMessageToSrc(iso);
            BC_SendMessageToSrc(iso) ;
            return -1;
        }

        dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0000>start to third call DESGETKY.",__FILE__,__LINE__);
        DESGETKY(resp_cd,p_tiBcInsInf->key_3,p_tiBcInsInf->key_index);
        if ( 0 != memcmp( resp_cd, "00", 2) )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ0000>DESGETKY failure.",__FILE__,__LINE__);
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso,39,"96",2 );
            setbit_t(iso,28,"N00096",6 );
            BC_SetTermMacKey(0,0);
            strcpy(p_BcTransLog->resp_cd,"96");
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            //return BC_SendMessageToSrc(iso);
            BC_SendMessageToSrc(iso) ;
            return -1;
            return 0;
        }

        //update ins info
        dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0000>start to call tiBcInsInfOpr for update mackey.",__FILE__,__LINE__);
        tiBcInsInfOpr(GLB_DB_UPDATE2, p_tiBcInsInf, &sqlCode);
        if ( 0 != sqlCode )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ0000>tiBcInsInfOpr update ins info failure.",__FILE__,__LINE__);
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso,39,"96",2 );
            setbit_t(iso,28,"N00096",6 );
            BC_SetTermMacKey(0,0);
            strcpy(p_BcTransLog->resp_cd,"96");
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            //return BC_SendMessageToSrc(iso);
            BC_SendMessageToSrc(iso) ;
            return -1;
            return 0;
        }

        //成功
        strcpy(buff, "N00000");
    }

    //dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0000>buff[%s].",__FILE__,__LINE__,buff);

    //加密
    //iRet = BC_GenMac(p_tiInsInf->key_index,p_tiInsInf->key_1,p_tiInsInf->key_1,16,mac_out);
    setbit_t(iso,28,buff,6 );
    if ( 0 == memcmp("N00000", buff, 6) )
    {
    		memcpy(buff, p_tiBcInsInf->key_1, 16);
    		memcpy(buff+16, p_tiBcInsInf->key_2, 16);
        setbit_t(iso, 48, buff, 32);
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ0000>key:[%s%s].",__FILE__,__LINE__,p_tiBcInsInf->key_1, p_tiBcInsInf->key_2);
    }
    BC_StringAdder(p_BcTransLog->msg_tp,10,temp,4);
    setbit_t(iso,0,temp,4 );

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0000>start to call BC_SetTermMacKey.",__FILE__,__LINE__);
    BC_SetTermMacKey(0,0);

    strcpy(p_BcTransLog->resp_cd,buff+4);
    BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
    BC_SendMessageToSrc(iso) ;
    return -1;
    return 0;
}

//发送安捷订单查询报文
int BusZ0100_1(BcTransMsg *p_BcTransMsg,char *resp_code, ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_ins_inf_def *p_tiBcInsInf, ti_bc_qs_mchnt_inf_def *p_tiBcMchntInf)
{
    int iLen;
    int iRet;
    int sqlCode;
    char buff[2048];
    char packet_type[5];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ISO_data iso;
    ISO_data *p_iso = &iso ;

    //组建报文
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ01_1>BC_SetIsoMode failure。",__FILE__,__LINE__);
        BC_TRACE_END( -1 );
    }

    setbit_t(p_iso, 0 , "0620", 4); 
    setbit_t(p_iso, 3 , "610000", 6);
    setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog->mchnt_cd, strlen(p_BcTransLog->mchnt_cd));
    setbit_t(p_iso, 45, (unsigned char *)p_BcTransLog->fld_45, 19);
    setbit_t(p_iso, 64, "00000000", 8);

    //发送监控信息
    BC_SendMonitor_iso("0","0",p_BcTransLog->trans_chnl,p_iso,p_tiBcTransInf);

    //发送签到并更新数据表
    memset(buff, 0, sizeof(buff) );

    //dcs_debug(p_tiBcMchntInf,sizeof(ti_bc_mchnt_inf_def),"<FILE:%s,LINE:%d><BusZ01_1>start call BC_SendMessageToAJ 发送数据给安捷。",__FILE__,__LINE__);
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ01_1>start call BC_SendMessageToAJ 发送数据给安捷。",__FILE__,__LINE__);
    //转发给安捷    
    BC_SetMacKey(p_tiBcMchntInf->mkey_index,p_tiBcMchntInf->mkey_1);

    memset(buff, 0, sizeof(buff) );
    PrintISO( p_iso,"发送安捷订单查询请求报文",0);
    iLen = BC_SendMessageToAJ(p_iso, buff,p_tiBcInsInf);
    if ( iLen <= 0 )
    {
        //发送给安捷失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ01_1>send to rcv failure.",__FILE__,__LINE__);

        //更新库记录
        BC_SendExeception("02","警告，发送安捷商户签到信息失败.");
        strcpy(resp_code,"N00091");
        BC_TRACE_END( -1 );
    }

    //解包
    //to iso
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);
    if ( 0!= BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ01_1>BC_SetIsoMode failure。",__FILE__,__LINE__);
        BC_TRACE_END( -1 )
    }
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ01_1>start call BC_UnPack_Data 解包安捷数据。",__FILE__,__LINE__);
    iRet = BC_UnPack_Data( p_iso, buff,  iLen);
    PrintISO( p_iso,"收到安捷商户订单查询应答报文",0);

    //发送给安捷失败
    dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ01_1>已经收到安捷订单回复报文.",__FILE__,__LINE__);
    if ( iRet >= 0 )
    {
        BC_SendMonitor_iso("1","2",p_BcTransLog->trans_chnl,p_iso,p_tiBcTransInf);

        dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ01_1>安捷订单回复报文解包成功.",__FILE__,__LINE__);

        iRet = BC_CopyISOdata( p_iso, p_BcTransMsg);
        if ( iRet <0 )
        {
            strcpy(resp_code,"N00030");
            BC_TRACE_END( -1 );
        }
        strcpy(resp_code, p_BcTransMsg->BcTransLog.fld_28);

        //check
        if ( 0 != memcmp(p_BcTransMsg->BcTransLog.msg_tp, "0630", 4) 
             || 0 != memcmp(p_BcTransMsg->BcTransLog.proc_cd, "610000", 6) 
             || 0 != memcmp(p_BcTransMsg->BcTransLog.mchnt_cd, p_BcTransLog->mchnt_cd, strlen(p_BcTransMsg->BcTransLog.mchnt_cd))
           )
        {
            dcs_log(buff,50,"<FILE:%s,LINE:%d><BusZ01_1>check #0 #3 #42(%s) failure。",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
            strcpy(resp_code,"N00096");
            BC_TRACE_END( -1 );
        }

        //update 订单信息

        return 0;
    }
    dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ01_1>安捷订单回复报文解包失败.",__FILE__,__LINE__);
    strcpy(resp_code, "N00096");
    BC_TRACE_END( -1 );
}

/************************************
 *
 *       账单系统订单查询 req
 *
************************************/
int BusZ0100(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet = BC_Fork_Enter(0);
    if ( iRet >0 )//parent
    {
        return 0;
    }

    int iLen ;
    int sqlCode ;
    char resp_cd[3];
    strcpy(resp_cd,"96");
    char mac_out[32+1];
    ISO_data iso_resp;
    char buff[256];
    char resp_code[6+1];

    ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog ;
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
    ti_bc_ins_inf_def *p_tiBcInsInf= &g_tiBcInsInf ;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf ;
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
    ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf = (ti_bc_mchnt_term_charge_inf_def *) &g_tiBcMchntTermChargeInf; //商户终端转换信息

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0100>enter BusZ0100.",__FILE__,__LINE__);
    BC_SendMonitor_trans("0","3",p_BcTransLog,p_tiBcTransInf);

    if ( 0 != BC_CheckIsoDataField(iso, "33") )
    {
        strcpy(resp_cd,"30");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0100>call BC_CheckIsoDataField  #33域未填写.",__FILE__,__LINE__);
        BC_SendExeception("02","#33域未填写.");

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0100>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00096", 6 );
        setbit_t(iso,64,"00000000", 8 );
        //BC_SetMacKey(0,0); gy
    	  BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        //return BC_SendMessageToSrc(iso);
        BC_SendMessageToSrc(iso) ;
        return -1;
        return 0;
    }

    //search #33 ins inf
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0100>start to call tiBcInsInfOpr for select.",__FILE__,__LINE__,RPT_TO_LOG,0,0);
    memset( p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy( p_tiBcInsInf->ins_id_cd, p_BcTransLog->fwd_ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode  || '0' == *p_tiBcInsInf->openflag )
    {
        strcpy(resp_cd,"91");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0100>call BC_CheckIsoDataField  #33域机构不存在.",__FILE__,__LINE__);
        BC_SendExeception("02","#33域机构不存在.");

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0100>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00096", 6 );
        setbit_t(iso,64,"00000000", 8 );
        //BC_SetMacKey(0,0); gy
    	   BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return -1;
        return 0;
    }

    if ( *p_tiBcInsInf->key_type != '0' )
    {
        //check 账单系统密钥
        if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) )
        {
            //交易失败        
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0100>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso,28,"N000A0", 6 );
            setbit_t(iso,64,"00000000", 8 );
            //BC_SetMacKey(0,0); gy
	          BC_SetRcvMacKey(p_tiBcInsInf);
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            //return BC_SendMessageToSrc(iso);
            BC_SendMessageToSrc(iso) ;
            return -1;
            return 0;
        }
    }

    //订单查询请求check域填写状况
    if ( 0 != BC_CheckIsoDataField(iso, "3,42,45") )
    {
        strcpy(resp_cd,"30");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0100>call BC_CheckIsoDataField 域填写不完整.",__FILE__,__LINE__);
        BC_SendExeception("02","域填写不完整.");

        //入库

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0100>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00096", 6 );
        setbit_t(iso,64,"00000000", 8 );
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return -1;
        return 0;
    }

    // 获取商户信息，用于商户验证、密钥及相关交易金额控制等
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog->mchnt_cd);
    iRet = tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到商户信息
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0100>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

        //入库

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0100>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00003", 6 );
        setbit_t(iso,64,"00000000", 8 );        
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        //return BC_SendMessageToSrc(iso);
        BC_SendMessageToSrc(iso) ;
        return -1;
        return 0;
    }

    //查询账单信息表
    memset(p_tlBcBillLog, 0, sizeof(tl_bc_bill_log_def));
    strcpy(p_tlBcBillLog->fwd_ins_id_cd, p_BcTransLog->fwd_ins_id_cd);
    memcpy(p_tlBcBillLog->bill_id, p_BcTransLog->fld_45, 19);
    *(p_tlBcBillLog->bill_id+19) = 0;
    iRet = tlBcBillLogOpr(GLB_DB_SELECT2, p_tlBcBillLog, &sqlCode);
    if( 0 ==  sqlCode )
    {
        //查询到了订单信息
        strcpy(resp_code, "N00000");
        setbit_t(iso, 15, p_tlBcBillLog->settle_dt, 4);
        setbit_t(iso, 37, p_tlBcBillLog->retri_ref_no, 12);
        if( '1' == *p_tlBcBillLog->bc_settle_in ){
            setbit_t(iso, 48, "N00000", 6);
        }else if( '0' == *p_tlBcBillLog->bc_settle_in ){
            setbit_t(iso, 48, "N00001", 6);
        }else{
            setbit_t(iso, 48, "N00007", 6);
        }
    }
    else if ( SQL_ERD_NONE_RECORD == sqlCode )
    {
        //订单号不存在
        strcpy(resp_code, "N00201");
    }else{
        //查询数据发生异常
        strcpy(resp_code, "N00096");
    }

    //应答给账单系统
    BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
    setbit_t(iso, 0 ,buff,4 );
    setbit_t(iso, 28,resp_code, 8 );
    setbit_t(iso, 64,"00000000", 8 );
    BC_SetRcvMacKey(p_tiBcInsInf);
    BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
    BC_SendMessageToSrc(iso) ;
    return -1;
    return 0;
}


//发送安捷交易结果查询报文
int BusZ0200_1(BcTransMsg *p_BcTransMsg,char *resp_code, ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_ins_inf_def *p_tiBcInsInf, ti_bc_qs_mchnt_inf_def *p_tiBcMchntInf, int updateflag)
{
    int iLen;
    int iRet;
    int sqlCode;
    char buff[4096];
    char packet_type[5];
    TimeOut_Key tk;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ISO_data iso;
    ISO_data *p_iso = &iso ;

    //组建报文
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ01_1>BC_SetIsoMode failure。",__FILE__,__LINE__);
        BC_TRACE_END( -1 );
    }
    setbit_t(p_iso, 0 , "0621", 4); 
    setbit_t(p_iso, 3 , "610001", 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
    setbit_t(p_iso, 42, (unsigned char *)p_tiBcMchntInf->mchnt_cd, 15);
    tl_bc_trans_log_def bcTransLog;
    memset(&bcTransLog, 0, sizeof(tl_bc_trans_log_def));
    strcpy(bcTransLog.loc_trans_dt, p_BcTransLog->loc_trans_dt);
    memcpy(bcTransLog.sys_tra_no, p_BcTransLog->fld_48,6);
    strcpy(bcTransLog.mchnt_cd, p_BcTransLog->mchnt_cd);
    tlBcTransLogOpr(GLB_DB_SELECT2, &bcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        strcpy(resp_code,"N00096");
        dcs_log(&bcTransLog,sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d><BusZ01_1>根据查询结果更新交易记录信息失败sqlcode[%d]。",__FILE__,__LINE__,sqlCode);
        BC_TRACE_END( -1 );
    }
    setbit_t(p_iso, 48, (unsigned char *)bcTransLog.center_sys_tra_no, 6);
    setbit_t(p_iso, 64, "00000000", 8);

    //发送监控信息
    BC_SendMonitor_iso("0","0",p_BcTransLog->trans_chnl,p_iso, 0);

    //发送签到并更新数据表
    memset(buff, 0, sizeof(buff) );

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ01_1>start call BC_SendMessageToAJ 发送数据给安捷。",__FILE__,__LINE__);

    //转发给安捷    
    BC_SetMacKey(p_tiBcMchntInf->mkey_index,p_tiBcMchntInf->mkey_1);
    memset(buff, 0, sizeof(buff) );
    PrintISO( p_iso,"发送给安捷的交易结果查询报文",0);
    iLen = BC_SendMessageToAJ(p_iso, buff,p_tiBcInsInf);
    if ( iLen <= 0 )
    {
        //发送给安捷失败
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ01_1>send to rcv failure.",__FILE__,__LINE__);

        //更新库记录
        BC_SendExeception("03","警告，发送安捷商户交易结果信息失败.");
        strcpy(resp_code,"N00098");
        BC_TRACE_END( -1 );
    }

    //解包
    //to iso
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);
    if ( 0!= BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ01_1>BC_SetIsoMode failure。",__FILE__,__LINE__);
        BC_TRACE_END( -1 )
    }
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ01_1>start call BC_UnPack_Data 解包安捷数据。",__FILE__,__LINE__);
    iRet = BC_UnPack_Data( p_iso, buff,  iLen);
    PrintISO( p_iso,"收到安捷交易结果应答报文",0);

    //发送给安捷
    dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ01_1>已经收到安捷交易结果回复报文.",__FILE__,__LINE__);
    if ( iRet >= 0 )
    {
        BC_SendMonitor_iso("1","2",p_BcTransLog->trans_chnl,p_iso, 0);
        dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ01_1>交易结果回复报文解包成功.",__FILE__,__LINE__);

        iRet = BC_CopyISOdata( p_iso, p_BcTransMsg);
        if ( iRet <0 )
        {
            strcpy(resp_code,"N00030");
            BC_TRACE_END( -1 );
        }

        dcs_debug(0, 0,"<FILE:%s,LINE:%d><BusZ01_1>start to 检查收到的交易结果回复报文.",__FILE__,__LINE__);

        //check
        if ( 0 != memcmp(p_BcTransMsg->BcTransLog.msg_tp, "0631", 4) 
             || 0 != memcmp(p_BcTransMsg->BcTransLog.proc_cd, "610001", 6) 
             || 0 != memcmp(p_BcTransMsg->BcTransLog.mchnt_cd, p_tiBcMchntInf->mchnt_cd,15)
           )
        {
            dcs_log(buff,50,"<FILE:%s,LINE:%d><BusZ01_1>check #0 #3 #42(%s) failure。",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
            strcpy(resp_code,"N00096");
            BC_TRACE_END( -1 );
        }

        //update 订单信息
        strcpy(resp_code,p_BcTransLog->fld_28);

        if ( 0 == memcmp(resp_code, "N00000", 6)
             && 0 != updateflag )
        {
            //查询成功
            dcs_log(buff,50,"<FILE:%s,LINE:%d><BusZ01_1>查询成功，准备更新相关支付交易状态。",__FILE__,__LINE__ );
            int i;
            int len = strlen(p_BcTransMsg->BcTransLog.fld_48);
            memcpy(buff, p_BcTransMsg->BcTransLog.fld_48, len);
            rtrim(buff);
            for ( i=0; i< len/20; i++ )
            {
                tl_bc_trans_log_def bcTransLog;
                memset(&bcTransLog, 0, sizeof(tl_bc_trans_log_def));
                memcpy(bcTransLog.loc_trans_dt, buff+i*20, 4);
                memcpy(bcTransLog.settle_dt, buff+i*20+16, 4);
                memcpy(bcTransLog.fld_28,       buff+i*20+4 , 6);
                memcpy(bcTransLog.center_sys_tra_no,   buff+i*20+10, 6);
                memcpy(bcTransLog.center_mchnt_cd  ,   p_BcTransMsg->BcTransLog.mchnt_cd, 15);
                strcpy(bcTransLog.retri_ref_no, "  ");
                if(memcmp(bcTransLog.fld_28, "N00000", 6) == 0)
                	strcpy(bcTransLog.bc_settle_in, "1");
                else
                	strcpy(bcTransLog.bc_settle_in, "0");
                 
                tlBcTransLogOpr(GLB_DB_UPDATE3, &bcTransLog, &sqlCode);
                if ( 0!= sqlCode )
                {
                    dcs_log(&bcTransLog,sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d><BusZ01_1>根据查询结果更新交易记录信息失败sqlcode[%d]。",__FILE__,__LINE__,sqlCode);
                }
				tlBcBillLogUpdate(GLB_DB_UPDATE1,&bcTransLog, &sqlCode);
    		   memset(&tk, ' ', sizeof(TimeOut_Key));
		       memcpy(tk.sys_tra_no, bcTransLog.center_sys_tra_no, 6);
		       memcpy(tk.mchnt_cd, bcTransLog.center_mchnt_cd, 15);
		       tk.end = '\0';
        	  memcpy(g_TimeOutRec.caKey, (char *)&tk, sizeof(TimeOut_Key) );
        	//删除账单超时表
        	  BC_DeleteBillNotify(&g_TimeOutRec);
                //标志为 2 时，仅更新第一条记录，防止性能受到影响

                if ( 2 == updateflag ) break;
                break;
            }
        }

        return 0;
    }
    BC_TRACE_END( -1 );
}

/************************************
 *
 *       账单系统交易结果查询 req
 *
************************************/
int BusZ0200(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet;
    if ( 0 < BC_Fork_Enter(0) )//parent
    {
        return 0;
    }

    int iLen ;
    int sqlCode ;
    char resp_cd[3];
    strcpy(resp_cd,"96");
    char mac_out[32+1];
    ISO_data iso_resp;
    char buff[256];
    char resp_code[6+1];

    ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
    tl_bc_trans_log_def bcTransLog; //用于查询原始交易
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog ;   
    ti_bc_ins_inf_def *p_tiBcInsInf= &g_tiBcInsInf ; //发送机构信息，用于发送机构密钥验证
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf ;  //商户信息
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //清算商户信息
    ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf = (ti_bc_mchnt_term_charge_inf_def *) &g_tiBcMchntTermChargeInf; //商户终端转换信息

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0200>enter BusZ0100.",__FILE__,__LINE__);
    BC_SendMonitor_trans("0","3",p_BcTransLog,p_tiBcTransInf);

    //检查#33域是否填写
    if ( 0 != BC_CheckIsoDataField(iso, "33") )
    {
        strcpy(resp_cd,"30");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0200>call BC_CheckIsoDataField  #33域未填写.",__FILE__,__LINE__);
        BC_SendExeception("02","#33域未填写.");

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0200>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00096", 6 );
        setbit_t(iso,64,"00000000", 8 );
        //BC_SetMacKey(0,0); gy
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return -1;
        return 0;
    }

    //搜索#33域机构信息
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ0200>start to call tiBcInsInfOpr for select.",__FILE__,__LINE__,RPT_TO_LOG,0,0);
    memset( p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy( p_tiBcInsInf->ins_id_cd, p_BcTransLog->fwd_ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);

    if ( 0 != sqlCode  || '0' == *p_tiBcInsInf->openflag )
    {
        strcpy(resp_cd,"91");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0200>call BC_CheckIsoDataField  #33域机构不存在.",__FILE__,__LINE__);
        BC_SendExeception("02","#33域机构不存在.");

        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0200>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00096", 6 );
        setbit_t(iso,64,"00000000", 8 );
        //BC_SetMacKey(0,0); gy
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return -1;
        return 0;
    }

    //判断该#33域机构是否要验证密钥，如果需要，则进行机构密钥验证
    if ( *p_tiBcInsInf->key_type != '0' )
    {
        if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) )
        {
            //交易失败        
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0100>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso,28,"N000A0", 6 );
            setbit_t(iso,64,"00000000", 8 );
            //BC_SetMacKey(0,0); gy
            BC_SetRcvMacKey(p_tiBcInsInf);
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            BC_SendMessageToSrc(iso) ;
            return -1;
            return 0;
        }
    }

    //检查交易结果查询的相关域是否填写
    if ( 0 != BC_CheckIsoDataField(iso, "3,42,48") )
    {
        strcpy(resp_cd,"30");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0200>call BC_CheckIsoDataField 域填写不完整.",__FILE__,__LINE__);
        BC_SendExeception("02","域填写不完整.");

        //入库
        //交易失败        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0200>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00096", 6 );
        setbit_t(iso,64,"00000000", 8 );
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return -1;
    }

    //查询原始交易信息，如果原始交易已经有结果了，则直接返回
    dcs_debug(0, 0, "<FILE:%s,LINE:%d><BusZ0200>start to call tlBcTransLogOpr查询交易结果(#11:%S)!",__FILE__,__LINE__, p_BcTransLog->fld_48);
    if ( g_TransMsg.len_48 < 7 )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0200>start to call tlBcTransLogOpr查询交易结果!",__FILE__,__LINE__);

        //仅一位流水号的时候进行本地数据库的查询
        strcpy(bcTransLog.loc_trans_dt, p_BcTransLog->loc_trans_dt);
        memcpy(bcTransLog.sys_tra_no, p_BcTransLog->fld_48,6);
        strcpy(bcTransLog.mchnt_cd, p_BcTransLog->mchnt_cd);
        tlBcTransLogOpr(GLB_DB_SELECT2, &bcTransLog, &sqlCode);
        if ( 0 == sqlCode )
        {
            //查询到了原始交易，检查相关域，交易是否已经完成了
            //if ( '1' == *bcTransLog.is_over )
            {
                //交易已经处理完成，直接应答给账单系统
                BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
                setbit_t(iso,0,buff,4 );
                setbit_t(iso, 28, "N00000", 6);
                setbit_t(iso, 15, bcTransLog.settle_dt, 4);
                setbit_t(iso, 37, bcTransLog.retri_ref_no, 12);
                //set fld_48 result
                memset(buff, 0, sizeof(buff) );
                memcpy(buff, bcTransLog.loc_trans_dt, 4);
                memcpy(buff+4, ('1' == *bcTransLog.is_over)?bcTransLog.fld_28:"N00009", 6);
                memcpy(buff+4+6, bcTransLog.sys_tra_no, 6);
            	rtrim(bcTransLog.settle_dt);
		if (strlen(bcTransLog.settle_dt))
		{
                	memcpy(buff+4+6+6, bcTransLog.settle_dt, 4);
		}
		else
		{
                	memcpy(buff+4+6+6, "0000", 4);
		}
                setbit_t(iso, 48, buff, 20);
                //set mac key
                setbit_t(iso, 64,"00000000", 8 );
                BC_SetRcvMacKey(p_tiBcInsInf);
                BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
                BC_SendMessageToSrc(iso) ;

                //
                if ( '1' == *bcTransLog.is_over )
                {
                    return -1;
                }
            }
            //交易未完成，进行安捷交易结果的查询
        } else if ( SQL_ERD_NONE_RECORD == sqlCode )
        {
            //未查询到记录
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso, 28, "N000w5", 6);
            //set fld_48 result
            //set mac key
            setbit_t(iso, 64,"00000000", 8 );
            BC_SetRcvMacKey(p_tiBcInsInf);
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            BC_SendMessageToSrc(iso) ;
            return -1;
        }
    }
		
    // 获取商户信息，用于商户验证、密钥及相关交易金额控制等
    if(memcmp(p_BcTransLog->rcv_ins_id_cd, "00945500", 8)==0) //安捷系统转按捷查询
    {
	    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0200>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
	    strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog->mchnt_cd);
	    iRet = tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
	    if ( 0 != sqlCode )
	    {
	        //没有找到商户信息
	        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0200>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );
	        BC_SendExeception("03","cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ", p_tiBcMchntInf->mchnt_cd, sqlCode );
	        return -1;
	    }
	
	    //检查商户转换信息表 ti_bc_mchnt_term_charge_inf_def g_tiBcMchntTermChargeInf;
	    strcpy(p_tiBcMchntTermChargeInf->mchnt_tp,  p_BcTransLog->mchnt_tp);
	    strcpy(p_tiBcMchntTermChargeInf->term_id ,  p_BcTransLog->term_id);
	    strcpy(p_tiBcMchntTermChargeInf->mchnt_cd,  p_BcTransLog->mchnt_cd);
	    strcpy(p_tiBcMchntTermChargeInf->rcv_ins_id_cd,  p_BcTransLog->rcv_ins_id_cd);
	    iRet = tiBcMchntTermChargeInfOpr(GLB_DB_SELECT2, p_tiBcMchntTermChargeInf, &sqlCode);
	    if( 0 != sqlCode )
	    {
	        // 查询商户转换信息表失败
	        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0200>call tiBcMchntTermChargeInfOpr failure,cann't find ins(%s) inf.",__FILE__,__LINE__, p_tiBcMchntTermChargeInf->mchnt_cd);
	        BC_SendExeception("03","查询商户转换信息失败,mchnt_cd:[%s].",p_tiBcMchntTermChargeInf->mchnt_cd);
	        return -1;
	    }
	
	    // 获取清算商户信息
	    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0200>start to call tiBcQsMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
	    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_tiBcMchntTermChargeInf->mchnt_cd_c);
	    strcpy(p_tiBcQsMchntInf->term_id, p_tiBcMchntTermChargeInf->term_id_c);
	    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_tiBcMchntTermChargeInf->rcv_ins_id_cd);
	    iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
	    if ( 0 != sqlCode )
	    {
	        //没有找到清算商户信息
	        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0200>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd,sqlCode );
	        BC_SendExeception("03","cant not find qs mchnt.(mchnt_cd:%s,sqlcode=%d) ",p_tiBcQsMchntInf->mchnt_cd,sqlCode );
	        return -1;
	    }
	
	    //搜索路由机构信息，即#100域机构信息
	    if ( 0!= BC_Search_ins_inf(resp_code, &tiBcInsInf, g_rtIns_id_cd) )
	    {
	        // 查询接收机构线路信息失败
	        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0200>call BC_Search_ins_inf failure,cann't find ins inf.",__FILE__,__LINE__);
	        BC_SendExeception("03","查询机构线路信息失败,机构id:[%s].", g_rtIns_id_cd);
	        return -1;
	    }
	
	    //转发给安捷
	    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ0200>start to call BusZ01_1 向安捷发送交易结果查询。",__FILE__,__LINE__); 
	    iRet =BusZ0200_1(p_BcTransMsg,resp_code, p_tiBcTransInf,&tiBcInsInf,p_tiBcQsMchntInf, 2);
	    if ( iRet < 0 )
	    {
	        //发送给安捷失败
	        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ0200>转发给安捷交易结果查询失败,mchnt_cd:[%s],sys_tra_no:[%s].",__FILE__,__LINE__,p_BcTransLog->mchnt_cd, p_BcTransLog->sys_tra_no);
	        BC_SendExeception("03","转发给安捷交易结果查询失败,mchnt_cd:[%s],sys_tra_no:[%s].",p_BcTransLog->mchnt_cd, p_BcTransLog->sys_tra_no);
	        return -1;
	    }
	  }
    return -1;
}

//收到缴费请求
int BusZ00_ev1(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
	int iRet = 0;
	int sqlCode = 0 ;
	char resp_code[6+1]  ;
	char buff[1024]  ;
	int isExitFlag=0, ivrFlag;
	char vdate[17], cvv[17];
	
    //获取一些全局的变量，待用
    ti_bc_mt_flow_id_def tiBcMTFlowId ;
    ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //机构信息
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf = &g_tiBcQsMchntInf ; //清算商户信息
    ti_bc_route_card_bin_def *p_tiBcRouteCardBin = &g_tiBcRouteCardBinList;
    ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf = (ti_bc_mchnt_term_charge_inf_def *) &g_tiBcMchntTermChargeInf; //商户终端转换信息
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;
    strcpy(p_BcTransLog->flag_lock, "0");
    BC_SendMonitor_trans("0", "3", p_BcTransLog, p_tiBcTransInf);
	ISO_data t_iso;


    //获取#33域机构信息
    if( 0!= BC_Search_ins_inf(resp_code, p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd))
    {
    	// 查询接收机构线路信息失败
        sprintf(buff,"查询机构(%s)线路信息失败.", p_BcTransLog->fwd_ins_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00091", buff, 0);
		return ReturnPayMessageToRecive("N00091", iso, p_tiBcTransInf, p_BcTransLog);
    }

    //校验发送机构密钥
    if ( '0'  != *p_tiBcInsInf->key_type )
    {
        if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer + 4 , g_RcvBufferSize - 4) )
        {
            sprintf(buff, "校验接收机构(ins_id_cd:%s)密钥信息 !", p_tiBcInsInf->ins_id_cd);
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N000A0", buff, 0);
        	return ReturnPayMessageToRecive("N000A0", iso, p_tiBcTransInf, p_BcTransLog);
        }
    }

	iRet = ToGBK(p_BcTransLog->fld_48,g_TransMsg.len_48, buff,sizeof(buff)-1);
	if ( iRet>0 )
	{
		iRet = MIN(iRet,sizeof(p_BcTransLog->fld_48)-1);
		memcpy(p_BcTransLog->fld_48,buff, iRet);
		p_BcTransLog->fld_48[iRet] = 0;
		g_TransMsg.len_48 = iRet;
	}
	else
	{
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00091", "48域转GBK转码失败！", 0);
		return ReturnPayMessageToRecive("N00091", iso, p_tiBcTransInf, p_BcTransLog);
	}
    //账单入库
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tlBcBillLogOpr !",__FILE__,__LINE__ );
    isExitFlag = 0;
    BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
    iRet = tlBcBillLogOpr(GLB_DB_INSERT1, p_tlBcBillLog, &sqlCode);
    if ( SQL_ERD_UNIKEY == sqlCode )
    {
        //查询该账单交易状态
        tlBcBillLogOpr(GLB_DB_SELECT2, p_tlBcBillLog, &sqlCode);
        if ( 0 == sqlCode )
        {   
            //搜索到了该账单号有成功交易或待处理交易 NC0001
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            if ( '1' == *p_tlBcBillLog->bc_settle_in )
            {
	            sprintf(buff, "账单号重复(fwd_ins_id_cd:%s,bill_id:%s)。!", p_BcTransLog->fwd_ins_id_cd, p_tlBcBillLog->bill_id);
				ErrInfoInBcTransLog(__FILE__, __LINE__, "NC0001", buff, 0);
                strcpy(resp_code, "NC0001");
                isExitFlag = 1;
            } 
            else if ( '0' == *p_tlBcBillLog->bc_settle_in )
            {
                BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
                strcpy(p_tlBcBillLog->bc_settle_in, " ");
                iRet = tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
                if ( 0!= sqlCode )
                {
					sprintf(buff, "账单BILL入库失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
					ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
                    strcpy(resp_code, "N00096");
                    isExitFlag = 1;
                }
                //更新了状态，继续进行交易
            } 
            else
            {
                strcpy(resp_code, "N00009");
                isExitFlag = 1;
            }            
        } 
        else
        {
            //查询该账单交易状态失败
			sprintf(buff, "账单BILL查询失败.流水号:[%s],终端号:[%s],商户号:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
            strcpy(resp_code, "N00096");
            isExitFlag = 1;
        }
    }
    else if ( 0 != sqlCode )
    {
    	sprintf(buff, "账单BILL入库失败.流水号:[%s],终端号:[%s],商户号:[%s],sqlCode[%d]",p_BcTransLog->sys_tra_no, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd, sqlCode);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
        strcpy(resp_code, "N00096");
        isExitFlag = 1;
    } 
    else
		dcs_debug(0,0, "<FILE:%s,LINE:%d>tlBcBillLogOpr 入库成功",__FILE__,__LINE__); 
    //入库
    iRet = BC_InsertIntoDB(p_BcTransLog, &sqlCode);
    if ( 0 != iRet )
    {
        if( sqlCode == SQL_ERD_UNIKEY ){
            strcpy(resp_code, "N00094");
        } else{
            strcpy(resp_code, "N00096");
        }
    	sprintf(buff, "非绑定用户请求入库失败.流水号:[%s],终端号:[%s],商户号:[%s], sqlCode[%d]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd, sqlCode);
		ErrInfoInBcTransLog(__FILE__, __LINE__, resp_code, buff, 0);
        isExitFlag = 1;
    } else
        dcs_debug(0,0, "<FILE:%s,LINE:%d>BC_InsertIntoDB 入库成功",__FILE__,__LINE__);

    //判断是否需要退出
    if( isExitFlag )
    {
		tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		return ReturnPayMessageToRecive(resp_code, iso, p_tiBcTransInf, p_BcTransLog);
    }
   
    // 获取商户信息，area_cd
    strcpy(p_BcTransLog->area_cd, p_tiBcMchntInf->area_cd);

    //检查商户转换信息表
    iRet = BC_GetMchntTransInf( resp_code, p_tiBcMchntTermChargeInf,  p_BcTransLog->mchnt_tp, p_BcTransLog->term_id, p_BcTransLog->mchnt_cd,g_rtIns_id_cd );
    if( 0 != iRet )
    {
        // 查询商户转换信息表失败
        sprintf(buff,"查询商户转换信息失败,mchnt_cd:[%s].",p_BcTransLog->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00003", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		return ReturnPayMessageToRecive("N00003", iso, p_tiBcTransInf, p_BcTransLog);
    }

    //终端号，商户号
    strcpy(p_BcTransLog->center_term_id   ,  p_tiBcMchntTermChargeInf->term_id_c);
    strcpy(p_BcTransLog->center_mchnt_cd  ,  p_tiBcMchntTermChargeInf->mchnt_cd_c);

    // 获取清算商户信息
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_tiBcMchntTermChargeInf->mchnt_cd_c);
    strcpy(p_tiBcQsMchntInf->term_id, p_tiBcMchntTermChargeInf->term_id_c);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_tiBcMchntTermChargeInf->rcv_ins_id_cd);
    iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        sprintf(buff,"查询清算商户信息失败,mchnt_cd:[%s].",p_tiBcMchntTermChargeInf->mchnt_cd_c);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00003", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		return ReturnPayMessageToRecive("N00003", iso, p_tiBcTransInf, p_BcTransLog);
    } 
    
    //搜索接收机构信息
    strcpy(p_BcTransLog->rcv_ins_id_cd, g_rtIns_id_cd);
    if( 0!= BC_Search_ins_inf(resp_code, &tiBcInsInf, g_rtIns_id_cd))
    {
    	// 查询接收机构线路信息失败
		sprintf(buff, "查询机构线路信息失败,机构id:[%s].", g_rtIns_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00091", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		return ReturnPayMessageToRecive("N00003", iso, p_tiBcTransInf, p_BcTransLog);
    }
    if(tiBcInsInf.link_tp[0] == '1')
    {
    	rtrim(tiBcInsInf.line_nm);
    	if ( IsReady( tiBcInsInf.line_nm )<0 )
    	{
    		sprintf(buff, "the foldName [%s] isn't online ", tiBcInsInf.line_nm );
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
	        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
			return ReturnPayMessageToRecive("N00096", iso, p_tiBcTransInf, p_BcTransLog);
    	}
    }
    //check field
    if ( 0 != BC_CheckIsoDataField(iso, "2,3,4,5,8,11,12,13,42,48,61,64") )
    {
		sprintf(buff, "doBus_CheckIsoDataField failure.");
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00030", buff, 1);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		return ReturnPayMessageToRecive("N00030", iso, p_tiBcTransInf, p_BcTransLog);
    }
	//处理信用卡信息
	if(IsCreditCard(p_BcTransLog->card_attr))//信用卡
	{
		rtrim(p_BcTransLog->encrypt_info);
		if(strlen(p_BcTransLog->encrypt_info) == 32)
		{
			if(0>decryptInfo(cvv, vdate, p_BcTransLog->encrypt_info, p_tiBcInsInf, 0, 1))
			{
				sprintf(buff, "解密ivr,VALIDDATA应答报文信息失败!(应答码:%s)", resp_code);
				ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
	        	tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		    	return ReturnPayMessageToRecive("N00096", iso, p_tiBcTransInf, p_BcTransLog); 
			}
			EncryptInfo(cvv, vdate, p_BcTransLog->encrypt_info);
			ivrFlag = 0;
	    	strcpy(p_BcTransLog->flag_6, "0" );
		}
		else if(*p_tiBcRouteCardBin->get_inf_flag =='0')
		{
	    	strcpy(p_BcTransLog->flag_6, "2" );
	    	strcpy(cvv, "999");
	    	strcpy(vdate, "1299");
			ivrFlag = 0;
		}
		else
		{
	    	strcpy(p_BcTransLog->flag_6, "1" );
			ivrFlag = 1;
		}
	}
	else
	{
    	strcpy(p_BcTransLog->flag_6, "1" );
		ivrFlag = 1;		
	}    
    //更新库标记
    strcpy(p_BcTransLog->trans_flag, "B"); //设置当前状态为第一次IVR获取
    strcpy(p_BcTransLog->flag_verifi, "2");  //正在发送
    memset(p_BcTransLog->line_nm_verifi, 0, sizeof(p_BcTransLog->line_nm_verifi));
    fold_get_name( g_IVR_FOLDER_ID, p_BcTransLog->line_nm_verifi, sizeof(p_BcTransLog->line_nm_verifi)-1);
    strcpy_safe(p_BcTransLog->line_nm_rcv, tiBcInsInf.line_nm, sizeof(p_BcTransLog->line_nm_rcv)-1);

	//发送给IVRS
	if(ivrFlag)
	{
		iRet = BusIVR_SendIvr(p_BcTransLog, p_tiBcMchntInf->mchnt_cn_abbr);
		if ( iRet < 0 )
		{
			strcpy(p_BcTransLog->flag_verifi, "3");
			sprintf(buff, "send to IVR failure");
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);				
	        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
			return ReturnPayMessageToRecive("N00096", iso, p_tiBcTransInf, p_BcTransLog);
		}
	}
	else
	{
		//获取转换流水号
		strcpy(tiBcMTFlowId.term_id   ,  p_BcTransLog->center_term_id);
		strcpy(tiBcMTFlowId.mchnt_cd  ,  p_BcTransLog->center_mchnt_cd);
		if(0 != tiBcMTFlowIdOpr(GLB_DB_SELECT1, &tiBcMTFlowId, &sqlCode))
		{
			sprintf(buff, "获取转换流水号失败,中心商户号:[%s].中心终端号[%s].", p_BcTransLog->center_mchnt_cd, p_BcTransLog->center_term_id);
			ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
			return SendPayResultToRecive("N00096");
		}
		strcpy(p_BcTransLog->center_sys_tra_no,  tiBcMTFlowId.sys_tra_no);
		strcpy(p_BcTransLog->trans_flag, "C");
		if(0 == memcmp(p_BcTransLog->rcv_ins_id_cd, INS_HFTX, 6) || 0 == memcmp(p_BcTransLog->rcv_ins_id_cd, INS_YBZF, 6))
		{
			if(0 > SendPayMessageToJAVA(&t_iso, &tiBcInsInf, p_BcTransLog, vdate, cvv, 0))
			{
				ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", "发送到JAVA前置失败！", 1);
	        	tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
				return ReturnPayMessageToRecive("N00096", iso, p_tiBcTransInf, p_BcTransLog); 
			}
		}
		else if(memcmp(p_BcTransLog->rcv_ins_id_cd, INS_YL, 6) == 0)
		{
			if(0 > ExpendApp_YL_Credit(&t_iso, p_BcTransLog, 0))
			{
				ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", "发送到银联前置交易失败！", 1);
	        	tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
				return ReturnPayMessageToRecive("N00096", iso, p_tiBcTransInf, p_BcTransLog); 
			}	
		}
	}
	//保存到数据库
	iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
		sprintf(buff, "tlBcTransLogOpr 更新失败!");
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 0);
        tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
		return ReturnPayMessageToRecive("N00096", iso, p_tiBcTransInf, p_BcTransLog);
    }
    //应答给受理方
    return ReturnPayMessageToRecive("N00000", iso, p_tiBcTransInf, p_BcTransLog);
}

//返回应答报文0860到接收 
int ReturnPayMessageToRecive(char *resp_code, ISO_data *iso, ti_bc_trans_inf_def *p_tiBcTransInf, tl_bc_trans_log_def *p_BcTransLog)
{
	setbit_t(iso, 0, "0860", 4);
	setbit_t(iso,28, resp_code, 6);
	setbit_t(iso,64, "00000000", 8);
	setbitoff(iso, 48);
	BC_SetRcvMacKey(&g_tiBcInsInf);
	BC_SendMonitor_iso("1", "0", p_BcTransLog->trans_chnl, iso, p_tiBcTransInf);
	BC_SendMessageToSrc(iso);
	return 0;
}

int SendPayResultToRecive_Msg(char *resp_code, char *msg)
{
	int iRet = 0;
	int sqlCode = 0 ;
	ISO_data iso; 
	ISO_data *p_iso=&iso; 
	ti_bc_ins_inf_def tiBcInsInf; // 接收机构的机构信息
	ti_bc_mchnt_inf_def  tiBcMchntInf ;
	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog; // from db
	char buff[1024],retLen;
	 //组建报文
	memset(&tiBcInsInf, 0, sizeof(tiBcInsInf));
	memset(&tiBcMchntInf, 0, sizeof(tiBcMchntInf));
	memset(p_iso, 0, sizeof(ISO_data));
	ti_bc_mchnt_inf_def *p_tiBcMchntInf;
	ti_bc_ins_inf_def *p_tiBcInsInf;
	clearbit(p_iso);
  //搜索机构信息
	p_tiBcInsInf = &tiBcInsInf ; 
	if( 0!= BC_Search_ins_inf(buff, p_tiBcInsInf, p_BcTransLog->fwd_ins_id_cd))
	{
		sprintf(buff, "搜索机构信息[%s]失败!", p_BcTransLog->fwd_ins_id_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		return -1;
	}
  //搜索商户信息
	p_tiBcMchntInf = &tiBcMchntInf ; 
	strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog->mchnt_cd);
	iRet = tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
	if( 0 != sqlCode )
	{
		sprintf(buff, "搜索商户信息[%s]失败!", p_tiBcMchntInf->mchnt_cd);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		return -1;
	}
	if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
	{
		sprintf(buff, "设置ISO8583包类型[%s]失败!", p_tiBcInsInf->packet_type);
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00096", buff, 1);
		return -1;
	}
	setbit_t(p_iso, 0 , "0870", 4); 
	rtrim(p_BcTransLog->pri_acct_no);
	setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
	setbit_t(p_iso, 3 , "811001", 6);
	sprintf(buff, "%.12d", atoi(p_BcTransLog->trans_at) );
	setbit_t(p_iso, 4 , buff, 12);
	setbit_t(p_iso, 5 , (unsigned char *)p_BcTransLog->fld_5, min_strlen(p_BcTransLog->fld_5,12));
	setbit_t(p_iso, 8 , (unsigned char *)p_BcTransLog->fld_8, min_strlen(p_BcTransLog->fld_8,8));
	setbit_t(p_iso, 11, (unsigned char *)p_BcTransLog->sys_tra_no, 6);
	setbit_t(p_iso, 12, (unsigned char *)p_BcTransLog->loc_trans_tm, 6);
	setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
	if(memcmp(resp_code,"N00000",6)==0)//交易成功
	{
		setbit_t(p_iso, 15, (unsigned char *)p_BcTransLog->settle_dt, 4);
		setbit_t(p_iso, 37, (unsigned char *)p_BcTransLog->retri_ref_no, 12);
	}
	setbit_t(p_iso, 28, (unsigned char *)resp_code, 6);
	setbit_t(p_iso, 33, (unsigned char *)p_BcTransLog->fwd_ins_id_cd, 8);
	setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog->mchnt_cd, 15);
	if(msg != NULL)
	{	
		rtrim(msg);
//		dcs_log(0,0,"msg=%s,%d", msg, strlen(msg));
		retLen = GBKToUTF8(msg, strlen(msg), buff, sizeof(buff));
		setbit_t(p_iso, 44, buff, MIN(retLen, 99));
	}
	if ( 0 != *p_BcTransLog->fld_45 )
	{
		strcpy(buff, p_BcTransLog->fld_45);
		rtrim(buff);
		setbit_t(p_iso, 45 , (unsigned char *)buff, strlen(buff));
	}
	setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog->trans_curr_cd, 3);
	setbit_t(p_iso, 64, "00000000", 8);
	//发送监控信息
	BC_SetMacKey(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1);
	BC_SendMonitor_iso("0","0",p_BcTransLog->trans_chnl,p_iso,&g_tiBcTransInf);
   	tlBcBillLogUpdate(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);     
	return BC_SendMessage(p_iso, p_BcTransLog->trans_chnl );
}

//发送支付结果0870到接收方
int SendPayResultToRecive(char *resp_code)
{
	SendPayResultToRecive_Msg(resp_code, NULL);
}
