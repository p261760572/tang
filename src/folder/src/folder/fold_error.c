#include <folder.h>

struct 
{
  int  iFoldErrCode;
  char *pcFoldErrDesc;
} g_FoldErrTbl[] =
{
  {0,            "unkown error about folder system"},
  {FOLD_EEXIST,  "specified folder exists already"},
  {FOLD_ENOFOLD, "no free entry for creating new folder"},
  {FOLD_ENOENT,  "the specified folder not existing"},
  {FOLD_ENOSYS,  "not attached to Folder system yet"},
  {FOLD_ENOMBUF, "not enough buffers in shared memory"},
  {FOLD_EINVAL,  "invalid argument"},
  
  {FOLD_EMSGOVFLW,"message in folder overflow"},  
  {-1,           NULL}
};

char *ise_strerror(int u_errno)
{
  int i;

  if(u_errno > 0) //system error number
    return  strerror(u_errno);

  for(i=1; g_FoldErrTbl[i].pcFoldErrDesc != NULL; i++)
  {
    if(g_FoldErrTbl[i].iFoldErrCode == u_errno)
      return g_FoldErrTbl[i].pcFoldErrDesc;
  }

  return g_FoldErrTbl[0].pcFoldErrDesc;
}
