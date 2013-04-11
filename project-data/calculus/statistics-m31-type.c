#include <rutines/headers.h>
#include <gsl/gsl_statistics_double.h>


#define HALOmASSrEFF   1.4E12  
#define DISKsTELLARmASSrEF 8.5E10  
#define DISKgASmASSrEF  0.55E10   
#define BULGEsTELLARmASSrEF 2.6E10
#define CIRCULARvELOCITYrEF  275
#define DISKsCALElEGTHrEF   5.4833
#define REPORT   printf("Not enough arguments\nNumber of arguments:%d\n",argc);
#define USAGE    printf("\nUSAGE:\n%s input output\n\n",__FILE__)


int main(int argc, char *argv[])
{
  if(argc<3)
    {
      REPORT;
      USAGE;
      exit(1);
    }
  if(!CheckFile(argv[1]))
    {
      printf("\nfile argv[1]:%s does not exist!.\n",argv[1]);
      exit(1);
    }

  char *name_file_input = argv[1];
  char *name_file_output = argv[2];
  

  int i, nb_lines;
  char char_dumb[100];
  int tree_id, count;
  double node_mass, disk_stellar_mass, spheroid_stellar_mass;
  double disk_gas_mass, spheroid_gas_mass, disk_circular_velocity, spheroid_circular_velocity, disk_scale_length, circular_velocity;

  double delta, eta, relative_error, error_tolerance;

  FILE *file_input, *file_output;  


////////////////////////////////////////////////////////////////////////////////////////////////////
  
  file_input = fopen(name_file_input,"r");

  nb_lines = 0;
  while( !feof(file_input) )
    {  
      for(i=0; i<9; i++)  	
	fscanf(file_input,"%s", char_dumb);      
      
      nb_lines++;
    }
  fclose(file_input);
  nb_lines = nb_lines - 2;

  //printf("Number of lines:%d\n",nb_lines);

////////////////////////////////////////////////////////////////////////////////////////////////////
 
  file_input = fopen(name_file_input,"r");

  file_output = fopen(name_file_output,"w");
  fprintf(file_output, "#1.ID  #2.nodeMass   #3.spheroidStellarMass   #4.spheroidGasMass  #5.diskStellarMass  #6.diskGasMass"
	  "#7.diskCircularVelocity  #8.spheroidCircularVelocity   #9.diskScaleLength   #10.delta   #11.eta-counter\n"
	  );

  error_tolerance = 0.15;
  count = 0;
  for(i=0; i<9; i++)
    fscanf(file_input,"%s", char_dumb);
    
  for(i=0; i<nb_lines; i++ )
    {

      fscanf(file_input,"%d  %lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf"
	     , &tree_id, &node_mass, &spheroid_stellar_mass, &spheroid_gas_mass, &disk_stellar_mass, &disk_gas_mass, &disk_circular_velocity
	     , &spheroid_circular_velocity, &disk_scale_length
	     );

      if(log10(node_mass)>=11.5 && log10(node_mass)<=12.5){
	count++;
	fprintf(file_output,"%d     %e    %e    %e    %e    %e    %lf    %lf    %lf    "
		, tree_id, node_mass, spheroid_stellar_mass, spheroid_gas_mass, disk_stellar_mass, disk_gas_mass, disk_circular_velocity
		, spheroid_circular_velocity, disk_scale_length
		);
            
	delta = 0.0; eta = 0.0;

	relative_error = pow( (spheroid_stellar_mass - BULGEsTELLARmASSrEF)/BULGEsTELLARmASSrEF , 2.0);
	delta += relative_error;
	if(sqrt(relative_error) <= error_tolerance) eta++;

	relative_error =  pow( (disk_stellar_mass - DISKsTELLARmASSrEF)/DISKsTELLARmASSrEF, 2.0 );
	delta += relative_error;
	if(sqrt(relative_error) <= error_tolerance) eta++;

	relative_error =  pow( (disk_gas_mass - DISKgASmASSrEF)/DISKgASmASSrEF, 2.0 );
	delta += relative_error;
	if(sqrt(relative_error) < error_tolerance) eta++;

	/* circular_velocity = disk_circular_velocity > spheroid_circular_velocity ? disk_circular_velocity : spheroid_circular_velocity; */
	/* relative_error = pow( (circular_velocity - CIRCULARvELOCITYrEF)/CIRCULARvELOCITYrEF, 2.0 ); */
	/* delta += relative_error; */
	/* if(sqrt(relative_error) <= error_tolerance) eta++; */
      
	fprintf(file_output," %f     %f\n", sqrt(delta), eta);
      }
    }
  printf("Number of data:%d\n",count);  
  fclose(file_input);
  fclose(file_output);

  return 0;
}

