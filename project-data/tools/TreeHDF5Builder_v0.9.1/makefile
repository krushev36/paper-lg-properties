CC = gcc

PREFIX = /usr
GSL_PATH = /usr/local
MY_PATH = include

CFLAGS = -Wall  -g  -I$(PREFIX) -I$(MY_PATH) -I$(GSL_PATH) -I. -c 
LFLAGS = -L$(PREFIX)/lib64 -lgsl -lgslcblas -lhdf5	

PROGRAMS = MergerTreeHDF5.9.0.out MergerTreeCleanBad.out
RUTINES =  $(MY_PATH)/SimDataMan.o  $(MY_PATH)/SysTools.o $(MY_PATH)/MergerTreeGalacticus9.0.o

all:clean
	make $(PROGRAMS)

%.out: %.o
	$(CC)  $^ $(RUTINES)  $(LFLAGS) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf  *~ *.out *.o  *.inf
	clear

