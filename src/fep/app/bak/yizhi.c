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

    if ( u_fabricatefile("log/yizhi.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

int HFTXSignIn(char *pcains_id_cd)
{
    //declare
    int i, sqlCode, iLen ;
    ISO_data iso;
    ti_bc_mt_flow_id_def tiBcMTFlowId ;
    ti_bc_qs_mchnt_inf_def tiBcQsMchntInf ;
    ti_bc_ins_inf_def tiBcInsInf;
    char buff[2048];
    char caItem[2048];
    char Rsp[1024];
    int buffLen;

    memset((char *)&tiBcInsInf, 0, sizeof(tiBcInsInf));
    strcpy(tiBcInsInf.ins_id_cd, pcains_id_cd);
    tiBcInsInfOpr(GLB_DB_SELECT1, &tiBcInsInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到机构信息
        printf("没有在机构信息表中找到接收机构信息[%s]sqlcode[%d].\n", tiBcInsInf.ins_id_cd,sqlCode );
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
    setbit_t(&iso, 33, tiBcInsInf.ins_id_cd, 8);

    memset(buff,0,sizeof(buff)); 
    buffLen = isotostr(buff, &iso);
    if ( buffLen < 0 )
    {
        printf("ISO打包失败.\n");
        return -1;
    }

    memset( Rsp, 0, sizeof(Rsp));
    memset(g_folder_msg.data,0 ,sizeof(g_folder_msg.data));
    memcpy( g_folder_msg.data, buff, buffLen);
    memset(g_folder_msg.OutFoldName,0 ,sizeof(g_folder_msg.OutFoldName));
    rtrim(tiBcInsInf.line_nm);
    strcpy(g_folder_msg.OutFoldName, tiBcInsInf.line_nm);
    rtrim(g_folder_msg.OutFoldName);
    g_folder_msg.datalen= buffLen;
    g_folder_msg.OutFoldType = 1;
    BC_SendMessageToFolder(Rsp);

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
    
    yizhi_translog_opr();
}



