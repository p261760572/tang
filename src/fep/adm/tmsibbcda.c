#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "tmcibtms.h"
#include "dccglobl.h"
#include <folder.h>
// 程序名称：tmsbcda.c
// 函数清单：
//          1.CreateBcda()
//          2.RemoveBcda()
//          3.ConnectBcda()
//          4.DumpBcda()
//          5.LocateBankByName
//          6.LocateBankByCode
//          7.UnlockBcda()
//          8.LockBcda()
//          9.AddBcda
//          10.UpdateBcda
//          11.DelBcda


//#define     min(a,b)   (((a) > (b)) ? (b) : (a))

////////////////////////////////////////////////////////////////

// 函数名称：CreateBcda()
// 参数说明：无
// 返 回 值：
// 功能描述：

int CreateBcda()
{
  int    iBankCnt, iFd, iRc, iShmSize,iShmId, i;
  struct BCDAst *pBCDAst = NULL;
  char   caBuffer[512],caFileName[256];
  char   *pcBankName, *pcBankCode, *psSecurity;
  char   *pcType,*pcOpenLog;


  if(shm_connect(BCDA_SHM_NAME,NULL) != (long)NULL)
  {
     dcs_log(0,0, "<CreateBcda> share memory 'BCDA' exists already." );
  	return (0);
  }

  dcs_log(0,0, "<CreateBcda> Begin to create 'BCDA' shared memory...");

  //fabricate the configure file's name and open it
  memset(caFileName, 0, sizeof(caFileName) );
  sprintf(caFileName, "%s/config/%s",
          (char *)getenv( "ICS_HOME" ),
          ISDBANK );
  if((iFd = conf_open(caFileName)) < 0)
  {
    dcs_log( 0,0, "<CreatBcda> open  '%s' failed!", caFileName );
    return ( -1 );
  }

  //figure out how many banks configured
  iBankCnt = 0;
/*  for( iRc = conf_get_first_string(iFd, "bank.entry",caBuffer);
       iRc == 0;
       iRc = conf_get_next_string(iFd, "bank.entry",caBuffer))
  {
  	iBankCnt ++;
  }
*/
  conf_get_first_number(iFd,"bank.max",&iBankCnt); 
  if(iBankCnt <= 0)
  {
    dcs_log( 0,0, "<CreatBcda> no banks configured in '%s'!", caFileName );
    goto lblFailure;
  }

  //next try to create shared memory 'BCDA'
  iShmSize = (iBankCnt - 1) *  sizeof(struct BCDA) + sizeof(struct BCDAst);
  pBCDAst  = (struct BCDAst *)shm_create(BCDA_SHM_NAME,iShmSize,&iShmId);
  if(pBCDAst == NULL)
  {
    dcs_log(0,0, "<CreatBcda> Create shared memory '%s' failed:%s!",
                           BCDA_SHM_NAME,strerror(errno));
    goto lblFailure;
  }

  //save the address into global
  g_pcBcdaShmPtr = (char *)pBCDAst;

  memset(pBCDAst,0,iShmSize);
  pBCDAst->iBankNum = iBankCnt;

  //create semaphore associated with shared memory
 // pBCDAst->iSemId = sem_create(BCDA_SHM_NAME, iBankCnt);
  pBCDAst->iSemId = sem_create(BCDA_SHM_NAME, 1);
  if (pBCDAst->iSemId < 0)
  {
    dcs_log( 0,0, "<CreatBcda> create semaphore '%s' failed:%s!",
                           BCDA_SHM_NAME,strerror(errno));
    goto lblFailure;
  }

  //read each bank's info
  dcs_debug(0,0, "<CreatBcda> begin to load configuration of [%d] banks",
                     iBankCnt);

  for(i=0, iRc = conf_get_first_string(iFd, "bank.entry",caBuffer);
       iRc == 0 && i < iBankCnt;
       iRc = conf_get_next_string(iFd, "bank.entry",caBuffer))
  {
    pcBankName     = strtok(caBuffer," \t\r");
    pcBankCode     = strtok(NULL    ," \t\r\n");
    psSecurity = strtok(NULL    ," \t\r\n");
    pcType 	   = strtok(NULL    ," \t\r\n");
		pcOpenLog  = strtok(NULL    ," \t\r\n");
    if(pcBankName == NULL || pcBankCode == NULL)
      continue;

    pBCDAst->stBcda[i].iBankSeq = i;
    memcpy(pBCDAst->stBcda[i].caBankCode,pcBankCode,
               min(TMS_BANK_CODE_LEN,strlen(pcBankCode)));
    memcpy(pBCDAst->stBcda[i].caBankName,pcBankName,min(15,strlen(pcBankName)));
    pBCDAst->stBcda[i].cNetFlag =  0;
    //pBCDAst->stBcda[i].cAppFlag =  TMS_APCONN_OFF;
    pBCDAst->stBcda[i].cSafFlag =  TMS_SAFLAG_OFF;
//    pBCDAst->stBcda[i].cDateSwhBgnFlag = TMS_DATESW_OFF;
    pBCDAst->stBcda[i].SendFlag=0;
    pBCDAst->stBcda[i].ReadFlag=0;
    pBCDAst->stBcda[i].cResetKeyFlag   = TMS_RESETKEY_OFF;
    if( (pcType != NULL) && (strlen(pcType) < 8) )
	  strcpy(pBCDAst->stBcda[i].caType,pcType); 
/*
    if(psSecurity == NULL)
      pBCDAst->stBcda[i].cSecurityFlag  = 1; //default on yes
    else
      pBCDAst->stBcda[i].cSecurityFlag  =
            (0 == u_stricmp(psSecurity, "yes")) ? 1 : 0;
*/
	//打开关闭日志标志
    if(pcOpenLog == NULL)
      pBCDAst->stBcda[i].cAppFlag  = 1; //default on yes
    else
      pBCDAst->stBcda[i].cAppFlag  =
            (0 == u_stricmp(pcOpenLog, "yes")) ? 1 : 0;
		pBCDAst->stBcda[i].iUseFlag =1;
    i++;
  }

  dcs_debug(0,0, "<CreateBcda> BCDA share memory creation finished!!" );
  conf_close(iFd);
  return (1);

lblFailure:
  if(pBCDAst)
  {
    sem_delete(pBCDAst->iSemId);
    shm_detach((char *)pBCDAst);
    shm_delete(iShmId);
  }
  if(iFd >= 0)
    conf_close(iFd);
  return (0);
}

////////////////////////////////////////////////////////////////

// 函数名称：RemoveBcda()
// 参数说明：无
// 返 回 值：
// 功能描述：

int RemoveBcda()
{
  int iBankNum;
  int iBcdaShmId;
  int iBcdaSemId;
  struct BCDAst *PBCDAst = NULL;


  PBCDAst = (struct BCDAst *)shm_connect(BCDA_SHM_NAME, &iBcdaShmId);
  if(!PBCDAst)
  {
    dcs_log( 0,0,"<RemoveBcda> connect to share memory 'BCDA' failed!" );
    return ( -1 );
  }

  if ((sem_delete(PBCDAst->iSemId)) < 0)
  {
    dcs_log( 0,0,"<RemoveBcda> Remove semaphore 'BCDA' failed!" );
  }

  if ((shm_delete(iBcdaShmId)) < 0)
  {
    dcs_log( 0,0,"<RemoveBcda> Remove share memory 'BCDA' failed!" );
  }

  dcs_log( 0,0,"<RemoveBcda> Remove share memory 'BCDA' successfully." );

  return (0);
}

////////////////////////////////////////////////////////////////

// 函数名称：ConnectBcda()
// 参数说明：无
// 返 回 值：
// 功能描述：

int ConnectBcda()
{

  //连接共享内存BCDA
//  g_pcBcdaShmPtr = (char *)shm_connect(BCDA_SHM_NAME,&g_iBcdaShmId);
  g_pcBcdaShmPtr = (char *)shm_connect(BCDA_SHM_NAME,NULL);
  if(g_pcBcdaShmPtr == NULL)
  {
    return (-1);
  }

  return (0);
}

////////////////////////////////////////////////////////////////

// 函数名称：LockBcda()
// 参数说明：无
// 返 回 值：
// 功能描述：

int LockBcda()
{
    struct BCDAst *pBCDAst;


    pBCDAst  = (struct BCDAst *)g_pcBcdaShmPtr;
    if(pBCDAst == NULL)
        return (-1);

    if(sem_lock(pBCDAst->iSemId, 0, 1) < 0)
    {
        dcs_log(0,0,
                "<LockBcda> sem_lock() failed:%s!",strerror(errno));
        return (-1);
    }

    return (0);
}

////////////////////////////////////////////////////////////////

// 函数名称：UnlockBcda()
// 参数说明：无
// 返 回 值：
// 功能描述：

int UnlockBcda()
{
    struct BCDAst *pBCDAst;


    pBCDAst  = (struct BCDAst *)g_pcBcdaShmPtr;
    if(pBCDAst == NULL)
        return (-1);

    if(sem_unlock(pBCDAst->iSemId, 0, 1) < 0)
    {
        dcs_log(0,0,
                "<UnlockBcda> sem_unlock() failed:%s!",strerror(errno));
        return (-1);
    }

    return (0);
}

////////////////////////////////////////////////////////////////

// 函数名称：BumpBcda()
// 参数说明：无
// 返 回 值：
// 功能描述：

int DumpBcda()
{
  static struct BCDAst *pBCDAst = NULL;
  int    i, iLinkCnt;
  char   caBankName[16], caBankCode[64];

  if(!pBCDAst)
      pBCDAst = (struct BCDAst *)shm_connect(BCDA_SHM_NAME, NULL);
  if(!pBCDAst)
  {
    fprintf(stdout,"cannot connect to share memory 'BCDA'!\n");
    return 0;
  }

  fprintf(stdout,"      BankName  BankCode  Conns SAF RendLink KyReset SendLink LogOpen\n");
  fprintf(stdout,"----------------------------------------------------------------------\n");
  for(i=0; i <  pBCDAst->iBankNum; i++)
  {
  	if ( pBCDAst->stBcda[i].iUseFlag !=1)
  	     continue;
    memset(caBankName,0,sizeof(caBankName));
    memcpy(caBankName,pBCDAst->stBcda[i].caBankName,
                          sizeof(pBCDAst->stBcda[i].caBankName));
    memset(caBankCode,0,sizeof(caBankCode));
    memcpy(caBankCode,pBCDAst->stBcda[i].caBankCode,
                          sizeof(pBCDAst->stBcda[i].caBankCode));
    iLinkCnt = (int)pBCDAst->stBcda[i].cNetFlag;

    fprintf(stdout,"[%.2d]  %-8s  %-12s %d      %d      %d      %d      %d      %d\n",
                       i,caBankName,caBankCode,iLinkCnt,
                       //(int)pBCDAst->stBcda[i].cAppFlag,
                       (int)pBCDAst->stBcda[i].cSafFlag,
                       (int)pBCDAst->stBcda[i].ReadFlag,
                       (int)pBCDAst->stBcda[i].cResetKeyFlag,
                       (int)pBCDAst->stBcda[i].SendFlag,
                       (int)pBCDAst->stBcda[i].cAppFlag
                       );
   fflush(stdout);
  }//for

  return 0;
}
/*
int UnionGetOutBcda(char *BcdaBuf,int iLenBuf)
{
	static struct BCDAst *pBCDAst = NULL;
	char	caBankName[16];
	int 	i,iDumpCount,iPtr;

	if(!pBCDAst)
		pBCDAst = (struct BCDAst *)shm_connect(BCDA_SHM_NAME, NULL);
	if(!pBCDAst)
	{
		dcs_log(0,0,"File:%s,Line:%d,Share memory connect error",
			__FILE__,__LINE__);
		return -1;
	}

  
	if( pBCDAst->iBankNum*27 > iLenBuf-3 )
	{
		dcs_log(0,0,"File: %s,Line: %d, GetOutBcda Buffer Less,BufferLen = %d, Need Len = %d",__FILE__,__LINE__,iLenBuf,pBCDAst->iBankNum+3);
		iDumpCount = (iLenBuf-3)/27;
	}
	else
		iDumpCount = pBCDAst->iBankNum;
	

	memset(BcdaBuf,' ',iDumpCount*27+3);

	sprintf(BcdaBuf,"%03.3d",iDumpCount);
	iPtr = 3;

	for(i=0; i <  iDumpCount; i++)
	{

		if( strlen(pBCDAst->stBcda[i].caBankCode) > 11)
			return -2;
		sprintf(caBankName,"%02.2d",strlen(pBCDAst->stBcda[i].caBankCode));
		memcpy(BcdaBuf+iPtr,caBankName,2);
		iPtr += 2;
		memcpy(BcdaBuf+iPtr,pBCDAst->stBcda[i].caBankCode,strlen(pBCDAst->stBcda[i].caBankCode));
		iPtr += 11;

		memcpy(BcdaBuf+iPtr,pBCDAst->stBcda[i].caBankName,strlen(pBCDAst->stBcda[i].caBankName));
		iPtr += 4;

		sprintf(caBankName,"%d",pBCDAst->stBcda[i].cNetFlag);
		memcpy(BcdaBuf+iPtr,caBankName,1);
		iPtr ++;

		sprintf(caBankName,"%d",pBCDAst->stBcda[i].cAppFlag);
		memcpy(BcdaBuf+iPtr,caBankName,1);
		iPtr ++;

		sprintf(caBankName,"%d",pBCDAst->stBcda[i].cSafFlag);
		memcpy(BcdaBuf+iPtr,caBankName,1);
		iPtr ++;

		sprintf(caBankName,"%d",pBCDAst->stBcda[i].cResetKeyFlag);
		memcpy(BcdaBuf+iPtr,caBankName,1);
		iPtr ++;

		sprintf(caBankName,"%d",pBCDAst->stBcda[i].cSecurityFlag);
		memcpy(BcdaBuf+iPtr,caBankName,1);
		iPtr ++;

		sprintf(caBankName,"%d",pBCDAst->stBcda[i].cDateSwhBgnFlag);
		memcpy(BcdaBuf+iPtr,caBankName,1);
		iPtr ++;

		memcpy(BcdaBuf+iPtr,pBCDAst->stBcda[i].caType,4);
		iPtr += 4;
	}

	dcs_debug(&BcdaBuf,iDumpCount*27+3,"The Send BCDA:");

	return	iDumpCount*27+3; 
}
*/


////////////////////////////////////////////////////////////////////

int LocateBankByCode(char *pcaBankCode)
{
    int    iBnkNum, i;
    struct BCDA *pBCDA;


    iBnkNum = ((struct BCDAst *)g_pcBcdaShmPtr)->iBankNum;
    pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;

    for(i=0; i < iBnkNum; i++)
    {
        if(memcmp(pcaBankCode , pBCDA[i].caBankCode,TMS_BANK_CODE_LEN) == 0)
            return (i);
    }

    return (-1);
}

////////////////////////////////////////////////////////////////////

int LocateBankByName(char *pcaBankName)
{
    int    iBnkNum, i;
    struct BCDA *pBCDA;


    iBnkNum = ((struct BCDAst *)g_pcBcdaShmPtr)->iBankNum;
    pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;

    for(i=0; i < iBnkNum; i++)
    {
        if(strcmp(pcaBankName, pBCDA[i].caBankName) == 0)
            return (i);
    }

    return (-1);
}

////////////////////////////////////////////////////////////////////

int ModifyBcda(char *pBnkCode, int whichFlg, int newVal)
{
    int pos;
    struct BCDA *pBCDA;

    
    pos = LocateBankByCode(pBnkCode);    
    if(pos < 0)
    {
		dcs_log( 0,0,
			 "<ModifyBcda> LocateBankByCode() for '%s' failed.",pBnkCode);
		return (-1);
    }
 
    pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;
    
    switch(whichFlg)
    {
        case BCDA_NETFLAG:       // 网络连接数目
                pBCDA[pos].cNetFlag = newVal;
                break;
                        
        case BCDA_APPFLAG:       // 应用连接标志
                pBCDA[pos].cAppFlag = newVal;
                break;
        
        case BCDA_SAFFLAG:       // 存储转发标志
                pBCDA[pos].cSafFlag = newVal;
                break;
        
        case BCDA_RESETKEYFLAG:  // 重置密钥标志
                pBCDA[pos].cResetKeyFlag = newVal;
                break;
        
 /*       case BCDA_SECCHECK: // 安全检测标志
                pBCDA[pos].cSecurityFlag = newVal;
                break;
       
        case BCDA_DATESWHBGNFLAG:// 日期切换标志
                pBCDA[pos].cDateSwhBgnFlag = newVal;
                break;        
*/          
        default:
            return (-1);
    }//switch
    
    return (0);    
}

int UnionGetKind(char *pBankCode, char *caKind)
{
	int iLen,iPosition;
	struct BCDA *pBCDA;

	iPosition = LocateBankByCode(pBankCode);    
	if(iPosition < 0)
		return -1;
	pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;
    
	iLen = strlen(pBCDA[iPosition].caType);
	if(iLen > 4)
		iLen = 4;
	memcpy(caKind,pBCDA[iPosition].caType,iLen);
	return iLen;
}

//根据文件夹名，取通道类型，机构编码。返回安全检测标志
/*
int UnionGetTypeByName(char *pFoldName, char *caType,char *caInstitution)
{
	int iLen,iPosition,iFlag;
	struct BCDA *pBCDA;

	iPosition = LocateBankByName(pFoldName);    
	if(iPosition < 0)
		return -1;
	pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;
    
	iLen = strlen(pBCDA[iPosition].caType);
	if(iLen > 4)
		iLen = 4;
	memcpy(caType,pBCDA[iPosition].caType,iLen);
	iLen = strlen(pBCDA[iPosition].caBankCode);
	if(iLen > 11)
		iLen = 11;
	memcpy(caInstitution,pBCDA[iPosition].caBankCode,iLen);
	iFlag = pBCDA[iPosition].cSecurityFlag;
	return iFlag;
}

int UnionGetBcda(char *pBnkCode, int whichFlg)
{
    int pos;
    struct BCDA *pBCDA;
    int Val;

    
    pos = LocateBankByCode(pBnkCode);    
    if(pos < 0)
    {
		dcs_log(0,0,"<ModifyBcda> LocateBankByCode() for '%s' \
			failed.",pBnkCode);
		return (-1);
    }
 
    pBCDA = ((struct BCDAst *)g_pcBcdaShmPtr)->stBcda;
    
    switch(whichFlg)
    {
        case BCDA_NETFLAG:
                Val = pBCDA[pos].cNetFlag;
                break;
                        
        case BCDA_APPFLAG:     
                Val = pBCDA[pos].cAppFlag;
                break;
        
        case BCDA_SAFFLAG:
                Val = pBCDA[pos].cSafFlag ;
                break;
        
        case BCDA_RESETKEYFLAG:
                Val = pBCDA[pos].cResetKeyFlag;
                break;
        
        case BCDA_SECCHECK:
                Val = pBCDA[pos].cSecurityFlag;
                break;
        
        case BCDA_DATESWHBGNFLAG:
                Val = pBCDA[pos].cDateSwhBgnFlag ;
                break;        
         
        default:
            return (-1);
    }//switch
    return (Val);
}
*/
/*
int dumpSSA()
{
    	char tmpBuf[128];
    	if(g_pstSwa == NULL)
    	{
       		ConnectIseShms();
    	}
    	if(g_pstSwa == NULL)
    	{
        	fprintf(stderr,"cannot attach shared memory SWA!\n");
        	return 0;
    	}
    	memset(tmpBuf, 0, sizeof(tmpBuf));
    	memcpy(tmpBuf, g_pstSwa->stSsa.caSettleDate, 8);
    	RPAD(tmpBuf,' ',10);
    	fprintf(stdout, "             清算日期 : %s\n", tmpBuf);
    	fprintf(stdout, "           测试模式否 : %s\n", g_pstSwa->stSsa.iTestMode == 1 ? "yes" : "no ");
    	fprintf(stdout, "   系统使用数据库编号 : %d\n", g_pstSwa->stSsa.iDumpId);
    	fprintf(stdout, "     跨行交易超时时间 : %d\n", g_pstSwa->stSsa.iIBTimeout);
    	fprintf(stdout, "     中心命令超时时间 : %d\n", g_pstSwa->stSsa.iSWTimeout);
    	fprintf(stdout, " 跨行取款资金转出超时 : %d\n", g_pstSwa->stSsa.iIBOutTimeout);
    	fprintf(stdout, " 跨行取款资金转入超时 : %d\n", g_pstSwa->stSsa.iIBInTimeout);
    	fprintf(stdout, "         中心日切标志 : %s\n", g_pstSwa->stSsa.iDaySwitch == 0 ? "日切完毕" : "正在日切" );

}
*/

/*
int SetIse(char *which,char *flag)
{
	int iflg;
	
	if(flag == NULL || which == NULL)
		return -1;
	iflg = atoi(flag);
    	if(g_pstSwa == NULL)
    	{
       		ConnectIseShms();
    	}
    	if(g_pstSwa == NULL)
    	{
        	fprintf(stderr,"cannot attach shared memory SWA!\n");
        	return -1;
    	}
	LockSSA();
	if(u_stricmp(which,"IseDebug")==0)
		g_pstSwa->stSsa.iTestMode = iflg;
	if(u_stricmp(which,"SWTimeout")==0)
		g_pstSwa->stSsa.iSWTimeout = iflg;
	if(u_stricmp(which,"IBTimeout")==0)
		g_pstSwa->stSsa.iIBTimeout = iflg;
	if(u_stricmp(which,"IBInTimeout")==0)
		g_pstSwa->stSsa.iIBInTimeout= iflg;
	if(u_stricmp(which,"IBOutTimeout")==0)
		g_pstSwa->stSsa.iIBOutTimeout= iflg;
	UnlockSSA();
       	fprintf(stderr,"Set [%s] OK!\n",which);
	return 0;
}
*/
int SetTraceFlag(char *which,int flag)
{
	static struct BCDAst *pBCDAst = NULL;
	int    i, iLinkCnt;
	char   caBankName[16], caBankCode[64];


   	if(which == NULL)
   	{
   		return -1;
   	}


	if(!pBCDAst)
		pBCDAst = (struct BCDAst *)shm_connect(BCDA_SHM_NAME, NULL);
	if(!pBCDAst)
	{
		fprintf(stdout,"cannot connect to share memory 'BCDA'!\n");
    	return 0;
  	}


	for(i=0; i <  pBCDAst->iBankNum; i++)
	{
		memset(caBankName,0,sizeof(caBankName));
    	if(memcmp(which,pBCDAst->stBcda[i].caBankName,4)==0)
    	{
			pBCDAst->stBcda[i].cAppFlag = flag;
	       	fprintf(stderr,"Set [%s] = [%d] OK!\n",which,flag);
			return 0;
    	}
  	}			
   	fprintf(stderr,"Set [%s] failure! not this host\n",which);
	return -1;
}

int AddBcda ( char * name )
{
	static struct BCDAst *pBCDAst = NULL;
  int    i ,iLinkCnt;
  int    iBankCnt, iFd, iRc;
  char   caBankName[16], caBankCode[64];
  char   caBuffer[512],caFileName[256];
  char   *pcBankName, *pcBankCode, *psSecurity;
  char   *pcType,*pcOpenLog;


  if(!pBCDAst)
      pBCDAst = (struct BCDAst *)shm_connect(BCDA_SHM_NAME, NULL);
  if(!pBCDAst)
  {
    fprintf(stdout,"cannot connect to share memory 'BCDA'!\n");
    return 0;
  }
  
  memset(caFileName, 0, sizeof(caFileName) );
  sprintf(caFileName, "%s/config/%s",
          (char *)getenv( "ICS_HOME" ),
          ISDBANK );
  if((iFd = conf_open(caFileName)) < 0)
  {
    dcs_log( 0,0, "<CreatBcda> open  '%s' failed!", caFileName );
    return ( -1 );
  }
  for( iRc = conf_get_first_string(iFd, "bank.entry",caBuffer);
       iRc == 0 ;
       iRc = conf_get_next_string(iFd, "bank.entry",caBuffer))
  {
    pcBankName     = strtok(caBuffer," \t\r");
    if ( u_stricmp(name ,pcBankName)!=0) continue;
    pcBankCode     = strtok(NULL    ," \t\r\n");
    psSecurity = strtok(NULL    ," \t\r\n");
    pcType 	   = strtok(NULL    ," \t\r\n");
		pcOpenLog  = strtok(NULL    ," \t\r\n");
    break;
   
  
  }
   conf_close(iFd);
		iLinkCnt = pBCDAst->iBankNum;
		for( i=0; i<iLinkCnt; i++)
		{
				if ( pBCDAst->stBcda[i].iUseFlag !=1 ) 
						break;
		}
		if ( i == iLinkCnt)
		{
			   fprintf(stdout,"cannot free slot in 'BCDA'!\n");
				 return 0;
		}
    pBCDAst->stBcda[i].iBankSeq = i;
    memcpy(pBCDAst->stBcda[i].caBankCode,pcBankCode,
               min(TMS_BANK_CODE_LEN,strlen(pcBankCode)));
    memcpy(pBCDAst->stBcda[i].caBankName,pcBankName,min(15,strlen(pcBankName)));
    pBCDAst->stBcda[i].cNetFlag =  0;
    pBCDAst->stBcda[i].cSafFlag =  TMS_SAFLAG_OFF;
//    pBCDAst->stBcda[i].cDateSwhBgnFlag = TMS_DATESW_OFF;
    pBCDAst->stBcda[i].SendFlag=0;
    pBCDAst->stBcda[i].ReadFlag=0;
    pBCDAst->stBcda[i].cResetKeyFlag   = TMS_RESETKEY_OFF;
    if( (pcType != NULL) && (strlen(pcType) < 8) )
		strcpy(pBCDAst->stBcda[i].caType,pcType); 
/*
    if(psSecurity == NULL)
      pBCDAst->stBcda[i].cSecurityFlag  = 1; //default on yes
    else
      pBCDAst->stBcda[i].cSecurityFlag  =
            (0 == u_stricmp(psSecurity, "yes")) ? 1 : 0;
*/
	//打开关闭日志标志
    if(pcOpenLog == NULL)
      pBCDAst->stBcda[i].cAppFlag  = 1; //default on yes
    else
      pBCDAst->stBcda[i].cAppFlag  =
            (0 == u_stricmp(pcOpenLog, "yes")) ? 1 : 0;
		pBCDAst->stBcda[i].iUseFlag =1;
  

 
	return 1;
}
int UpdateBcda ( char * name )
{
static struct BCDAst *pBCDAst = NULL;
  int    i, iLinkCnt;
  int    iBankCnt, iFd, iRc;
  char   caBankName[16], caBankCode[64];
  char   caBuffer[512],caFileName[256];
  char   *pcBankName, *pcBankCode, *psSecurity;
  char   *pcType,*pcOpenLog;


  if(!pBCDAst)
      pBCDAst = (struct BCDAst *)shm_connect(BCDA_SHM_NAME, NULL);
  if(!pBCDAst)
  {
    fprintf(stdout,"cannot connect to share memory 'BCDA'!\n");
    return 0;
  }
  
  memset(caFileName, 0, sizeof(caFileName) );
  sprintf(caFileName, "%s/config/%s",
          (char *)getenv( "ICS_HOME" ),
          ISDBANK );
  if((iFd = conf_open(caFileName)) < 0)
  {
    dcs_log( 0,0, "<CreatBcda> open  '%s' failed!", caFileName );
    return ( -1 );
  }
  for( iRc = conf_get_first_string(iFd, "bank.entry",caBuffer);
       iRc == 0 ;
       iRc = conf_get_next_string(iFd, "bank.entry",caBuffer))
  {
    pcBankName     = strtok(caBuffer," \t\r");
    if ( u_stricmp(name ,pcBankName)!=0) continue;
    pcBankCode     = strtok(NULL    ," \t\r\n");
    psSecurity = strtok(NULL    ," \t\r\n");
    pcType 	   = strtok(NULL    ," \t\r\n");
		pcOpenLog  = strtok(NULL    ," \t\r\n");
    
    break;
  
  }
   conf_close(iFd);
		iLinkCnt = pBCDAst->iBankNum;
		for( i=0; i<iLinkCnt; i++)
		{
				if ( pBCDAst->stBcda[i].iUseFlag !=1 ) 
						continue;
				if ( u_stricmp(pBCDAst->stBcda[i].caBankName,pcBankName) ==0)
						break;
		}
		if ( i == iLinkCnt)
		{
			   fprintf(stdout,"cannot found %s slot in 'BCDA'!\n",name);
				 return 0;
		}
    memcpy(pBCDAst->stBcda[i].caBankCode,pcBankCode,
               min(TMS_BANK_CODE_LEN,strlen(pcBankCode)));
    pBCDAst->stBcda[i].cNetFlag =  0;
    if( (pcType != NULL) && (strlen(pcType) < 8) )
		strcpy(pBCDAst->stBcda[i].caType,pcType); 
/* 
    if(psSecurity == NULL)
      pBCDAst->stBcda[i].cSecurityFlag  = 1; //default on yes
    else
      pBCDAst->stBcda[i].cSecurityFlag  =
            (0 == u_stricmp(psSecurity, "yes")) ? 1 : 0;
*/
	//打开关闭日志标志
    if(pcOpenLog == NULL)
      pBCDAst->stBcda[i].cAppFlag  = 1; //default on yes
    else
      pBCDAst->stBcda[i].cAppFlag  =
            (0 == u_stricmp(pcOpenLog, "yes")) ? 1 : 0;
		pBCDAst->stBcda[i].iUseFlag =1;
  

 
	return 1;
}
int DelBcda ( char * name )
{
	  static struct BCDAst *pBCDAst = NULL;
  int    i, iLinkCnt;
  char   caBankName[16], caBankCode[64];

  if(!pBCDAst)
      pBCDAst = (struct BCDAst *)shm_connect(BCDA_SHM_NAME, NULL);
  if(!pBCDAst)
  {
    fprintf(stdout,"cannot connect to share memory 'BCDA'!\n");
    return 0;
  }
  iLinkCnt = pBCDAst->iBankNum;
		for( i=0; i<iLinkCnt; i++)
		{
				if ( pBCDAst->stBcda[i].iUseFlag !=1 ) 
						continue;
				if ( u_stricmp(pBCDAst->stBcda[i].caBankName,name) ==0)
						break;
		}
		if ( i == iLinkCnt)
		{
			   fprintf(stdout,"cannot found %s slot in 'BCDA'!\n",name);
				 return 0;
		}
		pBCDAst->stBcda[i].iUseFlag =0;
	return 1;
}
