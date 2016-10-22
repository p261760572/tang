#include <ipc_func.h>

//
// These routines manipulate Semaphore,all
// of which are prefixed by 'sem_':
//
// interface to external:
// (1) int sem_create(const char *name, int nsem);
// (2) int sem_connect(const char *name, int nsems);
// (3) int sem_lock(int semid, int semnum, int lockflg);
// (4) int sem_unlock(int semid, int semnum, int lockflg);
// (5) int sem_delete(int semid);
//
// internal implementation helpers invisible to outer:
/* (1)*/  static int sem_getsem(const char *name, int nsems, int createflg);
//
//import from other module:
/*(1)*/ extern int ipc_makekey(const char *name);

//-----------------------------------------------------------

int sem_create(const char *name, int nsem)
{
   // creates a brand new semaphore.	
   
   // name   name of semaphore to create
   // nsem   number os semaphores in array
   // 
   // Returns:	Semaphore id
   //		-1 on error

   int  semid;
   int 	i;
   union  semum arg;

   //get the semaphore
   semid = sem_getsem(name, nsem, 1);
   if(semid < 0) return -1;
   
   //clear the semaphores to 1 (one) resource
   arg.val = 1;
   for(i = 0; i < nsem; ++i)
    semctl(semid, i, SETVAL, arg);

   return(semid);
}

//-----------------------------------------------------------

int sem_connect(const char *name, int nsems)
{
   // connect to a existing semaphore and
   // fetch its ID
   	
   return(sem_getsem(name, nsems, 0));
}

//-----------------------------------------------------------

int sem_lock(int semid, int semnum, int lockflg)
{
  //Input:semid   sem id from unix
  //	  semnum  which semaphore to lock in the semaphores array
  //	  lockflg if set then we block until operation done
  //
  //Action: Attempts to gain exclusive use of this semaphore

  int	rc;
  struct sembuf sembuf;
  
  //setup our internal buffer
  sembuf.sem_num = semnum;
  sembuf.sem_op  = -1;
  sembuf.sem_flg = (!lockflg ? IPC_NOWAIT : 0) | SEM_UNDO;
  for(;;)
  {
     rc = semop(semid, &sembuf, 1);
     if(rc == -1 && errno == EINTR)
	continue;
     else	
	break;
  }

  return(rc);
}

//-----------------------------------------------------------

int sem_unlock(int semid, int semnum, int lockflg)
{
  //Input:semid   sem id from unix
  //	  semnum  which semaphore to lock in the semaphores array
  //	  lockflg if set then we block until operation done
  //
  //Action: This function attempts to release a semaphore taken via sem_lock()

  int	rc;
  struct sembuf sembuf;
  
  //setup our internal buffer
  sembuf.sem_num = semnum;
  sembuf.sem_op  = 1;
  sembuf.sem_flg = (!lockflg ? IPC_NOWAIT : 0) | SEM_UNDO;
  
  rc = semop(semid, &sembuf, 1);
  return(rc);
}

//-----------------------------------------------------------

int sem_delete(int semid)
{
 //Input:  semid semaphore id
 //Action: Removes a semaphore structure from the operating system

  return(semctl(semid, 0, IPC_RMID, 0));
}

//-----------------------------------------------------------

static int sem_getsem(const char *name, int nsems, int createflg)
{

  //Input:  name	name of semaphore
  // 	    nsem	number of semaphores
  //	    createflg  if set then the semaphore is created
  //	 
  //Action: Create or get an existing semaphore id.
  //Returns:semaphore id or -1 on error

	
  key_t	key;
  int	semid;
  int	flags;

  //make the key
  key = ipc_makekey(name);

  //set create and read/write flags
  if(createflg)
    flags = IPC_CREAT | IPC_EXCL;
  else
    flags = 0;
	
  flags = flags | 0666;//every body reads and writes

  semid = semget(key, nsems, flags);
    return(semid);
}

//-----------------------------------------------------------

int sem_get_info(int semid, int *nsemcount)
{
    struct semid_ds  info;
    union semun
    {
       int val;
       struct semid_ds *buf;
       ushort *array;
    } arg;
    
    arg.buf = &info;
    if(0 > semctl(semid, 0, IPC_STAT, arg))
        return -1;

    if(nsemcount)
        *nsemcount = info.sem_nsems;

    return 0;
}
