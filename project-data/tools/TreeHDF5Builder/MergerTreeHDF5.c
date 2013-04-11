/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  Readme:
  this code reads (complete) merger tree histories extract
  ed from a merger trees catalogue. Builds the merger tree
  HDF5 structure requiered by Galacticus Code (Benson 2009
  ). 
  If interested in building HDF5 merger trees between diff
  erent initial and final snapshots than provided by sourc
  e merger tree, these can be setted via command line by p
  assing

  $export initLevel (int)init
  $export endLevel (int)end
  
  where init_snap is the array counter number of the level
  or snpashot of interest, it means 
  snpashot[init] = init_snap
  snpashot[end] = end_snap.

  Note:
  this code do not make association to sibling nodes. each
  sibling node ID stored is setted to -1.
  remove environment variables using "unset" command.

  Run:
  ./MergerTreeHDF5.out (char *)imput_tree (char *)output_tree (int)tree_number

  °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/
#include <headers.h>

#define USAGE printf("\nUSAGE:\n      %s  (char *)input_file (char *)output_file  (int)tree_number\n\n",__FILE__)
#define LENGTH  91.428
#define SUBLENGTH 20.0
#define NTREES 173

void HDF5_NodeAliasIndex(char *name_file, int tree, MergerTree **halo, int tree_levels);
void HDF5_ParentAliasNode(char *name_file, int tree, MergerTree **halo, int tree_levels);
void HDF5_ChildAliasNode(char *name_file, int tree, MergerTree **halo, int tree_levels);
void HDF5_SiblingAliasNode(char *name_file,int tree, MergerTree **halo, int tree_levels);
void HDF5_NodeRedshift(char *name_file, int tree, MergerTree **halo, int tree_levels);
void HDF5_NodeMass(char *name_file, int tree, MergerTree **halo, int tree_levels);
void HDF5_SiblingNode(char *name_file, int tree, MergerTree **halo, int tree_levels);
void HDF5_ChildNode(char *name_file, int tree, MergerTree **halo, int tree_levels);
void HDF5_ParentNode(char *name_file, int tree, MergerTree **halo, int tree_levels);
void HDF5_NodeIndex(char *name_file, int tree, MergerTree **halo, int tree_levels);
void HDF5_VolumeWeight(char *name_file, int tree, double volume);
void HDF5_TreeIndex(char *name_file, int tree, int index);


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int main(int argc, char *argv[])
{

  //begin:imput arguments check:
  if(argc != 4)
    {
      USAGE;
      exit(1);
    }
  if(!CheckFile(argv[1]))
    {
      printf("\nfile argv[1]:%s does not exist!.\n",argv[1]);
      USAGE;
      exit(1);
    }
  //end:imput arguments check.
  


  //begin: global variables:
  char *name_file_input = (char *)argv[1];
  char *name_file_out = (char *)argv[2];
  //end: global variables.


  //variables:
  //  double   H0 = 70.0;
  //double   boxLength = LENGTH;//(H0/100.0); // Length of the  simulation cube.
  //  double   volumeWeight = (NTREES*pow(LENGTH, 3.0)/pow(SUBLENGTH, 3.0))/pow(boxLength,3.0);
  double   volumeWeight = 1.0/pow(SUBLENGTH,3.0);
  


  //begin:data load: 
  int tree_levels = TreeLevelRead2(name_file_input, NULL);
  tree_levels = tree_levels - 1;

  if(tree_levels<2)
    {
      fprintf(stderr,"tree %s too short! needs to be removed\n",argv[1]);
      exit(1);
    }
    
  MergerTree  **milkyway = TreeDataLoad2(name_file_input, tree_levels, NULL);
  //end:data load.


  //aliasing IDs
  TreeAliasingID(milkyway, tree_levels);

  if( getenv("showInfo") != NULL)		
    {
      fprintf(stdout,"Ascii Merger Tree -> HDF5 Merger Tree.\n");			
      fprintf(stdout,"Reading: %s\n",argv[1]);					
      fprintf(stdout,"Box:%.lf SubBox:%.lf\n", LENGTH, SUBLENGTH);  	
      if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)		
	{									
	  fprintf(stdout,"::::::::::::::::::::::::::::::::\n");	
	  fprintf(stdout,"initLevel & endLevel setted!\n");
	  fprintf(stdout,"initLevel: %d;    endLevel: %d\n",atoi(getenv("initLevel")),atoi(getenv("endLevel"))); 
	  fprintf(stdout,"::::::::::::::::::::::::::::::::\n");	
	}									
      fprintf(stdout,"Tree ID: %d\nTree Nb. of Levels: %d\n",milkyway[0][0].id, tree_levels); 
    }
  //end:data load.
    
  
  //storing data in hdf5:
  
  //--store treeID:
  HDF5_TreeIndex(name_file_out, atoi(argv[3]), milkyway[0][0].id);
  
  //--store volumeWeight:
  HDF5_VolumeWeight(name_file_out, atoi(argv[3]), volumeWeight);
  
  //--store nodeIndex:
  HDF5_NodeAliasIndex(name_file_out, atoi(argv[3]), milkyway, tree_levels);
  
  //--store parentNode:
  HDF5_ParentAliasNode(name_file_out, atoi(argv[3]), milkyway, tree_levels);
  
  //--store childNode:
  HDF5_ChildAliasNode(name_file_out, atoi(argv[3]), milkyway, tree_levels);
  
  //--store SiblingNode:
  HDF5_SiblingAliasNode(name_file_out, atoi(argv[3]), milkyway, tree_levels);
  
  //--store nodeMass:
  HDF5_NodeMass(name_file_out, atoi(argv[3]), milkyway, tree_levels);
  
  //--store nodeRedshift:  
  HDF5_NodeRedshift(name_file_out, atoi(argv[3]), milkyway, tree_levels);

  fprintf(stdout,"tree %d stored to hdf5\n",milkyway[0][0].id);
  
  if( getenv("showInfo") != NULL)		
    fprintf(stdout,"Stored: %s\n\n",argv[2]);  
  
  
  return 0;
}












//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


void HDF5_NodeAliasIndex(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i,j;
  int status;

  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  int nb_node = 0;


  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)  
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j=0; j<halo[i][0].snapshot.nb_halos; j++)
	  {
	    fprintf(file_write_data,"%d\n",halo[i][j].idAlias);
	    nb_node++;
	  }
      }
  else
    for(i = tree_levels-1; i >= 0; i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    fprintf(file_write_data,"%d\n",halo[i][j].idAlias);
	    nb_node++;
	  }
      }

  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/nodeIndex\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTIN\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS IN\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}


void HDF5_ParentAliasNode(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i, j, k;
  int status;
  int parentID=0;

  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  int nb_node = 0;
  
  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    if( halo[i][j].nb_sons > 0 && i > 0)
	      {
		for(k = 0; k < halo[i-1][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i-1][k].id == halo[i][j].sons_id)
		      {
			parentID = halo[i][j].son_idAlias;
			break;
		      }		  
		    else
		      parentID = -1;
		  }
	      }
	    else
	      parentID = -1;
	  
	    if(i == atoi(getenv("endLevel")))
	      parentID = -1;
	  
	    fprintf(file_write_data,"%d\n", parentID);

	    nb_node++;
	  }
      }
  else
    for(i = tree_levels-1; i >= 0; i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    if( halo[i][j].nb_sons > 0 && i > 0)
	      {
		for(k = 0; k < halo[i-1][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i-1][k].id == halo[i][j].sons_id)
		      {
			parentID = halo[i][j].son_idAlias;
			break;
		      }		  
		    else
		      parentID = -1;
		  }
	      }
	    else
	      parentID = -1;
	  
	    if(i == 0)
	      parentID = -1;
	  
	    fprintf(file_write_data,"%d\n", parentID);

	    nb_node++;
	  }
      }


  fclose(file_write_data);
  
  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/parentNode\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTIN\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS IN\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}


void HDF5_ChildAliasNode(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i, j, k;
  int status;
  int childID=0;

  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  int nb_node = 0;
  
  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    if( halo[i][j].nb_fathers > 1 && i < tree_levels-1)
	      {
		for(k = 0; k < halo[i+1][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i+1][k].id == halo[i][j].fathers_id[1])
		      {
			childID = halo[i][j].father_idAlias;
			break;
		      }
		    else
		      childID = -1;
		  }
	      }
	    else
	      childID = -1;

	    if(i == atoi(getenv("initLevel")))
	      childID = -1;

	    fprintf(file_write_data,"%d\n",childID);

	    nb_node++;
	  }
      }
  else  
    for(i = tree_levels-1; i >= 0; i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    if( halo[i][j].nb_fathers > 1 && i < tree_levels-1)
	      {
		for(k=0; k<halo[i+1][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i+1][k].id == halo[i][j].fathers_id[1])
		      {
			childID = halo[i][j].father_idAlias;
			break;
		      }
		    else
		      childID = -1;
		  }
	      }
	    else
	      childID = -1;	    
	
	    if(i == tree_levels-1)
	      childID = -1;

	    fprintf(file_write_data,"%d\n",childID);
	    
	    nb_node++;
	  }
      }


  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/childNode\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTIN\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS IN\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}

//esta rutina solo sirve para cuando los halos estan ordenados por hijos en comun y por masas de mayor a menor
void HDF5_SiblingAliasNode(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i, j, k;
  int status, nb_node;
  int siblingID;

  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");

  nb_node = 0;  siblingID = 0;
  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    if(j < halo[i][0].snapshot.nb_halos - 1)
	      {
		for(k = j+1; k < halo[i][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i][k].sons_id == halo[i][j].sons_id)
		      {
			siblingID = halo[i][k].idAlias;
			break;
		      }
		    else
		      siblingID = -1;
		  }
	      }
	    else	     
	      siblingID = -1;
	    
	    fprintf(file_write_data,"%d\n",siblingID);
	    nb_node++;
	  }
      }
  else
    for(i = tree_levels-1; i >= 0; i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {	    
	    if(j < halo[i][0].snapshot.nb_halos - 1)
	      {

		for(k = j+1; k < halo[i][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i][k].sons_id == halo[i][j].sons_id)
		      {
			siblingID = halo[i][k].idAlias;
			break;
		      }
		    else
		      siblingID = -1;
		  }
		  
	      }
	    else	     
	      siblingID = -1;

	    fprintf(file_write_data,"%d\n",siblingID);	  
	    nb_node++;
	  }
      }


  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/siblingNode\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTIN\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS IN\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void HDF5_TreeIndex(char *name_file,int tree, int index)
{
  int status;

  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  fprintf(file_write_data,"%d\n",index);
  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");

  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/treeIndex\n",tree);
  fprintf(file_write_conf, "INPUT-CLASS TEXTIN\n");
  fprintf(file_write_conf, "INPUT-SIZE 64\n");
  fprintf(file_write_conf, "OUTPUT-CLASS IN\n");
  fprintf(file_write_conf, "OUTPUT-SIZE 64\n");
  fprintf(file_write_conf, "RANK 1\n");
  fprintf(file_write_conf, "DIMENSION-SIZES 1\n");
  fprintf(file_write_conf, "OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}


void HDF5_VolumeWeight(char *name_file,int tree, double volume)
{
  int status;

  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  fprintf(file_write_data,"%e\n",volume);
  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");

  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/volumeWeight\n",tree);
  fprintf(file_write_conf, "INPUT-CLASS TEXTFP\n");
  fprintf(file_write_conf, "INPUT-SIZE 64\n");
  fprintf(file_write_conf, "OUTPUT-CLASS FP\n");
  fprintf(file_write_conf, "OUTPUT-SIZE 64\n");
  fprintf(file_write_conf, "RANK 1\n");
  fprintf(file_write_conf, "DIMENSION-SIZES 1\n");
  fprintf(file_write_conf, "OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}



void HDF5_NodeIndex(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i,j;
  int status;


  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  int nb_node = 0;

  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    fprintf(file_write_data,"%d\n",halo[i][j].id);	  
	    nb_node++;
	  }
      }
  else
    for(i = tree_levels-1; i >= 0; i--)
      {
	for(j = 0; j<halo[i][0].snapshot.nb_halos; j++)
	  {
	    fprintf(file_write_data,"%d\n",halo[i][j].id);	  
	    nb_node++;
	  }
      }


  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/nodeIndex\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTIN\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS IN\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}


void HDF5_ParentNode(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i, j, k;
  int status;
  int parentID = 0;

  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  int nb_node = 0;

  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {

	    if( halo[i][j].nb_sons > 0 )
	      {
		for(k = 0; k < halo[i-1][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i-1][k].id == halo[i][j].sons_id)
		      {parentID = halo[i][j].sons_id;     break;}		  
		    else
		      parentID = -1;
		  }
	      }
	    else
	      parentID = -1;
	  
	    if(i == atoi(getenv("endLevel")))
	      parentID = -1;
	  
	    fprintf(file_write_data,"%d\n", parentID);

	    nb_node++;
	  }
      }
  else
    for(i = tree_levels; i >= 0; i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {

	    if( halo[i][j].nb_sons > 0 )
	      {
		for(k = 0; k < halo[i-1][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i-1][k].id == halo[i][j].sons_id)
		      {parentID = halo[i][j].sons_id;     break;}		  
		    else
		      parentID = -1;
		  }
	      }
	    else
	      parentID = -1;
	  
	    if(i == 0)
	      parentID = -1;
	  
	    fprintf(file_write_data,"%d\n", parentID);

	    nb_node++;
	  }
      }


  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/parentNode\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTIN\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS IN\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}


void HDF5_ChildNode(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i, j, k;
  int status;
  int childID = 0;

  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  int nb_node = 0;

  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j=0; j<halo[i][0].snapshot.nb_halos; j++)
	  {
	    if( halo[i][j].nb_fathers > 1 )
	      {
		for(k = 0; k < halo[i+1][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i+1][k].id == halo[i][j].fathers_id[1])
		      {childID = halo[i][j].fathers_id[1];   break;}		  
		    else
		      childID = -1;
		  }
	      }
	    else
	      childID = -1;

	    if(i == atoi(getenv("initLevel")))
	      childID = -1;

	    fprintf(file_write_data,"%d\n",childID);
	  
	    nb_node++;
	  }
      }
  else
    for(i = tree_levels-1; i >= 0; i--)
      {
	for(j=0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    if( halo[i][j].nb_fathers > 1 )
	      {
		for(k = 0; k<halo[i+1][0].snapshot.nb_halos; k++)
		  {
		    if(halo[i+1][k].id == halo[i][j].fathers_id[1])
		      {childID = halo[i][j].fathers_id[1];   break;}		  
		    else
		      childID = -1;
		  }
	      }
	    else
	      childID = -1;

	    if(i == tree_levels-1)
	      childID = -1;

	    fprintf(file_write_data,"%d\n",childID);
	  
	    nb_node++;
	  }
      }

  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/childNode\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTIN\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS IN\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}


void HDF5_SiblingNode(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i,j;
  int status;


  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  int nb_node = 0;

  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    fprintf(file_write_data,"%d\n",-1);	  
	    nb_node++;
	  }
      }
  else
    for(i = tree_levels-1; i >= 0; i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    fprintf(file_write_data,"%d\n",-1);	  
	    nb_node++;
	  }
      }


  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/siblingNode\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTIN\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS IN\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}



void HDF5_NodeMass(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i,j;
  int status;


  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  int nb_node = 0;

  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    fprintf(file_write_data,"%15f\n",(float)halo[i][j].mvir);	  
	    nb_node++;
	  }
      }
  else
    for(i = tree_levels-1; i >= 0; i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    fprintf(file_write_data,"%15f\n",(float)halo[i][j].mvir);	  
	    nb_node++;
	  }
      }

  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/nodeMass\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTFP\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS FP\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}


void HDF5_NodeRedshift(char *name_file,int tree, MergerTree **halo, int tree_levels)
{
  int i,j;
  int status;


  char name_data_tmp[50] = "data.tmp";
  char name_conf_tmp[50] = "config.tmp";
  char string_command[100];

  FILE *file_write_data = fopen(name_data_tmp,"w");
  int nb_node = 0;

  if( getenv("initLevel") != NULL && getenv("endLevel") != NULL)
    for(i = atoi(getenv("initLevel")); i >= atoi(getenv("endLevel")); i--)
      {
	for(j = 0; j<halo[i][0].snapshot.nb_halos; j++)
	  {	    
	    fprintf(file_write_data,"%10f\n",(float)( 1.0/halo[i][j].sfactor-1.0));	  
	    nb_node++;
	  }
      }
  else
    for(i = tree_levels-1; i >= 0; i--)
      {
	for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
	  {
	    if(i>0)
	      fprintf(file_write_data,"%10f\n", (float)( 1.0/halo[i][j].sfactor-1.0));
	    else
	      fprintf(file_write_data,"%10f\n", 0.0);

	    nb_node++;
	  }
      }


  fclose(file_write_data);

  FILE *file_write_conf = fopen(name_conf_tmp,"w");
  fprintf(file_write_conf,"PATH /mergerTrees/mergerTree%d/nodeRedshift\n",tree);
  fprintf(file_write_conf,"INPUT-CLASS TEXTFP\n");
  fprintf(file_write_conf,"INPUT-SIZE 64\n");
  fprintf(file_write_conf,"OUTPUT-CLASS FP\n");
  fprintf(file_write_conf,"OUTPUT-SIZE 64\n");
  fprintf(file_write_conf,"RANK 1\n");
  fprintf(file_write_conf,"DIMENSION-SIZES %d\n",nb_node);
  fprintf(file_write_conf,"OUTPUT-ARCHITECTURE NATIVE\n");
  fclose(file_write_conf);

  sprintf(string_command, "h5import %s -c %s -o %s", name_data_tmp, name_conf_tmp, name_file);
  status = system(string_command);  status = unlink(name_data_tmp);    status = unlink(name_conf_tmp);

}

