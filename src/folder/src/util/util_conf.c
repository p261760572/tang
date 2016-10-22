#include <util_func.h>

typedef struct _tagProfileTbl
{
    int    conf_handle;
    FILE  *conf_fp;
} CONFIG_FILE_TBL;

#define CONFIG_TBL_ROWS  16
#define COMMENT_CHAR     '#'

//---------------------------------------------------------------
//internal data structures and helpers which are invisible to outer

static  CONFIG_FILE_TBL  gs_conf_tbl[CONFIG_TBL_ROWS];
static  int              gs_fInitialized = FALSE;
static  int  conf_get_nextline(FILE *fp, char *key, char *value);
static  int  conf_init_impl();
static  int  conf_find_impl(int handle);
static  int  IsCommentLine(char *line);
static  int  IsEmptyLine(char *line);

//---------------------------------------------------------------

int  conf_open(const char *filename)
{
    int i;
	
    //guarantee the arguments valid
    if(filename == NULL || filename[0] == 0)
    {
        errno = EINVAL;
        return -1;
    }
    	
    conf_init_impl();
	
    for(i=0; i<CONFIG_TBL_ROWS; i++)
	    if(gs_conf_tbl[i].conf_fp == NULL)
		  break;
    if(i >= CONFIG_TBL_ROWS)
    {
	   //cannot find free entry
	   errno =  ENOMEM;
	   return -1;	
    }
	  	
    gs_conf_tbl[i].conf_fp = fopen(filename,"r+");
    if(gs_conf_tbl[i].conf_fp==NULL)
	    return -1;
		
    // set the close-on-exec flag
    fcntl(fileno(gs_conf_tbl[i].conf_fp), F_SETFD, FD_CLOEXEC);
	
    return (gs_conf_tbl[i].conf_handle);	
}

//---------------------------------------------------------------

int conf_close(int handle)
{
	int i;
	
	i = conf_find_impl(handle);
	if(i < 0) 
		return -1;
		
	//close the file
	fclose(gs_conf_tbl[i].conf_fp);
	gs_conf_tbl[i].conf_fp = NULL;
	return 0;
}

//---------------------------------------------------------------

int  conf_get_first_number(int handle, const char *key, int *out_value)
{
	char  out_string[256];
	
	memset(out_string,0,sizeof(out_string));
	if(0 > conf_get_first_string(handle,key,out_string))
		return -1;
	
	if(out_value)
		*out_value = strtol(out_string, (char **)NULL, 0);
	return(0);
}

//---------------------------------------------------------------

int  conf_get_next_number(int handle, const char *key, int *out_value)
{
	char  out_string[256];
	
	memset(out_string,0,sizeof(out_string));
	if(0 > conf_get_next_string(handle,key,out_string))
		return -1;
	
	if(out_value)
		*out_value = strtol(out_string, (char **)NULL, 0);
	return(0);
}

//---------------------------------------------------------------

int  conf_get_first_string(int handle, const char *key, char *out_string)
{
	char tmpKey[256],tmpValue[256];
        int  i;
	FILE *fp;
		
	i = conf_find_impl(handle);
    if(i < 0)
		return -1;
		
	//scan until 'key' found
	fp = gs_conf_tbl[i].conf_fp;
	rewind(fp);
	
	for(;;)
	{
		memset(tmpKey,0,sizeof(tmpKey));
		memset(tmpValue,0,sizeof(tmpValue));		
		if(0 > conf_get_nextline(fp, tmpKey, tmpValue))
			break;
		if(u_stricmp((char*)key, (char *)tmpKey) == 0)
		{
			strcpy(out_string, tmpValue);
			return(0);
		}
	}

	*out_string = '\0';
	return -1;		
}

//-----------------------------------------------------------------

int  conf_get_next_string(int handle, const char *key, char *out_string)
{
	char tmpKey[256],tmpValue[256];
    int  i;
	FILE *fp;
		
	if(0 > (i = conf_find_impl(handle)))
		return -1;
		
	//scan until 'key' found
	fp = gs_conf_tbl[i].conf_fp;
	
	for(;;)
	{
		memset(tmpKey,0,sizeof(tmpKey));
		memset(tmpValue,0,sizeof(tmpValue));		
		if(0 > conf_get_nextline(fp, tmpKey, tmpValue))
			break;
		
		if(u_stricmp((char *)key, (char *)tmpKey) == 0)
		{
			strcpy(out_string, tmpValue);
			return(0);
		}
	}

	*out_string = '\0';
	return -1;		
}

//////////////////////////////////////////////////////////////
//internal helpers go here...

int conf_get_nextline(FILE *fp, char *key, char *value)
{
	char	*start_value;
	char    line[1024], *p, *p1, *p2;
	
	*value = *key  = 0;
	
	for(;;)
	{
	   memset(line, 0,sizeof(line));
	   if(fgets(line, sizeof(line) - 1, fp) == NULL)
	       return(-1);

	   if(IsEmptyLine(line))   continue;
       if(IsCommentLine(line)) continue;
           	
       //handle layout such as "keyName  Value  #comment"
       strip_trailing_comments(line);
       
	   //scan for the 'key'
	   for(p = line; isspace(*p); ++p)  //skip the heading space
	          ;
	   for(; *p && !isspace(*p); )
	 	 *key++ = *p++;
	   *key = 0;
	
	   //scan for the 'value'
	   for( ;isspace(*p); ++p)
		    ;
	   for(start_value = value; *p && *p != '\n';)
	   {
	      	if(*p != '\\')
	      	{
                	*value++ = *p++;
                  	continue;	      	
	      	}
	      
 	      	// check if this is a line continuation
	      	++p;
	      	if(!(*p) || isspace(*p))
	      	   while(1)
	      	   {
		   	        memset(line, 0,sizeof(line));
		   	        if(fgets(line, sizeof(line) - 1, fp) == NULL)
			            break;
			            
	           	    if(IsEmptyLine(line))   continue;
                   	if(IsCommentLine(line)) continue;
			        strip_trailing_comments(line);
			        
		   	        p = line;
			        break;
		       } 	
		    else
		       *value++ = '\\';
	    }//for
	    
        *value = '\0';

	    // remove the trailing space
	    for(--value; value > start_value && isspace(*value); --value)
		    *value = 0;
	    ++value;

	    return (value - start_value);
	}//for(;;)
}

//---------------------------------------------------------------

int  IsEmptyLine(char *line)
{
    for(; line && *line; line++)
    {
	    if(!isspace(*line))
	        return FALSE;
    }
    
    return (*line == 0) ? TRUE : FALSE;
}

//---------------------------------------------------------------

int  IsCommentLine(char *line)
{
    for(; line && *line;)
    {
       if(isspace(*line))
   	       line++;
       else
	       break;
    }

    return ( *line == COMMENT_CHAR) ? TRUE : FALSE;
}

//---------------------------------------------------------------

int  conf_init_impl()
{
	int i;
	int base_handle;
        
    if(gs_fInitialized)  
        return 0;
        	
	gs_fInitialized = TRUE;	        	
	for(base_handle = getpid(),i=0; i<CONFIG_TBL_ROWS; i++)
	{
		gs_conf_tbl[i].conf_handle = base_handle + i;
		gs_conf_tbl[i].conf_fp     = NULL;
	}
	
	return 0;
}

//---------------------------------------------------------------

int  conf_find_impl(int handle)
{
	int i;

	//locate the entry
	for(i=0; i<CONFIG_TBL_ROWS; i++)
	{
		if(gs_conf_tbl[i].conf_fp == NULL)
		  continue;
		if(gs_conf_tbl[i].conf_handle != handle)
		  continue;
		  
		return i; //get it
	}
		  
	if(i >= CONFIG_TBL_ROWS)
	{
	   //cannot find the given handle
	   errno =   ENOENT;
	   return -1;	
	}
}

//---------------------------------------------------------------

int strip_trailing_comments(char *line)
{
   char *p,*p1,*p2;
       
   for(p=line; *p; p++)
   {
       if(*p != COMMENT_CHAR)
           continue;
               
       if(*(p+1) != COMMENT_CHAR) 
       {
           //strip off the comments
           *p = 0;    
           break;
       }
           
       //"##" stands for a '#' character
       for(p1 = p+1, p2 = p + 2; ;p1++,p2++)
       {
           *p1 = *p2;
           if(! *p2)
               break;
       }    
   }   
       
   return 0;
}
