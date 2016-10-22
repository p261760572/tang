#include <ibdcs.h>

int evkey_equal( evkey_t key1, evkey_t key2 )
{
    if( key1.k_len != key2.k_len )
    {
//     dcs_debug(0,0,"key1 len=%d,key2 len=%d",key1.k_len,key2.k_len);
     return 0;
    }
        
    if( memcmp( key1.k_val, key2.k_val,  key2.k_len ) != 0 )
    {
 //    dcs_debug(0,0,"key1 val=[%s],key2 val=[%s],key2 len=[%d]",key1.k_val,key2.k_val,key2.k_len);
    return 0;
    }
    
    return 1;
}

int evkey_makekey( evkey_t *outkey, void *keydata, int keylen )
{
    if( !outkey || !keydata || keylen < 0 )
        return -1;
        
    outkey->k_len = min( keylen, EV_K_LEN );
    memcpy( outkey->k_val, keydata, outkey->k_len );
    return 0;
}
