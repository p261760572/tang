#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"
#include  "CMDI.h"

/*��ʱ�Ĵ���*/
int process_cmdi_timeout( char* srcBuf,int  iFid, int iLen )
{
    int iRet;
    int sqlCode;
    char msg_tp[4+1];
    char proc_cd[6+1];
    char procedure[30];
    char msg_tp_new[4+1];
    char buff[256];

    struct TimeOut_REC *pTimeOutRec = &g_TimeOutRec;        
    TimeOut_Key *tk = (TimeOut_Key *)pTimeOutRec->caKey;
    TimeOut_ReMark *trm = (TimeOut_ReMark *)pTimeOutRec->caRemark;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;

    memcpy(&g_TimeOutRec, srcBuf, sizeof(g_TimeOutRec));    

    //key
    memset(&g_TransMsg_o, 0, sizeof(BcTransMsg));
    memcpy(p_BcTransLog_o->sys_tra_no,  trm->sys_tra_no, 6);
    memcpy(p_BcTransLog_o->loc_trans_dt,  trm->loc_trans_dt, 4);
    memcpy(p_BcTransLog_o->term_id,  trm->term_id, 8);
    memcpy(p_BcTransLog_o->mchnt_cd,  trm->mchnt_cd, 15);

    dcs_log(0,0,"<FILE:%s,LINE:%d><process_cmdi_timeout>start to call tlBcTransLogOpr ����ԭʼ������Ϣ��sys_tra_no:%s,loc_trans_dt:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->loc_trans_dt,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
    tlBcTransLogOpr(GLB_DB_SELECT11, p_BcTransLog_o, &sqlCode);
    if ( 0 != sqlCode )
    {
        BC_SendExeception("02","��ѯ������־��Ϣʧ�ܣ�δ�ҵ�Ӧ���׵���������Ϣ.sys_tra_no:%s,loc_trans_dt:%s,term_id:%s,mchnt_cd:%s\n",p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->loc_trans_dt,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);

        //δ��ѯ�����ݿ��¼�����¼�����������ڱ�������
        dcs_log(p_BcTransLog_o,sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d><process_cmdi_timeout>�����ݿ���δ�ҵ��ý��ף�sys_tra_no:%s,loc_trans_dt:%s,term_id:%s,mchnt_cd:%s\n",__FILE__,__LINE__,p_BcTransLog_o->sys_tra_no,p_BcTransLog_o->loc_trans_dt,p_BcTransLog_o->term_id,p_BcTransLog_o->mchnt_cd);
        return -1;
    }

    //��¼������
    *g_TransLock = '1';

    //copy trans
    memcpy(&g_TransMsg, &g_TransMsg_o, sizeof(BcTransMsg));
  
    //�õ���Ӧ�õõ���Ӧ���׵���Ϣ����
    memset(msg_tp_new, 0, sizeof(msg_tp_new) );
    memset(buff, 0, sizeof(buff));
    memcpy(buff, trm->msg_tp, 4);
    BC_StringAdder(buff, 10, msg_tp_new, 4);
    
     // ����ԭʼ���״�����
	   iRet = BC_Search_Trans_det(&g_tiBcTransDet,
	     "TIMEOUT"   ,    
	     p_BcTransLog_o->msg_tp       ,        
	     p_BcTransLog_o->proc_cd      ,       
	     p_BcTransLog_o->pos_cond_cd  ,
	     p_BcTransLog_o->mchnt_tp     , 
	     msg_tp_new                   , //new msg_tp
	     p_BcTransLog_o->trans_flag   ,
	     p_BcTransLog_o->rcv_ins_id_cd
	  );
	      
	  if( 0 != iRet )
	  {
	    BC_SendExeception("02","<FILE:%s,LINE:%d>δ�ҵ��ý��׵ĺ�������ʽ���������ñ�ti_bc_trans_det:trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    dcs_log(0,0,"<FILE:%s,LINE:%d>δ�ڱ�ti_bc_trans_det���ҵ��ý��׵Ĵ���ʽ����ʱ������trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s],msg_tp_new:[%s].\n",__FILE__,__LINE__,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
	    BC_UnLockTrans(p_BcTransLog_o);
	    	    
	    //����
	    return -1;
	  }
	  
	  dcs_debug(0,0,"msg_tp_new:%s.\n", msg_tp_new);
	  
	  memset(procedure, 0, sizeof(procedure) );
	  strcpy(procedure, g_tiBcTransDet.procedure); 
    rtrim(procedure);
    dcs_log(0,0,"<FILE:%s,LINE:%d>��ʱ���ײ��ҵ�����ʽ[%s]trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s,msg_tp_new:%s, procedure:%s.\n",__FILE__,__LINE__,g_tiBcTransDet.procedure,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new, procedure);
    
    //�˴�Ϊ���н��׳�ʱ��Ĵ������
    if ( 0 );
    ON_BC_TIMEOUT("Z2211"  , BusZ2210_T)       //�ǰ��û�֧��
    //IVR
    ON_BC_TIMEOUT("Z22IVR_T", BusIVR_Z22_T) //�ǰ��û�֧������Ӧ��ʱ����
    //������
    ON_BC_TIMEOUT("Z22G4"  , BusGZ_Z22_ev4_T)  //�ǰ��û�֧��������Ӧ��ʱ����
    ON_BC_TIMEOUT("Z22G4_R", BusGZ_Z22_ev4_RT) //�ǰ��û�֧������Ӧ��ʱ����
//	ON_BC_TIMEOUT("Z22G_I_T",BusGZ_Z22_I_T)   
    //�㸶
    ON_BC_TIMEOUT("Z22H4"  , BusHF_Z22_RT)  //�ǰ��û�֧���㸶Ӧ��ʱ����
//    ON_BC_TIMEOUT("Z22J_I_T", BusJV_Z22_I_T)
    //CUPS
    ON_BC_TIMEOUT("Z22P1_R_T"  , CUPS_ExpendAppTimeOut)  //POSP���ѹ�����Ӧ��ʱ����
    ON_BC_TIMEOUT("Z22P2_R_T"  , CUPS_ExpendUndoTimeOut)  //POSP���ѳ���������Ӧ��ʱ����
    ON_BC_TIMEOUT("Z22P1_R_RT" , CUPS_ExpendAppRoolBackTimeOut)  //POSP���ѳ�����ʱ������Ӧ��ʱ����
    ON_BC_TIMEOUT("Z22P2_R_RT" , CUPS_ExpendUndoRollBackTimeOut)  //POSP���ѳ���������ʱ������Ӧ��ʱ����
    ON_BC_TIMEOUT("Z22P3_R_T"  , CUPS_BalanceQueryTimeOut)  //POSP���ѳ���������ʱ������Ӧ��ʱ����
    //SMS
    ON_BC_TIMEOUT("Z22S1_R_T"  , SMS_SendTimeOut)  //���Ͷ���ϢӦ��ʱ����
    //�����ܲ�
    ON_BC_TIMEOUT("Z22Y3_R_T1"  , BusYL_Z22_ev3_T1)  //ͬ��Ӧ��ʱ
    ON_BC_TIMEOUT("Z22Y3_R_T2"  , BusYL_Z22_ev3_T2)  //�첽Ӧ��ʱ
    
    //δ�ҵ��ý��׵Ĵ���ʽ
    ELSE
    {
    	  //û��ƥ�䵽
        dcs_log(0,0,"<FILE:%s,LINE:%d><process_cmdi_timeout>�ý��׺�������ʽ[%s]�����ڣ�trans_chnl:%s,msg_tp:%s,proc_cd:%s,pos_cond_cd:%s,mchnt_tp:%s,msg_tp_new:%s.\n",__FILE__,__LINE__,procedure,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp,g_tiBcTransDet.msg_tp_new);
        BC_SendExeception("02","��ʱ�����׵ĺ�������ʽ[%s]�����ڣ��������ñ�trans_chnl:[%s],msg_tp:[%s],proc_cd:[%s],pos_cond_cd:[%s],mchnt_tp:[%s].",g_tiBcTransDet.procedure,g_tiBcTransDet.trans_chnl,g_tiBcTransDet.msg_tp,g_tiBcTransDet.proc_cd,g_tiBcTransDet.pos_cond_cd,g_tiBcTransDet.mchnt_tp);
        BC_UnLockTrans(p_BcTransLog_o);
        
        //����
        return -1;
    }
    return 0;
}

int SIGNIN001( ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf  )
{
	int iRet, sqlCode, iLen;
	char buff[1024];
	ISO_data iso ;
	ISO_data *p_iso = &iso ;
	char resp_code [6+1];
	
	ti_bc_ins_inf_def *p_tiBcInsInf= &g_tiBcInsInf;
	
	//search ins
	memset( p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcQsMchntInf->rcv_ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
    	return -1;
    }
    
  //packet
	memset(p_iso,0,sizeof(ISO_data));
  clearbit(p_iso);
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        return -1;
    }
    
    setbit_t(p_iso, 0 , "0520", 4); 
    setbit_t(p_iso, 3 , "510000", 6);
    setbit_t(p_iso, 42, (unsigned char *)p_tiBcQsMchntInf->mchnt_cd, strlen(p_tiBcQsMchntInf->mchnt_cd) );
    
    BC_SetMacKey(0,0);
    memset(buff, 0, sizeof(buff) );
    PrintISO( p_iso,"���Ͱ����̻�ǩ��������",0);
    
    //���ͼ����Ϣ
    BC_SendMonitor_iso("0","0",p_tiBcInsInf->line_nm, p_iso, 0);
    
    iLen = BC_SendMessageToAJ(p_iso, buff, p_tiBcInsInf);
    if ( iLen <= 0 )
    {
        //���͸�����ʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><SIGNIN001>send to rcv failure.",__FILE__,__LINE__);
        return -1;
    }

    //���
    //to iso
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0!= BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        dcs_log(0, 0,"<FILE:%s,LINE:%d><SIGNIN001>BC_SetIsoMode failure.",__FILE__,__LINE__);
        return -1;
    }
   
    iRet = BC_UnPack_Data( p_iso, buff,  iLen);
    PrintISO( p_iso,"�յ������̻�ǩ��Ӧ����",0 );

    //���͸�����ʧ��
    dcs_debug(0, 0,"<FILE:%s,LINE:%d><SIGNIN001>�Ѿ��յ������̻�ǩ���ظ�����.",__FILE__,__LINE__);
    if ( iRet >= 0 )
    {
        BC_SendMonitor_iso("1","2",p_tiBcInsInf->line_nm,p_iso, 0);
        
        //����ɹ�
        if ( (iRet = getbit(p_iso, 0, buff)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #0 failure��",__FILE__,__LINE__);
            return -1;
        }
        if ( (iRet = getbit(p_iso, 3, buff+4)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #3 failure��",__FILE__,__LINE__);
            return -1;
        }
        if ( (iRet = getbit(p_iso, 42, buff+20)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #42 failure��",__FILE__,__LINE__);
            return -1;
        }

        //check
        if ( 0 != memcmp(buff, "0530510000", 10) 
             || 0 != memcmp(buff+20, p_tiBcQsMchntInf->mchnt_cd,15)
           )
        {
            dcs_log(buff,50,"<FILE:%s,LINE:%d><SIGNIN001>check #0 #3 #42(%s) failure��",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd);
            return -1;
        }

        //return code
        if ( (iRet = getbit(p_iso, 28, buff)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #28 failure��",__FILE__,__LINE__);
            return -1;
        }

        memcpy(resp_code, buff, 6);
        resp_code[6] = 0;
        dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>return #28[%s]��",__FILE__,__LINE__, resp_code);
        if ( 0 != memcmp(resp_code, "N00000", 6) )
        {
        	return -1;
        }

        //mac
        if ( (iRet = getbit(p_iso, 48, buff)) <= 0 )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>get #48 failure��",__FILE__,__LINE__);
            return -1;
        }

        //update key
        dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>start to call tiBcMchntInfOpr update key .",__FILE__,__LINE__);
        memcpy(p_tiBcQsMchntInf->mkey_1, buff, 16);
        *(p_tiBcQsMchntInf->mkey_1 + 16) = 0;
        iRet = tiBcQsMchntInfOpr(GLB_DB_UPDATE2, p_tiBcQsMchntInf, &sqlCode);
        if ( 0!= sqlCode )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><SIGNIN001>call tiBcMchntInfOpr update key failure��",__FILE__,__LINE__);
            //������Կʧ��
            return -1;
        }
        return 0;
    }
    
	return -1;
}

int SIGNIN002( ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf  )
{
	int iRet, sqlCode, iLen;
	ISO_data iso ;
	ISO_data *p_iso = &iso ;
	char resp_code [6+1];
	
	ti_bc_ins_inf_def *p_tiBcInsInf= &g_tiBcInsInf;
	
	//search ins
	memset( p_tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy(p_tiBcInsInf->ins_id_cd, p_tiBcQsMchntInf->rcv_ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
    	return -1;
    }
    
  //packet
	memset(p_iso,0,sizeof(ISO_data));
  clearbit(p_iso);
    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        return -1;
    }
    
    setbit_t(p_iso, 0 , "0800", 4); 
    setbit_t(p_iso, 3 , "510000", 6);
    setbit_t(p_iso, 42, (unsigned char *)p_tiBcQsMchntInf->mchnt_cd, strlen(p_tiBcQsMchntInf->mchnt_cd) );
    
    BC_SetRcvMacKey(p_tiBcInsInf);
    
    PrintISO( p_iso,"�����̻�ǩ��������",0);
    
    //���ͼ����Ϣ
    BC_SendMonitor_iso("0","0",p_tiBcInsInf->line_nm, p_iso, 0);
    
    return BC_SendMessage(p_iso, p_tiBcInsInf->line_nm );
}

int mchntSignIn(char *param)
{
	int sqlCode;
	char procedure[16];
	ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ;
	ti_bc_trans_det_def *p_tiBcTransDet = &g_tiBcTransDet;
	
	memset(p_tiBcQsMchntInf, 0, sizeof(ti_bc_qs_mchnt_inf_def));
	strcpy_safe(p_tiBcQsMchntInf->mchnt_cd, param, 20 );
	strcpy_safe(p_tiBcQsMchntInf->term_id, param+20, 8 );
	strcpy_safe(p_tiBcQsMchntInf->rcv_ins_id_cd, param+28, 8 );
	
	tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
  if ( 0 != sqlCode )
  {
  	//û���ҵ��̻���Ϣ
  	dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210_T>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd,sqlCode );
    
    //����ʧ��
    return -1;
  }
  
    strcpy(p_tiBcTransDet->trans_chnl    , "*");
    strcpy(p_tiBcTransDet->msg_tp        , "0520");
    strcpy(p_tiBcTransDet->proc_cd       , "*");
    strcpy(p_tiBcTransDet->pos_cond_cd   , "*");
    strcpy(p_tiBcTransDet->mchnt_tp      , "*");
    strcpy(p_tiBcTransDet->msg_tp_new    , "*"); //new msgtp
    strcpy(p_tiBcTransDet->trans_flag    , "*" );
    strcpy(p_tiBcTransDet->rcv_ins_id_cd , p_tiBcQsMchntInf->rcv_ins_id_cd );
    
    //������Ϣ���ͣ������룬����������룬�����ױ�ʶ��ȷ�����״������    
    tiBcTransDetOpr(GLB_DB_SELECT1, p_tiBcTransDet, &sqlCode);
    if ( 0 != sqlCode ){
    	dcs_debug(0,0,"<FILE:%s,LINE:%d>����tiBcTransDetOpr��ѯ������ʧ��(msg_tp:0520, rcv_ins_id_cd:%s).\n",__FILE__,__LINE__,p_tiBcTransDet->rcv_ins_id_cd);
    	return -1;
    }
    
    strcpy(procedure, p_tiBcTransDet->procedure); 
    rtrim(procedure);
    
    if( 0 == strcmp(procedure, "SIGNIN001") ){
    	//����ǩ��
    	return SIGNIN001( p_tiBcQsMchntInf );
    }else if( 0 == strcmp(procedure, "SIGNIN002") ){
    	//�㶫����
    	return SIGNIN002(p_tiBcQsMchntInf);
    } 
    
	return -1;	
}

