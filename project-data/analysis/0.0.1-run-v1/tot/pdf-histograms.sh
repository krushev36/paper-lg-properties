#!/bin/bash



less 0.0.1-run-v1-mw-type-estimation-z0.005.dat | awk 'NR>1{print $10}' | gsl-histogram 0.0 5.2 20 > chi-squared-histogram.dat
less 0.0.1-run-v1-mw-type-estimation-z0.005.dat | awk 'NR>1{print $11}' | gsl-histogram 0.0 6.0 6 > eta-histogram.dat