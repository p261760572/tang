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

    if ( u_fabricatefile("log/GDUnionPSignIn.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

int GDUnionPSignIn(char *mchnt_cd, char *term_id)
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

    //check param
    if(!mchnt_cd){
        printf("商户号不能为空.\n");
        return -1;
    }

    if(!term_id){
        printf("设备号不能为空.\n");
        return -1;
    }

    //search mchnt inf
    strcpy(tiBcQsMchntInf.rcv_ins_id_cd, "00944400");
    strcpy(tiBcQsMchntInf.mchnt_cd, mchnt_cd);
    strcpy(tiBcQsMchntInf.term_id, term_id);
    tiBcQsMchntInfOpr(GLB_DB_SELECT1, &tiBcQsMchntInf, &sqlCode);
    if ( 0 != sqlCode )
    {
        //没有找到商户信息
        printf("没有在商户资料库中找到商户号[%s]设备号[%s]sqlcode[%d].\n", tiBcQsMchntInf.mchnt_cd,tiBcQsMchntInf.term_id,sqlCode );
        return -1;
    } 

    if (tiBcQsMchntInf.openflag[0] != '1')
    {
	printf("商户号[%s]设备号[%s]没有开通!!\n",tiBcQsMchntInf.mchnt_cd,tiBcQsMchntInf.term_id);
	return -1;
    }

    //search ins inf
    i = MIN(sizeof(tiBcQsMchntInf.rcv_ins_id_cd),sizeof(tiBcQsMchntInf.rcv_ins_id_cd));
    memcpy(tiBcInsInf.ins_id_cd, tiBcQsMchntInf.rcv_ins_id_cd, i);
    tiBcInsInf.ins_id_cd[i] = 0;
    rtrim(tiBcInsInf.ins_id_cd);
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

    strcpy(tiBcMTFlowId.term_id   ,  tiBcQsMchntInf.term_id);
    strcpy(tiBcMTFlowId.mchnt_cd  ,  tiBcQsMchntInf.mchnt_cd);
    tiBcMTFlowIdOpr(GLB_DB_SELECT1, &tiBcMTFlowId, &sqlCode);
    if ( 0 != sqlCode )
    {
        printf("获取流水号失败sqlcode[%d].\n", sqlCode );
        return -1;
    }
    //打包
    setbit_t(&iso, 0 , "0800", 4); 
    setbit_t(&iso, 11, tiBcMTFlowId.sys_tra_no, 6);
    setbit_t(&iso, 41, tiBcQsMchntInf.term_id, 8);
    setbit_t(&iso, 42, (unsigned char *)tiBcQsMchntInf.mchnt_cd, 15);
    setbit_t(&iso, 60, "00000001003", 11);
    setbit_t(&iso, 63, "001", 3);

    memset(buff,0,sizeof(buff)); 
    set_msghead( &iso, "\x60\x00\x05\x00\x00\x60\x21\x13\x00\x00\x00", 11);
    buffLen = isotostr(buff, &iso);
    if ( buffLen < 0 )
    {
        printf("ISO打包失败.\n");
        return -1;
    }
    dcs_debug(buff, buffLen, "ISOSTR");
    memset( Rsp, 0, sizeof(Rsp));
    memset(g_folder_msg.data,0 ,sizeof(g_folder_msg.data));
    memcpy( g_folder_msg.data, buff, buffLen);
    memset(g_folder_msg.OutFoldName,0 ,sizeof(g_folder_msg.OutFoldName));
    rtrim(tiBcInsInf.line_nm);
    strcpy(g_folder_msg.OutFoldName, tiBcInsInf.line_nm);
    g_folder_msg.datalen= buffLen;
    g_folder_msg.OutFoldType = 1;
    BC_SendMessageToFolder(Rsp);

    return 0;
}

int main(int argc, char *argv[])
{
    int i;
    char mchnt_cd[16];
    char term_id[9];
    if ( argc<=2 )
    {
        printf("请输入:%s 商户号 设备号.\n", argv[0]);
        exit(1);
    }
    i = MIN(strlen(argv[1]), 15 );
    memcpy(mchnt_cd, argv[1], i);
    mchnt_cd[i] = 0;
    i = MIN(strlen(argv[2]), 8 );
    memcpy(term_id, argv[2], i);
    term_id[i] = 0;

    //打开日志文件
    if ( 0 > OpenLog(argv[0]) )
    {
        printf("打开日志失败，appSrv进程启动失败，退出.\n");
        exit(1);
    }

    //attach to SHM of IBDCS
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",strerror(errno),__FILE__,__LINE__);
        BC_SendExeception("04","连接SHM失败，appSrv进程启动失败，退出");
        exit(1);
    }
    g_pcBcdaShmPtr = (char *)shm_connect("BCDA",NULL);
    if ( g_pcBcdaShmPtr == NULL )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d><appSrv> cannot connect shared memory 'BCDA'!",__FILE__,__LINE__);
        BC_SendExeception("04","连接共享内存BCDA失败，appSrv进程启动失败，退出");
        return(-1);
    }

    //连接数据库
    if ( DasConectDB()!=0 )
    {
        printf("连接oracle DB失败.\n");
        return -1;
    }
    
    if ( fold_initsys() < 0 )
    {
        printf("fold_init failure.\n");
        return -1;
    }

    if ( IsoLoad8583config(&iso8583_GDYLconf[0],"iso8583_GDYL.conf") < 0 )
    {
        printf("装载iso8583_GDYL.conf失败,退出.\n");
        return -1;
    }
    if( 0 == GDUnionPSignIn(mchnt_cd,term_id) ){
        printf("向广东银联签到发送成功\n");
    }else{
        printf("向广东银联签到发送失败\n");
    }
}



