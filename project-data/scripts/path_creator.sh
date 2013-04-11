#!/bin/bash


for run in 0.0.1-LG-run-v1 0.0.1-LG-run-v2; do
    mkdir analysis-v2/$run
    mkdir output/$run
    mkdir parameters/$run
    for box in 10909 2710 16953; do
	mkdir analysis-v2/$run/$box
	mkdir output/$run/$box
	mkdir parameters/$run
    done
done 