#include "ibdcs.h"
#include <time.h>

//MsgData msg;

int	pclogin_proc( char * srcBuf, int  iConnSock, int iLen);

void	sysinfo_proc( char * srcBuf, int  iConnSock, int iLen );
void	sysinfofind_proc( char * srcBuf, int  iConnSock, int iLen );
void	sysinfoadd_proc( char * srcBuf, int  iConnSock, int iLen );
void	sysinfomodi_proc( char * srcBuf, int  iConnSock, int iLen );
void	sysinfodele_proc( char * srcBuf, int  iConnSock, int iLen );

void	userinfo_proc( char * srcBuf, int  iConnSock, int iLen );
void	userinfofind_proc( char * srcBuf, int  iConnSock, int iLen );
void	userinfodetafind_proc( char * srcBuf, int  iConnSock, int iLen );
void	userinfoadd_proc( char * srcBuf, int  iConnSock, int iLen );
void	userinfomodi_proc( char * srcBuf, int  iConnSock, int iLen );
void	userinfodele_proc( char * srcBuf, int  iConnSock, int iLen );

void	psaminfo_proc( char * srcBuf, int  iConnSock, int iLen );
void	psaminfofind_proc( char * srcBuf, int  iConnSock, int iLen );
void	psaminfodetafind_proc( char * srcBuf, int  iConnSock, int iLen );
void	psaminfoadd_proc( char * srcBuf, int  iConnSock, int iLen );
void	psaminfomodi_proc( char * srcBuf, int  iConnSock, int iLen );
void	psaminfodele_proc( char * srcBuf, int  iConnSock, int iLen );

void	terminfo_proc( char * srcBuf, int  iConnSock, int iLen );
void	terminfofind_proc( char * srcBuf, int  iConnSock, int iLen );
void	terminfoadd_proc( char * srcBuf, int  iConnSock, int iLen );
void	terminfoverify_proc( char * srcBuf, int  iConnSock, int iLen );

void	opinfo_proc( char * srcBuf, int  iConnSock, int iLen );
void	opinfofind_proc( char * srcBuf, int  iConnSock, int iLen );
void	opinfoadd_proc( char * srcBuf, int  iConnSock, int iLen );
void	opinfomodi_proc( char * srcBuf, int  iConnSock, int iLen );
void	opinfodele_proc( char * srcBuf, int  iConnSock, int iLen );

void	tradesearch_proc( char * srcBuf, int  iConnSock, int iLen );


//int VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac);
int VerifyPcMac(char * caKey , char * caData , int iLen , char * inPcMac);
//int GetPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * outPcMac);
int GetPcMac(char * caKey , char * caData , int iLen , char * outPcMac);

void ErrResponseForPC(char * srcBuf,char *errCode,int iConnSock, int iLen,char *msg,int msglen);
void getheadinfo(	char *srcBuf,char *m_len,char *m_info,char *m_handle,char *m_encrypt,char *m_date,char *m_time,char *m_seq, \
									char *m_status,char *m_opid,char *m_bmp_len,char *m_bmp,char *m_databuff );
									
void getpcheadinfo( char * srcBuf,int  iConnSock, int iLen );									

int zddown_proc( char * srcBuf,int  iConnSock, int iLen );
int zddownkfd_proc( char * srcBuf,int  iConnSock, int iLen );
int sendpos(char * pos_ip,char * pos_port,char * SendBuf,int SendLen,char * ReceiveBuf);
int setsendbuf(char * shopid,char * termid,char * organcode,char * SendBuf);
int verifyreceivebuf(char * ReceiveBuf,int ReceiveLen);
void shopinfofind_proc( char * srcBuf, int iConnSock, int iLen);
void shopinfodetafind_proc( char * srcBuf, int iConnSock, int iLen);
void opinfomodipsw_proc( char * srcBuf, int iConnSock, int iLen);

void shopkeyadd_proc( char * srcBuf, int iConnSock, int iLen);
void shopkeymodi_proc( char * srcBuf, int iConnSock, int iLen);
void shopkeydele_proc( char * srcBuf, int iConnSock, int iLen);

void organadd_proc( char * srcBuf, int iConnSock, int iLen);
void organmodi_proc( char * srcBuf, int iConnSock, int iLen);
void organdele_proc( char * srcBuf, int iConnSock, int iLen);


char m_workkey[16+1],m_workkey8[8+1];	//签到后得到的工作密钥密文，用于传输
char x_workkey[16+1],x_workkey8[8+1];	//签到后得到的工作密钥明文，用于计算mac值

char mm_workkey[16+1],mm_workkey8[8+1];//pos传输工作密钥密文ASC码和BCD码
char xx_workkey[16+1],xx_workkey8[8+1];//pos传输工作密钥明文ASC码和BCD码


char msg_len[4+1];          //报文长度(含本字段,右对齐,左边填'0')
char msg_info[4+1];         //消息类型
char msg_handle[4+1];       //操作类型
char msg_encrypt[2+1];      //加密方式(报文加密方式,00表示明文传输,01表示软加密传输,02表示硬件加密)
char msg_date[4+1];         //交易日期
char msg_time[6+1];         //交易时间
char msg_seq[6+1];          //交易流水
int  msg_statu[1+1];        //报文传输状态(0表示数据传输完毕,1表示还有数据未传输)
char msg_organcodelen[2+1]; //机构代码长度
char msg_organcode[20+1];     //机构代码
char msg_opid[6+1];         //操作员ID
char msg_bmplen[2+1];       //BITMAP图长度(0代表该字段没有值,1代表该字段有值)
char msg_bmp[32+1];         //BITMAP图
char msg_databuff[1024+1];	//消息数据
									
char m_op_area[256+1];
char m_term_expand[256+1];
char m_term_join[256+1];
char m_term_keep[256+1];
char m_op_type[2+1];								

char m_msg[2048];

char errmsg[100];

//char user_limit[256+1];
									
void pc_proc( char * srcBuf,int  iConnSock , int iLen ) 
{
// 1.进行签到交易处理，判断操作员的合法性及权限。
// 2.返回端签到处理结果。
// 3.进入业务部接收处理循环。若通讯口超过30分钟未发送业务处理，则关闭端口退出线程。
	

	/*dcs_log(srcBuf,iLen,"收到报文 iConnSock =%d, len=%d",iConnSock,iLen);
	
	char m_info[4+1];
	
	memset(m_info, 0, sizeof(m_info) );
	
	memcpy(m_info,srcBuf+4,4);*/
	
	getpcheadinfo( srcBuf,iConnSock,iLen );	
	
	if ( (memcmp( msg_info ,"0010",4) ==0) )    // 用户签到
	{
		dcs_log(0,0,"用户签到报文");
		pclogin_proc( srcBuf, iConnSock, iLen);
	}
	else if ( (memcmp( msg_info ,"0020",4) ==0) && memcmp( msg_handle ,"0001",4) ==0 )    // 操作员增加报文
	{
		dcs_log(0,0,"操作员增加报文");
		opinfoadd_proc( srcBuf, iConnSock, iLen);
	}
	else if ( (memcmp( msg_info ,"0020",4) ==0) && memcmp( msg_handle ,"0002",4) ==0 )    // 操作员修改报文
	{
		dcs_log(0,0,"操作员修改报文");
		opinfomodi_proc( srcBuf, iConnSock, iLen);
	}
	else if ( (memcmp( msg_info ,"0020",4) ==0) && memcmp( msg_handle ,"0003",4) ==0 )    // 操作员删除报文
	{
		dcs_log(0,0,"操作员删除报文");
		opinfodele_proc( srcBuf, iConnSock, iLen);
	}
	else if ( (memcmp( msg_info ,"0020",4) ==0) && memcmp( msg_handle ,"0005",4) ==0 )    // 操作员条件查询报文
	{
		dcs_log(0,0,"操作员条件查询报文");
		opinfofind_proc( srcBuf, iConnSock, iLen);
	}
	else if ( (memcmp( msg_info ,"0020",4) ==0) && memcmp( msg_handle ,"0010",4) ==0 )    // 操作员密码修改报文
	{
		dcs_log(0,0,"操作员密码修改报文");
		opinfomodipsw_proc( srcBuf, iConnSock, iLen);
	}
	else if ( (memcmp( msg_info ,"0030",4) ==0) && memcmp( msg_handle ,"0001",4) ==0 )    // 密钥数据增加报文
	{
		dcs_log(0,0,"密钥数据增加报文");
		shopkeyadd_proc( srcBuf, iConnSock, iLen);
	}
	else if ( (memcmp( msg_info ,"0030",4) ==0) && memcmp( msg_handle ,"0002",4) ==0 )    // 密钥数据修改报文
	{
		dcs_log(0,0,"密钥数据修改报文");
		shopkeymodi_proc( srcBuf, iConnSock, iLen);
	}
	else if ( (memcmp( msg_info ,"0030",4) ==0) && memcmp( msg_handle ,"0003",4) ==0 )    // 密钥数据删除报文
	{
		dcs_log(0,0,"密钥数据删除报文");
		shopkeydele_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( msg_info ,"0040",4) ==0 && memcmp( msg_handle ,"0007",4) ==0 )    // 密钥主动下发报文
	{
		dcs_log(0,0,"密钥主动下发报文");
		zddown_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( msg_info ,"0040",4) ==0 && memcmp( msg_handle ,"0009",4) ==0 )    // 密钥主动下发报文(客户端申请)
	{
		dcs_log(0,0,"密钥主动下发报文(客户端申请)");
		zddownkfd_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( msg_info ,"0040",4) ==0 && memcmp( msg_handle ,"0005",4) ==0 )    // 商户密钥条件查询报文
	{
		dcs_log(0,0,"商户密钥条件查询报文");
		shopinfofind_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( msg_info ,"0040",4) ==0 && memcmp( msg_handle ,"0006",4) ==0 )    // 商户密钥详细查询报文
	{
		dcs_log(0,0,"商户密钥详细查询报文");
		shopinfodetafind_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( msg_info ,"0050",4) ==0 && memcmp( msg_handle ,"0001",4) ==0 )    // 机构增加报文
	{
		dcs_log(0,0,"机构增加报文");
		organadd_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( msg_info ,"0050",4) ==0 && memcmp( msg_handle ,"0002",4) ==0 )    // 机构修改报文
	{
		dcs_log(0,0,"机构修改报文");
		organmodi_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( msg_info ,"0050",4) ==0 && memcmp( msg_handle ,"0003",4) ==0 )    // 机构删除报文
	{
		dcs_log(0,0,"机构删除报文");
		organdele_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( msg_info ,"0060",4) ==0)    // 流水查询报文
	{
		dcs_log(0,0,"流水查询报文");
		tradesearch_proc( srcBuf, iConnSock, iLen);
	}
	else
		dcs_log(0,0,"Unkown Packet,discard !");
	
}


int	pclogin_proc( char * srcBuf,int  iConnSock, int iLen )
{
	int s_len,i;
	char m_password[16+1],m_password8[8+1];
	char x_password[16+1],x_password8[8+1];
	char m_random[16+1],m_random8[8+1];
	
	char m_opnamelen[2+1];
	char m_opname[20+1];
	char m_limitlen[2+1];
	char m_limit[20+1];
	
	char sysrandom[16+1],sysrandom8[8+1];
	char x_sysrandom[16+1],x_sysrandom8[8+1];
	char mkey[16+1],mkey8[8+1];
	
	memset(m_password,0,sizeof(m_password));memset(m_password8,0,sizeof(m_password8));
	memset(x_password,0,sizeof(x_password));memset(x_password8,0,sizeof(x_password8));
	memset(m_random,0,sizeof(m_random));memset(m_random8,0,sizeof(m_random8));
	
	memset(m_opnamelen,0,sizeof(m_opnamelen));
	memset(m_opname,0,sizeof(m_opname));
	memset(m_limitlen,0,sizeof(m_limitlen));
	memset(m_limit,0,sizeof(m_limit));
	
	s_len=0;
	memcpy(m_password,msg_databuff+s_len,16);s_len=s_len+16;
	memcpy(m_random,msg_databuff+s_len,16);s_len=s_len+16;
	
	asc_to_bcd((unsigned char *)m_random8,(unsigned char *)m_random,16,0);
	asc_to_bcd((unsigned char *)m_password8,(unsigned char *)m_password,16,0);
	
	_DES(m_random8,m_password8,x_password8);//解密得到密码BCD码
	
	bcd_to_asc((unsigned char *)x_password,(unsigned char *)x_password8,16,0);
	
	if( 0>getop(msg_organcode,msg_opid,x_password,m_opname,m_limit) )
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"02","null","null",msg_organcode);
		
		dcs_log(0,0,"<pclogin_proc> getopinfo error , %s ,%s !",msg_opid,x_password);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取记录失败!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}	
	
	sprintf(m_opnamelen,"%02d",strlen(m_opname));
	sprintf(m_limitlen,"%02d",strlen(m_limit));
	dcs_log(0,0,"id:%s\npwd:%s\nname:%s\nlimit:%s",msg_opid,x_password,m_opname,m_limit);
	
	memset(sysrandom,0,sizeof(sysrandom));memset(sysrandom8,0,sizeof(sysrandom8));
	GetRandomData(sysrandom);
	
	asc_to_bcd((unsigned char *)sysrandom8,(unsigned char *)sysrandom,16,0);
	asc_to_bcd((unsigned char *)m_random8,(unsigned char *)m_random,16,0);
	
	memset(mkey,0,sizeof(mkey));memset(mkey8,0,sizeof(mkey8));
	
	for( i=0; i<8; i++)
	{
		mkey8[i]=m_random8[i]^sysrandom8[i];
	}
	bcd_to_asc((unsigned char *)mkey,(unsigned char *)mkey8,16,0);	
	
	//asc_to_bcd((unsigned char *)sysrandom8,(unsigned char *)sysrandom,16,0);
	
	memset(x_sysrandom,0,sizeof(x_sysrandom));memset(x_sysrandom8,0,sizeof(x_sysrandom8));
	GetRandomData(x_sysrandom);
	asc_to_bcd((unsigned char *)x_sysrandom8,(unsigned char *)x_sysrandom,16,0);
	
	DES(mkey8,x_sysrandom8,m_workkey8);//加密得到工作密钥密文BCD码
	bcd_to_asc((unsigned char *)m_workkey,(unsigned char *)m_workkey8,16,0);
	
	dcs_debug(0,0,"key=[%s] 工作密钥明文=[%s] DES加密得到工作密钥密文 工作密钥密文=[%s]",mkey,x_sysrandom,m_workkey);
	
	memcpy(x_workkey,x_sysrandom,16);
	memcpy(x_workkey8,x_sysrandom8,8);

	
	//组织返回报文
	struct  tm *tm;   time_t  t;
	char msg_ret[2+1];
	char msg_opnamelen[2+1];
	char msg_opname[100+1];
	char msg_limitlen[2+1];
	char msg_limit[100+1];
	char msg_wkey[16+1];
	char msg_random[16+1];
	
	
	memset(msg_info,0,sizeof(msg_info));
	memset(msg_bmplen,0,sizeof(msg_bmplen));
	memset(msg_bmp,0,sizeof(msg_bmp));
	memset(msg_ret,0,sizeof(msg_ret));
	
	memset(msg_opnamelen,0,sizeof(msg_opnamelen));
	memset(msg_opname,0,sizeof(msg_opname));
	memset(msg_limitlen,0,sizeof(msg_limitlen));
	memset(msg_limit,0,sizeof(msg_limit));
	memset(msg_wkey,0,sizeof(msg_wkey));
	memset(msg_random,0,sizeof(msg_random));
	
	memcpy(msg_info,"0011",4);
	memcpy(msg_bmplen,"04",2);
	memcpy(msg_bmp,"1111",4);
	memcpy(msg_ret,"00",2);
	memcpy(msg_opnamelen,m_opnamelen,2);
	memcpy(msg_opname,m_opname,atoi(m_opnamelen));
	memcpy(msg_limitlen,m_limitlen,2);
	memcpy(msg_limit,m_limit,atoi(m_limitlen));
	memcpy(msg_wkey,m_workkey,16);
	memcpy(msg_random,sysrandom,16);

	time(&t);
  tm = localtime(&t);
	sprintf(msg_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(msg_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	s_len=0;
	s_len=s_len+4;
	
	//消息头
	memcpy(srcBuf+s_len,msg_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_statu,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,msg_organcodelen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_organcode,atoi(msg_organcodelen));s_len=s_len+atoi(msg_organcodelen);
	memcpy(srcBuf+s_len,msg_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_bmplen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_bmp,atoi(msg_bmplen));s_len=s_len+atoi(msg_bmplen);
	
	//消息体
	memcpy(srcBuf+s_len,msg_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_opnamelen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_opname,atoi(msg_opnamelen));s_len=s_len+atoi(msg_opnamelen);
	memcpy(srcBuf+s_len,msg_limitlen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_limit,atoi(msg_limitlen));s_len=s_len+atoi(msg_limitlen);
	memcpy(srcBuf+s_len,msg_wkey,16);s_len=s_len+16;
	memcpy(srcBuf+s_len,msg_random,16);s_len=s_len+16;
	
	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
  dcs_debug(srcBuf,s_len,"<pclogin_proc> 返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
	write_msg_to_AMP(iConnSock, srcBuf, s_len,60000); 
	
	insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"00","null","null",msg_organcode);
	
	return 1;	

}

void	sysinfo_proc( char * srcBuf,int  iConnSock, int iLen  )
{
	char m_handle[4+1];
	
	memset(m_handle, 0, sizeof(m_handle) );
	
	memcpy(m_handle,srcBuf+8,4);
	
	
	if ( memcmp( m_handle ,"0004",4) ==0 )    		// 资源参数查询报文
	{
		dcs_log(0,0,"资源参数查询报文");
		sysinfofind_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0001",4) ==0)    // 资源参数增加报文
	{
		dcs_log(0,0,"资源参数增加报文");
		sysinfoadd_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0002",4) ==0)    // 资源参数修改报文
	{
		dcs_log(0,0,"资源参数修改报文");
		sysinfomodi_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0003",4) ==0)    // 资源参数删除报文
	{
		dcs_log(0,0,"资源参数删除报文");
		sysinfodele_proc( srcBuf, iConnSock, iLen);
	}
	else
	{
		dcs_log(0,0,"资源参数无效报文");
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"资源参数无效报文!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
	}
	
}


void ErrResponseForPC(char * srcBuf,char *errCode,int iConnSock, int iLen,char *errmsg,int errmsglen)
{
	//组织返回报文
	struct  tm *tm;   time_t  t;
	char msg_ret[2+1];
	char msg_errmsglen[2+1];
	char msg_errmsg[100+1];
	int s_len;
	
	memset(msg_ret,0,sizeof(msg_ret));
	memset(msg_errmsglen,0,sizeof(msg_errmsglen));
	memset(msg_errmsg,0,sizeof(msg_errmsg));
	
	if ( memcmp( msg_info ,"0010",4) ==0)
	{
		memcpy(msg_info,"0011",4);
	}
	else if ( memcmp( msg_info ,"0020",4) ==0)
	{
		memcpy(msg_info,"0021",4);
	}
	else if ( memcmp( msg_info ,"0030",4) ==0)
	{
		memcpy(msg_info,"0031",4);
	}
	else if ( memcmp( msg_info ,"0040",4) ==0)
	{
		memcpy(msg_info,"0041",4);
	}
	else if ( memcmp( msg_info ,"0050",4) ==0)
	{
		memcpy(msg_info,"0051",4);
	}
	else if ( memcmp( msg_info ,"0060",4) ==0)
	{
		memcpy(msg_info,"0061",4);
	}
	else
	{
		dcs_debug(0,0,"<ErrResponseForPC> 无效报文。");
		return;
	}	
	
	memcpy(msg_bmplen,"02",2);
	memcpy(msg_bmp,"11",2);
	memcpy(msg_ret,errCode,2);
	sprintf(msg_errmsglen,"%02d",errmsglen);
	memcpy(msg_errmsg,errmsg,errmsglen);

	time(&t);
  tm = localtime(&t);
	sprintf(msg_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(msg_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	s_len=0;
	s_len=s_len+4;
	
	//消息头
	memcpy(srcBuf+s_len,msg_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_statu,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,msg_organcodelen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_organcode,atoi(msg_organcodelen));s_len=s_len+atoi(msg_organcodelen);
	memcpy(srcBuf+s_len,msg_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_bmplen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_bmp,atoi(msg_bmplen));s_len=s_len+atoi(msg_bmplen);
	
	//消息体
	memcpy(srcBuf+s_len,msg_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_errmsglen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_errmsg,atoi(msg_errmsglen));s_len=s_len+atoi(msg_errmsglen);
	
	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
  dcs_debug(srcBuf,s_len,"<ErrResponseForPOS> 返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
	write_msg_to_AMP(iConnSock, srcBuf, s_len,60000); 	

}

void getheadinfo(	char *srcBuf,char *m_len,char *m_info,char *m_handle,char *m_encrypt,char *m_date,char *m_time,char *m_seq, \
									char *m_status,char *m_opid,char *m_bmp_len,char *m_bmp,char *m_databuff )
{
	int s_len;
	
	s_len=0;
	memcpy(m_len,srcBuf,4);s_len=s_len+4;
	memcpy(m_info,srcBuf+s_len,4);s_len=s_len+4;
	memcpy(m_handle,srcBuf+s_len,4);s_len=s_len+4;
	memcpy(m_encrypt,srcBuf+s_len,2);s_len=s_len+2;
	memcpy(m_date,srcBuf+s_len,4);s_len=s_len+4;
	memcpy(m_time,srcBuf+s_len,6);s_len=s_len+6;
	memcpy(m_seq,srcBuf+s_len,6);s_len=s_len+6;
	memcpy(m_status,srcBuf+s_len,1);s_len=s_len+1;
	memcpy(m_opid,srcBuf+s_len,6);s_len=s_len+6;
	memcpy(m_bmp_len,srcBuf+s_len,2);s_len=s_len+2;
	memcpy(m_bmp,srcBuf+s_len,atoi(m_bmp_len));s_len=s_len+atoi(m_bmp_len);
	memcpy(m_databuff,srcBuf+s_len,atoi(m_len)-s_len);
	
	dcs_log(0,0,"\
报文长度:%s\n\
消息类型:%s\n\
操作类型:%s\n\
加密方式:%s\n\
交易日期:%s\n\
交易时间:%s\n\
交易流水:%s\n\
传输状态:%s\n\
操作员ID:%s\n\
BITMAP图长度:%s\n\
BITMAP图:%s\n\
消息数据:%s",m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff);	

	addmsglog(m_info,m_handle,m_date,m_time,m_seq,m_status,m_opid);
	
}					

void getpcheadinfo( char * srcBuf,int  iConnSock, int iLen )
{
	int s_len=0;
	
	memset(msg_len, 0, sizeof(msg_len) );
	memset(msg_info, 0, sizeof(msg_info) );
	memset(msg_handle, 0, sizeof(msg_handle) );
	memset(msg_encrypt, 0, sizeof(msg_encrypt) );
	memset(msg_date, 0, sizeof(msg_date) );
	memset(msg_time, 0, sizeof(msg_time) );
	memset(msg_seq, 0, sizeof(msg_seq) );
	memset(msg_statu, 0, sizeof(msg_statu) );
	memset(msg_organcodelen, 0, sizeof(msg_organcodelen) );
	memset(msg_organcode, 0, sizeof(msg_organcode) );
	memset(msg_opid, 0, sizeof(msg_opid) );
	memset(msg_bmplen, 0, sizeof(msg_bmplen) );
	memset(msg_bmp, 0, sizeof(msg_bmp) );
	memset(msg_databuff, 0, sizeof(msg_databuff) );
		
	memcpy(msg_len,srcBuf,4);s_len=s_len+4;
	memcpy(msg_info,srcBuf+s_len,4);s_len=s_len+4;
	memcpy(msg_handle,srcBuf+s_len,4);s_len=s_len+4;
	memcpy(msg_encrypt,srcBuf+s_len,2);s_len=s_len+2;
	memcpy(msg_date,srcBuf+s_len,4);s_len=s_len+4;
	memcpy(msg_time,srcBuf+s_len,6);s_len=s_len+6;
	memcpy(msg_seq,srcBuf+s_len,6);s_len=s_len+6;
	memcpy(msg_statu,srcBuf+s_len,1);s_len=s_len+1;
	memcpy(msg_organcodelen,srcBuf+s_len,2);s_len=s_len+2;
	memcpy(msg_organcode,srcBuf+s_len,atoi(msg_organcodelen));s_len=s_len+atoi(msg_organcodelen);
	memcpy(msg_opid,srcBuf+s_len,6);s_len=s_len+6;
	memcpy(msg_bmplen,srcBuf+s_len,2);s_len=s_len+2;
	memcpy(msg_bmp,srcBuf+s_len,atoi(msg_bmplen));s_len=s_len+atoi(msg_bmplen);
	memcpy(msg_databuff,srcBuf+s_len,iLen-s_len);		
	
	dcs_log(0,0,"<getposheadinfo>\n\
报文长度:%s\n\
消息类型:%s\n\
操作类型:%s\n\
加密方式:%s\n\
交易日期:%s\n\
交易时间:%s\n\
交易流水:%s\n\
传输状态:%s\n\
机构代码长度:%s\n\
机构代码:%s\n\
操作员ID:%s\n\
BITMAP图长度:%s\n\
BITMAP图:%s\n\
消息数据:%s",msg_len,msg_info,msg_handle,msg_encrypt,msg_date,msg_time,msg_seq,msg_statu,msg_organcodelen,msg_organcode,msg_opid,msg_bmplen,msg_bmp,msg_databuff);	
	
}

void	sysinfofind_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据
	char type_id[6+1];
	char para_id[6+1];
	char m_bz[1+1];
	int s_len=0;
	char m_mac[16+1];
	char m_mkey[16+1];
	char m_wkey[16+1];
	char s_ret[2+1];
	char s_infobuff_len[3+1];
	char s_infobuff[2048+1];
	
	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );

	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	

	memset(type_id, 0,sizeof(type_id) );
	memset(para_id, 0,sizeof(para_id) );
	memset(m_bz, 0,sizeof(m_bz) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	

	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(type_id,m_databuff+s_len,6);
		s_len=s_len+6;
	}
	
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(para_id,m_databuff+s_len,6);
		s_len=s_len+6;
	}
	
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(m_bz,m_databuff+s_len,1);
		s_len=s_len+1;
	}	
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"type_id:%s\npara_id:%s\nm_bz:%s\nm_mac:%s",type_id,para_id,m_bz,m_mac);
	
	
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	
	
	memset(s_ret,0,sizeof(s_ret));
	memset(s_infobuff,0,sizeof(s_infobuff));
	dcs_debug(0,0,"<sysinfofind_proc>	entry getparainfo");
	if(0>getparainfo(type_id,para_id,m_bz,s_infobuff,m_op_area,m_term_expand,m_term_join,m_term_keep,m_op_type))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"参数资源表获取记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"type_id:%s,para_id:%s,m_bz:%s,m_mac:%s",type_id,para_id,m_bz,m_mac);
	//upmsglog(m_date,m_time,m_seq,m_msg,"00");
	
	if(strlen(s_infobuff)>0)
		sprintf(s_infobuff_len,"%03d",strlen(s_infobuff)-1);
	else	sprintf(s_infobuff_len,"%03d",0);
	
	
	//组织返回报文
	struct  tm *tm;   time_t  t;	
	
	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	memcpy(m_info,"0051",4);
	
	memcpy(m_bmp_len,"02",2);
	memcpy(m_bmp,"11",2);
	
	
	s_len=0;
	s_len=s_len+4;
	
	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,2);s_len=s_len+2;
	
	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);
	
	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,s_infobuff_len,3);s_len=s_len+3;
	memcpy(srcBuf+s_len,s_infobuff,atoi(s_infobuff_len));s_len=s_len+atoi(s_infobuff_len);
	
	//VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-21 , m_mac))
	//GetPcMac(caMKey,caWKey,caData,iLen,m_mac);
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;
	
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);
	
	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	
	
	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	sysinfoadd_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char type_id[6+1];
	char para_id[6+1];
	char m_name_len[2+1];
	char m_name[256+1];
	char memo_len[3+1];
	char memo[256+1];
	
	char m_mac[16+1];
	
	memset(type_id, 0,sizeof(type_id) );
	memset(para_id, 0,sizeof(para_id) );
	memset(m_name_len, 0,sizeof(m_name_len) );
	memset(m_name, 0,sizeof(m_name) );
	memset(memo_len, 0,sizeof(memo_len) );
	memset(memo, 0,sizeof(memo) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(type_id,m_databuff+s_len,6);
		s_len=s_len+6;
	}
	
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(para_id,m_databuff+s_len,6);
		s_len=s_len+6;
	}
	
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(m_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(m_name,m_databuff+s_len,atoi(m_name_len));s_len=s_len+atoi(m_name_len);
	}
	
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(memo_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(memo,m_databuff+s_len,atoi(memo_len));s_len=s_len+atoi(memo_len);
	}
	
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"type_id:%s\npara_id:%s\nname:%s\nmemo:%s\nm_mac:%s",type_id,para_id,m_name,memo,m_mac);
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}	*/
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>addparainfo(type_id,para_id,m_name,memo))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"参数资源表增加记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<参数资源表增加> type_id:%s,para_id:%s,name:%s,memo:%s,m_mac:%s",type_id,para_id,m_name,memo,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0051",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	sysinfomodi_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char type_id[6+1];
	char para_id[6+1];
	char m_name_len[2+1];
	char m_name[256+1];
	char memo_len[3+1];
	char memo[256+1];
	
	char m_mac[16+1];
	
	memset(type_id, 0,sizeof(type_id) );
	memset(para_id, 0,sizeof(para_id) );
	memset(m_name_len, 0,sizeof(m_name_len) );
	memset(m_name, 0,sizeof(m_name) );
	memset(memo_len, 0,sizeof(memo_len) );
	memset(memo, 0,sizeof(memo) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(type_id,m_databuff+s_len,6);
		s_len=s_len+6;
	}
	
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(para_id,m_databuff+s_len,6);
		s_len=s_len+6;
	}
	
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(m_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(m_name,m_databuff+s_len,atoi(m_name_len));s_len=s_len+atoi(m_name_len);
	}
	
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(memo_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(memo,m_databuff+s_len,atoi(memo_len));s_len=s_len+atoi(memo_len);
	}
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"type_id:%s\npara_id:%s\nname:%s\nmemo:%s\nm_mac:%s",type_id,para_id,m_name,memo,m_mac);
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}	*/
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>modiparainfo(type_id,para_id,m_name,memo))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"参数资源表修改记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<参数资源表修改> type_id:%s,para_id:%s,name:%s,memo:%s,m_mac:%s",type_id,para_id,m_name,memo,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0051",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	sysinfodele_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char type_id[6+1];
	char para_id[6+1];
	char m_name_len[2+1];
	char m_name[256+1];
	char memo_len[3+1];
	char memo[256+1];
	
	char m_mac[16+1];
	
	memset(type_id, 0,sizeof(type_id) );
	memset(para_id, 0,sizeof(para_id) );
	memset(m_name_len, 0,sizeof(m_name_len) );
	memset(m_name, 0,sizeof(m_name) );
	memset(memo_len, 0,sizeof(memo_len) );
	memset(memo, 0,sizeof(memo) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(type_id,m_databuff+s_len,6);
		s_len=s_len+6;
	}
	
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(para_id,m_databuff+s_len,6);
		s_len=s_len+6;
	}
	
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(m_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(m_name,m_databuff+s_len,atoi(m_name_len));s_len=s_len+atoi(m_name_len);
	}
	
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(memo_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(memo,m_databuff+s_len,atoi(memo_len));s_len=s_len+atoi(memo_len);
	}
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"type_id:%s\npara_id:%s\nname:%s\nmemo:%s\nm_mac:%s",type_id,para_id,m_name,memo,m_mac);
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}	*/
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>deleparainfo(type_id,para_id))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"参数资源表删除记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}

	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<参数资源表删除> type_id:%s,para_id:%s,name:%s,memo:%s,m_mac:%s",type_id,para_id,m_name,memo,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0051",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	userinfo_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_handle[4+1];
	
	memset(m_handle, 0, sizeof(m_handle) );
	
	memcpy(m_handle,srcBuf+8,4);
	
	
	if ( memcmp( m_handle ,"0005",4) ==0 )    		// 用户信息查询报文
	{
		dcs_log(0,0,"用户信息查询报文");
		userinfofind_proc( srcBuf, iConnSock, iLen);
	}
	if ( memcmp( m_handle ,"0006",4) ==0 )    		// 用户详细信息查询报文
	{
		dcs_log(0,0,"用户详细信息查询报文");
		userinfodetafind_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0001",4) ==0)    // 用户信息增加报文
	{
		dcs_log(0,0,"用户信息增加报文");
		userinfoadd_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0002",4) ==0)    // 用户信息修改报文
	{
		dcs_log(0,0,"用户信息修改报文");
		userinfomodi_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0003",4) ==0)    // 用户信息删除报文
	{
		dcs_log(0,0,"用户信息删除报文");
		userinfodele_proc( srcBuf, iConnSock, iLen);
	}
	
}

void	psaminfo_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_handle[4+1];
	
	memset(m_handle, 0, sizeof(m_handle) );
	
	memcpy(m_handle,srcBuf+8,4);
	
	
	if ( memcmp( m_handle ,"0005",4) ==0 )    		// PSAM卡信息查询报文
	{
		dcs_log(0,0,"PSAM卡信息查询报文");
		psaminfofind_proc( srcBuf, iConnSock, iLen);
	}
	if ( memcmp( m_handle ,"0006",4) ==0 )    		// PSAM卡详细信息查询报文
	{
		dcs_log(0,0,"PSAM卡详细信息查询报文");
		psaminfodetafind_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0001",4) ==0)    // PSAM卡信息增加报文
	{
		dcs_log(0,0,"PSAM卡信息增加报文");
		psaminfoadd_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0002",4) ==0)    // PSAM卡信息修改报文
	{
		dcs_log(0,0,"PSAM卡信息修改报文");
		psaminfomodi_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0003",4) ==0)    // PSAM卡信息删除报文
	{
		dcs_log(0,0,"PSAM卡信息删除报文");
		psaminfodele_proc( srcBuf, iConnSock, iLen);
	}
		
}

void	terminfo_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_handle[4+1];
	
	memset(m_handle, 0, sizeof(m_handle) );
	
	memcpy(m_handle,srcBuf+8,4);
	
	
	if ( memcmp( m_handle ,"0005",4) ==0 )    		// 终端信息查询报文
	{
		dcs_log(0,0,"终端信息查询报文");
		terminfofind_proc( srcBuf, iConnSock, iLen);
	}
	else if ( (memcmp( m_handle ,"0007",4) ==0) )    // 终端信息开通报文
	{
		dcs_log(0,0,"终端信息开通申请报文");
		terminfoadd_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0008",4) ==0 || \
						memcmp( m_handle ,"0009",4) ==0 || \
						memcmp( m_handle ,"0010",4) ==0 || \
						memcmp( m_handle ,"0011",4) ==0 || \
						memcmp( m_handle ,"0012",4) ==0 )    // 终端信息操作申请或审核或驳回报文
	{
		dcs_log(0,0," 终端信息操作申请或审核或驳回报文");
		terminfoverify_proc( srcBuf, iConnSock, iLen);
	}
			
}

void	opinfo_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_handle[4+1];
	memset(m_handle, 0, sizeof(m_handle) );
	
	memcpy(m_handle,srcBuf+8,4);
	
	
	if ( memcmp( m_handle ,"0005",4) ==0 )    		// 系统用户查询报文
	{
		dcs_log(0,0,"系统用户查询报文");
		opinfofind_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0001",4) ==0)    // 系统用户增加报文
	{
		dcs_log(0,0,"系统用户增加报文");
		opinfoadd_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0002",4) ==0)    // 系统用户修改报文
	{
		dcs_log(0,0,"系统用户修改报文");
		opinfomodi_proc( srcBuf, iConnSock, iLen);
	}
	else if ( memcmp( m_handle ,"0003",4) ==0)    // 系统用户删除报文
	{
		dcs_log(0,0,"系统用户删除报文");
		opinfodele_proc( srcBuf, iConnSock, iLen);
	}
				
}

void	tradesearch_proc( char * srcBuf, int  iConnSock, int iLen )
{
}

void	userinfofind_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );

	char user_name_len[2+1];
	char user_name[256+1];
	char cover[4+1];
	char tel_type_len[2+1];
	char tel_type[256+1];
	char tel_number_len[2+1];
	char tel_number[256+1];
	char address_len[3+1];
	char address[256+1];
	char tel_contact_len[2+1];
	char tel_contact[256+1];
	char certificate_type_len[2+1];
	char certificate_type[256+1];
	char certificate_number_len[2+1];
	char certificate_number[256+1];
	char area[6+1];
	char expand[6+1];
	char begin_page[6+1];
	char memo_len[3+1];
	char memo[256+1];
	char m_mac[16+1];

	memset(user_name_len, 0,sizeof(user_name_len) );
	memset(user_name, 0,sizeof(user_name) );
	memset(tel_number_len, 0,sizeof(tel_number_len) );
	memset(tel_number, 0,sizeof(tel_number) );
	memset(certificate_number_len, 0,sizeof(certificate_number_len) );
	memset(certificate_number, 0,sizeof(certificate_number) );
	memset(area, 0,sizeof(area) );
	memset(expand, 0,sizeof(expand) );
	memset(begin_page, 0,sizeof(begin_page) );
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	char whe[1024+1];
	char whe0[1024+1];
	char whe1[128+1];
	char whe2[128+1];
	char whe3[128+1];
	char whe4[128+1];
	char whe5[128+1];
	memset(whe,0,sizeof(whe));
	memset(whe0,0,sizeof(whe0));
	memset(whe1,0,sizeof(whe1));
	memset(whe2,0,sizeof(whe2));
	memset(whe3,0,sizeof(whe3));
	memset(whe4,0,sizeof(whe4));
	memset(whe5,0,sizeof(whe5));
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(user_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(user_name,m_databuff+s_len,atoi(user_name_len));s_len=s_len+atoi(user_name_len);
		
		sprintf(whe1,"user_name like '%s%s%s' and ","%",user_name,"%");
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(tel_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_number,m_databuff+s_len,atoi(tel_number_len));s_len=s_len+atoi(tel_number_len);
		
		sprintf(whe2,"tel_number like '%s%s%s' and ","%",tel_number,"%");
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(certificate_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(certificate_number,m_databuff+s_len,atoi(certificate_number_len));s_len=s_len+atoi(certificate_number_len);
		
		sprintf(whe3,"certificate_number like '%s%s%s' and ","%",certificate_number,"%");
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(area,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(whe4,"area like '%s%s%s' and ","%",area,"%");
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(expand,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(whe5,"expand like '%s%s%s' and ","%",expand,"%");
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(begin_page,m_databuff+s_len,6);s_len=s_len+6;
	}	
	
	sprintf(whe0,"%s%s%s%s%s",whe1,whe2,whe3,whe4,whe5);
	if(strlen(whe0)!=0)	memcpy(whe,whe0,strlen(whe0)-5);
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"user_name_len:%s\nuser_name:%s\ntel_number_len:%s\ntel_number:%s\ncertificate_number_len:%s\ncertificate_number:%s\narea:%s\nexpand:%s\nbegin_page:%s\nm_mac:%s\n", \
	user_name_len,user_name,tel_number_len,tel_number,certificate_number_len,certificate_number,area,expand,begin_page,m_mac);
	
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	char s_ret[2+1];
	char s_infobuff_len[3+1];
	char s_infobuff[2048+1];
	
	memset(s_ret,0,sizeof(s_ret));
	memset(s_infobuff,0,sizeof(s_infobuff));
	
	char over_bz[1+1];
	memset(over_bz,0,sizeof(over_bz));

	if(0>getuserinfo(whe,s_infobuff,atoi(begin_page),atoi(begin_page)+7,over_bz,m_op_area,m_term_expand,m_term_join,m_term_keep,m_op_type))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"用户资料表获取记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}

	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"user_name_len:%s,user_name:%s,tel_number_len:%s,tel_number:%s,certificate_number_len:%s,certificate_number:%s,area:%s,expand:%s,begin_page:%s,m_mac:%s", \
	user_name_len,user_name,tel_number_len,tel_number,certificate_number_len,certificate_number,area,expand,begin_page,m_mac);
	//upmsglog(m_date,m_time,m_seq,m_msg,"00");
	
	if(memcmp(over_bz,"1",1)==0)	memcpy(m_status,"0",1);
	else	memcpy(m_status,"1",1);
	
	if(strlen(s_infobuff)>0)
		sprintf(s_infobuff_len,"%03d",strlen(s_infobuff)-1);
	else	sprintf(s_infobuff_len,"%03d",0);
	
	
	//组织返回报文
	struct  tm *tm;   time_t  t;	
	
	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	memcpy(m_info,"0021",4);
	
	memcpy(m_bmp_len,"02",2);
	memcpy(m_bmp,"11",2);
	
	
	s_len=0;
	s_len=s_len+4;
	
	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,2);s_len=s_len+2;
	
	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);
	
	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,s_infobuff_len,3);s_len=s_len+3;
	memcpy(srcBuf+s_len,s_infobuff,atoi(s_infobuff_len));s_len=s_len+atoi(s_infobuff_len);
	
	//VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-21 , m_mac))
	//GetPcMac(caMKey,caWKey,caData,iLen,m_mac);
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;
	
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);
	
	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	
	
	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	userinfodetafind_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );

	char user_id[6+1];
	char m_mac[16+1];

	memset(user_id, 0,sizeof(user_id) );
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(user_id,m_databuff+s_len,6);s_len=s_len+6;
	}
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"user_id:%s\n",user_id);
	
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	char s_ret[2+1];
	char user_name_len[2+1];
	char user_name[256+1];
	char cover[4+1];
	char tel_type_len[2+1];
	char tel_type[256+1];
	char tel_number_len[2+1];
	char tel_number[256+1];
	char address_len[3+1];
	char address[256+1];
	char tel_contact_len[2+1];
	char tel_contact[256+1];
	char certificate_type_len[2+1];
	char certificate_type[256+1];
	char certificate_number_len[2+1];
	char certificate_number[256+1];
	char area[6+1];
	char expand[6+1];
	char memo_len[3+1];
	char memo[256+1];
	
	memset(s_ret,0,sizeof(s_ret));
	memset(user_name_len,0,sizeof(user_name_len));
	memset(user_name,0,sizeof(user_name));
	memset(cover,0,sizeof(cover));
	memset(tel_type_len,0,sizeof(tel_type_len));
	memset(tel_type,0,sizeof(tel_type));
	memset(tel_number_len,0,sizeof(tel_number_len));
	memset(tel_number,0,sizeof(tel_number));
	memset(address_len,0,sizeof(address_len));
	memset(address,0,sizeof(address));
	memset(tel_contact_len,0,sizeof(tel_contact_len));
	memset(tel_contact,0,sizeof(tel_contact));
	memset(certificate_type_len,0,sizeof(certificate_type_len));
	memset(certificate_type,0,sizeof(certificate_type));
	memset(certificate_number_len,0,sizeof(certificate_number_len));
	memset(certificate_number,0,sizeof(certificate_number));
	memset(area,0,sizeof(area));
	memset(expand,0,sizeof(expand));
	memset(memo_len,0,sizeof(memo_len));
	memset(memo,0,sizeof(memo));
	
	
	if(getuserinfodeta(user_id, user_name, cover, tel_type, tel_number, address, tel_contact, certificate_type, certificate_number, area, expand, memo)>0)
	{
		sprintf(user_name_len,"%02d",strlen(user_name));
		sprintf(tel_type_len,"%02d",strlen(tel_type));
		sprintf(tel_number_len,"%02d",strlen(tel_number));
		sprintf(address_len,"%03d",strlen(address));
		sprintf(tel_contact_len,"%02d",strlen(tel_contact));
		sprintf(certificate_type_len,"%02d",strlen(certificate_type));
		sprintf(certificate_number_len,"%02d",strlen(certificate_number));
		sprintf(memo_len,"%03d",strlen(memo));
		
		dcs_log(0,0,"user_id:%s\nuser_name:%s\ncover:%s\ntel_type:%s\ntel_number:%s\naddress:%s\ntel_contact:%s\ncertificate_type:%s\ncertificate_number:%s\narea:%s\nexpand:%s\nmemo:%s\n",user_id, user_name, cover, tel_type, tel_number, address, tel_contact, certificate_type, certificate_number, area, expand, memo);
	}
	else
	{
		
		dcs_log(0,0,"getuserinfodeta error !");

		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"用户资料表获取记录操作失败!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
		
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"user_id:%s,user_name:%s,cover:%s,tel_type:%s,tel_number:%s,address:%s,tel_contact:%s,certificate_type:%s,certificate_number:%s,area:%s,expand:%s,memo:%s",user_id, user_name, cover, tel_type, tel_number, address, tel_contact, certificate_type, certificate_number, area, expand, memo);
	//upmsglog(m_date,m_time,m_seq,m_msg,"00");
	
	
	//组织返回报文
	struct  tm *tm;   time_t  t;	
	
	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	memcpy(m_info,"0021",4);
	
	memcpy(m_bmp_len,"12",2);
	memcpy(m_bmp,"111111111111",12);
	
	
	s_len=0;
	s_len=s_len+4;
	
	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,12);s_len=s_len+12;
	
	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);
	
	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	//memcpy(srcBuf+s_len,s_infobuff_len,3);s_len=s_len+3;
	//memcpy(srcBuf+s_len,s_infobuff,atoi(s_infobuff_len));s_len=s_len+atoi(s_infobuff_len);
	
	memcpy(srcBuf+s_len,user_name_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,user_name,atoi(user_name_len));s_len=s_len+atoi(user_name_len);
	memcpy(srcBuf+s_len,cover,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,tel_type_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,tel_type,atoi(tel_type_len));s_len=s_len+atoi(tel_type_len);
	memcpy(srcBuf+s_len,tel_number_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,tel_number,atoi(tel_number_len));s_len=s_len+atoi(tel_number_len);
	memcpy(srcBuf+s_len,address_len,3);s_len=s_len+3;
	memcpy(srcBuf+s_len,address,atoi(address_len));s_len=s_len+atoi(address_len);
	memcpy(srcBuf+s_len,tel_contact_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,tel_contact,atoi(tel_contact_len));s_len=s_len+atoi(tel_contact_len);
	memcpy(srcBuf+s_len,certificate_type_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,certificate_type,atoi(certificate_type_len));s_len=s_len+atoi(certificate_type_len);
	memcpy(srcBuf+s_len,certificate_number_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,certificate_number,atoi(certificate_number_len));s_len=s_len+atoi(certificate_number_len);
	memcpy(srcBuf+s_len,area,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,expand,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,memo_len,3);s_len=s_len+3;
	memcpy(srcBuf+s_len,memo,atoi(memo_len));s_len=s_len+atoi(memo_len);
	
	
	//VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-21 , m_mac))
	//GetPcMac(caMKey,caWKey,caData,iLen,m_mac);
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;
	
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);
	
	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	
	
	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	userinfoadd_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char user_name_len[2+1];
	char user_name[256+1];
	char cover[4+1];
	char tel_type_len[2+1];
	char tel_type[256+1];
	char tel_number_len[2+1];
	char tel_number[256+1];
	char address_len[3+1];
	char address[256+1];
	char tel_contact_len[2+1];
	char tel_contact[256+1];
	char certificate_type_len[2+1];
	char certificate_type[256+1];
	char certificate_number_len[2+1];
	char certificate_number[256+1];
	char area[6+1];
	char expand[6+1];
	char memo_len[3+1];
	char memo[256+1];
	char m_mac[16+1];

	memset(user_name_len, 0,sizeof(user_name_len) );
	memset(user_name, 0,sizeof(user_name) );
	memset(cover, 0,sizeof(cover) );
	memset(tel_type_len, 0,sizeof(tel_type_len) );
	memset(tel_type, 0,sizeof(tel_type) );
	memset(tel_number_len, 0,sizeof(tel_number_len) );
	memset(tel_number, 0,sizeof(tel_number) );
	memset(address_len, 0,sizeof(address_len) );
	memset(address, 0,sizeof(address) );
	memset(tel_contact_len, 0,sizeof(tel_contact_len) );
	memset(tel_contact, 0,sizeof(tel_contact) );
	memset(certificate_type_len, 0,sizeof(certificate_type_len) );
	memset(certificate_type, 0,sizeof(certificate_type) );
	memset(certificate_number_len, 0,sizeof(certificate_number_len) );
	memset(certificate_number, 0,sizeof(certificate_number) );
	memset(area, 0,sizeof(area) );
	memset(expand, 0,sizeof(expand) );
	memset(memo_len, 0,sizeof(memo_len) );
	memset(memo, 0,sizeof(memo) );
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(user_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(user_name,m_databuff+s_len,atoi(user_name_len));s_len=s_len+atoi(user_name_len);
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(cover,m_databuff+s_len,4);s_len=s_len+4;
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(tel_type_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_type,m_databuff+s_len,atoi(tel_type_len));s_len=s_len+atoi(tel_type_len);
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(tel_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_number,m_databuff+s_len,atoi(tel_number_len));s_len=s_len+atoi(tel_number_len);
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(address_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(address,m_databuff+s_len,atoi(address_len));s_len=s_len+atoi(address_len);
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(tel_contact_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_contact,m_databuff+s_len,atoi(tel_contact_len));s_len=s_len+atoi(tel_contact_len);
	}
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(certificate_type_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(certificate_type,m_databuff+s_len,atoi(certificate_type_len));s_len=s_len+atoi(certificate_type_len);
	}
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		memcpy(certificate_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(certificate_number,m_databuff+s_len,atoi(certificate_number_len));s_len=s_len+atoi(certificate_number_len);
	}
	if(memcmp(m_bmp+8,"1",1)==0)
	{
		memcpy(area,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+9,"1",1)==0)
	{
		memcpy(expand,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+10,"1",1)==0)
	{
		memcpy(memo_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(memo,m_databuff+s_len,atoi(memo_len));s_len=s_len+atoi(memo_len);
	}
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_debug(0,0,"user_name_len:%s\nuser_name:%s\ncover:%s\ntel_type_len:%s\ntel_type:%s\ntel_number_len:%s\ntel_number:%s\naddress_len:%s\naddress:%s\ntel_contact_len:%s\ntel_contact:%s\ncertificate_type_len:%s\ncertificate_type:%s\ncertificate_number_len:%s\ncertificate_number:%s\narea:%s\nexpand:%s\nmemo_len:%s\nmemo:%s\nm_mac:%s\n", \
	user_name_len,user_name,cover,tel_type_len,tel_type,tel_number_len,tel_number,address_len,address,tel_contact_len,tel_contact,certificate_type_len,certificate_type,certificate_number_len,certificate_number,area,expand,memo_len,memo,m_mac);

	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	dcs_debug(0,0,"begin verify mac ");
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	dcs_debug(0,0," verify mac flinsh");
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	memset(errmsg,0,sizeof(errmsg));
	dcs_debug(0,0,"begin add user");
	if(0>adduserinfo(user_name,cover,tel_type,tel_number,address,tel_contact,certificate_type,certificate_number,area,expand,memo,errmsg))
	{
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	dcs_debug(0,0," add user succ");
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<用户资料表增加> user_name_len:%s,user_name:%s,cover:%s,tel_type_len:%s,tel_type:%s,tel_number_len:%s,tel_number:%s,address_len:%s,address:%s,tel_contact_len:%s,tel_contact:%s,certificate_type_len:%s,certificate_type:%s,certificate_number_len:%s,certificate_number:%s,area:%s,expand:%s,memo_len:%s,memo:%s,m_mac:%s", \
	user_name_len,user_name,cover,tel_type_len,tel_type,tel_number_len,tel_number,address_len,address,tel_contact_len,tel_contact,certificate_type_len,certificate_type,certificate_number_len,certificate_number,area,expand,memo_len,memo,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0021",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	userinfomodi_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char user_id[6+1];
	char user_name_len[2+1];
	char user_name[256+1];
	char cover[4+1];
	char tel_type_len[2+1];
	char tel_type[256+1];
	char tel_number_len[2+1];
	char tel_number[256+1];
	char address_len[3+1];
	char address[256+1];
	char tel_contact_len[2+1];
	char tel_contact[256+1];
	char certificate_type_len[2+1];
	char certificate_type[256+1];
	char certificate_number_len[2+1];
	char certificate_number[256+1];
	char area[6+1];
	char expand[6+1];
	char memo_len[3+1];
	char memo[256+1];
	char m_mac[16+1];

	memset(user_id,0,sizeof(user_id) );
	memset(user_name_len, 0,sizeof(user_name_len) );
	memset(user_name, 0,sizeof(user_name) );
	memset(cover, 0,sizeof(cover) );
	memset(tel_type_len, 0,sizeof(tel_type_len) );
	memset(tel_type, 0,sizeof(tel_type) );
	memset(tel_number_len, 0,sizeof(tel_number_len) );
	memset(tel_number, 0,sizeof(tel_number) );
	memset(address_len, 0,sizeof(address_len) );
	memset(address, 0,sizeof(address) );
	memset(tel_contact_len, 0,sizeof(tel_contact_len) );
	memset(tel_contact, 0,sizeof(tel_contact) );
	memset(certificate_type_len, 0,sizeof(certificate_type_len) );
	memset(certificate_type, 0,sizeof(certificate_type) );
	memset(certificate_number_len, 0,sizeof(certificate_number_len) );
	memset(certificate_number, 0,sizeof(certificate_number) );
	memset(area, 0,sizeof(area) );
	memset(expand, 0,sizeof(expand) );
	memset(memo_len, 0,sizeof(memo_len) );
	memset(memo, 0,sizeof(memo) );
	memset(m_mac, 0,sizeof(m_mac) );
	
	
	char up_data[1024+1];
	char up_data0[1024+1];
	char up_data1[128+1];
	char up_data2[128+1];
	char up_data3[128+1];
	char up_data4[128+1];
	char up_data5[128+1];
	char up_data6[128+1];
	char up_data7[128+1];
	char up_data8[128+1];
	char up_data9[128+1];
	char up_data10[128+1];
	char up_data11[128+1];
	
	memset(up_data,0,sizeof(up_data));
	memset(up_data0,0,sizeof(up_data0));
	memset(up_data1,0,sizeof(up_data1));
	memset(up_data2,0,sizeof(up_data2));
	memset(up_data3,0,sizeof(up_data3));
	memset(up_data4,0,sizeof(up_data4));
	memset(up_data5,0,sizeof(up_data5));
	memset(up_data6,0,sizeof(up_data6));
	memset(up_data7,0,sizeof(up_data7));
	memset(up_data8,0,sizeof(up_data8));
	memset(up_data9,0,sizeof(up_data9));
	memset(up_data10,0,sizeof(up_data10));
	memset(up_data11,0,sizeof(up_data11));
	
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(user_id,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(user_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(user_name,m_databuff+s_len,atoi(user_name_len));s_len=s_len+atoi(user_name_len);
		
		sprintf(up_data1,"user_name='%s',",user_name);
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(cover,m_databuff+s_len,4);s_len=s_len+4;
		
		sprintf(up_data2,"cover='%s',",cover);
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(tel_type_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_type,m_databuff+s_len,atoi(tel_type_len));s_len=s_len+atoi(tel_type_len);
		
		sprintf(up_data3,"tel_type='%s',",tel_type);
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(tel_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_number,m_databuff+s_len,atoi(tel_number_len));s_len=s_len+atoi(tel_number_len);
		
		sprintf(up_data4,"tel_number='%s',",tel_number);
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(address_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(address,m_databuff+s_len,atoi(address_len));s_len=s_len+atoi(address_len);
		
		sprintf(up_data5,"address='%s',",address);
	}
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(tel_contact_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_contact,m_databuff+s_len,atoi(tel_contact_len));s_len=s_len+atoi(tel_contact_len);
		
		sprintf(up_data6,"tel_contact='%s',",tel_contact);
	}
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		memcpy(certificate_type_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(certificate_type,m_databuff+s_len,atoi(certificate_type_len));s_len=s_len+atoi(certificate_type_len);
		
		sprintf(up_data7,"certificate_type='%s',",certificate_type);
	}
	if(memcmp(m_bmp+8,"1",1)==0)
	{
		memcpy(certificate_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(certificate_number,m_databuff+s_len,atoi(certificate_number_len));s_len=s_len+atoi(certificate_number_len);
		
		sprintf(up_data8,"certificate_number='%s',",certificate_number);
	}
	if(memcmp(m_bmp+9,"1",1)==0)
	{
		memcpy(area,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data9,"area='%s',",area);
	}
	if(memcmp(m_bmp+10,"1",1)==0)
	{
		memcpy(expand,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data10,"expand='%s',",expand);
	}
	if(memcmp(m_bmp+11,"1",1)==0)
	{
		memcpy(memo_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(memo,m_databuff+s_len,atoi(memo_len));s_len=s_len+atoi(memo_len);
		
		sprintf(up_data11,"memo='%s',",memo);
	}
	
	sprintf(up_data0,"%s%s%s%s%s%s%s%s%s%s%s",up_data1,up_data2,up_data3,up_data4,up_data5,up_data6,up_data7,up_data8,up_data9,up_data10,up_data11);
	if(strlen(up_data0)!=0)
	{
		memcpy(up_data,"set ",4);
		memcpy(up_data+4,up_data0,strlen(up_data0)-1);
	}
	
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"user_id:%s\nuser_name_len:%s\nuser_name:%s\ncover:%s\ntel_type_len:%s\ntel_type:%s\ntel_number_len:%s\ntel_number:%s\naddress_len:%s\naddress:%s\ntel_contact_len:%s\ntel_contact:%s\ncertificate_type_len:%s\ncertificate_type:%s\ncertificate_number_len:%s\ncertificate_number:%s\narea:%s\nexpand:%s\nmemo_len:%s\nmemo:%s\nm_mac:%s\n", \
	user_id,user_name_len,user_name,cover,tel_type_len,tel_type,tel_number_len,tel_number,address_len,address,tel_contact_len,tel_contact,certificate_type_len,certificate_type,certificate_number_len,certificate_number,area,expand,memo_len,memo,m_mac);
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}	*/
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>modiuserinfo(user_id,up_data))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"用户资料表修改记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<用户资料表修改> user_id:%s,user_name_len:%s,user_name:%s,cover:%s,tel_type_len:%s,tel_type:%s,tel_number_len:%s,tel_number:%s,address_len:%s,address:%s,tel_contact_len:%s,tel_contact:%s,certificate_type_len:%s,certificate_type:%s,certificate_number_len:%s,certificate_number:%s,area:%s,expand:%s,memo_len:%s,memo:%s,m_mac:%s", \
	user_id,user_name_len,user_name,cover,tel_type_len,tel_type,tel_number_len,tel_number,address_len,address,tel_contact_len,tel_contact,certificate_type_len,certificate_type,certificate_number_len,certificate_number,area,expand,memo_len,memo,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0021",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	userinfodele_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char user_id[6+1];
	char user_name_len[2+1];
	char user_name[256+1];
	char cover[4+1];
	char tel_type_len[2+1];
	char tel_type[256+1];
	char tel_number_len[2+1];
	char tel_number[256+1];
	char address_len[3+1];
	char address[256+1];
	char tel_contact_len[2+1];
	char tel_contact[256+1];
	char certificate_type_len[2+1];
	char certificate_type[256+1];
	char certificate_number_len[2+1];
	char certificate_number[256+1];
	char area[6+1];
	char expand[6+1];
	char memo_len[3+1];
	char memo[256+1];
	char m_mac[16+1];

	memset(user_id,0,sizeof(user_id) );
	memset(user_name_len, 0,sizeof(user_name_len) );
	memset(user_name, 0,sizeof(user_name) );
	memset(cover, 0,sizeof(cover) );
	memset(tel_type_len, 0,sizeof(tel_type_len) );
	memset(tel_type, 0,sizeof(tel_type) );
	memset(tel_number_len, 0,sizeof(tel_number_len) );
	memset(tel_number, 0,sizeof(tel_number) );
	memset(address_len, 0,sizeof(address_len) );
	memset(address, 0,sizeof(address) );
	memset(tel_contact_len, 0,sizeof(tel_contact_len) );
	memset(tel_contact, 0,sizeof(tel_contact) );
	memset(certificate_type_len, 0,sizeof(certificate_type_len) );
	memset(certificate_type, 0,sizeof(certificate_type) );
	memset(certificate_number_len, 0,sizeof(certificate_number_len) );
	memset(certificate_number, 0,sizeof(certificate_number) );
	memset(area, 0,sizeof(area) );
	memset(expand, 0,sizeof(expand) );
	memset(memo_len, 0,sizeof(memo_len) );
	memset(memo, 0,sizeof(memo) );
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(user_id,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(user_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(user_name,m_databuff+s_len,atoi(user_name_len));s_len=s_len+atoi(user_name_len);
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(cover,m_databuff+s_len,4);s_len=s_len+4;
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(tel_type_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_type,m_databuff+s_len,atoi(tel_type_len));s_len=s_len+atoi(tel_type_len);
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(tel_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_number,m_databuff+s_len,atoi(tel_number_len));s_len=s_len+atoi(tel_number_len);
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(address_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(address,m_databuff+s_len,atoi(address_len));s_len=s_len+atoi(address_len);
	}
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(tel_contact_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_contact,m_databuff+s_len,atoi(tel_contact_len));s_len=s_len+atoi(tel_contact_len);
	}
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		memcpy(certificate_type_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(certificate_type,m_databuff+s_len,atoi(certificate_type_len));s_len=s_len+atoi(certificate_type_len);
	}
	if(memcmp(m_bmp+8,"1",1)==0)
	{
		memcpy(certificate_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(certificate_number,m_databuff+s_len,atoi(certificate_number_len));s_len=s_len+atoi(certificate_number_len);
	}
	if(memcmp(m_bmp+9,"1",1)==0)
	{
		memcpy(area,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+10,"1",1)==0)
	{
		memcpy(expand,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+11,"1",1)==0)
	{
		memcpy(memo_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(memo,m_databuff+s_len,atoi(memo_len));s_len=s_len+atoi(memo_len);
	}
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"user_id:%s\nuser_name_len:%s\nuser_name:%s\ncover:%s\ntel_type_len:%s\ntel_type:%s\ntel_number_len:%s\ntel_number:%s\naddress_len:%s\naddress:%s\ntel_contact_len:%s\ntel_contact:%s\ncertificate_type_len:%s\ncertificate_type:%s\ncertificate_number_len:%s\ncertificate_number:%s\narea:%s\nexpand:%s\nmemo_len:%s\nmemo:%s\nm_mac:%s\n", \
	user_id,user_name_len,user_name,cover,tel_type_len,tel_type,tel_number_len,tel_number,address_len,address,tel_contact_len,tel_contact,certificate_type_len,certificate_type,certificate_number_len,certificate_number,area,expand,memo_len,memo,m_mac);

	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}	*/
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>deleuserinfo(user_id))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"用户资料表删除记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<用户资料表删除> user_id:%s,user_name_len:%s,user_name:%s,cover:%s,tel_type_len:%s,tel_type:%s,tel_number_len:%s,tel_number:%s,address_len:%s,address:%s,tel_contact_len:%s,tel_contact:%s,certificate_type_len:%s,certificate_type:%s,certificate_number_len:%s,certificate_number:%s,area:%s,expand:%s,memo_len:%s,memo:%s,m_mac:%s", \
	user_id,user_name_len,user_name,cover,tel_type_len,tel_type,tel_number_len,tel_number,address_len,address,tel_contact_len,tel_contact,certificate_type_len,certificate_type,certificate_number_len,certificate_number,area,expand,memo_len,memo,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0021",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	psaminfofind_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );

	char psam_cdno_len[2+1];
	char psam_cdno[16+1];
	char term_event[6+1];//所属项目
	char term_join[6+1];//投入方
	char term_keep[6+1];//维护方
	char term_area[6+1];//所属区域
	char term_e1code[6+1];//FSK服务器
	char begin_page[6+1];
	
	char m_mac[16+1];

	memset(psam_cdno_len, 0,sizeof(psam_cdno_len) );
	memset(psam_cdno, 0,sizeof(psam_cdno) );
	memset(term_event, 0,sizeof(term_event) );
	memset(term_join, 0,sizeof(term_join) );
	memset(term_keep, 0,sizeof(term_keep) );
	memset(term_area, 0,sizeof(term_area) );
	memset(term_e1code, 0,sizeof(term_e1code) );
	memset(begin_page,0,sizeof(begin_page) );

	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	char whe[1024+1];
	char whe0[1024+1];
	char whe1[128+1];
	char whe2[128+1];
	char whe3[128+1];
	char whe4[128+1];
	char whe5[128+1];
	char whe6[128+1];
	char whe7[128+1];
	char whe8[128+1];
	memset(whe,0,sizeof(whe));
	memset(whe0,0,sizeof(whe0));
	memset(whe1,0,sizeof(whe1));
	memset(whe2,0,sizeof(whe2));
	memset(whe3,0,sizeof(whe3));
	memset(whe4,0,sizeof(whe4));
	memset(whe5,0,sizeof(whe5));
	memset(whe6,0,sizeof(whe6));
	memset(whe7,0,sizeof(whe7));
	memset(whe8,0,sizeof(whe8));
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(psam_cdno_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(psam_cdno,m_databuff+s_len,atoi(psam_cdno_len));s_len=s_len+atoi(psam_cdno_len);
		
		sprintf(whe1,"psam_cdno like '%s%s%s' and ","%",psam_cdno,"%");
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(term_event,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(whe2,"term_event like '%s%s%s' and ","%",term_event,"%");
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(term_join,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(whe3,"term_join like '%s%s%s' and ","%",term_join,"%");
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(term_keep,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(whe4,"term_keep like '%s%s%s' and ","%",term_keep,"%");
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(term_area,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(whe7,"term_area like '%s%s%s' and ","%",term_area,"%");
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(term_e1code,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(whe8,"term_e1code like '%s%s%s' and ","%",term_e1code,"%");
	}	
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(begin_page,m_databuff+s_len,6);s_len=s_len+6;
	}
	
	sprintf(whe0,"%s%s%s%s%s%s%s%s",whe1,whe2,whe3,whe4,whe5,whe6,whe7,whe8);
	if(strlen(whe0)!=0)	memcpy(whe,whe0,strlen(whe0)-5);
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"psam_cdno_len:%s\npsam_cdno:%s\nterm_event:%s\nterm_join:%s\nterm_keep:%s\nterm_area:%s\nterm_e1code:%s\nbegin_page:%s\nm_mac:%s\n", \
	psam_cdno_len,psam_cdno,term_event,term_join,term_keep,term_area,term_e1code,begin_page,m_mac);
	
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	char s_ret[2+1];
	char s_infobuff_len[3+1];
	char s_infobuff[2048+1];
	
	memset(s_ret,0,sizeof(s_ret));
	memset(s_infobuff,0,sizeof(s_infobuff));
	
	char over_bz[1+1];
	memset(over_bz,0,sizeof(over_bz));
	
	if(0>getpsaminfo(whe,s_infobuff,atoi(begin_page),atoi(begin_page)+7,over_bz,m_op_area,m_term_expand,m_term_join,m_term_keep,m_op_type))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"PSAM卡资料表获取记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"psam_cdno_len:%s,psam_cdno:%s,term_event:%s,term_join:%s,term_keep:%s,term_area:%s,begin_page:%s,m_mac:%s", \
	psam_cdno_len,psam_cdno,term_event,term_join,term_keep,term_area,begin_page,m_mac);
	//upmsglog(m_date,m_time,m_seq,m_msg,"00");
	
	if(memcmp(over_bz,"1",1)==0)	memcpy(m_status,"0",1);
	else	memcpy(m_status,"1",1);
	
	if(strlen(s_infobuff)>0)
		sprintf(s_infobuff_len,"%03d",strlen(s_infobuff)-1);
	else	sprintf(s_infobuff_len,"%03d",0);
	
	
	//组织返回报文
	struct  tm *tm;   time_t  t;	
	
	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	memcpy(m_info,"0031",4);
	
	memcpy(m_bmp_len,"02",2);
	memcpy(m_bmp,"11",2);
	
	
	s_len=0;
	s_len=s_len+4;
	
	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,2);s_len=s_len+2;
	
	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);
	
	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,s_infobuff_len,3);s_len=s_len+3;
	memcpy(srcBuf+s_len,s_infobuff,atoi(s_infobuff_len));s_len=s_len+atoi(s_infobuff_len);
	
	//VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-21 , m_mac))
	//GetPcMac(caMKey,caWKey,caData,iLen,m_mac);
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;
	
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);
	
	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	
	
	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	psaminfodetafind_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );

	char cdno_len[2+1];
	char cdno[16+1];
	char m_mac[16+1];

	memset(cdno_len, 0,sizeof(cdno_len) );
	memset(cdno, 0,sizeof(cdno) );
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(cdno_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(cdno,m_databuff+s_len,atoi(cdno_len));s_len=s_len+atoi(cdno_len);
	}
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"cdno:%s\n",cdno);
	
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	char s_ret[2+1];
	char psam_cdno_len[2+1];
	char psam_cdno[16+1];
	char term_type[6+1];
	char term_join[6+1];
	char term_keep[6+1];
	char menu_ver[6+1];
	char term_event[6+1];
	char term_area[6+1];
	char psam_statu[1+1];
	char term_e1code[6+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	memset(psam_cdno_len,0,sizeof(psam_cdno_len));
	memset(psam_cdno,0,sizeof(psam_cdno));
	memset(term_type,0,sizeof(term_type));
	memset(term_join,0,sizeof(term_join));
	memset(term_keep,0,sizeof(term_keep));
	memset(menu_ver,0,sizeof(menu_ver));
	memset(term_event,0,sizeof(term_event));
	memset(term_area,0,sizeof(term_area));
	memset(psam_statu,0,sizeof(psam_statu));
	memset(term_e1code,0,sizeof(term_e1code));
	
	
	if(getpsaminfodeta(cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,psam_statu,term_e1code)>0)
	{
		strcpy(psam_cdno,cdno);
		sprintf(psam_cdno_len,"%02d",strlen(psam_cdno));
			
		dcs_log(0,0,"psam_cdno:%s\nterm_type:%s\nterm_join:%s\nterm_keep:%s\nmenu_ver:%s\nterm_event:%s\nterm_area:%s\npsam_statu:%s\nterm_e1code:%s\n",psam_cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,psam_statu,term_e1code);
	}
	else
	{
		
		dcs_log(0,0,"getpsaminfodeta error !");

		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"PSAM卡资料表获取记录操作失败!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
		
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"psam_cdno:%s,term_type:%s,term_join:%s,term_keep:%s,menu_ver:%s,term_event:%s,term_area:%s,psam_statu:%s,term_e1code:%s",psam_cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,psam_statu,term_e1code);
	//upmsglog(m_date,m_time,m_seq,m_msg,"00");
	
	
	//组织返回报文
	struct  tm *tm;   time_t  t;	
	
	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	memcpy(m_info,"0031",4);
	
	memcpy(m_bmp_len,"10",2);
	memcpy(m_bmp,"1111111111",10);
	
	
	s_len=0;
	s_len=s_len+4;
	
	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,10);s_len=s_len+10;
	
	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);
	
	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	//memcpy(srcBuf+s_len,s_infobuff_len,3);s_len=s_len+3;
	//memcpy(srcBuf+s_len,s_infobuff,atoi(s_infobuff_len));s_len=s_len+atoi(s_infobuff_len);
	
	memcpy(srcBuf+s_len,psam_cdno_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,psam_cdno,atoi(psam_cdno_len));s_len=s_len+atoi(psam_cdno_len);
	
	memcpy(srcBuf+s_len,term_type,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,term_join,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,term_keep,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,menu_ver,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,term_event,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,term_area,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,psam_statu,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,term_e1code,6);s_len=s_len+6;

	//VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-21 , m_mac))
	//GetPcMac(caMKey,caWKey,caData,iLen,m_mac);
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;
	
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);
	
	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	
	
	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	psaminfoadd_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char psam_cdno_len[2+1];
	char psam_cdno[16+1];
	char term_type[6+1];
	char term_join[6+1];
	char term_keep[6+1];
	char menu_ver[6+1];
	char term_event[6+1];
	char term_area[6+1];
	char term_e1code[6+1];

	char m_mac[16+1];

	memset(psam_cdno_len, 0,sizeof(psam_cdno_len) );
	memset(psam_cdno, 0,sizeof(psam_cdno) );
	memset(term_type, 0,sizeof(term_type) );
	memset(term_join, 0,sizeof(term_join) );
	memset(term_keep, 0,sizeof(term_keep) );
	memset(menu_ver, 0,sizeof(menu_ver) );
	memset(term_event, 0,sizeof(term_event) );
	memset(term_area, 0,sizeof(term_area) );
	memset(term_e1code, 0,sizeof(term_e1code) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(psam_cdno_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(psam_cdno,m_databuff+s_len,atoi(psam_cdno_len));s_len=s_len+atoi(psam_cdno_len);
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(term_type,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(term_join,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(term_keep,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(menu_ver,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(term_event,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(term_area,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		memcpy(term_e1code,m_databuff+s_len,6);s_len=s_len+6;
	}

	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"psam_cdno_len:%s\npsam_cdno:%s\nterm_type:%s\nterm_join:%s\nterm_keep:%s\nmenu_ver:%s\nterm_event:%s\nterm_area:%s\nterm_e1code:%s\nm_mac:%s\n", \
	psam_cdno_len,psam_cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,term_e1code,m_mac);

	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	memset(errmsg,0,sizeof(errmsg));
	if(0>addpsaminfo(psam_cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,term_e1code,errmsg))
	{
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
		
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<PSAM卡资料表增加> psam_cdno_len:%s,psam_cdno:%s,term_type:%s,term_join:%s,term_keep:%s,menu_ver:%s,term_event:%s,term_area:%s,term_e1code:%s,m_mac:%s", \
	psam_cdno_len,psam_cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,term_e1code,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0031",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	psaminfomodi_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char psam_cdno_len[2+1];
	char psam_cdno[16+1];
	char term_type[6+1];
	char term_join[6+1];
	char term_keep[6+1];
	char menu_ver[6+1];
	char term_event[6+1];
	char term_area[6+1];
	char term_e1code[6+1];

	char m_mac[16+1];

	memset(psam_cdno_len, 0,sizeof(psam_cdno_len) );
	memset(psam_cdno, 0,sizeof(psam_cdno) );
	memset(term_type, 0,sizeof(term_type) );
	memset(term_join, 0,sizeof(term_join) );
	memset(term_keep, 0,sizeof(term_keep) );
	memset(menu_ver, 0,sizeof(menu_ver) );
	memset(term_event, 0,sizeof(term_event) );
	memset(term_area, 0,sizeof(term_area) );
	memset(term_e1code, 0,sizeof(term_e1code) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	
	char up_data[1024+1];
	char up_data0[1024+1];
	char up_data1[128+1];
	char up_data2[128+1];
	char up_data3[128+1];
	char up_data4[128+1];
	char up_data5[128+1];
	char up_data6[128+1];
	char up_data7[128+1];
	
	memset(up_data,0,sizeof(up_data));
	memset(up_data0,0,sizeof(up_data0));
	memset(up_data1,0,sizeof(up_data1));
	memset(up_data2,0,sizeof(up_data2));
	memset(up_data3,0,sizeof(up_data3));
	memset(up_data4,0,sizeof(up_data4));
	memset(up_data5,0,sizeof(up_data5));
	memset(up_data6,0,sizeof(up_data6));
	memset(up_data7,0,sizeof(up_data7));
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(psam_cdno_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(psam_cdno,m_databuff+s_len,atoi(psam_cdno_len));s_len=s_len+atoi(psam_cdno_len);
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(term_type,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data1,"term_type='%s',",term_type);
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(term_join,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data2,"term_join='%s',",term_join);
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(term_keep,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data3,"term_keep='%s',",term_keep);
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(menu_ver,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data4,"menu_ver='%s',",menu_ver);
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(term_event,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data5,"term_event='%s',",term_event);
	}
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(term_area,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data6,"term_area='%s',",term_area);
	}	
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		memcpy(term_e1code,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data7,"term_e1code='%s',",term_e1code);
	}	
	
	sprintf(up_data0,"%s%s%s%s%s%s%s",up_data1,up_data2,up_data3,up_data4,up_data5,up_data6,up_data7);
	if(strlen(up_data0)!=0)
	{
		memcpy(up_data,"set ",4);
		memcpy(up_data+4,up_data0,strlen(up_data0)-1);
	}
	
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"psam_cdno_len:%s\npsam_cdno:%s\nterm_type:%s\nterm_join:%s\nterm_keep:%s\nmenu_ver:%s\nterm_event:%s\nterm_area:%s\nterm_e1code\nm_mac:%s\n", \
	psam_cdno_len,psam_cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,term_e1code,m_mac);
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}	*/
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>modipsaminfo(psam_cdno,up_data))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"PSAM卡资料表修改记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		//sprintf(up_data4,"menu_ver='%s',",menu_ver);
		update_menu_ver_msg_term_info(psam_cdno,menu_ver);
		update_menu_ver_term_info(psam_cdno,menu_ver);
	}
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		//sprintf(up_data7,"term_e1code='%s',",term_e1code);
		update_term_e1code_msg_term_info(psam_cdno,term_e1code);
		update_term_e1code_term_info(psam_cdno,term_e1code);
	}		
		
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<PSAM卡资料表修改> psam_cdno_len:%s,psam_cdno:%s,term_type:%s,term_join:%s,term_keep:%s,menu_ver:%s,term_event:%s,term_area:%s,term_e1code:%s,m_mac:%s", \
	psam_cdno_len,psam_cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,term_e1code,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0031",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	psaminfodele_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char psam_cdno_len[2+1];
	char psam_cdno[16+1];
	char term_type[6+1];
	char term_join[6+1];
	char term_keep[6+1];
	char menu_ver[6+1];
	char term_event[6+1];
	char term_area[6+1];
	char term_e1code[6+1];

	char m_mac[16+1];

	memset(psam_cdno_len, 0,sizeof(psam_cdno_len) );
	memset(psam_cdno, 0,sizeof(psam_cdno) );
	memset(term_type, 0,sizeof(term_type) );
	memset(term_join, 0,sizeof(term_join) );
	memset(term_keep, 0,sizeof(term_keep) );
	memset(menu_ver, 0,sizeof(menu_ver) );
	memset(term_event, 0,sizeof(term_event) );
	memset(term_area, 0,sizeof(term_area) );
	memset(term_e1code, 0,sizeof(term_e1code) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(psam_cdno_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(psam_cdno,m_databuff+s_len,atoi(psam_cdno_len));s_len=s_len+atoi(psam_cdno_len);
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(term_type,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(term_join,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(term_keep,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(menu_ver,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(term_event,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(term_area,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		memcpy(term_e1code,m_databuff+s_len,6);s_len=s_len+6;
	}

	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"psam_cdno_len:%s\npsam_cdno:%s\nterm_type:%s\nterm_join:%s\nterm_keep:%s\nmenu_ver:%s\nterm_event:%s\nterm_area:%s\nterm_e1code:%s\nm_mac:%s\n", \
	psam_cdno_len,psam_cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,term_e1code,m_mac);

	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}	*/
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>delepsaminfo(psam_cdno))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"PSAM卡资料表删除记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
		
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<PSAM卡资料表删除> psam_cdno_len:%s,psam_cdno:%s,term_type:%s,term_join:%s,term_keep:%s,menu_ver:%s,term_event:%s,term_area:%s,term_e1code:%s,m_mac:%s", \
	psam_cdno_len,psam_cdno,term_type,term_join,term_keep,menu_ver,term_event,term_area,term_e1code,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0031",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	terminfofind_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char psam_cdno_len[2+1];
	char psam_cdno[16+1];
	char tel_number_len[2+1];
	char tel_number[256+1];
	char op_statu[4+1];
	char term_statu[2+1];
	char area_id[6+1];
	char begin_page[6+1];
	
	char m_mac[16+1];
	
	memset(psam_cdno_len, 0,sizeof(psam_cdno_len) );
	memset(psam_cdno, 0,sizeof(psam_cdno) );
	memset(tel_number_len, 0,sizeof(tel_number_len) );
	memset(tel_number, 0,sizeof(tel_number) );
	memset(op_statu, 0,sizeof(op_statu) );
	memset(term_statu, 0,sizeof(term_statu) );
	memset(area_id, 0,sizeof(area_id) );
	memset(begin_page,0,sizeof(begin_page) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	char whe[1024+1];
	char whe0[1024+1];
	char whe1[128+1];
	char whe2[128+1];
	char whe3[128+1];
	char whe4[128+1];
	char whe5[128+1];
	memset(whe,0,sizeof(whe));
	memset(whe0,0,sizeof(whe0));
	memset(whe1,0,sizeof(whe1));
	memset(whe2,0,sizeof(whe2));
	memset(whe3,0,sizeof(whe3));
	memset(whe4,0,sizeof(whe4));
	memset(whe5,0,sizeof(whe5));
	
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(psam_cdno_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(psam_cdno,m_databuff+s_len,atoi(psam_cdno_len));s_len=s_len+atoi(psam_cdno_len);
		
		sprintf(whe1,"psam_cdno like '%s%s%s' and ","%",psam_cdno,"%");
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(tel_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_number,m_databuff+s_len,atoi(tel_number_len));s_len=s_len+atoi(tel_number_len);
		
		sprintf(whe2,"tel_number like '%s%s%s' and ","%",tel_number,"%");
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(op_statu,m_databuff+s_len,4);s_len=s_len+4;
		sprintf(whe3,"op_statu like '%s%s%s' and ","%",op_statu,"%");
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(term_statu,m_databuff+s_len,2);s_len=s_len+2;
		sprintf(whe4,"term_statu like '%s%s%s' and ","%",term_statu,"%");
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(area_id,m_databuff+s_len,6);s_len=s_len+6;
		sprintf(whe5,"area_id like '%s%s%s' and ","%",area_id,"%");
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(begin_page,m_databuff+s_len,6);s_len=s_len+6;
	}

	sprintf(whe0,"%s%s%s%s%s",whe1,whe2,whe3,whe4,whe5);
	if(strlen(whe0)!=0)	memcpy(whe,whe0,strlen(whe0)-5);

	
	memcpy(m_mac,m_databuff+s_len,16);
			
	
			
	dcs_log(0,0,"psam_cdno_len:%s\npsam_cdno:%s\ntel_number_len:%s\ntel_number:%s\nop_statu:%s\nterm_statu:%s\narea_id:%s\nbegin_page:%s\nm_mac:%s",psam_cdno_len,psam_cdno,tel_number_len,tel_number,op_statu,term_statu,area_id,begin_page,m_mac);
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	char s_ret[2+1];
	char s_infobuff_len[3+1];
	char s_infobuff[2048+1];
	
	memset(s_ret,0,sizeof(s_ret));
	memset(s_infobuff,0,sizeof(s_infobuff));
	
	char over_bz[1+1];
	memset(over_bz,0,sizeof(over_bz));
	
	if(0>getterminfo(whe,s_infobuff,atoi(begin_page),atoi(begin_page)+7,over_bz,m_op_area,m_term_expand,m_term_join,m_term_keep,m_op_type))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"终端资料表获取记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"psam_cdno_len:%s,psam_cdno:%s,tel_number_len:%s,tel_number:%s,op_statu:%s,term_statu:%s,area_id:%s,begin_page:%s,m_mac:%s",psam_cdno_len,psam_cdno,tel_number_len,tel_number,op_statu,term_statu,area_id,begin_page,m_mac);
	//upmsglog(m_date,m_time,m_seq,m_msg,"00");
	
	if(memcmp(over_bz,"1",1)==0)	memcpy(m_status,"0",1);
	else	memcpy(m_status,"1",1);
	
	if(strlen(s_infobuff)>0)
		sprintf(s_infobuff_len,"%03d",strlen(s_infobuff)-1);
	else	sprintf(s_infobuff_len,"%03d",0);
	
	
	//组织返回报文
	struct  tm *tm;   time_t  t;	
	
	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	memcpy(m_info,"0041",4);
	
	memcpy(m_bmp_len,"02",2);
	memcpy(m_bmp,"11",2);
	
	
	s_len=0;
	s_len=s_len+4;
	
	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,2);s_len=s_len+2;
	
	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);
	
	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,s_infobuff_len,3);s_len=s_len+3;
	memcpy(srcBuf+s_len,s_infobuff,atoi(s_infobuff_len));s_len=s_len+atoi(s_infobuff_len);
	
	//VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-21 , m_mac))
	//GetPcMac(caMKey,caWKey,caData,iLen,m_mac);
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;
	
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);
	
	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	
	
	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	terminfoadd_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char psam_cdno_len[2+1];
	char psam_cdno[16+1];
	char tel_number_len[2+1];
	char tel_number[256+1];
	char term_statu[2+1];
	char extra_len[2+1];
	char extra[256+1];

	char m_mac[16+1];

	memset(psam_cdno_len, 0,sizeof(psam_cdno_len) );
	memset(psam_cdno, 0,sizeof(psam_cdno) );
	memset(tel_number_len, 0,sizeof(tel_number_len) );
	memset(tel_number, 0,sizeof(tel_number) );
	memset(term_statu, 0,sizeof(term_statu) );
	memset(extra_len, 0,sizeof(extra_len) );
	memset(extra, 0,sizeof(extra) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(psam_cdno_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(psam_cdno,m_databuff+s_len,atoi(psam_cdno_len));s_len=s_len+atoi(psam_cdno_len);
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(tel_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_number,m_databuff+s_len,atoi(tel_number_len));s_len=s_len+atoi(tel_number_len);
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(term_statu,m_databuff+s_len,2);s_len=s_len+2;
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(extra_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(extra,m_databuff+s_len,atoi(extra_len));s_len=s_len+atoi(extra_len);
	}
	


	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"psam_cdno_len:%s\npsam_cdno:%s\ntel_number_len:%s\ntel_number:%s\nterm_statu:%s\nextra_len:%s\nextra:%s\nm_mac:%s\n", \
	psam_cdno_len,psam_cdno,tel_number_len,tel_number,term_statu,extra_len,extra,m_mac);

	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>=getpsamstatu(psam_cdno))
	{
  	dcs_log(0,0,"psam_cdno:[%s],psam卡号不能用!",psam_cdno);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"PSAM卡资料表获取PSAM卡状态操作失败!");
  	ErrResponseForPC(srcBuf,"10",iConnSock,iLen,errmsg,strlen(errmsg));
  	return;
  }
	
	memset(errmsg,0,sizeof(errmsg));
	if(0>addmsgterminfo(psam_cdno,tel_number,term_statu,errmsg))
	{
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
		
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<终端资料表增加> psam_cdno_len:%s,psam_cdno:%s,tel_number_len:%s,tel_number:%s,term_statu:%s,extra_len:%s,extra:%s,m_mac:%s", \
	psam_cdno_len,psam_cdno,tel_number_len,tel_number,term_statu,extra_len,extra,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0041",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	terminfoverify_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char psam_cdno_len[2+1];
	char psam_cdno[16+1];
	char tel_number_len[2+1];
	char tel_number[256+1];
	char term_statu[2+1];
	char extra_len[2+1];
	char extra[256+1];

	char m_mac[16+1];
	
	char t_bz[1+1];
	char t_data[64+1];
	
	memset(psam_cdno_len, 0,sizeof(psam_cdno_len) );
	memset(psam_cdno, 0,sizeof(psam_cdno) );
	memset(tel_number_len, 0,sizeof(tel_number_len) );
	memset(tel_number, 0,sizeof(tel_number) );
	memset(term_statu, 0,sizeof(term_statu) );
	memset(extra_len, 0,sizeof(extra_len) );
	memset(extra, 0,sizeof(extra) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(psam_cdno_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(psam_cdno,m_databuff+s_len,atoi(psam_cdno_len));s_len=s_len+atoi(psam_cdno_len);
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(tel_number_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(tel_number,m_databuff+s_len,atoi(tel_number_len));s_len=s_len+atoi(tel_number_len);
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(term_statu,m_databuff+s_len,2);s_len=s_len+2;
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(extra_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(extra,m_databuff+s_len,atoi(extra_len));s_len=s_len+atoi(extra_len);
	}

	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"psam_cdno_len:%s\npsam_cdno:%s\ntel_number_len:%s\ntel_number:%s\nterm_statu:%s\nextra_len:%s\nextra:%s\nm_mac:%s\n", \
	psam_cdno_len,psam_cdno,tel_number_len,tel_number,term_statu,extra_len,extra,m_mac);

	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	
	if ( ( memcmp( m_handle ,"0008",4) ==0 ) && ( (memcmp( term_statu ,"02",2) ==0)||(memcmp( term_statu ,"03",2) ==0) ) )//绑定审核
	{
		dcs_log(0,0,"操作员拥有[绑定审核权限]");
	}
	else if ( ( memcmp( m_handle ,"0009",4) ==0 ) && ( memcmp( term_statu ,"01",2) ==0 ) )//停机申请
	{
		dcs_log(0,0,"操作员拥有[停机申请权限]");
	}
	else if ( ( memcmp( m_handle ,"0009",4) ==0 ) && ( (memcmp( term_statu ,"02",2) ==0)||(memcmp( term_statu ,"03",2) ==0) ) )//停机审核
	{
		dcs_log(0,0,"操作员拥有[停机审核权限]");
	}
	else if ( ( memcmp( m_handle ,"0010",4) ==0 ) && ( memcmp( term_statu ,"01",2) ==0 ) )//复机申请
	{
		dcs_log(0,0,"操作员拥有[复机申请权限]");
	}
	else if ( ( memcmp( m_handle ,"0010",4) ==0 ) && ( (memcmp( term_statu ,"02",2) ==0)||(memcmp( term_statu ,"03",2) ==0) ) )//复机审核
	{
		dcs_log(0,0,"操作员拥有[复机审核权限]");
	}
	else if ( ( memcmp( m_handle ,"0011",4) ==0 ) && ( memcmp( term_statu ,"01",2) ==0 ) )//变更申请
	{
		dcs_log(0,0,"操作员拥有[变更申请权限]");
	}
	else if ( ( memcmp( m_handle ,"0011",4) ==0 ) && ( (memcmp( term_statu ,"02",2) ==0)||(memcmp( term_statu ,"03",2) ==0) ) )//变更审核
	{
		dcs_log(0,0,"操作员拥有[变更审核权限]");
	}
	else if ( ( memcmp( m_handle ,"0012",4) ==0 ) && ( memcmp( term_statu ,"01",2) ==0 ) )//撤机申请
	{
		dcs_log(0,0,"操作员拥有[撤机申请权限]");
	}
	else if ( ( memcmp( m_handle ,"0012",4) ==0 ) && ( (memcmp( term_statu ,"02",2) ==0)||(memcmp( term_statu ,"03",2) ==0) ) )//撤机审核
	{
		dcs_log(0,0,"操作员拥有[撤机审核权限]");
	}
	else
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员无此操作权限!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if ( ( memcmp( m_handle ,"0008",4) ==0 ) && ( memcmp( term_statu ,"02",2) ==0 ) )//绑定审核通过报文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,绑定审核通过!",m_handle,term_statu);
		
		if(0>addterminfo(psam_cdno,tel_number))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表增加记录操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}	
		
		if(0>changemsgterminfo(psam_cdno,tel_number,"0008","02",extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}
	
		if(0>settermstatu(psam_cdno,tel_number,"1"))//终端可用
			dcs_log(0,0,"term_simno:[%s],term_code:[%s],term_statu:[%s],term_info状态修改失败!",psam_cdno,tel_number,"1");
		if(0>setpsamstatu(psam_cdno,"0"))//psam卡占用
			dcs_log(0,0,"psam_cdno:[%s],psam_statu:[%s],msg_psam_info状态修改失败!",psam_cdno,"0");	
		if(0>setuserstatu(tel_number,"0"))//用户占用
			dcs_log(0,0,"tel_number:[%s],statu:[%s],msg_user_info状态修改失败!",tel_number,"0");	
	}
	else	if ( ( memcmp( m_handle ,"0008",4) ==0 ) && ( memcmp( term_statu ,"03",2) ==0 ) )//绑定审核驳回报文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,绑定审核驳回!",m_handle,term_statu);
		
		/*if(0>delemsgterminfo(psam_cdno,tel_number))
			dcs_log(0,0,"psam_cdno:[%s],tel_number:[%s]删除失败!",psam_cdno,tel_number);*/
		if(0>changemsgterminfo(psam_cdno,tel_number,"0008","03",extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}
	}
	else	if ( ( memcmp( m_handle ,"0009",4) ==0 ) && ( memcmp( term_statu ,"02",2) ==0 ) )//停机审核通过报文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,停机审核通过!",m_handle,term_statu);
		
		if(0>changemsgterminfo(psam_cdno,tel_number,"0009","02",extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}		
		
		if(0>settermstatu(psam_cdno,tel_number,"0"))//终端不可用
			dcs_log(0,0,"term_simno:[%s],term_code:[%s],term_statu:[%s],term_info状态修改失败!",psam_cdno,tel_number,"0");
		if(0>setpsamstatu(psam_cdno,"0"))//psam卡占用
			dcs_log(0,0,"psam_cdno:[%s],psam_statu:[%s],msg_psam_info状态修改失败!",psam_cdno,"1");	
		if(0>setuserstatu(tel_number,"0"))//用户占用
			dcs_log(0,0,"tel_number:[%s],statu:[%s],msg_user_info状态修改失败!",tel_number,"1");	
	}
	else	if ( ( memcmp( m_handle ,"0009",4) ==0 ) && ( memcmp( term_statu ,"03",2) ==0 ) )//停机审核驳回报文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,停机审核驳回!",m_handle,term_statu);
		
		if(0>changemsgterminfo(psam_cdno,tel_number,"0008","02",extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}
	}
	else	if ( ( memcmp( m_handle ,"0010",4) ==0 ) && ( memcmp( term_statu ,"02",2) ==0 ) )//复机审核通过报文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,复机审核通过!",m_handle,term_statu);
		
		if(0>changemsgterminfo(psam_cdno,tel_number,"0008","02",extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}
				
		if(0>settermstatu(psam_cdno,tel_number,"1"))//终端可用
			dcs_log(0,0,"term_simno:[%s],term_code:[%s],term_statu:[%s],term_info状态修改失败!",psam_cdno,tel_number,"0");
		if(0>setpsamstatu(psam_cdno,"0"))//psam卡占用
			dcs_log(0,0,"psam_cdno:[%s],psam_statu:[%s],msg_psam_info状态修改失败!",psam_cdno,"1");	
		if(0>setuserstatu(tel_number,"0"))//用户占用
			dcs_log(0,0,"tel_number:[%s],statu:[%s],msg_user_info状态修改失败!",tel_number,"1");	
	}
	else	if ( ( memcmp( m_handle ,"0010",4) ==0 ) && ( memcmp( term_statu ,"03",2) ==0 ) )//复机审核驳回报文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,复机审核驳回!",m_handle,term_statu);
		
		if(0>changemsgterminfo(psam_cdno,tel_number,"0009","02",extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}
	}
	else	if ( ( memcmp( m_handle ,"0011",4) ==0 ) && ( memcmp( term_statu ,"02",2) ==0 ) )//变更审核通过报文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,变更审核通过!",m_handle,term_statu);
		
		memset(errmsg,0,sizeof(errmsg));
		if(0>moditerminfoandmsgterminfo(psam_cdno,tel_number,extra,errmsg))
		{
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}	
		
		memset(t_bz,0,sizeof(t_bz));
		memset(t_data,0,sizeof(t_data));
	
		memcpy(t_bz,extra,1);
		memcpy(t_data,extra+1,strlen(extra)-1);
		
		if(memcmp(t_bz,"1",1)==0)
		{
			if(0>setuserstatu(tel_number,"1"))//用户未占用
				dcs_log(0,0,"tel_number:[%s],statu:[%s],msg_user_info状态修改失败!",tel_number,"1");	
			if(0>setuserstatu(t_data,"0"))//用户占用
				dcs_log(0,0,"tel_number:[%s],statu:[%s],msg_user_info状态修改失败!",t_data,"0");	
				
			if(0>changemsgterminfo(psam_cdno,t_data,"0008","02",extra))
			{
				memset(errmsg,0,sizeof(errmsg));
				sprintf(errmsg,"终端资料表记录状态变更操作失败!");
				ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
				return;
			}
		}
		else	if(memcmp(t_bz,"2",1)==0)
		{
			if(0>setpsamstatu(psam_cdno,"1"))//psam卡未占用
				dcs_log(0,0,"psam_cdno:[%s],psam_statu:[%s],msg_psam_info状态修改失败!",psam_cdno,"1");	
			if(0>setpsamstatu(t_data,"0"))//psam卡占用
				dcs_log(0,0,"psam_cdno:[%s],psam_statu:[%s],msg_psam_info状态修改失败!",psam_cdno,"0");	
				
			if(0>changemsgterminfo(t_data,tel_number,"0008","02",extra))
			{
				memset(errmsg,0,sizeof(errmsg));
				sprintf(errmsg,"终端资料表记录状态变更操作失败!");
				ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
				return;
			}
		}
	}
	else	if ( ( memcmp( m_handle ,"0011",4) ==0 ) && ( memcmp( term_statu ,"03",2) ==0 ) )//变更审核驳回报文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,变更审核驳回!",m_handle,term_statu);
		
		if(0>changemsgterminfo(psam_cdno,tel_number,"0008","02",extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}
	}
	else	if ( ( memcmp( m_handle ,"0012",4) ==0 ) && ( memcmp( term_statu ,"02",2) ==0 ) )//撤机审核通过è文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,撤机审核通过!",m_handle,term_statu);
		
		/*if(0>settermstatu(psam_cdno,tel_number,"0"))//终端不可用
			dcs_log(0,0,"term_simno:[%s],term_code:[%s],term_statu:[%s],term_info状态修改失败!",psam_cdno,tel_number,"0");
		if(0>changemsgterminfo(psam_cdno,tel_number,"0012","02",extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}*/
		if(0>deleterminfo(psam_cdno,tel_number))
			dcs_log(0,0,"<term_info> psam_cdno:[%s],tel_number:[%s]删除失败!",psam_cdno,tel_number);
		if(0>delemsgterminfo(psam_cdno,tel_number))
			dcs_log(0,0,"<msg_term_info> psam_cdno:[%s],tel_number:[%s]删除失败!",psam_cdno,tel_number);
			
		if(0>setpsamstatu(psam_cdno,"1"))//psam卡未占用
			dcs_log(0,0,"psam_cdno:[%s],psam_statu:[%s],msg_psam_info状态修改失败!",psam_cdno,"1");	
		if(0>setuserstatu(tel_number,"1"))//用户未占用
			dcs_log(0,0,"tel_number:[%s],statu:[%s],msg_user_info状态修改失败!",tel_number,"1");	
	}
	else	if ( ( memcmp( m_handle ,"0012",4) ==0 ) && ( memcmp( term_statu ,"03",2) ==0 ) )//撤机审核驳回è文
	{
		dcs_debug(0,0,"<terminfoverify_proc> op_statu=%s,term_statu=%s,撤机审核驳回!",m_handle,term_statu);
		
		if(0>changemsgterminfo(psam_cdno,tel_number,"0009","02",extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}
	}
	else
	{
		if(0>changemsgterminfo(psam_cdno,tel_number,m_handle,term_statu,extra))
		{
			memset(errmsg,0,sizeof(errmsg));
			sprintf(errmsg,"终端资料表记录状态变更操作失败!");
			ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
			return;
		}
	}
	
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<终端资料表或申请或审核或驳回><%s><%s> psam_cdno_len:%s,psam_cdno:%s,tel_number_len:%s,tel_number:%s,term_statu:%s,extra_len:%s,extra:%s,m_mac:%s", \
	m_handle,term_statu,psam_cdno_len,psam_cdno,tel_number_len,tel_number,term_statu,extra_len,extra,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
	
	/*
	if(0>=getpsamstatu(psam_cdno))
	{
  	dcs_log(0,0,"psam_cdno:[%s],psam卡号不能用!",psam_cdno);
  	ErrResponseForPC(srcBuf,"10",iConnSock,iLen);
  	return;
  }
	if(0>setpsamstatu(psam_cdno,"0"))
		dcs_log(0,0,"psam_cdno:[%s],psam_statu:[%s],psam卡号状态修改失败!",psam_cdno,"0");	
	*/

	
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0041",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	opinfofind_proc( char * srcBuf, int  iConnSock, int iLen )
{
	
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据
	char op_area[6+1];
	char user_name_len[2+1];
	char user_name[256+1];
	char begin_page[6+1];
	
	char m_mac[16+1];
	int s_len=0;
	
	char whe[256+1];
	char whe0[256+1];
	char whe1[128+1];
	char whe2[128+1];

  dcs_debug(0,0,"opid test");

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
  dcs_debug(0,0,"getheadinfo succ");

	memset(op_area, 0,sizeof(op_area) );
	memset(user_name_len, 0,sizeof(user_name_len) );
	memset(user_name, 0,sizeof(user_name) );
	memset(begin_page,0,sizeof(begin_page) );

	memset(m_mac, 0,sizeof(m_mac) );
	
	
	memset(whe,0,sizeof(whe));
	memset(whe0,0,sizeof(whe0));
	memset(whe1,0,sizeof(whe1));
	memset(whe2,0,sizeof(whe2));
	

	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(op_area,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(whe1,"op_area like '%s%s%s' and ","%",op_area,"%");
	}
	
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(user_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(user_name,m_databuff+s_len,atoi(user_name_len));s_len=s_len+atoi(user_name_len);
		
		sprintf(whe2,"op_name='%s' and ",user_name);
	}
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(begin_page,m_databuff+s_len,6);s_len=s_len+6;
	}

	sprintf(whe0,"%s%s",whe1,whe2);
	if(strlen(whe0)!=0)	memcpy(whe,whe0,strlen(whe0)-5);
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"op_area:%s\nuser_name_len:%s\nuser_name:%s\nbegin_page:%s\nm_mac:%s\n", \
	op_area,user_name_len,user_name,begin_page,m_mac);
	
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	char s_ret[2+1];
	char s_infobuff_len[3+1];
	char s_infobuff[2048+1];
	
	memset(s_ret,0,sizeof(s_ret));
	memset(s_infobuff_len,0,sizeof(s_infobuff_len));
	memset(s_infobuff,0,sizeof(s_infobuff));
	
	char over_bz[1+1];
	memset(over_bz,0,sizeof(over_bz));
	
	
	
	int bgp,egp;
	bgp=atoi(begin_page);
	egp=atoi(begin_page)+7;
	
	dcs_log(0,0,"opinfofind_proc test0,%d,%d",bgp,egp);
	
	if(0>findopinfo(whe,s_infobuff,bgp,egp,over_bz))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"op_area:%s,user_name_len:%s,user_name:%s,begin_page:%s,m_mac:%s", \
	op_area,user_name_len,user_name,begin_page,m_mac);
	//upmsglog(m_date,m_time,m_seq,m_msg,"00");
	
	
	if(memcmp(over_bz,"1",1)==0)	memcpy(m_status,"0",1);
	else	memcpy(m_status,"1",1);
	
	if(strlen(s_infobuff)>0)
		sprintf(s_infobuff_len,"%03d",strlen(s_infobuff)-1);
	else	sprintf(s_infobuff_len,"%03d",0);
	
	
	//组织返回报文
	struct  tm *tm;   time_t  t;	
	
	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	memcpy(m_info,"0061",4);
	
	memcpy(m_bmp_len,"02",2);
	memcpy(m_bmp,"11",2);
	
	
	s_len=0;
	s_len=s_len+4;
	
	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,2);s_len=s_len+2;
	
	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);
	
	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,s_infobuff_len,3);s_len=s_len+3;
	memcpy(srcBuf+s_len,s_infobuff,atoi(s_infobuff_len));s_len=s_len+atoi(s_infobuff_len);
	
	//VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-21 , m_mac))
	//GetPcMac(caMKey,caWKey,caData,iLen,m_mac);
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;
	
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);
	
	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	
	
	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	opinfoadd_proc( char * srcBuf, int  iConnSock, int iLen )
{

	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char op_id[6+1];
	char op_type[2+1];
	char op_name_len[2+1];
	char op_name[256+1];
	char op_area_len[3+1];
	char op_area[256+1];
	char term_expand_len[3+1];
	char term_expand[256+1];
	char term_join_len[3+1];
	char term_join[256+1];
	char term_keep_len[3+1];
	char term_keep[256+1];
	char op_passwd[6+1];
	char op_limit_len[2+1];
	char op_limit[256+1];
	char op_memo_len[2+1];
	char op_memo[256+1];
	
	char m_mac[16+1];

	memset(op_id, 0,sizeof(op_id) );
	memset(op_type, 0,sizeof(op_type) );
	memset(op_name_len, 0,sizeof(op_name_len) );
	memset(op_name, 0,sizeof(op_name) );
	memset(op_area_len, 0,sizeof(op_area_len) );
	memset(op_area, 0,sizeof(op_area) );
	memset(term_expand_len, 0,sizeof(term_expand_len) );
	memset(term_expand, 0,sizeof(term_expand) );
	memset(term_join_len, 0,sizeof(term_join_len) );
	memset(term_join, 0,sizeof(term_join) );
	memset(term_keep_len, 0,sizeof(term_keep_len) );
	memset(term_keep, 0,sizeof(term_keep) );
	memset(op_passwd, 0,sizeof(op_passwd) );
	memset(op_limit_len, 0,sizeof(op_limit_len) );
	memset(op_limit, 0,sizeof(op_limit) );
	memset(op_memo_len, 0,sizeof(op_memo_len) );
	memset(op_memo, 0,sizeof(op_memo) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(op_id,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(op_type,m_databuff+s_len,2);s_len=s_len+2;
	}	
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(op_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(op_name,m_databuff+s_len,atoi(op_name_len));s_len=s_len+atoi(op_name_len);
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(op_area_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(op_area,m_databuff+s_len,atoi(op_area_len));s_len=s_len+atoi(op_area_len);
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(term_expand_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(term_expand,m_databuff+s_len,atoi(term_expand_len));s_len=s_len+atoi(term_expand_len);
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(term_join_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(term_join,m_databuff+s_len,atoi(term_join_len));s_len=s_len+atoi(term_join_len);
	}
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(term_keep_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(term_keep,m_databuff+s_len,atoi(term_keep_len));s_len=s_len+atoi(term_keep_len);
	}
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		memcpy(op_passwd,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+8,"1",1)==0)
	{
		memcpy(op_limit_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(op_limit,m_databuff+s_len,atoi(op_limit_len));s_len=s_len+atoi(op_limit_len);
	}
	if(memcmp(m_bmp+9,"1",1)==0)
	{
		memcpy(op_memo_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(op_memo,m_databuff+s_len,atoi(op_memo_len));s_len=s_len+atoi(op_memo_len);
	}

	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"op_id:%s\nop_type:%s\nop_name_len:%s\nop_name:%s\nop_area_len:%s\nop_area:%s\nterm_expand_len:%s\nterm_expand:%s\nterm_join_len:%s\nterm_join:%s\nterm_keep_len:%s\nterm_keep:%s\nop_passwd:%s\nop_limit_len:%s\nop_limit:%s\nop_memo_len:%s\nop_memo:%s\nm_mac:%s\n", \
	op_id,op_type,op_name_len,op_name,op_area_len,op_area,term_expand_len,term_expand,term_join_len,term_join,term_keep_len,term_keep,op_passwd,op_limit_len,op_limit,op_memo_len,op_memo,m_mac);

	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}*/
	
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>addopinfo(op_id,op_type,op_name,op_area,term_expand,term_join,term_keep,op_passwd,op_limit,op_memo))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表增加记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
		
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<操作员资料表增加> op_id:%s,op_type:%s,op_name_len:%s,op_name:%s,op_area_len:%s,op_area:%s,term_expand_len:%s,term_expand:%s,term_join_len:%s,term_join:%s,term_keep_len:%s,term_keep:%s,op_passwd:%s,op_limit_len:%s,op_limit:%s,op_memo_len:%s,op_memo:%s,m_mac:%s", \
	op_id,op_type,op_name_len,op_name,op_area_len,op_area,term_expand_len,term_expand,term_join_len,term_join,term_keep_len,term_keep,op_passwd,op_limit_len,op_limit,op_memo_len,op_memo,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0061",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
	
}

void	opinfomodi_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char op_id[6+1];
	char op_type[2+1];
	char op_name_len[2+1];
	char op_name[256+1];
	char op_area_len[3+1];
	char op_area[256+1];
	char term_expand_len[3+1];
	char term_expand[256+1];
	char term_join_len[3+1];
	char term_join[256+1];
	char term_keep_len[3+1];
	char term_keep[256+1];
	char op_passwd[6+1];
	char op_limit_len[2+1];
	char op_limit[256+1];
	char op_memo_len[2+1];
	char op_memo[256+1];
	
	char m_mac[16+1];

	memset(op_id, 0,sizeof(op_id) );
	memset(op_type, 0,sizeof(op_type) );
	memset(op_name_len, 0,sizeof(op_name_len) );
	memset(op_name, 0,sizeof(op_name) );
	memset(op_area_len, 0,sizeof(op_area_len) );
	memset(op_area, 0,sizeof(op_area) );
	memset(term_expand_len, 0,sizeof(term_expand_len) );
	memset(term_expand, 0,sizeof(term_expand) );
	memset(term_join_len, 0,sizeof(term_join_len) );
	memset(term_join, 0,sizeof(term_join) );
	memset(term_keep_len, 0,sizeof(term_keep_len) );
	memset(term_keep, 0,sizeof(term_keep) );
	memset(op_passwd, 0,sizeof(op_passwd) );
	memset(op_limit_len, 0,sizeof(op_limit_len) );
	memset(op_limit, 0,sizeof(op_limit) );
	memset(op_memo_len, 0,sizeof(op_memo_len) );
	memset(op_memo, 0,sizeof(op_memo) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	
	char up_data[1024+1];
	char up_data0[1024+1];
	char up_data1[128+1];
	char up_data2[128+1];
	char up_data3[128+1];
	char up_data4[128+1];
	char up_data5[128+1];
	char up_data6[128+1];
	char up_data7[128+1];
	char up_data8[128+1];
	char up_data9[128+1];
		
	memset(up_data,0,sizeof(up_data));
	memset(up_data0,0,sizeof(up_data0));
	memset(up_data1,0,sizeof(up_data1));
	memset(up_data2,0,sizeof(up_data2));
	memset(up_data3,0,sizeof(up_data3));
	memset(up_data4,0,sizeof(up_data4));
	memset(up_data5,0,sizeof(up_data5));
	memset(up_data6,0,sizeof(up_data6));
	memset(up_data7,0,sizeof(up_data7));
	memset(up_data8,0,sizeof(up_data8));
	memset(up_data9,0,sizeof(up_data9));
		
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(op_id,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(op_type,m_databuff+s_len,2);s_len=s_len+2;
		
		sprintf(up_data1,"op_type='%s',",op_type);
	}	
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(op_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(op_name,m_databuff+s_len,atoi(op_name_len));s_len=s_len+atoi(op_name_len);
		
		sprintf(up_data2,"op_name='%s',",op_name);
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(op_area_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(op_area,m_databuff+s_len,atoi(op_area_len));s_len=s_len+atoi(op_area_len);
		
		sprintf(up_data3,"op_area='%s',",op_area);
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(term_expand_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(term_expand,m_databuff+s_len,atoi(term_expand_len));s_len=s_len+atoi(term_expand_len);
		
		sprintf(up_data4,"term_expand='%s',",term_expand);
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(term_join_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(term_join,m_databuff+s_len,atoi(term_join_len));s_len=s_len+atoi(term_join_len);
		
		sprintf(up_data5,"term_join='%s',",term_join);
	}
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(term_keep_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(term_keep,m_databuff+s_len,atoi(term_keep_len));s_len=s_len+atoi(term_keep_len);
		
		sprintf(up_data6,"term_keep='%s',",term_keep);
	}
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		memcpy(op_passwd,m_databuff+s_len,6);s_len=s_len+6;
		
		sprintf(up_data7,"op_passwd='%s',",op_passwd);
	}
	if(memcmp(m_bmp+8,"1",1)==0)
	{
		memcpy(op_limit_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(op_limit,m_databuff+s_len,atoi(op_limit_len));s_len=s_len+atoi(op_limit_len);
		
		sprintf(up_data8,"op_limit='%s',",op_limit);
	}
	if(memcmp(m_bmp+9,"1",1)==0)
	{
		memcpy(op_memo_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(op_memo,m_databuff+s_len,atoi(op_memo_len));s_len=s_len+atoi(op_memo_len);
		
		sprintf(up_data9,"op_memo='%s',",op_memo);
	}	
	
	sprintf(up_data0,"%s%s%s%s%s%s%s%s%s",up_data1,up_data2,up_data3,up_data4,up_data5,up_data6,up_data7,up_data8,up_data9);
	if(strlen(up_data0)!=0)
	{
		memcpy(up_data,"set ",4);
		memcpy(up_data+4,up_data0,strlen(up_data0)-1);
	}
	
	
	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"op_id:%s\nop_type:%s\nop_name_len:%s\nop_name:%s\nop_area_len:%s\nop_area:%s\nterm_expand_len:%s\nterm_expand:%s\nterm_join_len:%s\nterm_join:%s\nterm_keep_len:%s\nterm_keep:%s\nop_passwd:%s\nop_limit_len:%s\nop_limit:%s\nop_memo_len:%s\nop_memo:%s\nm_mac:%s\n", \
	op_id,op_type,op_name_len,op_name,op_area_len,op_area,term_expand_len,term_expand,term_join_len,term_join,term_keep_len,term_keep,op_passwd,op_limit_len,op_limit,op_memo_len,op_memo,m_mac);
	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
//	if(0>VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac))
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}	*/
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>modiopinfo(op_id,up_data))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表修改记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
		
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<操作员资料表修改> op_id:%s,op_type:%s,op_name_len:%s,op_name:%s,op_area_len:%s,op_area:%s,term_expand_len:%s,term_expand:%s,term_join_len:%s,term_join:%s,term_keep_len:%s,term_keep:%s,op_passwd:%s,op_limit_len:%s,op_limit:%s,op_memo_len:%s,op_memo:%s,m_mac:%s", \
	op_id,op_type,op_name_len,op_name,op_area_len,op_area,term_expand_len,term_expand,term_join_len,term_join,term_keep_len,term_keep,op_passwd,op_limit_len,op_limit,op_memo_len,op_memo,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0061",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

void	opinfodele_proc( char * srcBuf, int  iConnSock, int iLen )
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_handle[4+1];			//操作类型
	char m_encrypt[2+1];		//加密方式
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_handle, 0, sizeof(m_handle) );
	memset(m_encrypt, 0, sizeof(m_encrypt) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	getheadinfo(srcBuf,m_len,m_info,m_handle,m_encrypt,m_date,m_time,m_seq,m_status,m_opid,m_bmp_len,m_bmp,m_databuff );
	
	char op_id[6+1];
	char op_type[2+1];
	char op_name_len[2+1];
	char op_name[256+1];
	char op_area_len[3+1];
	char op_area[256+1];
	char term_expand_len[3+1];
	char term_expand[256+1];
	char term_join_len[3+1];
	char term_join[256+1];
	char term_keep_len[3+1];
	char term_keep[256+1];
	char op_passwd[6+1];
	char op_limit_len[2+1];
	char op_limit[256+1];
	char op_memo_len[2+1];
	char op_memo[256+1];
	
	char m_mac[16+1];

	memset(op_id, 0,sizeof(op_id) );
	memset(op_type, 0,sizeof(op_type) );
	memset(op_name_len, 0,sizeof(op_name_len) );
	memset(op_name, 0,sizeof(op_name) );
	memset(op_area_len, 0,sizeof(op_area_len) );
	memset(op_area, 0,sizeof(op_area) );
	memset(term_expand_len, 0,sizeof(term_expand_len) );
	memset(term_expand, 0,sizeof(term_expand) );
	memset(term_join_len, 0,sizeof(term_join_len) );
	memset(term_join, 0,sizeof(term_join) );
	memset(term_keep_len, 0,sizeof(term_keep_len) );
	memset(term_keep, 0,sizeof(term_keep) );
	memset(op_passwd, 0,sizeof(op_passwd) );
	memset(op_limit_len, 0,sizeof(op_limit_len) );
	memset(op_limit, 0,sizeof(op_limit) );
	memset(op_memo_len, 0,sizeof(op_memo_len) );
	memset(op_memo, 0,sizeof(op_memo) );
	
	memset(m_mac, 0,sizeof(m_mac) );
	
	int s_len=0;
	
	if(memcmp(m_bmp,"1",1)==0)
	{
		memcpy(op_id,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+1,"1",1)==0)
	{
		memcpy(op_type,m_databuff+s_len,2);s_len=s_len+2;
	}	
	if(memcmp(m_bmp+2,"1",1)==0)
	{
		memcpy(op_name_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(op_name,m_databuff+s_len,atoi(op_name_len));s_len=s_len+atoi(op_name_len);
	}
	if(memcmp(m_bmp+3,"1",1)==0)
	{
		memcpy(op_area_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(op_area,m_databuff+s_len,atoi(op_area_len));s_len=s_len+atoi(op_area_len);
	}
	if(memcmp(m_bmp+4,"1",1)==0)
	{
		memcpy(term_expand_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(term_expand,m_databuff+s_len,atoi(term_expand_len));s_len=s_len+atoi(term_expand_len);
	}
	if(memcmp(m_bmp+5,"1",1)==0)
	{
		memcpy(term_join_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(term_join,m_databuff+s_len,atoi(term_join_len));s_len=s_len+atoi(term_join_len);
	}
	if(memcmp(m_bmp+6,"1",1)==0)
	{
		memcpy(term_keep_len,m_databuff+s_len,3);s_len=s_len+3;
		memcpy(term_keep,m_databuff+s_len,atoi(term_keep_len));s_len=s_len+atoi(term_keep_len);
	}
	if(memcmp(m_bmp+7,"1",1)==0)
	{
		memcpy(op_passwd,m_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(m_bmp+8,"1",1)==0)
	{
		memcpy(op_limit_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(op_limit,m_databuff+s_len,atoi(op_limit_len));s_len=s_len+atoi(op_limit_len);
	}
	if(memcmp(m_bmp+9,"1",1)==0)
	{
		memcpy(op_memo_len,m_databuff+s_len,2);s_len=s_len+2;
		memcpy(op_memo,m_databuff+s_len,atoi(op_memo_len));s_len=s_len+atoi(op_memo_len);
	}

	memcpy(m_mac,m_databuff+s_len,16);
			
	dcs_log(0,0,"op_id:%s\nop_type:%s\nop_name_len:%s\nop_name:%s\nop_area_len:%s\nop_area:%s\nterm_expand_len:%s\nterm_expand:%s\nterm_join_len:%s\nterm_join:%s\nterm_keep_len:%s\nterm_keep:%s\nop_passwd:%s\nop_limit_len:%s\nop_limit:%s\nop_memo_len:%s\nop_memo:%s\nm_mac:%s\n", \
	op_id,op_type,op_name_len,op_name,op_area_len,op_area,term_expand_len,term_expand,term_join_len,term_join,term_keep_len,term_keep,op_passwd,op_limit_len,op_limit,op_memo_len,op_memo,m_mac);

	
	char m_mkey[16+1];
	char m_wkey[16+1];
	memset(m_mkey,0,sizeof(m_mkey));
	memset(m_wkey,0,sizeof(m_wkey));
	if(0>getkey(m_opid,m_mkey,m_wkey))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表获取主密钥和工作密钥操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	/*if(0>VerifyPcMac(m_mkey , m_wkey , srcBuf+4 , iLen-20 , m_mac))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC校验错误!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}	*/
	
	char s_ret[2+1];
	
	memset(s_ret,0,sizeof(s_ret));
	
	if(0>deleopinfo(op_id))
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员资料表删除记录操作失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
		
	memset(m_msg,0,sizeof(m_msg));
	sprintf(m_msg,"<操作员资料表删除> op_id:%s,op_type:%s,op_name_len:%s,op_name:%s,op_area_len:%s,op_area:%s,term_expand_len:%s,term_expand:%s,term_join_len:%s,term_join:%s,term_keep_len:%s,term_keep:%s,op_passwd:%s,op_limit_len:%s,op_limit:%s,op_memo_len:%s,op_memo:%s,m_mac:%s", \
	op_id,op_type,op_name_len,op_name,op_area_len,op_area,term_expand_len,term_expand,term_join_len,term_join,term_keep_len,term_keep,op_passwd,op_limit_len,op_limit,op_memo_len,op_memo,m_mac);
	upmsglog(m_date,m_time,m_seq,m_msg,"00");
		
	//组织返回报文
	struct  tm *tm;   time_t  t;	

	time(&t);
  tm = localtime(&t);
	sprintf(m_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(m_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);

	memcpy(m_info,"0061",4);

	memcpy(m_bmp_len,"01",2);
	memcpy(m_bmp,"1",1);

	s_len=0;
	s_len=s_len+4;

	memcpy(srcBuf+s_len,m_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,m_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_status,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,m_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,m_bmp_len,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,m_bmp,1);s_len=s_len+1;

	//组织消息体
	memcpy(s_ret,"00",2);	
	//memcpy(m_mac,"0000000000000000",16);

	memcpy(srcBuf+s_len,s_ret,2);s_len=s_len+2;
	
	//GetPcMac(m_mkey,m_wkey,srcBuf+4,s_len-4,m_mac);
	
	memcpy(srcBuf+s_len,m_mac,16);s_len=s_len+16;

	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);

	/*memset(m_databuff,0,sizeof(m_databuff));
	sprintf(m_databuff,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	memcpy(m_databuff+8,srcBuf,s_len);

	dcs_debug(m_databuff,s_len+8,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len+8);	

	if( 0 > fold_write(iConnSock,-1,m_databuff,s_len+8) )
  {
		dcs_log(m_databuff,s_len+8,"fold_write() error: errno=%d ,%s\n", errno,ise_strerror(errno));
		return ;
  }	*/
  dcs_debug(srcBuf,s_len,"返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
  write_msg_to_AMP(iConnSock, srcBuf, s_len,60000);
}

static int sendtel()
{
	char m_len[4+1];				//报文长度
	char m_info[4+1];				//消息类型
	char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据

	char psam_cdno_len[2+1];
	char psam_cdno[16+1];							//PSAM卡号
	char tel_number_len[2+1];
	char tel_number[256+1];						//终端号码
	char user_name_len[2+1];
	char user_name[256+1];						//用户名称
	char address_len[3+1];
	char address[256+1];							//安装地址
	char certificate_type_len[2+1];
	char certificate_type[256+1];			//证件类型
	char certificate_number_len[2+1];
	char certificate_number[256+1];		//证件号码
	char tel_contact_len[2+1];
	char tel_contact[256+1];					//联系电话
	char memo_len[3+1];
	char memo[256+1];									//备注
	
	int m_sock,i;
	
	char SendBuf[2048];
	int SendLen;
	
	char server_ip[64];
	char server_port[64];

	memset(m_len, 0, sizeof(m_len) );
	memset(m_info, 0, sizeof(m_info) );
	memset(m_date, 0, sizeof(m_date) );
	memset(m_time, 0, sizeof(m_time) );
	memset(m_seq, 0, sizeof(m_seq) );
	memset(m_status, 0, sizeof(m_status) );
	memset(m_opid, 0, sizeof(m_opid) );
	memset(m_bmp_len, 0, sizeof(m_bmp_len) );
	memset(m_bmp, 0, sizeof(m_bmp) );
	memset(m_databuff, 0, sizeof(m_databuff) );
	
	memset(psam_cdno_len, 0, sizeof(psam_cdno_len) );
	memset(psam_cdno, 0, sizeof(psam_cdno) );
	memset(tel_number_len, 0, sizeof(tel_number_len) );
	memset(tel_number, 0, sizeof(tel_number) );
	memset(user_name_len, 0, sizeof(user_name_len) );
	memset(user_name, 0, sizeof(user_name) );
	memset(address_len, 0, sizeof(address_len) );
	memset(address, 0, sizeof(address) );
	memset(certificate_type_len, 0, sizeof(certificate_type_len) );
	memset(certificate_type, 0, sizeof(certificate_type) );
	memset(certificate_number_len, 0, sizeof(certificate_number_len) );
	memset(certificate_number, 0, sizeof(certificate_number) );
	memset(tel_contact_len, 0, sizeof(tel_contact_len) );
	memset(tel_contact, 0, sizeof(tel_contact) );
	memset(memo_len, 0, sizeof(memo_len) );
	memset(memo, 0, sizeof(memo) );
	
	memset(SendBuf,0,sizeof(SendBuf));
	
	SendLen=0;
	memcpy(m_info,"0100",4);
	/*char m_date[4+1];				//交易日期
	char m_time[6+1];				//交易时间
	char m_seq[6+1];				//交易流水
	char m_status[1+1];			//报文传输状态
	char m_opid[6+1];				//操作员ID
	char m_bmp_len[6+1];		//BITMAP图长度
	char m_bmp[32+1];				//BITMAP图
	char m_databuff[1024+1];//消息数据*/
	
	dcs_log( 0,0, "发送给电信的报文:[%s]",SendBuf);
	dcs_log(0,0,"建立连接><");
	m_sock=tcp_connet_server(server_ip,atoi(server_port),0);
	if(m_sock<0)
	{
		dcs_log(0,0,"连接失败:%s:%s",server_ip,server_port);
		//ErrResponse(iso,"91",iFid);
		return -1;
	}
	else
	{
		//dcs_log(0,0,"连接成功:%s:%s",server_ip,server_port);
		dcs_log(0,0,"发送数据>>>");
		
		if(write_msg_to_AMP(m_sock,SendBuf,SendLen,600)<=0)
		{
			dcs_log(0,0,"发送失败:%s:%s",server_ip,server_port);
			//ErrResponse(iso,"96",iFid);
			tcp_close_socket(m_sock);
			dcs_log(0,0,"关闭连接:%s:%s",server_ip,server_port);
			return -1;
		}
		/*else
		{
			//dcs_log(0,0,"发送成功:%s:%s",server_ip,server_port);
			dcs_log(0,0,"接收数据<<<");
			
			if((i=read_msg_from_AMP(m_sock,ReceiveBuf,ReceiveLen,30000))<=0)
			{
				dcs_log(0,0,"i=%d,errno=%d,%s",i,errno,ise_strerror(errno));
				dcs_log( 0,0, "电信返回的报文:[%s]",ReceiveBuf);
				dcs_log(0,0,"接收失败:%s:%s",server_ip,server_port);
				ErrResponse(iso,"68",iFid);
				tcp_close_socket(m_sock);
				dcs_log(0,0,"关闭连接:%s:%s",server_ip,server_port);
				return -1;	
			}
		}*/
	}	
	
	//dcs_log( 0,0, "电信返回的报文:[%s]",ReceiveBuf);	
	
	tcp_close_socket(m_sock);
	
	//dcs_log(0,0,"关闭连接:%s:%s",server_ip,server_port);	
	
	return i;
}


int zddown_proc( char * srcBuf,int  iConnSock, int iLen )
{
	int s_len;
	//char m_shopidlen[2+1];
	char m_shopid[20+1];
	//char m_termidlen[2+1];
	char m_termid[20+1];
	char m_posiplen[2+1];
	char m_posip[20+1];
	char m_posportlen[2+1];
	char m_posport[10+1];
	char m_mac[16+1];
	
	char SendBuf[2048];
	char ReceiveBuf[2048];
	int SendLen=0;
	int ReceiveLen=0;
	
	//memset(m_shopidlen,0,sizeof(m_shopidlen));
	memset(m_shopid,0,sizeof(m_shopid));
	//memset(m_termidlen,0,sizeof(m_termidlen));
	memset(m_termid,0,sizeof(m_termid));
	memset(m_posiplen,0,sizeof(m_posiplen));
	memset(m_posip,0,sizeof(m_posip));
	memset(m_posportlen,0,sizeof(m_posportlen));
	memset(m_posport,0,sizeof(m_posport));
	memset(m_mac,0,sizeof(m_mac));
	
	s_len=0;
	//memcpy(m_shopidlen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_shopid,msg_databuff+s_len,15);s_len=s_len+15;
	//memcpy(m_termidlen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_termid,msg_databuff+s_len,8);s_len=s_len+8;
	memcpy(m_posiplen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_posip,msg_databuff+s_len,atoi(m_posiplen));s_len=s_len+atoi(m_posiplen);
	memcpy(m_posportlen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_posport,msg_databuff+s_len,atoi(m_posportlen));s_len=s_len+atoi(m_posportlen);
	memcpy(m_mac,msg_databuff+s_len,16);s_len=s_len+16;
	
	dcs_debug(0,0,"%s",msg_databuff);
	dcs_debug(0,0,"%s,%s,%s,%s,%s,%s,%s",m_shopid,m_termid,m_posiplen,m_posip,m_posportlen,m_posport,m_mac);
	
	if(0>VerifyPcMac(x_workkey , srcBuf+4 , iLen-4-16 , m_mac))
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"02",m_shopid,m_termid,msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}
	
	memset(SendBuf,0,sizeof(SendBuf));
	memset(ReceiveBuf,0,sizeof(ReceiveBuf));
	
	SendLen=setsendbuf(m_shopid,m_termid,msg_organcode,SendBuf);
	if(SendLen<=0)
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"03",m_shopid,m_termid,msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"密钥下发失败!");
		ErrResponseForPC(srcBuf,"03",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}
	
	ReceiveLen=sendpos(m_posip,m_posport,SendBuf,SendLen,ReceiveBuf);
	if(ReceiveLen<=0)
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"01",m_shopid,m_termid,msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"密钥下发失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}
	
	if( 0>verifyreceivebuf(ReceiveBuf,ReceiveLen) )
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"01",m_shopid,m_termid,msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"密钥下发失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}

	//组织返回报文
	struct  tm *tm;   time_t  t;
	char msg_ret[2+1];
	char msg_mac[16+1];
	
	memset(msg_info,0,sizeof(msg_info));
	memset(msg_bmplen,0,sizeof(msg_bmplen));
	memset(msg_bmp,0,sizeof(msg_bmp));
	memset(msg_ret,0,sizeof(msg_ret));
	memset(msg_mac,0,sizeof(msg_mac));
	
	memcpy(msg_info,"0041",4);
	memcpy(msg_bmplen,"02",2);
	memcpy(msg_bmp,"11",2);
	memcpy(msg_ret,"00",2);

	time(&t);
  tm = localtime(&t);
	sprintf(msg_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(msg_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	s_len=0;
	s_len=s_len+4;
	
	//消息头
	memcpy(srcBuf+s_len,msg_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_statu,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,msg_organcodelen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_organcode,atoi(msg_organcodelen));s_len=s_len+atoi(msg_organcodelen);
	memcpy(srcBuf+s_len,msg_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_bmplen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_bmp,atoi(msg_bmplen));s_len=s_len+atoi(msg_bmplen);
	
	//消息体
	memcpy(srcBuf+s_len,msg_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_mac,16);s_len=s_len+16;
	
	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	GetPcMac(x_workkey , srcBuf+4 , s_len-4-16 , msg_mac);
	memcpy(srcBuf+s_len-16,msg_mac,16);
	
  dcs_debug(srcBuf,s_len,"<zddown_proc> 返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
	write_msg_to_AMP(iConnSock, srcBuf, s_len,60000); 
	
	insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"00",m_shopid,m_termid,msg_organcode);
	
	return 1;	
}

int sendpos(char * pos_ip,char * pos_port,char * SendBuf,int SendLen,char * ReceiveBuf)
{
	int m_sock,i;
	
	dcs_log( SendBuf,SendLen, "<sendpos> 发送给POS的报文:[%d]",SendLen);
	dcs_log(0,0,"<sendpos> 建立连接><");
	
	m_sock=tcp_connet_server(pos_ip,atoi(pos_port),0);
	if(m_sock<0)
	{
		dcs_log(0,0,"<sendpos> 连接失败:%s:%s",pos_ip,pos_port);
		return -1;
	}
	else
	{
		dcs_log(0,0,"<sendpos> 连接成功:%s:%s",pos_ip,pos_port);
		dcs_log(0,0,"<sendpos> 发送数据>>>");
		
		if(write_msg_to_AMP(m_sock,SendBuf,SendLen,600)<=0)
		{
			dcs_log(0,0,"<sendpos> 发送失败:%s:%s",pos_ip,pos_port);
			tcp_close_socket(m_sock);
			dcs_log(0,0,"<sendpos> 关闭连接:%s:%s",pos_ip,pos_port);
			return -1;
		}
		
		dcs_log(0,0,"<sendpos> 发送成功:%s:%s",pos_ip,pos_port);
		dcs_log(0,0,"<sendpos> 接收数据<<<");
			
		if((i=read_msg_from_AMP(m_sock,ReceiveBuf,2048,30000))<=0)
		{
			dcs_log(0,0,"<sendpos> i=%d,errno=%d,%s",i,errno,ise_strerror(errno));
			dcs_log(0,0,"<sendpos> 接收失败:%s:%s",pos_ip,pos_port);
			tcp_close_socket(m_sock);
			dcs_log(0,0,"<sendpos> 关闭连接:%s:%s",pos_ip,pos_port);
			return -1;	
		}
	}	
	
	tcp_close_socket(m_sock);
	
	dcs_log(0,0,"<sendpos> 关闭连接:%s:%s",pos_ip,pos_port);
	
	dcs_log( ReceiveBuf,i, "POS返回的报文:[%d]",i);	
	
	return i;
}

int setsendbuf(char * shopid,char * termid,char * organcode,char * SendBuf)
{
	int s_len;
	char m_mkey[16+1],m_mkey8[8+1];//传输主密钥密文ASC码和BCD码
	char x_mkey[16+1],x_mkey8[8+1];//传输主密钥明文ASC码和BCD码
	char m_factor[16+1],m_factor8[8+1];//分散因子ASC码和BCD码
	char m_random[16+1],m_random8[8+1];//用于产生工作密钥随机数ASC码和BCD码
	
	memset(m_mkey,0,sizeof(m_mkey));memset(m_mkey8,0,sizeof(m_mkey8));
	memset(x_mkey,0,sizeof(x_mkey));memset(x_mkey8,0,sizeof(x_mkey8));
	memset(m_factor,0,sizeof(m_factor));memset(m_factor8,0,sizeof(m_factor8));
	memset(m_random,0,sizeof(m_random));memset(m_random8,0,sizeof(m_random8));
	memset(mm_workkey,0,sizeof(mm_workkey));memset(mm_workkey8,0,sizeof(mm_workkey8));
	memset(xx_workkey,0,sizeof(xx_workkey));memset(xx_workkey8,0,sizeof(xx_workkey8));
	
	if(0>getorgan(organcode,m_mkey,m_factor))
		return -1;
	
	asc_to_bcd((unsigned char *)m_mkey8,(unsigned char *)m_mkey,16,0);
	asc_to_bcd((unsigned char *)m_factor8,(unsigned char *)m_factor,16,0);
	_DES(m_factor8,m_mkey8,x_mkey8);//解密得到传输主密钥明文BCD码
	bcd_to_asc((unsigned char *)x_mkey,(unsigned char *)x_mkey8,16,0);	

	dcs_debug(0,0,"<poslogin_proc>\nDATA=传输主密钥密文:[%s]\nKEY=分散因子:[%s]\n->DES解密\nDATA=传输主密钥明文:[%s]",m_mkey,m_factor,x_mkey);
	
	GetRandomData(m_random);
	asc_to_bcd((unsigned char *)m_random8,(unsigned char *)m_random,16,0);
	DES(x_mkey8,m_random8,xx_workkey8);//加密得到传输工作密钥明文BCD码
	bcd_to_asc((unsigned char *)xx_workkey,(unsigned char *)xx_workkey8,16,0);
	
	dcs_debug(0,0,"<poslogin_proc>\nDATA=随机数:[%s]\nKEY=传输主密钥明文:[%s]\n->DES加密\nDATA=传输工作密钥明文:[%s]",m_random,x_mkey,xx_workkey);
	
	DES(x_mkey8,xx_workkey8,mm_workkey8);//加密得到传输工作密钥密文BCD码
	bcd_to_asc((unsigned char *)mm_workkey,(unsigned char *)mm_workkey8,16,0);
	
	dcs_debug(0,0,"<poslogin_proc>\nDATA=传输工作密钥明文:[%s]\nKEY=传输主密钥明文:[%s]\n->DES加密\nDATA=传输工作密钥密文:[%s]",xx_workkey,x_mkey,mm_workkey);
	
	
	char x_keydata1[32+1];//POS密钥数据1明文ASC码
	char x_keydata2[32+1];//POS密钥数据2明文ASC码
	char m_keydata1[32+1];//POS密钥数据1密文ASC码
	char m_keydata2[32+1];//POS密钥数据2密文ASC码

	char x_tmpkey[16+1],x_tmpkey8[8+1];//临时明文数据ASC码和BCD码
	char m_tmpkey[16+1],m_tmpkey8[8+1];//临时密文数据ASC码和BCD码
	
	
	memset(x_keydata1,0,sizeof(x_keydata1));
	memset(x_keydata2,0,sizeof(x_keydata2));
	memset(m_keydata1,0,sizeof(m_keydata1));
	memset(m_keydata2,0,sizeof(m_keydata2));
	
	if(0>getshopkey(organcode,shopid,termid,x_keydata1,x_keydata2))
		return -1;

	memset(x_tmpkey,0,sizeof(x_tmpkey));memset(x_tmpkey8,0,sizeof(x_tmpkey8));
	memset(m_tmpkey,0,sizeof(m_tmpkey));memset(m_tmpkey8,0,sizeof(m_tmpkey8));
	memcpy(x_tmpkey,x_keydata1,16);
	asc_to_bcd((unsigned char *)x_tmpkey8,(unsigned char *)x_tmpkey,16,0);
	DES(xx_workkey8,x_tmpkey8,m_tmpkey8);//加密得到密钥数据前半部分密文BCD码
	bcd_to_asc((unsigned char *)m_tmpkey,(unsigned char *)m_tmpkey8,16,0);	
	memcpy(m_keydata1,m_tmpkey,16);
	
	memset(x_tmpkey,0,sizeof(x_tmpkey));memset(x_tmpkey8,0,sizeof(x_tmpkey8));
	memset(m_tmpkey,0,sizeof(m_tmpkey));memset(m_tmpkey8,0,sizeof(m_tmpkey8));
	memcpy(x_tmpkey,x_keydata1+16,16);
	asc_to_bcd((unsigned char *)x_tmpkey8,(unsigned char *)x_tmpkey,16,0);
	DES(xx_workkey8,x_tmpkey8,m_tmpkey8);//加密得到密钥数据后半部分密文BCD码
	bcd_to_asc((unsigned char *)m_tmpkey,(unsigned char *)m_tmpkey8,16,0);	
	memcpy(m_keydata1+16,m_tmpkey,16);

	memset(x_tmpkey,0,sizeof(x_tmpkey));memset(x_tmpkey8,0,sizeof(x_tmpkey8));
	memset(m_tmpkey,0,sizeof(m_tmpkey));memset(m_tmpkey8,0,sizeof(m_tmpkey8));
	memcpy(x_tmpkey,x_keydata2,16);
	asc_to_bcd((unsigned char *)x_tmpkey8,(unsigned char *)x_tmpkey,16,0);
	DES(xx_workkey8,x_tmpkey8,m_tmpkey8);//加密得到密钥数据前半部分密文BCD码
	bcd_to_asc((unsigned char *)m_tmpkey,(unsigned char *)m_tmpkey8,16,0);	
	memcpy(m_keydata2,m_tmpkey,16);
	
	memset(x_tmpkey,0,sizeof(x_tmpkey));memset(x_tmpkey8,0,sizeof(x_tmpkey8));
	memset(m_tmpkey,0,sizeof(m_tmpkey));memset(m_tmpkey8,0,sizeof(m_tmpkey8));
	memcpy(x_tmpkey,x_keydata2+16,16);
	asc_to_bcd((unsigned char *)x_tmpkey8,(unsigned char *)x_tmpkey,16,0);
	DES(xx_workkey8,x_tmpkey8,m_tmpkey8);//加密得到密钥数据后半部分密文BCD码
	bcd_to_asc((unsigned char *)m_tmpkey,(unsigned char *)m_tmpkey8,16,0);	
	memcpy(m_keydata2+16,m_tmpkey,16);

	dcs_debug(0,0,"<bddown_proc>\nkeydata1:[%s],keydata2:[%s]\nKEY:[%s]\n->DES加密\nkeydata1:[%s],keydata2:[%s]",x_keydata1,x_keydata2,xx_workkey,m_keydata1,m_keydata2);	
	
	//组织返回报文
	struct  tm *tm;   time_t  t;
	
	char posmsg_len[4+1];
	char posmsg_info[4+1];
	char posmsg_handle[4+1];
	char posmsg_encrypt[2+1];
	char posmsg_date[4+1];
	char posmsg_time[6+1];
	char posmsg_seq[6+1];
	int  posmsg_statu[1+1];
	char posmsg_organcodelen[2+1];
	char posmsg_organcode[20+1];
	char posmsg_opid[6+1];
	char posmsg_bmplen[2+1];
	char posmsg_bmp[32+1];
	
	char posmsg_shopidlen[2+1];
	char posmsg_shopid[20+1];
	char posmsg_termidlen[2+1];
	char posmsg_termid[20+1];
	char posmsg_workkey[20+1];
	char posmsg_keydata1[32+1];
	char posmsg_keydata2[32+1];
	char posmsg_mac[16+1];
	
	memset(posmsg_len,0,sizeof(posmsg_len));
	memset(posmsg_info,0,sizeof(posmsg_info));
	memset(posmsg_handle,0,sizeof(posmsg_handle));
	memset(posmsg_encrypt,0,sizeof(posmsg_encrypt));
	memset(posmsg_date,0,sizeof(posmsg_date));
	memset(posmsg_time,0,sizeof(posmsg_time));
	memset(posmsg_seq,0,sizeof(posmsg_seq));
	memset(posmsg_statu,0,sizeof(posmsg_statu));
	memset(posmsg_organcodelen,0,sizeof(posmsg_organcodelen));
	memset(posmsg_organcode,0,sizeof(posmsg_organcode));
	memset(posmsg_opid,0,sizeof(posmsg_opid));
	memset(posmsg_bmplen,0,sizeof(posmsg_bmplen));
	memset(posmsg_bmp,0,sizeof(posmsg_bmp));
	
	memset(posmsg_shopidlen,0,sizeof(posmsg_shopidlen));
	memset(posmsg_shopid,0,sizeof(posmsg_shopid));
	memset(posmsg_termidlen,0,sizeof(posmsg_termidlen));
	memset(posmsg_termid,0,sizeof(posmsg_termid));
	memset(posmsg_workkey,0,sizeof(posmsg_workkey));
	memset(posmsg_keydata1,0,sizeof(posmsg_keydata1));
	memset(posmsg_keydata2,0,sizeof(posmsg_keydata2));
	memset(posmsg_mac,0,sizeof(posmsg_mac));
	
	//dcs_debug(posmsg_handle,4,"posmsg_handle:[%s]",posmsg_handle);
	
	memcpy(posmsg_info,"2000",4);
	//memcpy(posmsg_handle,"0001",4);
	memcpy(posmsg_encrypt,"01",4);
	memcpy(posmsg_seq,"000001",6);
	memcpy(posmsg_statu,"0",1);
	memcpy(posmsg_organcodelen,"10",2);
	memcpy(posmsg_organcode,organcode,atoi(posmsg_organcodelen));
	memcpy(posmsg_opid,msg_opid,6);
	memcpy(posmsg_bmplen,"06",2);
	memcpy(posmsg_bmp,"111111",6);
	memcpy(posmsg_shopidlen,"15",2);
	memcpy(posmsg_shopid,shopid,15);
	memcpy(posmsg_termidlen,"08",2);
	memcpy(posmsg_termid,termid,8);
	memcpy(posmsg_workkey,mm_workkey,16);
	memcpy(posmsg_keydata1,m_keydata1,32);
	memcpy(posmsg_keydata2,m_keydata2,32);

	time(&t);
  tm = localtime(&t);
	sprintf(posmsg_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(posmsg_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	s_len=0;
	s_len=s_len+4;
	
	//dcs_debug(posmsg_handle,4,"posmsg_handle:[%s]",posmsg_handle);
	//消息头
	memcpy(SendBuf+s_len,posmsg_info,4);s_len=s_len+4;
	memcpy(SendBuf+s_len,"0001"/*posmsg_handle*/,4);s_len=s_len+4;
	memcpy(SendBuf+s_len,posmsg_encrypt,2);s_len=s_len+2;
	memcpy(SendBuf+s_len,posmsg_date,4);s_len=s_len+4;
	memcpy(SendBuf+s_len,posmsg_time,6);s_len=s_len+6;
	memcpy(SendBuf+s_len,posmsg_seq,6);s_len=s_len+6;
	memcpy(SendBuf+s_len,posmsg_statu,1);s_len=s_len+1;
	memcpy(SendBuf+s_len,posmsg_organcodelen,2);s_len=s_len+2;
	memcpy(SendBuf+s_len,posmsg_organcode,atoi(posmsg_organcodelen));s_len=s_len+atoi(posmsg_organcodelen);
	memcpy(SendBuf+s_len,posmsg_opid,6);s_len=s_len+6;
	memcpy(SendBuf+s_len,posmsg_bmplen,2);s_len=s_len+2;
	memcpy(SendBuf+s_len,posmsg_bmp,atoi(posmsg_bmplen));s_len=s_len+atoi(posmsg_bmplen);
	
	//消息体
	memcpy(SendBuf+s_len,posmsg_shopidlen,2);s_len=s_len+2;
	memcpy(SendBuf+s_len,posmsg_shopid,atoi(posmsg_shopidlen));s_len=s_len+atoi(posmsg_shopidlen);
	memcpy(SendBuf+s_len,posmsg_termidlen,2);s_len=s_len+2;
	memcpy(SendBuf+s_len,posmsg_termid,atoi(posmsg_termidlen));s_len=s_len+atoi(posmsg_termidlen);
	memcpy(SendBuf+s_len,posmsg_workkey,16);s_len=s_len+16;
	memcpy(SendBuf+s_len,posmsg_keydata1,32);s_len=s_len+32;
	memcpy(SendBuf+s_len,posmsg_keydata2,32);s_len=s_len+32;
	memcpy(SendBuf+s_len,posmsg_mac,16);s_len=s_len+16;
	
	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(SendBuf,m_len,4);
	
	GetPosMac(xx_workkey , SendBuf+4 , s_len-4-16 , posmsg_mac);
	memcpy(SendBuf+s_len-16,posmsg_mac,16);
	
	return s_len;
}

int verifyreceivebuf(char * ReceiveBuf,int ReceiveLen)
{
	int s_len=0;
	
	char posmsg_len[4+1];
	char posmsg_info[4+1];
	char posmsg_handle[4+1];
	char posmsg_encrypt[2+1];
	char posmsg_date[4+1];
	char posmsg_time[6+1];
	char posmsg_seq[6+1];
	int  posmsg_statu[1+1];
	char posmsg_organcodelen[2+1];
	char posmsg_organcode[20+1];
	char posmsg_opid[6+1];
	char posmsg_bmplen[2+1];
	char posmsg_bmp[32+1];
	char posmsg_databuff[1024+1];
	
	char posmsg_ret[2+1];
	char posmsg_mac[16+1];
	
	memset(posmsg_len,0,sizeof(posmsg_len));
	memset(posmsg_info,0,sizeof(posmsg_info));
	memset(posmsg_handle,0,sizeof(posmsg_handle));
	memset(posmsg_encrypt,0,sizeof(posmsg_encrypt));
	memset(posmsg_date,0,sizeof(posmsg_date));
	memset(posmsg_time,0,sizeof(posmsg_time));
	memset(posmsg_seq,0,sizeof(posmsg_seq));
	memset(posmsg_statu,0,sizeof(posmsg_statu));
	memset(posmsg_organcodelen,0,sizeof(posmsg_organcodelen));
	memset(posmsg_organcode,0,sizeof(posmsg_organcode));
	memset(posmsg_opid,0,sizeof(posmsg_opid));
	memset(posmsg_bmplen,0,sizeof(posmsg_bmplen));
	memset(posmsg_bmp,0,sizeof(posmsg_bmp));
	
	memset(posmsg_ret,0,sizeof(posmsg_ret));
	memset(posmsg_mac,0,sizeof(posmsg_mac));
		
	
	memset(posmsg_len, 0, sizeof(msg_len) );
	memset(posmsg_info, 0, sizeof(msg_info) );
	memset(posmsg_handle, 0, sizeof(msg_handle) );
	memset(posmsg_encrypt, 0, sizeof(msg_encrypt) );
	memset(posmsg_date, 0, sizeof(msg_date) );
	memset(posmsg_time, 0, sizeof(msg_time) );
	memset(posmsg_seq, 0, sizeof(msg_seq) );
	memset(posmsg_statu, 0, sizeof(msg_statu) );
	memset(posmsg_organcodelen, 0, sizeof(msg_organcodelen) );
	memset(posmsg_organcode, 0, sizeof(msg_organcode) );
	memset(posmsg_opid, 0, sizeof(msg_opid) );
	memset(posmsg_bmplen, 0, sizeof(msg_bmplen) );
	memset(posmsg_bmp, 0, sizeof(msg_bmp) );
	memset(posmsg_databuff, 0, sizeof(msg_databuff) );
		     
	memcpy(posmsg_len,ReceiveBuf,4);s_len=s_len+4;
	memcpy(posmsg_info,ReceiveBuf+s_len,4);s_len=s_len+4;
	memcpy(posmsg_handle,ReceiveBuf+s_len,4);s_len=s_len+4;
	memcpy(posmsg_encrypt,ReceiveBuf+s_len,2);s_len=s_len+2;
	memcpy(posmsg_date,ReceiveBuf+s_len,4);s_len=s_len+4;
	memcpy(posmsg_time,ReceiveBuf+s_len,6);s_len=s_len+6;
	memcpy(posmsg_seq,ReceiveBuf+s_len,6);s_len=s_len+6;
	memcpy(posmsg_statu,ReceiveBuf+s_len,1);s_len=s_len+1;
	memcpy(posmsg_organcodelen,ReceiveBuf+s_len,2);s_len=s_len+2;
	memcpy(posmsg_organcode,ReceiveBuf+s_len,atoi(posmsg_organcodelen));s_len=s_len+atoi(posmsg_organcodelen);
	memcpy(posmsg_opid,ReceiveBuf+s_len,6);s_len=s_len+6;
	memcpy(posmsg_bmplen,ReceiveBuf+s_len,2);s_len=s_len+2;
	memcpy(posmsg_bmp,ReceiveBuf+s_len,atoi(posmsg_bmplen));s_len=s_len+atoi(posmsg_bmplen);
	memcpy(posmsg_databuff,ReceiveBuf+s_len,ReceiveLen-s_len);		
	
	dcs_log(0,0,"<getposheadinfo>\n\
报文长度:%s\n\
消息类型:%s\n\
操作类型:%s\n\
加密方式:%s\n\
交易日期:%s\n\
交易时间:%s\n\
交易流水:%s\n\
传输状态:%s\n\
机构代码长度:%s\n\
机构代码:%s\n\
操作员ID:%s\n\
BITMAP图长度:%s\n\
BITMAP图:%s\n\
消息数据:%s",posmsg_len,posmsg_info,posmsg_handle,posmsg_encrypt,posmsg_date,posmsg_time,posmsg_seq,posmsg_statu,posmsg_organcodelen,posmsg_organcode,posmsg_opid,posmsg_bmplen,posmsg_bmp,posmsg_databuff);	

	if ( memcmp( posmsg_info ,"2001",4) ==0 && memcmp( posmsg_handle ,"0001",4) ==0 )
	{
		s_len=0;	
		memcpy(posmsg_ret,posmsg_databuff+s_len,2);s_len=s_len+2;
		memcpy(posmsg_mac,posmsg_databuff+s_len,16);s_len=s_len+16;

		if(0>VerifyPosMac(xx_workkey , ReceiveBuf+4 , ReceiveLen-4-16 , posmsg_mac))
			return -1;
		
		if ( (memcmp( posmsg_ret ,"00",2) ==0) )	return 1;
		else	return -1;
	}
	else	return -1;
		
}

void shopinfofind_proc( char * srcBuf, int iConnSock, int iLen)
{
	int s_len;
	
	//char m_down[1+1];
	char m_shopidlen[2+1];
	char m_shopid[20+1];
	char m_termidlen[2+1];
	char m_termid[20+1];
	char m_shopnamelen[2+1];
	char m_shopname[120+1];
	char m_begintime[14+1];
	char m_endtime[14+1];
	char m_beginpage[6+1];
	char m_mac[16+1];
	
	int m_datalen=0;
	char m_data[1024+1];
	
	//memset(m_down,0,sizeof(m_down));
	memset(m_shopidlen,0,sizeof(m_shopidlen));
	memset(m_shopid,0,sizeof(m_shopid));
	memset(m_termidlen,0,sizeof(m_termidlen));
	memset(m_termid,0,sizeof(m_termid));
	memset(m_shopnamelen,0,sizeof(m_shopnamelen));
	memset(m_shopname,0,sizeof(m_shopname));
	memset(m_begintime,0,sizeof(m_begintime));
	memset(m_endtime,0,sizeof(m_endtime));
	memset(m_beginpage,0,sizeof(m_beginpage));
	memset(m_mac,0,sizeof(m_mac));
	
	memset(m_data,0,sizeof(m_data));
	
	s_len=0;
	
	char whe[1024+1];
	char whe0[1024+1];
	char whe1[128+1];
	char whe2[128+1];
	char whe3[128+1];
	char whe4[128+1];
	char whe5[128+1];
	char whe6[128+1];
	char whe7[128+1];
	memset(whe,0,sizeof(whe));
	memset(whe0,0,sizeof(whe0));
	memset(whe1,0,sizeof(whe1));
	memset(whe2,0,sizeof(whe2));
	memset(whe3,0,sizeof(whe3));
	memset(whe4,0,sizeof(whe4));
	memset(whe5,0,sizeof(whe5));
	memset(whe6,0,sizeof(whe6));
	memset(whe7,0,sizeof(whe7));
	
	if(memcmp(msg_bmp,"1",1)==0)
	{
		memcpy(m_shopidlen,msg_databuff+s_len,2);s_len=s_len+2;
		memcpy(m_shopid,msg_databuff+s_len,atoi(m_shopidlen));s_len=s_len+atoi(m_shopidlen);
		sprintf(whe1,"shopid like '%s%s%s' and ","%",m_shopid,"%");
	}
	if(memcmp(msg_bmp+1,"1",1)==0)
	{
		memcpy(m_termidlen,msg_databuff+s_len,2);s_len=s_len+2;
		memcpy(m_termid,msg_databuff+s_len,atoi(m_termidlen));s_len=s_len+atoi(m_termidlen);
		sprintf(whe2,"termid like '%s%s%s' and ","%",m_termid,"%");
	}
	if(memcmp(msg_bmp+2,"1",1)==0)
	{
		memcpy(m_shopnamelen,msg_databuff+s_len,2);s_len=s_len+2;
		memcpy(m_shopname,msg_databuff+s_len,atoi(m_shopnamelen));s_len=s_len+atoi(m_shopnamelen);
		sprintf(whe3,"shopname like '%s%s%s' and ","%",m_shopname,"%");
	}
	if(memcmp(msg_bmp+3,"1",1)==0)
	{
		memcpy(m_begintime,msg_databuff+s_len,8);s_len=s_len+8;
		sprintf(whe4,"addtime >= '%s%s' and ",m_begintime,"000000");
	}
	if(memcmp(msg_bmp+4,"1",1)==0)
	{
		memcpy(m_endtime,msg_databuff+s_len,8);s_len=s_len+8;
		sprintf(whe5,"addtime <= '%s%s' and ",m_endtime,"235959");
	}
	if(memcmp(msg_bmp+5,"1",1)==0)
	{
		memcpy(m_beginpage,msg_databuff+s_len,6);s_len=s_len+6;
	}
	if(memcmp(msg_bmp+6,"1",1)==0)
	{
		memcpy(m_mac,msg_databuff+s_len,16);s_len=s_len+16;
	}
	
	sprintf(whe6,"organcode = '%s' and ",msg_organcode);

	sprintf(whe0,"%s%s%s%s%s%s",whe1,whe2,whe3,whe4,whe5,whe6);
	if(strlen(whe0)!=0)	memcpy(whe,whe0,strlen(whe0)-5);	

	if(0>VerifyPcMac(x_workkey , srcBuf+4 , iLen-4-16 , m_mac))
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"02",m_shopid,m_termid,msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	char over_bz[1+1];
	memset(over_bz,0,sizeof(over_bz));
	
	if( 0>getshopinfo(whe,m_data,atoi(m_beginpage),atoi(m_beginpage)+10,over_bz) )
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"03",m_shopid,m_termid,msg_organcode);
		dcs_log(0,0,"<shopinfofind_proc> get shopkey error , %s ,%s !",msg_organcode,m_shopid,m_termid);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"商户密钥数据表获取记录失败!");
		ErrResponseForPC(srcBuf,"03",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	if(strlen(m_data)>0)	m_datalen=strlen(m_data)-1;
	else	m_datalen=0;

	
	//dcs_debug(m_data,m_datalen,"m_data [%d]",m_datalen);

	
	//组织返回报文
	struct  tm *tm;   time_t  t;
	char msg_ret[2+1];
	char msg_datalen[2+1];
	char msg_data[1024+1];
	char msg_mac[16+1];

	memset(msg_info,0,sizeof(msg_info));
	memset(msg_statu,0,sizeof(msg_statu));
	memset(msg_bmplen,0,sizeof(msg_bmplen));
	memset(msg_bmp,0,sizeof(msg_bmp));
	memset(msg_ret,0,sizeof(msg_ret));
	memset(msg_datalen,0,sizeof(msg_datalen));
	memset(msg_data,0,sizeof(msg_data));
	memset(msg_mac,0,sizeof(msg_mac));
	
	memcpy(msg_info,"0041",4);
	if(memcmp(over_bz,"1",1)==0)	memcpy(msg_statu,"0",1);
	else	memcpy(msg_statu,"1",1);
	memcpy(msg_bmplen,"02",2);
	memcpy(msg_bmp,"11",2);
	//memcpy(msg_ret,"00",2);
	sprintf(msg_datalen,"%03d",m_datalen);
	memcpy(msg_data,m_data,m_datalen);

	time(&t);
  tm = localtime(&t);
	sprintf(msg_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(msg_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	s_len=0;
	s_len=s_len+4;
	
	//消息头
	memcpy(srcBuf+s_len,msg_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_statu,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,msg_organcodelen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_organcode,atoi(msg_organcodelen));s_len=s_len+atoi(msg_organcodelen);
	memcpy(srcBuf+s_len,msg_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_bmplen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_bmp,atoi(msg_bmplen));s_len=s_len+atoi(msg_bmplen);
	
	//消息体
	memcpy(srcBuf+s_len,"00"/*msg_ret*/,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_datalen,3);s_len=s_len+3;
	memcpy(srcBuf+s_len,msg_data,atoi(msg_datalen));s_len=s_len+atoi(msg_datalen);
	memcpy(srcBuf+s_len,msg_mac,16);s_len=s_len+16;

	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	GetPcMac(x_workkey , srcBuf+4 , s_len-4-16 , msg_mac);
	memcpy(srcBuf+s_len-16,msg_mac,16);
	
  dcs_debug(srcBuf,s_len,"<shopinfofind_proc> 返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
	write_msg_to_AMP(iConnSock, srcBuf, s_len,60000); 
	
	insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"00",m_shopid,m_termid,msg_organcode);
	
}

int zddownkfd_proc( char * srcBuf,int  iConnSock, int iLen )
{
	int s_len;
	//char m_shopidlen[2+1];
	char m_shopid[20+1];
	//char m_termidlen[2+1];
	char m_termid[20+1];
	char m_posiplen[2+1];
	char m_posip[20+1];
	char m_posportlen[2+1];
	char m_posport[10+1];
	char m_mac[16+1];
	
	char SendBuf[2048];
	char ReceiveBuf[2048];
	int SendLen=0;
	int ReceiveLen=0;
	
	//memset(m_shopidlen,0,sizeof(m_shopidlen));
	memset(m_shopid,0,sizeof(m_shopid));
	//memset(m_termidlen,0,sizeof(m_termidlen));
	memset(m_termid,0,sizeof(m_termid));
	memset(m_posiplen,0,sizeof(m_posiplen));
	memset(m_posip,0,sizeof(m_posip));
	memset(m_posportlen,0,sizeof(m_posportlen));
	memset(m_posport,0,sizeof(m_posport));
	memset(m_mac,0,sizeof(m_mac));
	
	s_len=0;
	//memcpy(m_shopidlen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_shopid,msg_databuff+s_len,15);s_len=s_len+15;
	//memcpy(m_termidlen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_termid,msg_databuff+s_len,8);s_len=s_len+8;
	memcpy(m_posiplen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_posip,msg_databuff+s_len,atoi(m_posiplen));s_len=s_len+atoi(m_posiplen);
	memcpy(m_posportlen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_posport,msg_databuff+s_len,atoi(m_posportlen));s_len=s_len+atoi(m_posportlen);
	memcpy(m_mac,msg_databuff+s_len,16);s_len=s_len+16;
	
	dcs_debug(0,0,"%s",msg_databuff);
	dcs_debug(0,0,"%s,%s,%s,%s,%s,%s,%s",m_shopid,m_termid,m_posiplen,m_posip,m_posportlen,m_posport,m_mac);
	
	if(0>VerifyPcMac(x_workkey , srcBuf+4 , iLen-4-16 , m_mac))
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"02",m_shopid,m_termid,msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}
	
	memset(SendBuf,0,sizeof(SendBuf));
	memset(ReceiveBuf,0,sizeof(ReceiveBuf));
	
	SendLen=setsendbuf(m_shopid,m_termid,msg_organcode,SendBuf);
	if(SendLen<=0)
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"03",m_shopid,m_termid,msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"密钥下发失败!");
		ErrResponseForPC(srcBuf,"03",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}
	
	/*ReceiveLen=sendpos(m_posip,m_posport,SendBuf,SendLen,ReceiveBuf);
	if(ReceiveLen<=0)
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"密钥下发失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}
	
	if( 0>verifyreceivebuf(ReceiveBuf,ReceiveLen) )
	{
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"密钥下发失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}*/

	//组织返回报文
	struct  tm *tm;   time_t  t;
	char msg_ret[2+1];
	char msg_sendposdatalen[3+1];
	char msg_sendposdata[1024+1];
	char msg_mac[16+1];
	
	memset(msg_info,0,sizeof(msg_info));
	memset(msg_bmplen,0,sizeof(msg_bmplen));
	memset(msg_bmp,0,sizeof(msg_bmp));
	memset(msg_ret,0,sizeof(msg_ret));
	memset(msg_sendposdatalen,0,sizeof(msg_sendposdatalen));
	memset(msg_sendposdata,0,sizeof(msg_sendposdata));
	memset(msg_mac,0,sizeof(msg_mac));
	
	memcpy(msg_info,"0041",4);
	memcpy(msg_bmplen,"03",2);
	memcpy(msg_bmp,"111",3);
	memcpy(msg_ret,"00",2);
	sprintf(msg_sendposdatalen,"%03d",SendLen);
	memcpy(msg_sendposdata,SendBuf,SendLen);

	time(&t);
  tm = localtime(&t);
	sprintf(msg_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(msg_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	s_len=0;
	s_len=s_len+4;
	
	//消息头
	memcpy(srcBuf+s_len,msg_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_statu,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,msg_organcodelen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_organcode,atoi(msg_organcodelen));s_len=s_len+atoi(msg_organcodelen);
	memcpy(srcBuf+s_len,msg_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_bmplen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_bmp,atoi(msg_bmplen));s_len=s_len+atoi(msg_bmplen);
	
	//消息体
	memcpy(srcBuf+s_len,msg_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_sendposdatalen,3);s_len=s_len+3;
	memcpy(srcBuf+s_len,msg_sendposdata,atoi(msg_sendposdatalen));s_len=s_len+atoi(msg_sendposdatalen);
	memcpy(srcBuf+s_len,msg_mac,16);s_len=s_len+16;
	
	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	GetPcMac(x_workkey , srcBuf+4 , s_len-4-16 , msg_mac);
	memcpy(srcBuf+s_len-16,msg_mac,16);
	
  dcs_debug(srcBuf,s_len,"<zddownkfd_proc> 返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
	write_msg_to_AMP(iConnSock, srcBuf, s_len,60000); 
	
	insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"00",m_shopid,m_termid,msg_organcode);
	
	return 1;	
}

void opinfomodipsw_proc( char * srcBuf, int iConnSock, int iLen)
{
	int s_len;
	
	//char m_down[1+1];
	char m_oldpsw[16+1];
	char m_newpsw[16+1];
	char m_mac[16+1];
	
	
	memset(m_oldpsw,0,sizeof(m_oldpsw));
	memset(m_newpsw,0,sizeof(m_newpsw));
	memset(m_mac,0,sizeof(m_mac));
	
	s_len=0;
	memcpy(m_oldpsw,msg_databuff+s_len,16);s_len=s_len+16;
	memcpy(m_newpsw,msg_databuff+s_len,16);s_len=s_len+16;
	memcpy(m_mac,msg_databuff+s_len,16);
		
	if(0>VerifyPcMac(x_workkey , srcBuf+4 , iLen-4-16 , m_mac))
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"02","null","null",msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC检验错误,非法报文!");
		ErrResponseForPC(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	
	
	if( 0>opinfomodipsw(msg_opid,msg_organcode,m_oldpsw,m_newpsw) )
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"01","null","null",msg_organcode);
		dcs_log(0,0,"<opinfomodipsw_proc> opinfo modify password error , opid:[%s] !",msg_opid);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"操作员密码修改失败!");
		ErrResponseForPC(srcBuf,"01",iConnSock,iLen,errmsg,strlen(errmsg));
		return;
	}
	
	
	//组织返回报文
	struct  tm *tm;   time_t  t;
	char msg_ret[2+1];
	char msg_mac[16+1];

	memset(msg_info,0,sizeof(msg_info));
	memset(msg_bmplen,0,sizeof(msg_bmplen));
	memset(msg_bmp,0,sizeof(msg_bmp));
	memset(msg_ret,0,sizeof(msg_ret));
	memset(msg_mac,0,sizeof(msg_mac));
	
	memcpy(msg_info,"0021",4);
	memcpy(msg_bmplen,"02",2);
	memcpy(msg_bmp,"11",2);
	//memcpy(msg_ret,"00",2);

	time(&t);
  tm = localtime(&t);
	sprintf(msg_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(msg_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	s_len=0;
	s_len=s_len+4;
	
	//消息头
	memcpy(srcBuf+s_len,msg_info,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_handle,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_encrypt,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_date,4);s_len=s_len+4;
	memcpy(srcBuf+s_len,msg_time,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_seq,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_statu,1);s_len=s_len+1;
	memcpy(srcBuf+s_len,msg_organcodelen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_organcode,atoi(msg_organcodelen));s_len=s_len+atoi(msg_organcodelen);
	memcpy(srcBuf+s_len,msg_opid,6);s_len=s_len+6;
	memcpy(srcBuf+s_len,msg_bmplen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_bmp,atoi(msg_bmplen));s_len=s_len+atoi(msg_bmplen);
	
	//消息体
	memcpy(srcBuf+s_len,"00"/*msg_ret*/,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_mac,16);s_len=s_len+16;

	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	GetPcMac(x_workkey , srcBuf+4 , s_len-4-16 , msg_mac);
	memcpy(srcBuf+s_len-16,msg_mac,16);
	
  dcs_debug(srcBuf,s_len,"<opinfomodipsw_proc> 返回报文 iConnSock =%d, len=%d",iConnSock,s_len);
	write_msg_to_AMP(iConnSock, srcBuf, s_len,60000); 
	
	insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"00","null","null",msg_organcode);	
}

void shopinfodetafind_proc( char * srcBuf, int iConnSock, int iLen)
{
}

void shopkeyadd_proc( char * srcBuf, int iConnSock, int iLen)
{
}

void shopkeymodi_proc( char * srcBuf, int iConnSock, int iLen)
{
}

void shopkeydele_proc( char * srcBuf, int iConnSock, int iLen)
{
}

void organadd_proc( char * srcBuf, int iConnSock, int iLen)
{
}

void organmodi_proc( char * srcBuf, int iConnSock, int iLen)
{
}

void organdele_proc( char * srcBuf, int iConnSock, int iLen)
{
}

