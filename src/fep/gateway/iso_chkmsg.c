#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include "iso8583.h"


static  int g_nChkWhich = CHK_LENGTH;


int _IsNumericString(char *str, int n)
{
    int i;

    for(i=0; i<n; i++)
        if(!isdigit(str[i]))
        {
            g_nChkWhich = CHK_FORMAT;
            return 0;
        }

    return 1;
}

int _IsHexString(char *str, int n)
{
    int i;

    for(i=0; i<n; i++)
        if(!isxdigit(str[i]))
        {
            g_nChkWhich = CHK_FORMAT;
            return 0;
        }

    return 1;
}

#define  CHAR2_TO_INT(a, b)  (((a) - '0') * 10 + ((b) - '0'))

int _IsDateTime(char *str, int n, int format)
{
    struct tm  *ptm;
    time_t  t;
    int year, month, day, hour, minute, second;

    if( !_IsNumericString(str, n) )
        goto lbl_Err;

    time(&t);
    ptm = localtime(&t);

    switch(format)
    {
        case FMT_MMDDHHMMSS:
            month = CHAR2_TO_INT(str[0] , str[1]);
            day   = CHAR2_TO_INT(str[2] , str[3]);
            if(!IsValidYYMMDD(ptm->tm_year+1900, month, day))
                goto lbl_Err;

            hour  = CHAR2_TO_INT(str[4] , str[5]);
            minute= CHAR2_TO_INT(str[6] , str[7]);
            second= CHAR2_TO_INT(str[8] , str[9]);

            if(!IsValidHHMMSS(hour, minute, second))
                goto lbl_Err;
            return 1;

        case FMT_HHMMSS:
            hour  = CHAR2_TO_INT(str[0] , str[1]);
            minute= CHAR2_TO_INT(str[2] , str[3]);
            second= CHAR2_TO_INT(str[4] , str[5]);

            if(!IsValidHHMMSS(hour, minute, second))
                goto lbl_Err;
            return 1;

        case FMT_MMDD:
            month = CHAR2_TO_INT(str[0] , str[1]);
            day   = CHAR2_TO_INT(str[2] , str[3]);

            if(!IsValidYYMMDD(ptm->tm_year+1900, month, day))
                goto lbl_Err;
            return 1;


        case FMT_YYMMDD:
            year = CHAR2_TO_INT(str[0] , str[1]) + 2000;
            month= CHAR2_TO_INT(str[2] , str[3]);
            day  = CHAR2_TO_INT(str[4] , str[5]);

            if(!IsValidYYMMDD(year, month, day))
                goto lbl_Err;
            return 1;

        default:
            return 1;
    }//switch

lbl_Err:
    g_nChkWhich = CHK_DATETIME;
    return 0;
}

int IsValidYYMMDD(int yy, int mm, int dd)
{
    int dayTbl[2][13] = {{0, 31, 28, 31, 30, 31, 30, 31, 31,30,31, 30, 31},
                         {0, 31, 29, 31, 30, 31, 30, 31, 31,30,31, 30, 31}};
    int leap;

    if(mm <0 || mm > 12)
        return 0;

    if( yy % 100 == 0)
        leap = (yy % 400 == 0) ? 1 : 0;
    else
        leap = (yy % 4   == 0) ? 1 : 0;

    if( 1 <= dd  && dd <= dayTbl[leap][mm])
        return 1;
    return  0;
}

int IsValidHHMMSS(int hh, int mm, int ss)
{
    if(hh <0 || hh>23)  return 0;
    if(mm <0 || mm>59)  return 0;
    if(ss <0 || ss>59)  return 0;
    return 1;
}
/*
int dcs_print_tbl(FILE *fp)
{
    int i, j, k;

    for(i=0; g_BitLayoutTbl[i].msg_mti > 0; i++)
    {
        fprintf(fp, "trans.bitset %.4d  %.6d  {",
                g_BitLayoutTbl[i].msg_mti, g_BitLayoutTbl[i].msg_proccode);
        for(k=0, j=0; j<128; j++)
            if( ISBITSET(g_BitLayoutTbl[i].msg_bitmap, j))
            {
                if(k==0)
                    fprintf(fp, "%d", j+1);
                else
                    fprintf(fp, ",%d", j+1);
                k++;
            }


        fprintf(fp, "}\n");
    }

    fprintf(fp, "\n\n\n");

    for(i=0; g_QuestAnswerTbl[i].mti_quest > 0; i++)
        fprintf(fp, "trans.askanswer %.4d  %.4d \n",
                g_QuestAnswerTbl[i].mti_quest,g_QuestAnswerTbl[i].mti_answer);
}
*/
/*
int iso_load_8583config()
{
    // this function load configuration from file
    // "$IBDCS_RUNPATH/config/iso8583.conf"

    char cfgfile[256], caBuffer[1024];
    int  fd, ret, nBitNo;
    char *pBitNo, *pLenFlag, *pMaxSize, *pFormat, *pBitType;
    int   bitCounter;

    if(u_fabricatefile("config/iso8583.conf",cfgfile,sizeof(cfgfile)) < 0)
        return -1;

    if(0 > (fd = conf_open(cfgfile)) )
        return -1;

    memset(caBuffer,0,sizeof(caBuffer));

    bitCounter = 0;
    for(ret= conf_get_first_string(fd,"iso8583.bit",caBuffer);
        ret == 0;
        ret=conf_get_next_string(fd,"iso8583.bit",caBuffer))
    {
        pBitNo   = (char *)strtok(caBuffer," \t\r\b\n");
        pLenFlag = (char *)strtok(NULL    ," \t\r\b\n");
        pMaxSize = (char *)strtok(NULL    ," \t\r\b\n");
        pFormat  = (char *)strtok(NULL    ," \t\r\b\n");
        pBitType = (char *)strtok(NULL    ," \t\r\b\n");

        if(pBitNo == NULL || pLenFlag == NULL || pMaxSize == NULL ||
           pFormat == NULL)
            continue;

        nBitNo = atoi(pBitNo);
        if(!(0 <= nBitNo && nBitNo <= 127))
            continue;
        SetIsoElement( nBitNo , atoi(pMaxSize) , atoi(pBitType) ,iso_GetLenFlagInt(pLenFlag),iso_GetFormatInt(pFormat));

        memset(caBuffer,0,sizeof(caBuffer));
        bitCounter ++;
    }

    conf_close(fd);
    return bitCounter > 0 ? 0 : (-1);
}
*/
/*
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
*/
