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
  int nb_lines, nb_counts, nb_fracRanges;
  char char_dumb[20];
  double frac, dfrac, median, quartil1, quartil3, sigma2, mean;
  double *StellarMass, *gasFraction, *diskGasMass1, *diskGasMass2 , *StellarMassInRange;

  FILE *file_input, *file_output;  


////////////////////////////////////////////////////////////////////////////////////////////////////
  file_input = fopen(name_file_input,"r");

  nb_lines = 0;
  while( !feof(file_input) )
    {  
      for(i=0; i<8; i++)  	
	fscanf(file_input,"%s", char_dumb);      
    
    nb_lines++;
    }
  fclose(file_input);
  nb_lines=nb_lines-1;

  printf("Number of lines:%d\n",nb_lines);
  

  StellarMass = malloc((size_t)nb_lines*sizeof(double));
  diskGasMass1 = malloc((size_t)nb_lines*sizeof(double));
  diskGasMass2 = malloc((size_t)nb_lines*sizeof(double));
  gasFraction = malloc((size_t)nb_lines*sizeof(double));

  file_input = fopen(name_file_input,"r");     
  for(i=0; i<nb_lines; i++ )
    fscanf(file_input,"%s  %s  %lf  %s  %lf  %lf  %s  %s"
	   , char_dumb,  char_dumb, &StellarMass[i], char_dumb, &diskGasMass1[i], &diskGasMass2[i], char_dumb, char_dumb
	   );        
  
  fclose(file_input);

  
////////////////////////////////////////////////////////////////////////////////////////////////////

  file_output = fopen(name_file_output,"w");
  fprintf(file_output, "#1.gasFraction   #2.quartil1   #3.median   #4.quartil3   #5.mean  #6.sigma2\n");

  nb_fracRanges = 60;
  dfrac = (0.7-0.05)/nb_fracRanges;

  for(frac=0.00; frac<0.7; frac+=dfrac)
    {
      j = 0;
      for(i=0; i<nb_lines; i++){
	gasFraction[i] = pow(diskGasMass1[i],10.0) + pow(diskGasMass2[i],10.0);
	gasFraction[i] = gasFraction[i]/(pow(diskGasMass1[i],10.0) + pow(diskGasMass2[i],10.0) + pow(StellarMass[i],10.0));
	//printf("gasFrac=%lf\n",gasFraction[i]);
	if( (gasFraction[i] >= frac) && (gasFraction[i] < frac+dfrac )){
	  j++;
	}
      }

      nb_counts = j;    
      printf("frac=%lf matches=%d\n",frac,nb_counts);
      StellarMassInRange = malloc((size_t)nb_counts*sizeof(double));

      j = 0;
      for(i=0; i<nb_lines; i++  ){	
	if( (gasFraction[i] >= frac) && (gasFraction[i] < frac+dfrac ))
	  {
	    StellarMassInRange[j] = StellarMass[i];
	    j++;
	  } 
      }
      gsl_sort(StellarMassInRange, 1, nb_counts);
      mean = gsl_stats_mean (StellarMassInRange, 1,  nb_counts);
      median = gsl_stats_median_from_sorted_data(StellarMassInRange, 1, nb_counts);
      sigma2 = gsl_stats_variance (StellarMassInRange, 1, nb_counts);
      quartil1 = gsl_stats_quantile_from_sorted_data (StellarMassInRange, 1, nb_counts, 0.25);
      quartil3 = gsl_stats_quantile_from_sorted_data (StellarMassInRange, 1, nb_counts, 0.75);
      fprintf(file_output,"%f   %f    %f    %f   %f  %f\n", frac, quartil1, median, quartil3, mean, sigma2);
    }
  fclose(file_output);
  
  return 0;
}

