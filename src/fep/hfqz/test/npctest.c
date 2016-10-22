#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "netpayclient.h"

void SignAndVeriSign() ;
	char	MerId[ 7 ]  , MerKeyFile[ 200 ]  , PgKeyFile[ 200 ]  ;
	char 	MsgData[ 1024 ] , OrdId[ 17 ] , TransAmt[ 13 ] ;
	char	ChkValue[ 257 ] , MerDate[ 9 ] , SysDate[ 9 ] ;

void testEnc( ) ;

int main (int argc, char* argv[])
{

	testEnc( ) ;
}


void
testEnc( )
{
	char	buf[ 1024 ] , buf1[ 1024 ] ;
	int	ret , len ;

	strcpy( buf , "10RegistMerchant870015ha74B8E0F4FDE1E3F712DD579AC1347CF7lp我是小旋风11@163.com15829220258158292202581+1C81E728D9D4C2F636F067F89CC14862CP0101M" ) ;

	ret = EncMsg( "MerPubk999990.key" , buf , strlen( buf ) , buf1 ) ;
	//ret = EncMsg( "PgPubk.key" , buf , strlen( buf ) , buf1 ) ;
	printf ("EncMsg[%d][%s]\n" , ret , buf1 ) ;

	memset( buf , 0 , sizeof( buf ) ) ;

	ret = DecMsg( "999990" , "MerPrK999990.key" , buf1 , buf , & len ) ;

	buf[ len ] = '\0' ;

	printf( "DecMsg[%d][%s]\n" , ret , buf ) ;

	ret = SignMsg( "999990" , "MerPrK999990.key" , buf , strlen(buf),buf1) ;
	//ret = EncMsg( "PgPubk.key" , buf , strlen( buf ) , buf1 ) ;
	printf ("ChkValue[%d][%s]\n" , ret , buf1 ) ;


	ret = VeriSignMsg( "MerPubk999990.key" , buf , strlen( buf ) , buf1 ) ;

	printf( "verifysign[%d]\n" , ret ) ;

}

void testSignFile( ) 
{
	int	ret ;
	//SignAndVeriSign();

	SignMsg( "880001", "MerPrK880001.key" , "0000000000" , 10, ChkValue ) ;

	ChkValue[ 256 ] = '\0' ;
	//printf( "ChkValue[%s]\n" , ChkValue ) ;

	//ret = SignFile( "880001" , "MerPrK880001.key" , "880001.txt" ) ;
	ret = SignFile( "880001" , "MerPrK880001.key" , "t.txt" ) ;

	printf( "ret=[%d]\n" , ret ) ;

	return 0;
}

void SignAndVeriSign()
{
	char	SysSeqId[ 7 ] , TransType[ 2 ] , TransStat[ 2 ] ;
	char	GateId[ 3 ] , buf[ 1024 ] ;
	char	MerPriv[ 121 ] , BgRetUrl[ 121 ] , PageRetUrl[ 121 ] ;
	
	int ret = 0;

	strcpy( MerId , "880001" ) ;
	strcpy( MerKeyFile , "/app/netpay/src/npc/test/MerPrK880001.key" ) ;
	strcpy( PgKeyFile , "PgPubk.key" ) ;

	strcpy( MerDate , "20060620" ) ;
	strcpy( OrdId , "000861" ) ;
	strcpy( TransType, "P" ) ;
	strcpy( TransAmt , "0.01" ) ;

	strcpy( GateId , "08" ) ;
	strcpy( MerPriv , "This is a Test Private Data used by Merchant" ) ;
	strcpy( BgRetUrl , "http://payment.chinapnr.com:8081/pay/BgRetUrl-test.jsp" ) ;
	strcpy( PageRetUrl , "http://payment.chinapnr.com:8081/pay/PageRetUrl-test.jsp" ) ;

	ChkValue[ 256 ] = '\0' ;
	ret = SignOrder( MerId,  MerKeyFile,  OrdId,  TransAmt,  MerDate,  TransType,  GateId , MerPriv , BgRetUrl , PageRetUrl , ChkValue) ;
	
	printf("result[%d] ChkValue[%s]\n", ret, ChkValue);
	
	strcpy( ChkValue , "4B4016AEC374ADAB6A04674B0049EDF5A2E9C5D6DD5DACECB5B27A423ABC3A358526480A899CE86C1EED0CF2E13C884CADC097B31A08D405AFE9378FBA75DD0B568C7BA0015346CE21C9DDBC85946BAC89B797BBB842F262004948F0B5D090C2809DBDD93D4E28EFB92FA904CF5782506D27D17DAFE39D1F20F761613C161996" ) ;
	strcpy( MerId , "880001" ) ;
	strcpy( OrdId , "000822666" ) ;
	strcpy( TransAmt , "0.01" ) ;
	strcpy( MerDate , "20060619" ) ;
	strcpy( TransType , "P" ) ;
	strcpy( TransStat , "S" ) ;
	strcpy( SysDate , "20060619" ) ;
	strcpy( SysSeqId , "000047" ) ;
	strcpy( GateId , "05" ) ;
	
	ret = VeriSignOrder(MerId, PgKeyFile, OrdId, TransAmt, MerDate, TransType, TransStat, GateId, MerPriv , SysDate, SysSeqId, ChkValue);
	sprintf( buf , "veri_sign ret=[%d]chk[%d]\n" , ret , strlen( ChkValue )  ) ;
	printf( buf ) ;

}
