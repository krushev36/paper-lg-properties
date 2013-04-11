#include<headers.h>

/////////////////////////////////////////////////////////////////////////////////////////
/*                                MY RUTINES                                           */
/////////////////////////////////////////////////////////////////////////////////////////

//++++++++++++++++++++++++++++++GROWTH FACTOR++++++++++++++++++++++++++++++++++
//the growth factor normalized to g=1 when a=1 in the critical world model \Omega_0=1, 
//\Omega_l=0. which correspong to the growing mode of the density contrast in a expanding
//universe. Lahav et al.(1991)
//ARGUMENTS: x the scale factor
//          the Omega_0 double params[1]
//          the Omega_l double params[1]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
double FGrowthFactorLahav(double x, void *params)
{
  double *parameters = (double *)params;
  double Omega_m = parameters[0];
  double Omega_l = parameters[1];

  double D = 2.5*x*Omega_m/(pow(Omega_m, 0.5714) - Omega_l + (1.0 + 0.5*Omega_m)*(1.0 + Omega_l/70.0));
  /*
  printf("Parameters:\n");
  printf("           Omega_m = %.5lf\n", parameters[0]);
  printf("           Omega_l = %.5lf\n", parameters[1]);
  */
  //  printf ("Lahav et al Growth Factor(a = %.5f)  = %.5f\n", x, D);
  return D;
}

//++++++++++++++++++++++++++++++GROWTH RATE++++++++++++++++++++++++++++++++++
//the growth rate normalized to g=1 when a=1 in the critical world model \Omega_0=1, 
//\Omega_l=0. which correspong to the growing mode of the density contrast in a expanding
//universe. Lahav et al.(1991)
//ARGUMENTS: x the scale factor
//          the Omega_0 double params[1]
//          the Omega_l double params[1]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

double FGrowthRateLahav(double x, void *params)
{
  double  *parameters = (void *)params;  

  double Omega_m = parameters[0];
  double Omega_l = parameters[1];

  double Omega[2] = {Omega_m, Omega_l};

  double delta = FGrowthFactorLahav(x, Omega)/FGrowthFactorLahav(1.0, Omega);

  //  printf ("Lahav Growth Rate(a = %.5f)    = %.5f\n", x, delta);

  return delta;
}

//++++++++++++++++++++++++++++++GROWTH FACTOR++++++++++++++++++++++++++++++++++
//the growth factor normalized to g=1 when a=1 in the critical world model \Omega_0=1, 
//\Omega_l=0. which correspong to the growing mode of the density contrast in a expanding
//universe. M S Longair 2008.
//ARGUMENTS: x the scale factor
//          the Hubble Constant double params[0]
//          the Omega_0 double params[1]
//          the Omega_l double params[1]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

double FGrowthRateLongair(double x, void *params)
{
  double  *parameters = (void *)params;  
      
  double parameters_2[4];
  parameters_2[0] = parameters[0];//hubble_0
  parameters_2[1] = parameters[1];//omega_m
  parameters_2[2] = parameters[2];//omega_l


  double delta = FGrowthFactorLongair(x, parameters_2)/FGrowthFactorLongair(1.0, parameters_2);

  //  printf ("Longair Growth Rate(a = %.5f)    = %.5f\n", x, delta);

  return delta;
}


//++++++++++++++++++++++++++++++GROWTH FACTOR++++++++++++++++++++++++++++++++++
//the growth factor normalized to g=1 when a=1 in the critical world model \Omega_0=1, 
//\Omega_l=0. which correspong to the growing mode of the density contrast in a expanding
//universe. M S Longair 2008.
//ARGUMENTS: x the scale factor
//          the Hubble Constant double params[0]
//          the Omega_0 double params[1]
//          the Omega_l double params[1]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//arroja el valor de la growth function integrando el modo creciente lineal
double FGrowthFactorLongair(double x, void *params)
{ 
  double result, error;  

  double  *parameters = (void *)params;  

  double  Hubble_0 = parameters[0];//hubble_0
  double  OmegaM = parameters[1];//omega_m
  double  OmegaLambda = parameters[2];//omega_l
      
  double parameters_2[4];
  parameters_2[0] = parameters[0];//hubble_0
  parameters_2[1] = parameters[1];//omega_m
  parameters_2[2] = parameters[2];//omega_l
    
  double dadt = Hubble_0*sqrt( OmegaM*( pow(x, -1.0) - 1.0) + OmegaLambda*(x*x - 1.0) + 1.0 );
  
  gsl_integration_workspace * R = gsl_integration_workspace_alloc (1000);
  
  gsl_function F;
  
  F.function = &FUnNormGrowthFactorLongair;
  
  F.params = &parameters_2;
    
  gsl_integration_qags(&F, 0, x, 0, 1e-7, 1000, R, &result, &error); 
    
  gsl_integration_workspace_free(R);
  
  //  return result;

  /*
  printf("Parameters:\n");
  printf("           H_0 = %.5lf\n", parameters[0]);
  printf("           Omega_m = %.5lf\n", parameters[1]);
  printf("           Omega_l = %.5lf\n", parameters[2]);
  */

  //  printf ("Longair Growth Factor(a = %.5f)  = %.5f\n", x, 2.5*OmegaM*dadt*result/x);

  return 2.5*OmegaM*dadt*result/x;
}



double FUnNormGrowthFactorLongair(double x, void *params)
{

  double *imput_param = (double *)params;

  float Hubble_0 = imput_param[0];
  float OmegaM = imput_param[1];
  float OmegaLambda = imput_param[2];


  double dadt = Hubble_0*sqrt( OmegaM*( pow(x, -1.0) - 1.0) + OmegaLambda*(x*x - 1.0) + 1.0 );

  double f = 1.0/(dadt*dadt*dadt);

  return f;

}



//+++++++++++++++++++++++++++HUBBLE FUNCTION++++++++++++++++++++++++++++++++++
//ARGUMENTS: x the scale factor
//          the Omega_m double params[0] 
//          the Omega_l double params[1]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
double FHubble(double x, void *params)
{
  double *parameters = (double *)params;
  double Omega_m = parameters[0];
  double Omega_l = parameters[1];

  double H = sqrt( Omega_m*pow(x, -3.0) + Omega_l);

  return H;
}


//++++++++++++++++++++++++++++++GROWTH FACTOR++++++++++++++++++++++++++++++++++
//the growth factor without the amplitud
//ARGUMENTS: x the scale factor
//          the Omega_m double params[0] 
//          the Omega_l double params[1]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
double FUnNormGrowthFactorHamilt(double x, void *params)
{
  double *parameters = (double *)params;
  double Omega_m = parameters[0];
  double Omega_l = parameters[1];

  double Omega[2] = {Omega_m, Omega_l};

  return  pow( x*FHubble(x, Omega) , -3.0);
}


//++++++++++++++++++++++++++++++GROWTH FACTOR++++++++++++++++++++++++++++++++++
//the growth factor with the amplitud normalizad to g=1 when a->0, taken from
//A. J. S. Hamilton, MNRAS 322, 419-425 (2001)
//ARGUMENTS: x the scale factor
//          the Omega_m double params[0] 
//          the Omega_l double params[1]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

double FGrowthFactorHamilt(double x, void *params)
{
  double *parameters = (double *)params;
  double Omega_m = parameters[0];
  double Omega_l = parameters[1];
  double Omega[2] = {Omega_m, Omega_l};

  double integral_result, error;

  gsl_function F;
  F.function = &FUnNormGrowthFactorHamilt;
  F.params = &Omega;
  
  gsl_integration_workspace *w = gsl_integration_workspace_alloc (1000);

  gsl_integration_qags (&F, 0, x, 0, 1e-7, 1000, w, &integral_result, &error); 
  /*
  printf("Parameters:\n");
  printf("           Omega_m = %.5lf\n", Omega[0]);
  printf("           Omega_l = %.5lf\n", Omega[1]);
  */
  double g = 2.5*Omega_m*FHubble(x, Omega)*integral_result/x;

  //  printf ("Hubble(a = %.5f)  = %.5f\n", x, FHubble(x, Omega) );
  //  printf ("Hamilton Growth Factor(a = %.5f)  = %.5f\n", x, g);

  gsl_integration_workspace_free (w);

  return  g;
  //  return integral_result;
}

//++++++++++++++++++++++++++++++GROWTH RATE++++++++++++++++++++++++++++++++++
//the growth rate with the amplitud normalizad to g=1 when a->0, taken from
//A. J. S. Hamilton, MNRAS 322, 419-425 (2001)
//ARGUMENTS: x the scale factor
//          the Omega_m double params[0] 
//          the Omega_l double params[1]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

double FGrowthRateHamilt(double x, void *params)
{
  double *parameters = (double *)params;
  double Omega_m = parameters[0];
  double Omega_l = parameters[1];
  double Omega[2] = {Omega_m, Omega_l};

  double f = -1.0 - (Omega_m/2.0)+Omega_l + 2.5*Omega_m/FGrowthFactorHamilt(x, Omega);

  //  printf ("Hamilton Growth Rate(a = %.5f)    = %.5f\n", x, f);

  return f;
}



double FCharactMassZ(double c0, double c1, double growing_mode_z)
{
  double m_char;

  m_char = pow(10, -c0/c1)*pow(1.686/growing_mode_z, 2.0/c1);

  return m_char;

}


double FWindowTopHat(double k, void *params)
{

  double *parameters = (double *)params;

  double R = parameters[0];
  double W;

  W = 3*(sin(k*R) - k*R*cos(k*R))/(k*R*k*R*k*R);

  return W;
}



double FZeldovichSpect(double k, void *params)
{
  double *parameters = (double *)params;
  double A = parameters[0];
  double P = A*k;

  return P;
}


double FTransferBBKS(double q, void *params)
{

  double T_k;

  T_k=log(1.0 + 2.34*q)*pow(1.0 + 3.89*q + 16.1*q*16.1*q 
			    + 5.46*q*5.46*q*5.46*q 
			    + 6.71*q*6.71*q*6.71*q*6.71*q ,-0.25)/(2.34*q);


  return T_k;
}


double FPowerSpectraTopHat(double k, void *params)
{

  double *parameters = (double *)params;
  double growth_function = parameters[0];
  double P;
  double R[1], A[1];

  R[0] = parameters[1];
  
  A[0] = parameters[2];
 
  P = growth_function*FZeldovichSpect(k,A)*FTransferBBKS(k,NULL)*FWindowTopHat(k,R)*FWindowTopHat(k,R);

  return P;
}


