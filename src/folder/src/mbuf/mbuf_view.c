#include <mbuf_func.h>

int mbuf_view_ctrlblk(const SHMOBJ *shmobj, FILE *fp_out)
{
     SHMCTRL *pCtrl;

    if(shmobj == NULL || shmobj->shm_pShmCtrl == NULL ||
       shmobj->shm_pShmCtrl->shm_magic != SHM_MAGIC)
    {
        errno = EINVAL;
        return -1;
    }

    if(!fp_out)
        fp_out = stdout;

    pCtrl = shmobj->shm_pShmCtrl;

    fprintf(fp_out,"[shared memory]\n");
    fprintf(fp_out,"shm_id       = %-12d",  pCtrl->shm_id);
    fprintf(fp_out,"shm_name    = %s\n",    pCtrl->shm_name);
    fprintf(fp_out,"shm_magic    = 0x%-10x",pCtrl->shm_magic);
    fprintf(fp_out,"shm_sem     = %d\n",    pCtrl->shm_sem);

    fprintf(fp_out,"[resources]\n");
    fprintf(fp_out,"hdr_nalloc   = %-10d  buf_nalloc  = %d\n",
                     pCtrl->hdr_nalloc,pCtrl->buf_nalloc);
    fprintf(fp_out,"hdr_nfree    = %-10d  buf_nfree   = %d\n",
                     pCtrl->hdr_nfree,pCtrl->buf_nfree);
    fprintf(fp_out,"buf_nsize    = %-10d\n",pCtrl->buf_nsize);

    fprintf(fp_out,"[link information]\n");
    fprintf(fp_out,"hdr_freehead = %-10d  buf_freehead = %d\n",
                     pCtrl->hdr_freehead,pCtrl->buf_freehead);
    fprintf(fp_out,"hdr_freetail = %-10d  buf_freetail = %d\n",
                     pCtrl->hdr_freetail,pCtrl->buf_freetail);

    fprintf(fp_out,"[statistics]\n");
    fprintf(fp_out,"shm_alloctimes= %-10d shm_allocfaults = %d\n",
                     pCtrl->shm_alloctimes,pCtrl->shm_allocfaults);
    fprintf(fp_out,"shm_freetimes = %-10d\n",pCtrl->shm_freetimes);

    fflush(fp_out);
    return 0;
}

//-------------------------------------------------------------------

int mbuf_view_link(const SHMOBJ *shmobj, FILE *fp_out)
{
    SHMCTRL *pCtrl     = NULL;
    SHMHDR  *pHdr      = NULL;
    SHMBUF  *pBuf      = NULL;
    int     i, curp;

    if(shmobj == NULL || shmobj->shm_pShmCtrl == NULL ||
       shmobj->shm_pShmCtrl->shm_magic != SHM_MAGIC)
    {
        errno = EINVAL;
        return -1;
    }

    if(!fp_out)
        fp_out = stdout;

    pCtrl = shmobj->shm_pShmCtrl;
    pHdr  = shmobj->shm_pHdrArray;
    pBuf  = shmobj->shm_pBufArray;

    //free header link
    fprintf(fp_out,"\n[free header link]\n");
    for(i=0, curp = pCtrl->hdr_freehead; curp >= 0; i++)
    {
     	if(curp == pCtrl->hdr_freetail)
     	{
            fprintf(fp_out,"%.5d\n",pHdr[curp].hdr_id);
            break;
        }    
        else
            fprintf(fp_out,"%.5d-->",pHdr[curp].hdr_id);

        if(((i+1) % 8) == 0)
          fprintf(fp_out,"\n");

        curp = pHdr[curp].hdr_next;
     }

    //free buffer link
    fprintf(fp_out,"\n[free buffer link]\n");
    for(i=0, curp = pCtrl->buf_freehead; curp >= 0; i++)
    {
     	if(curp == pCtrl->buf_freetail)
     	{
            fprintf(fp_out,"%.5d\n",pBuf[curp].buf_id);
            break;
        }    
        else
            fprintf(fp_out,"%.5d-->",pBuf[curp].buf_id);

        if(((i+1) % 8) == 0)
          fprintf(fp_out,"\n");

        curp = pBuf[curp].buf_next;
     }

     fflush(fp_out);
     return 0;
}

//------------------------------------------------------------

int mbuf_view_statistics(const SHMOBJ *shmobj, FILE *fp_out)
{
     SHMCTRL *pCtrl;
     int     i, j, k=0;
     double  percent, d1;

    if(shmobj == NULL || shmobj->shm_pShmCtrl == NULL ||
       shmobj->shm_pShmCtrl->shm_magic != SHM_MAGIC)
    {
        errno = EINVAL;
        return -1;
    }

    if(!fp_out) fp_out = stdout;

    pCtrl = shmobj->shm_pShmCtrl;
    fprintf(fp_out,"[general statistics]\n");
    fprintf(fp_out,"shm_alloctimes= %-10d shm_allocfaults = %d\n",
                     pCtrl->shm_alloctimes,pCtrl->shm_allocfaults);
    fprintf(fp_out,"shm_freetimes = %-10d\n",pCtrl->shm_freetimes);
    
    fprintf(fp_out,"[usage percent statistics]\n");
    if(pCtrl->shm_alloctimes <= 0)
    {
        printf("not available\n");
        return 0;
    }

    for(i=0; i < 100;i++)
    {
        d1 = pCtrl->shm_statArray[i] *100;
        percent = d1  / pCtrl->shm_alloctimes;
        if(percent ==0)
            continue;
        fprintf(fp_out,"%.4d--%.4d %.2f%%   ", i*50 ,i*50+49, percent);

        k ++;
        if( k % 4 == 0) fprintf(fp_out,"\n");
    }//for
    
    d1 = pCtrl->shm_statArray[i] *100;
    percent = d1  / pCtrl->shm_alloctimes;
    fprintf(fp_out,"%.4d--.... %.2f%%\n", i*50 ,i*50+49, percent);
    return 0;
}
