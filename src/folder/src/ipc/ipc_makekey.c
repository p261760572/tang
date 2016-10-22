#include <ipc_func.h>

int ipc_makekey(const char *name)
{
    // description:
    //   this function generates a key from a string
    // arguments:
    //   name   a string to make key from, only the 
    //          first 4 characters take on effect
    // returns:
    //   a key that can be identifies a IPC object
    
    key_t   key = 0;
    int     i, version = 1;
    int     isdigit;
    char    *p;

    if(!name) name = "VIPC";

    //determine the string 'name' is a digital one or not
    for(p=(char*)name,isdigit=1; *p; p++)
    {
        if('0' <= *p && *p <= '9') continue;
        isdigit = 0;
        break;
    } 

    if(isdigit)
    {
//        return atol(name);
        key= atol(name);
    }
    else
    {
        for(key = 0, i = 0; i < 8; ++i)
    	{
    		if(name[i] == 0)
       		     break;
     		else
                {
                    if ( i==0 )
                         key= (unsigned char )name[i]*100;
                    else if ( i==1)
                         key= key +(unsigned char)name[i]*10;   
                    else key = key + (unsigned char )name[i];
                }
    	}
    }	       
    p = getenv("ICS_SIGN");
    if(p == NULL)
         version = 0;
    else
         version = atoi(p);
/* printf( "ICS_SIGN =%s\n version=%08x\n",p,version); */
//    version <<= 28;
/* printf("version  = %08x\n",version);
printf("key=%08x\n",key); */
    key = key+version;

/* printf("back key=%08x\n",key); */
    return(key);     
}


int ipc_namefromkey(long key, char *name)
{
    char tmp_name[16], *p;
    int  i, j;
    long version;
    
    p = getenv("ICS_SIGN");
    if(p == NULL)
         version = 0;
    else
         version = atoi(p);
    version <<= 28;     
    
    key = (key & ~version);
    
    memset(tmp_name, 0, sizeof(tmp_name));    
    
    for(i=0; key > 0;)
    {
        tmp_name[i++] = key - (key >> 7) * 128;
        key = key >> 7;
    }
    
    for(j=0, i--; i >=0; i--)
        name[j++] = tmp_name[i];

    return 0;
}
