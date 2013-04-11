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
  double mag_i, dmag_i, mag_bins, median, sigma2, quartil1, quartil3, quartil4, mean;
  int *treeID;
  double *diskStellarMass, *spheroidStellarMass, *diskGasMass;
  double *diskCircularVelocity, *SpheroidCircularVelocity, *diskLuminosity, *spheroidLuminosity, *velocityInRange;

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
  nb_lines = nb_lines-2;

  printf("Number of lines:%d\n",nb_lines);
 
  treeID  = malloc((size_t)nb_lines*sizeof(int));
  diskStellarMass  = malloc((size_t)nb_lines*sizeof(double));
  spheroidStellarMass  = malloc((size_t)nb_lines*sizeof(double));
  diskCircularVelocity = malloc((size_t)nb_lines*sizeof(double));
  SpheroidCircularVelocity = malloc((size_t)nb_lines*sizeof(double));
  diskLuminosity = malloc((size_t)nb_lines*sizeof(double));
  spheroidLuminosity = malloc((size_t)nb_lines*sizeof(double));
  diskGasMass = malloc((size_t)nb_lines*sizeof(double));
 
  file_input = fopen(name_file_input,"r");
  for(i=0; i<8; i++)
    fscanf(file_input,"%s", char_dumb);

    
  for(i=0; i<nb_lines; i++ )
      fscanf(file_input,"%d  %lf  %lf  %lf  %lf  %lf  %lf %lf"
	     , &treeID[i],  &spheroidStellarMass[i],  &diskStellarMass[i], &diskGasMass[i], &diskLuminosity[i]
	     , &spheroidLuminosity[i], &diskCircularVelocity[i], &SpheroidCircularVelocity[i]
	     );

  fclose(file_input);

  
    
////////////////////////////////////////////////////////////////////////////////////////////////////

  file_output = fopen(name_file_output,"w");
  fprintf(file_output, "#1.mag_i   #2.quartil1   #3.median   #4.quartil3    #5.mean    #6.sigma2\n");

  mag_bins = 15.0;
  dmag_i = (-18.0+23.0)/mag_bins;

  for(mag_i=-23.0; mag_i<-18.0; mag_i+=dmag_i)
    {
      j = 0;
      for(i=0; i<nb_lines; i++  )
  	if( (-2.5*log10( diskLuminosity[i] + spheroidLuminosity[i]) >= mag_i) && (-2.5*log10( diskLuminosity[i] + spheroidLuminosity[i])<mag_i+dmag_i)  &&
	    (spheroidStellarMass[i]/(diskStellarMass[i]+spheroidStellarMass[i])<=0.3) )
  	  j++;	  
      
      nb_counts = j;

      
      velocityInRange = malloc((size_t)nb_counts*sizeof(double));
      
      j = 0;
      for(i=0; i<nb_lines; i++  )
  	if( (-2.5*log10( diskLuminosity[i] + spheroidLuminosity[i]) >= mag_i) && (-2.5*log10( diskLuminosity[i] + spheroidLuminosity[i])<mag_i+dmag_i) &&
	    ( spheroidStellarMass[i]/(diskStellarMass[i]+spheroidStellarMass[i])<=0.3 ) )
  	  {
  	    velocityInRange[j] = diskCircularVelocity[i] > SpheroidCircularVelocity[i] ? diskCircularVelocity[i] : SpheroidCircularVelocity[i];
  	    j++;
  	  }

      mean = gsl_stats_mean (velocityInRange, 1,  nb_counts);
      gsl_sort(velocityInRange, 1, nb_counts);
      
      median = gsl_stats_median_from_sorted_data(velocityInRange, 1, nb_counts);

      quartil1 = gsl_stats_quantile_from_sorted_data (velocityInRange, 1, nb_counts, 0.25);
      quartil3 = gsl_stats_quantile_from_sorted_data (velocityInRange, 1, nb_counts, 0.75);
      //quartil4 = gsl_stats_quantile_from_sorted_data (velocityInRange, 1, nb_counts, 1.0);
      sigma2 = gsl_stats_variance (velocityInRange, 1, nb_counts);

      fprintf(file_output,"%f   %f    %f    %f   %f  %f\n", mag_i, quartil1, median, quartil3, mean, sigma2);
    }
  fclose(file_output);

  return 0;
}

