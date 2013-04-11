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
  char char_dumb[20];
  int *treeId;
  double mass, dmass, median, quartil1, quartil3, sigma2, mean;
  double *nodeMass, *diskStellarMass, *diskGasMass, *diskCircularVelocity, *spheroidStellarMass, *spheroidGasMass, *gasFractionInRange, *gasFraction;

  FILE *file_input, *file_output;  


////////////////////////////////////////////////////////////////////////////////////////////////////
  file_input = fopen(name_file_input,"r");

  nb_lines = 0;
  while( !feof(file_input) )
    {  
      for(i=0; i<5; i++)  	
	fscanf(file_input,"%s", char_dumb);      
    
    nb_lines++;
    }
  fclose(file_input);
  nb_lines=nb_lines-2;

  printf("Number of lines:%d\n",nb_lines);
 
  treeId = malloc((size_t)nb_lines*sizeof(int));
  nodeMass = malloc((size_t)nb_lines*sizeof(double));
  diskStellarMass = malloc((size_t)nb_lines*sizeof(double));
  diskGasMass = malloc((size_t)nb_lines*sizeof(double));
  diskCircularVelocity = malloc((size_t)nb_lines*sizeof(double));
  spheroidStellarMass = malloc((size_t)nb_lines*sizeof(double));
  spheroidGasMass = malloc((size_t)nb_lines*sizeof(double));
  gasFraction = malloc((size_t)nb_lines*sizeof(double));

  file_input = fopen(name_file_input,"r");
  for(i=0; i<5; i++)   
    fscanf(file_input,"%s", char_dumb);
    
  for(i=0; i<nb_lines; i++ ){         
    fscanf(file_input,"%lf  %lf  %lf  %lf  %lf"
	   , &nodeMass[i],  &spheroidStellarMass[i], &spheroidGasMass[i], &diskStellarMass[i], &diskGasMass[i]
	   );        
    gasFraction[i] = diskGasMass[i] + spheroidGasMass[i];
    gasFraction[i] = gasFraction[i]/(diskGasMass[i] + spheroidGasMass[i] + diskStellarMass[i] + spheroidStellarMass[i]);
  }
  fclose(file_input);


////////////////////////////////////////////////////////////////////////////////////////////////////

  file_output = fopen(name_file_output,"w");
  fprintf(file_output, "#1.haloMass   #2.quartil1   #3.median   #4.quartil3   #5.mean  #6.sigma2\n");

  nb_massRanges = 30;
  dmass = (11.0-9.0)/nb_massRanges;


  for(mass=9.0; mass<11.0; mass+=dmass)
    {

      j = 0;
      for(i=0; i<nb_lines; i++  )	
	if(  (log10(diskStellarMass[i] + spheroidStellarMass[i]) >= mass) && (log10(diskStellarMass[i] + spheroidStellarMass[i]) < mass+dmass) 
	     && (gasFraction[i]>0.05) )
	  j++;    
      
      nb_counts = j;    

      gasFractionInRange = malloc((size_t)nb_counts*sizeof(double));

      j = 0;
      for(i=0; i<nb_lines; i++  )	
	if(  (log10(diskStellarMass[i] + spheroidStellarMass[i]) >= mass) && (log10(diskStellarMass[i] + spheroidStellarMass[i]) < mass+dmass) 
	     && (gasFraction[i]>0.05) )
	  {
	    gasFractionInRange[j] = gasFraction[i];
	    j++;
	  } 
  
      gsl_sort(gasFractionInRange, 1, nb_counts);
      mean = gsl_stats_mean (gasFractionInRange, 1,  nb_counts);
      median = gsl_stats_median_from_sorted_data(gasFractionInRange, 1, nb_counts);
      sigma2 = gsl_stats_variance (gasFractionInRange, 1, nb_counts);
      quartil1 = gsl_stats_quantile_from_sorted_data (gasFractionInRange, 1, nb_counts, 0.25);
      quartil3 = gsl_stats_quantile_from_sorted_data (gasFractionInRange, 1, nb_counts, 0.75);

      fprintf(file_output,"%f   %f    %f    %f   %f  %f\n", mass, quartil1, median, quartil3, mean, sigma2);
    }
  fclose(file_output);

  return 0;
}

