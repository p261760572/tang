#include <stdio.h>
#include <string.h>

char *LPAD(char *src, char fillchar, int width)
{
    int len, n;
    char *p;
    
    for(len=0; src[len]; len ++)
        ;

    if(len >= width)
        return src;
    
    n = width - len;
    memmove(src+n, src,len+1);

    for(p = src; n >0; n --,p++)
        *p = fillchar;

    return src;            
}
    
char *RPAD(char *src, char fillchar, int width)
{
    int len, n;
    char *p;
    
    for(len=0; src[len]; len ++)
        ;
    if(len >= width)
        return src;
    
    n = width - len;
    for(p = src; *p; p++)
        ;
    for(; n >0; n --,p++)
        *p = fillchar;
        
    *p = 0;
        
    return src;            
}

int main()
{
    char  tmpBuf1[32] = "123456789\0";
    char  tmpBuf2[32] = "123456789\0";

    printf("%s\n", LPAD(tmpBuf1, '*',  16));
    printf("%s\n", RPAD(tmpBuf2, '*',  16));
}
