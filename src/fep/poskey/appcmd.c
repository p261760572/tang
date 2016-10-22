#include "ibdcs.h"
extern struct ISO_8583 *iso8583;
struct ISO_8583 iso8583_POSconf[128];/** 长沙卡友接入的自助终端接口  */
struct ISO_8583 iso8583_ZFYconf[128];/** 上海卡友支付易接口 ***/
struct ISO_8583 iso8583_POSPconf[128];/** 湖南银联POSP接口 **/
struct ISO_8583 iso8583_MPconf[128];/***   湖南银联公共支付平台接口  ***/
struct ISO_8583 iso8583_ATMconf[128];/***  银联ATMP接口  ****/
struct ISO_8583 iso8583_DSFconf[128];/*** 长沙卡友第三方业务接入接口  ****/

static char shopid[16];
static char posid[9];
static long StrToHex( char * str );
static void appSvr1Exit(int signo);
int main(int argc, char *argv[])
{
 char configfile[500];
    catch_all_signals(appSvr1Exit);

    //prepare the logging stuff
    if ( argc <2)
    	return -1;
    if(0 > OpenLog(argv[0]))
        exit(1);
		memset(shopid,0,sizeof(shopid));
		memset(posid,0,sizeof(posid));
    if( argc >=4)
    {
				memcpy(shopid,argv[2],15);
				memcpy(posid,argv[3],8);
    }
    dcs_log(0,0, "App Command is starting up...\n");
    if(fold_initsys() < 0)
    {
        dcs_log(0,0, "cannot attach to folder system!");
        return -1;
    }
    //attach to SHM of IBDCS
    if(dcs_connect_shm() < 0)
    {
        dcs_log(0,0,"dcs_connect_shm() failed:%s\n",strerror(errno));
        exit(1);
    }
    dcs_debug(0,0,"conect shm succ!");
    dcs_debug(0,0,"argv[0]=[%s],argv[1]=[%s],argv[2]=[%s],argv[3]=[%s],argv[4]=[%s],argv[5]=[%s]",
              argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]);
    //attach to folder system and create folder of myself
    if (! DssConect())
    {
    	dcs_log(0,0,"Can not open oracle DB !");
    	appSvr1Exit(0);
    }   
     memset (configfile,0,sizeof(configfile));
    if(u_fabricatefile("config/iso8583_POS.conf",configfile,sizeof(configfile)) < 0)
    {
    	   dcs_log(0,0,"不能找到iso8583_POS.conf文件!");
    	    appSvr1Exit(0);
    }
    //try to load configuration from file
    if(IsoLoad8583config(&iso8583_POSconf[0],"iso8583_POS.conf") < 0)
    {

        dcs_log(0,0," Load cs cardinfo POS interface iso8583config() failed:%s\n",strerror(errno));
        appSvr1Exit(0);
    }
    if(u_fabricatefile("config/iso8583_ZFY.conf",configfile,sizeof(configfile)) < 0)
    {
    	   dcs_log(0,0,"不能找到iso8583_ZFY.conf文件!");
    	   appSvr1Exit(0);
    }
    if(IsoLoad8583config(&iso8583_ZFYconf[0],"iso8583_ZFY.conf") < 0)
    {

        dcs_log(0,0," Load sh cardinfo zfy interface iso8583config() failed:%s\n",strerror(errno));
        appSvr1Exit(0);
    }
    if(u_fabricatefile("config/iso8583_POSP.conf",configfile,sizeof(configfile)) < 0)
    {
    	   dcs_log(0,0,"不能找到iso8583_POSP.conf文件!");
    	   appSvr1Exit(0);
    }
    if(IsoLoad8583config(&iso8583_POSPconf[0],"iso8583_POSP.conf") < 0)
    {

        dcs_log(0,0," Load hunan unionpay POSP interface iso8583config() failed:%s\n",strerror(errno));
        appSvr1Exit(0);
    }
    if(u_fabricatefile("config/iso8583_MP.conf",configfile,sizeof(configfile)) < 0)
    {
    	   dcs_log(0,0,"不能找到iso8583_MP.conf文件!");
    	   appSvr1Exit(0);
    }
    if(IsoLoad8583config(&iso8583_MPconf[0],"iso8583_MP.conf") < 0)
    {

        dcs_log(0,0," Load hunan unionpay move pay system interface iso8583config() failed:%s\n",strerror(errno));
        appSvr1Exit(0);
    }
    if(u_fabricatefile("config/iso8583_DSF.conf",configfile,sizeof(configfile)) < 0)
    {
    	   dcs_log(0,0,"不能找到iso8583_DSF.conf文件!");
    	   appSvr1Exit(0);
    }
    if(IsoLoad8583config(&iso8583_DSFconf[0],"iso8583_DSF.conf") < 0)
    {

        dcs_log(0,0," Load cs cardinfo POS interface iso8583config() failed:%s\n",strerror(errno));
        appSvr1Exit(0);
    }
    dcs_log(0,0,"*************************************************\n"
                "!!        AppCmd startup completed.        !!\n"
                "*************************************************\n");
    Service(argv[1],argv[4],argv[5]);
    appSvr1Exit(0);
}//main()

static void appSvr1Exit(int signo)
{
    char  return_code[3];

    if(signo > 0)
        dcs_log(0,0,"catch a signal %d\n",signo);
    if(signo !=0 && signo != SIGTERM && signo != SIGSEGV)
        return;
     DssEnd( 0);
    dcs_log(0,0,"AppCmd terminated.\n");
    exit(signo);
}

static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/appsvr1.log"
    if(u_fabricatefile("log/appcmd.log",logfile,sizeof(logfile)) < 0)
        return -1;

    return dcs_log_open(logfile, IDENT);
}
static int Service( char * ID ,char *type,char *commname)
{
    //
    // read message from my folder and handle it. After msg from switch
    // received by TCP communication  process dcscom, dcscom will put
    // the msg into my folder
    //
    
    char   buffer[1024];
    int    len,sock,s,gs_fid,n;
   
   ISO_data iso;
   struct IBDCSPacket *pktptr;
   struct  tm *tm;   time_t  t;
   memset(&iso,0,sizeof(iso));
   memset(buffer,0,sizeof(buffer));
   
   if ( memcmp( ID,"login",5) ==0 && strcmp(type,"pbsp")==0)
   {
   	iso.iso8583=&iso8583_MPconf[0];    
		SetIsoHeardFlag(&iso,1);   /** 设置报文类型内容为不压缩方式 **/
		SetIsoFieldLengthFlag(&iso,1); /** 设置可变长报文域长度为不压缩方式 **/
   		setbit(&iso,0,(unsigned char *)"0620",4);
   		setbit(&iso,3,(unsigned char *)"150000",6);
   		setbit(&iso,11,(unsigned char *)"000001",6);
   		setbit(&iso,33,(unsigned char *)"00995500",8);
   		setbit(&iso,41,(unsigned char *)posid,8);
   		setbit(&iso,42,(unsigned char *)shopid,15);
   	
   }
   else  if ( memcmp( ID,"login",5) ==0 && strcmp(type,"posp")==0)
   {
	   	iso.iso8583=&iso8583_POSPconf[0];    
			SetIsoHeardFlag(&iso,0);   /** 设置报文类型内容为压缩方式 **/
			SetIsoFieldLengthFlag(&iso,0); /** 设置可变长报文域长度为压缩方式 **/
   		setbit(&iso,0,(unsigned char *)"0303",4);
   		setbit(&iso,11,(unsigned char *)"000001",6);
   		setbit(&iso,41,(unsigned char *)posid,8);
   		setbit(&iso,42,(unsigned char *)shopid,15);
   		setbit(&iso,48,(unsigned char *)"1",1);
   		setbit(&iso,60,(unsigned char *)"1",1);
   	
   }
   else if ( memcmp( ID,"settle",5) ==0)
   {
   	
   }
   else
   		return -1;
		time(&t);
  tm = localtime(&t);
	sprintf(buffer,"%02d%02d%02d%02d",tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);

   if (strcmp(type,"pbsp")==0)
   {
   	len = isotostr((unsigned char *)(buffer+8),&iso);
  }
  else
  	{
    len = isotostr((unsigned char *)(buffer+5+8),&iso);
   memcpy(buffer+8,"\x60\x00\x51\x00\x00",5);
  }
   gs_fid = fold_locate_folder(commname);
   if(gs_fid < 0)        
   {
        dcs_log(0,0,"cannot locate folder '%s':%s\n", APPSRV_FOLD_NAME, ise_strerror(errno) );
        return -1;
   } 
   if (strcmp(type,"pbsp")==0)
     dcs_log(buffer,len+8,"send login packet ");
   else
   	dcs_log(buffer,len+5+8,"send login packet ");
   if (strcmp(type,"pbsp")==0)
     s=fold_write(gs_fid,-1,buffer,len+8);
   else
   	s=fold_write(gs_fid,-1,buffer,len+8+5);
   if ( s>0)
   		dcs_log(0,0,"Send Msg Succ!");
   else
   		dcs_log(0,0,"Send Msg Fail!");
   return -1;
}

static long cf( int i, int s)
{
	long value=1;
	
	
	if ( s==0)
		return 1;
	for (value=1;s>0;s--)
		value=value*i;
	return value;
		
}
static long StrToHex( char * str )
{
	long value;
	int s;
	
	 value=0;
	 s=strlen(str);
	 if ( s <=0) return -1;
	 for( ;s >0;s--,str++)
	 {
	 	 if ( (*str)<0x30 || (*str)>0x39)
	 	 	   return -1;
	 	value=value+((*str)&0x0f)*cf(16,s-1);
	 }
	 return value;
}
