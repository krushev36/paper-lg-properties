#!/bin/bash


if [ ! -d tot ]; then
    mkdir tot
fi


# cp 10909/10909-0.0.1-run-v5-props-z0.005.dat tot/0.0.1-run-v5-props-z0.005.dat
# cp 10909/10909-0.0.1-run-v5-luminosity-z0.1.dat tot/0.0.1-run-v5-luminosity-z0.1.dat
# cp 10909/10909-0.0.1-run-v5-tully-fisher-z0.1.dat tot/0.0.1-run-v5-tully-fisher-z0.1.dat


# less 16953/16953-0.0.1-run-v5-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-props-z0.005.dat
# less 2710/2710-0.0.1-run-v5-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-props-z0.005.dat


# less 16953/16953-0.0.1-run-v5-luminosity-z0.1.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-luminosity-z0.1.dat
# less 2710/2710-0.0.1-run-v5-luminosity-z0.1.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-luminosity-z0.1.dat


# less 16953/16953-0.0.1-run-v5-tully-fisher-z0.1.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-tully-fisher-z0.1.dat
# less 2710/2710-0.0.1-run-v5-tully-fisher-z0.1.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-tully-fisher-z0.1.dat


# cp 10909/10909-0.0.1-run-v5-mw-type-estimator.dat tot/0.0.1-run-v5-mw-type-estimator.dat
# less 16953/16953-0.0.1-run-v5-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-mw-type-estimator.dat
# less 2710/2710-0.0.1-run-v5-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-mw-type-estimator.dat



cp 10909/10909-0.0.1-run-v5-mwtype-props-z0.005.dat tot/0.0.1-run-v5-mwtype-props-z0.005.dat
less 16953/16953-0.0.1-run-v5-mwtype-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-mwtype-props-z0.005.dat
less 2710/2710-0.0.1-run-v5-mwtype-props-z0.005.dat | awk 'NR>1 {print }'  >> tot/0.0.1-run-v5-mwtype-props-z0.005.dat




