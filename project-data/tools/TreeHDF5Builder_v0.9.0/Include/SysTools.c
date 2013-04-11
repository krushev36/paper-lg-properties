#include "headers.h"
///////////////////////////////////////////////////////////////////////
//this rutine checks if the "name_dirfile" exist, if not, the directory 
//is created
void CheckCreateDir(char *name_dirfile)
{
  struct stat buf;
  
  stat(name_dirfile,&buf);
  
  if(S_ISDIR(buf.st_mode) == 0)	
	mkdir(name_dirfile, S_IRWXU);    
}


///////////////////////////////////////////////////////////////////////
//this rutine checks if the "name_dirfile" exist, if not, 0 is returned 
//else 1 is returned
int CheckDir(char *name_dirfile)
{
  struct stat buf;
  
  stat(name_dirfile,&buf);
  
  if(S_ISDIR(buf.st_mode) == 0)	
    return 0;
  else 
    return 1;

}

///////////////////////////////////////////////////////////////////////
//this rutine checks if the "name_file" exist, if not, 0 is returned 
//else 1 is returned
int CheckFile(char *name_file)
{
  struct stat buf;
  
  stat(name_file,&buf);
  
  if(S_ISREG(buf.st_mode) == 0)	
    return 0;
  else 
    return 1;

}

