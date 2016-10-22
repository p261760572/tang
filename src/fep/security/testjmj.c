#include "dccdcs.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static int OpenLog(char *IDENT)
{
    char logfile[256];

    //assuming the log file is "$FEP_RUNPATH/log/appsvr1.log"
    if ( u_fabricatefile("log/testjmj.log",logfile,sizeof(logfile)) < 0 )
        return -1;
    return dcs_log_open(logfile, IDENT);
}

int main(int argc, char *argv[])
{

    char configfile[500];
    char return_code[4],priv_key_data[2048],pub_key_data[2048];
    int priv_len,pub_len,i;
         //prepare the logging stuff
    if ( 0 > OpenLog(argv[0]) )
    {
         return(1);
    }
 
    //attach to SHM of IBDCS
    if ( dcs_connect_shm() < 0 )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d>dcs_connect_shm() failed:%s\n",strerror(errno),__FILE__,__LINE__);
        return(1);
    }
   
    if ( 1 != DESInit() )
    {
        dcs_log(0,0,"<FILE:%s,LINE:%d> DESInit() failed:%s\n",__FILE__,__LINE__);
        return(1);
    }
    printf("\n begin work\n");
    memset(return_code,0,sizeof(return_code));
/*
    i= RSAGETKY(return_code, priv_key_data,&priv_len,pub_key_data,&pub_len,99,1024);
    printf("\nreturn code =[%s]",return_code);
    if ( i>0)
    {
       dcs_debug(priv_key_data,priv_len,"priv len=[%d]",priv_len);
       dcs_debug(pub_key_data,pub_len,"pub len=[%d]",pub_len);
       
       printf("\nRSAGETKY perform succ!\n");   
    }
    
    memset(return_code,0,sizeof(return_code));
//    i= RSAGETKY(return_code, priv_key_data,&priv_len,pub_key_data,&pub_len,99,2048);
    i= RSALOADKEY(return_code, priv_key_data,priv_len ,0);
    printf("\nreturn code =[%s]",return_code);
    if ( i>0)
    {
       dcs_debug(priv_key_data,priv_len,"priv len=[%d]",priv_len);
       dcs_debug(pub_key_data,pub_len,"pub len=[%d]",pub_len);
       
       printf("\nRSALOADKEY perform succ!\n");   
    }
    memset(priv_key_data,0,sizeof(priv_key_data));
    priv_len=-1;
    i= RSAIMPKEY(return_code, priv_key_data,&priv_len ,0);
    printf("\nreturn code =[%s]",return_code);
    if ( i>0)
    {
       dcs_debug(priv_key_data,priv_len,"RSAIMPKEY priv len=[%d]",priv_len);
       printf("\nRSAIMPKEY perform succ!\n");   
    }
    */
    char key_data[33],des_mdata[33];
    memset(key_data,0,sizeof(key_data));
    memset(des_mdata,0,sizeof(des_mdata));
/*    DESGETKY(return_code, key_data, "008");
    i=RSATODES_PIN(return_code, key_data,16 ,0,1,"0000000000000011","0000000000000000",
                 "B9DB18E9C8ED40E4B3070D0355AA089340681006F56E50C27E8EB94A0151F14ABDBCD08F99778AABB3AA58E3034B2125FA792C3D0BC51A4478DF641B6729585663E80448941241AFFA5F94B2AC0B7F784C6C4B402D47B8DE133D0D3347807A75D7E2BBB60EF39F82C842603E628FAF62D35DA44DE68F57FBC187E8DB0646AA2A",
 
                 256,des_mdata);

    printf("\n<RSATODES_PIN>return code =[%s]key_data=[%s]\n,",return_code,key_data);
    if ( i>0)
    {
       dcs_debug(des_mdata,8,"RSATODES_PIN ");
       printf("\n<RSATODES_PIN> perform succ!\n");   
    }
*/ 
    char rsa_mdata[1024],rsa_key[512],rsa_key1[512];
    int rsa_datalen;
    memset(return_code,0,sizeof(return_code));
    memset(rsa_mdata,0,sizeof(rsa_mdata));
    memset(rsa_key,0,sizeof(rsa_key));
//	strcpy(rsa_key,"E543E557AC261B0A411475FD9FBF15D4EF0A331E83E728F88F84A0E015B7849E9653B4CFCD63C7AE3386BA599D018A5DCE2651392EB282DBBA9AD07AB3FE78BBBBA7F09501D47FADDE5E2597C0D7BC4DBEBD7EB1A0AAA7994BB2007036EA86AF1D4DB1210AC66F938B4346928029FD7988ECA8B459C1A34F108B5204FF7DC53D");
      strcpy(rsa_key,"30818902818100e543e557ac261b0a411475fd9fbf15d4ef0a331e83e728f88f84a0e015b7849e9653b4cfcd63c7ae3386ba599d018a5dce2651392eb282dbba9ad07ab3fe78bbbba7f09501d47fadde5e2597c0d7bc4dbebd7eb1a0aaa7994bb2007036ea86af1d4db1210ac66f938b4346928029fd7988eca8b459c1a34f108b5204ff7dc53d0203010001");
    asc_to_bcd(rsa_key1,rsa_key,280,0);
    i=DESTORAS_PIN(return_code, "6E15887F3CBA607E",16 ,
                 rsa_key1,
                  140,
                 "008","0000111111111111","0000000000000000",
                 "435343139CE3EC13",rsa_mdata,&rsa_datalen);
    printf("\n<DESTORAS_PIN>return code =[%s]i=[%d]\n,",return_code,i);

    if ( i>0)
    {
       dcs_debug(rsa_mdata,rsa_datalen,"DESTORAS_PIN len=%d",rsa_datalen);
       printf("\n<DESTORAS_PIN> perform succ!\n");   
    }
 
}//main()
