#filein="Clues2710-MWMassRange-Total.dat"
filein="Clues16953-MWMassRange-Total.dat"

# selection in stellar mass
awk < $filein '{if(($3+$6) <7.1E10 && ($3+$6)>5.8E10) print $0}' > tmp.dat

# selection in gasesous mass
awk < tmp.dat '{if(($4+$7) <8.0E9&& ($3+$6)>6.0E9) print $0}' > tmp2.dat

# selection in morphology
awk < tmp2.dat '{if($3 > $6) print $0}' > tmp3.dat