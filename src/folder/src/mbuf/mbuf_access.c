#include <mbuf_func.h>

int mbuf_putdata(const SHMOBJ *shmobj,int cluster_id,
                 const void *data, int nbyte)
{
    SHMCTRL *pCtrl     = NULL;
    SHMHDR  *pHdr      = NULL;
    SHMBUF  *pBuf      = NULL;
    char    *dest, *src;
    int     nwritten, curp, nput;
    
    if(shmobj == NULL || shmobj->shm_pShmCtrl == NULL ||
       shmobj->shm_pShmCtrl->shm_magic != SHM_MAGIC || 
       data == NULL || nbyte < 0)
    {
        errno = EINVAL;
        return -1;    
    }
    
    pCtrl = shmobj->shm_pShmCtrl;
    pHdr  = shmobj->shm_pHdrArray;
    pBuf  = shmobj->shm_pBufArray;
    
    //check the validity of the cluster 
    //specified by 'cluster_id'
    if(cluster_id <0 || cluster_id >= pCtrl->hdr_nalloc)
        return -1;
    if(pHdr[cluster_id].hdr_id != cluster_id ||
       (!(pHdr[cluster_id].hdr_flags & SLOT_USED)) ||
       (pHdr[cluster_id].hdr_buf0 < 0))
        return -1;   
    
    //put the data from caller's space into shared memory    
    curp = pHdr[cluster_id].hdr_buf0;
    dest = shmobj->shm_pDataArea + pBuf[curp].buf_id * pCtrl->buf_nsize;
    src  = (char *)data;
    
    for(nwritten=0; nbyte > 0;)    
    {
      nput = min(nbyte, pCtrl->buf_nsize);	
      memcpy(dest,src,nput);
      
      nwritten += nput;
      nbyte    -= nput;
      src      += nput;	
      
      //move to next buffer
      pBuf[curp].buf_nbyte = nput;
      curp = pBuf[curp].buf_next;
      if(curp < 0)
        break;
      
      dest = shmobj->shm_pDataArea + 
             pBuf[curp].buf_id * pCtrl->buf_nsize;           
    }//for
    
    pHdr[cluster_id].hdr_nbyte = nwritten;
    return nwritten;
}

//----------------------------------------------------
                 
int mbuf_countbytes(const SHMOBJ *shmobj,int cluster_id)
{
    SHMCTRL *pCtrl     = NULL;
    SHMHDR  *pHdr      = NULL;
    
    if(shmobj == NULL || shmobj->shm_pShmCtrl == NULL ||
       shmobj->shm_pShmCtrl->shm_magic != SHM_MAGIC)
    {
        errno = EINVAL;
        return -1;    
    }
    
    pCtrl = shmobj->shm_pShmCtrl;
    pHdr  = shmobj->shm_pHdrArray;
    
    //check the validity of the cluster 
    //specified by 'cluster_id'
    if(cluster_id <0 || cluster_id >= pCtrl->hdr_nalloc)
        return -1;
    if(pHdr[cluster_id].hdr_id != cluster_id ||
       (!(pHdr[cluster_id].hdr_flags & SLOT_USED)))
        return -1;   
    
    return pHdr[cluster_id].hdr_nbyte;    
}

//----------------------------------------------------

int mbuf_getdata(const SHMOBJ *shmobj,int cluster_id,
                 void *buffer, int nsize)
{
    SHMCTRL *pCtrl     = NULL;
    SHMHDR  *pHdr      = NULL;
    SHMBUF  *pBuf      = NULL;
    char    *dest, *src;
    int     nread, curp, nget;
    
    if(shmobj == NULL || shmobj->shm_pShmCtrl == NULL ||
       shmobj->shm_pShmCtrl->shm_magic != SHM_MAGIC || 
       buffer == NULL || nsize <= 0)
    {
        errno = EINVAL;
        return -1;    
    }
    
    pCtrl = shmobj->shm_pShmCtrl;
    pHdr  = shmobj->shm_pHdrArray;
    pBuf  = shmobj->shm_pBufArray;
    
    //check the validity of the cluster 
    //specified by 'cluster_id'
    if(cluster_id <0 || cluster_id >= pCtrl->hdr_nalloc)
        return -1;
    if(pHdr[cluster_id].hdr_id != cluster_id ||
       (!(pHdr[cluster_id].hdr_flags & SLOT_USED)) ||
       (pHdr[cluster_id].hdr_buf0 < 0))
        return -1;   
    
    //put the data from caller's space into shared memory    
    curp = pHdr[cluster_id].hdr_buf0;
    src = shmobj->shm_pDataArea + pBuf[curp].buf_id * pCtrl->buf_nsize;
    dest  = (char *)buffer;
    
    for(nread=0; nsize > 0;)    
    {
      nget = min(nsize, pBuf[curp].buf_nbyte);	
      memcpy(dest,src,nget);
      
      nread    += nget;
      nsize    -= nget;
      dest     += nget;	
      
      //move to next buffer
      curp = pBuf[curp].buf_next;
      if(curp < 0)
        break;
      
      src = shmobj->shm_pDataArea + 
             pBuf[curp].buf_id * pCtrl->buf_nsize;           
    }//for
    
    return nread;    
}                 
