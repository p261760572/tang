extern int SignOrder(char *MerId, char *MerKeyFile, char *OrdId, char *TransAmt, char *MerDate, char *TransType, char * GateId , char * MerPriv , char * BgRetUrl , char * PageRetUrl , char *ChkValue) ;

extern int VeriSignOrder(char *MerId, char *PgKeyFile, char *OrdId, char *TransAmt, char *MerDate, char *TransType, char *TransStat, char *GateId, char * MerPriv , char *SysDate, char *SysSeqId, char *ChkValue) ;

extern int SignMsg(char *MerId, char *MerKeyFile, char * MsgData, int MsgLen, char *ChkValue) ;
extern int VeriSignMsg(char *PgKeyFile, char *MsgData, int  MsgLen, char *ChkValue) ;

