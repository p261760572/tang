#ifndef EMCGLOBAL_H
#define EMCGLOBAL_H

//#ifndef _MainProg
//#define EXTERN extern
//#else
//#define EXTERN
//#endif

#define EXTERN extern

EXTERN char   g_caSwaShmName[];	// ϵͳ�������Ĺ����ڴ�����
EXTERN char   g_caBtaShmName[];	// ҵ�������Ĺ����ڴ�����
EXTERN char   g_caTcaShmName[];	// ��ʱ���Ʊ�Ĺ����ڴ�����
EXTERN char   g_caMonFoldName[];	// �����������ļ�������
extern char   g_caIsaFoldName[];	// ���н���ϵͳ�����ļ�������
EXTERN int    g_iTestMode;		// ����ģʽ��0:����  1������
extern int    g_iMaxPacketSize;		// ���紫���е������ֽ���
extern int    g_iIbTimer2;		// �������н��׵����ʱʱ��
EXTERN int    g_iIbTimer3;		// ��ʼ����ĳ�ʱʱ��
EXTERN int    g_iIbTimer4;		// ��������ĳ�ʱʱ��
EXTERN int    g_iIbTimer5;		// ���п�ʼ����ĳ�ʱʱ��
EXTERN int    g_iIbTimer6;		// ���н�������ĳ�ʱʱ��
EXTERN char   g_caHostName[];		// ��������
EXTERN short  g_iListenPort;		// �����˿�

extern int    g_iIsaFoldId;		// ��ſ��н��׹���ϵͳ�����ļ��б�ʶ
EXTERN int    g_iMonFoldId;		// ��Ż�������ϵͳ�����ļ��б�ʶ

EXTERN int    g_iBcdaShmId;		// �����ڴ�BCDA�ı�ʶ
//EXTERN char   *g_pcBcdaShmPtr;		// �����ڴ�BCDA���׵�ַ
 char   *g_pcBcdaShmPtr;		// �����ڴ�BCDA���׵�ַ

EXTERN int    g_iShutDownFlag;		// ͨѶ���������̹ػ�֪ͨ��1���ػ�
EXTERN int    g_iMaxTxnPerTime;		// ͬһʱ��������������
EXTERN char   g_cTermFlag;		// ������ֹ��־; 'y'��ʾ��ֹ

/* Mary add begin, 2001-6-5 */
EXTERN char   g_caCenterCode[12];	// ���Ļ�������
EXTERN int    g_iCenterCodeLen;		// ���Ļ������볤��
/* Mary add end, 2001-6-5 */

#endif
