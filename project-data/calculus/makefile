CC = gcc

PREFIX = /usr/
GSL_PATH = /home/krushev/.local/
MY_PATH = /media/E/proyectos-v2/project-galacticus9.0-v1/calculus-v2/rutines

CFLAGS = -Wall  -g  -I$(PREFIX) -I$(MY_PATH) -I$(GSL_PATH)/include -I. -c 
LFLAGS = -L$(PREFIX)/lib -L$(GSL_PATH)/lib/  -lgsl   -lgslcblas -lm

PROGRAMS = LG-params-medians-v2.out
#statistics-mw-type.out statistics-m31-type.out
#statistics-.out statistics-gas-ref.out statistics-gas.out
#
# LG-params-medians.out
#statistics.out statistics-m31-type.out statistics-gas.out statistics-HR.out statistics-mw-type.out statistics-tully.out statistics-bulge.out
#statistics-m31-type.out   statistics-mw-type.out
RUTINES =     $(MY_PATH)/SimDataMan.o  $(MY_PATH)/SimPR.o  $(MY_PATH)/SysTools.o
#$(MY_PATH)/rutinesv1.o
#statistics.out  statistics-tully.out statistics-HR.out  statistics-gas.out  statistics-bulge.out  statistics-vel.out statistics-mw-type.out

all:clean
	make $(PROGRAMS)

%.out: %.o
	$(CC)  $^ $(RUTINES)  $(LFLAGS) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf  *~ *.out *.o  *.inf
	clear

