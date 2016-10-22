#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{

   printf("errno=%s\n",strerror(errno));
  return 1;
}
