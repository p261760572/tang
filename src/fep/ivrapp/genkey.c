/****************************************************************************/
/*                                                                          */
/*   PRPGRAM NAME : GENKEY.C                                                */
/*   AUTHOR       :                                                         */ 
/*   VERSION      : 2.0                                                     */
/*   INITIAL DATE : 2010-11-22                                              */
/*   DESCRIPTION  : This program is a key management system under unix      */
/*                  system.                                                 */
/*                                                                          */
/****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

static int OpenLog(char *IDENT);

static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/appsvr1.log"
    if ( u_fabricatefile("log/ivrappsvr.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

/*---------------------------------------------------*/
/*  main() : Main function of key management system. */
/*---------------------------------------------------*/
main(int argc, char *argv[])
{
   int          	i, iLen, rc, iReturn = 0;
   unsigned char	caMid[15+1];
   unsigned char	caIP[15+1];
   unsigned char	caData[4096+1];
   unsigned char	caData1[4096+1];

    if ( 0 > OpenLog(argv[0]) )
    {
        BC_SendExeception("04","<FILE:%s,LINE:%d>打开日志失败，appSrv进程启动失败，退出");
        exit(1);
    }

    if ( DasConectDB()!=0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>Can not open oracle DB !",__FILE__,__LINE__);
        BC_SendExeception("04","连接oracle DB失败，appSrv进程启动失败，退出");
        return 0;
    }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL 请求发送给 IVR OP01 */
   strcpy( caData, "CMDI0005OP0101234567811010819750127541000123456789012345000<~~INFO~~>订购了机票一张<~~/~~><~~TEL~~>13501389223<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   /* IVR 回应 APPL请求应答  OP01 */
   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   strcpy( caData, "2112345678OP011101081975012754100012345678901234500000220006N00000");
   iLen = strlen(caData);
   iReturn = BC_IvrConvToAppl( caData,  iLen, caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_IvrConvToAppl fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   /* IVR 发请求给 APPL OP03 */
   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   strcpy( caData, "1012345678OP01110108197501275410001234567890123450000042021015N00000");
   iLen = strlen(caData);
   memcpy( caData+iLen, "12345678", 8 );
   iLen =iLen+8;
   iReturn = BC_IvrConvToAppl( caData,  iLen, caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_IvrConvToAppl fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL回应IVR OP03 */
   strcpy( caData, "CMDI0005OP0311234567811010819750127541000123456789012345000<~~RSPCODE~~>N00000<~~/~~><~~RSPMSG~~>交易成功<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL回应IVR OP03 */
   strcpy( caData, "CMDI0005OP0311234567811010819750127541000123456789012345000<~~RSPCODE~~>N00055<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL回应IVR OP03 */
   strcpy( caData, "CMDI0005OP0311234567811010819750127541000123456789012345000<~~RSPCODE~~>N99999<~~/~~><~~RSPMSG~~>交易失败<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL 请求发送给 IVR OP02 */
   strcpy( caData, "CMDI0005OP0201234567811010819750127541000123456789012345000<~~INFO~~>订购了机票一张<~~/~~><~~TEL~~>13501389223<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   /* IVR 回应 APPL请求应答  OP02 */
   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   strcpy( caData, "2112345678OP021101081975012754100012345678901234500000220006N00000");
   iLen = strlen(caData);
   iReturn = BC_IvrConvToAppl( caData,  iLen, caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_IvrConvToAppl fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   /* IVR 发请求给 APPL OP04 */
   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   strcpy( caData, "1012345678OP021101081975012754100012345678901234500000820222324015N00000");
   iLen = strlen(caData);
   memset(caData+iLen, 0x18, 1);
   iLen = iLen + 1;
   memcpy(caData+iLen, "110108197501275410", 18);
   iLen = iLen + 18;
   memcpy(caData+iLen, "1211356", 7);
   iLen = iLen + 7;
   iReturn = BC_IvrConvToAppl( caData,  iLen, caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_IvrConvToAppl fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL回应IVR OP04 */
   strcpy( caData, "CMDI0005OP0411234567811010819750127541000123456789012345000<~~RSPCODE~~>N00000<~~/~~><~~RSPMSG~~>交易成功<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL回应IVR OP04 */
   strcpy( caData, "CMDI0005OP0411234567811010819750127541000123456789012345000<~~RSPCODE~~>N30301<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL回应IVR OP04 */
   strcpy( caData, "CMDI0005OP0411234567811010819750127541000123456789012345000<~~RSPCODE~~>N99999<~~/~~><~~RSPMSG~~>交易失败<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL回应IVR OP04 */
   strcpy( caData, "CMDI0005OP0411234567811010819750127541000123456789012345000<~~RSPCODE~~>N00065<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

   memset( caData, 0, sizeof(caData));
   memset( caData1, 0, sizeof(caData1));
   /* APPL回应IVR OP04 */
   strcpy( caData, "CMDI0005OP0411234567811010819750127541000123456789012345000<~~RSPCODE~~>N00053<~~/~~>" );
   iReturn = BC_ApplConvToIvr( caData,  strlen(caData), caData1, &i);
   dcs_log(caData1, i,"<FILE:%s,LINE:%d>output data !",__FILE__,__LINE__);
   if (iReturn)
   {
		printf( " BC_ApplConvToIvr fail %d\n", iReturn);
   }
   else
   {
		printf( "data %s\n", caData1);
   }

    DasEnd( 1 );
}
