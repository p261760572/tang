#include <mbuf_func.h>

int mbuf_create(SHMOBJ *shmobj,const char *name,
                int nheader,int nbuffer,int nbufsize)
{
    int   space, i;
    int   shmid = -1;
    _sem_t semid = -1;
    char  *ptr  = NULL;
    
    //make sure all parameters valid
    if((shmobj==NULL) || (name==NULL || name[0]==0) ||
       (nheader<=0) || (nbuffer<=0) || (nbufsize<0))
    {
        errno = EINVAL;
        return -1;    
    }
    
    //calculate the size of shared memory to be created and adjust it
    //to multiple 4 bytes
    space =   sizeof(SHMCTRL) //control block
            + nheader * sizeof(SHMHDR) //header array
            + nbuffer * sizeof(SHMBUF) //buffer array
            + nbuffer * nbufsize;      //data area
    space = ((space-sizeof(long))/sizeof(long) +1 )* sizeof(long);
    
    //try creating shared memory & semaphore
    ptr = (char *)shm_create(name,space,&shmid);
    if(ptr==NULL)
        goto failure;
    if((semid = sem_create(name,1)) < 0)
        goto failure;
        
    //lock the whole memory block until initialization over
    sem_lock(semid,0,TRUE);
    
    //zero this memory block 
    memset(ptr,0,space);      
              
    //save all pointers into 'shmobj'
    if(TRUE)
    {
        int offset = 0;
        
        shmobj->shm_pShmCtrl = (SHMCTRL *)(ptr+offset);
        offset += sizeof(SHMCTRL);
        shmobj->shm_pHdrArray= (SHMHDR  *)(ptr+offset);
        offset += (nheader * sizeof(SHMHDR));
        shmobj->shm_pBufArray= (SHMBUF  *)(ptr+offset);    
        offset += (nbuffer * sizeof(SHMBUF));    
        shmobj->shm_pDataArea= (char  *)(ptr+offset);
    }
    
    //initialize the control block
    if(TRUE)
    {
        SHMCTRL  *pCtrl;
        
        pCtrl =  shmobj->shm_pShmCtrl;
        for(i=0; name[i] && i<sizeof(pCtrl->shm_name);i++)
            pCtrl->shm_name[i] = name[i]; //copy the name
        pCtrl->shm_id    = shmid;    
        pCtrl->shm_magic = SHM_MAGIC;            
        pCtrl->shm_sem   = semid;
        
        pCtrl->hdr_nalloc  = nheader;
        pCtrl->hdr_nfree   = nheader;
        pCtrl->buf_nalloc  = nbuffer;
        pCtrl->buf_nfree   = nbuffer;
        pCtrl->buf_nsize   = nbufsize;
        
        pCtrl->shm_alloctimes  = 0;
        pCtrl->shm_allocfaults = 0;
        pCtrl->shm_freetimes   = 0;
    }//initialize the control block

    //initialize the header array
    if(TRUE)
    {
        SHMHDR *pHdr;    
        
        pHdr = shmobj->shm_pHdrArray;
        for(i=0; i<nheader; i++)
        {
           pHdr[i].hdr_id   = i;
           pHdr[i].hdr_next = i+1;
           pHdr[i].hdr_buf0 = -1;    
           pHdr[i].hdr_flags= 0;               
        }
        
        pHdr[nheader-1].hdr_next = -1;
        shmobj->shm_pShmCtrl->hdr_freehead = 0;
        shmobj->shm_pShmCtrl->hdr_freetail = nheader-1;        
    }//initialize the header array

    //initialize the buffer array
    if(TRUE)
    {
        SHMBUF *pBuf;    
        
        pBuf = shmobj->shm_pBufArray;
        for(i=0; i<nbuffer; i++)
        {
           pBuf[i].buf_id   = i;
           pBuf[i].buf_next = i+1;
        }
        
        pBuf[nbuffer-1].buf_next = -1;
        shmobj->shm_pShmCtrl->buf_freehead = 0;
        shmobj->shm_pShmCtrl->buf_freetail = nbuffer-1;        
    }//initialize the buffer array
    
    //since initialization is over, unlock the whole memory 
    //block and return
    sem_unlock(semid,0,TRUE);
    return 0;
       
failure:
    if(ptr)
    {
        //clean up IPC resources before return
        shm_detach(ptr);
        shm_delete(shmid);
        if(semid >= 0)
            sem_delete(semid);    
    }
    return -1;
}

//----------------------------------------------------
                
int mbuf_connect(SHMOBJ *shmobj,const char *name)
{
    char *ptr = NULL;
    
    //make sure all parameters valid
    if(shmobj == NULL || name == NULL || name[0] == 0)   
    {
        errno = EINVAL;
        return -1;    
    }
    
    //try to connect to the shared memory block
    ptr = (char *)shm_connect(name, NULL);
    if(ptr==NULL)
        return -1;
        
    //save all pointers into 'shmobj',which is the 
    //holder of the result
    if(TRUE)
    {
        int offset = 0, nbuffer, nheader;
        
        shmobj->shm_pShmCtrl = (SHMCTRL *)(ptr+offset);
        offset += sizeof(SHMCTRL);
        shmobj->shm_pHdrArray= (SHMHDR  *)(ptr+offset);
	
	    nheader = shmobj->shm_pShmCtrl->hdr_nalloc;
        offset += (nheader * sizeof(SHMHDR));
        shmobj->shm_pBufArray= (SHMBUF  *)(ptr+offset);    

	    nbuffer = shmobj->shm_pShmCtrl->buf_nalloc;
        offset += (nbuffer * sizeof(SHMBUF));    
        shmobj->shm_pDataArea= (char  *)(ptr+offset);
    }
   
    return 0;    
}

//----------------------------------------------------

int mbuf_delete(const SHMOBJ *shmobj)
{
    if(shmobj == NULL || shmobj->shm_pShmCtrl == NULL ||
       shmobj->shm_pShmCtrl->shm_magic != SHM_MAGIC)   
    {
        errno = EINVAL;
        return -1;    
    }
    
    if(shmobj->shm_pShmCtrl)
    {
        int  shmid;
        
        //clean up all IPC resources
        shmid = shmobj->shm_pShmCtrl->shm_id;
        sem_delete(shmobj->shm_pShmCtrl->shm_sem);
        shm_detach((char *)shmobj->shm_pShmCtrl);
        shm_delete(shmid);
        return 0;
    }
    
    return -1;   
}
