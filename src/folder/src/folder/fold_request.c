#include <folder.h>

int fold_send_request(struct qrypacket *pQry)
{
    pQry->fold_msgtype = gl_pFldCtrl->foldsvr_pid; //message type of foldSvr
    pQry->fold_pid     = getpid();
    
    return queue_send(gl_pFldCtrl->msg_reqqid,
                      pQry,
                      sizeof(struct qrypacket) - sizeof(long),
                      FALSE //blocked sending
                      );
}

////////////////////////////////////////////////////////////////////////

int fold_wait_answer(struct qrypacket *pQry)
{
    int ret;
    
    pQry->fold_msgtype = getpid(); //message type of ourself
    
    for(;;)
    {
        ret = queue_recv(gl_pFldCtrl->msg_reqqid,
                         pQry,
                         sizeof(struct qrypacket) - sizeof(long),
                         FALSE); //blocked receving
        if(ret < 0)
        {
            if(errno == EINTR)
                continue;
            else
                return -1;
        }
        else
            break;
    }
    
    if(pQry->fold_result >=0 && pQry->fold_errno == 0)
        return pQry->fold_result;
        
    errno = pQry->fold_errno;    
    return -1;    
}
