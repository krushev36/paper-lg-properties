#!/bin/bash

awk 'NR==1{print $3"  "0.3}' analysis/0.0.1-run-v3/tot/m31-chi-squared-no-halo-histogram.dat  > analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
awk 'NR==1{print $3"  "0.2}' analysis/0.0.1-run-v10/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
#awk 'NR==1{print $3"  "3.0}' analysis/0.0.1-run-v9/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat


echo -e "\n\n" >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat

awk 'NR==2{print $3"  "0.3}' analysis/0.0.1-run-v3/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
awk 'NR==2{print $3"  "0.2}' analysis/0.0.1-run-v10/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
#awk 'NR==2{print $3"  "3.0}' analysis/0.0.1-run-v9/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat


echo -e "\n\n" >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat

awk 'NR==3{print $3"  "0.3}' analysis/0.0.1-run-v3/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
awk 'NR==3{print $3"  "0.2}' analysis/0.0.1-run-v10/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
#awk 'NR==3{print $3"  "3.0}' analysis/0.0.1-run-v9/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat



# echo -e "\n\n" >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat

# awk 'NR==4{print $3"  "0.01}' analysis/0.0.1-run-v7/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
# awk 'NR==4{print $3"  "0.02}' analysis/0.0.1-run-v8/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
# awk 'NR==4{print $3"  "0.05}' analysis/0.0.1-run-v9/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
