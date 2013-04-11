#!/bin/bash


PARAMETERS="H_0 Omega_DE"
PROPERTIES="nodeMass   spheroidStellarMass   spheroidGasMass  diskStellarMass  diskGasMass"


PROJECT=/media/D/proyectos-v2/project-galacticus-v1/
BOX=10909
VERSION=run-v4
#haloName=milky
treeId=831
outputId=2
PATHINPUT=output/$VERSION/$BOX/
FILEOUTPUT=$BOX-run-v4-default.dat


##################### EXTRACTING RUTINE ######################
PrintGalacticusDataset(){
    if [ -f $1 ]; then     
	
	outputId=$2
	treeId=$3
	FILE=$1
	LIST1=$4
	LIST2=$5

	#datasetInput="/massAccretionHistories/mergerTree0/"
        datasetProps="/Outputs/Output$outputId/nodeData"
        datasetParams="/Parameters/"

        treeId=`echo $1 | tr "/" "\n" | awk 'END {print $1}' | tr "_" "\n" | awk 'NR==2 {print $1}'`

        h5ls -dS  $1$datasetProps"/nodeMass" | sed '1,2{d}' > dumbfile
        main_value=`awk '$1 > max { max=$1; line=NR }; END { print line }' dumbfile`	

	i=1
	for params in `echo $LIST1 | tr "[:blank:]" "\n"`; do
	    h5ls -gv $1$datasetParams > dumbfile
	    PARAMETERS[i]=`awk "/$params/{getline;getline;print}" dumbfile | awk '{print $2}'`
	    echo -n "${PARAMETERS[i]}              "	    
	    i=$(($i+1))
	done

	rm dumbfile

	i=1
	for props in `echo $LIST2 | tr "[:blank:]" "\n"`; do
	    h5ls -dS $1$datasetProps"/"$props   | sed '1,2{d}'   > dumbfile
	    PROPERTIES[i]=`awk "NR==$main_value"' {print $1}' dumbfile`
	    echo -n "${PROPERTIES[i]}        "
	    i=$(($i+1))
	done
	echo ""

	rm dumbfile
      
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









#PARAMETERS="starFormationSpheroidEfficiency"
#starFormationSpheroidEfficiency  stabilityThresholdStellar stabilityThresholdGaseous" #  diskCircularVelocity"

	    #PROPERTIES[i]=`awk 'NR==3 {print $1}' dumbfile`
	    #`awk 'BEGIN {sum=0}  {sum+=$1} END {print sum}' dumbfile`

        # h5ls -dS $1$datasetInput"/nodeMass"  > dumbfile
        # nodeInputMass=`awk 'NR==3 {print $1}' dumbfile`



        # h5ls -dS $1$datasetProps"/spheroidGasMass" | sed '1,2{d}'  > dumbfile
        # spheroidGasMass=`awk "NR==$main_value"' {print $1}' dumbfile`

        # h5ls -dS $1$datasetProps"/diskStellarMass" | sed '1,2{d}'  > dumbfile
        # diskStellarMass=`awk "NR==$main_value"' {print $1}' dumbfile`

        # h5ls -dS $1$datasetProps"/diskGasMass" | sed '1,2{d}'  > dumbfile
        # diskGasMass=`awk "NR==$main_value"' {print $1}' dumbfile`

        # h5ls -dS $1$datasetProps"/diskCircularVelocity" | sed '1,2{d}'  > dumbfile
        # diskCircularVelocity=`awk "NR==$main_value"' {print $1}' dumbfile`
        # rm dumbfile*




        # h5ls -dS $1$datasetProps"/spheroidStellarMass" | sed '1,2{d}'  > dumbfile
        # spheroidStellarMass=`awk "NR==$main_value"' {print $1}' dumbfile`


        # nodeMass=`awk "NR==$main_value"' {print $1}' dumbfile`

