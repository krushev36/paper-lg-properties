#include "headers.h"

#define UNITS_MEGAPARSEC 3.0856776E+22
#define UNITS_SOLARMASS 1.98892E30
#define UNITS_GIGAYEARS 3.1536E+16
#define UNITS_KMETERSPSEC 1000.0

#define COSMO_POWERSPECINDEX 0.96
#define COSMO_HUBBLECONST    0.70
#define COSMO_OMEGAMATTER    0.279
#define COSMO_OMEGALAMBDA    0.721
#define COSMO_OMEGABARYON    0.046
#define COSMO_SIGMA8         0.817
#define COSMO_TRANSFERFUNC   "Eisenstein-Hu1999"

int yes = 1;
int no = 0;


//////////////////////////////////////////////////////////////////////////////////////////
		    //TreesIndex Group Attributes//
//////////////////////////////////////////////////////////////////////////////////////////
void TreeIndexGroupNew(hid_t file_out_hdf5, int nb_trees, int nb_nodes, void *params)
{
  int *parameters = (int *)params;
  int firstNode = parameters[0];
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



//////////////////////////////////////////////////////////////////////////////////////////
		    //HaloTrees Group Builder//
//////////////////////////////////////////////////////////////////////////////////////////
void HaloTreesGroupNew(hid_t file_out_hdf5, MergerTree **halo, int tree_levels, int nb_nodes, void *params)
{
  hid_t group_id;

  group_id = H5Gcreate(file_out_hdf5, "/haloTrees", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  HaloTreesGroupAttribute(group_id, NULL);
  HaloTreesGroupDataNew(file_out_hdf5, halo, tree_levels, nb_nodes);
  H5Gclose(group_id);
}




//////////////////////////////////////////////////////////////////////////////////////////
		    //HaloTrees New Datasets Group//
//////////////////////////////////////////////////////////////////////////////////////////
void HaloTreesGroupDataNew(hid_t file_out_hdf5, MergerTree **halo, int tree_levels, int nb_nodes)
{
  int i,j;
  hid_t dataset, dataspace, cparams;

  hsize_t   dims[1],maxdims[1], dims2[2], maxdims2[2];
  hsize_t   chunk_dims[1] = {10};
  hsize_t   chunk_dims2[2] = {10, 3};
  dims[0] = nb_nodes;
  dims2[0] = nb_nodes;  dims2[1] = 3;
  maxdims[0] = H5S_UNLIMITED;
  maxdims2[0] = H5S_UNLIMITED;   maxdims2[1] = 3;

  int count;
  int *nodeIndex, *nodeHost, *nodeDescendent;

  double *time, *expansionFactor, *redshift, *nodeMass, *spin;
  double nodePosition[nb_nodes][3], nodeVelocity[nb_nodes][3]; //for some reason dynamic memory allocation don't work with hdf5 2D datasets

  nodeIndex = (int *)malloc(nb_nodes*sizeof(int));
  nodeDescendent = (int *)malloc(nb_nodes*sizeof(int));

  nodeMass = (double *)malloc(nb_nodes*sizeof(double));
  expansionFactor = (double *)malloc(nb_nodes*sizeof(double));
  redshift = (double *)malloc(nb_nodes*sizeof(double));
  time = (double *)malloc(nb_nodes*sizeof(double));
  spin = (double *)malloc(nb_nodes*sizeof(double));

  for(i = 0, count = 0; i < tree_levels; i++)
    for(j = 0; j < halo[i][0].snapshot.nb_halos; j++)
      {
	nodeIndex[count] = halo[i][j].idAlias;
	nodeMass[count] = halo[i][j].mvir;
	nodeDescendent[count] = halo[i][j].son_idAlias;

	spin[count] = halo[i][j].spin;

	expansionFactor[count] = halo[i][j].sfactor;
	time[count] = halo[i][j].age;
	redshift[count] = 1.0/halo[i][j].sfactor - 1.0;

	nodePosition[count][0] = halo[i][j].position[0];
	nodePosition[count][1] = halo[i][j].position[1];
	nodePosition[count][2] = halo[i][j].position[2];

	nodeVelocity[count][0] = halo[i][j].velocity[0];
	nodeVelocity[count][1] = halo[i][j].velocity[1];
	nodeVelocity[count][2] = halo[i][j].velocity[2];

	count++;
      }

  /* hostIndex  Dataset */
  dataspace = H5Screate_simple(1, dims, dims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/hostIndex", H5T_STD_I64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, nodeIndex );
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* nodeIndex  Dataset */
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/nodeIndex", H5T_STD_I64LE, dataspace,  H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, nodeIndex );
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* descendentIndex Dataset*/
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/descendentIndex", H5T_STD_I64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, nodeDescendent );
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* nodeMass Dataset*/
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/nodeMass", H5T_IEEE_F64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite( dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, nodeMass);
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* exapansionFactor Dataset*/
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/exapansionFactor",  H5T_IEEE_F64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite( dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, expansionFactor);
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* redshift Dataset */
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/redshift", H5T_IEEE_F64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite( dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, redshift);
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* time Dataset */
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/time", H5T_IEEE_F64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite( dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, time);
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* spin Dataset */
  dataspace = H5Screate_simple(1, dims, maxdims);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 1, chunk_dims);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/spin", H5T_IEEE_F64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite( dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, spin);
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* position Dataset */
  dataspace = H5Screate_simple(2, dims2, maxdims2);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 2, chunk_dims2);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/position", H5T_IEEE_F64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite( dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, nodePosition);
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);

  /* velocity Dataset */
  dataspace = H5Screate_simple(2, dims2, maxdims2);
  cparams = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk ( cparams, 2, chunk_dims2);
  dataset = H5Dcreate(file_out_hdf5, "/haloTrees/velocity", H5T_IEEE_F64LE, dataspace, H5P_DEFAULT, cparams, H5P_DEFAULT);
  H5Dwrite( dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, nodeVelocity);
  H5Pclose(cparams);
  H5Sclose(dataspace);
  H5Dclose(dataset);



}





//////////////////////////////////////////////////////////////////////////////////////////
		    //HaloTrees Group Attributes//
//////////////////////////////////////////////////////////////////////////////////////////

void HaloTreesGroupAttribute(hid_t group_id, void *params)
{
  hid_t attribute;

  //begin: haloMassesIncludeSUbhalos
  attribute = H5Acreate (group_id, "haloMassesIncludeSubhalos" ,  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &yes);
  H5Aclose(attribute);

  //begin: treesAreSelfContained
  attribute = H5Acreate (group_id, "treesAreSelfContained" ,  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &yes);
  H5Aclose(attribute);

  //begin: treesHaveSubhalos
  attribute = H5Acreate (group_id, "treesHaveSubhalos",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &no);
  H5Aclose(attribute);

  //begin: velocitiesIncludeHubbleFlow
  attribute = H5Acreate (group_id, "velocitiesIncludeHubbleFlow",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &no);
  H5Aclose(attribute);

}



void UnitsGroup(hid_t file_out_hdf5, void *params)
{
  hid_t group_id, attribute;

  int lengthHubbleExponent, lengthScaleFactorExponent;
  double lengthUnitsInSI;

  lengthUnitsInSI = UNITS_MEGAPARSEC;
  lengthHubbleExponent = 0;
  lengthScaleFactorExponent = 0;

  int massHubbleExponent, massScaleFactorExponent;
  double massUnitsInSI;

  massUnitsInSI = UNITS_SOLARMASS;
  massHubbleExponent = 0;
  massScaleFactorExponent = 0;

  int timeHubbleExponent, timeScaleFactorExponent;
  double timeUnitsInSI;

  timeUnitsInSI = UNITS_GIGAYEARS;
  timeHubbleExponent = 0;
  timeScaleFactorExponent = 0;

  int velocityHubbleExponent, velocityScaleFactorExponent;
  double velocityUnitsInSI;

  velocityUnitsInSI = UNITS_KMETERSPSEC;
  velocityHubbleExponent = 0;
  velocityScaleFactorExponent = 0;


  group_id = H5Gcreate(file_out_hdf5, "/units", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

///////////////////////////////////////////////////////////////////////////////
			    //Length Units//
///////////////////////////////////////////////////////////////////////////////

  //begin: lengthHubbleExponent
  attribute = H5Acreate (group_id, "lengthHubbleExponent",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &lengthHubbleExponent);
  H5Aclose(attribute);

  //begin: lengthScaleFactorExponent
  attribute = H5Acreate (group_id, "lengthScaleFactorExponent",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &lengthScaleFactorExponent);
  H5Aclose(attribute);

  //begin: lengthUnitsInSI
  attribute = H5Acreate (group_id, "lengthUnitsInSI",  H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_DOUBLE, &lengthUnitsInSI);
  H5Aclose(attribute);


///////////////////////////////////////////////////////////////////////////////
			    //Mass Units//
///////////////////////////////////////////////////////////////////////////////

  //begin: massHubbleExponent
  attribute = H5Acreate (group_id, "massHubbleExponent",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &massHubbleExponent);
  H5Aclose(attribute);

  //begin: massScaleFactorExponent
  attribute = H5Acreate (group_id, "massScaleFactorExponent",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &massScaleFactorExponent);
  H5Aclose(attribute);

  //begin: massUnitsInSI
  attribute = H5Acreate (group_id, "massUnitsInSI",  H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_DOUBLE, &massUnitsInSI);
  H5Aclose(attribute);


///////////////////////////////////////////////////////////////////////////////
			    //Time Units//
///////////////////////////////////////////////////////////////////////////////

  //begin: timeHubbleExponent
  attribute = H5Acreate (group_id, "timeHubbleExponent",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &timeHubbleExponent);
  H5Aclose(attribute);

  //begin: timeScaleFactorExponent
  attribute = H5Acreate (group_id, "timeScaleFactorExponent",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &timeScaleFactorExponent);
  H5Aclose(attribute);

  //begin: timeUnitsInSI
  attribute = H5Acreate (group_id, "timeUnitsInSI",  H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_DOUBLE, &timeUnitsInSI);
  H5Aclose(attribute);


///////////////////////////////////////////////////////////////////////////////
			  //Velocity Units//
///////////////////////////////////////////////////////////////////////////////

  //begin: velocityHubbleExponent
  attribute = H5Acreate (group_id, "velocityHubbleExponent",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &velocityHubbleExponent);
  H5Aclose(attribute);

  //begin: velocityScaleFactorExponent
  attribute = H5Acreate (group_id, "velocityScaleFactorExponent",  H5T_STD_I32LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_INT, &velocityScaleFactorExponent);
  H5Aclose(attribute);

  //begin: velocityUnitsInSI
  attribute = H5Acreate (group_id, "velocityUnitsInSI",  H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_DOUBLE, &velocityUnitsInSI);
  H5Aclose(attribute);

  H5Gclose(group_id);  
}



void CosmologyGroup(hid_t file_out_hdf5, void *params)
{
  hid_t group_id, attribute, atype; 

  float powerSpecIndex = COSMO_POWERSPECINDEX;  
  float hubbleValue = COSMO_HUBBLECONST;
  float omegaMatter = COSMO_OMEGAMATTER;
  float omegaLambda = COSMO_OMEGALAMBDA;
  float omegaBaryon = COSMO_OMEGABARYON;
  float sigma8 = COSMO_SIGMA8;
  char  transferFunction[30] = COSMO_TRANSFERFUNC;

  group_id = H5Gcreate(file_out_hdf5, "/cosmology", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  //begin: Hubble Param
  attribute = H5Acreate (group_id, "HubbleParam" , H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_FLOAT, &hubbleValue);
  H5Aclose(attribute);

  //begin: Omega Matter
  attribute = H5Acreate (group_id, "OmegaMatter" , H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_FLOAT, &omegaMatter);
  H5Aclose(attribute);

  //begin: Omega Lambda
  attribute = H5Acreate (group_id, "OmegaLambda" , H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_FLOAT, &omegaLambda);
  H5Aclose(attribute);

  //begin: Omega Lambda
  attribute = H5Acreate (group_id, "OmegaBaryon" , H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_FLOAT, &omegaBaryon);
  H5Aclose(attribute);

  //begin: Power Spectrum Index
  attribute = H5Acreate (group_id, "PowerSpectrumIndex" , H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_FLOAT, &powerSpecIndex);
  H5Aclose(attribute);

  //begin: Sigma 8
  attribute = H5Acreate (group_id, "Sigma8" , H5T_IEEE_F64LE, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, H5T_NATIVE_FLOAT, &sigma8);
  H5Aclose(attribute);

  //begin: transfer Function
  atype = H5Tcopy(H5T_C_S1);
  H5Tset_size(atype, 30);
  H5Tset_strpad(atype, H5T_STR_SPACEPAD);
  H5Tset_cset(atype,  H5T_CSET_ASCII);

  attribute = H5Acreate (group_id, "transferFunction" , atype, H5Screate(H5S_SCALAR), H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attribute, atype, transferFunction);
  H5Aclose(attribute);

  H5Gclose(group_id);  
}




