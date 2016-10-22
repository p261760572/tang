#include <ipc_func.h>

char *fmap_create(char *mappedfile, int nsize)
{
    int fd    = -1;
    char *ptr = NULL;
    
    if(mappedfile==NULL || mappedfile[0]==0)
        return NULL;
        
    fd = open(mappedfile, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if(fd < 0)
      return NULL;
      
    lseek(fd, nsize-1, SEEK_SET);
    write(fd, "", 1);
   
    ptr = mmap(NULL,nsize,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == (char *)-1)
    {
        close(fd);
        return NULL;
    }    

    memset(ptr,0,nsize);
    close(fd);
    return ptr;
}

char *fmap_connect(char *mappedfile, int nsize)
{
    int fd    = -1;
    char *ptr = NULL;
    
    if(mappedfile==NULL || mappedfile[0]==0)
        return NULL;
        
    fd = open(mappedfile, O_RDWR, 0666);
    if(fd < 0)
      return NULL;

    ptr = mmap(NULL,nsize,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == (char *)-1)
    {
        close(fd);
        return NULL;
    }    

    close(fd);
    return ptr;
}

int fmap_unmap(char *pAddr, int nsize)
{
    return munmap(pAddr,nsize);
}    

