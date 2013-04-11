/***************************************************
cheks if input file is too short or giant

***************************************************/
#include <headers.h>

#define SIZE_LIMIT 65419056
#define USAGE printf("\nUSAGE:\n      %s  (char *)input_file \n\n",__FILE__)


int main(int argc, char *argv[])
{
  //begin:imput arguments check:
  if(argc != 2)
    {
      USAGE;
      exit(1);
    }
  if(!CheckFile(argv[1]))
    {
      printf("\nfile argv[1]:%s does not exist!.\n",argv[1]);
      USAGE;
      exit(1);
    }
  //end:imput arguments check.
  


  //begin: global variables:
  char *name_file_input = (char *)argv[1];
  //end: global variables.

  //variables:
  char cmmd[100];


  //begin:data load: 
  int tree_levels = TreeLevelRead2(name_file_input, NULL);
  tree_levels = tree_levels - 1;

  long size = 0;

  FILE *f = fopen( name_file_input, "r" );

  if (f)
    {
      fseek(f, 0, SEEK_END);
      size = ftell(f);
      fclose(f);
    }

  if(size>SIZE_LIMIT)
    {
      //printf("size limit exceded\n");
      //printf("file size: %ld\n",size);
      fprintf(stdout,"1\n");
      exit(1);
    }


  if( tree_levels<2 )
    {      
      /*
      fprintf(stderr,"tree %s too short!\n",argv[1]);
      sprintf(cmmd,"rm %s",argv[1]);
      system(cmmd);
      */
      fprintf(stdout,"1\n");
    }
  else
    fprintf(stdout,"0\n");


      return 1;
}
