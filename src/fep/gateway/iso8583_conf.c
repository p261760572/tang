#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iso8583.h"
extern int _head_len,_msg_id_flg,_msg_fld_len_flg,_bitmap_flg;
int IsoLoad8583config (struct ISO_8583 *pIso8583Bit ,char *pFile )
{
  char cfgfile[256], caBuffer[1024];
    int  fd, ret, nBitNo;
    char *pBitNo, *pLenFlag, *pMaxSize, *pFormat, *pType;
    int   bitCounter;
/*     
    sprintf(caBuffer,"config/%s",pFile);
    if(u_fabricatefile(caBuffer,cfgfile,sizeof(cfgfile)) < 0)
        return -1;
*/
    if(0 > (fd = conf_open(pFile)) )
        return -1;

    conf_get_first_number(fd,"iso8583.head_len",&_head_len);
//    conf_get_first_number(fd,"iso8583.bitmap_flag",pIso8583Bit->bitmap_flag);
dcs_debug(0,0,"<%s> _head_len=%d",__FUNCTION__,_head_len);
    conf_get_first_number(fd,"iso8583.var_flag",&_msg_fld_len_flg);
    dcs_debug(0,0,"<%s> _msg_fld_len_flg=%d",__FUNCTION__,_msg_fld_len_flg);
    conf_get_first_number(fd,"iso8583.msgid_flag",&_msg_id_flg);
    dcs_debug(0,0,"<%s> _msg_id_flg=%d",__FUNCTION__,_msg_id_flg);
    conf_get_first_number(fd,"iso8583.bitmap_flag",&_bitmap_flg);
    dcs_debug(0,0,"<%s> _bitmap_flg=%d",__FUNCTION__,_bitmap_flg);
    memset(caBuffer,0,sizeof(caBuffer));

    bitCounter = 0;
    for(ret= conf_get_first_string(fd,"iso8583.bit",caBuffer);
        ret == 0;
        ret=conf_get_next_string(fd,"iso8583.bit",caBuffer))
    {
        pBitNo   = strtok(caBuffer," \t\r\b\n");
        pLenFlag = strtok(NULL    ," \t\r\b\n");
        pMaxSize = strtok(NULL    ," \t\r\b\n");
        pFormat  = strtok(NULL    ," \t\r\b\n");
        pType = strtok(NULL    ," \t\r\b\n");
//        pBitName = strtok(NULL    ," \t\r\b\n");

        if(pBitNo == NULL || pLenFlag == NULL || pMaxSize == NULL ||
           pFormat == NULL || pType == NULL )
            continue;

        nBitNo = atoi(pBitNo);
        if(!(0 <= nBitNo && nBitNo <= 128))
            continue;
//        pIso8583Bit[nBitNo].bit_index    = nBitNo;
        pIso8583Bit[nBitNo].flag = iso_GetLenFlagInt(pLenFlag);
        pIso8583Bit[nBitNo].len   = atoi(pMaxSize);
        pIso8583Bit[nBitNo].fmt   = iso_GetFormatInt(pFormat);
        pIso8583Bit[nBitNo].type = atoi(pType);
//        memcpy(pIso8583Bit[nBitNo].bit_name , pBitName,(strlen(pBitName)>=40 )?40:strlen(pBitName));
        memset(caBuffer,0,sizeof(caBuffer));
        bitCounter ++;
    }
    conf_close(fd);
    return bitCounter > 0 ? 0 : (-1);
}



int  iso_GetLenFlagInt(char *pLenFlag)
{
    if(u_stricmp("LEN_FIXED", pLenFlag) == 0) return LEN_FIXED;
    if(u_stricmp("LEN_LLVAR", pLenFlag) == 0) return LEN_LLVAR;
    if(u_stricmp("LEN_LLLVAR",pLenFlag) == 0) return LEN_LLLVAR;

    return LEN_FIXED;
}

int iso_GetFormatInt(char *pFormat)
{
    if(u_stricmp("FMT_MMDDHHMMSS",  pFormat) == 0) return FMT_MMDDHHMMSS;
    if(u_stricmp("FMT_YYMMDDHHMMSS",pFormat) == 0) return FMT_YYMMDDHHMMSS;
    if(u_stricmp("FMT_YYMM",        pFormat) == 0) return FMT_YYMM;
    if(u_stricmp("FMT_YYYYMMDD",    pFormat) == 0) return FMT_YYYYMMDD;
    if(u_stricmp("FMT_YYMMDD",      pFormat) == 0) return FMT_YYMMDD;
    if(u_stricmp("FMT_MMDD",        pFormat) == 0) return FMT_MMDD;
    if(u_stricmp("FMT_HHMMSS",      pFormat) == 0) return FMT_HHMMSS;
    if(u_stricmp("FMT_MONEY",       pFormat) == 0) return FMT_MONEY;
    if(u_stricmp("FMT_B",           pFormat) == 0) return FMT_B;
    if(u_stricmp("FMT_N",           pFormat) == 0) return FMT_N;
    if(u_stricmp("FMT_A",           pFormat) == 0) return FMT_A;
    if(u_stricmp("FMT_AN",          pFormat) == 0) return FMT_AN;
    if(u_stricmp("FMT_ANS",         pFormat) == 0) return FMT_ANS;
    if(u_stricmp("FMT_Z",           pFormat) == 0) return FMT_Z;

    return FMT_N;
}
