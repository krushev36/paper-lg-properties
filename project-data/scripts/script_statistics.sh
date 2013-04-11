#!/bin/bash


# for version in 10 14 15 16 17 18; do
#     ./calculus/statistics.out analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-props-temp-z0.005.dat \
# 	analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-star-frac-stats.dat
# done

# for version in 10 14 15 16 17 18; do
#     ./calculus/statistics-bulge.out  analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-props-temp-z0.005.dat \
# 	analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-bulge-frac-stats.dat
# done


for version in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18; do
    ./calculus/statistics-gas.out  analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-props-temp-z0.005.dat \
	analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-gas-frac-stats.dat
done


# for version in 10 14 15 16 17 18; do
#     ./calculus/statistics-HR.out  analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-Color-Mag-z0.1.dat \
# 	analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-Color-Mag-stats-z0.1.dat
# done


# for version in 10 14 15 16 17 18; do
#     ./calculus/statistics-tully.out  analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-tully-fisher2-z0.1.dat \
# 	analysis-v2/0.0.1-run-v$version/tot/0.0.1-run-v$version-tully-fisher2-stats-z0.1.dat
# done










#analysis-v2/0.0.1-run-v1/tot/0.0.1-run-v1-mwtype-props-z0.005.dat