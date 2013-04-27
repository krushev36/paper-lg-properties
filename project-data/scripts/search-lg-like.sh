#!/bin/bash

# awk 'NR==1{print $3"  "0.3}' analysis/0.0.1-run-v3/tot/m31-chi-squared-no-halo-histogram.dat  > analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
# awk 'NR==1{print $3"  "0.2}' analysis/0.0.1-run-v10/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat
# #awk 'NR==1{print $3"  "3.0}' analysis/0.0.1-run-v9/tot/m31-chi-squared-no-halo-histogram.dat  >> analysis/0.0.1-run-v1-10/0.0.1-run-v1-10-m31-chi-squared-merger-rate.dat




DiskGasMin="0.6e10"
DiskGasMax="0.8e10"
DiskStarMassMin="3.3e10"
DiskStarMassMax="4.5e10"
BulgeMassMin="0.5e10"
BulgeMassMax="1.2e10"

echo "searching MW-like galaxies:"
pathdata="analysis-v4"
for id in `echo "for(i=1; i<=16; i=i+1) i" | bc`; do
    file=$pathdata"/0.0.1-run-v"$id"/tot/0.0.1-run-v"$id"-mw-type-estimation-no-halo-z0.005-v4.dat"
    if [ -f  $file ]; then
	echo $file
	#awk 'NR>1  && $5>='"$DiskStarMassMin"' && $5<='"$DiskStarMassMax"'  && $3>='"$BulgeMassMin"' && $3<='"$BulgeMassMax"'{print}' $file
	awk 'NR>1  && $5>=3.3e10 && $5<=4.5e10  && $3>=0.5e10 && $3<=1.2e10 {print}' $file
    fi 
done

########################################################################################################################

DiskGasMin="0.5e10"
DiskGasMax="0.6e10"
DiskStarMassMin="7e10"
DiskStarMassMax="10e10"
BulgeMassMin="1.9e10"
BulgeMassMax="3.3e10"

echo "searching M31-like galaxies:"
pathdata="analysis-v4"
for id in `echo "for(i=1; i<=16; i=i+1) i" | bc`; do
    file=$pathdata"/0.0.1-run-v"$id"/tot/0.0.1-run-v"$id"-mw-type-estimation-no-halo-z0.005-v4.dat"
    if [ -f  $file ]; then
	echo $file
	awk 'NR>1  && $5>='"$DiskStarMassMin"' && $5<='"$DiskStarMassMax"'  && $3>='"$BulgeMassMin"' && $3<='"$BulgeMassMax"'{print}' $file
	
    fi 
done


