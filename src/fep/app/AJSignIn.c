#include "ibdcs.h"
#include "tmcibtms.h"
#include "dbstruct.h"
#include "../db/inc/glbdb.h"
#include "CMDI.h"
#include "bc.h"
#include "iso8583.h"
#include <string.h>
#include <stdlib.h>

#define MIN(a,b) a>b?b:a

static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/appsvr1.log"
    if ( u_fabricatefile("log/AJSignIn.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

//���͸�����
int SendMessageToRcv(ISO_data *iso, char *pRecvData, int rcvSize, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    int iRet;
    int iConnSock ;
    char buff[2048];
    int buffLen;

    buffLen = isotostr(buff, iso);
    if ( buffLen < 0 )
    {
        printf("ISO���ʧ��.\n");
        return -1;
    }

    //��������
    printf("��ʼ����[ip:%s,port:%s].\n", p_tiBcInsInf->ip_addr, p_tiBcInsInf->port);
    iConnSock = tcp_connet_server(p_tiBcInsInf->ip_addr, atoi(p_tiBcInsInf->port),0);
    if ( iConnSock < 0 )
    {
        printf("����ʧ��[ip:%s,port:%s].\n", p_tiBcInsInf->ip_addr, p_tiBcInsInf->port);
        return -1;
    }

    //������Ϣ
    iRet = write_msg_to_net(iConnSock, buff, buffLen, 30*1000);
    if ( iRet < 0 )
    {
        printf("����ǩ����Ϣʧ��.\n");
        tcp_close_socket(iConnSock);
        return iRet;
    }

    dcs_debug(buff, buffLen, "������־\n");

    //������Ϣ
    iRet = read_msg_from_net(iConnSock, pRecvData,rcvSize,5*1000);
    if ( iRet < 0 )
    {
        printf("����ǩ��Ӧ����Ϣʧ��[%d].\n", iRet);
        dcs_debug(0,0,"����ǩ��Ӧ����Ϣʧ��[%d].\n", iRet);
        tcp_close_socket(iConnSock);
        return iRet;         
    }

    tcp_close_socket(iConnSock);
    return iRet;
}

int SignIn()
{
    //declare
    int i, sqlCode, iLen ;
    ISO_data iso;
    ti_bc_ins_inf_def tiBcInsInf;
    char buff[2048];
    char *aj_mchnt_cd;
    //search ins inf
    strcpy(tiBcInsInf.ins_id_cd, "00945500");
    tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //û���ҵ�������Ϣ
        printf("û���ڻ�����Ϣ�����ҵ����ջ�����Ϣ[%s].\n", tiBcInsInf.ins_id_cd );
        return -1;
    }
   
    //set line 
    memset(&iso,0,sizeof(ISO_data)); 
    clearbit(&iso);
    if ( 0 != BC_SetIsoMode(&iso, tiBcInsInf.packet_type) )
    {
        printf("���ý��շ�[%s]��·[%s]��Ϣʧ��.\n", tiBcInsInf.ins_id_cd,tiBcInsInf.line_nm);
        return -1;
    }
    //���
    setbit_t(&iso, 0 , "0520", 4); 
    setbit_t(&iso, 3 , "510000", 6);
	aj_mchnt_cd = getenv("AJ_MCHNT_CD");
	if(aj_mchnt_cd == NULL) aj_mchnt_cd = "302430145119998";
	printf("aj_mchnt_cd=[%s]\n", aj_mchnt_cd);
    setbit_t(&iso, 42, aj_mchnt_cd, 15);

    //����
    iLen = SendMessageToRcv(&iso, buff, sizeof(buff), &tiBcInsInf);
    if ( iLen <= 0 )
    {
        //���͸�����ʧ��
        return -1;
    }

    //��� to iso
    //memset(&iso,0,sizeof(ISO_data));
    clearbit(&iso);
    i = BC_UnPack_Data( &iso, buff, iLen);
    if ( i >= 0 )
    {
        //��ȡ���ش���
        memset(buff, 0, sizeof(buff));
        if ( (i = getbit(&iso, 28, buff)) <= 0 )
        {
            printf("��ȡǩ�����ձ���#28��ʧ��.\n");
            return -1;
        }

        if( 0!= memcmp(buff, "N00000", 6) ){
            printf("ǩ��Ӧ�𷵻�ʧ��[#28�򷵻ش��룺%s].\n", buff);
            return -1;
        }
         
        //��ȡ��Կ
        if ( (i = getbit(&iso, 48, buff)) <= 0 )
        {
            printf("��ȡǩ��Ӧ����Կʧ��.\n");
            return -1;
        }
        memcpy(tiBcInsInf.ins_id_cd, "00945500", 8);
        memcpy(tiBcInsInf.key_1, buff, 16);
        memcpy(tiBcInsInf.key_2, buff, 16);
        memcpy(tiBcInsInf.key_3, buff, 16);
        tiBcInsInfOpr(GLB_DB_UPDATE2, &tiBcInsInf, &sqlCode);
        if ( 0!= sqlCode )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>���°�����Կʧ�ܣ�",__FILE__,__LINE__);
            //������Կʧ��
            return 0;
        }
    }else
    {
        printf("����յ��İ���Ӧ����ʧ��.\n");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int i;

    //����־�ļ�
    if ( 0 > OpenLog(argv[0]) )
    {
        printf("����־ʧ�ܣ�appSrv��������ʧ�ܣ��˳�.\n");
        exit(1);
    }

    //�������ݿ�
    if ( DasConectDB()!=0 )
    {
        printf("����oracle DBʧ��.\n");
        return -1;
    }
    
    if ( IsoLoad8583config(&iso8583_AJXTconf[0],"iso8583_AJXT.conf") < 0 )
    {
        printf("װ��iso8583_AJXT.confʧ��,�˳�.\n");
        return -1;
    }
    if( 0 == SignIn() ){
        printf("ǩ���ɹ�\n");
    }else{
        printf("ǩ��ʧ��\n");
    }
}



