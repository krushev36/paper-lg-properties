//*************************************************************************************
//                           SIMULATIONAL DATA MANAGER
//*************************************************************************************
//last update:15/09/10


//esta rutina esta desarrollada basicamente para contar el numero de niveles en el arbol
//devuelve el numero de niveles en un entero.
#include<headers.h>

int TreeLevelRead2(char *name_file_imput, void **params)
{

  int i;
  int dumb;
  int tree_id, tree_levels_max;


  int snapshot_level;
  int counter_halo, counter_level;

  int snapshot_halos;


  MergerTree halo_test;

  FILE *file_imput_read;
  file_imput_read = fopen(name_file_imput,"r");

  dumb = fscanf(file_imput_read,"%d", &tree_id);
  dumb = fscanf(file_imput_read,"%d", &tree_levels_max);

  //levels are the number of snapshots
  counter_level = 0;
  while(!feof(file_imput_read))
    {

      dumb = fscanf(file_imput_read,"%d",&snapshot_level);
      dumb = fscanf(file_imput_read,"%d",&snapshot_halos);

      counter_halo = 0;
      while(counter_halo<snapshot_halos)
	{
	  dumb = fscanf(file_imput_read,"%d",&halo_test.id);
	  dumb = fscanf(file_imput_read,"%lf",&halo_test.sfactor);
	  dumb = fscanf(file_imput_read,"%lf %lf",&halo_test.age,&halo_test.mfof);

	  halo_test.position = (double *)malloc((size_t)3*sizeof(double));
	  halo_test.velocity = (double *)malloc((size_t)3*sizeof(double));

	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo_test.position[0], &halo_test.position[1], &halo_test.position[2]);
	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo_test.velocity[0], &halo_test.velocity[1], &halo_test.velocity[2]);

	  dumb = fscanf(file_imput_read,"%lf %lf %lf %lf",&halo_test.rfof, &halo_test.a, &halo_test.b, &halo_test.c);
	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo_test.ek, &halo_test.ep, &halo_test.et);
	  dumb = fscanf(file_imput_read,"%lf",&halo_test.spin);
	  dumb = fscanf(file_imput_read,"%lf %lf %lf %lf",&halo_test.rvir, &halo_test.mvir, &halo_test.tvir, &halo_test.cvel);
	  dumb = fscanf(file_imput_read,"%lf %lf ",&halo_test.rho_0,&halo_test.r_c);
	  dumb = fscanf(file_imput_read,"%d",&halo_test.nb_fathers);


	  halo_test.fathers_id = (int *)malloc((size_t)halo_test.nb_fathers*sizeof(int));

	  dumb = fscanf(file_imput_read,"%d",&halo_test.fathers_id[0]);

	  if(halo_test.nb_fathers>0)
	    {
	      for(i=1; i<halo_test.nb_fathers; i++)
		dumb = fscanf(file_imput_read,"%d",&halo_test.fathers_id[i]);
	    }


	  dumb = fscanf(file_imput_read,"%d",&halo_test.nb_sons);

	  if(halo_test.nb_sons>0)    dumb = fscanf(file_imput_read,"%d",&halo_test.sons_id);

	  if(halo_test.nb_sons>1)
	    {
	      printf("tree data show halo split in %d, this is not allowed\n",halo_test.id);
	      exit(0);
	    }

	  counter_halo++;
	}
      counter_level++;
    }

  fclose(file_imput_read);

  return counter_level;
}


//**************************************************************************************
//this rutine loads all the relevant data to a structure and links fathers to sons.
MergerTree  **TreeDataLoad2(char *name_file_imput, int tree_levels, void **params)
{
  int i, j, k, l;
  int dumb;
  int tree_id, tree_levels_max;
  int snapshot_level, snapshot_halos;

  FILE *file_imput_read;

  MergerTree **halo;
  halo = (MergerTree **)malloc((size_t)tree_levels*sizeof(MergerTree *));

  level *snapshot;

  snapshot = (level *)malloc((size_t)tree_levels*sizeof(level));

  file_imput_read = fopen(name_file_imput,"r");

  dumb = fscanf(file_imput_read,"%d", &tree_id);
  dumb = fscanf(file_imput_read,"%d", &tree_levels_max);

  i=0;
  while(i<tree_levels)
    {


      dumb = fscanf(file_imput_read,"%d",&snapshot_level);
      dumb = fscanf(file_imput_read,"%d",&snapshot_halos);

      snapshot[i].nb = snapshot_level;
      snapshot[i].nb_halos = snapshot_halos;


      halo[i] = (MergerTree *)malloc((size_t)snapshot_halos*sizeof(MergerTree));


      j=0;
      while(j<snapshot[i].nb_halos)
      	{

	  halo[i][j].snapshot = snapshot[i];
	  halo[i][j].level = snapshot[i].nb;

	  dumb = fscanf(file_imput_read,"%d", &halo[i][j].id);
	  dumb = fscanf(file_imput_read,"%lf", &halo[i][j].sfactor);
	  dumb = fscanf(file_imput_read,"%lf %lf", &halo[i][j].age, &halo[i][j].mfof);

	  halo[i][j].position = (double *)malloc((size_t)3*sizeof(double));
	  halo[i][j].velocity = (double *)malloc((size_t)3*sizeof(double));

	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo[i][j].position[0], &halo[i][j].position[1], &halo[i][j].position[2]);
	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo[i][j].velocity[0], &halo[i][j].velocity[1], &halo[i][j].velocity[2]);
	  dumb = fscanf(file_imput_read,"%lf %lf %lf %lf",&halo[i][j].rfof, &halo[i][j].a, &halo[i][j].b, &halo[i][j].c);
	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo[i][j].ek, &halo[i][j].ep, &halo[i][j].et);
	  dumb = fscanf(file_imput_read,"%lf",&halo[i][j].spin);
	  dumb = fscanf(file_imput_read,"%lf %lf %lf %lf",&halo[i][j].rvir, &halo[i][j].mvir, &halo[i][j].tvir, &halo[i][j].cvel);
	  dumb = fscanf(file_imput_read,"%lf %lf ",&halo[i][j].rho_0,&halo[i][j].r_c);
	  dumb = fscanf(file_imput_read,"%d",&halo[i][j].nb_fathers);

	  halo[i][j].fathers_id = (int *)malloc((size_t)halo[i][j].nb_fathers*sizeof(int));

	  dumb = fscanf(file_imput_read,"%d",&halo[i][j].fathers_id[0]);

	  if(halo[i][j].nb_fathers>0)
	    {
	      for(k=1; k<halo[i][j].nb_fathers; k++)
		dumb = fscanf(file_imput_read,"%d",&halo[i][j].fathers_id[k]);
	      
	    }
	     

	  dumb = fscanf(file_imput_read,"%d",&halo[i][j].nb_sons);
	  halo[i][j].halo_son = (MergerTree *)malloc((size_t)halo[i][j].nb_sons*sizeof(MergerTree));


	  if(halo[i][j].nb_sons>0)
	    {
	      dumb = fscanf(file_imput_read,"%d",&halo[i][j].sons_id);
	      
	      //begin:halo->son linking:
	      l = 0;
	      while(l<halo[i-1][0].snapshot.nb_halos)
		{
		  
		  if(halo[i][j].sons_id == halo[i-1][l].id)
		    {

		      halo[i][j].halo_son = &halo[i-1][l];
		      //		      printf("at snapshot %d: halo %d has %d as a son\n"
		      //			     ,halo[i][0].snapshot.nb, halo[i][j].id
		      //			     , halo[i][j].halo_son->id);
		      break;
		    }
		  else
		    halo[i][j].halo_son = NULL; 

		  l++;
		}

	      if(halo[i][j].halo_son == NULL) 
		printf("at snapshot %d: halo %d has no son\n",halo[i][0].snapshot.nb, halo[i][j].id);
	      //end:halo->son linking.

	    }
	  else	    
	    {
	      halo[i][j].halo_son = NULL; 
	      halo[i][j].sons_id = -1; 
	    }
	    

      	  halo[i][j].plot_x = -10000.0;
	  halo[i][j].plot_y = -10000.0;

	  j++;
	}
      i++;
    }

  //begin:halo->fathers linking(v0.1):
  //this links halo to most massive progenitor
  
  for(i=0; i<tree_levels; i++)
    {
      for(j=0; j<halo[i][0].snapshot.nb_halos; j++)
      	{
	  halo[i][j].halo_father = (MergerTree *)malloc((size_t)halo[i][j].nb_fathers*sizeof(MergerTree));
	  if(halo[i][j].nb_fathers > 1)
	    {	     
	      if(i<tree_levels-1)
		{
		  for(l = 0; l<halo[i+1][0].snapshot.nb_halos; l++)
		    {
		      if(halo[i][j].fathers_id[1] == halo[i+1][l].id)
			{			     
			  halo[i][j].halo_father = &halo[i+1][l];
			  //fprintf(stdout,"linked %d -> %d[%d]!\n", halo[i][j].id, halo[i+1][l].id, halo[i][j].halo_father->id);
			  break;
			}
		      else
			halo[i][j].halo_father = NULL;			
		    }
		}
	      else		
		halo[i][j].halo_father = NULL;		
	    }
	  else	    
	    halo[i][j].halo_father = NULL;	  
	}
    }
  
  //end:halo->fathers linking(v0.1).

  //begin:halo->fathers linking(v0.2):
  /*
  for(i=0; i<tree_levels; i++)
    {
      for(j=0; j<halo[i][0].snapshot.nb_halos; j++)
      	{
	  halo[i][j].halo_father = (MergerTree **)malloc((int)halo[i][j].nb_fathers*sizeof(MergerTree *));

	  if(halo[i][j].nb_fathers > 1)
	    {	     
	      if(i < tree_levels - 1)
		{
		  
		  for(l = 0; l<halo[i+1][0].snapshot.nb_halos; l++)
		    {
		      halo[i][j].halo_father[0] = NULL;		      
		      for(k = 1; k<halo[i][j].nb_fathers; k++)
			{
			  //se debe tener en cuenta que esta rutina solo esta recorriedo
			  //los padres correctamente siempre que esten en orden en el 
			  //snapshot, de lo contrario se debe modificar.
			  //halo[i][j].halo_father[k] = (MergerTree *)malloc((int)1*sizeof(MergerTree));
			  if(halo[i][j].fathers_id[k] == halo[i+1][l].id)
			    {			     
			      halo[i][j].halo_father[k] = &halo[i+1][l];
			      fprintf(stdout,"snap %d linked %d -> %d[%d](%d)!\n", i, halo[i][j].id, halo[i+1][l].id, halo[i][j].halo_father[k]->id, k);
			      continue;
			    }
			  else
			    halo[i][j].halo_father[k] = NULL;
			}
		    }		  
		}
	      else
		{

		  for(k = 1; k<halo[i][j].nb_fathers; k++)
		    {
		      //halo[i][j].halo_father[k] = (MergerTree *)malloc((int)1*sizeof(MergerTree));
		      halo[i][j].halo_father[k] = NULL;
		    }
		}
	    }
	  else	    
	    {
	      //halo[i][j].halo_father[0] = (MergerTree *)malloc((int)1*sizeof(MergerTree));
	      halo[i][j].halo_father[0] = NULL;	  
	    }
	}
    }
*/  
  //end:halo->fathers linking.

  fclose(file_imput_read);


  return halo;
}

//**************************************************************************************
//this rutine test while loading all the relevant data to a structure and links fathers to sons.
MergerTree  **TreeDataLoad_Test(char *name_file_imput, int tree_levels, void **params)
{
  int i, j, k, l;
  int dumb;
  int tree_id, tree_levels_max;
  int snapshot_level, snapshot_halos;

  FILE *file_imput_read;

  MergerTree **halo;
  halo = (MergerTree **)malloc((size_t)tree_levels*sizeof(MergerTree *));

  level *snapshot;

  snapshot = (level *)malloc((size_t)tree_levels*sizeof(level));

  file_imput_read = fopen(name_file_imput,"r");

  dumb = fscanf(file_imput_read,"%d", &tree_id);
  fprintf(stdout,"%d\n", tree_id);
  dumb = fscanf(file_imput_read,"%d", &tree_levels_max);
  fprintf(stdout,"%d\n", tree_levels_max);

  i=0;
  while(i<tree_levels)
    {

      dumb = fscanf(file_imput_read,"%d",&snapshot_level);
      fprintf(stdout,"%d\n",snapshot_level);
      dumb = fscanf(file_imput_read,"%d",&snapshot_halos);
      fprintf(stdout,"%d\n",snapshot_halos);

      fprintf(stdout,"\n");

      snapshot[i].nb = snapshot_level;
      snapshot[i].nb_halos = snapshot_halos;


      halo[i] = (MergerTree *)malloc((size_t)snapshot_halos*sizeof(MergerTree));


      j=0;
      while(j<snapshot[i].nb_halos)
      	{

	  halo[i][j].snapshot = snapshot[i];
	  halo[i][j].level = snapshot[i].nb;

	  dumb = fscanf(file_imput_read,"%d", &halo[i][j].id);
	  fprintf(stdout,"%d\n", halo[i][j].id);

	  dumb = fscanf(file_imput_read,"%lf", &halo[i][j].sfactor);
	  fprintf(stdout,"%.6lf\n", halo[i][j].sfactor);

	  dumb = fscanf(file_imput_read,"%lf %lf", &halo[i][j].age, &halo[i][j].mfof);
	  fprintf(stdout,"%.5lf %.7E\n", halo[i][j].age, halo[i][j].mfof);

	  halo[i][j].position = (double *)malloc((size_t)3*sizeof(double));
	  halo[i][j].velocity = (double *)malloc((size_t)3*sizeof(double));

	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo[i][j].position[0], &halo[i][j].position[1], &halo[i][j].position[2]);
	  fprintf(stdout,"%.5lf %.5lf %.5lf\n", halo[i][j].position[0], halo[i][j].position[1], halo[i][j].position[2]);

	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo[i][j].velocity[0], &halo[i][j].velocity[1], &halo[i][j].velocity[2]);
	  fprintf(stdout,"%.5lf %.5lf %.5lf\n", halo[i][j].velocity[0], halo[i][j].velocity[1], halo[i][j].velocity[2]);

	  dumb = fscanf(file_imput_read,"%lf %lf %lf %lf",&halo[i][j].rfof, &halo[i][j].a, &halo[i][j].b, &halo[i][j].c);
	  fprintf(stdout,"%.7E %.7E %.7E %.7E\n", halo[i][j].rfof, halo[i][j].a, halo[i][j].b, halo[i][j].c);

	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo[i][j].ek, &halo[i][j].ep, &halo[i][j].et);
	  fprintf(stdout,"%.7E %.4lf %.4lf\n", halo[i][j].ek, halo[i][j].ep, halo[i][j].et);

	  dumb = fscanf(file_imput_read,"%lf",&halo[i][j].spin);
	  fprintf(stdout,"%.7E\n", halo[i][j].spin);

	  dumb = fscanf(file_imput_read,"%lf %lf %lf %lf",&halo[i][j].rvir, &halo[i][j].mvir, &halo[i][j].tvir, &halo[i][j].cvel);
	  fprintf(stdout,"%.7E %.7E %.2lf %.5lf\n", halo[i][j].rvir, halo[i][j].mvir, halo[i][j].tvir, halo[i][j].cvel);

	  dumb = fscanf(file_imput_read,"%lf %lf ",&halo[i][j].rho_0,&halo[i][j].r_c);
	  fprintf(stdout,"%.5lf %.7E \n", halo[i][j].rho_0, halo[i][j].r_c);

	  dumb = fscanf(file_imput_read,"%d",&halo[i][j].nb_fathers);
	  fprintf(stdout,"%d\n", halo[i][j].nb_fathers);

	  halo[i][j].fathers_id = (int *)malloc((size_t)halo[i][j].nb_fathers*sizeof(int));

	  dumb = fscanf(file_imput_read,"%d",&halo[i][j].fathers_id[0]);
	  fprintf(stdout,"%d  ", halo[i][j].fathers_id[0]);

	  if(halo[i][j].nb_fathers>0)
	    {
	      for(k=1; k<halo[i][j].nb_fathers; k++)
		dumb = fscanf(file_imput_read,"%d",&halo[i][j].fathers_id[k]);
	      fprintf(stdout,"%d  ", halo[i][j].fathers_id[k]);
	    }
	  fprintf(stdout,"\n");

	  dumb = fscanf(file_imput_read,"%d",&halo[i][j].nb_sons);
	  fprintf(stdout,"%d\n", halo[i][j].nb_sons);

	  halo[i][j].halo_son = (MergerTree *)malloc((size_t)halo[i][j].nb_sons*sizeof(MergerTree));

	  if(halo[i][j].nb_sons>0)
	    {

	      dumb = fscanf(file_imput_read,"%d",&halo[i][j].sons_id);
	      fprintf(stdout,"%d\n", halo[i][j].sons_id);

	      //begin:halo->son linking:
	      l=0;
	      while(l<halo[i-1][0].snapshot.nb_halos)
		{

		  if(halo[i][j].sons_id == halo[i-1][l].id)
		    {

		      halo[i][j].halo_son = &halo[i-1][l];
		      //		      printf("at snapshot %d: halo %d has %d as a son\n"
		      //			     ,halo[i][0].snapshot.nb, halo[i][j].id
		      //			     , halo[i][j].halo_son->id);
		      break;
		    }
		  else
		    halo[i][j].halo_son = NULL;

		  l++;
		}
	      //end:halo->son linking.

	      if(halo[i][j].halo_son == NULL)
		printf("at snapshot %d: halo %d has no son\n",halo[i][0].snapshot.nb, halo[i][j].id);

	    }
	  else
	    halo[i][j].halo_son = NULL;



	  halo[i][j].plot_x = -10000.0;
	  halo[i][j].plot_y = -10000.0;

	  j++;
	}
      fprintf(stdout,"\n");
      i++;
    }

  fclose(file_imput_read);

  return halo;
}



//**************************************************************************************
//this rutine test while loading all the relevant data to a structure and links fathers to sons.
MergerTree  **TreeDataLoad_CopyTest(char *name_file_imput, int tree_levels, char *name_file_output, void **params)
{
  int i, j, k, l;
  int dumb;
  int tree_id, tree_levels_max;
  int snapshot_level, snapshot_halos;

  FILE *file_imput_read, *file_output_write;


  MergerTree **halo;
  halo = (MergerTree **)malloc((size_t)tree_levels*sizeof(MergerTree *));

  level *snapshot;

  snapshot = (level *)malloc((size_t)tree_levels*sizeof(level));

  file_imput_read = fopen(name_file_imput,"r");
  file_output_write = fopen(name_file_output,"w");

  dumb = fscanf(file_imput_read,"%d", &tree_id);
  fprintf(file_output_write,"%12d\n", tree_id);
  dumb = fscanf(file_imput_read,"%d", &tree_levels_max);
  fprintf(file_output_write,"%12d\n\n", tree_levels_max);

  i=0;
  while(i<tree_levels)
    {

      dumb = fscanf(file_imput_read,"%d",&snapshot_level);
      fprintf(file_output_write,"%12d\n",snapshot_level);
      dumb = fscanf(file_imput_read,"%d",&snapshot_halos);
      fprintf(file_output_write,"%12d\n",snapshot_halos);

      fprintf(file_output_write,"\n");

      snapshot[i].nb = snapshot_level;
      snapshot[i].nb_halos = snapshot_halos;


      halo[i] = (MergerTree *)malloc((size_t)snapshot_halos*sizeof(MergerTree));


      j=0;
      while(j<snapshot[i].nb_halos)
      	{

	  halo[i][j].snapshot = snapshot[i];
	  halo[i][j].level = snapshot[i].nb;

	  dumb = fscanf(file_imput_read,"%d", &halo[i][j].id);
	  fprintf(file_output_write,"%12d\n", halo[i][j].id);

	  dumb = fscanf(file_imput_read,"%lf", &halo[i][j].sfactor);
	  fprintf(file_output_write,"%11.6lf\n", halo[i][j].sfactor);

	  dumb = fscanf(file_imput_read,"%lf %lf", &halo[i][j].age, &halo[i][j].mfof);
	  fprintf(file_output_write,"%11.5lf \n%15.7E\n", halo[i][j].age, halo[i][j].mfof);

	  halo[i][j].position = (double *)malloc((size_t)3*sizeof(double));
	  halo[i][j].velocity = (double *)malloc((size_t)3*sizeof(double));

	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo[i][j].position[0], &halo[i][j].position[1], &halo[i][j].position[2]);
	  fprintf(file_output_write,"%11.5lf    %11.5lf    %11.5lf\n", halo[i][j].position[0], halo[i][j].position[1], halo[i][j].position[2]);

	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo[i][j].velocity[0], &halo[i][j].velocity[1], &halo[i][j].velocity[2]);
	  fprintf(file_output_write,"%11.4lf    %11.4lf    %11.4lf\n", halo[i][j].velocity[0], halo[i][j].velocity[1], halo[i][j].velocity[2]);

	  dumb = fscanf(file_imput_read,"%lf %lf %lf %lf",&halo[i][j].rfof, &halo[i][j].a, &halo[i][j].b, &halo[i][j].c);
	  fprintf(file_output_write,"%11.7f    %11.7f    %11.7f    %11.7f\n", halo[i][j].rfof, halo[i][j].a, halo[i][j].b, halo[i][j].c);

	  dumb = fscanf(file_imput_read,"%lf %lf %lf",&halo[i][j].ek, &halo[i][j].ep, &halo[i][j].et);
	  fprintf(file_output_write,"%15.7E     %11f     %11f\n", halo[i][j].ek, halo[i][j].ep, halo[i][j].et);

	  dumb = fscanf(file_imput_read,"%lf",&halo[i][j].spin);
	  fprintf(file_output_write,"%11g\n", halo[i][j].spin);

	  dumb = fscanf(file_imput_read,"%lf %lf %lf %lf",&halo[i][j].rvir, &halo[i][j].mvir, &halo[i][j].tvir, &halo[i][j].cvel);
	  fprintf(file_output_write,"%9E %9E %8f %8f\n", halo[i][j].rvir, halo[i][j].mvir, halo[i][j].tvir, halo[i][j].cvel);

	  dumb = fscanf(file_imput_read,"%lf %lf ",&halo[i][j].rho_0,&halo[i][j].r_c);
	  fprintf(file_output_write,"%.5lf %.7E \n", halo[i][j].rho_0, halo[i][j].r_c);

	  dumb = fscanf(file_imput_read,"%d",&halo[i][j].nb_fathers);
	  fprintf(file_output_write,"%12d\n", halo[i][j].nb_fathers);

	  halo[i][j].fathers_id = (int *)malloc((size_t)halo[i][j].nb_fathers*sizeof(int));

	  dumb = fscanf(file_imput_read,"%d",&halo[i][j].fathers_id[0]);
	  //fprintf(file_output_write,"%12d  ", halo[i][j].fathers_id[0]);

	  if(halo[i][j].nb_fathers>0)
	    {
	      for(k=1; k<halo[i][j].nb_fathers; k++)		
		dumb = fscanf(file_imput_read,"%d",&halo[i][j].fathers_id[k]);

	      k=0;
	      while( k<halo[i][j].nb_fathers)		
		{
		  fprintf(file_output_write,"%12d", halo[i][j].fathers_id[k]);
		  k++;	if(k>=halo[i][j].nb_fathers) { fprintf(file_output_write,"\n"); break;}
		  fprintf(file_output_write,"%12d", halo[i][j].fathers_id[k]);
		  k++;	if(k>=halo[i][j].nb_fathers) { fprintf(file_output_write,"\n"); break;}
		  fprintf(file_output_write,"%12d", halo[i][j].fathers_id[k]);
		  k++;	if(k>=halo[i][j].nb_fathers) { fprintf(file_output_write,"\n"); break;}
		  fprintf(file_output_write,"%12d", halo[i][j].fathers_id[k]);
		  k++;	if(k>=halo[i][j].nb_fathers) { fprintf(file_output_write,"\n"); break;}
		  fprintf(file_output_write,"%12d", halo[i][j].fathers_id[k]);
		  k++;	if(k>=halo[i][j].nb_fathers) { fprintf(file_output_write,"\n"); break;}
		  fprintf(file_output_write,"%12d\n", halo[i][j].fathers_id[k]);
		  k++;	
		}

	    }

	  dumb = fscanf(file_imput_read,"%d",&halo[i][j].nb_sons);
	  fprintf(file_output_write,"%12d\n", halo[i][j].nb_sons);

	  halo[i][j].halo_son = (MergerTree *)malloc((size_t)halo[i][j].nb_sons*sizeof(MergerTree));

	  if(halo[i][j].nb_sons>0)
	    {

	      dumb = fscanf(file_imput_read,"%d",&halo[i][j].sons_id);
	      fprintf(file_output_write,"%12d\n", halo[i][j].sons_id);

	      //son-father linking
	      l=0;
	      while(l<halo[i-1][0].snapshot.nb_halos)
		{

		  if(halo[i][j].sons_id == halo[i-1][l].id)
		    {

		      halo[i][j].halo_son = &halo[i-1][l];
		      //		      printf("at snapshot %d: halo %d has %d as a son\n"
		      //			     ,halo[i][0].snapshot.nb, halo[i][j].id
		      //			     , halo[i][j].halo_son->id);
		      break;
		    }
		  else
		    halo[i][j].halo_son = NULL;

		  l++;
		}

	      if(halo[i][j].halo_son == NULL)
		printf("at snapshot %d: halo %d has no son\n",halo[i][0].snapshot.nb, halo[i][j].id);

	    }
	  else
	    halo[i][j].halo_son = NULL;

	  halo[i][j].plot_x = -10000.0;
	  halo[i][j].plot_y = -10000.0;

	  fprintf(file_output_write,"\n");
	  j++;
	}
      fprintf(file_output_write,"\n\n");
      i++;
    }

  fclose(file_imput_read);
  fclose(file_output_write);

  return halo;
}

//**************************************************************************************
//this routine defines a second ID to each halo in the input tree. this second ID is 
//defined to be unique inside the merger tree.
void TreeAliasingID(MergerTree **halo, int tree_levels)
{
  int i, j, nb_node = 0;
  for(i = 0; i < tree_levels; i++)
    for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
      {
	halo[i][j].idAlias = nb_node;
	nb_node++;
      }
  
  for(i = 0; i < tree_levels; i++)
    for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
      {
	if(halo[i][j].nb_sons > 0)
	  halo[i][j].son_idAlias = halo[i][j].halo_son->idAlias;
	else
	  halo[i][j].son_idAlias = -1;
	
	if(i < tree_levels-1)
	  {
	    if(halo[i][j].nb_fathers > 1)
	      halo[i][j].father_idAlias = halo[i][j].halo_father->idAlias; //links father_idAlias to the most massive progenitor
	    else
	      halo[i][j].father_idAlias = -1;
	  }
	else
	  halo[i][j].father_idAlias = -1;	
      }
}

//**************************************************************************************
//this routine defines a second ID to each halo in the input tree. this second ID is 
//defined to be unique inside the merger tree, this new version also stores the number 
//of nodes in the tree.
int TreeAliasingID2(MergerTree **halo, int tree_levels)
{
  int i, j, nb_node = 0;
  for(i = 0; i < tree_levels; i++)
    for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
      {
	halo[i][j].idAlias = nb_node;
	nb_node++;
      }
  
  for(i = 0; i < tree_levels; i++)
    for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
      {
	if(halo[i][j].nb_sons > 0)
	  halo[i][j].son_idAlias = halo[i][j].halo_son->idAlias;
	else
	  halo[i][j].son_idAlias = -1;
	
	if(i < tree_levels-1)
	  {
	    if(halo[i][j].nb_fathers > 1)
	      halo[i][j].father_idAlias = halo[i][j].halo_father->idAlias; //links father_idAlias to the most massive progenitor
	    else
	      halo[i][j].father_idAlias = -1;
	  }
	else
	  halo[i][j].father_idAlias = -1;	
      }
  return nb_node;
}






//**************************************************************************************
//this rutine is used to generate initial plot_positions for any halos at any snapshot
floatptr1 TreePlotSeed(int nb_halos,float nb_halo_largest)
{

  int i, k;
  float step;
  floatptr1 halos_position;

  halos_position = (float *)malloc((size_t)nb_halos*sizeof(float));


  step = nb_halos/nb_halo_largest;

  if( GSL_IS_EVEN(nb_halos) == 1 )
    {
      i=0; k=1;
      while(i<nb_halos)
	{
	  halos_position[i] = k*step;
	  i++;
	  halos_position[i] = -k*step;
	  i++; k++;
	}

    }
  else
    {
      i=0;
      halos_position[i] = 0.0;

      i++; k=1;
      while(i<nb_halos)
	{
	  halos_position[i] = k*step;
	  i++;
	  halos_position[i] = -k*step;
	  i++; k++;
	}

    }

  return halos_position;
}

