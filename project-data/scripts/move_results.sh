#!/bin/bash


for box in 10909 2710 16953; do
    for version in 10 14 15 16 17 18; do
	mv $box-0.0.1-run-v$version-* analysis-v2/0.0.1-run-v$version/$box/
    done
done