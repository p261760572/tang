#include <folder.h>

//
// all globals in Folder system defined in this module
//

//
//object to refrence the shared meory acting as buffer pool
//for Folder system
//
SHMOBJ gl_shmobj = {0};

//pointer to the control block in Folder list shared memory
FOLDCTRLBLK *gl_pFldCtrl  = NULL;

//pointer to the slots in Folder list
FOLDERENTRY *gl_pFldArray = NULL;

//message packet 
struct msgpacket gl_msgpacket = {0};

