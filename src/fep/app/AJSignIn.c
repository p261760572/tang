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

//发送给安捷
int SendMessageToRcv(ISO_data *iso, char *pRecvData, int rcvSize, ti_bc_ins_inf_def *p_tiBcInsInf)
{
    int iRet;
    int iConnSock ;
    char buff[2048];
    int buffLen;

    buffLen = isotostr(buff, iso);
    if ( buffLen < 0 )
    {
        printf("ISO打包失败.\n");
        return -1;
    }

    //建立连接
    printf("开始连接[ip:%s,port:%s].\n", p_tiBcInsInf->ip_addr, p_tiBcInsInf->port);
    iConnSock = tcp_connet_server(p_tiBcInsInf->ip_addr, atoi(p_tiBcInsInf->port),0);
    if ( iConnSock < 0 )
    {
        printf("连接失败[ip:%s,port:%s].\n", p_tiBcInsInf->ip_addr, p_tiBcInsInf->port);
        return -1;
    }

    //发送信息
    iRet = write_msg_to_net(iConnSock, buff, buffLen, 30*1000);
    if ( iRet < 0 )
    {
        printf("发送签到信息失败.\n");
        tcp_close_socket(iConnSock);
        return iRet;
    }

    dcs_debug(buff, buffLen, "发送日志\n");

    //接收信息
    iRet = read_msg_from_net(iConnSock, pRecvData,rcvSize,5*1000);
    if ( iRet < 0 )
    {
        printf("接收签到应答信息失败[%d].\n", iRet);
        dcs_debug(0,0,"接收签到应答信息失败[%d].\n", iRet);
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
        //没有找到机构信息
        printf("没有在机构信息表中找到接收机构信息[%s].\n", tiBcInsInf.ins_id_cd );
        return -1;
    }
   
    //set line 
    memset(&iso,0,sizeof(ISO_data)); 
    clearbit(&iso);
    if ( 0 != BC_SetIsoMode(&iso, tiBcInsInf.packet_type) )
    {
        printf("设置接收方[%s]线路[%s]信息失败.\n", tiBcInsInf.ins_id_cd,tiBcInsInf.line_nm);
        return -1;
    }
    //打包
    setbit_t(&iso, 0 , "0520", 4); 
    setbit_t(&iso, 3 , "510000", 6);
	aj_mchnt_cd = getenv("AJ_MCHNT_CD");
	if(aj_mchnt_cd == NULL) aj_mchnt_cd = "302430145119998";
	printf("aj_mchnt_cd=[%s]\n", aj_mchnt_cd);
    setbit_t(&iso, 42, aj_mchnt_cd, 15);

    //发送
    iLen = SendMessageToRcv(&iso, buff, sizeof(buff), &tiBcInsInf);
    if ( iLen <= 0 )
    {
        //发送给安捷失败
        return -1;
    }

    //解包 to iso
    //memset(&iso,0,sizeof(ISO_data));
    clearbit(&iso);
    i = BC_UnPack_Data( &iso, buff, iLen);
    if ( i >= 0 )
    {
        //获取返回代码
        memset(buff, 0, sizeof(buff));
        if ( (i = getbit(&iso, 28, buff)) <= 0 )
        {
            printf("获取签到接收报文#28域失败.\n");
            return -1;
        }

        if( 0!= memcmp(buff, "N00000", 6) ){
            printf("签到应答返回失败[#28域返回代码：%s].\n", buff);
            return -1;
        }
         
        //获取密钥
        if ( (i = getbit(&iso, 48, buff)) <= 0 )
        {
            printf("获取签到应答密钥失败.\n");
            return -1;
        }
        memcpy(tiBcInsInf.ins_id_cd, "00945500", 8);
        memcpy(tiBcInsInf.key_1, buff, 16);
        memcpy(tiBcInsInf.key_2, buff, 16);
        memcpy(tiBcInsInf.key_3, buff, 16);
        tiBcInsInfOpr(GLB_DB_UPDATE2, &tiBcInsInf, &sqlCode);
        if ( 0!= sqlCode )
        {
            dcs_log(0,0,"<FILE:%s,LINE:%d><BusZ00_SignInReq>更新安捷密钥失败！",__FILE__,__LINE__);
            //更新密钥失败
            return 0;
        }
    }else
    {
        printf("解包收到的安捷应答报文失败.\n");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int i;

    //打开日志文件
    if ( 0 > OpenLog(argv[0]) )
    {
        printf("打开日志失败，appSrv进程启动失败，退出.\n");
        exit(1);
    }

    //连接数据库
    if ( DasConectDB()!=0 )
    {
        printf("连接oracle DB失败.\n");
        return -1;
    }
    
    if ( IsoLoad8583config(&iso8583_AJXTconf[0],"iso8583_AJXT.conf") < 0 )
    {
        printf("装载iso8583_AJXT.conf失败,退出.\n");
        return -1;
    }
    if( 0 == SignIn() ){
        printf("签到成功\n");
    }else{
        printf("签到失败\n");
    }
}



