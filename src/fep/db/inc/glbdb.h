/*
 *  Copyright 2008, China UnionPay Co., Ltd.  All right reserved.
 *
 *
 *  $Id: glbdb.h,v 1.0 2008-11-21 14:56:04 weichai Exp $
 *
 *  Edit History:
 *    2008-11-21 14:57:17 - Created by weichai.
 *
 */

#ifndef __GLBDB_H
#define __GLBDB_H

/* 操作数据库操作类型定义 */
/* 第一组附加操作 */
#define GLB_DB_SELECT1                    101
#define GLB_DB_UPDATE1                    102
#define GLB_DB_INSERT1                    103
#define GLB_DB_DELETE1                    104
#define GLB_DB_DECLARE1                   105
#define GLB_DB_OPEN1                      106
#define GLB_DB_CLOSE1                     107
#define GLB_DB_FETCH1                     108
#define GLB_DB_LOAD1					            109
#define GLB_DB_TRUNCATE1				          110

/* 第二组附加操作 */
#define GLB_DB_SELECT2                    201
#define GLB_DB_UPDATE2                    202
#define GLB_DB_INSERT2                    203
#define GLB_DB_DELETE2                    204
#define GLB_DB_DECLARE2                   205
#define GLB_DB_OPEN2                      206
#define GLB_DB_CLOSE2                     207
#define GLB_DB_FETCH2                     208

/* 第三组附加操作 */
#define GLB_DB_SELECT3                    301
#define GLB_DB_UPDATE3                    302
#define GLB_DB_INSERT3                    303
#define GLB_DB_DELETE3                    304
#define GLB_DB_DECLARE3                   305
#define GLB_DB_OPEN3                      306
#define GLB_DB_CLOSE3                     307
#define GLB_DB_FETCH3                     308

/* 第四组附加操作 */
#define GLB_DB_SELECT4                    401
#define GLB_DB_UPDATE4                    402
#define GLB_DB_INSERT4                    403
#define GLB_DB_DELETE4                    404
#define GLB_DB_DECLARE4                   405
#define GLB_DB_OPEN4                      406
#define GLB_DB_CLOSE4                     407
#define GLB_DB_FETCH4                     408

/* 第五组附加操作 */
#define GLB_DB_SELECT5                    501
#define GLB_DB_UPDATE5                    502
#define GLB_DB_INSERT5                    503
#define GLB_DB_DELETE5                    504
#define GLB_DB_DECLARE5                   505
#define GLB_DB_OPEN5                      506
#define GLB_DB_CLOSE5                     507
#define GLB_DB_FETCH5                     508

/* 第六组附加操作 */
#define GLB_DB_SELECT6                    601
#define GLB_DB_UPDATE6                    602
#define GLB_DB_INSERT6                    603
#define GLB_DB_DELETE6                    604
#define GLB_DB_DECLARE6                   605
#define GLB_DB_OPEN6                      606
#define GLB_DB_CLOSE6                     607
#define GLB_DB_FETCH6                     608

/* 第七组附加操作 */
#define GLB_DB_SELECT7                    701
#define GLB_DB_UPDATE7                    702
#define GLB_DB_INSERT7                    703
#define GLB_DB_DELETE7                    704
#define GLB_DB_DECLARE7                   705
#define GLB_DB_OPEN7                      706
#define GLB_DB_CLOSE7                     707
#define GLB_DB_FETCH7                     708

/* 第八组附加操作 */
#define GLB_DB_SELECT8                    801
#define GLB_DB_UPDATE8                    802
#define GLB_DB_INSERT8                    803
#define GLB_DB_DELETE8                    804
#define GLB_DB_DECLARE8                   805
#define GLB_DB_OPEN8                      806
#define GLB_DB_CLOSE8                     807
#define GLB_DB_FETCH8                     808

/* 第九组附加操作 */
#define GLB_DB_SELECT9                    901
#define GLB_DB_UPDATE9                    902
#define GLB_DB_INSERT9                    903
#define GLB_DB_DELETE9                    904
#define GLB_DB_DECLARE9                   905
#define GLB_DB_OPEN9                      906
#define GLB_DB_CLOSE9                     907
#define GLB_DB_FETCH9                     908

/* 第十组附加操作 */
#define GLB_DB_SELECT10                  1001
#define GLB_DB_UPDATE10                  1002
#define GLB_DB_INSERT10                  1003
#define GLB_DB_DELETE10                  1004
#define GLB_DB_DECLARE10                 1005
#define GLB_DB_OPEN10                    1006
#define GLB_DB_CLOSE10                   1007
#define GLB_DB_FETCH10                   1008

/* 第十一组附加操作 */
#define GLB_DB_SELECT11                  1101
#define GLB_DB_UPDATE11                  1102
#define GLB_DB_INSERT11                  1103
#define GLB_DB_DELETE11                  1104
#define GLB_DB_DECLARE11                 1105
#define GLB_DB_OPEN11                    1106
#define GLB_DB_CLOSE11                   1107
#define GLB_DB_FETCH11                   1108

/* 第十二组附加操作 */
#define GLB_DB_SELECT12                  1201
#define GLB_DB_UPDATE12                  1202
#define GLB_DB_INSERT12                  1203
#define GLB_DB_DELETE12                  1204
#define GLB_DB_DECLARE12                 1205
#define GLB_DB_OPEN12                    1206
#define GLB_DB_CLOSE12                   1207
#define GLB_DB_FETCH12                   1208

/* 第十三组附加操作 */
#define GLB_DB_SELECT13                  1301
#define GLB_DB_UPDATE13                  1302
#define GLB_DB_INSERT13                  1303
#define GLB_DB_DELETE13                  1304
#define GLB_DB_DECLARE13                 1305
#define GLB_DB_OPEN13                    1306
#define GLB_DB_CLOSE13                   1307
#define GLB_DB_FETCH13                   1308

/* 第十四组附加操作 */
#define GLB_DB_SELECT14                  1401
#define GLB_DB_UPDATE14                  1402
#define GLB_DB_INSERT14                  1403
#define GLB_DB_DELETE14                  1404
#define GLB_DB_DECLARE14                 1405
#define GLB_DB_OPEN14                    1406
#define GLB_DB_CLOSE14                   1407
#define GLB_DB_FETCH14                   1408

/* 第十五组附加操作 */
#define GLB_DB_SELECT15                  1501
#define GLB_DB_UPDATE15                  1502
#define GLB_DB_INSERT15                  1503
#define GLB_DB_DELETE15                  1504
#define GLB_DB_DECLARE15                 1505
#define GLB_DB_OPEN15                    1506
#define GLB_DB_CLOSE15                   1507
#define GLB_DB_FETCH15                   1508

/* 第十六组附加操作 */
#define GLB_DB_SELECT16                  1601
#define GLB_DB_UPDATE16                  1602
#define GLB_DB_INSERT16                  1603
#define GLB_DB_DELETE16                  1604
#define GLB_DB_DECLARE16                 1605
#define GLB_DB_OPEN16                    1606
#define GLB_DB_CLOSE16                   1607
#define GLB_DB_FETCH16                   1608

/* 第十七组附加操作 */
#define GLB_DB_SELECT17                  1701
#define GLB_DB_UPDATE17                  1702
#define GLB_DB_INSERT17                  1703
#define GLB_DB_DELETE17                  1704
#define GLB_DB_DECLARE17                 1705
#define GLB_DB_OPEN17                    1706
#define GLB_DB_CLOSE17                   1707
#define GLB_DB_FETCH17                   1708

/* 第十八组附加操作 */
#define GLB_DB_SELECT18                  1801
#define GLB_DB_UPDATE18                  1802
#define GLB_DB_INSERT18                  1803
#define GLB_DB_DELETE18                  1804
#define GLB_DB_DECLARE18                 1805
#define GLB_DB_OPEN18                    1806
#define GLB_DB_CLOSE18                   1807
#define GLB_DB_FETCH18                   1808

/* 第十九组附加操作 */
#define GLB_DB_SELECT19                  1901
#define GLB_DB_UPDATE19                  1902
#define GLB_DB_INSERT19                  1903
#define GLB_DB_DELETE19                  1904
#define GLB_DB_DECLARE19                 1905
#define GLB_DB_OPEN19                    1906
#define GLB_DB_CLOSE19                   1907
#define GLB_DB_FETCH19                   1908

/* 第二十组附加操作 */
#define GLB_DB_SELECT20                  2001
#define GLB_DB_UPDATE20                  2002
#define GLB_DB_INSERT20                  2003
#define GLB_DB_DELETE20                  2004
#define GLB_DB_DECLARE20                 2005
#define GLB_DB_OPEN20                    2006
#define GLB_DB_CLOSE20                   2007
#define GLB_DB_FETCH20                   2008


#define RPT_TO_LOG		             1
#define RPT_TO_TTY		             2
#define RPT_TO_CON		             4
#define RPT_TO_TTY_DUMP	           8
#define DUMP_TWA		               1
#define DUMP_SWA		               2

#define SQL_ERD_NONE_RECORD       1433
#define SQL_ERD_OUT_FORK_NUM      1
#define SQL_ERD_UNIKEY            2
#define SQL_ERD_LOCKED            3

#define ADJUST_STRING(X) *(X+sizeof(X)-1) = 0;rtrim(X);if(0==*X)strcpy(X," ");

#define MEMSET_0(X) memset(X,0,sizeof(X));

//interface
int glb_Adjust_Decimal(char *output, char *decimal, int len);
char* rtrim(char* str);
char* rtrim_null(char* str, char nullchar);
int glb_min_strlen(char *src, int len);
int glb_SetErrLog_FuncPtr(int addr);
int glb_ErrLog(int iErrCode, char *pcaDispMsg,char cDumpDev,char *pcaDumpAddr,long lDumpLen);

#endif
