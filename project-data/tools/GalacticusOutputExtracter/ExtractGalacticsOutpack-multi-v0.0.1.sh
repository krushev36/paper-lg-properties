#!/bin/bash

txtred=$(tput setaf 1)    # Red   
txtrst=$(tput sgr0)       # Text reset   

# if  [ $# -ne 2 ]; then
#     echo -e  "\n${txtred}This script is designed to extract the baryonic information of a pack of merger trees in a file.\n"
#     echo -e  "USAGE:"
#     echo -e  "      ./BuildTreePack.sh (char)input-path  (char)output-file ${txtrst}"
#     exit
# fi


PARAMETERS="diskOutflowExponent"
#PARAMETERS=""
PROPERTIES="nodeMass   spheroidStellarMass   spheroidGasMass   diskStellarMass    diskGasMass   diskCircularVelocity     spheroidCircularVelocity   diskScaleLength"

#"  diskStellarLuminosity:SDSS_g:observed:z0.1000   diskStellarLuminosity:SDSS_i:observed:z0.1000   diskStellarLuminosity:SDSS_r:observed:z0.1000   diskStellarLuminosity:SDSS_z:observed:z0.1000   spheroidStellarLuminosity:SDSS_g:observed:z0.1000   spheroidStellarLuminosity:SDSS_i:observed:z0.1000   spheroidStellarLuminosity:SDSS_r:observed:z0.1000   spheroidStellarLuminosity:SDSS_z:observed:z0.1000"


PROJECT=/media/D/proyectos-v2/project-galacticus9.0-v1
BOX=10909
VERSION=0.0.1-LG-run-v3
treeId=894
outputId=2
haloName=milky
PATHINPUT=output/$VERSION/$BOX/
FILEOUTPUT=$haloName-$BOX-$VERSION-properties-z0.005.dat

##################### EXTRACTING RUTINE ######################
PrintGalacticusDataset(){
    if [ -f $1 ]; then

	outputId=$2
	FILE=$1
	treeId=$3
	LIST1=$4
	LIST2=$5

	#datasetInput="/massAccretionHistories/mergerTree0/"
	# treeId=`echo $1 | tr "/" "\n" | awk 'END {print $1}' | tr "_" "\n" | awk 'NR==2 {print $1}'`

	datasetParams="/Parameters/"
	datasetProps="/Outputs/Output$outputId/mergerTree$treeId"
	echo -n "$treeId              "

	nbparams=`echo $LIST1 | wc | awk '{print $2}'`
	nbprops=`echo $LIST2 | wc | awk '{print $2}'`

	if [ $nbparams -gt 0  ]; then
	    i=1
	    for params in `echo $LIST1 | tr "[:blank:]" "\n"`; do
		h5ls -dS $1$datasetParams/$params > dumbfile
		PARAMETERS[i]=`awk 'NR==3 {print}' dumbfile`
		echo -n "${PARAMETERS[i]}              "
		i=$(($i+1))
	    done
	    rm dumbfile
	fi


	if [ $nbprops -gt 0  ]; then
	    h5ls -dS  $1$datasetProps"/nodeMass" | sed '1,2{d}' > dumbfile
	    main_value=`awk '$1 > max { max=$1; line=NR }; END { print line }' dumbfile`
	    i=1
	    for props in `echo $LIST2 | tr "[:blank:]" "\n"`; do
	    	h5ls -dS $1$datasetProps"/"$props   | sed '1,2{d}' > dumbfile
	    	PROPERTIES[i]=`awk "NR==$main_value"' {print $1}' dumbfile`
	    	echo -n "${PROPERTIES[i]}        "
	    	i=$(($i+1))
	    done
	    rm dumbfile
	fi

	echo ""

    else
	echo "$1 not found"
	exit 1
    fi
}



###################### LOOP OVER PATH ######################
nbparams=`echo $PARAMETERS |  wc | awk '{print $2}'`
nbprops=`echo $PROPERTIES |  wc | awk '{print $2}'`

i=1
for file in `ls -v $PATHINPUT/*hdf5`; do
    if [ ! -f $FILEOUTPUT ]; then
	header=$header"#$i.ID             "
	i=$(($i+1))
	
	if [ $nbparams -gt 0  ]; then
	    for params in `echo $PARAMETERS | tr "[:blank:]" "\n"`; do
		header=$header"#$i.$params             "
		i=$(($i+1))
	    done
	fi
	
	if [ $nbprops -gt 0  ]; then
	    for props in `echo $PROPERTIES | tr "[:blank:]" "\n"`; do
		header=$header"#$i.$props              "
		i=$(($i+1))
	    done
	fi
	
	echo $header >> $FILEOUTPUT
    fi
    echo "extracting....$file"
    PrintGalacticusDataset $file $outputId  $treeId "$PARAMETERS" "$PROPERTIES" >> $FILEOUTPUT
done

