#include  "ibdcs.h"
#include  "tmcibtms.h"
#include  "dbstruct.h"
#include  "bc.h"

#define IVR_START1 "<~~"
#define IVR_START2 "~~>"
#define IVR_END "<~~/~~>"
 
int g_IVR_FOLDER_ID = -1;

/*******************************

	����Ϊ�ڲ����������Ƽ�ʹ��
	
********************************/
// ��ivr �����л�ȡ�ؼ��򣬳ɹ������䳤�ȣ����򷵻�ʧ�ܴ��루��ֵ��
int IVR_GetFieldValue(char *outbuff, char *ivrdata, char *key)
{
	int len;
	char *p1,*p2;
	char skey[256];
	
	strcpy(skey, IVR_START1);
	strcat(skey, key);
	strcat(skey, IVR_START2);
	
	dcs_debug(0,0, "<FILE:%s,LINE:%d>start search [%s] in[%s]!",__FILE__,__LINE__, skey, ivrdata);
	
	p1 = strstr(ivrdata, skey);
	if( 0 == p1 ){
		 dcs_debug(0,0, "<FILE:%s,LINE:%d>δ�ҵ����[%s]��[%s]��!",__FILE__,__LINE__, skey, ivrdata);	
		 return IVR_E_NOTKEY; //û���ҵ��ñ��
	}
	p1 += strlen(skey);	
	p2 = strstr(p1, IVR_END);
	if( 0 == p2 ){
		dcs_debug(0,0, "<FILE:%s,LINE:%d>δ�ҵ��������[%s]��[%s]��!",__FILE__,__LINE__, IVR_END, ivrdata);	
		return IVR_E_NOTEND; //û���ҵ��������
  }
		
	len = 	p2-p1;
	memcpy(outbuff, p1, len);
	*(outbuff+len)	= 0;	
	return len;
}

int IVR_Init()
{
	g_IVR_FOLDER_ID = fold_locate_folder( "IVRAPP" );
	if( -1 == g_IVR_FOLDER_ID){
		dcs_log(0,0, "<FILE:%s,LINE:%d>��ȡIVR,foldidʧ��!",__FILE__,__LINE__,g_IVR_FOLDER_ID);
    return -1;
	}
	return 0;
}

int IVR_AddFieldValue(char *ivrdata, char *key, char *value)
{
	strcat(ivrdata, IVR_START1);
	strcat(ivrdata, key);
	strcat(ivrdata, IVR_START2);
	strcat(ivrdata, value);
	strcat(ivrdata, IVR_END);
	return 0;
}

int IVR_SetFieldValue (char *ivrdata, char *key, char *value)
{
	int len, lenvalue;
	char *p1,*p2,*p3;
	char skey[256];
	
	strcpy(skey, IVR_START1);
	strcat(skey, key);
	strcat(skey, IVR_START2);
	
	dcs_debug(0,0, "<FILE:%s,LINE:%d>start search [%s] in[%s]!",__FILE__,__LINE__, skey, ivrdata);
	
	p1 = strstr(ivrdata, skey);
	if( 0 == p1 ){
		 return IVR_AddFieldValue(ivrdata, key, value);
	}
	p1 += strlen(skey);	
	p2 = strstr(p1, IVR_END);
	if( 0 == p2 ){
		dcs_debug(0,0, "<FILE:%s,LINE:%d>δ�ҵ��������[%s]��[%s]��!",__FILE__,__LINE__, IVR_END, ivrdata);	
		return IVR_E_NOTEND; //û���ҵ��������
  }
  
  //move
  lenvalue = strlen(value);
	len = 	p2-p1;
	if( lenvalue < len ){
	  memcpy(p1+lenvalue, p2, strlen(p2));
	}else if(lenvalue > len){
		p3 = p2 + strlen(p2); //tail
	 	while( p3-lenvalue >= p2 ) *(p3--) = *(p3-lenvalue);
	}
		 	
	//set value
	memcpy(p1, value, lenvalue );
	return 0;
}



int IVR_SendMsg(int foldid, const char *packet, const int size)
{
	//IVR_STRUCT
	if ( 0 > fold_write(g_IVR_FOLDER_ID, foldid, packet, size) )
  {
      dcs_log(0,0, "<FILE:%s,LINE:%d><IVR_SendMsg>Failure to write msg to folder[%d]!",__FILE__,__LINE__,g_IVR_FOLDER_ID);
      return -1;
  } else
  {
      dcs_debug(0,0, "<FILE:%s,LINE:%d><IVR_SendMsg>Success to write msg to folder[%d]",__FILE__,__LINE__,g_IVR_FOLDER_ID );
      return 0;
  }
	return 0;
}


/*******************************

	����Ϊ���ⲿ���õĺ����ӿ�
	
********************************/

int IVR_SendIVR( int foldid, IVR_STRUCT *ivr )
{
	int size = sizeof(IVR_STRUCT) - sizeof(ivr->data) + strlen(ivr->data) ;
	dcs_debug(0,0,"<FILE:%s,LINE:%d>����IVR(%d)��Ϣ(%s).\n",__FILE__,__LINE__, foldid, ivr);
	return IVR_SendMsg(foldid, (const char *)ivr, size);
}

int IVR_Get_Field(char *outbuff,  IVR_STRUCT *ivr, char *key )
{
	return IVR_GetFieldValue(outbuff, ivr->data, key);
}

int IVR_Add_Field(IVR_STRUCT *ivr, char *key, char *value)
{
	return IVR_AddFieldValue(ivr->data, key, value);
}

int IVR_Set_Field(IVR_STRUCT *ivr, char *key, char *value)
{
	return IVR_SetFieldValue(ivr->data, key, value);
}
