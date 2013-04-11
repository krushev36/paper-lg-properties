#include<rutines/headers.h>
#include<gsl/gsl_statistics_double.h>

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


  int i, j;
  int nb_lines, nb_counts;
  char char_dumb[100];
  double mag_r, dmag_r, mag_bins, median, sigma2, quartil1, quartil3, quartil4, mean;
  int *treeID;
  double *magnitude_r, *color, *colorInRange;
  double  *diskLuminosity_r, *spheroidLuminosity_r,*diskLuminosity_g, *spheroidLuminosity_g;

  FILE *file_input, *file_output;  


////////////////////////////////////////////////////////////////////////////////////////////////////
  file_input = fopen(name_file_input,"r");

  nb_lines = 0;
  while( !feof(file_input) )
    {  
      for(i=0; i<4; i++)	
	fscanf(file_input,"%s", char_dumb);

    nb_lines++;
    }
  fclose(file_input);
  nb_lines = nb_lines-2;

  printf("Number of lines:%d\n",nb_lines);
 
  magnitude_r = malloc((size_t)nb_lines*sizeof(double));
  color = malloc((size_t)nb_lines*sizeof(double));
  diskLuminosity_r = malloc((size_t)nb_lines*sizeof(double));
  spheroidLuminosity_r = malloc((size_t)nb_lines*sizeof(double));
  diskLuminosity_g = malloc((size_t)nb_lines*sizeof(double));
  spheroidLuminosity_g = malloc((size_t)nb_lines*sizeof(double));
  
 
  file_input = fopen(name_file_input,"r");
  for(i=0; i<4; i++)
    fscanf(file_input,"%s", char_dumb);

    
  for(i=0; i<nb_lines; i++ ){
      fscanf(file_input,"%lf  %lf  %lf  %lf"
	     ,  &diskLuminosity_g[i], &diskLuminosity_r[i],  &spheroidLuminosity_g[i],  &spheroidLuminosity_r[i]);

      magnitude_r[i] = -2.5*log10( diskLuminosity_r[i] + spheroidLuminosity_r[i] );
      color[i] = -2.5*log10( diskLuminosity_g[i] + spheroidLuminosity_g[i] )+2.5*log10( diskLuminosity_r[i] + spheroidLuminosity_r[i] );
  }
  fclose(file_input);


  
    
////////////////////////////////////////////////////////////////////////////////////////////////////

  file_output = fopen(name_file_output,"w");
  fprintf(file_output, "#1.mag_r   #2.quartil1   #3.median   #4.quartil3    #5.mean    #6.sigma2\n");

  mag_bins = 15.0;
  dmag_r = (-17.0+24.0)/mag_bins;

  for(mag_r=-22.0; mag_r<-17.0; mag_r+=dmag_r)
    {
      j = 0;
      for(i=0; i<nb_lines; i++  )
  	if( ( magnitude_r[i] >= mag_r ) && ( magnitude_r[i] < mag_r+dmag_r) )
  	  j++;
      
      nb_counts = j;
      
      colorInRange = malloc((size_t)nb_counts*sizeof(double));
      
      j = 0;
      for(i=0; i<nb_lines; i++  )
  	if( ( magnitude_r[i] >= mag_r ) && ( magnitude_r[i] < mag_r+dmag_r) )
  	  {
  	    colorInRange[j] = color[i];
  	    j++;
  	  }

      mean = gsl_stats_mean (colorInRange, 1,  nb_counts);
      gsl_sort(colorInRange, 1, nb_counts);
      
      median = gsl_stats_median_from_sorted_data(colorInRange, 1, nb_counts);

      quartil1 = gsl_stats_quantile_from_sorted_data (colorInRange, 1, nb_counts, 0.25);
      quartil3 = gsl_stats_quantile_from_sorted_data (colorInRange, 1, nb_counts, 0.75);
      //quartil4 = gsl_stats_quantile_from_sorted_data (velocityInRange, 1, nb_counts, 1.0);
      sigma2 = gsl_stats_variance (colorInRange, 1, nb_counts);

      fprintf(file_output,"%f   %f    %f    %f   %f  %f\n", mag_r, quartil1, median, quartil3, mean, sigma2);
    }
  fclose(file_output);

  return 0;
}

