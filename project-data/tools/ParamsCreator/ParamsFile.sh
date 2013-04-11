#!/bin/bash

VERSION=0.0.1-LG-run-v2
PROJECT="/media/D/proyectos-v2/project-galacticus9.0-v1/"

##  Filling Parameter Rutine
ParamsBlock(){
    if [ $# -eq 2 ]; then
	echo -e "<parameter>"
	echo -e "<name>"$1"</name>"
	echo -e " <value>"$2"</value>"
	echo -e "</parameter>\n"
    fi
}

## Setting Parameters Rutine
ParamsSetting(){
    if [ $# -gt 2 ]; then
	echo -e "<parameters>\n"
	ParamsBlock starFormationDiskEfficiency $3
	ParamsBlock minorMergerGasMovesTo disk
	#ParamsBlock diskOutflowExponent $3
	ParamsBlock cosmologyMethod  "matter + lambda"
	ParamsBlock H_0  70.0
	ParamsBlock Omega_0 0.279
	ParamsBlock Omega_DE 0.721
	ParamsBlock Omega_b  0.046
	ParamsBlock sigma_8  0.817
	ParamsBlock reionizationSuppressionRedshift 7.0
	ParamsBlock reionizationSuppressionVelocity 30
	ParamsBlock majorMergerMassRatio 0.3
	ParamsBlock haloSpinDistributionMethod lognormal
	ParamsBlock lognormalSpinDistributionMedian 0.031
	ParamsBlock lognormalSpinDistributionSigma 0.57
	ParamsBlock luminosityFilter  "bJ UKIRT_K SDSS_g SDSS_r SDSS_i SDSS_z bJ UKIRT_K SDSS_g SDSS_r SDSS_i SDSS_z"
	ParamsBlock luminosityType  "observed observed observed observed observed observed rest rest rest rest rest rest"
	ParamsBlock luminosityRedshift "0.1 0.1 0.1 0.1 0.1 0.1 0.0050252 0.0050252 0.0050252 0.0050252 0.0050252 0.0050252"
	ParamsBlock outputRedshifts "0.1 0.0050252"
	ParamsBlock mergerTreeConstructMethod read
	ParamsBlock mergerTreeReadFileName $1
	ParamsBlock galacticusOutputFile  $2
	echo -e "</parameters>"
    else
	echo $# "input arguments"
	exit 0
    fi
}





for dir in output analysis parameters; do
    if [ ! -d "$PROJECT/$dir/$VERSION" ]; then
	echo -n "path $PROJECT/$dir/$VERSION not exist!"
	echo ".....creating"
	mkdir $PROJECT"/"$dir"/"$VERSION
    else
	echo "path $PROJECT/$dir/$VERSION already exist"
    fi
    for box in 10909 2710 16953; do
	if [ $dir == 'plots' ]; then
	    continue
	fi
	if [ ! -d "$PROJECT/$dir/$VERSION/$box" ] && [ $dir != 'plots' ]; then
	    echo -n "path $PROJECT/$dir/$VERSION/$box not exist!"
	    echo ".....creating"
	    mkdir $dir"/"$VERSION"/"$box
	else
	    echo "path $PROJECT/$dir/$VERSION already exist"
	fi
    done
done


for box in 10909 2710 16953; do

    FILE_IN=$PROJECT"/input/LG/"$box"/CLUES_"$box"_LG.hdf5"
    PATH_OUT=$PROJECT"/output/"$VERSION"/"$box"/"
    PATH_STORE=$PROJECT"/parameters/"$VERSION"/"$box"/"
    
    if [ `ls $PATH_STORE | wc | awk 'NR==1 {print $1}'` -gt 0 ]; then
    	echo "path $PATH_STORE already used"	
    	continue
    fi

    for i in `echo "for(i=0.005; i<=0.1; i=i+0.005) i" | bc `; do
	echo "genereting file $i"
	ParamsSetting  $FILE_IN  $PATH_OUT"Clues"$box"LGDsikOutExpo"$i".hdf5" $i > $PATH_STORE"Clues"$box"LGDsikOutExpo"$i".xml"	
    done    
done



