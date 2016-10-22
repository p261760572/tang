#ifndef _IVR_H_
#define _IVR_H_

#define IVR_E_NOTKEY -1
#define IVR_E_NOTEND -2

//IVR 报文结构
typedef struct{
    char head   [8];
    char opcode [4];
    char flag   [1];
    char fwd_ins_id_cd [8];
    char key[38]; // 6 + 8 + 20 + 4
    char data[2048];
}IVR_STRUCT;

int IVR_Init();
int IVR_SendIVR(int foldid, IVR_STRUCT *ivr );
int IVR_Get_Field(char *outbuff,  IVR_STRUCT *ivr, char *key );
int IVR_Add_Field(IVR_STRUCT *ivr, char *key, char *value);

#endif

