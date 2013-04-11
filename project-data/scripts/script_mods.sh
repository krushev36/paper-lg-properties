#!/bin/bash

# for version in 10 14 15 16 17 18;do 
#     echo $version
#     awk '{print $2"    "$4"   "$6"    "$8 }' analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-luminosity-z0.1.dat > \
# 	analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-Color-Mag-z0.1.dat
# done


# for version in 10 14 15 16 17 18;do 
#     echo $version
#     awk 'NF>1 {print $2"    "$3"    "$4"    "$5"      "$6 }' analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-mwtype-props-z0.005.dat > \
# 	analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-props-temp-z0.005.dat
# done

#0.0.1-run-v1-mwtype-props-z0.005.dat
for version in 0.0.1-LG-run-v1 0.0.1-LG-run-v2; do
    for box in 10909 2710 16953; do
	awk 'NF>1 {print}' analysis-v2/$version/$box/MW/milky-$box-$version-props-z0.005.dat >  analysis-v2/$version/$box/MW/milky-$box-$version-props-z0.005-v2.dat
    done
done