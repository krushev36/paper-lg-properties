#!/bin/bash


if [ ! -d tot ]; then
    mkdir tot
fi

awk 'NF>1 {print }' 10909/10909-0.0.1-run-v16-mwtype-props-z0.005.dat > tot/0.0.1-run-v16-mwtype-props-z0.005.dat
awk 'NF>1 {print }' 10909/10909-0.0.1-run-v16-luminosity-z0.1.dat > tot/0.0.1-run-v16-luminosity-z0.1.dat
awk 'NF>1 {print }' 10909/10909-0.0.1-run-v16-tully-fisher2-z0.1.dat > tot/0.0.1-run-v16-tully-fisher2-z0.1.dat


awk 'NR>1 && NF>1 {print }' 16953/16953-0.0.1-run-v16-mwtype-props-z0.005.dat  >> tot/0.0.1-run-v16-mwtype-props-z0.005.dat
awk 'NR>1 && NF>1 {print }' 2710/2710-0.0.1-run-v16-mwtype-props-z0.005.dat   >> tot/0.0.1-run-v16-mwtype-props-z0.005.dat


awk 'NR>1 && NF>1 {print }' 16953/16953-0.0.1-run-v16-luminosity-z0.1.dat   >> tot/0.0.1-run-v16-luminosity-z0.1.dat
awk 'NR>1 && NF>1 {print }' 2710/2710-0.0.1-run-v16-luminosity-z0.1.dat   >> tot/0.0.1-run-v16-luminosity-z0.1.dat


awk 'NR>1 && NF>1 {print }' 16953/16953-0.0.1-run-v16-tully-fisher2-z0.1.dat   >> tot/0.0.1-run-v16-tully-fisher2-z0.1.dat
awk 'NR>1 && NF>1 {print }' 2710/2710-0.0.1-run-v16-tully-fisher2-z0.1.dat   >> tot/0.0.1-run-v16-tully-fisher2-z0.1.dat





# cp 10909/10909-0.0.1-run-v16-mw-type-estimator.dat tot/0.0.1-run-v16-mw-type-estimator.dat
# less 16953/16953-0.0.1-run-v16-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v16-mw-type-estimator.dat
# less 2710/2710-0.0.1-run-v16-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v16-mw-type-estimator.dat


# cp 10909/10909-0.0.1-run-v16-mwtype-props-z0.005.dat tot/0.0.1-run-v16-mwtype-props-z0.005.dat
# less 16953/16953-0.0.1-run-v16-mwtype-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v16-mwtype-props-z0.005.dat
# less 2710/2710-0.0.1-run-v16-mwtype-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v16-mwtype-props-z0.005.dat




