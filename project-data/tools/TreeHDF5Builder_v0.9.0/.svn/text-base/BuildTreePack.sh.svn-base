#!/bin/bash


#************************************************************
#                 check passed arguments
#************************************************************
if  [ $# -ne 3 ]; then
    echo "not enough arguments"
    exit
fi


if  [ ! -d $1 ] || [ ! -d $1 ]; then
    echo "$1 or $2 not a directory"
    exit
fi

PATH_LOCAL_IN=$1
PATH_LOCAL_OUT=$2

FILE_OUT=$PATH_LOCAL_OUT/$3

FILE_REPORT_LOAD=$PATH_LOCAL_OUT/packed_trees.inf 
FILE_REPORT_WRONG=$PATH_LOCAL_OUT/blacklisted_trees.inf


#************************************************************
#                remove earlier  files
#************************************************************
if  [ -f $FILE_OUT ]; then
    rm $FILE_OUT
fi

i=1
for FILE_INF in `ls -vx $PATH_LOCAL_OUT/*.inf`; do
    
    echo  $FILE_INF

    if  [ -f  $FILE_INF ]; then
	rm $PATH_LOCAL_OUT/*.inf
    fi
    
    i=$(($i+1))

    if [ i==1 ]; then
	break
    fi

done

#************************************************************
#                    check rutines 
#************************************************************
cd RUTINES

i=1
for FILE_O in *.o; do
    
    if  [ ! -f  $FILE_O ]; then
	make
    fi
    
    i=$(($i+1))

    if [ i==1 ]; then
	break
    fi

done

cd ..
make


#************************************************************
#       check trees integrity and store output
#************************************************************
i=1
for FILE_IN in  `ls -vx $PATH_LOCAL_IN/`;do

    FILE_IN=$PATH_LOCAL_IN/$FILE_IN
    ID=`awk 'NR==1 {print $1}' $FILE_IN`

    STATUS=`./MergerTreeCleanBad.out $FILE_IN  | awk \"$0\"`

    if	[ $STATUS -eq 1 ]  ; then
	echo  "$ID  not stored: $FILE_IN" >>  $FILE_REPORT_WRONG
    else 
	./MergerTreeHDF5.out $FILE_IN  $FILE_OUT $i

	echo  "$ID  stored: $FILE_IN  as  $i" >> $FILE_REPORT_LOAD

	i=$(($i+1))       
    fi

done





# PATH_LOCAL_IN=storage/BOX64/IC_16953/TOT
# PATH_LOCAL_OUT=storage/BOX64/IC_16953/HDF5TreePack

# FILE_OUT=$PATH_LOCAL_OUT/clues_16953_tot.hdf5
