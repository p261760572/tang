#include <unixhdrs.h>

sigfunc_t * catch_signal(int sig_no, sigfunc_t *sig_catcher)
{
    //
    //catch a specified signal with the given handler
    //
    
    struct sigaction act, oact;
    
    act.sa_handler = sig_catcher;
    act.sa_flags   = 0;

    //block no signals when this signal is being handled.        
    sigemptyset(&act.sa_mask);

    if(sig_no == SIGALRM || sig_no == SIGUSR1 || sig_no == SIGUSR2)
    {
        #ifdef SA_INTERRUPT        
            act.sa_flags |=  SA_INTERRUPT;    
        #endif        
    }
    else
    {
        #ifdef SA_INTERRUPT        
            act.sa_flags |=  SA_INTERRUPT;    
        #endif        

        //#ifdef SA_RESTART
          // act.sa_flags |=  SA_RESTART;
        //#endif    
    }
    
    if(sigaction(sig_no, &act, &oact) < 0)
        return (sigfunc_t *)0;
    return oact.sa_handler;    
}

//-------------------------------------------------

int catch_all_signals(sigfunc_t *sig_catcher)
{
    int signo;
    
    for(signo=1; signo < NSIG; signo++)
    {
        switch(signo)
        {
            case SIGHUP:             
            case SIGCONT:             
                 catch_signal(signo, SIG_IGN);
                 break;

            case SIGCHLD:
            default:
                 catch_signal(signo,sig_catcher);
                 break;     
        }//switch         
    }//for
    
    return 0;    
}

//----------------------------------------------------

int wait_child_or_signal(int *stat_loc)
{
    //this function will block the calling process
    //until ether of the following events occurs:
    //(1) any child of the caller died; the return value
    //    will be the pid of the dead child and its exit 
    //    status will be placed into stat_loc
    //(2) any signal delivered to the caller; the return 
    //    value will be 0;
    
    int status, result;
    
    for(;;)
    {
        result = wait(&status);

        if(result > 0) //a child died
        {
            if(stat_loc) *stat_loc = status;
            return result;   
        }
        
        if(errno == ECHILD)
        {
            //there are no children at all to wait for
            return 0;
        }

        if(errno == EINTR)
        {
            //a signal make us return from wait()    
            return waitpid(-1,NULL,WNOHANG);
        }
        
        //other case, to continue waiting...
    }//for(;;)
}

// the exit status of a child process is a 16 bits integer.
// from the perspective of parent process,when it called
// wait() and got the status of the child,it can determine
// what caused the child to terminate as following table:
//
// <cause>     <high 8 bits of status>  <low 8 bits of status>
// stopped ,   signal number,            WSTOPFLG
// signaled,   0,                        signal
// exit(code), low 8 bits of code,       0   

#ifndef WIFEXITED
  #define         WIFEXITED(sv)           ((sv & 0xff) == 0)
  #define         WEXITSTATUS(sv)         ((sv >> 8) & 0xff)
  #define         WIFSIGNALED(sv)         ((sv) && (sv == (sv & 0x00ff)))
  #define         WTERMSIG(sv)             (sv & 0x7f)
  #define         WIFSTOPPED(sv)          ((sv & 0xff) == 0177)
  #define         WSTOPSIG(sv)            ((sv >> 8) & 0xff)
#endif //WIFEXITED   

