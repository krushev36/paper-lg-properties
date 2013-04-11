#!/bin/bash

#parameters/
i=1
for box in 10909 16953 2710; do
    for file in parameters/0.0.1-LG-run-v2/$box/*; do
	nice Galacticus $file &
	if [ $i -ge 2 ]; then
            wait
            i=1
	else
            let i=$i+1
	fi
    done
done
wait
