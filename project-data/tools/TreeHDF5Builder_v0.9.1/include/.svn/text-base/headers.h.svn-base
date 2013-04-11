struct level
{
  int nb;
  int nb_halos;

};

typedef struct level level;


struct MergerTree
{
  int id;
  int idAlias;
  int level;
  double age;
  double sfactor;
  double mfof;
  double *position;
  double *velocity;
  double rfof, a , b, c;
  double ek, ep, et;
  double spin;
  double rvir, mvir, tvir, cvel;
  double rho_0,r_c;
  int nb_fathers;
  int *fathers_id;
  int father_idAlias;
  int nb_sons;
  int sons_id;
  int son_idAlias;
  float plot_x;
  float plot_y; 


  level snapshot;

  struct MergerTree *halo_son;
  struct MergerTree *halo_father;
};

typedef struct MergerTree MergerTree;

struct halo_data
{
  int id;

  double mass_vir;
  double mass_fof;

  float pos_x;
  float pos_y;
  float pos_z;


  float periodic_x;
  float periodic_y;
  float peridoic_z;

  float rho_0;

  float r_c;
  float r_vir;
  float r_fof;

  float spin;

  float delta;
};

    
typedef struct halo_data properties;


////////////////////////////////////////////////////////////////

typedef float *floatptr;
typedef float *floatptr1;
////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h> 
#include <unistd.h>


//////////////////////////////////////////////////////////////////////////////////////

#include<hdf5.h>

//////////////////////////////////////////////////////////////////////////////////////

#include <gsl/gsl_errno.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_qrng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sort_double.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_histogram2d.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_integration.h>


//////////////////////////////////////////////////////////////////////////////////////

////simulational data management rutines
#include "SimDataMan.h"
////set of file system management rutines
#include "SysTools.h"
////set of routines dedicated to build Galacticus v0.9.0 input merger tree format
#include "MergerTreeGalacticus9.0.h"
//////////////////////////////////////////////////////////////////////////////////////

#define OMEGA_M 0.268
#define OMEGA_LAMBDA 0.742
#define CRITICAL_DENSITY 2.75e11 //M_sol/Mpc³ densidad de masacritica  del universo
#define MEAN_DENSITY 1.462e11 //M_sol/Mpc³ densidadmasa  media del universo

//////////////////////////////////////////////////////////////////////////////////////
