
#include "iso8583.h"
#include "def8583.h"
#include <string.h>
int setbit_t(ISO_data *, int ,unsigned char *, int );

struct  trans_bitmap_struc  g_sIsoData;
int  strtoiso (unsigned char *  dstr , ISO_data *iso , int  strlen )
{
	
unsigned char  *rpt, bitmask ;
unsigned char i , j , n , bitnum ;
unsigned char tmpbuf[1000],bitmap[32];
char buf[20];
int           off , len , k,msgtypelen ;


  if ( iso == NULL) return -1;
	if ( iso->iso8583 == NULL)
	     return -1;
	if ( dstr == NULL ) return -1;
  if ( strlen <10) return -1;
	/* Initialize the iso->f[].bitf */
    for ( i = 0 ; i < 128 ; i ++ )
            iso -> f [ i ].bitf = 0 ;
   off = 0 ;
/*   dcs_debug(0,0,"Begin unpaket "); */
	/* Get Message ID */
	
	if ( iso->head_flag )
	{
		if ( iso->headlen >49  || iso->headlen < 0) return -1;
		if( iso->headlen >strlen ) return -1;
		 memcpy( iso->head_data ,dstr,iso->headlen);
/*		 dcs_debug(dstr,strlen,"dstr data ,strlen=%d",strlen); */
		 strlen = strlen - iso->headlen;
		 dstr = dstr + iso->headlen;
/*		 dcs_debug(dstr,strlen,"dstr data,strlen=%d",strlen); */
	}
	if (! iso->msgid_flag )
  {
  	  bcd_to_asc ( (unsigned char*)iso -> message_id , dstr , 4 , 0 ) ;
  	  strlen = strlen -2;
  	  
      msgtypelen =2;
  }
  else
  {
  	    memcpy(iso -> message_id,dstr,4);
  	    strlen =strlen -4;
  	  
        msgtypelen =4;
  }
     
  iso -> message_id [ 4 ] = '\0' ;
/*  if (! iso->msgid_flag )
  		dcs_debug(dstr,2,"msgtype ");
  else
  		dcs_debug(dstr,4,"msgtype ");
*/
  if ( !iso->bitmap_flag )
	{
		if ( dstr [ msgtypelen ] & 0x80 )
            bitnum = 16 ;
      else
            bitnum = 8 ;
	   strlen = strlen -bitnum;
	  if ( strlen <0)
	  {
	  	  dcs_log( 0, 0," no data, Can not succ unpacket!");
	  		return -1;
		}
		/* initialize <rpt> point to data elements */
		
	  rpt = dstr + msgtypelen + bitnum ;
	  memcpy(bitmap,dstr+msgtypelen,bitnum);
/*	  dcs_debug(dstr+msgtypelen,bitnum,"bit map bitnum=%d",bitnum); */
  }
  else
  {
  	
  	asc_to_bcd(bitmap,dstr+msgtypelen,2,0);
  	if ( bitmap[0]&0x80)
  	{
  		    asc_to_bcd(bitmap,dstr+msgtypelen,32,0);
  		    bitnum = 16 ;
  	}
    else
    {
    	    asc_to_bcd(bitmap,dstr+msgtypelen,16,0);
    	    bitnum = 8 ;
    }
	   strlen = strlen -bitnum*2;
	  if ( strlen <0)
	  {
	  	  dcs_log( 0, 0,"no data ! can not succ unpacket");
	  		return -1;
		}
		/* initialize <rpt> point to data elements */
		
	  rpt = dstr + msgtypelen + bitnum*2 ;
/*	  dcs_debug(dstr+msgtypelen,bitnum*2,"bit map bitnum=%d",bitnum); */
  }
/*  dcs_debug(rpt,20,"rpt data,iso->headlen=%d",iso->headlen); */
	
	/* copy dstr elements to iso */
    for ( i = 0 ; i < bitnum ; i ++ ) 
    {
        bitmask = 0x80 ;
        for ( j = 0 ; j < 8 ; j ++ , bitmask >>= 1 ) 
        {
            if ( i == 0 && bitmask == 0x80 )
                    continue ;
//            if ( ( dstr [ i + msgtypelen ] & bitmask ) == 0 )
            if ( ( bitmap [ i ] & bitmask ) == 0 )
                    continue ;
            n = ( i << 3 ) + j ;

			/* process variable length data elements */
            if ( iso->iso8583 [ n ].flag > 0 ) 
            {
            	  if( !iso->var_flag )
            	  {
			                len = ( * rpt ) - ( ( * rpt ) >> 4 ) * 6 ;
			                rpt ++ ;
			                if ( iso->iso8583 [ n ].flag == 2 ) 
			                {
			                    len = len * 100 + ( * rpt ) - ( * rpt >> 4 ) * 6 ;
			                    rpt ++ ;
											}
								}
								else
								{
//									  len = ((( *rpt )<<4)>>4)*10+ (*(rpt+1)<<4)>>4;
//									  rpt =rpt+2;
									  if ( iso->iso8583 [ n ].flag == 2 ) 
									 	{
//									 				len =len *10 +(*rpt)&0x0f;
//									 		  	rpt++;
									 		  	memset(buf,0,sizeof(buf));
									 		  	memcpy(buf,rpt,3);
									 		  	len  = atoi(buf);
									 		  	rpt = rpt+3;
									 		  	strlen = strlen -3;
/*									 		  	dcs_debug(rpt-3,3,"LLLvar Data,len =%d",len);  */
									 	 }
									 	 else
									 	 {
									 	 	  	memset(buf,0,sizeof(buf));
									 		  	memcpy(buf,rpt,2);
									 		  	len  = atoi(buf);
									 		  	rpt = rpt+2;
									 		  	strlen = strlen -2;
/*									 		  	dcs_debug(rpt-2,2,"LLvar Data,len =%d",len);  */
									 	}
								}
                if ( len > iso->iso8583 [ n ].len ) 
                {
                    dcs_log(0,0,"can not analyse field = %d len=%d ,fix len= %d",n+1,len,iso->iso8583[n].len); 
										return -1 ;
								}
            }
            else
                len = iso->iso8583 [ n ].len ;
            if (len <0 || len >999) 
            {
            		return -1;
            }
            iso -> f [ n ].len = len ;
            iso -> f [ n ].dbuf_addr = off ;

            
            /* BCD field Bytes=len/2 */
            if ( iso->iso8583 [ n ].type & 0x07 ) 
            {
                    len ++ ;
                    len >>= 1 ;
            }

            /* Copy to iso buffer */
            if ( len + off >= 2048 )
                    return ( -1 ) ;
				    strlen = strlen -len ;
				    if ( strlen <0)
				    {
				    	  dcs_log(0,0," Can not resolve field %d  ! len invalid len = %d",n+1,len);
				    		return -1;
				    }
/*			    dcs_debug(rpt,len,"field %d=%d",n+1,len); */
				    memset(tmpbuf,0,sizeof(tmpbuf));
				    if ( iso->iso8583 [ n ].type & 0x07 ) 
				    {
/*				    	    dcs_debug(0,0," flag =%d , type=%d,",iso->iso8583 [ n ].type & 0x03,iso->iso8583 [ n ].type); */
				    			bcd_to_asc(tmpbuf,rpt,iso -> f [ n ].len,iso->iso8583 [ n ].type & 0x03);
				    }
				    else 
				    {
				    			memcpy ( tmpbuf , rpt , iso -> f [ n ].len ) ;
				  	}
		
				   
            for ( k = 0 ; k < iso -> f [ n ].len ; k ++ )
          			iso -> dbuf [ off ++ ] =  tmpbuf[k] ; 	  
          	
          	rpt = rpt + len ;  
/*            for ( k = 0 ; k < len ; k ++ )
                    iso -> dbuf [ off ++ ] = * rpt ++ ;
 */
            iso -> f [ n ].bitf = 1 ;
				}              
	 	}                  
    iso -> off = off ; 
    return 0 ;         
}                      
                       
                       
int                    
isotostr ( dstr , iso )
unsigned char *dstr ;
ISO_data      *iso ;
{
unsigned char   *wpt , bitmask , bitmap ;
int             k , lc ;
unsigned char   i , j , n , bitnum ,msgtypelen;

   /* dcs_debug(0,0,"begin pakcet !"); */
     if ( iso->iso8583 == NULL)
	     iso->iso8583 = &iso8583_def[0];
	
     /*dcs_debug(iso,sizeof(ISO_data),"begin pakcet --1 !"); */
        
	  if ( iso->head_flag )
		{

            /*dcs_debug(0,0,"begin pakcet %d(%d)---1 !", iso->head_flag,iso->headlen); */
			 if ( iso->headlen < 0 || iso->headlen > 50) return -1;

      /*dcs_debug(0,0,"begin pakcet ---2 !"); */
			 memcpy( dstr,iso->head_data ,iso->headlen);
			 dstr = dstr + iso->headlen;
		}
	
      /*dcs_debug(0,0,"begin pakcet --2 !"); */

     if ( !iso->msgid_flag)
    			asc_to_bcd ( dstr ,(unsigned char*) iso -> message_id , 4 , 0 ) ;
     else
					memcpy ( dstr,iso -> message_id , 4 ) ;
    for ( lc = 64 , bitnum = 8 ; lc < 128 ; lc ++ )  /*扩展bitmap*/
        if ( iso -> f [ lc ].bitf ) 
        {
            bitnum = 16 ;
            break ;
	      }
	   if ( !iso->msgid_flag )
	   	   	  msgtypelen =2;
	   else
       		msgtypelen =4;
     if ( iso->bitmap_flag)
     		wpt = dstr + msgtypelen + bitnum*2 ;    /* wpt是指向bitmap后具体内容的指针 */
     else
     		wpt = dstr + msgtypelen + bitnum ;    /* wpt是指向bitmap后具体内容的指针 */

   /*  dcs_debug(dstr,msgtypelen,"msgtype");*/
     
	/* copy iso elements to dstr */
    for ( i = 0 ; i < bitnum ; i ++ ) 
    {
        bitmap = 0 ;
        bitmask = 0x80 ; /*变量bitmap表示8个位图域*/
        for ( j = 0 ; j < 8 ; j ++ , bitmask >>= 1 ) 
        {
            n = ( i << 3 ) + j ;
            if ( iso -> f [ n ].bitf == 0 )
                continue ;
            bitmap |= bitmask ;

            if ( iso->iso8583[ n ].flag == 0 )
            {
		             lc = iso->iso8583[n]. len;
            }
            else
            {
            	  if ( iso->iso8583[n]. len >= iso -> f [ n ].len )
		            		lc = iso -> f [ n ].len ;
		            else
		            		lc = iso->iso8583[n]. len;
            }
            if ( iso->iso8583 [ n ].flag == 1 ) 
            {   if ( !iso->var_flag )
                		( * wpt ++ ) = ( unsigned char ) ( ( lc % 10 ) + ( lc / 10 ) * 16 ) ;
                else
                {
               		 	sprintf((char *) wpt,"%02d",lc);
               		 	wpt=wpt+2;
/*               		 	dcs_debug(wpt-2,2,"lc = %d f[%d] = %d ,iso8583[%d]=%d",lc,n+1,iso->f[n].len,n+1,iso8583[n].len);*/
                }		 
                
            }
            else if ( iso->iso8583 [ n ].flag == 2 ) 
            {
            	  if ( ! iso->var_flag )
            	  {
            	  	
                			( * wpt ++ ) = ( unsigned char ) ( lc / 100 ) ;
                			( * wpt ++ ) = ( unsigned char ) ( ( ( lc % 100 ) / 10 ) * 16 + ( lc % 100 ) % 10 ) ;
                }
                else
                {
                		 	sprintf((char *) wpt,"%03d",lc);
               		 		wpt=wpt+3;
/*               		 		dcs_debug(wpt-3,3,"lc = %d",lc);*/
              	}
						}       /* 存放VARII,VARIII型长度 */

            k = 0 ;
/*            if ( iso8583 [ n ].type & 0x01 ) 
            
                ( * wpt ++ ) = iso -> dbuf [ iso -> f [ n ].dbuf_addr + k ] ;    
                k ++ ;
                lc ++ ;
						}       
						
*/          
/*            dcs_debug(&iso -> dbuf [ iso -> f [ n ].dbuf_addr],lc,"bit[%d] len= %d,iso->iso8583 [ n ].type=%d",n+1,lc,iso->iso8583 [ n ].type); */
            if ( iso->iso8583 [ n ].type & 0x07 ) 
            {                /* n or z BCD型数据*/
/*                dcs_debug(&iso -> dbuf [ iso -> f [ n ].dbuf_addr],lc,"bit[%d] len= %d,iso->iso8583 [ n ].type=%d",n+1,lc,iso->iso8583 [ n ].type); */
                asc_to_bcd(wpt ,(unsigned char *)&(iso -> dbuf [ iso -> f [ n ].dbuf_addr]),lc,  iso->iso8583 [ n ].type & 0x03); 
//                asc_to_bcd(wpt ,(unsigned char *)&(iso -> dbuf [ iso -> f [ n ].dbuf_addr]),lc,  0); 
                lc ++ ;
                lc >>= 1 ;           /* lc = (lc+1)/2 */
                wpt = wpt+lc;
						}
            else 
            {
            	for ( ; k < lc ; k ++ ) 
	            {
	                ( * wpt ++ ) = iso -> dbuf [ iso -> f [ n ].dbuf_addr + k ] ;    /* C or D */
							}
						}
/*						dcs_debug(wpt -lc,lc,"bit[%d] data:",n+1); */
		    }
		    if ( iso->bitmap_flag)
		    {
		    	dstr [ i*2 + msgtypelen ] = ((bitmap>>4)& 0x0f)+0x30 ;
		    	dstr [ i*2+1 + msgtypelen ] = (bitmap & 0x0f) +0x30 ;
		    }
		    else	
        	dstr [ i + msgtypelen ] = bitmap ;
	}
   if ( iso -> f [ 127 ].bitf && bitnum == 8 ) {  
/*
        dstr [ bitnum + 1 ] |= 0x01 ;
        memcpy ( wpt , &iso -> dbuf [ iso -> f [ 127 ].dbuf_addr ] , iso8583[127].len ) ;
        wpt += iso8583[127].len ;
        dcs_debug(wpt-iso8583[127].len,iso8583[127].len,"bit 64 data");
*/
	}
	
    if ( bitnum == 16 )
    {
    	   if ( iso ->bitmap_flag)
    	   	  dstr [ msgtypelen ] |=0x38;
    	   else
    	   		dstr [ msgtypelen ] |= 0x80 ;        
    }
/*    if ( iso ->bitmap_flag)
    	 dcs_debug(dstr+msgtypelen,bitnum*2,"bit map");
    else        
       dcs_debug(dstr+msgtypelen,bitnum,"bit map");
*/    
  if ( iso->head_flag )
  	return wpt - dstr+iso->headlen ;
  else
    return wpt - dstr ;
}

int setbit_t ( iso , n , str , len )
int         n ;
ISO_data    *iso ;
int         len ;
unsigned char *str ;
{
int             i , l ;
unsigned char   *pt , tmp [ 1024 ] ;

    if ( len == 0 && n>1)
    {
    		iso -> f [ n-1 ].bitf = 0;
    		iso->f[n-1].len = 0;
        return 0 ;
    }
    if ( iso->iso8583 == NULL)
	     iso->iso8583 = &iso8583_def[0];
    if ( n == 0 ) 
    {
        memcpy ( iso -> message_id , str , 4 ) ;
        iso -> message_id [ 4 ] = '\0' ;
        iso -> f [ n ].dbuf_addr = -1;
        return 0 ;
	  }
    if ( n <= 1 || n > 128 )
        return -1 ;

/*    if ( n == 64 )
        n = 128 ;
  */
    n -- ;
    if ( len > iso->iso8583 [ n ].len )
        len = iso->iso8583 [ n ].len ;

    iso -> f [ n ].bitf = 1 ;
    l = len ;

    if ( iso->iso8583 [ n ].flag == 0 )
        len = iso->iso8583 [ n ].len ;

    iso -> f [ n ].len = len ;
    pt = (unsigned char*)& ( iso -> dbuf [ iso -> off ] ) ;
    iso -> f [ n ].dbuf_addr = iso -> off ;

    if ( len + iso -> off >= 2048 )
        return ( -1 ) ;

    iso -> off += len ;

    if ( iso->iso8583 [ n ].type & 0x01 ) {                   
        * ( pt ++ ) = * ( str ++ ) ;
        ( iso -> off ) ++ ;
	  }

    i = 0 ;
    if ( iso->iso8583 [ n ].type & 0x03 )                     
        for ( ; i < len - l ; i ++ )
            tmp [ i ] = '0' ;
    memcpy ( tmp + i , str , l ) ;
    i += l ;
    if ( iso->iso8583 [ n ].type & 0x08 )                     
        for ( ; i < len ; i ++ )
            tmp [ i ] = 0 ;
    else
        for ( ; i < len ; i ++ )
            tmp [ i ] = ' ' ;         /* z or others */
 /*   if ( iso8583 [ n ].type & 0x07 )
        asc_to_bcd ( pt , tmp , len , iso8583 [ n ].type & 0x03 ) ;
    else
*/
    memcpy ( pt , tmp , len ) ;

    return 0 ;
}


void 
setbitoff(ISO_data *iso ,int n)
{
        iso -> f [ n-1 ].bitf = 0 ;
    return ;
}

int
getbit ( iso , n , str )
ISO_data    *iso ;
int         n ;
unsigned char *str ;
{
int             j ;
unsigned char   *pt ;

    if ( n == 0 ) {
        memcpy ( str , iso -> message_id , 4 ) ;
        str [ 4 ] = '\0' ;
        return 4 ;
	}
	if ( iso->iso8583 == NULL)
	     iso->iso8583 = &iso8583_def[0];
    if ( n <= 1 || n > 128 )
        return 0 ;
    n -- ;
    if ( iso -> f [ n ].bitf == 0 ) {
        if ( n == 127 )
            if ( iso -> f [ 63 ].bitf == 0 )
                return 0 ;
        str [ 0 ] = 0 ;
        return 0 ;
	}
    pt = (unsigned char*)&iso -> dbuf [ iso -> f [ n ].dbuf_addr ] ;
    j = iso -> f [ n ].len ;
//    if ( iso->iso8583 [ n ].type & 0x01 )
//        * str ++ = * pt ++ ;
//    if ( iso->iso8583 [ n ].type & 0x07 )
//        bcd_to_asc ( str , pt , j , iso->iso8583 [ n ].type & 0x03 ) ;
//        bcd_to_asc ( str , pt , j , 0 ) ;
 //   else
        memcpy ( str , pt , j ) ;
    str [ j ] = '\0' ;

    return j ;
}

void
clearbit ( iso )
ISO_data *iso ;
{
int     i ;

    for ( i = 0 ; i < 128 ; i ++ )
        iso -> f [ i ].bitf = 0 ;
    iso -> off = 0 ;

    iso->head_flag =0;
    iso->headlen=0;

    return ;
}

void asc_to_bcd ( unsigned char* bcd_buf , unsigned char* ascii_buf , int conv_len ,unsigned char type )
{
int         cnt ;
char        ch , ch1 ;

    if ( conv_len & 0x01 && type )
        ch1 = 0 ;
    else
        ch1 = 0x55 ;

    for ( cnt = 0 ; cnt < conv_len ; ascii_buf ++ , cnt ++ ) 
    {
        if ( *ascii_buf >= 'a' )
            ch = *ascii_buf - 'a' + 10 ;
        else if ( *ascii_buf >= 'A' )
            ch = *ascii_buf- 'A' + 10 ;
        else if ( *ascii_buf >= '0' )
            ch = *ascii_buf-'0' ;
        else
            ch = 0;
        if ( ch1 == 0x55 )
            ch1 = ch ;
				else {
            *bcd_buf ++ = ch1 << 4 | ch ;
            ch1 = 0x55 ;
				}
		}
    if ( ch1 != 0x55 )
        *bcd_buf = ch1 << 4 ;

    return ;
}


void bcd_to_asc (unsigned char* ascii_buf ,unsigned char* bcd_buf , int conv_len ,unsigned char type )
{
    int         cnt ;

    if ( conv_len & 0x01 && type ) {
            cnt = 1 ;
            conv_len ++ ;
    }
    else
            cnt = 0 ;
    for ( ; cnt < conv_len ; cnt ++ , ascii_buf ++ ) {
        *ascii_buf = ( ( cnt & 0x01 ) ? ( *bcd_buf ++ & 0x0f ) : ( *bcd_buf >> 4 ) ) ;
        *ascii_buf += ( ( *ascii_buf > 9 ) ? ( 'A' - 10 ) : '0' ) ;
    }

    return ;
}



int SetIsoHeardFlag( ISO_data *iso ,int  type )
{
	if ( type )
			iso->msgid_flag = 0x01;
	else
			iso->msgid_flag = 0x00;
	return 1;
}

int GetIsoHeardFlg(ISO_data *iso )
{
	return iso->msgid_flag;
}

int SetIsoFieldLengthFlag (ISO_data *iso , int type )
{
	if ( type )
			iso->var_flag = 0x01;
	else
			iso->var_flag = 0x00;
			
	return 1;
}
int GetFieldLeagthFlag(ISO_data *iso )
{
	return iso->var_flag;
}

int SetIsoBitmapFlag (ISO_data *iso , int type )
{
	if ( type )
			iso->bitmap_flag = 0x01;
	else
			iso->bitmap_flag = 0x00;
			
	return 1;
}
int GetBitmapFlag(ISO_data *iso )
{
	return iso->bitmap_flag;
}
/*
int CheckField( int n, char * tmpbuf ,int len)
{
		    switch ( iso8583 [ n ].fmt )
				    {
				    	    case      FMT_MMDDHHMMSS   :
									case			FMT_YYMMDDHHMMSS :
									case			FMT_YYMM         : 
									case			FMT_YYYYMMDD     :
									case			FMT_YYMMDD       :
									case			FMT_MMDD         :
									case			FMT_HHMMSS       :
										        if ( ! _IsDateTime(tmpbuf, len , iso8583 [ n ].fmt  ))
										        {
										        			dcs_log(tmpbuf,len," field %d is invalid format!", n);
										        			return -1;
										        } 
										        break ;
									case			FMT_MONEY        :
									case			FMT_N            :
										        
										        if ( ! _IsNumericString( tmpbuf,len ) )
										        {
										        			dcs_log(tmpbuf,len," field %d is invalid format!", n);
										        			return -1;
										        }
										        break;
									
									case			FMT_A            :
									case			FMT_AN           :
									case			FMT_ANS          :
										        if ( ! _IsHexString( tmpbuf, len ))
										        {
										        			dcs_log(tmpbuf,len," field %d is invalid format!", n);
										        			return -1;
										        }
										        break;
									case			FMT_B            :
									case			FMT_Z            :
									default :
										        break;
             }
             
             return 0;
}

int SetBitValue( int i , char *buf , int len)
{
	  
	  if ( i <0 || i >128) return -1;
	  if ( len ==0)
	  {
	  		g_sIsoData.element[i].flag=0;
			  g_sIsoData.element[i].len=0;
			  g_sIsoData.element[i].pointer=0;
		    return 0;	  
			  
	  }
	  if (buf == NULL) return -1;
	  if ( len <0 ) return -1;
	  if ( (len + g_sIsoData.off) >=2048) return -1;
	 
	  
	  {
	  	g_sIsoData.element[i].flag=1;
	  	g_sIsoData.element[i].len=len;
	  	g_sIsoData.element[i].pointer=g_sIsoData.off;
	  	memcpy(g_sIsoData.buf+g_sIsoData.off,buf,len);
			g_sIsoData.off = g_sIsoData.off +len;
			
	  }
	  return 0;
}

void ClearAllBitValues()
{
	int i;
	
	for ( i=0; i<129; i++)
	{
			g_sIsoData.element[i].flag=0;
			g_sIsoData.element[i].len=0;
			g_sIsoData.element[i].pointer=0;
	}
	memset(g_sIsoData.buf,0,sizeof(g_sIsoData.buf));
	
} 
int GetBitValue( int i, char *buf)
{
	 if (buf == NULL) return -1;
	  if ( i <0 || i >128) return -1;
	 
	  memcpy(buf,g_sIsoData.buf+g_sIsoData.element[i].pointer,g_sIsoData.element[i].len);
	  return g_sIsoData.element[i].len;
}
*/

void change_tpdu(ISO_data *iso)
{
char   tmpbuf[6];
	
	if ( iso == NULL ) return ;
	if ( iso->head_flag )
	{
		 if ( iso->headlen >=5 )
		 {
		 	 memcpy(tmpbuf,iso->head_data+1,2);
		 	 memcpy(iso->head_data+1,iso->head_data+3,2);
		 	 memcpy(iso->head_data+3,tmpbuf,2);
		 }
	}
}

int get_msghead(ISO_data *iso,char *caData)
{
	if ( iso == NULL ) return  -1;
	if ( iso->head_flag )
	{
		 if ( iso->headlen >=1 )
		 {
		 	 memcpy(caData,iso->head_data,iso->headlen);
		 	 return iso->headlen;
		 }
		 else 
		 	 return -1;
	}
	else
		return -1;
}

int set_msghead(ISO_data *iso,char *caData ,int len)
{
	if ( iso == NULL ) return  -1;
	
		 if ( len >=30 || len <1 ) return -1;
		 iso->head_flag =1;
		 memcpy(iso->head_data,caData,len);
		 iso->headlen=len;
		 return len;
}

int
isbitset (ISO_data *iso ,int n)
{
        return(iso -> f [ n-1 ].bitf) ;
}

