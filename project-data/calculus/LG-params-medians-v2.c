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


  float  paramvalue1, paramvalue2;
  double param1, dparam1, param2, dparam2;

  double  gasFracMedian, gasFracQuartil1, gasFracQuartil3, gasFracSigma2;
  double  bulgeFracMedian, bulgeFracQuartil1, bulgeFracQuartil3, bulgeFracSigma2;
  double  starFracMedian, starFracQuartil1, starFracQuartil3, starFracSigma2;

  double treeid, nodemass, spheroidstellarmass, spheroidgasmass, diskstellarmass,
    diskgasmass, diskcircularvelocity, spheroidcircularvelocity, diskscalelength;

  double *gasFrac, *starFrac, *bulgeFrac;
  


  FILE *file_input, *file_output;  
////////////////////////////////////////////////////////////////////////////////////////////////////
  file_input = fopen(name_file_input,"r");

  nb_lines = 0;
  while( !feof(file_input) )
    {  
      for(i=0; i<11; i++)  	
	fscanf(file_input,"%s", char_dumb);      
    
    nb_lines++;
    }
  fclose(file_input);
  nb_lines=nb_lines-2;

  printf("Number of lines:%d\n",nb_lines);
 
////////////////////////////////////////////////////////////////////////////////////////////////////

  file_output = fopen(name_file_output,"w");
  fprintf(file_output, "#1.starFormEff  #2.alphaExpo #3.haloMass  #4.gasFracMedian #5.gasFracQuartil1 #6.gasFracQuartil3 #7.gasFracSigma2"
	  "#8.bulgeFracMedian #9.bulgeFracQuartil1 #10.bulgeFracQuartil3 #11.bulgeFracSigma2"
	  "#12.starFracMedian #13.starFracQuartil1 #14.starFracQuartil3 #15.starFracSigma2\n"
	  );

  dparam1=0.01;   dparam2=0.15;
  for(param1=0.01; param1<=0.1; param1+=dparam1){ 
    for(param2=1.5; param2<=3.0; param2+=dparam2){ 

      //param1=0.1; param2=3.0;
      file_input = fopen(name_file_input,"r");
      for(i=0; i<11; i++)
	fscanf(file_input,"%s", char_dumb);
    
      j = 0;
      for(i=0; i<nb_lines; i++  ){
	fscanf(file_input,"%lf  %f  %f  %lf  %lf %lf  %lf  %lf  %lf  %lf %lf",
	       &treeid, &paramvalue1, &paramvalue2, &nodemass, &spheroidstellarmass, &spheroidgasmass, &diskstellarmass,
	       &diskgasmass, &diskcircularvelocity, &spheroidcircularvelocity, &diskscalelength
	       );
	
	if( (paramvalue1 >= param1-0.001) && (paramvalue1 <= param1+0.001) && (paramvalue2 >= param2-0.001) && (paramvalue2 <= param2+0.001)) 	
	  j++;	
	//printf("%lf  %lf  %lf   %lf\n", param1, param2, paramvalue1, paramvalue2);
	 
	 
      }
      nb_counts = j;
      printf("star-disk-eff:%.10f   disk-outflow-exp:%.10f   nb=%d\n", param1, param2, nb_counts);
      fclose(file_input);

    
      gasFrac = malloc((size_t)nb_counts*sizeof(double));
      starFrac = malloc((size_t)nb_counts*sizeof(double));
      bulgeFrac = malloc((size_t)nb_counts*sizeof(double));


      file_input = fopen(name_file_input,"r");
      for(i=0; i<11; i++)
    	fscanf(file_input,"%s", char_dumb);
      
      j = 0;
      for(i=0; i<nb_lines; i++  ){
    	fscanf(file_input,"%lf  %f  %f  %lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf",
    	       &treeid, &paramvalue1, &paramvalue2, &nodemass, &spheroidstellarmass, &spheroidgasmass, &diskstellarmass,
    	       &diskgasmass, &diskcircularvelocity, &spheroidcircularvelocity, &diskscalelength
    	       );
	if( (paramvalue1 >= param1-0.001) && (paramvalue1 <= param1+0.001) && (paramvalue2 >= param2-0.001) && (paramvalue2 <= param2+0.001)){	
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



      fprintf(file_output,"%lf   %lf   %e   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf   %lf\n"
	      , param1, param2, nodemass, gasFracMedian, gasFracQuartil1, gasFracQuartil3, gasFracSigma2
	      , bulgeFracMedian, bulgeFracQuartil1, bulgeFracQuartil3, bulgeFracSigma2
	      , starFracMedian, starFracQuartil1, starFracQuartil3, starFracSigma2
	      );
    }
  }
  fclose(file_output);


  return 0;
}












