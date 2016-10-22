#ifndef  __UTIL__H__
#define  __UTIL__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define TRUE (1==1)
#define FALSE (1==0)
//utilities for profile reading, from util_prof.c 
extern int  conf_open(const char *filename);
extern int  conf_close(int handle);
extern int  conf_get_first_number(int handle, 
            const char *key, int *out_value);
extern int  conf_get_next_number(int handle, 
            const char *key, int *out_value);
extern int  conf_get_first_string(int handle, 
            const char *key, char *out_string);
extern int  conf_get_next_string(int handle, 
            const char *key, char *out_string);



#endif //__UTIL__H__
