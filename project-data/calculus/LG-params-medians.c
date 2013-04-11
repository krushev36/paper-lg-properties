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

  int *treeId;

  double param, dparam;

  double  gasFracMedian, gasFracQuartil1, gasFracQuartil3, gasFracSigma2;
  double  bulgeFracMedian, bulgeFracQuartil1, bulgeFracQuartil3, bulgeFracSigma2;
  double  starFracMedian, starFracQuartil1, starFracQuartil3, starFracSigma2;

  double treeid, paramvalue, nodemass, spheroidstellarmass, spheroidgasmass, diskstellarmass,
    diskgasmass, diskcircularvelocity, spheroidcircularvelocity, diskscalelength;

  double *paramValue;
  double *gasFrac, *starFrac, *bulgeFrac;
  double  *nodeMass, *spheroidStellarMass, *spheroidGasMass,*diskStellarMass,
    *diskGasMass, *diskCircularVelocity, *spheroidCircularVelocity, *diskScaleLength;


  FILE *file_input, *file_output;  
////////////////////////////////////////////////////////////////////////////////////////////////////
  file_input = fopen(name_file_input,"r");

  nb_lines = 0;
  while( !feof(file_input) )
    {  
      for(i=0; i<10; i++)  	
	fscanf(file_input,"%s", char_dumb);      
    
    nb_lines++;
    }
  fclose(file_input);
  nb_lines=nb_lines-2;

  //printf("Number of lines:%d\n",nb_lines);
 
////////////////////////////////////////////////////////////////////////////////////////////////////

  file_output = fopen(name_file_output,"w");
  fprintf(file_output, "#1.starFormEff  #2.haloMass  #3.gasFracMedian #4.gasFracQuartil1 #5.gasFracQuartil3 #6.gasFracSigma2"
	  "#7.bulgeFracMedian #8.bulgeFracQuartil1 #9.bulgeFracQuartil3 #10.bulgeFracSigma2"
	  "#11.starFracMedian #12.starFracQuartil1 #13.starFracQuartil3 #14.starFracSigma2\n"
	  );

  dparam=0.1/20.0;
  for(param=0.005; param<=0.1; param+=dparam){ 
    file_input = fopen(name_file_input,"r");
    for(i=0; i<10; i++)
      fscanf(file_input,"%s", char_dumb);
    
    j = 0;
    for(i=0; i<nb_lines; i++  ){
      fscanf(file_input,"%lf  %lf  %lf  %lf  %lf %lf  %lf  %lf  %lf  %lf",
    	     &treeid, &paramvalue, &nodemass, &spheroidstellarmass, &spheroidgasmass, &diskstellarmass,
    	     &diskgasmass, &diskcircularvelocity, &spheroidcircularvelocity, &diskscalelength
    	     );
      if( (paramvalue >= param-0.0001) &&  (paramvalue <= param+0.0001)) 	
	j++;
    }
    nb_counts = j;
    printf("range+:%lf   range-:%lf  disk-outflow-exp:%.10f   nb=%d\n",param-0.001,param+0.001,param, nb_counts);
    fclose(file_input);

    /* nodeMass = malloc((size_t)nb_counts*sizeof(double)); */
    /* diskStellarMass = malloc((size_t)nb_counts*sizeof(double)); */
    /* diskGasMass = malloc((size_t)nb_counts*sizeof(double)); */
    /* diskCircularVelocity = malloc((size_t)nb_counts*sizeof(double)); */
    /* spheroidCircularVelocity = malloc((size_t)nb_counts*sizeof(double)); */
    /* spheroidStellarMass = malloc((size_t)nb_counts*sizeof(double)); */
    /* spheroidGasMass = malloc((size_t)nb_counts*sizeof(double)); */
    /* diskScaleLength = malloc((size_t)nb_counts*sizeof(double)); */

    gasFrac = malloc((size_t)nb_counts*sizeof(double));
    starFrac = malloc((size_t)nb_counts*sizeof(double));
    bulgeFrac = malloc((size_t)nb_counts*sizeof(double));


    file_input = fopen(name_file_input,"r");
    for(i=0; i<10; i++)
    	fscanf(file_input,"%s", char_dumb);
      
    j = 0;
    for(i=0; i<nb_lines; i++  ){
    	fscanf(file_input,"%lf  %lf  %lf  %lf  %lf %lf  %lf  %lf  %lf  %lf",
    	       &treeid, &paramvalue, &nodemass, &spheroidstellarmass, &spheroidgasmass, &diskstellarmass,
    	       &diskgasmass, &diskcircularvelocity, &spheroidcircularvelocity, &diskscalelength
    	       );
	if( (paramvalue >= param-0.0001)  &&  (paramvalue <= param+0.0001)){	
    	  /* spheroidStellarMass[j]=spheroidstellarmass; */
    	  /* spheroidGasMass[j]= spheroidgasmass; */
    	  /* diskStellarMass[j]= diskstellarmass; */
    	  /* diskGasMass[j]= diskgasmass; */
    	  /* diskCircularVelocity[j]= diskcircularvelocity; */
    	  /* spheroidCircularVelocity[j]=spheroidcircularvelocity; */
    	  /* diskScaleLength[j]= diskscalelength; */
	  gasFrac[j] = (spheroidgasmass + diskgasmass)/(spheroidstellarmass + spheroidgasmass + diskstellarmass + diskgasmass );
	  bulgeFrac[j] = (spheroidgasmass +spheroidstellarmass)/(spheroidstellarmass + spheroidgasmass + diskstellarmass + diskgasmass );
	  starFrac[j] = (spheroidstellarmass + diskstellarmass)/nodemass;
    	  j++;
    	}
    }
    fclose(file_input);

    gsl_sort(gasFrac, 1, nb_counts);       
    gasFracMedian = gsl_stats_median_from_sorted_data(gasFrac, 1, nb_counts);
    gasFracQuartil1 = gsl_stats_quantile_from_sorted_data (gasFrac, 1, nb_counts, 0.25);
    gasFracQuartil3 = gsl_stats_quantile_from_sorted_data (gasFrac, 1, nb_counts, 0.75);
    gasFracSigma2 = gsl_stats_variance(gasFrac, 1, nb_counts);

    gsl_sort(bulgeFrac, 1, nb_counts);       
    bulgeFracMedian = gsl_stats_median_from_sorted_data(bulgeFrac, 1, nb_counts);
    bulgeFracQuartil1 = gsl_stats_quantile_from_sorted_data (bulgeFrac, 1, nb_counts, 0.25);
    bulgeFracQuartil3 = gsl_stats_quantile_from_sorted_data (bulgeFrac, 1, nb_counts, 0.75);
    bulgeFracSigma2 = gsl_stats_variance(bulgeFrac, 1, nb_counts);

    gsl_sort(starFrac, 1, nb_counts);       
    starFracMedian = gsl_stats_median_from_sorted_data(starFrac, 1, nb_counts);
    starFracQuartil1 = gsl_stats_quantile_from_sorted_data (starFrac, 1, nb_counts, 0.25);
    starFracQuartil3 = gsl_stats_quantile_from_sorted_data (starFrac, 1, nb_counts, 0.75);
    starFracSigma2 = gsl_stats_variance(starFrac, 1, nb_counts);



    fprintf(file_output,"%lf   %e   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf\n"
	    , param, nodemass, gasFracMedian, gasFracQuartil1, gasFracQuartil3, gasFracSigma2
	    , bulgeFracMedian, bulgeFracQuartil1, bulgeFracQuartil3, bulgeFracSigma2
	    , starFracMedian, starFracQuartil1, starFracQuartil3, starFracSigma2
	    );
  }
  fclose(file_output);


  return 0;
}












