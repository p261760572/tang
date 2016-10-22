#include "ibdcs.h"
#include <time.h>
//#include "poskey.h"

char msg_len[4+1];          //���ĳ���(�����ֶ�,�Ҷ���,�����'0')
char msg_info[4+1];         //��Ϣ����
char msg_handle[4+1];       //��������
char msg_encrypt[2+1];      //���ܷ�ʽ(���ļ��ܷ�ʽ,00��ʾ���Ĵ���,01��ʾ����ܴ���,02��ʾӲ������)
char msg_date[4+1];         //��������
char msg_time[6+1];         //����ʱ��
char msg_seq[6+1];          //������ˮ
int  msg_statu[1+1];        //���Ĵ���״̬(0��ʾ���ݴ������,1��ʾ��������δ����)
char msg_organcodelen[2+1]; //�������볤��
char msg_organcode[20+1];     //��������
char msg_opid[6+1];         //����ԱID
char msg_bmplen[2+1];       //BITMAPͼ����(0������ֶ�û��ֵ,1������ֶ���ֵ)
char msg_bmp[32+1];         //BITMAPͼ
char msg_databuff[1024+1];	//��Ϣ����

char m_workkey[16+1],m_workkey8[8+1];//���乤����Կ����ASC���BCD��
char x_workkey[16+1],x_workkey8[8+1];//���乤����Կ����ASC���BCD��

char errmsg[100];

//MsgData msg;
//int VerifyPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * inPcMac);
//int GetPcMac(char * caMKey , char * caWKey , char * caData , int iLen , char * outPcMac);
//void ErrResponseForPC(char * srcBuf,char *errCode,int iConnSock, int iLen,char *msg,int msglen);

void getposheadinfo( char * srcBuf,int  iConnSock, int iLen );
int	poslogin_proc( char * srcBuf,int  iConnSock, int iLen );
void ErrResponseForPOS(char * srcBuf,char *errCode,int iConnSock, int iLen,char *errmsg,int errmsglen);
int bddown_proc( char * srcBuf,int  iConnSock, int iLen );

void pos_proc( char * srcBuf,int  iFid , int iLen) 
{
	getposheadinfo( srcBuf,iFid,iLen );
	
	if ( (memcmp( msg_info ,"1000",4) ==0) )    // POSǩ�����뱨��
	{
		dcs_log(0,0,"<pos_proc> POSǩ�����뱨��");
		if(0>poslogin_proc( srcBuf, iFid, iLen ))
		{
			dcs_log(0,0,"<pos_proc> POSǩ������ʧ��");
			return;
		}
	}
	else if ( (memcmp( msg_info ,"2000",4) ==0) && (memcmp( msg_handle ,"0002",4) ==0) )    // POS��Կ�����·�����
	{
		dcs_log(0,0,"<pos_proc> ��Կ�����·�����");
		if(0>bddown_proc( srcBuf, iFid, iLen ))
		{
			dcs_log(0,0,"<pos_proc> POS��Կ�����·�ʧ��");
			return;
		}
	}
	else
		dcs_log(0,0,"Unkown Packet,discard !");
}

void getposheadinfo( char * srcBuf,int  iConnSock, int iLen )
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
���ĳ���:%s\n\
��Ϣ����:%s\n\
��������:%s\n\
���ܷ�ʽ:%s\n\
��������:%s\n\
����ʱ��:%s\n\
������ˮ:%s\n\
����״̬:%s\n\
�������볤��:%s\n\
��������:%s\n\
����ԱID:%s\n\
BITMAPͼ����:%s\n\
BITMAPͼ:%s\n\
��Ϣ����:%s",msg_len,msg_info,msg_handle,msg_encrypt,msg_date,msg_time,msg_seq,msg_statu,msg_organcodelen,msg_organcode,msg_opid,msg_bmplen,msg_bmp,msg_databuff);	
	
}

void ErrResponseForPOS(char * srcBuf,char *errCode,int iConnSock, int iLen,char *errmsg,int errmsglen)
{
	//��֯���ر���
	struct  tm *tm;   time_t  t;
	char msg_ret[2+1];
	char msg_errmsglen[2+1];
	char msg_errmsg[100+1];
	int s_len;
	
	memset(msg_ret,0,sizeof(msg_ret));
	memset(msg_errmsglen,0,sizeof(msg_errmsglen));
	memset(msg_errmsg,0,sizeof(msg_errmsg));
	
	if ( memcmp( msg_info ,"1000",4) ==0)    		// POSǩ�����뱨��
	{
		memcpy(msg_info,"1001",4);
	}
	else if ( memcmp( msg_info ,"2000",4) ==0)    // POS��Կ�·�����
	{
		memcpy(msg_info,"2001",4);
	}
	else
	{
		dcs_debug(0,0,"<ErrResponseForPC> ��Ч���ġ�");
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
	
	//��Ϣͷ
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
	
	//��Ϣ��
	memcpy(srcBuf+s_len,msg_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_errmsglen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_errmsg,atoi(msg_errmsglen));s_len=s_len+atoi(msg_errmsglen);
	
	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
  dcs_debug(srcBuf,s_len,"<ErrResponseForPOS> ���ر��� iConnSock =%d, len=%d",iConnSock,s_len);
	write_msg_to_AMP(iConnSock, srcBuf, s_len,60000); 	
}

int	poslogin_proc( char * srcBuf,int  iConnSock, int iLen )
{

	int s_len;
	char m_organcodelen[2+1];
	char m_organcode[20];
	char m_mkey[16+1],m_mkey8[8+1];//��������Կ����ASC���BCD��
	char x_mkey[16+1],x_mkey8[8+1];//��������Կ����ASC���BCD��
	char m_factor[16+1],m_factor8[8+1];//��ɢ����ASC���BCD��
	char m_random[16+1],m_random8[8+1];//���ڲ���������Կ�����ASC���BCD��
	
	//getposheadinfo( srcBuf,iConnSock,iLen );
	
	memset(m_organcodelen,0,sizeof(m_organcodelen));
	memset(m_organcode,0,sizeof(m_organcode));
	memset(m_mkey,0,sizeof(m_mkey));memset(m_mkey8,0,sizeof(m_mkey8));
	memset(x_mkey,0,sizeof(x_mkey));memset(x_mkey8,0,sizeof(x_mkey8));
	memset(m_factor,0,sizeof(m_factor));memset(m_factor8,0,sizeof(m_factor8));
	memset(m_random,0,sizeof(m_random));memset(m_random8,0,sizeof(m_random8));
	memset(m_workkey,0,sizeof(m_workkey));memset(m_workkey8,0,sizeof(m_workkey8));
	memset(x_workkey,0,sizeof(x_workkey));memset(x_workkey8,0,sizeof(x_workkey8));
	
	s_len=0;
	memcpy(m_organcodelen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_organcode,msg_databuff+s_len,atoi(m_organcodelen));s_len=s_len+atoi(m_organcodelen);
	
	if(0>getorgan(m_organcode,m_mkey,m_factor))
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"03","null","null",msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"��Ч�Ļ�������");
		ErrResponseForPOS(srcBuf,"03",iConnSock,iLen,errmsg,strlen(errmsg));
		
		return -1;
	}
	
	asc_to_bcd((unsigned char *)m_mkey8,(unsigned char *)m_mkey,16,0);
	asc_to_bcd((unsigned char *)m_factor8,(unsigned char *)m_factor,16,0);
	_DES(m_factor8,m_mkey8,x_mkey8);//���ܵõ���������Կ����BCD��
	bcd_to_asc((unsigned char *)x_mkey,(unsigned char *)x_mkey8,16,0);	

	dcs_debug(0,0,"<poslogin_proc>\nDATA=��������Կ����:[%s]\nKEY=��ɢ����:[%s]\n->DES����\nDATA=��������Կ����:[%s]",m_mkey,m_factor,x_mkey);
	
	GetRandomData(m_random);
	asc_to_bcd((unsigned char *)m_random8,(unsigned char *)m_random,16,0);
	DES(x_mkey8,m_random8,x_workkey8);//���ܵõ����乤����Կ����BCD��
	bcd_to_asc((unsigned char *)x_workkey,(unsigned char *)x_workkey8,16,0);
	
	dcs_debug(0,0,"<poslogin_proc>\nDATA=�����:[%s]\nKEY=��������Կ����:[%s]\n->DES����\nDATA=���乤����Կ����:[%s]",m_random,x_mkey,x_workkey);
	
	DES(x_mkey8,x_workkey8,m_workkey8);//���ܵõ����乤����Կ����BCD��
	bcd_to_asc((unsigned char *)m_workkey,(unsigned char *)m_workkey8,16,0);
	
	dcs_debug(0,0,"<poslogin_proc>\nDATA=���乤����Կ����:[%s]\nKEY=��������Կ����:[%s]\n->DES����\nDATA=���乤����Կ����:[%s]",x_workkey,x_mkey,m_workkey);


	//��֯���ر���
	struct  tm *tm;   time_t  t;
	char msg_ret[2+1];
	char msg_workkeylen[2+1];
	char msg_workkey[16+1];
	//char msg_mac[16+1];
	
	memset(msg_info,0,sizeof(msg_info));
	memset(msg_bmplen,0,sizeof(msg_bmplen));
	memset(msg_bmp,0,sizeof(msg_bmp));
	memset(msg_ret,0,sizeof(msg_ret));
	memset(msg_workkeylen,0,sizeof(msg_workkeylen));
	memset(msg_workkey,0,sizeof(msg_workkey));
	//memset(msg_mac,0,sizeof(msg_mac));
	
	memcpy(msg_info,"1001",4);
	memcpy(msg_bmplen,"02",2);
	memcpy(msg_bmp,"11",2);
	memcpy(msg_ret,"00",2);
	memcpy(msg_workkeylen,"16",2);
	memcpy(msg_workkey,m_workkey,16);

	time(&t);
  tm = localtime(&t);
	sprintf(msg_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(msg_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	s_len=0;
	s_len=s_len+4;
	
	//��Ϣͷ
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
	
	//��Ϣ��
	memcpy(srcBuf+s_len,msg_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_workkeylen,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_workkey,atoi(msg_workkeylen));s_len=s_len+atoi(msg_workkeylen);
	//memcpy(srcBuf+s_len,msg_mac,16);s_len=s_len+16;
	
	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	//GetPosMac(x_workkey , srcBuf+4 , s_len-4-16 , msg_mac);
	//memcpy(srcBuf+s_len-16,msg_mac,16);
	
  dcs_debug(srcBuf,s_len,"<poslogin_proc> ���ر��� iConnSock =%d, len=%d",iConnSock,s_len);
	write_msg_to_AMP(iConnSock, srcBuf, s_len,60000); 
	
	insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"00","null","null",msg_organcode);
	
	return 1;
}

int bddown_proc( char * srcBuf,int  iConnSock, int iLen )
{
	int s_len;
	char m_shopidlen[2+1];
	char m_shopid[20+1];
	char m_termidlen[2+1];
	char m_termid[20+1];
	char m_mac[16+1];
	
	char x_keydata1[32+1];//POS��Կ����1����ASC��
	char x_keydata2[32+1];//POS��Կ����2����ASC��
	char m_keydata1[32+1];//POS��Կ����1����ASC��
	char m_keydata2[32+1];//POS��Կ����2����ASC��

	char x_tmpkey[16+1],x_tmpkey8[8+1];//��ʱ��������ASC���BCD��
	char m_tmpkey[16+1],m_tmpkey8[8+1];//��ʱ��������ASC���BCD��
	
	memset(m_shopidlen,0,sizeof(m_shopidlen));
	memset(m_shopid,0,sizeof(m_shopid));
	memset(m_termidlen,0,sizeof(m_termidlen));
	memset(m_termid,0,sizeof(m_termid));
	memset(m_mac,0,sizeof(m_mac));
	memset(x_keydata1,0,sizeof(x_keydata1));
	memset(x_keydata2,0,sizeof(x_keydata2));
	memset(m_keydata1,0,sizeof(m_keydata1));
	memset(m_keydata2,0,sizeof(m_keydata2));
	
	s_len=0;
	memcpy(m_shopidlen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_shopid,msg_databuff+s_len,atoi(m_shopidlen));s_len=s_len+atoi(m_shopidlen);
	memcpy(m_termidlen,msg_databuff+s_len,2);s_len=s_len+2;
	memcpy(m_termid,msg_databuff+s_len,atoi(m_termidlen));s_len=s_len+atoi(m_termidlen);
	memcpy(m_mac,msg_databuff+s_len,16);s_len=s_len+16;
	
	if(0>VerifyPosMac(x_workkey , srcBuf+4 , iLen-4-16 , m_mac))
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"02",m_shopid,m_termid,msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"MAC�������,�Ƿ�����!");
		ErrResponseForPOS(srcBuf,"02",iConnSock,iLen,errmsg,strlen(errmsg));
		return -1;
	}
	
	
	if(0>getshopkey(msg_organcode,m_shopid,m_termid,x_keydata1,x_keydata2))
	{
		insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"03",m_shopid,m_termid,msg_organcode);
		memset(errmsg,0,sizeof(errmsg));
		sprintf(errmsg,"��Ч���̻�");
		ErrResponseForPOS(srcBuf,"03",iConnSock,iLen,errmsg,strlen(errmsg));
		
		return -1;
	}

	memset(x_tmpkey,0,sizeof(x_tmpkey));memset(x_tmpkey8,0,sizeof(x_tmpkey8));
	memset(m_tmpkey,0,sizeof(m_tmpkey));memset(m_tmpkey8,0,sizeof(m_tmpkey8));
	memcpy(x_tmpkey,x_keydata1,16);
	asc_to_bcd((unsigned char *)x_tmpkey8,(unsigned char *)x_tmpkey,16,0);
	DES(x_workkey8,x_tmpkey8,m_tmpkey8);//���ܵõ���Կ����ǰ�벿������BCD��
	bcd_to_asc((unsigned char *)m_tmpkey,(unsigned char *)m_tmpkey8,16,0);	
	memcpy(m_keydata1,m_tmpkey,16);
	
	memset(x_tmpkey,0,sizeof(x_tmpkey));memset(x_tmpkey8,0,sizeof(x_tmpkey8));
	memset(m_tmpkey,0,sizeof(m_tmpkey));memset(m_tmpkey8,0,sizeof(m_tmpkey8));
	memcpy(x_tmpkey,x_keydata1+16,16);
	asc_to_bcd((unsigned char *)x_tmpkey8,(unsigned char *)x_tmpkey,16,0);
	DES(x_workkey8,x_tmpkey8,m_tmpkey8);//���ܵõ���Կ���ݺ�벿������BCD��
	bcd_to_asc((unsigned char *)m_tmpkey,(unsigned char *)m_tmpkey8,16,0);	
	memcpy(m_keydata1+16,m_tmpkey,16);

	memset(x_tmpkey,0,sizeof(x_tmpkey));memset(x_tmpkey8,0,sizeof(x_tmpkey8));
	memset(m_tmpkey,0,sizeof(m_tmpkey));memset(m_tmpkey8,0,sizeof(m_tmpkey8));
	memcpy(x_tmpkey,x_keydata2,16);
	asc_to_bcd((unsigned char *)x_tmpkey8,(unsigned char *)x_tmpkey,16,0);
	DES(x_workkey8,x_tmpkey8,m_tmpkey8);//���ܵõ���Կ����ǰ�벿������BCD��
	bcd_to_asc((unsigned char *)m_tmpkey,(unsigned char *)m_tmpkey8,16,0);	
	memcpy(m_keydata2,m_tmpkey,16);
	
	memset(x_tmpkey,0,sizeof(x_tmpkey));memset(x_tmpkey8,0,sizeof(x_tmpkey8));
	memset(m_tmpkey,0,sizeof(m_tmpkey));memset(m_tmpkey8,0,sizeof(m_tmpkey8));
	memcpy(x_tmpkey,x_keydata2+16,16);
	asc_to_bcd((unsigned char *)x_tmpkey8,(unsigned char *)x_tmpkey,16,0);
	DES(x_workkey8,x_tmpkey8,m_tmpkey8);//���ܵõ���Կ���ݺ�벿������BCD��
	bcd_to_asc((unsigned char *)m_tmpkey,(unsigned char *)m_tmpkey8,16,0);	
	memcpy(m_keydata2+16,m_tmpkey,16);

	dcs_debug(0,0,"<bddown_proc>\nkeydata1:[%s],keydata2:[%s]\nKEY:[%s]\n->DES����\nkeydata1:[%s],keydata2:[%s]",x_keydata1,x_keydata2,x_workkey,m_keydata1,m_keydata2);	
	
	//��֯���ر���
	
	struct  tm *tm;   time_t  t;
	char msg_ret[2+1];
	char msg_keydata1[32+1];
	char msg_keydata2[32+1];
	char msg_mac[16+1];
	
	memset(msg_info,0,sizeof(msg_info));
	memset(msg_bmplen,0,sizeof(msg_bmplen));
	memset(msg_bmp,0,sizeof(msg_bmp));
	memset(msg_ret,0,sizeof(msg_ret));
	memset(msg_keydata1,0,sizeof(msg_keydata1));
	memset(msg_keydata2,0,sizeof(msg_keydata2));
	memset(msg_mac,0,sizeof(msg_mac));
	
	memcpy(msg_info,"2001",4);
	memcpy(msg_bmplen,"04",2);
	memcpy(msg_bmp,"1111",4);
	memcpy(msg_ret,"00",2);
	memcpy(msg_keydata1,m_keydata1,32);
	memcpy(msg_keydata2,m_keydata2,32);

	time(&t);
  tm = localtime(&t);
	sprintf(msg_date,"%02d%02d",tm->tm_mon+1,tm->tm_mday);
	sprintf(msg_time,"%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	
	s_len=0;
	s_len=s_len+4;
	
	//��Ϣͷ
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
	
	//��Ϣ��
	memcpy(srcBuf+s_len,msg_ret,2);s_len=s_len+2;
	memcpy(srcBuf+s_len,msg_keydata1,32);s_len=s_len+32;
	memcpy(srcBuf+s_len,msg_keydata2,32);s_len=s_len+32;
	memcpy(srcBuf+s_len,msg_mac,16);s_len=s_len+16;
	
	char m_len[4+1];
	memset(m_len,0,sizeof(m_len));
	sprintf(m_len,"%04d",s_len);
	memcpy(srcBuf,m_len,4);
	
	GetPosMac(x_workkey , srcBuf+4 , s_len-4-16 , msg_mac);
	memcpy(srcBuf+s_len-16,msg_mac,16);
	
  dcs_debug(srcBuf,s_len,"<bddown_proc> ���ر��� iConnSock =%d, len=%d",iConnSock,s_len);
	write_msg_to_AMP(iConnSock, srcBuf, s_len,60000); 
	
	insmsglog(msg_info,msg_handle,msg_date,msg_time,msg_seq,msg_statu,msg_opid,"00",m_shopid,m_termid,msg_organcode);
	
	return 1;	
}
