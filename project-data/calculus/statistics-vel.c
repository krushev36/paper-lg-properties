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
  int nb_lines, nb_counts, nb_massRanges;
  char char_dumb[100];

  double mass, dmass, median, quartil1, quartil3, sigma2, mean;
  double *diskStellarMass, *spheroidStellarMass, *velocityInRange, *diskCircularVelocity, *spheroidCircularVelocity;

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
  //fclose(file_input);
  nb_lines = nb_lines - 2;

  printf("Number of lines:%d\n",nb_lines);
 
  spheroidStellarMass = malloc((size_t)nb_lines*sizeof(double));
  diskStellarMass = malloc((size_t)nb_lines*sizeof(double));
  spheroidCircularVelocity =  malloc((size_t)nb_lines*sizeof(double));
  diskCircularVelocity = malloc((size_t)nb_lines*sizeof(double));


 
  file_input = fopen(name_file_input,"r");
  for(i=0; i<4; i++)
    fscanf(file_input,"%s", char_dumb);
    
  for(i=0; i<nb_lines; i++ )
    fscanf(file_input,"%lf  %lf  %lf  %lf"
  	   , &spheroidStellarMass[i], &diskStellarMass[i], &diskCircularVelocity[i], &spheroidCircularVelocity[i]
  	   );
    
  fclose(file_input);


////////////////////////////////////////////////////////////////////////////////////////////////////

  file_output = fopen(name_file_output,"w");
  fprintf(file_output, "#1.starMass   #2.quartil1   #3.median   #4.quartil3   #5.mean #6.sigma2\n");

  nb_massRanges = 20;
  dmass = (11.5-9.5)/nb_massRanges;

  for(mass=8.5; mass<11.5; mass+=dmass)
    {

      j = 0;
      for(i=0; i<nb_lines; i++  )
  	if( (log10(diskStellarMass[i] + spheroidStellarMass[i]) >= mass) && (log10(diskStellarMass[i] + spheroidStellarMass[i]) < mass+dmass) )
  	  j++;

      nb_counts = j;
      //printf("number of matches:%d\n",nb_counts);

      velocityInRange = malloc((size_t)nb_counts*sizeof(double));

      j = 0;
      for(i=0; i<nb_lines; i++  )
  	if(  (log10(diskStellarMass[i] + spheroidStellarMass[i]) >= mass) && (log10(diskStellarMass[i] + spheroidStellarMass[i]) < mass+dmass) )
  	  {
  	    velocityInRange[j] = diskCircularVelocity[i] > spheroidCircularVelocity[i] ? diskCircularVelocity[i] : spheroidCircularVelocity[i];
  	    j++;
  	  }
  
      gsl_sort(velocityInRange, 1, nb_counts);
      mean = gsl_stats_mean (velocityInRange, 1,  nb_counts);
      median = gsl_stats_median_from_sorted_data(velocityInRange, 1, nb_counts);
      sigma2 = gsl_stats_variance (velocityInRange, 1, nb_counts);
      quartil1 = gsl_stats_quantile_from_sorted_data (velocityInRange, 1, nb_counts, 0.25);
      quartil3 = gsl_stats_quantile_from_sorted_data (velocityInRange, 1, nb_counts, 0.75);
      //quartil4 = gsl_stats_quantile_from_sorted_data (velocityInRange, 1, nb_counts, 1.0);
      fprintf(file_output,"%f    %f    %f    %f    %f     %f\n", mass, quartil1, median, quartil3, mean, sigma2);
    }
  fclose(file_output);

  return 0;
}

