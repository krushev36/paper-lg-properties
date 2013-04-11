#!/bin/bash


PARAMETERS="H_0 Omega_DE"
PROPERTIES="nodeMass   spheroidStellarMass   spheroidGasMass  diskStellarMass  diskGasMass"


PROJECT=/media/D/proyectos-v2/project-galacticus9.0-v1/
BOX=10909
VERSION=0.0.1-LG-run-v1
haloName=milky
treeId=894
outputId=2
PATHINPUT=output/$VERSION/$BOX/
FILEOUTPUT=v0.0.1-LG-$BOX-$VERSION.dat


##################### EXTRACTING RUTINE ######################
PrintGalacticusDataset(){
    if [ -f $1 ]; then     
	
	outputId=$2
	treeId=$3
	FILE=$1
	LIST1=$4
	LIST2=$5

	#datasetInput="/massAccretionHistories/mergerTree0/"
        datasetProps="/Outputs/Output$outputId/mergerTree"$treeId
        datasetParams="/Parameters/"

        treeId=`echo $1 | tr "/" "\n" | awk 'END {print $1}' | tr "_" "\n" | awk 'NR==2 {print $1}'`

        h5ls -dS  $1$datasetProps"/nodeMass" | sed '1,2{d}' > dumbfile
        main_value=`awk '$1 > max { max=$1; line=NR }; END { print line }' dumbfile`	

	i=1
	for params in `echo $LIST1 | tr "[:blank:]" "\n"`; do
	    PARAMETERS[i]=`awk "/$params/{getline;getline;print}"  | awk '{print $2}' | h5ls -dS $1$datasetParams `
	    echo -n "${PARAMETERS[i]}              "	    
	    i=$(($i+1))
	done

	i=1
	for props in `echo $LIST2 | tr "[:blank:]" "\n"`; do	    
	    PROPERTIES[i]=`awk "NR==$main_value"' {print $1}' | sed '1,2{d}' | h5ls -dS $1$datasetProps"/"$props`
	    echo -n "${PROPERTIES[i]}        "
	    i=$(($i+1))
	done
	echo ""
      
    else
	echo "$1 not found"
	#exit 0    
    fi
}


###################### LOOP OVER PATH ######################
for file in $PATHINPUT/*; do
    if [ ! -f $FILEOUTPUT ]; then

    	i=1
    	for params in `echo $PARAMETERS | tr "[:blank:]" "\n"`; do
    	    header=$header"#$i.$params           "
    	    i=$(($i+1))
    	done

    	for props in `echo $PROPERTIES | tr "[:blank:]" "\n"`; do
    	    header=$header"#$i.$props            "
    	    i=$(($i+1))
    	done

    	echo $header >> $FILEOUTPUT
    fi
    echo "extracting....$file"
    PrintGalacticusDataset $file $outputId $treeId  "$PARAMETERS" "$PROPERTIES" >> $FILEOUTPUT
done
