#include "ibdcs.h"
#include "tmcibtms.h"
#include "dbstruct.h"
#include "../db/inc/glbdb.h"
#include "CMDI.h"
#include "bc.h"
#include "iso8583.h"
#include <string.h>
#include <stdlib.h>

#define MIN(a,b) a>b?b:a

int main(int argc, char *argv[])
{
	ti_bc_ins_inf_def tiBcInsInf; // ���ջ����Ļ�����Ϣ
  ti_bc_ins_inf_def *p_tiBcInsInf=&tiBcInsInf;
  char resp_code[7];
  int sqlCode;
  //�������ݿ�
  if ( DasConectDB()!=0 )
  {
    printf("����oracle DBʧ��.\n");
    return -1;
  }
  if ( fold_initsys() < 0 )
  {
  	printf("fold_init failure.\n");
    return -1;
  }
	if( 0!= BC_Search_ins_inf(resp_code, p_tiBcInsInf, "00005500"))
	{
		printf("����������Ϣʧ��!\n");
		return -1;
	}    
	if(0>DESGETKY(resp_code, p_tiBcInsInf->key_1, p_tiBcInsInf->key_index))
	{
		printf("ȡKeyʧ�ܣ�\n");
		return -1;
	}
	else
	{
		printf("%s,%s\n", p_tiBcInsInf->key_1, p_tiBcInsInf->key_index);
	}
	memcpy(p_tiBcInsInf->key_2, p_tiBcInsInf->key_1, 16);
	memcpy(p_tiBcInsInf->key_3, p_tiBcInsInf->key_1, 16);
	tiBcInsInfOpr(GLB_DB_UPDATE2, p_tiBcInsInf, &sqlCode);
}



