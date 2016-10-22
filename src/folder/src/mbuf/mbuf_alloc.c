#include <mbuf_func.h>

int mbuf_alloc(const SHMOBJ *shmobj,int nsize)
{
    SHMCTRL *pCtrl     = NULL;
    SHMHDR  *pHdr      = NULL;
    SHMBUF  *pBuf      = NULL;
    int      cluster_id = -1;
    int      nneed, first_buf, last_buf, i;
    
    //check all parameters
    if(shmobj == NULL || shmobj->shm_pShmCtrl == NULL ||
       shmobj->shm_pShmCtrl->shm_magic != SHM_MAGIC || 
       nsize < 0)   
    {
        errno = EINVAL;
        return -1;    
    }
    
    pCtrl = shmobj->shm_pShmCtrl;
    pHdr  = shmobj->shm_pHdrArray;
    pBuf  = shmobj->shm_pBufArray;
            
    //calculate how many buffers needed
    nneed = nsize / pCtrl->buf_nsize;
    if(nsize % pCtrl->buf_nsize) nneed ++;
    
    //lock the whole memory block
    mbuf_lock(shmobj);

    if(pCtrl->hdr_nfree < 1 || pCtrl->buf_nfree < nneed)
    { 
        //no enough resource,allocation failed
        pCtrl->shm_allocfaults ++;        
        mbuf_unlock(shmobj);        
        errno = ENOMEM;
        return -1;
    }    
        
    //pick off the first header from free head link
    cluster_id = pCtrl->hdr_freehead;
    pCtrl->hdr_freehead = pHdr[cluster_id].hdr_next;    
    if(pCtrl->hdr_freehead < 0) //link already empty
        pCtrl->hdr_freetail = -1;
    pHdr[cluster_id].hdr_next = -1; //isolate this header
    
    if(nsize > 0)
    {
        //pick off the first 'nneed' bufferes 
        //from free buffer link

        last_buf = first_buf = pCtrl->buf_freehead;
        for(i=1; i < nneed; i++)
            last_buf = pBuf[last_buf].buf_next;
        pCtrl->buf_freehead = pBuf[last_buf].buf_next;
        if(pCtrl->buf_freehead < 0) //link already empty
        pCtrl->buf_freetail = -1;
        pBuf[last_buf].buf_next = -1; 
    }
    else
        first_buf = last_buf = -1;
    
    //build the allocated cluster
    pHdr[cluster_id].hdr_buf0 = first_buf;
    pHdr[cluster_id].hdr_nbuf = nneed;
    pHdr[cluster_id].hdr_nbyte= 0;
    pHdr[cluster_id].hdr_flags |= SLOT_USED;
            
    //update the resource info
    pCtrl->hdr_nfree -= 1;
    pCtrl->buf_nfree -= nneed;    

    //update the statisics
    pCtrl->shm_alloctimes ++;
    i = nsize / 50;
    if( 0 <= i && i<100)
        pCtrl->shm_statArray[i] ++;
    else
        pCtrl->shm_statArray[100] ++;
    
    //unlock the whole memory block
    mbuf_unlock(shmobj);

    //return the fruit to caller
    return cluster_id;
}

//----------------------------------------------------

int mbuf_free(const SHMOBJ *shmobj,int cluster_id)
{
    SHMCTRL *pCtrl     = NULL;
    SHMHDR  *pHdr      = NULL;
    SHMBUF  *pBuf      = NULL;
    int     first_buf, last_buf, i;
    
    if(shmobj == NULL || shmobj->shm_pShmCtrl == NULL ||
       shmobj->shm_pShmCtrl->shm_magic != SHM_MAGIC)
    {
        errno = EINVAL;
        return -1;    
    }
    
    pCtrl = shmobj->shm_pShmCtrl;
    pHdr  = shmobj->shm_pHdrArray;
    pBuf  = shmobj->shm_pBufArray;
    
    //check the validity of the cluster_id 
    //specified by 'cluster_id'
    if(cluster_id <0 || cluster_id >= pCtrl->hdr_nalloc)
        return -1;
    if(pHdr[cluster_id].hdr_id != cluster_id ||
       !(pHdr[cluster_id].hdr_flags & SLOT_USED))
        return -1;   
        
    //lock the whole memory block
    mbuf_lock(shmobj);

    if(pHdr[cluster_id].hdr_buf0 >= 0)
    {
        //return back the buffers link associated with the cluster
        last_buf = first_buf = pHdr[cluster_id].hdr_buf0;
        for(i=1; i< pHdr[cluster_id].hdr_nbuf; i++)
        {
            pBuf[last_buf].buf_nbyte =0;
            last_buf = pBuf[last_buf].buf_next;
        }    
    
        if(pCtrl->buf_nfree <= 0) //link is empty
        {
            pCtrl->buf_freehead = first_buf;
            pCtrl->buf_freetail = last_buf;
        }
        else
        {
            pBuf[pCtrl->buf_freetail].buf_next = first_buf;    
            pCtrl->buf_freetail = last_buf;
        }
    
        pCtrl->buf_nfree += pHdr[cluster_id].hdr_nbuf;     
    }
    
    //return back the header
    pHdr[cluster_id].hdr_next = -1;
    pHdr[cluster_id].hdr_buf0 = -1;
    pHdr[cluster_id].hdr_nbuf = 0;
    pHdr[cluster_id].hdr_nbyte= 0;
    pHdr[cluster_id].hdr_flags &= (~SLOT_USED); 
    
    if(pCtrl->hdr_nfree <= 0) //link is empty
        pCtrl->hdr_freehead = pCtrl->hdr_freetail = cluster_id;
    else
    {
        pHdr[pCtrl->hdr_freetail].hdr_next = cluster_id;
        pCtrl->hdr_freetail =  cluster_id;   
    }
    
    pCtrl->hdr_nfree += 1;
    
    //update statistics
    pCtrl->shm_freetimes ++;
    
    //unlock the whole memory block and return
    mbuf_unlock(shmobj);
    return 0;
}
