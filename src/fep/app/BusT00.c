#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"


/************************************
 *
 *        Ç©µ½´¦Àí req
 *
************************************/
int Bus_SignInReq_term(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{

    int iRet = -1;
    int sqlCode ;
    char resp_cd[3];
    strcpy(resp_cd,"96");
	char buff[256];

	tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_term_inf_def *p_tiBcTermInf= &g_tiBcTermInf;

    BC_SendMonitor("0","3",p_BcTransLog->trans_chnl,iso);

	if( 0 != BC_CheckIsoDataField(iso, "11,41,42") )
	{
		dcs_log(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>doBus_CheckIsoDataField failure.",__FILE__,__LINE__);
		BC_StringAdder(p_tiBcTransDet->msg_tp,10,buff,4);
		setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"97",2 );
        BC_SetTermMacKey(0,0);
		strcpy(p_BcTransLog->resp_cd,"97");
		BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,iso);
        return BC_SendMessageToTerm(iso);
	}
		
	//search term info
	dcs_debug(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>start to call tiBcTermInfOpr for select.",__FILE__,__LINE__,RPT_TO_LOG,0,0);
	memset( p_tiBcTermInf, 0, sizeof(ti_bc_term_inf_def) );
	strcpy(p_tiBcTermInf->term_id , g_TransMsg.BcTransLog.term_id);
	iRet = tiBcTermInfOpr(GLB_DB_SELECT1, p_tiBcTermInf, &sqlCode);
	
	if( 0 != sqlCode)
	{
		dcs_log(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>tiIseTermInfOpr search term info failure[sqlcode:%d].",__FILE__,__LINE__,sqlCode);
		BC_StringAdder(p_tiBcTransDet->msg_tp,10,buff,4);
		setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"97",2 );
        BC_SetTermMacKey(0,0);
		strcpy(p_BcTransLog->resp_cd,"97");
		BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,iso);
        return BC_SendMessageToTerm(iso);
	}

	if( '0' == *p_tiBcTermInf->openflag )
	{
		dcs_log(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>term openflag is closed.",__FILE__,__LINE__);
		BC_StringAdder(p_tiBcTransDet->msg_tp,10,buff,4);
		setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"93",2 );
        BC_SetTermMacKey(0,0);
		strcpy(p_BcTransLog->resp_cd,"93");
		BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,iso);
        return BC_SendMessageToTerm(iso);
	}

	//GET KEY
	dcs_debug(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>start to first call DESGETKY.",__FILE__,__LINE__);
	DESGETKY(resp_cd,p_tiBcTermInf->mkey_1,p_tiBcTermInf->mkey_index);
	if( 0 != memcmp( resp_cd, "00", 2) )
	{
		dcs_log(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>DESGETKY failure.",__FILE__,__LINE__);
		BC_StringAdder(p_tiBcTransDet->msg_tp,10,buff,4);
		setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"96",2 );
        BC_SetTermMacKey(0,0);
		strcpy(p_BcTransLog->resp_cd,"96");
		BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,iso);
        return BC_SendMessageToTerm(iso);
	}

	dcs_debug(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>start to second call DESGETKY.",__FILE__,__LINE__);
	DESGETKY(resp_cd,p_tiBcTermInf->mkey_2,p_tiBcTermInf->mkey_index);
	if( 0 != memcmp( resp_cd, "00", 2) )
	{
		dcs_log(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>DESGETKY failure.",__FILE__,__LINE__);
		BC_StringAdder(p_tiBcTransDet->msg_tp,10,buff,4);
		setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"96",2 );
        BC_SetTermMacKey(0,0);
		strcpy(p_BcTransLog->resp_cd,"96");
		BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,iso);
        return BC_SendMessageToTerm(iso);
	}

	dcs_debug(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>start to third call DESGETKY.",__FILE__,__LINE__);
	DESGETKY(resp_cd,p_tiBcTermInf->mkey_3,p_tiBcTermInf->mkey_index);
	if( 0 != memcmp( resp_cd, "00", 2) )
	{
		dcs_log(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>DESGETKY failure.",__FILE__,__LINE__);
		BC_StringAdder(p_tiBcTransDet->msg_tp,10,buff,4);
		setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"96",2 );
        BC_SetTermMacKey(0,0);
		strcpy(p_BcTransLog->resp_cd,"96");
		BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,iso);
        return BC_SendMessageToTerm(iso);
	}

	//update term info
	dcs_debug(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>start to call tiIseTermInfOpr for update mackey.",__FILE__,__LINE__);
	tiBcTermInfOpr(GLB_DB_UPDATE2, p_tiBcTermInf, &sqlCode);
	if( 0 != sqlCode)
	{
		dcs_log(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>tiBcTermInfOpr update term info failure.",__FILE__,__LINE__);
		BC_StringAdder(p_tiBcTransDet->msg_tp,10,buff,4);
		setbit_t(iso,0,buff,4 );
        setbit_t(iso,39,"96",2 );
        BC_SetTermMacKey(0,0);
		strcpy(p_BcTransLog->resp_cd,"96");
		BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,iso);
        return BC_SendMessageToTerm(iso);
	}	
	
	BC_StringAdder(p_tiBcTransDet->msg_tp,10,buff,4);
	setbit_t(iso,0,buff,4 );
    setbit_t(iso,39,"00",2 );

	dcs_debug(0,0,"<FILE:%s,LINE:%d><Bus_SignInReq_term>start to call BC_SetTermMacKey.",__FILE__,__LINE__);
	BC_SetTermMacKey(p_tiBcTermInf->mkey_index,p_tiBcTermInf->mkey_1);

	strcpy(p_BcTransLog->resp_cd,"00");
	BC_SendMonitor("1","0",p_BcTransLog->trans_chnl,iso);
    return BC_SendMessageToTerm(iso);
}

//
