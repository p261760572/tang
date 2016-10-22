#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

char* getIdStr(char *idType, char *idStr);

/*�ɷ�����*/
int BusZ_BillCopy(tl_bc_trans_log_def *p_BcTransLog, tl_bc_bill_log_def *pBcBillLog)
{
    #define BusZ_BillCopy_MEMCPY(X,Y) memcpy(X,Y, min(sizeof(X),sizeof(Y)));
    memset(pBcBillLog, 0, sizeof(tl_bc_bill_log_def) );

    //copy
    BusZ_BillCopy_MEMCPY(pBcBillLog->bc_settle_in       , p_BcTransLog->bc_settle_in        )
    BusZ_BillCopy_MEMCPY(pBcBillLog->trans_at           , p_BcTransLog->trans_at            )
    BusZ_BillCopy_MEMCPY(pBcBillLog->mobile_no          , p_BcTransLog->fld_5               )
    BusZ_BillCopy_MEMCPY(pBcBillLog->sys_tra_no         , p_BcTransLog->sys_tra_no          )
    BusZ_BillCopy_MEMCPY(pBcBillLog->loc_trans_tm       , p_BcTransLog->loc_trans_tm        )
    BusZ_BillCopy_MEMCPY(pBcBillLog->loc_trans_dt       , p_BcTransLog->loc_trans_dt        )
    BusZ_BillCopy_MEMCPY(pBcBillLog->settle_dt          , p_BcTransLog->settle_dt           )
    BusZ_BillCopy_MEMCPY(pBcBillLog->resp_code          , p_BcTransLog->fld_28              )
    BusZ_BillCopy_MEMCPY(pBcBillLog->fwd_ins_id_cd      , p_BcTransLog->fwd_ins_id_cd       )
    BusZ_BillCopy_MEMCPY(pBcBillLog->retri_ref_no       , p_BcTransLog->retri_ref_no             )
    BusZ_BillCopy_MEMCPY(pBcBillLog->resp_cd            , p_BcTransLog->resp_cd             )
    BusZ_BillCopy_MEMCPY(pBcBillLog->mchnt_cd           , p_BcTransLog->mchnt_cd            )
    BusZ_BillCopy_MEMCPY(pBcBillLog->bill_id            , p_BcTransLog->fld_45              )
    BusZ_BillCopy_MEMCPY(pBcBillLog->bill_id_rcv        , p_BcTransLog->bill_id_rcv         )
    return 0;
}

//���Ͱ��ݷǰ��˵�֧������
// resp_flag Ӧ�� 0 Ϊ��ϵͳ
int BusZ2210_1(BcTransMsg *p_BcTransMsg,int *resp_flag,char *resp_code, 
               ti_bc_trans_inf_def *p_tiBcTransInf,
               ti_bc_ins_inf_def *p_tiBcInsInf , 
               ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf,
               ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf)
{
    int iLen;
    int iRet;
    int sqlCode;
    char buff[2048],temp[256];
    char packet_type[5];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ISO_data iso;
    ISO_data *p_iso = &iso ;
    *resp_flag = 1;

    //�齨����
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0 != BC_SetIsoMode(p_iso, p_tiBcInsInf->packet_type) )
    {
        *resp_flag = 0;
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ2210_1>BC_SetIsoMode failure(ins_id_cd:%s,packet_type:%s)��",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd, p_tiBcInsInf->packet_type);
        BC_TRACE_END( -1 );
    }

    //ת�����룬ע�ⳤ�ȣ�����strlen���㣬�Է���������
    iLen = GBKToUnicode_2(p_BcTransLog->fld_48,g_TransMsg.len_48,buff,sizeof(buff)-1);
    if ( iLen >= 0 )
    {
        buff[iLen] = 0;
    }

    setbit_t(p_iso, 0 , "0820", 4); 
    setbit_t(p_iso, 2 , p_BcTransLog->pri_acct_no, strlen(p_BcTransLog->pri_acct_no));
    setbit_t(p_iso, 3 , "810000", 6);
    setbit_t(p_iso, 4 , (unsigned char *)p_BcTransLog->trans_at, 12);
    setbit_t(p_iso, 5 , (unsigned char *)p_BcTransLog->fld_5, 12);
    setbit_t(p_iso, 8 , (unsigned char *)p_BcTransLog->fld_8, 8);
    setbit_t(p_iso, 11, (unsigned char *)p_tiBcMchntTermChargeInf->sys_tra_no, 6);
    setbit_t(p_iso, 12, (unsigned char *)p_BcTransLog->loc_trans_tm, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog->loc_trans_dt, 4);
    setbit_t(p_iso, 42, aj_mchnt_cd, 15);
    setbit_t(p_iso, 46, (unsigned char *)p_tiBcMchntTermChargeInf->mchnt_cd_c, 15);
    setbit_t(p_iso, 48, (unsigned char *)buff, iLen);    
    setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog->trans_curr_cd, min_strlen(p_BcTransLog->trans_curr_cd,30));
    setbit_t(p_iso, 61, (unsigned char *)p_BcTransLog->fld_61, strlen(p_BcTransLog->fld_61));
    if ( 0 != *p_BcTransLog->fld_45 )
    {
        strcpy(buff, p_BcTransLog->fld_45);
        rtrim(buff);
        setbit_t(p_iso, 45 , (unsigned char *)buff, strlen(buff));
    }
    setbit_t(p_iso, 64, "00000000", 8);

    //���ͼ����Ϣ
    BC_SendMonitor_iso("0","0", "AJXT", p_iso, p_tiBcTransInf);
    
    //����ǩ�����������ݱ�
    memset(buff, 0, sizeof(buff) );

    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ2210_1>start call BC_SendMessageToAJ �������ݸ����ݡ�",__FILE__,__LINE__);

    //ת��������    
    memset(buff, 0, sizeof(buff) );
    PrintISO( p_iso,"���͸����ݵķǰ�֧������",0);
    iLen = BC_SendMessageToAJ(p_iso, buff,p_tiBcInsInf);
    if ( iLen <= 0 )
    {
        //���͸�����ʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210_1>send to rcv failure.",__FILE__,__LINE__);

        *resp_flag = 0;
        strcpy(resp_code,"N00091");

        //���¿��¼
        BC_SendExeception("03","���棬���Ͱ��ݷǰ�֧������ʧ��.");
        return -1;
    }
    //���
    memcpy(resp_code,buff,6);
    resp_code[6] = 0;
    if ( 0==memcmp(resp_code,"N00000",6) )
    {
        //�����˵���ʱ��
    	setbit_t(p_iso, 42, (unsigned char *)p_tiBcMchntTermChargeInf->mchnt_cd_c, 15);
        iRet = BC_InsertBillNotify(gs_myFid,g_iSrcFoldId,I_TIMEOUT_BILL,p_iso, &g_TransMsg_o);
        if ( 0== iRet )
        {
            dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210_1>BC_InsertBillNotify success.(p_tiBcInsInf->packet_type:%s)",__FILE__,__LINE__,p_tiBcInsInf->packet_type);
        } else
        {
            dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210_1>BC_InsertBillNotify failure.(p_tiBcInsInf->packet_type:%s)",__FILE__,__LINE__,p_tiBcInsInf->packet_type);
        }
    }
    return 0;
}

//�ǰ��û�֧�� 0850 ����
int BusZ2210(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet ;

    int sqlCode    ;
    int resp_flag ;
    char resp_code[6+1];
    char buff[1024] ;
    int isExitFlag = 0;

    ti_bc_mt_flow_id_def tiBcMTFlowId ;
    ti_bc_ins_inf_def tiBcInsInf; // ���ջ����Ļ�����Ϣ
    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //�����̻���Ϣ
    ti_bc_mchnt_term_charge_inf_def *p_tiBcMchntTermChargeInf = (ti_bc_mchnt_term_charge_inf_def *) &g_tiBcMchntTermChargeInf; //�̻��ն�ת����Ϣ
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;

    strcpy(p_BcTransLog->flag_lock, "0");
    BC_SendMonitor_trans("0","3", p_BcTransLog,p_tiBcTransInf);

    //first ����#48�����,��֤ΪUTF-8����
    //��ȡ#33�������Ϣ
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog->fwd_ins_id_cd );
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcInsInfOpr(ins_id_cd:%s) ��ѯ����(�˵�ϵͳ)�Ļ�����Ϣ !",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // ��ѯ������Ϣʧ��
        strcpy(resp_code,"96");
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>call tiBcInsInfOpr return failure,cann't find ins[%s] inf.",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
        BC_SendExeception("��ѯ������·��Ϣʧ��,����id:[%s].",p_tiBcInsInf->ins_id_cd);

       //�쳣��¼���
				if( 0 > BC_exp_log("0091","��ѯ������Ϣʧ��",g_Rcv_Buffer,g_RcvBufferSize) )
				{
					dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcExpLogOpr �쳣���¼���ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]!",__FILE__,__LINE__,p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
         BC_SendExeception("03","���棬�ǰ��û������¼�쳣��ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
				}

        //����ʧ��        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N000D1", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
        BC_SetMacKey(0,0);
		    //BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso);
        return 0;
    }

    //У���˵�������Կ
    if ( *p_tiBcInsInf->key_type != '0' )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_TermCheckMac(ins_id_cd:%s) У�����(�˵�ϵͳ)�Ļ�����Կ��Ϣ !",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
        if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) )
        {
            //�������ݿ�
            //dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

            //���
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->resp_cd,"A0");
            strcpy(p_BcTransLog->fld_28 ,"N000A0");
            strcpy(p_BcTransLog->bc_settle_in, "0");
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
            
            //�쳣��¼���
				    if( 0 > BC_exp_log("00A0","У���˵�������Կ��Ϣʧ��",g_Rcv_Buffer,g_RcvBufferSize) )
				    {
					     dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcExpLogOpr �쳣���¼���ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]!",__FILE__,__LINE__,p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
              BC_SendExeception("03","���棬�ǰ��û������¼�쳣��ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
				    }

            //����ʧ��        
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            setbit_t(iso,28,"N000A0", 6 );
            setbit_t(iso,64,"00000000", 8 );
            BC_SetRcvMacKey(p_tiBcInsInf);
            BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
            BC_SendMessageToSrc(iso);
            return 0;
        }
    }
    iRet = ToGBK(p_BcTransLog->fld_48,g_TransMsg.len_48,
                  buff,sizeof(buff)-1);
    if ( iRet>0 )
    {
        iRet = MIN(iRet,sizeof(p_BcTransLog->fld_48)-1);
        memcpy(p_BcTransLog->fld_48,buff, iRet);
        p_BcTransLog->fld_48[iRet] = 0;
        g_TransMsg.len_48 = iRet;
    }
	else
	{
		ErrInfoInBcTransLog(__FILE__, __LINE__, "N00091", "48��תGBKת��ʧ�ܣ�", 0);
		return ReturnPayMessageToRecive("N00091", iso, p_tiBcTransInf, p_BcTransLog);
	}

    //�˵����
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tlBcBillLogOpr !",__FILE__,__LINE__ );
    isExitFlag = 0;
    BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
    iRet = tlBcBillLogOpr(GLB_DB_INSERT1, p_tlBcBillLog, &sqlCode);
    if ( SQL_ERD_UNIKEY == sqlCode )
    {
        //��ѯ���˵�����״̬
        tlBcBillLogOpr(GLB_DB_SELECT2, p_tlBcBillLog, &sqlCode);
        if ( 0 == sqlCode )
        {   
            //�������˸��˵����гɹ����׻�������� NC0001
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>�˵����ظ�(fwd_ins_id_cd:%s,bill_id:%s)��!",__FILE__,__LINE__,p_BcTransLog->fwd_ins_id_cd, p_tlBcBillLog->bill_id );
            BC_SendExeception("02","���棬�ǰ��û��������ʧ��.(fwd_ins_id_cd:%s,bill_id:%s)",p_BcTransLog->fwd_ins_id_cd, p_tlBcBillLog->bill_id );
            BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
            setbit_t(iso,0,buff,4 );
            if ( '1'==*p_tlBcBillLog->bc_settle_in )
            {
                strcpy(resp_code, "NC0001");
                setbit_t(iso,28, resp_code, 6 ) ;  //�˵��Ѿ��ɹ�֧��
                setbit_t(iso,64, "00000000", 8 ) ;
                setbitoff(iso,48);
                BC_SetRcvMacKey(p_tiBcInsInf);
                isExitFlag = 1;
                strcpy(p_BcTransLog->is_over, "1");
                strcpy(p_BcTransLog->fld_28, resp_code);
                strcpy(p_BcTransLog->resp_cd, resp_code+4);
                
            } else if ( '0'==*p_tlBcBillLog->bc_settle_in )
            {
                BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
                strcpy(p_tlBcBillLog->bc_settle_in, " ");
                iRet = tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
                if ( 0!= sqlCode )
                {
                    //update failure
                    strcpy(resp_code, "N00096");
                    setbit_t(iso,28, resp_code, 6 );
                    setbit_t(iso,64,"00000000", 8 );
                    setbitoff(iso,48);
                    BC_SetRcvMacKey(p_tiBcInsInf);
                    isExitFlag = 1;
                    strcpy(p_BcTransLog->is_over, "1");
                    strcpy(p_BcTransLog->fld_28, resp_code);
                    strcpy(p_BcTransLog->resp_cd, resp_code+4);
                }
                //������״̬���������н���
            } else
            {
                strcpy(resp_code, "N00009");
                setbit_t(iso,28, resp_code, 6 );
                setbit_t(iso,64,"00000000", 8 );
                setbitoff(iso,48);
                BC_SetRcvMacKey(p_tiBcInsInf);
                isExitFlag = 1;
                strcpy(p_BcTransLog->is_over, "1");
                strcpy(p_BcTransLog->fld_28, resp_code);
                strcpy(p_BcTransLog->resp_cd, resp_code+4);
            }            
        } else
        {
            //��ѯ���˵�����״̬ʧ��
            strcpy(resp_code, "N00096");
            setbit_t(iso,28, resp_code, 6 );
            setbit_t(iso,64,"00000000", 8 );
            setbitoff(iso,48);
            BC_SetRcvMacKey(p_tiBcInsInf);
            isExitFlag = 1;
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->fld_28, resp_code);
            strcpy(p_BcTransLog->resp_cd, resp_code+4);
        }
    }
    else if ( 0 != sqlCode )
    {
        strcpy(resp_code, "N00096");
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcBillLogOpr ���ʧ��!",__FILE__,__LINE__);
        BC_SendExeception("03","���棬�˵�BILL���ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,resp_code, 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
	      BC_SetRcvMacKey(p_tiBcInsInf);
        isExitFlag = 1;
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->fld_28, resp_code);
        strcpy(p_BcTransLog->resp_cd, resp_code+4);
    } else
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcBillLogOpr ���ɹ�",__FILE__,__LINE__); 

    //���
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_InsertIntoDB !",__FILE__,__LINE__ );
    BC_InsertIntoDB(p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        if( sqlCode == SQL_ERD_UNIKEY ){
            strcpy(resp_code, "N00094");
        } else{
            strcpy(resp_code, "N00096");
        }
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>BC_InsertIntoDB ���ʧ��!",__FILE__,__LINE__);
        BC_SendExeception("03","���棬�ǰ��û��������ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,resp_code, 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
	      BC_SetRcvMacKey(p_tiBcInsInf);
        isExitFlag = 1;
    } else
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>BC_InsertIntoDB ���ɹ�",__FILE__,__LINE__);

    //�ж��Ƿ���Ҫ�˳�
    if( isExitFlag ){
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }
   
    // ��ȡ�̻���Ϣ
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcMchntInf->mchnt_cd, p_BcTransLog->mchnt_cd);
    iRet = tiBcMchntInfOpr(GLB_DB_SELECT1, p_tiBcMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //û���ҵ��̻���Ϣ
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

        //���¿�
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->fld_28 ,"N00003");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
            BC_SendExeception("03","���棬�ǰ��û�������¿��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ɹ�",__FILE__,__LINE__); 

        //����ʧ��        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00003", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);        
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }

    //area_cd
    strcpy(p_BcTransLog->area_cd, p_tiBcMchntInf->area_cd);

    //����̻�ת����Ϣ�� ti_bc_mchnt_term_charge_inf_def g_tiBcMchntTermChargeInf;
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcMchntTermChargeInfOpr ��ȡ�̻�ת����Ϣ !",__FILE__,__LINE__ );
    strcpy(p_tiBcMchntTermChargeInf->mchnt_tp,  p_BcTransLog->mchnt_tp);
    strcpy(p_tiBcMchntTermChargeInf->term_id ,  p_BcTransLog->term_id);
    strcpy(p_tiBcMchntTermChargeInf->mchnt_cd,  p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcMchntTermChargeInf->rcv_ins_id_cd,  g_rtIns_id_cd);
    iRet = tiBcMchntTermChargeInfOpr(GLB_DB_SELECT1, p_tiBcMchntTermChargeInf, &sqlCode);
    if( 0 != sqlCode )
    {
        // ��ѯ�̻�ת����Ϣ��ʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>call tiBcMchntTermChargeInfOpr failure,cann't find ins(%s) inf.",__FILE__,__LINE__, p_tiBcMchntTermChargeInf->mchnt_cd);
        BC_SendExeception("03","��ѯ�̻�ת����Ϣʧ��,mchnt_cd:[%s].",p_tiBcMchntTermChargeInf->mchnt_cd);
        
        //û���ҵ��̻���Ϣ
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcMchntInf->mchnt_cd,sqlCode );

        //���¿�
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->fld_28 ,"N00003");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
            BC_SendExeception("03","���棬�ǰ��û�������¿��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ɹ�",__FILE__,__LINE__); 

        //����ʧ��        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00003", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);        
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }

    //��ȡת����ˮ��
    strcpy(tiBcMTFlowId.term_id   ,  p_tiBcMchntTermChargeInf->term_id_c);
    strcpy(tiBcMTFlowId.mchnt_cd  ,  p_tiBcMchntTermChargeInf->mchnt_cd_c);
    tiBcMTFlowIdOpr(GLB_DB_SELECT1, &tiBcMTFlowId, &sqlCode);

    //ת����ˮ�ţ��ն˺ţ��̻���
    strcpy(p_tiBcMchntTermChargeInf->sys_tra_no, tiBcMTFlowId.sys_tra_no);
    strcpy(p_BcTransLog->center_sys_tra_no,  tiBcMTFlowId.sys_tra_no);
    strcpy(p_BcTransLog->center_term_id   ,  p_tiBcMchntTermChargeInf->term_id_c);
    strcpy(p_BcTransLog->center_mchnt_cd  ,  p_tiBcMchntTermChargeInf->mchnt_cd_c);

    // ��ȡ�����̻���Ϣ
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcQsMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__, p_tiBcMchntTermChargeInf->mchnt_cd_c);
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_tiBcMchntTermChargeInf->mchnt_cd_c);
    strcpy(p_tiBcQsMchntInf->term_id, p_tiBcMchntTermChargeInf->term_id_c);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_tiBcMchntTermChargeInf->rcv_ins_id_cd);
    iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //û���ҵ������̻���Ϣ
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd,sqlCode );
        BC_SendExeception("03","cant not find qs mchnt.(mchnt_cd:%s,sqlcode=%d) ",p_tiBcQsMchntInf->mchnt_cd,sqlCode );
        
        //���¿�
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->fld_28 ,"N00003");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
            BC_SendExeception("03","���棬�ǰ��û�������¿��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ɹ�",__FILE__,__LINE__); 

        //����ʧ��        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00003", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }
    
    //����·�ɻ�����Ϣ����#100�������Ϣ
    if( 0!= BC_Search_ins_inf(resp_code, &tiBcInsInf, g_rtIns_id_cd))
    {
    	// ��ѯ���ջ�����·��Ϣʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>call BC_Search_ins_inf failure,cann't find ins inf.",__FILE__,__LINE__);
        BC_SendExeception("03","��ѯ������·��Ϣʧ��,����id:[%s].", g_rtIns_id_cd);
        
        //���¿�
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"03");
        strcpy(p_BcTransLog->fld_28 ,"N00091");
        strcpy(p_BcTransLog->bc_settle_in, "0");
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
            BC_SendExeception("03","���棬�ǰ��û�������¿��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ɹ�",__FILE__,__LINE__); 

        //����ʧ��        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BC_SendMessageToSrc!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00091", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);        
        BC_SetRcvMacKey(p_tiBcInsInf);
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso) ;
        return 0;
    }

    //check field
    if ( 0 != BC_CheckIsoDataField(iso, "2,3,4,5,8,11,12,13,42,48,61,64") )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ2210>doBus_CheckIsoDataField failure.",__FILE__,__LINE__);        
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,"30");
        strcpy(p_BcTransLog->fld_28,"N00030");
        strcpy(p_BcTransLog->bc_settle_in, "0");

        dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ2210>start to call BC_InsertIntoDB.",__FILE__,__LINE__);
        iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
        if ( 0 != iRet )
        {
            dcs_log(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ʧ��!",__FILE__,__LINE__);
            BC_SendExeception("03","���棬�ǰ��û�������¿�ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        } else
            dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ɹ�",__FILE__,__LINE__); 

        //����ʧ��        
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BusT22_send_term_resp!",__FILE__,__LINE__);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00030", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
        BC_SetRcvMacKey(p_tiBcInsInf);        
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso);
        return 0;
    }

    //���¿���
    strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
    strcpy(p_BcTransLog->flag_rcv, "0");  //���ڷ���

    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ����ʧ��!",__FILE__,__LINE__);
        BC_SendExeception("���棬�ǰ��û�֧�����¿�ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);

        BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
        setbit_t(iso,0,buff,4 );
        setbit_t(iso,28,"N00096", 6 );
        setbit_t(iso,64,"00000000", 8 );
        setbitoff(iso,48);
        BC_SetRcvMacKey(p_tiBcInsInf);        
        BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
        BC_SendMessageToSrc(iso);
        return 0;
    }
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿�ɹ�",__FILE__,__LINE__); 
    //���͸�����
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call BusZ01_1 �򰲽ݷ��ͷǰ�֧������.",__FILE__,__LINE__); 
    iRet =BusZ2210_1(p_BcTransMsg,&resp_flag, resp_code, p_tiBcTransInf,&tiBcInsInf,p_tiBcQsMchntInf, p_tiBcMchntTermChargeInf );
    if ( iRet < 0 )
    {
        //���͸�����ʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>send to rcv failure.",__FILE__,__LINE__);

        //���¿��¼
        strcpy(p_BcTransLog->flag_rcv, "3");
        strcpy(p_BcTransLog->is_over, "1");
        strcpy(p_BcTransLog->resp_cd,resp_code+4);
        strcpy(p_BcTransLog->bc_settle_in, "0");
        strcpy(p_BcTransLog->fld_28,resp_code);
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);

        //����ʧ��
        //BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
        strcpy(p_tlBcBillLog->bc_settle_in, "0");
        strcpy(p_tlBcBillLog->resp_code, resp_code);
        iRet = tlBcBillLogOpr(GLB_DB_INSERT1, p_tlBcBillLog, &sqlCode);     
    } else
    {
        strcpy(p_BcTransLog->flag_rcv, "2");
        strcpy(p_BcTransLog->resp_cd, resp_code+4);
        if ( 0 == memcmp(resp_code, "N00000", 6) )
        {
            //strcpy(p_BcTransLog->bc_settle_in, "1");
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);

        } else
        {
            strcpy(p_BcTransLog->bc_settle_in, "0");
            strcpy(p_BcTransLog->fld_28,resp_code);
            strcpy(p_BcTransLog->is_over, "1");
            strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);
        }
    }

    //�������ݿ�
    strcpy(p_BcTransLog->rcv_ins_id_cd, tiBcInsInf.ins_id_cd);
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcTransLogOpr ���¿��¼ʧ��!",__FILE__,__LINE__);
        BC_SendExeception("03","���棬�����˵��������ݿ��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }

    //Ӧ����˵�ϵͳ
    BC_StringAdder(p_BcTransLog->msg_tp,10,buff,4);
    setbit_t(iso,0,buff,4 );
    setbit_t(iso,28,resp_code, 6  );
    setbit_t(iso,64,"00000000", 8 );
    setbitoff(iso,48);
    BC_SetRcvMacKey(p_tiBcInsInf);
    BC_SendMonitor_iso("1","0",p_BcTransLog->trans_chnl,iso,p_tiBcTransInf);
    BC_SendMessageToSrc(iso);
    return 0;
}

//���Ͱ��ݷǰ�֧�����ĸ��˵�ϵͳ
int BusZ2211_1(BcTransMsg *p_BcTransMsg,char *resp_code, ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_ins_inf_def *p_tiBcInsInf)
{
    int iLen;
    int iRet;
    int sqlCode;
    char buff[2048];
    char packet_type[5];
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg.BcTransLog;
    tl_bc_trans_log_def *p_BcTransLog1 = &g_TransMsg_o.BcTransLog;
    ISO_data iso;
    ISO_data *p_iso = &iso ;

    //�齨����
    memset(p_iso,0,sizeof(ISO_data));
    clearbit(p_iso);

    if ( 0 != BC_SetIsoMode(p_iso, p_BcTransLog1->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ2211_1>BC_SetIsoMode failure(p_BcTransLog->trans_chnl:%s)��",__FILE__,__LINE__,p_BcTransLog->trans_chnl);
        BC_TRACE_END( -1 );
    }

    setbit_t(p_iso, 0 , "0870", 4); 
    rtrim(p_BcTransLog1->pri_acct_no);
    setbit_t(p_iso, 2 , p_BcTransLog1->pri_acct_no, strlen(p_BcTransLog1->pri_acct_no));
    setbit_t(p_iso, 3 , "811001", 6);
    setbit_t(p_iso, 4 , (unsigned char *)p_BcTransLog->trans_at, 12);
    setbit_t(p_iso, 5 , (unsigned char *)p_BcTransLog->fld_5, min_strlen(p_BcTransLog->fld_5,12));
    setbit_t(p_iso, 8 , (unsigned char *)p_BcTransLog->fld_8, min_strlen(p_BcTransLog->fld_8,8));
    setbit_t(p_iso, 11, (unsigned char *)p_BcTransLog1->sys_tra_no, 6);
    setbit_t(p_iso, 12, (unsigned char *)p_BcTransLog1->loc_trans_tm, 6);
    setbit_t(p_iso, 13, (unsigned char *)p_BcTransLog1->loc_trans_dt, 4);
    setbit_t(p_iso, 28, (unsigned char *)resp_code, 6);	
    setbit_t(p_iso, 33, (unsigned char *)p_BcTransLog1->fwd_ins_id_cd, 8);
    setbit_t(p_iso, 42, (unsigned char *)p_BcTransLog1->mchnt_cd, 15);
	  if(memcmp(resp_code,"N00000",6)==0)//���׳ɹ�
	  {
	  	setbit_t(p_iso, 15, (unsigned char *)p_BcTransLog->settle_dt, 4);
	  	setbit_t(p_iso, 37, (unsigned char *)p_BcTransLog->retri_ref_no, 12);
		}
    if ( 0 != *p_BcTransLog1->fld_45 )
    {
        strcpy(buff, p_BcTransLog1->fld_45);
        rtrim(buff);
        setbit_t(p_iso, 45 , (unsigned char *)buff, strlen(buff));
    }
    setbit_t(p_iso, 49, (unsigned char *)p_BcTransLog1->trans_curr_cd, min_strlen(p_BcTransLog1->trans_curr_cd,30));
    setbit_t(p_iso, 64, "00000000", 8);

    //memset(buff, 0, sizeof(buff) );
    dcs_debug(0,0,"<FILE:%s,LINE:%d><BusZ2211_1>start call BC_SendMessageToAJ �������ݸ����ݡ�",__FILE__,__LINE__);

    //ת�����˵�ϵͳ 
    PrintISO( p_iso,"���ķ��͸��˵�ϵͳ�ǰ�֧������Ӧ��",0);
    if ( '0' == *p_tiBcInsInf->key_type )
    {
        BC_SetMacKey(0,0);
    } else
    {
        BC_SetMacKey(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1);
    }

    //���ͼ����Ϣ
    BC_SendMonitor_iso("0","0",p_BcTransLog1->trans_chnl,p_iso,p_tiBcTransInf);
    return BC_SendMessage(p_iso, p_BcTransLog1->trans_chnl );
}

//�ǰ��û�֧��Ӧ�� 0830
int BusZ2211(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int succeed = 1 ;
    int sys_succeed = 1 ;
    int iRet = -1   ;
    int sqlCode     ;
    char resp_code[6+1];
    char buff[256] ;

    ti_bc_ins_inf_def tiBcInsInf; // ���ջ����Ļ�����Ϣ
    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    ti_bc_mchnt_inf_def *p_tiBcMchntInf= &g_tiBcMchntInf;
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //�����̻���Ϣ
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog; // from db
    tl_bc_trans_log_def *p_BcTransLog1 = &g_TransMsg.BcTransLog;  // new resp
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;

    strcpy(p_BcTransLog->flag_lock, "0");
    BC_SendMonitor_iso("1","2", "AJXT", iso, p_tiBcTransInf);
    memcpy(resp_code,p_BcTransLog1->fld_28, 6);
    *(resp_code+6) = 0;

    // ��ȡ�����̻���Ϣ
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog->mchnt_cd);
    strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog->center_term_id);
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog->center_mchnt_cd);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, g_rtIns_id_cd);
    iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //û���ҵ��̻���Ϣ
        dcs_log(p_BcTransLog, sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d><BusZ2210>cant not find mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_BcTransLog->center_mchnt_cd,sqlCode );
        BC_SendExeception("02","���棬�ǰ��û�Ӧ���׻�ȡ�����̻���Ϣʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->center_sys_tra_no,p_BcTransLog->center_term_id,p_BcTransLog->center_mchnt_cd);
        sys_succeed = 0;
    }

    //У����ջ����̻���Կ
    if ( 1 == succeed )
    {
  	  if( 0!= BC_Search_ins_inf(buff, p_tiBcInsInf, "00945500"))
    	{
        	dcs_log(0,0,"<FILE:%s,LINE:%d><BC_SendMessageToAJ>��ѯ���ݻ�����Ϣʧ��!",__FILE__,__LINE__);
        	BC_TRACE_END( -1 );
    	}
      if ( 0 != BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) )
        {
            sys_succeed = 0;
            strcpy(resp_code,"N000A0");

            //�쳣��־
            dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2210>�����̻�MACУ��δͨ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s] ",__FILE__,__LINE__,p_BcTransLog->sys_tra_no,p_BcTransLog->center_term_id,p_BcTransLog->center_mchnt_cd );
            BC_SendExeception("02","���棬�̻�MACУ��δͨ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->center_sys_tra_no,p_BcTransLog->center_term_id,p_BcTransLog->center_mchnt_cd);

            //�쳣���
            tl_bc_exp_log_def tlBcExpLog;
            strcpy(tlBcExpLog.rsn_code,"00A0");
            strcpy(tlBcExpLog.rsn_desc,"MACУ���");
            bcd_to_asc(tlBcExpLog.data, g_Rcv_Buffer, MIN(sizeof(tlBcExpLog.data),g_RcvBufferSize),0);
            tlBcExpLogOpr(GLB_DB_INSERT1, &tlBcExpLog, &sqlCode);
            if ( 0 != sqlCode )
            {
                dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210>tlBcExpLogOpr �쳣���¼���ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]!",__FILE__,__LINE__,p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
                BC_SendExeception("03","���棬�ǰ��û������¼�쳣��ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            }
        }
    }

    //�жϽ����Ƿ�ɹ�
    strcpy(p_BcTransLog->is_over, "1");
    strcpy(p_BcTransLog->fld_28, resp_code);
    strcpy(p_BcTransLog->resp_cd, resp_code+4);
    strcpy(p_BcTransLog->resp_cd_rcv, resp_code);
    strcpy(p_BcTransLog->flag_rcv, "2" );
    strcpy(p_BcTransLog->bill_id_rcv, p_BcTransLog1->fld_45);
    strcpy(p_BcTransLog->flag_6, "2" );
    if ( 0 != memcmp(resp_code,"N00000",6) )
    {
        //���ײ��ɹ�
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e );
        strcpy(p_BcTransLog->bc_settle_in, "0");
        succeed = 0;
    } else
    {
        //���׳ɹ�
        chkCardnoList();
        SMS_Send(1);
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s );
        strcpy(p_BcTransLog->bc_settle_in, "1");
    }

    //��ȡ#100���ջ�����Ϣ
    memset(&tiBcInsInf.ins_id_cd, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy(tiBcInsInf.ins_id_cd, p_BcTransLog->rcv_ins_id_cd );
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211_1>start to call tiBcInsInfOpr(ins_id_cd:%s) ��ѯ����(�˵�ϵͳ)�Ļ�����Ϣ !",__FILE__,__LINE__, tiBcInsInf.ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // ��ѯ������Ϣʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2211_1>call tiBcInsInfOpr return failure,cann't find ins[%s] inf.",__FILE__,__LINE__, tiBcInsInf.ins_id_cd);
        BC_SendExeception("03","��ѯ������·��Ϣʧ��,����id:[%s].",tiBcInsInf.ins_id_cd);    

        //��Ȼ��Ϊ�ɹ�,����
        sys_succeed = 0;
        strcpy(resp_code,"N00001");
    }

    //check field
    if ( 1 == succeed ){
        if ( 0 != BC_CheckIsoDataField(iso, "3,4,5,8,11,12,13,28,42,48,49") )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ2211>doBus_CheckIsoDataField failure.",__FILE__,__LINE__);        
            BC_SendExeception("03","���棬�ǰ��û�������¿�ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
            sys_succeed = 0;
        }
    }
    //�������ݿ�
    strcpy(p_BcTransLog->settle_dt, p_BcTransLog1->settle_dt);
    strcpy(p_BcTransLog->retri_ref_no, p_BcTransLog1->retri_ref_no);
    dcs_debug(0,0, "<FILE:%s,LINE:%d>mchnt_cd[%s],term_id[%s],sys_tra_no[%s],loc_trans_dt[%s],settle_dt[%s]",__FILE__,__LINE__,p_BcTransLog->mchnt_cd,p_BcTransLog->term_id,p_BcTransLog->sys_tra_no,p_BcTransLog->loc_trans_dt,p_BcTransLog->settle_dt);
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211>tlBcTransLogOpr ���¿��¼ʧ��!",__FILE__,__LINE__);
        BC_SendExeception("03","���棬�����˵��������ݿ��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        sys_succeed = 0;
        strcpy(resp_code,"N00096");
    }

    //����bill��
    BusZ_BillCopy(p_BcTransLog, p_tlBcBillLog);
    iRet = tlBcBillLogOpr(GLB_DB_UPDATE2, p_tlBcBillLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        sys_succeed = 0;
        succeed = 0;
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211>tlBcBillLogOpr ����ʧ��(bill_id:%s,bill_id_rcv:%s, sqlCode:%d)!",__FILE__,__LINE__,p_tlBcBillLog->bill_id,p_tlBcBillLog->bill_id_rcv,sqlCode);
        BC_SendExeception("03","���棬�˵�BILL���ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    } else
    {
        dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211>tlBcBillLogOpr ���¿�ɹ�",__FILE__,__LINE__); 
    }     

    //������Ӧ��
    BC_SendMessageToFolder_direct( (sys_succeed==1)?"N00000":resp_code,6);
    if ( 0 != iRet )
    {
        strcpy(resp_code,"96");
        dcs_log(0,0, "<FILE:%s,LINE:%d><BusT22_send_search_req>���볬ʱ��ʧ��",__FILE__,__LINE__);
        BC_SendExeception("03","���棬���볬ʱ��ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
    }

    //��ȡ#33�������Ϣ
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog->fwd_ins_id_cd );
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211_1>start to call tiBcInsInfOpr(ins_id_cd:%s) ��ѯ����(�˵�ϵͳ)�Ļ�����Ϣ !",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        // ��ѯ������Ϣʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d><BusZ2211_1>call tiBcInsInfOpr return failure,cann't find ins[%s] inf.",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
        BC_SendExeception("03","��ѯ������·��Ϣʧ��,����id:[%s].",p_tiBcInsInf->ins_id_cd);              
        return -1;
    }

    //Ӧ����˵�ϵͳ,�����鱨��
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2211>׼��ת�����˵�ϵͳ(sys_succeed,%d)",__FILE__,__LINE__, sys_succeed);
    if ( 1 == sys_succeed )
    {
        BC_SetRcvMacKey(p_tiBcInsInf);
        return BusZ2211_1(&g_TransMsg_o,resp_code,p_tiBcTransInf,p_tiBcInsInf);
    }
    return 0;
}

//�ǰ��û�֧��֪ͨӦ��, -- �˵�ϵͳ���� 0880
int BusZ2212(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    int iRet = -1   ;
    int succeed = 1 ;
    int sys_succeed = 1 ;    
    int sqlCode     ;
    char resp_code[6+1];
    char buff[256] ;

    ti_bc_mchnt_det_def *p_tiBcMchntDet = &g_tiBcMchntDet;
    tl_bc_trans_log_def *p_BcTransLog = &g_TransMsg_o.BcTransLog; // from db
    tl_bc_trans_log_def *p_BcTransLog1 = &g_TransMsg.BcTransLog;  // new resp
    ti_bc_ins_inf_def *p_tiBcInsInf = &g_tiBcInsInf ; //������Ϣ
    tl_bc_bill_log_def *p_tlBcBillLog = &g_tlBcBillLog;
    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //����ԭʼ���׽��д���

    BC_SendMonitor_trans("1","3",p_BcTransLog1, p_tiBcTransInf);

    //��ȡ#33�������Ϣ
    strcpy(p_tiBcInsInf->ins_id_cd, p_BcTransLog->fwd_ins_id_cd );
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call tiBcInsInfOpr(ins_id_cd:%s) ��ѯ����(�˵�ϵͳ)�Ļ�����Ϣ !",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
    iRet = tiBcInsInfOpr(GLB_DB_SELECT1, p_tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_s);
        // ��ѯ������Ϣʧ��
        strcpy(resp_code,"96");
        dcs_log(0, 0,"<FILE:%s,LINE:%d>call tiBcInsInfOpr return failure,cann't find ins[%s] inf.",__FILE__,__LINE__, p_tiBcInsInf->ins_id_cd);
        BC_SendExeception("��ѯ������Ϣʧ��,����id:[%s].",p_tiBcInsInf->ins_id_cd);
        BC_UnLockTrans(p_BcTransLog_o);
        return -1;
    } else
    {
        strcpy(p_BcTransLog->trans_flag, p_tiBcTransDet->trans_flag_e);

        if ( *p_tiBcInsInf->key_type != '0' )
        {
            if ( 0!= BC_TermCheckMac(p_tiBcInsInf->key_index,p_tiBcInsInf->key_1, g_Rcv_Buffer+4,g_RcvBufferSize-4) ){
                BC_UnLockTrans(p_BcTransLog_o);
                return -1;
            }
        }
    }

    //�������ݿ⣬����
    strcpy(p_BcTransLog->flag_lock, "0");
    strcpy(p_BcTransLog->is_over, "1");
    //strcpy(p_BcTransLog->flag_pay, "2");
    iRet = tlBcTransLogOpr(GLB_DB_UPDATE1, p_BcTransLog, &sqlCode);
    if ( 0 != sqlCode )
    {
        dcs_log(0,0, "<FILE:%s,LINE:%d>tlBcTransLogOpr ���¿��¼ʧ��!",__FILE__,__LINE__);
        BC_SendExeception("03","���棬���·ǰ��û�֧��֪ͨӦ�����ݿ��¼ʧ��.��ˮ��:[%s],�ն˺�:[%s],�̻���:[%s]",p_BcTransLog->sys_tra_no,p_BcTransLog->term_id,p_BcTransLog->mchnt_cd);
        return -1;
    }

    dcs_debug(0,0, "<FILE:%s,LINE:%d>���¿��¼�ɹ�!",__FILE__,__LINE__);
    return 0;
}

//���û�֧��
int BusZ2220(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    return -1;
}


int BusZ2221(ti_bc_trans_inf_def *p_tiBcTransInf,ti_bc_trans_det_def *p_tiBcTransDet, ISO_data *iso,char* flag)
{
    return 0;
}

//���Ͱ��ݽ��׽����ѯ����
int BusZ2210_T_S(char *resp_code, ti_bc_ins_inf_def *p_tiBcInsInf, ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf )
{
    int iLen;
    int iRet;
    int sqlCode;
    char buff[4096];
    char packet_type[5];
    TimeOut_Key tk;
    
    ISO_data iso;
    BcTransMsg bcTransMsg;
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog; //����ԭʼ���׽��д���

    //�齨����
    memset(&iso,0,sizeof(ISO_data));
    clearbit(&iso);

    if ( 0 != BC_SetIsoMode(&iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d>���û�����������ʧ�ܡ�",__FILE__,__LINE__);
        return -1;
    }
    
    setbit_t(&iso, 0 , "0621", 4); 
    setbit_t(&iso, 3 , "610001", 6);
    setbit_t(&iso, 13, (unsigned char *)p_BcTransLog_o->loc_trans_dt, 4);
    setbit_t(&iso, 42, aj_mchnt_cd, 15);
    setbit_t(&iso, 46, (unsigned char *)p_BcTransLog_o->center_mchnt_cd, 15);
    setbit_t(&iso, 48, (unsigned char *)p_BcTransLog_o->center_sys_tra_no, 6);
    setbit_t(&iso, 64, "00000000", 8);

    //���ͼ����Ϣ

	BC_SendMonitor_iso("0","0", "AJXT", &iso, 0);

    //�򰲽ݷ��ͽ��׽����ѯ����
    memset(buff, 0, sizeof(buff) );    
    PrintISO( &iso, "���͸����ݵĽ��׽����ѯ����", 0);
    iLen = BC_SendMessageToAJ(&iso, buff,p_tiBcInsInf);
    if ( iLen <= 0 )
    {
        //���͸�����ʧ��
        dcs_log(0, 0,"<FILE:%s,LINE:%d>send to rcv failure.",__FILE__,__LINE__);

        //���¿��¼
        BC_SendExeception("03","���棬���Ͱ����̻����׽����Ϣʧ��.");
        strcpy(resp_code,"N00098");
        return -1;
    }

    //���
    memset(&iso,0,sizeof(ISO_data));
    clearbit(&iso);
    if ( 0!= BC_SetIsoMode(&iso, p_tiBcInsInf->packet_type) )
    {
        strcpy(resp_code,"N00096");
        dcs_log(0,0,"<FILE:%s,LINE:%d>BC_SetIsoMode failure��",__FILE__,__LINE__);
        return -1;
    }
    dcs_debug(0,0,"<FILE:%s,LINE:%d>start call BC_UnPack_Data ����������ݡ�",__FILE__,__LINE__);
    iRet = BC_UnPack_Data( &iso, buff,  iLen);
    PrintISO( &iso, "�յ����ݽ��׽��Ӧ����", 0);

    //���͸�����
    dcs_debug(0, 0,"<FILE:%s,LINE:%d>�Ѿ��յ����ݽ��׽���ظ�����.",__FILE__,__LINE__);
    if ( iRet >= 0 )
    {
        BC_SendMonitor_iso("1","2", "AJXT",&iso, 0);
        dcs_debug(0, 0,"<FILE:%s,LINE:%d>���׽���ظ����Ľ���ɹ�.",__FILE__,__LINE__);

        memset(&bcTransMsg, 0, sizeof(bcTransMsg));
        iRet = BC_CopyISOdata( &iso, &bcTransMsg);
        if ( iRet <0 )
        {
            strcpy(resp_code,"N00030");
            return -1;
        }

        dcs_debug(0, 0,"<FILE:%s,LINE:%d>start to ����յ��Ľ��׽���ظ�����.",__FILE__,__LINE__);

        //check
        if ( 0 != memcmp(bcTransMsg.BcTransLog.msg_tp, "0631", 4) 
             || 0 != memcmp(bcTransMsg.BcTransLog.proc_cd, "610001", 6) 
             || 0 != memcmp(bcTransMsg.BcTransLog.fld_46, p_BcTransLog_o->center_mchnt_cd,15)
           )
        {
            dcs_log(buff,50,"<FILE:%s,LINE:%d>check #0 #3 #46[%s](%s) failure��",__FILE__,__LINE__, bcTransMsg.BcTransLog.fld_46, p_BcTransLog_o->center_mchnt_cd);
            strcpy(resp_code,"N00096");
            return -1;
        }

        //update ������Ϣ
        strcpy(resp_code,bcTransMsg.BcTransLog.fld_28);
        if ( 0 == memcmp(resp_code, "N00000", 6) )
        {
            //��ѯ�ɹ�
            dcs_log(buff,50,"<FILE:%s,LINE:%d>��ѯ�ɹ���׼���������֧������״̬��",__FILE__,__LINE__ );
            int i;
            int len = strlen(bcTransMsg.BcTransLog.fld_48);
            memcpy(buff, bcTransMsg.BcTransLog.fld_48, len);
            rtrim(buff);
            for ( i=0; i< len/20; i++ )
            {
                tl_bc_trans_log_def bcTransLog;
                memset(&bcTransLog, 0, sizeof(tl_bc_trans_log_def));
                memcpy(bcTransLog.loc_trans_dt, buff+i*20, 4);
                memcpy(bcTransLog.fld_28,       buff+i*20+4 , 6);
                if(memcmp(bcTransLog.fld_28, "N00000", 6) == 0 && memcmp(buff+i*20+16, "0000", 4))
                {
                	memcpy(bcTransLog.settle_dt, buff+i*20+16, 4);
                	strcpy(bcTransLog.bc_settle_in, "1");
                }
                else
                {
                	strcpy(bcTransLog.settle_dt, " ");
                	strcpy(bcTransLog.bc_settle_in, "0");
                }
                strcpy(bcTransLog.retri_ref_no, "  ");
                memcpy(bcTransLog.center_sys_tra_no,   buff+i*20+10, 6);
                memcpy(bcTransLog.center_mchnt_cd  ,   p_BcTransLog_o->center_mchnt_cd, 15);
                tlBcTransLogOpr(GLB_DB_UPDATE3, &bcTransLog, &sqlCode);
                if ( 0!= sqlCode )
                {
                    dcs_log(&bcTransLog,sizeof(tl_bc_trans_log_def),"<FILE:%s,LINE:%d>���ݲ�ѯ������½��׼�¼��Ϣʧ��sqlcode[%d]��",__FILE__,__LINE__,sqlCode);
                }
				tlBcBillLogUpdate(GLB_DB_UPDATE1,&bcTransLog, &sqlCode);
               dcs_debug(0,0,"<FILE:%s,LINE:%d>���ݲ�ѯ������½��׼�¼��Ϣ�ɹ�center_sys_tra_no[%s]��",__FILE__,__LINE__,bcTransLog.center_sys_tra_no);
               
		    		   memset(&tk, ' ', sizeof(TimeOut_Key));
				       memcpy(tk.sys_tra_no, bcTransLog.center_sys_tra_no, 6);
				       memcpy(tk.mchnt_cd, bcTransLog.center_mchnt_cd, 15);
				       tk.end = '\0';
		        	 memcpy(g_TimeOutRec.caKey, (char *)&tk, sizeof(TimeOut_Key) );
        	    
        	     //ɾ���˵���ʱ��
        	     BC_DeleteBillNotify(&g_TimeOutRec);
              break;
            }
        }

        return 0;
    }
    return -1;
}

//0820 ���׳�ʱ�Ĵ���
int BusZ2210_T( )
{
	struct TimeOut_REC *pTimeOutRec;
	
	ti_bc_trans_inf_def *p_tiBcTransInf = &g_tiBcTransInf;
	ti_bc_trans_det_def *p_tiBcTransDet = &g_tiBcTransDet;
		
  //�������ݵķǰ�֧���˵����׳�ʱ����Ҫ���ͽ��׽����ѯ����
    int iLen;
    int iRet;
    int sqlCode;
    char resp_code[6+1];
    char buff[2048],temp[256];

    BcTransMsg *p_BcTransMsg = &g_TransMsg_o ;//ԭʼ������Ϣ
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    ti_bc_ins_inf_def tiBcInsInf; // ���ջ����Ļ�����Ϣ   
    ti_bc_qs_mchnt_inf_def *p_tiBcQsMchntInf= &g_tiBcQsMchntInf ; //�̻���Ϣ
    
    if ( *p_BcTransLog_o->is_over == '1' )
    {
        //���״��������������н��׳�ʱ����
        BC_SendExeception("01","��ʱ������򣬷��ָñʽ����Ѿ����������������г�ʱ����.��mchnt_cd:[%s],sys_tra_no:[%s]��",p_BcTransLog_o->mchnt_cd, p_BcTransLog_o->sys_tra_no);

        //ԭ���׽���
        BC_UnLockTrans(p_BcTransLog_o);
        return 0;
    }

    // ��ȡ�����̻���Ϣ
    dcs_debug(0,0, "<FILE:%s,LINE:%d><BusZ2210_T>start to call tiBcMchntInfOpr to search mchnt(%s) inf !",__FILE__,__LINE__,p_BcTransLog_o->mchnt_cd);
    strcpy(p_tiBcQsMchntInf->mchnt_cd, p_BcTransLog_o->center_mchnt_cd);
    strcpy(p_tiBcQsMchntInf->term_id, p_BcTransLog_o->center_term_id);
    strcpy(p_tiBcQsMchntInf->rcv_ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    iRet = tiBcQsMchntInfOpr(GLB_DB_SELECT1, p_tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //û���ҵ��̻���Ϣ
        dcs_log(0, 0,"<FILE:%s,LINE:%d>cant not find qs mchnt.(mchnt_cd:%s,sqlcode=%d) ",__FILE__,__LINE__,p_tiBcQsMchntInf->mchnt_cd,sqlCode );

        //ԭ���׽���
        BC_UnLockTrans(p_BcTransLog_o);

        //����ʧ��
        return -1;
    }

    //��ȡ���ջ�����Ϣ
    dcs_debug(0,0,"<FILE:%s,LINE:%d>start to call tiBcInsInfOpr for select.",__FILE__,__LINE__,RPT_TO_LOG,0,0);
    memset( &tiBcInsInf, 0, sizeof(ti_bc_ins_inf_def) );
    strcpy( tiBcInsInf.ins_id_cd, p_BcTransLog_o->rcv_ins_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode  || '0' == *tiBcInsInf.openflag )
    {
        dcs_log(0, 0,"<FILE:%s,LINE:%d>call BC_CheckIsoDataField  #33�����������.",__FILE__,__LINE__);
        BC_SendExeception("02","#33�����������.");

        //ԭ���׽���
        BC_UnLockTrans(p_BcTransLog_o);
        return -1;
    }

    //���Ͱ��ݽ��׽����ѯ����
    dcs_debug(0,0, "<FILE:%s,LINE:%d>start to call BusZ2210_T_S �򰲽ݷ��ͽ��׽����ѯ��",__FILE__,__LINE__); 
    iRet = BusZ2210_T_S( resp_code , &tiBcInsInf, p_tiBcQsMchntInf);
    if ( iRet < 0 )
    {
        dcs_log(0, 0,"<FILE:%s,LINE:%d>��ѯ���׽��ʧ��.",__FILE__,__LINE__);

        //ԭ���׽���
        BC_UnLockTrans(p_BcTransLog_o);
				return -1;
    }
    //ԭ���׽���
    BC_UnLockTrans(p_BcTransLog_o);
    return 0;
}

void chkCardnoList()
{
    tl_bc_trans_log_def *p_BcTransLog_o = &g_TransMsg_o.BcTransLog;
    tl_bc_cardno_list_inf_def tlBcCardnoListInf, *p_tlBcCardnoListInf;
    char tmpBuf[1024];
    char idStr[20];
    int sqlCode;
    p_tlBcCardnoListInf = &tlBcCardnoListInf;
    memset(p_tlBcCardnoListInf, 0, sizeof(tl_bc_cardno_list_inf_def));
    rtrim(p_BcTransLog_o->pname);
    rtrim(p_BcTransLog_o->pri_acct_no);
    rtrim(p_BcTransLog_o->fld_5);
    rtrim(p_BcTransLog_o->id_type);
    rtrim(p_BcTransLog_o->fld_61);
    strcpy(p_tlBcCardnoListInf->cardno, p_BcTransLog_o->pri_acct_no);
    memset(p_tlBcCardnoListInf->relation1, ' ', sizeof(p_tlBcCardnoListInf->relation1));
    memcpy(p_tlBcCardnoListInf->relation1, p_BcTransLog_o->fld_5, strlen(p_BcTransLog_o->fld_5));
    memcpy(p_tlBcCardnoListInf->relation1+15, p_BcTransLog_o->id_type, strlen(p_BcTransLog_o->id_type));
    memcpy(p_tlBcCardnoListInf->relation1+17, p_BcTransLog_o->fld_61, strlen(p_BcTransLog_o->fld_61));
    memcpy(p_tlBcCardnoListInf->relation1+37, p_BcTransLog_o->pname, MIN(strlen(p_BcTransLog_o->pname),62));
    tlBcCardnoListInfOpr(GLB_DB_SELECT1, p_tlBcCardnoListInf, &sqlCode);
    if (0 == sqlCode)
    {
    	switch(p_tlBcCardnoListInf->index)
    	{
    		case '1':
    			return;
    		case '2':
    		case '3':
    		case '4':
    		case '5':
    			sprintf(tmpBuf,"����[%s],֧���ֻ�[%s],%s��[%s],����[%s],ԭ������Ϣ[%s]",p_BcTransLog_o->pri_acct_no,p_BcTransLog_o->fld_5,getIdStr(p_BcTransLog_o->id_type, idStr),p_BcTransLog_o->fld_61,p_BcTransLog_o->pname,p_tlBcCardnoListInf->relation1);
    			break;
    		default:
    			sprintf(tmpBuf,"����[%s], �ѱ���5�������������֧���ֻ�[%s],%s��[%s],����[%s]",p_BcTransLog_o->pri_acct_no,p_BcTransLog_o->fld_5,getIdStr(p_BcTransLog_o->id_type, idStr),p_BcTransLog_o->fld_61,p_BcTransLog_o->pname);
				break;    			
    	}
		BC_SendExeception("03", "����, %s", tmpBuf);
		if( 0 > BC_exp_log("CA", tmpBuf, g_Rcv_Buffer, g_RcvBufferSize))
		{
	  		BC_SendExeception("03","����, ��¼�쳣��ʧ��.");
	  		dcs_log(0, 0, "<FILE:%s,LINE:%d>,��¼�쳣��ʧ��.",__FILE__,__LINE__);
		}
		dcs_log(0,0,"<FILE:%s,LINE:%d>%s\n",__FILE__,__LINE__, tmpBuf);
    }
}

char* getIdStr(char *idType, char *idStr)
{
	memset(idStr, 0, sizeof(idStr));
	if(memcmp("00", idType, 2) == 0)strcpy(idStr,"���֤");
	else if(memcmp("01", idType, 2) == 0)strcpy(idStr,"����");
	else if(memcmp("02", idType, 2) == 0)strcpy(idStr,"����֤");
	else if(memcmp("03", idType, 2) == 0)strcpy(idStr,"ʿ��֤");
	else if(memcmp("04", idType, 2) == 0)strcpy(idStr,"����֤");
	else if(memcmp("05", idType, 2) == 0)strcpy(idStr,"���ڱ�");
	else if(memcmp("06", idType, 2) == 0)strcpy(idStr,"�������");
	else strcpy(idStr,"����֤��");		
	return idStr;
}
