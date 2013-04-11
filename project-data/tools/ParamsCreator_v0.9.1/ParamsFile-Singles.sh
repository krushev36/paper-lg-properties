#!/bin/bash

VERSION=run-v1
BOX2RUN=16953
PROJECT="/media/D/proyectos-v2/project-galacticus9.0-v1/"
PATH_INPUT=$PROJECT"/input/"$VERSION"/"$BOX2RUN"/"
#PATH_INPUT="/z/ssanes/PROCOL_PROJECT/projects/project-galacticus9.1-v1/cathalogue/milky/"
PATH_OUT=$PROJECT"/output/"$VERSION"/"$BOX2RUN"/"
PATH_STORE=$PROJECT"/parameters/"$VERSION"/"$BOX2RUN"/"

#####################PARAMETER BLOCK RUTINE#############################
ParamsBlock(){
    if [ $# -eq 2 ]; then 
	echo -e "<parameter>"
	echo -e "<name>"$1"</name>"
	echo -e " <value>"$2"</value>"
	echo -e "</parameter>\n"
    fi
}

######################SETTLES ALL PARAMETERS############################
ParamsSetting(){
    if [ $# -eq 2 ]; then
	echo -e "<parameters>\n"
	echo -e "<!-- variable parameters -->"
	#ParamsBlock stabilityThresholdGaseous $3
	#ParamsBlock stabilityThresholdStellar $4
	echo -e "<!-- clues cosmology -->"
	ParamsBlock cosmologyMethod  "matter + lambda"
	ParamsBlock H_0  70.0
	ParamsBlock Omega_0 0.279
	ParamsBlock Omega_DE 0.721
	ParamsBlock Omega_b  0.046
	ParamsBlock sigma_8  0.817
	echo -e "<!-- some model settings -->"
	#ParamsBlock luminosityFilter  "UKIRT_K bJ RGO_I SDSS_g SDSS_r SDSS_i SDSS_z"
	#ParamsBlock luminosityType  "rest rest rest observed observed observed observed"
	#ParamsBlock luminosityRedshift "0.0 0.0 0.0 0.1 0.1 0.1 0.1"
	ParamsBlock mergerTreeOutputReferences true
	#ParamsBlock outputRedshifts "0.0050252" <------2710
	ParamsBlock outputRedshifts "0.0"
	ParamsBlock diskOutputStarFormationRate true
	ParamsBlock spheroidOutputStarFormationRate true
	ParamsBlock massAccretionHistoryOutput true
	echo -e "<!-- Properties to be read -->"
	ParamsBlock mergerTreeConstructMethod read
	ParamsBlock mergerTreeReadFileName $1
	ParamsBlock galacticusOutputFileName  $2
	ParamsBlock mergerTreeReadPresetSpins false
        #ParamsBlock treeNodeMethodPosition preset
        #ParamsBlock mergerTreeReadPresetPositions true	
	ParamsBlock treeNodeMethodPosition null
	ParamsBlock mergerTreeReadPresetPositions false
	ParamsBlock mergerTreeReadPresetMergerTimes false
	ParamsBlock mergerTreeReadPresetMergerNodes false
	ParamsBlock mergerTreeReadPresetSubhaloMasses false
	ParamsBlock mergerTreeReadPresetScaleRadii false
	ParamsBlock darkMatterProfileMinimumConcentration 3.8
	ParamsBlock haloSpinDistributionMethod lognormal
	ParamsBlock lognormalSpinDistributionMedian 0.031
	ParamsBlock lognormalSpinDistributionSigma 0.57
	ParamsBlock darkMatterConcentrationMethod "Gao 2008"
	ParamsBlock reionizationSuppressionVelocity 30.0
	ParamsBlock reionizationSuppressionRedshift 7.0
	ParamsBlock majorMergerMassRatio 0.2
	ParamsBlock minorMergerGasMovesTo disk
	echo -e "</parameters>"
    else
	echo $# "input arguments"
	exit 0
    fi    
}

######################SETTLES THE DIRECTORY SYSTEM############################
for dir in plots output analysis parameters input; do
    if [ ! -d "$PROJECT/$dir" ]; then
        echo -n "path $PROJECT/$dir not exist!"
        echo ".....creating"
        mkdir $PROJECT"/"$dir
    fi

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


####################ASK WETHER A DIRECTORY SHOUD BE CLEANED##############################
for dir in parameters output; do 
    if [ `ls $PROJECT"/$dir/"$VERSION"/"$BOX2RUN"/" | wc | awk '{print $1}' ` -gt 0 ]; then 
	echo "Path $PROJECT/$dir/$VERSION/$BOX2RUN/ already in use. Should it be cleaned? [yes/no]"
	read answer
	if [ $answer == yes ]; then 
	    rm  $PROJECT/$dir/$VERSION/$BOX2RUN/*
	fi    
    else
	echo "Path $PROJECT/$dir/$VERSION/$BOX2RUN/ ready to use"       
    fi
done 

###################OUTPUTS THE PARAMETER FILES###############################
if [ `ls $PATH_STORE | wc | awk 'NR==1 {print $1}'` -gt 0 ]; then
    echo "path $PATH_STORE already used! Store there anyway? [yes/no]"
    read answer
    if [ $answer == yes ]; then 
	for file in `ls $PATH_INPUT/*.hdf5`;do	    
	    echo $file
	    ID=`echo $file | tr "/" "\n" | awk 'END {print $1}' | sed -e 's/Clues2710_//' | sed -e 's/.hdf5//'`
	    echo $ID
	    echo "genereting "$PATH_STORE"/Clues"$BOX2RUN"_"$ID".xml"
	    ParamsSetting  $file  $PATH_OUT"/Clues"$BOX2RUN"_"$ID".hdf5" > $PATH_STORE"/Clues"$BOX2RUN"_"$ID".xml"
	done	
    else
	exit
    fi
else
    for file in `ls $PATH_INPUT/*.hdf5`;do	    	
	echo $file
	ID=`echo $file | tr "/" "\n" | awk 'END {print $1}' | sed -e 's/Clues2710_//' | sed -e 's/.hdf5//'  | sed -e 's/Clues16953_//'`		
	echo $ID
	if [ $ID -gt 200 ] && [ $ID -lt 1680 ]; then
	    echo "genereting "$PATH_STORE"/Clues"$BOX2RUN"_"$ID".xml"
	    ParamsSetting  $file  $PATH_OUT"/Clues"$BOX2RUN"_"$ID"_out.hdf5" > $PATH_STORE"/Clues"$BOX2RUN"_"$ID".xml"
        fi
    done	
fi



# for i in `echo "for(i=1.0; i<=3.0; i=i+0.1) i" | bc `; do
#     echo "genereting file $i"
#     ParamsSetting  $FILE_IN  $PATH_OUT"Clues"$box"LGDsikOutExpo"$i".hdf5" $i > $PATH_STORE"Clues"$box"LGDsikOutExpo"$i".xml"
# done

# for file in $path_in; do
#     #j=0.4
#     file_out=`awk ' {}'$file`
#     for i in `echo "for(i=0.7; i<0.9; i=i+0.01) i" | bc`; do
# 	ParamsSetting $file_in  $path_out"Clues2710StabilityGas"$j"Star"$i."hdf5" $j $i > $path_store"Clues2710StabilityGas"$j"Star"$i"Out.xml"
#     done

# done