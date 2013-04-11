CC=gcc

PREFIX = /usr

CFLAGS = -Wall -I.  -g  -I$(PREFIX)  -c
LFLAGS = -L$(PREFIX)/lib64 -lgsl -lgslcblas

MODULES = SysTools.o SimDataMan.o MergerTreeGalacticus9.0.o

all: clean $(MODULES) 


%.o:%.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *~ *.o *.out
	clear

