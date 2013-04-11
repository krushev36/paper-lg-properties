#!/bin/bash

for run in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do
    awk  'NR>1 {print $10}' analysis-v4/0.0.1-run-v$run/tot/0.0.1-run-v$run-mw-type-estimation-no-halo-no-vel-z0.005-v4.dat | gsl-histogram 0.0 20.0 25 > analysis-v4/0.0.1-run-v$run/tot/mw-chi-squared-no-halo-no-vel-histogram-v4.dat    
    
    awk 'NR>1 {print $10}' analysis-v4/0.0.1-run-v$run/tot/0.0.1-run-v$run-m31-type-estimation-no-halo-no-vel-z0.005-v4.dat | gsl-histogram 0.0 20.0 25 >  analysis-v4/0.0.1-run-v$run/tot/m31-chi-squared-no-halo-no-vel-histogram-v4.dat
done


