#include "ibdcs.h"
#include "tmcibtms.h"
#include "dbstruct.h"
#include <string.h>
#include <stdlib.h>

char *g_pcBcdaShmPtr;

int main(int argc, char *argv[])
{
    printf("start init.\n");

    if ( dcs_connect_shm() < 0 )
    {
        printf( "dcs_connect_shm() failed:%s\n" );
        return -1;
    }

    g_pcBcdaShmPtr = (char *)shm_connect("BCDA",NULL);
    if ( g_pcBcdaShmPtr == NULL )
    {
        printf("cannot connect shared memory 'BCDA'!");
        return(-1);
    }

    if ( fold_initsys() < 0 )
    {
        printf("fold_init failure.\n");
        return -1;
    }

    char buff[1024];
    strcpy(buff, "CMDI0004");

    int foldid = fold_locate_folder("IVRAPP"); 
    fold_write( foldid , -1, buff, strlen(buff) );
    return 0;
}

