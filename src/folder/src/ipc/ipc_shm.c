#include <ipc_func.h>

//
// These routines manipulate shared memory,all
// of which are prefixed by 'shm_':
//
// interface to external:
// (1) void *shm_create(const  char *name, size_t size, int *shmid);
// (2) void *shm_connect(const char *name, int *shmid);
// (3) int   shm_detach(char *addr);
// (4) int   shm_delete(int shid);
//
// internal implementation helpers invisible to outer:
/* (1)*/  key_t shm_getmem(const char *name, size_t size, int createflg);
          key_t _shm_getmem(const char *name, size_t size, int createflg);
/* (2)*/  void *shm_attach(int shid);
//
//import from other module:
/*(1)*/ extern int ipc_makekey(const char *name);

/*---------------------------------------------------------------------------*/

void * shm_create(const char *name, size_t size,int *shmid)
{
   // Allocates a shared memory
   // name	name of shared memory block
   // size	total bytes in block
   // Returns:
   // 	NULL	if unable to allocate block
   //   address of block connected to process address space

   int	shid;
   char	*shseg;
   
   //call shm_getmem() to create the block
   shid = shm_getmem(name, size, 1);
   if(shid == -1)
     return(NULL);
	
   //call shm_attach() to connect to block
   shseg = shm_attach(shid);
   
   //bring memory ID to caller
   if(shmid)
     *shmid = shid;
     
   return(shseg);
}

/*---------------------------------------------------------------------------*/

void *shm_connect(const char *name,int *shmid)
{
     //	Input:  name	name of shared memory block
     //
     //	Action:	This function connects the processes to an already
     //		existing memory block.
     //
     //	Return:	pointer to block
     //		NULL on error

     int  shid;
     char *shseg;

     /*call shm_getmem() to locate block */
     shid = shm_getmem(name, 0, 0);
     if(shid == -1)
	return(NULL);
	
     /*	call shm_attach() to connect to block */
     shseg = shm_attach(shid);
     
     //bring memory ID to caller
     if(shmid)
       *shmid = shid;
     
     return(shseg);
}



void * _shm_create(const char *name, size_t size,int *shmid)
{
   // Allocates a shared memory
   // name	name of shared memory block
   // size	total bytes in block
   // Returns:
   // 	NULL	if unable to allocate block
   //   address of block connected to process address space

   int	shid;
   char	*shseg;
   
   //call shm_getmem() to create the block
   shid = _shm_getmem(name, size, 1);
   if(shid == -1)
     return(NULL);
	
   //call shm_attach() to connect to block
   shseg = shm_attach(shid);
   
   //bring memory ID to caller
   if(shmid)
     *shmid = shid;
     
   return(shseg);
}

/*---------------------------------------------------------------------------*/

void * _shm_connect(const char *name,int *shmid)
{
     //	Input:  name	name of shared memory block
     //
     //	Action:	This function connects the processes to an already
     //		existing memory block.
     //
     //	Return:	pointer to block
     //		NULL on error

     int  shid;
     char *shseg;

     /*call shm_getmem() to locate block */
     shid = _shm_getmem(name, 0, 0);
     if(shid == -1)
	return(NULL);
	
     /*	call shm_attach() to connect to block */
     shseg = shm_attach(shid);
     
     //bring memory ID to caller
     if(shmid)
       *shmid = shid;
     
     return(shseg);
}
/*---------------------------------------------------------------------------*/

int shm_detach(char *addr)
{
  //Input:	addr	address of segement to detach
  //Action:	Detattach this process from the named memory
  //Returns:	0	if all is well
  //		-1	on error

  return(shmdt(addr));
}

/*---------------------------------------------------------------------------*/

int shm_delete(int shid)
{
  //	Input:	shid	memory to delete
  //	Action:	Deletes the memory associated with this identifier
  //		shid is the id returned by Unix for this memory
  //	Returns:0   if ok
  //		-1  on error
  
  return(shmctl(shid, IPC_RMID, NULL));
}

/*---------------------------------------------------------------------------*/

key_t shm_getmem(const char *name, size_t size, int createflg) 
{
	//Input: name	name of shared memory block
	//       size	size in bytes
	//	 createflg if true then memory is created
	//
	//Action:Attempt to create or attach to a block of shared memory of
	//	size bytes (if createflg == 0 then size should be 0 )
	//
	//Returns:  -1 if unable to create block
	//	    id shared memory id of new block
	//
	
	int	shid;
	long	flags;
	key_t   shmkey;

	if(createflg)
		flags = IPC_CREAT | IPC_EXCL;
	else
		flags = 0;
	flags |= 0666;
		
	//create the new entry
	shmkey = ipc_makekey(name);
	shid = shmget(shmkey, size, flags);
	return(shid);
}

key_t _shm_getmem(const char *name, size_t size, int createflg) 
{
	//Input: name	name of shared memory block
	//       size	size in bytes
	//	 createflg if true then memory is created
	//
	//Action:Attempt to create or attach to a block of shared memory of
	//	size bytes (if createflg == 0 then size should be 0 )
	//
	//Returns:  -1 if unable to create block
	//	    id shared memory id of new block
	//
	
	int	shid;
	long	flags;
	key_t   shmkey;

	if(createflg)
		flags = IPC_CREAT | IPC_EXCL;
	else
		flags = 0;
	flags |= 0600;
		
	//create the new entry
	shmkey = atoi(name);
	shid = shmget(shmkey, size, flags);
	return(shid);
}
/*---------------------------------------------------------------------------*/

void *shm_attach(int shid)
{
	//Input:   shid	shared memory id
	//Action:  connect the shared memory block pointed to by
	//	   shid to process space. Return a pointer to it.
	//
	//Returns: pointer to shared memory block
	//	   NULL if error occured
	 
	char *addr;

	//attach to the system and be done with it
	addr = (char *)shmat(shid, (char *)0, 0);
	if(addr == (char *)(-1))
		return(NULL);
	else
		return(addr);
}

/*---------------------------------------------------------------------------*/

int shm_get_info(int shmid, int *nshmsize)
{
    struct shmid_ds  info;
    
     if(0 > shmctl(shmid, IPC_STAT, &info))
        return -1;

    if(nshmsize)
        *nshmsize = info.shm_segsz;

    return 0;
}
