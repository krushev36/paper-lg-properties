#!/bin/bash


VERSION=test-v7
PROJECT="/media/D/proyectos-v2/project-galacticus-v1/"

##  Filling Parameter Rutine                                                                                                                                                                    
ParamsBlock(){
    if [ $# -eq 2 ]; then
        echo -e "<parameter>"
        echo -e "<name>"$1"</name>"
        echo -e " <value>"$2"</value>"
        echo -e "</parameter>\n"
    fi
}

ParamsSetting(){
    if [ $# -eq 4 ]; then
	echo -e "<parameters>\n"
	echo -e "<!-- variable parameters -->"
	ParamsBlock stabilityThresholdGaseous $3
	# ParamsBlock stabilityThresholdStellar $4
	echo -e "<!-- clues cosmology -->"
	ParamsBlock cosmologyMethod  "matter + lambda"
	ParamsBlock H0  70.0
	ParamsBlock Omega_Matter 0.279
	ParamsBlock Omega_DE 0.721
	ParamsBlock Omega_b  0.046
	ParamsBlock sigma_8  0.817
	echo -e "<!-- some settings -->"
	ParamsBlock luminosityFilter  "UKIRT_K bJ RGO_I SDSS_g SDSS_r SDSS_i SDSS_z"
	ParamsBlock luminosityType  "rest rest rest observed observed observed observed"
	ParamsBlock luminosityRedshift "0.0 0.0 0.0 0.1 0.1 0.1 0.1"
	ParamsBlock outputRedshifts "0.0 0.1"
	echo -e "<!-- Properties to be read -->"
	ParamsBlock mergerTreeConstructMethod read
	ParamsBlock mergerTreeReadFileName $1
	ParamsBlock galacticusOutputFile  $2
	ParamsBlock mergerTreeReadPresetSpins true
	#ParamsBlock treeNodeMethodPosition preset
	ParamsBlock treeNodeMethodPosition preset
	ParamsBlock mergerTreeReadPresetPositions false
	#ParamsBlock mergerTreeReadPresetPositions true
	ParamsBlock mergerTreeReadPresetMergerTimes false
	ParamsBlock mergerTreeReadPresetMergerNodes false
	ParamsBlock mergerTreeReadPresetSubhaloMasses false
	echo -e "</parameters>"
    else
	echo $# "input arguments"
	exit 0
    fi    
}



for dir in plots output analysis parameters; do
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
    #           |
    #input file V
    FILE_IN=$PROJECT"/cathalogue/clues_"$box"_LG.hdf5"    
    PATH_OUT=$PROJECT"/output/"$VERSION"/"$box"/"
    PATH_STORE=$PROJECT"/parameters/"$VERSION"/"$box"/"

    if [ `ls $PATH_STORE | wc | awk 'NR==1 {print $1}'` -gt 0 ]; then
        echo "path $PATH_STORE already used"
        continue
    fi

    for i in `echo "for(i=1.0; i<=3.0; i=i+0.1) i" | bc `; do
        echo "genereting file $i"
        ParamsSetting  $FILE_IN  $PATH_OUT"Clues"$box"LGDsikOutExpo"$i".hdf5" $i > $PATH_STORE"Clues"$box"LGDsikOutExpo"$i".xml"
    done

done




# file_in=/media/D/proyectos-v2/project-galacticus-v1/cathalogue/clues_10909_LG.hdf5
# path_out=/media/D/proyectos-v2/project-galacticus-v1/output/10909/
# path_store=/media/D/proyectos-v2/project-galacticus-v1/parameters/10909/

# for i in `echo "for(i=0.01; i<0.2; i=i+0.01) i" | bc`; do
#     for j in `echo "for(i=0.1; i>0.0; i=i-0.01) i" | bc`; do
# 	ParamsSetting() $file_in  $path_out"Clues10909LGOutDisk"$i"Bulge"$j".hdf5" $i $j > $path_store"Clues10909LGParamsDisk"$i"Bulge"$j".xml"
#     done
# done