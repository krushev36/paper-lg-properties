/*
SIMULATIONAL PROPERTIES RUTINES:
*/
#include<headers.h>

void FPrintDensityContrast(int nb_halos, properties *halo1, float radio, char *file_name_out)
{
  int i, j;
  
  float distance, density_8;
  double count_mass;
  
  FILE *file_write_data=fopen(file_name_out,"w");
  
  fprintf(file_write_data,"#halo_id\t #halo_mass_vir\t #halo_delta\n");
  
  i=0;
  while(i<nb_halos)
    {
      j=0; count_mass = 0.0;
      while(j<nb_halos)
	{
	  distance = sqrt(   (halo1[i].pos_x - halo1[j].pos_x)*(halo1[i].pos_x - halo1[j].pos_x)
			   + (halo1[i].pos_y - halo1[j].pos_y)*(halo1[i].pos_y - halo1[j].pos_y)
			   + (halo1[i].pos_z - halo1[j].pos_z)*(halo1[i].pos_z - halo1[j].pos_z) );
	  
	  if(distance<=radio)
	    {
	      count_mass = count_mass + halo1[j].mass_vir;
	    }
	  
	  j++;
	}
      density_8 = count_mass/(radio*radio*radio);

      halo1[i].delta = density_8/(CRITICAL_DENSITY*OMEGA_M)-1.0;

      fprintf(file_write_data,"%d\t %e\t %f\n", halo1[i].id, halo1[i].mass_vir, halo1[i].delta);
      
      i++;
    }  

  fclose(file_write_data);  

}
////////////////////////////////////////////////////////////////////////////////////////////////////////

void DensityContrast(int nb_halos, properties *halo1, float radio)
{
  int i, j;
  
  float distance, density_8;
  double count_mass;
  
  
  i=0;
  while(i<nb_halos)
    {
      j=0; count_mass = 0.0;
      while(j<nb_halos)
	{
	  distance = sqrt(   (halo1[i].pos_x - halo1[j].pos_x)*(halo1[i].pos_x - halo1[j].pos_x)
			   + (halo1[i].pos_y - halo1[j].pos_y)*(halo1[i].pos_y - halo1[j].pos_y)
			   + (halo1[i].pos_z - halo1[j].pos_z)*(halo1[i].pos_z - halo1[j].pos_z) );
	  
	  if(distance<=radio)
	    {
	      count_mass = count_mass + halo1[j].mass_vir;
	    }
	  
	  j++;
	}
      density_8 = count_mass/(radio*radio*radio);

      halo1[i].delta = density_8/(CRITICAL_DENSITY*OMEGA_M)-1.0;
      
      i++;
    }  

}


////////////////////////////////////////////////////////////////////////////////////////////////////////

float HaloDensityContrast(int nb_halos, properties *halo1, int id,float radio)
{
  int i, j;
  
  float distance, density_8;
  double count_mass;
  
  //  FILE *file_write_data=fopen(file_name_out,"w");
  
  //  fprintf(file_write_data,"#halo_id\t #halo_delta_8\n");
  
  i=id-1;
  //  while(i<nb_halos)
  //    {
      j=0; count_mass = 0.0;
      while(j<nb_halos)
	{
	  distance = sqrt(   (halo1[i].pos_x - halo1[j].pos_x)*(halo1[i].pos_x - halo1[j].pos_x)
			   + (halo1[i].pos_y - halo1[j].pos_y)*(halo1[i].pos_y - halo1[j].pos_y)
			   + (halo1[i].pos_z - halo1[j].pos_z)*(halo1[i].pos_z - halo1[j].pos_z) );
	  
	  if(distance<=radio)
	    {
	      count_mass = count_mass + halo1[j].mass_vir;
	    }
	  
	  j++;
	}
      density_8 = count_mass/(radio*radio*radio);

      //      halo1[i].delta = density_8/(CRITICAL_DENSITY*OMEGA_M)-1.0;

      //      fprintf(file_write_data,"%d\t %f \n", halo1[i].id, halo1[i].delta);
      
      //      i++;
      //    }  

      //  fclose(file_write_data);  

      return density_8/(CRITICAL_DENSITY*OMEGA_M)-1.0;

}

void FPrintDensityContrastSubBox(int nb_halos, properties *halo1, float boxsize, float radio, char *file_name_out)
{
  int i, j;
  
  float distance, density_8;
  double count_mass;
  
  FILE *file_write_data=fopen(file_name_out,"w");
  
  fprintf(file_write_data,"#halo_id\t #halo_mass_vir\t #halo_delta\n");
  
  i=0;
  while(i<nb_halos)
    {

      if(halo1[i].pos_x>radio && halo1[i].pos_y>radio && halo1[i].pos_z>radio
	 && halo1[i].pos_x+radio<boxsize && halo1[i].pos_z+radio<boxsize && halo1[i].pos_z+radio<boxsize)
	{
	  j=0; count_mass = 0.0;
	  while(j<nb_halos)
	    {

	      distance = sqrt(   (halo1[i].pos_x - halo1[j].pos_x)*(halo1[i].pos_x - halo1[j].pos_x)
				 + (halo1[i].pos_y - halo1[j].pos_y)*(halo1[i].pos_y - halo1[j].pos_y)
				 + (halo1[i].pos_z - halo1[j].pos_z)*(halo1[i].pos_z - halo1[j].pos_z) );
	  
	      if(distance<=radio)		
		count_mass = count_mass + halo1[j].mass_vir;
			  
	      j++;
	    }

	  density_8 = count_mass/(radio*radio*radio);
	  halo1[i].delta = density_8/(CRITICAL_DENSITY*OMEGA_M)-1.0;

	  fprintf(file_write_data,"%d\t %e\t %f\n", halo1[i].id, halo1[i].mass_vir, halo1[i].delta);
	}
      i++;
    }  

  fclose(file_write_data);  

}
////////////////////////////////////////////////////////////////////////////////////////////////////////
