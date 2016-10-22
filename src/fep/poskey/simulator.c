#include "ibdcs.h"
#include "iso8583.h"
struct ISO_8583 iso8583_ZFYconf[128];/** 上海卡友支付易接口 ***/
static long StrToHex( char * str );
static void appSvr1Exit(int signo);
extern struct ISO_8583 *iso8583;
int main(int argc, char *argv[])
{
 
   if ( argc <2)
   		return -1;
    catch_all_signals(appSvr1Exit);
    
  
    //prepare the logging stuff
    if(0 > OpenLog(argv[0]))
        exit(1);

    dcs_log(0,0, "Simulator is starting up...\n");
    //attach to SHM of IBDCS
    if(dcs_connect_shm() < 0)
    {
        dcs_log(0,0,"dcs_connect_shm() failed:%s\n",strerror(errno));
        exit(1);
    }
    dcs_debug(0,0,"conect shm succ!");

    //attach to folder system and create folder of myself
    

   
    dcs_log(0,0,"*************************************************\n"
                "!!        Simulator startup completed.        !!\n"
                "*************************************************\n");
    
    //do my main logic in a infinite loop   
   
    DoLoop( argv[1]);

    appSvr1Exit(0);
}//main()

static void appSvr1Exit(int signo)
{
    char  return_code[3];

    if(signo > 0)
        dcs_log(0,0,"catch a signal %d\n",signo);
    if(signo !=0 && signo != SIGTERM && signo != SIGSEGV)
        return;
    
    dcs_log(0,0,"Simulator terminated.\n");
    exit(signo);
}

static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/appsvr1.log"
    if(u_fabricatefile("log/simulator.log",logfile,sizeof(logfile)) < 0)
        return -1;

    return dcs_log_open(logfile, IDENT);
}
static int DoLoop( char * id)
{
    //
    // read message from my folder and handle it. After msg from switch
    // received by TCP communication  process dcscom, dcscom will put
    // the msg into my folder
    //
    
    char   buffer[1024],buffer1[1024],sysdate[20];
    int    len,sock,s;
    unsigned char ss[9]="\x00\x00\x01\x20\x21\x00\x10\x21"; 
    unsigned char tt[9]="\x06\x12\x34\x56\xff\xff\xff\xff"; 
    unsigned char key[17],pin[17]; 
   ISO_data iso;
  struct  tm *tm;   time_t  t;   
	int i;
         time(&t);
    tm = localtime(&t);

    sprintf(sysdate,"%02d%02d%02d",
                                tm->tm_hour,
            tm->tm_min,
            tm->tm_sec);

   memset(&iso,0,sizeof(iso));
   memset(buffer,0,sizeof(buffer));
   if(IsoLoad8583config(&iso8583_ZFYconf[0],"iso8583_ZFY.conf") < 0)
    {

        dcs_log(0,0," Load sh cardinfo zfy interface iso8583config() failed:%s\n",strerror(errno));
        return 0;
    }

   iso.iso8583 = &iso8583_ZFYconf[0];
    SetIsoFieldLengthFlag ( &iso,1 );
    SetIsoHeardFlag(&iso, 1 );
    SetIsoBitmapFlag(&iso,1); /** 设置bitmap为不压缩方式 **/
   setbit(&iso,0,(unsigned char *)"0200",4);
   if ( memcmp(id,"xf",2)==0)
   			setbit(&iso,3,(unsigned char *)"000000",6);
   else if ( memcmp(id,"cx",2)==0)
   			setbit(&iso,3,(unsigned char *)"300000",6);
   else if ( memcmp( id,"fx",2)==0)
   	    setbit(&iso,3,(unsigned char *)"9F0000",6);
   else if ( memcmp( id,"yd",2)==0)
   {
   		 setbit(&iso,3,(unsigned char *)"910000",6);
   		 setbit(&iso,18,(unsigned char *)"1100",4);
   		 setbit(&iso,20,(unsigned char *)"13507311111",11);
   }
   else if ( memcmp( id,"qb",2)==0)
   {
   		 setbit(&iso,3,(unsigned char *)"910000",6);
   		 setbit(&iso,18,(unsigned char *)"2000",4);
   		 setbit(&iso,20,(unsigned char *)"Y811111111",10);
   		 
   }
   if ( memcmp(id,"xf",2) ==0 || memcmp(id,"yd",2) ==0 ||memcmp(id,"qb",2) ==0)
   		setbit(&iso,4,(unsigned char *)"000000000001",12);
   setbit(&iso,11,(unsigned char *)sysdate,6);
   setbit(&iso,7,(unsigned char *)"0212092034",10);
   
   setbit(&iso,21,(unsigned char *)"567890AB31000000021   14415231    177166900010",46);
   setbit(&iso,22,(unsigned char *)"012",3);
   setbit(&iso,25,(unsigned char *)"00",2);
   setbit(&iso,26,(unsigned char *)"06",2);
 //  setbit(&iso,35,(unsigned char *)"9843010150000249=00002018197250000000",37);
  // setbit(&iso,36,(unsigned char *)"999843010150000249=15615600000000000008800000002404040000000000000000000=000000000000=000000000000000000",104);
   if ( memcmp(id,"fx",2)!=0 )
   {
	   setbit(&iso,35,(unsigned char *)"6225880120210010217=00002208070000000",37);
	   setbit(&iso,36,(unsigned char *)"996225880120210010217=156156000000000000000000000021414140000==000000000000=000000000000=039827600000000",104);
   }
   setbit(&iso,41,(unsigned char *)"10000001",8);
   setbit(&iso,42,(unsigned char *)"001584011111112",15);
   setbit(&iso,49,(unsigned char *)"156",3);
   setbit(&iso,53,(unsigned char *)"1000000000000000",16);
 //  setbit(&iso,64,(unsigned char *)"12345678",8);
   if ( memcmp(id,"fx",2)!=0 )
   {
   dcs_debug(ss,8,"ss data");
   dcs_debug(tt,8,"tt data");
   
   for (i=0 ;i< 8;i++)
   {
   	     
   	      ss[i]=ss[i] ^ tt[i];
   	      printf( "\n ss[%d]=%02x\n",i,ss[i]);
   	}
   dcs_debug(tt,8,"pin block");
   memset(key,0,sizeof(key));
   dcs_debug(key,8,"PIN Key data");
   memset(buffer1,0,sizeof(buffer1));
   bcd_to_asc((unsigned char *)buffer1,(unsigned char *)pin,16,0);
   dcs_debug(pin,8,"buffer=%s",buffer1);
   setbit(&iso,52,(unsigned char *)buffer1,16);
   dcs_debug(buffer1,8,"pin block data");
   setbit(&iso,53,(unsigned char *)"2000000000000000",16);
  }
   len = isotostr((unsigned char *)(buffer),&iso);
//	CalcMac( 0,buffer+6, len- 8 ,"1111111111111111", );
   sock = tcp_connet_server("127.0.0.1", 6804,0);
   if ( sock <0)
   {
   		dcs_log(0,0,"connect server error");
   		return -1;
   }
   
   dcs_log(buffer,len,"send data len=%d",len);
   dcs_debug_disp(0,buffer,len,"send data len=%d",len);
   s=write_msg_to_net(sock,buffer, len,50);
   if ( s <=0)
       dcs_log(buffer,len+8,"send error");
   else
      dcs_debug(buffer,len+8,"send succ");
   memset(buffer,0,sizeof(buffer));
   s=read_msg_from_net(sock,buffer,sizeof(buffer),20000);
   close(sock);
   if ( s >0)
      dcs_log(buffer,s,"read msg");
   else
   {
	   dcs_log(0,0,"recv fail");
           return -1;
	 }
   memset(&iso,0,sizeof(iso));
   iso.iso8583 = &iso8583_ZFYconf[0];
   SetIsoFieldLengthFlag ( &iso,1 );
   SetIsoHeardFlag(&iso, 1 );
   SetIsoBitmapFlag(&iso,1); /** 设置bitmap为不压缩方式 **/
   strtoiso((unsigned char *)buffer,&iso,s);
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

int CalcMac( int flag,unsigned char *Data , int len ,char* RemoteMacKey, char *Mac)
{
	int i,j;
	unsigned char  tmpbuf[8],mBuf[20],caBuf[20],caBuf1[20],caBuf2[20],MastKey[9];
	
	memset(tmpbuf,0,sizeof(tmpbuf));
	
//	dcs_debug(0,0,"calc mac entry");
  if ( flag )
		memcpy(MastKey,"\x10\x10\x10\x10\x10\x10\x10\x10",8);
//		memcpy(MastKey,"\x5b\x01\x73\x25\x20\xab\x1A\x7A",8); //远程主密钥
	else
	  memcpy(MastKey,"111111111",8);
	
	dcs_debug(MastKey,8,"Maskey data");
	if ( len <=0 || RemoteMacKey==NULL ||Mac == NULL)
		return -1;
	if ( len %8 )
	{
		memset(Data+len,0,8-len%8);
		len = len +(8-len%8);
  }
  dcs_debug(Data,len,"calc mac data len =%d",len);
  for ( i = 0; i<len/8 ;i++)
         for ( j=0;j<8;j++)
           tmpbuf[j]=tmpbuf[j] ^ *(Data+i*8+j);
 
//  dcs_debug(tmpbuf,8,"MAC DATA xoe result");       
  bcd_to_asc((unsigned char *)mBuf,(unsigned char *)tmpbuf,16,0);
//  dcs_debug(mBuf,16,"bcd_to_asc data");
  asc_to_bcd((unsigned char *)caBuf,(unsigned char *)RemoteMacKey,16,0);
//  dcs_debug(caBuf,8,"MAC KEY mdata");
//  dcs_debug(caBuf1,8,"MAC KEY DATA");
//  dcs_debug(tmpbuf,8,"first DES");
  memcpy(mBuf,tmpbuf,8);
  for( i =0;i<8;i++)
         mBuf[i]=mBuf[i] ^ mBuf[i+8];
//  dcs_debug(mBuf,8,"xor data");
//  dcs_debug(tmpbuf,8,"second DES");
  bcd_to_asc((unsigned char *)caBuf2,(unsigned char *)tmpbuf,16,0);
//  dcs_debug(caBuf2,16,"genrate MAC DATA");
  memcpy(Mac,caBuf2,8);

	dcs_debug(0,0,"calc mac finish data:[%s]",Mac);

	return 1;
}

