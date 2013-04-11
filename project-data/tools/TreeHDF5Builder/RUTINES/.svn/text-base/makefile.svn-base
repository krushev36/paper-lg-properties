CC=gcc

PREFIX = /usr

CFLAGS = -Wall -I.  -g  -I$(PREFIX)  -c
LFLAGS = -L$(PREFIX)/lib64 -lgsl -lgslcblas

MODULES = rutinesv1.o SimPR.o gnuplot_i.o SysTools.o SimDataMan.o

all: clean $(MODULES) 


%.o:%.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *~ *.o *.out
	clear

