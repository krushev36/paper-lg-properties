#include <rutines/headers.h>
#include <gsl/gsl_statistics_double.h>
#include <gsl/gsl_spline.h>



int main(int argc, char *argv[])
{
  if(argc<3)
    {
      printf("Not enough arguments\n");
      printf("Number of arguments:%d\n",argc);      
      exit(1);
    }
  if(!CheckFile(argv[1]))
    {
      printf("\nfile argv[1]:%s does not exist!.\n",argv[1]);
      // USAGE;
      exit(1);
    }

  char *name_file_input = argv[1];
  char *name_file_output = argv[2];
  

  int i, nb_lines;
  char char_dumb[100];

  double position[0], *chi_squared, *hist_counts;

  FILE *file_input, *file_output;  


////////////////////////////////////////////////////////////////////////////////////////////////////
  
  file_input = fopen(name_file_input,"r");

  nb_lines = 0;
  while( !feof(file_input) )
    {  
      for(i=0; i<3; i++)  	
	fscanf(file_input,"%s", char_dumb);      
      
      nb_lines++;
    }
  fclose(file_input);
  nb_lines = nb_lines - 1;

  printf("Number of lines:%d\n",nb_lines);

////////////////////////////////////////////////////////////////////////////////////////////////////
 
  chi_squared = malloc((size_t)nb_lines*sizeof(double));
  hist_counts = malloc((size_t)nb_lines*sizeof(double));


  file_input = fopen(name_file_input,"r");

  //file_output = fopen(name_file_output,"w");
    
  for(i=0; i<nb_lines; i++ )
    {

      fscanf(file_input,"%lf  %lf  %lf", &position[0], &position[1], &hist_counts[i]);
      chi_squared[i] = position[0] + (position[1]-position[0])/2.0;
            
    }
  fclose(file_input);


  gsl_interp_accel *acc = gsl_interp_accel_alloc ();
  gsl_spline *spline = gsl_spline_alloc (gsl_interp_cspline, nb_lines);  
  gsl_spline_init (spline, chi_squared, hist_counts, nb_lines);
 



  yi = gsl_spline_eval (spline, , acc);


  gsl_spline_free (spline);
  gsl_interp_accel_free (acc);
 
  //fclose(file_output);

  return 0;
}

     
/* for (xi = x[0]; xi < x[9]; xi += 0.01) */
/*   { */
/*     yi = gsl_spline_eval (spline, xi, acc); */
/*     printf ("%g %g\n", xi, yi); */
/*   } */

