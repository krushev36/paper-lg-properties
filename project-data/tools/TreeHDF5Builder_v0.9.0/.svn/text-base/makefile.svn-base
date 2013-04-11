CC = gcc

PREFIX = /usr
GSL_PATH = /usr/local
MY_PATH = RUTINES

CFLAGS = -Wall  -g  -I$(PREFIX) -I$(MY_PATH) -I$(GSL_PATH) -I. -c 
LFLAGS = -L$(PREFIX)/lib64 -lgsl -lgslcblas -lhdf5	

PROGRAMS = MergerTreeHDF5.out MergerTreeCleanBad.out
RUTINES = $(MY_PATH)/gnuplot_i.o  $(MY_PATH)/rutinesv1.o  $(MY_PATH)/SimDataMan.o  $(MY_PATH)/SimPR.o  $(MY_PATH)/SysTools.o

all:clean
	make $(PROGRAMS)

%.out: %.o
	$(CC)  $^ $(RUTINES)  $(LFLAGS) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf  *~ *.out *.o  *.inf
	clear

