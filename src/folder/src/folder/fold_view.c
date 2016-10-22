#include <folder.h>

int fold_list_entry(FILE *outfp)
{
 int i,j;
 FILE *fp;
    
 fp = (outfp!=NULL) ? outfp : stdout;
 fprintf(fp,"       Name            PID      address   references  messages   max\n");
 fprintf(fp,"--------------------------------------------------------------------\n");    
 for(i=-1,j=0;j < gl_pFldCtrl->fold_nalloc;j++)
 {
     if(!(gl_pFldArray[j].fold_flags &  FOLD_FLAG_USED))
        continue;

     i = j;
     fprintf(fp,"[%.4d] %-16s%.6d   %.6d    %-8d    %-4d      %d\n",
            i,
            gl_pFldArray[i].fold_name,
            gl_pFldArray[i].fold_pid,
            gl_pFldArray[i].fold_address,
            gl_pFldArray[i].fold_nref,
            gl_pFldArray[i].fold_nmsg,
            gl_pFldArray[i].fold_nmaxmsg
            );
 }    

 if(i >= 0)
 fprintf(fp,"-------------------------------------------------------------------\n");    
 fprintf(fp,"free entry = %d\n", gl_pFldCtrl->fold_nfree);
 fflush(fp);
 return 0;
}

///////////////////////////////////////////////////////////

int fold_view_ctrlblk(FILE *fpout)
{
    FILE *fp;
    
    fp = fpout ? fpout : stdout;
    fprintf(fp,"[control block contents]\n");
    fprintf(fp,"shm_id       = %6d  shm_magic  = 0x%x\n", 
                 gl_pFldCtrl->shm_id, gl_pFldCtrl->shm_magic);
    fprintf(fp,"sem_id       = %6d  sem_count  = %d\n",
                 gl_pFldCtrl->sem_id, gl_pFldCtrl->sem_count);    
    fprintf(fp,"msg_reqqid   = %6d  msg_viaqid = %d\n",
                 gl_pFldCtrl->msg_reqqid,gl_pFldCtrl->msg_viaqid);
    fprintf(fp,"fold_nalloc  = %6d  fold_nfree = %d\n",
                 gl_pFldCtrl->fold_nalloc,gl_pFldCtrl->fold_nfree);
    fprintf(fp,"foldsvr_pid  = %6d\n",gl_pFldCtrl->foldsvr_pid);    
    fprintf(fp,"sys_tickcount= %6lu\n",gl_pFldCtrl->fold_systickcount);    
    
    fflush(fp); return 0;
}
