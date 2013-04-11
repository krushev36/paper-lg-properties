#!/bin/bash


for run in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do
    ./calculus/statistics-mw-type.out analysis-v2/0.0.1-run-v$run/tot/0.0.1-run-v$run-mwtype-props-z0.005.dat \
   	analysis-v4/0.0.1-run-v$run/tot/0.0.1-run-v$run-mw-type-estimation-no-halo-no-vel-z0.005-v4.dat
    
   ./calculus/statistics-m31-type.out analysis-v2/0.0.1-run-v$run/tot/0.0.1-run-v$run-mwtype-props-z0.005.dat \
      analysis-v4/0.0.1-run-v$run/tot/0.0.1-run-v$run-m31-type-estimation-no-halo-no-vel-z0.005-v4.dat
done 
