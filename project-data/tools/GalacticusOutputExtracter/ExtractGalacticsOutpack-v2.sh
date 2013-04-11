#!/bin/bash

PrintGalacticusDataset(){
    if [ -f $1 ]; then     

	outputId=$2
	treeId=$3

	datasetProps="/Outputs/Output"$outputId"/mergerTree"$treeId
	datasetParams="/Parameters/"

	h5ls -dS $1$datasetParams"starFormationDiskEfficiency"  > dumbfile
	starFormationDiskEfficiency=`awk 'NR==3 {print $1}' dumbfile`

	h5ls -dS $1$datasetParams"starFormationSpheroidEfficiency"  > dumbfile
	starFormationSpheroidEfficiency=`awk 'NR==3 {print $1}' dumbfile`

	h5ls -dS $1$datasetProps"/nodeMass"  > dumbfile
	nodeMass=`awk 'NR==3 {print $1}' dumbfile`

	h5ls -dS $1$datasetProps"/spheroidStellarMass"  > dumbfile
	spheroidStellarMass=`awk 'NR==3 {print $1}' dumbfile`

	h5ls -dS $1$datasetProps"/spheroidGasMass"  > dumbfile
	spheroidGasMass=`awk 'NR==3 {print $1}' dumbfile`
	rm dumbfile

	h5ls -dS $1$datasetProps"/diskStellarMass"  > dumbfile
	diskStellarMass=`awk 'NR==3 {print $1}' dumbfile`

	h5ls -dS $1$datasetProps"/diskGasMass"  > dumbfile
	diskGasMass=`awk 'NR==3 {print $1}' dumbfile`

	h5ls -dS $1$datasetProps"/diskCircularVelocity"  > dumbfile
	diskCircularVelocity=`awk 'NR==3 {print $1}' dumbfile`
	rm dumbfile
	
	echo $starFormationSpheroidEfficiency"                  "$starFormationDiskEfficiency \
            "                "$nodeMass \
            "      "$diskStellarMass"      "$diskGasMass"    "$diskCircularVelocity \
    	    "              "$spheroidStellarMass"          "$spheroidGasMass

    else
	echo "file not found"
	exit 0    
    fi
}


treeId=675
outputId=2
pathInput=/media/D/proyectos-v2/project-galacticus-v1/output/10909/
fileOutput=LG-default-params.dat

for file in $pathInput/*; do
    echo $file
    if [ ! -f $fileOutput ]; then
	echo  '#1.starSpheroidEff    #2.starDiskEff      '\
              '#3.nodeMass      #4.diskStellarMass     #5.diskGasMass        #6.diskCircularVelocity' \
	      '     #7.spheroidStellarMass      #8.spheroidGasMass' >> $fileOutput
    fi
    PrintGalacticusDataset $file $outputId $treeId >> $fileOutput
done



