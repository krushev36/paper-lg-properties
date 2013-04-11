/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
  Readme:
  this code reads (complete) merger tree histories extract
  ed from a merger trees catalogue. Builds the merger tree
  HDF5 structure requiered by Galacticus Code v0.9.0 (Bens
  on 2009 ).

  Run:
  ./MergerTreeHDF5-v2.out (char *)input_tree \
                      (char *)output_tree (int)tree_number

  °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/
#include <headers.h>

#define USAGE printf("\nUSAGE:\n      %s  (char *)input_file (char *)output_file  (int)tree_id\n\n",__FILE__)

void TreeIndexGroupAppend(hid_t file_out_hdf5, int nb_trees, int nb_nodes, void *params);
void HaloTreesGroupDataAppend(hid_t file_out_hdf5, MergerTree **halo, int tree_levels, int nb_nodes);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int main(int argc, char *argv[])
{
  //begin: input check
  if(argc != 4)
    {
      USAGE;
      exit(1);
    }
  if(!CheckFile(argv[1]))
    {
      printf("\ninput file %s does not exist!.\n",argv[1]);
      USAGE;
      exit(1);
    }
  if(!CheckFile(argv[2]))
    {
      printf("\noutput file %s does not exist. new one will be created!\n",argv[2]);
      USAGE;
      exit(1);
    }


  //begin:data load
  int nb_nodes, tree_levels, nb_trees;

  nb_trees = 1;
  tree_levels = TreeLevelRead2(argv[1], NULL);
  tree_levels = tree_levels - 1;

  if(tree_levels<2)
    {
      fprintf(stderr,"tree %s too short! needs to be removed\n",argv[1]);
      exit(1);
    }

  MergerTree  **milkyway = TreeDataLoad2(argv[1], tree_levels, NULL);

  nb_nodes = TreeAliasingID2(milkyway, tree_levels);  //aliasing and get number of nodes

  int treeInfo[3];
  treeInfo[0] = 0;
  treeInfo[1] = nb_nodes;
  treeInfo[2] = atoi(argv[3]);// tree ID


  //begin: hdf5 building
  hid_t   file_out_hdf5;

  if(!CheckFile(argv[2]))
    {
      printf("\noutput file %s created!\n",argv[2]);

      file_out_hdf5 = H5Fcreate(argv[2], H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

      CosmologyGroup(file_out_hdf5, NULL);
      HaloTreesGroupNew(file_out_hdf5, milkyway, tree_levels, nb_nodes, NULL);

      TreeIndexGroupNew(file_out_hdf5, 1, nb_nodes, treeInfo);
      UnitsGroup(file_out_hdf5, NULL);

      H5Fclose(file_out_hdf5);
    }
  else
    {
      printf("\noutput file %s already exist! appending datasets is not allowed.\n",argv[2]);
      exit(1);
      /* printf("\nappending to %s!\n",argv[2]); */
      /* file_out_hdf5 = H5Fopen(argv[2], H5F_ACC_RDWR, H5P_DEFAULT); */

      /* HaloTreesGroupDataAppend(file_out_hdf5, milkyway, tree_levels, nb_nodes); */
      /* //TreeIndexGroupNew(file_out_hdf5, 1, nb_nodes, treeInfo); */

      /* H5Fclose(file_out_hdf5);           */
    }

  return 0;
}




//////////////////////////////////////////////////////////////////////////////////////////
		    //HaloTrees Append Datasets Group//
//////////////////////////////////////////////////////////////////////////////////////////
void HaloTreesGroupDataAppend(hid_t file_out_hdf5, MergerTree **halo, int tree_levels, int nb_nodes)
{
  int i, j, count;
  int rank, nb_elements;
  int *nodeIndex, *nodeHost, *nodeDescendent;

  hid_t dataset, dataspace, cparams, filespace;

  hsize_t   newdims[1], finaldims[1], newdims2[2], finaldims2[2], offset[1], offset2[2];

  newdims[0] = nb_nodes;
  newdims2[0] = nb_nodes;  newdims2[1] = 3;

  double *time, *expansionFactor, *redshift;
  double *nodeMass;
  double nodePosition[nb_nodes][3], nodeVelocity[nb_nodes][3]; //for some reason dynamic memory allocation doesn't work with hdf5 2D datasets

  nodeIndex = (int *)malloc(nb_nodes*sizeof(int));
  nodeDescendent = (int *)malloc(nb_nodes*sizeof(int));
  nodeMass = (double *)malloc(nb_nodes*sizeof(double));
  expansionFactor = (double *)malloc(nb_nodes*sizeof(double));
  redshift = (double *)malloc(nb_nodes*sizeof(double));
  time = (double *)malloc(nb_nodes*sizeof(double));

  for(i = 0, count = 0; i < tree_levels; i++)
    for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
      {
	nodeIndex[count] = halo[i][j].idAlias;
	nodeMass[count] = halo[i][j].mvir;
	nodeDescendent[count] = halo[i][j].son_idAlias;

	expansionFactor[count] = halo[i][j].sfactor;
	time[count] = halo[i][j].age;
	redshift[count] = 1.0/halo[i][j].sfactor - 1.0;

	nodePosition[count][0] = halo[i][j].position[0];
	nodePosition[count][1] = halo[i][j].position[1];
	nodePosition[count][2] = halo[i][j].position[2];

	count++;
      }


  /* hostIndex  Dataset */
  dataset = H5Dopen (file_out_hdf5, "/haloTrees/hostIndex", H5P_DEFAULT);

  filespace = H5Dget_space (dataset);
  rank = H5Sget_simple_extent_ndims (filespace);
  nb_elements = H5Sget_simple_extent_npoints( filespace );

  finaldims[0] = nb_elements+10;// + nb_nodes-1000;
  finaldims2[0] = nb_elements + nb_nodes;  finaldims2[1] = 3;

  offset[0] = nb_elements;
  offset2[0] = nb_elements;   offset2[1] = 0;

  printf("finaldims:%d\n",(int)finaldims[0]);

  H5Dextend(dataset, finaldims);

  filespace = H5Dget_space( dataset );
  H5Sselect_hyperslab (filespace, H5S_SELECT_SET, offset, NULL, newdims, NULL); 
  dataspace = H5Screate_simple (1, newdims, NULL); 

  //H5Dwrite (dataset, H5T_NATIVE_INT, dataspace, filespace, H5P_DEFAULT, nodeIndex);

  H5Sclose(dataspace);
  H5Sclose(filespace);
  H5Dclose(dataset);


  /* nodeIndex  Dataset */
  /* dataset = H5Dopen (file_out_hdf5, "/haloTrees/nodeIndex", H5P_DEFAULT); */

  /* H5Dextend(dataset, finaldims); */

  /* filespace = H5Dget_space( dataset ); */
  /* H5Sselect_hyperslab (filespace, H5S_SELECT_SET, offset, NULL, newdims, NULL);  */
  /* dataspace = H5Screate_simple (1, newdims, NULL);  */

  /* H5Dwrite (dataset, H5T_NATIVE_INT, dataspace, filespace, H5P_DEFAULT, nodeIndex); */

  /* H5Sclose(dataspace); */
  /* H5Dclose(filespace); */
  /* H5Dclose(dataset); */


  /* descendentIndex Dataset*/
  /* dataset = H5Dopen (file_out_hdf5, "/haloTrees/descendentIndex", H5P_DEFAULT); */

  /* H5Dextend(dataset, finaldims); */

  /* filespace = H5Dget_space( dataset ); */
  /* H5Sselect_hyperslab (filespace, H5S_SELECT_SET, offset, NULL, newdims, NULL);  */
  /* dataspace = H5Screate_simple (1, newdims, NULL);  */

  /* H5Dwrite (dataset, H5T_NATIVE_INT, dataspace, filespace, H5P_DEFAULT, nodeDescendent); */

  /* H5Sclose(dataspace); */
  /* H5Dclose(filespace); */
  /* H5Dclose(dataset); */



  /* nodeMass Dataset*/
  /* dataset = H5Dopen (file_out_hdf5, "/haloTrees/nodeMass", H5P_DEFAULT); */

  /* H5Dextend(dataset, finaldims); */

  /* filespace = H5Dget_space( dataset ); */
  /* H5Sselect_hyperslab (filespace, H5S_SELECT_SET, offset, NULL, newdims, NULL);  */
  /* dataspace = H5Screate_simple (1, newdims, NULL);  */

  /* H5Dwrite (dataset, H5T_NATIVE_DOUBLE, dataspace, filespace, H5P_DEFAULT, nodeMass); */

  /* H5Sclose(dataspace); */
  /* H5Dclose(filespace); */
  /* H5Dclose(dataset); */


  /* /\* exapansionFactor Dataset*\/ */
  /* dataset = H5Dopen (file_out_hdf5, "/haloTrees/exapansionFactor", H5P_DEFAULT); */

  /* H5Dextend(dataset, finaldims); */

  /* filespace = H5Dget_space( dataset ); */
  /* H5Sselect_hyperslab (filespace, H5S_SELECT_SET, offset, NULL, newdims, NULL);  */
  /* dataspace = H5Screate_simple (1, newdims, NULL);  */

  /* H5Dwrite (dataset, H5T_NATIVE_DOUBLE, dataspace, filespace, H5P_DEFAULT, expansionFactor); */

  /* H5Sclose(dataspace); */
  /* H5Dclose(filespace); */
  /* H5Dclose(dataset); */



  /* /\* redshift Dataset *\/ */
  /* dataset = H5Dopen (file_out_hdf5, "/haloTrees/redshift", H5P_DEFAULT); */

  /* H5Dextend(dataset, finaldims); */

  /* filespace = H5Dget_space( dataset ); */
  /* H5Sselect_hyperslab (filespace, H5S_SELECT_SET, offset, NULL, newdims, NULL);  */
  /* dataspace = H5Screate_simple (1, newdims, NULL);  */

  /* H5Dwrite (dataset, H5T_NATIVE_DOUBLE, dataspace, filespace, H5P_DEFAULT, redshift); */

  /* H5Sclose(dataspace); */
  /* H5Dclose(filespace); */
  /* H5Dclose(dataset); */



  /* /\* time Dataset *\/ */
  /* dataset = H5Dopen (file_out_hdf5, "/haloTrees/time", H5P_DEFAULT); */

  /* H5Dextend(dataset, finaldims); */

  /* filespace = H5Dget_space( dataset ); */
  /* H5Sselect_hyperslab (filespace, H5S_SELECT_SET, offset, NULL, newdims, NULL);  */
  /* dataspace = H5Screate_simple (1, newdims, NULL);  */

  /* H5Dwrite (dataset, H5T_NATIVE_DOUBLE, dataspace, filespace, H5P_DEFAULT, time); */

  /* H5Sclose(dataspace); */
  /* H5Dclose(filespace); */
  /* H5Dclose(dataset); */




  /* /\* position Dataset *\/ */
  /* dataset = H5Dopen (file_out_hdf5, "/haloTrees/position", H5P_DEFAULT); */

  /* H5Dextend(dataset, finaldims2); */

  /* filespace = H5Dget_space( dataset ); */
  /* H5Sselect_hyperslab (filespace, H5S_SELECT_SET, offset2, NULL, newdims2, NULL);  */
  /* dataspace = H5Screate_simple (2, newdims2, NULL);  */

  /* H5Dwrite (dataset, H5T_NATIVE_DOUBLE, dataspace, filespace, H5P_DEFAULT, nodePosition); */

  /* H5Sclose(dataspace); */
  /* H5Dclose(filespace); */
  /* H5Dclose(dataset); */


  /* velocity Dataset */                   //not used by Galacticus yet!
  /* dataspace = H5Screate_simple(2, dims2, maxdims2); */
  /* cparams = H5Pcreate (H5P_DATASET_CREATE); */
  /* H5Pset_chunk ( cparams, 2, chunk_dims2); */
  /* dataset = H5Dcreate(file_out_hdf5, "/haloTrees/velocity", H5T_IEEE_F64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT); */
  /* H5Dwrite( dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, nodeVelocity); */
  /* H5Pclose(cparams); */
  /* H5Sclose(dataspace); */
  /* H5Dclose(dataset); */

}



//////////////////////////////////////////////////////////////////////////////////////////
		    //TreesIndex Group Attributes//
//////////////////////////////////////////////////////////////////////////////////////////
void TreeIndexGroupAppend(hid_t file_out_hdf5, int nb_trees, int nb_nodes, void *params)
{
  int *parameters = (int *)params;
  int  firstNode = parameters[0];
  int  numberOfNodes = parameters[1];
  int  treeIndex = parameters[2];

  hid_t group_id, dataset, dataspace, cparams;
  hsize_t   dims[1], maxdims[1], chunk_dims[1];
  chunk_dims[0] = 1;
  dims[0] = 1; 
  maxdims[0] = H5S_UNLIMITED;


  group_id = H5Gcreate(file_out_hdf5, "/treeIndex", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  /* firstNode  Dataset */
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/treeIndex/firstNode", H5T_STD_I64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &firstNode);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* numberOfNodes  Dataset */
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/treeIndex/numberOfNodes", H5T_STD_I64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,  &numberOfNodes);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* treeIndex  Dataset */
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/treeIndex/treeIndex", H5T_STD_I64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &treeIndex );
  H5Sclose(dataspace);
  H5Dclose(dataset);

  H5Gclose(group_id);
}







