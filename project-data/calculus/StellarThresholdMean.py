#!/usr/bin/python

import sys, math,  array
from numpy import *


inputFile = open('/media/D/proyectos-v2/project-galacticus-v1/analysis/10909/milky-run1-params.dat', 'r'); inputFile.readline()
lines = inputFile.readlines()
properties = zeros( (len(lines),8) )

i=0
for line in lines:
    for j in range(0,8):
        properties[i,j] = float(line.split()[j])        
    i += 1

inputFile.close()


meanNodeMass = zeros(20)
meanDiskStellarMass = zeros(20); meaDiskgasMass = zeros(20)
meanSpheroidStellarMass = zeros(20); meanSpheroidGassMass = zeros(20)

meanProperties = zeros(8)

count = 0
for diskOutFlowExp in arange(1, 3, 0.1):

    for i in range(0,len(lines)):
        if properties[i,0] == diskOutFlowExp:
            meanNodeMass[count] += properties[i,2]
            meanDiskStellarMass[count] += properties[i,3];   meaDiskgasMass[count] += properties[i,4]
            meanSpheroidStellarMass[count] = properties[i,6];   meanSpheroidGassMass[count] = properties[i,7]
    meanNodeMass[count] /= 20
    meanDiskStellarMass[count] /= 20;   meaDiskgasMass[count] /= 20
    meanSpheroidStellarMass[count] /= 20; meanSpheroidGassMass[count] /= 20
    print diskOutFlowExp, meanNodeMass[count], count
    count += 1

# for i in   range(0,len(meanNodeMass)):    
#     print "nodeMass:%g  DiskStellarMass:%g DiskgasMass:%g  SpheroidStellarMass:%g SpheroidGassMass:%g " % ( meanNodeMass[i], meanDiskStellarMass[i]
#                                                                                                             , meaDiskgasMass[i], meanSpheroidStellarMass[i]
#                                                                                                             , meanSpheroidGassMass[i] )

