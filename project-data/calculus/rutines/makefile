CC=gcc

PREFIX = /usr

CFLAGS = -Wall -I.  -g  -I$(PREFIX)  -I/home/krushev/.local/include  -c
LFLAGS = -L$(PREFIX)/lib64 -L/home/krushev/.local/lib/lgsl -L/home/krushev/.local/lib/lgslcblas


MODULE1 = rutinesv1
MODULE2 = SimPR
MODULE3 = gnuplot_i
MODULE4 = SysTools
MODULE5 = SimDataMan

all:clean  $(MODULE1).o $(MODULE2).o $(MODULE3).o $(MODULE4).o $(MODULE5).o


$(MODULE1).o:$(MODULE2).o
	$(CC) $(CFLAGS) $(MODULE1).c -o $@
$(MODULE2).o:
	$(CC) $(CFLAGS) $(MODULE2).c -o $@
$(MODULE3).o:
	$(CC) $(CFLAGS) $(MODULE3).c -o $@
$(MODULE4).o:
	$(CC) $(CFLAGS) $(MODULE4).c -o $@
$(MODULE5).o:
	$(CC) $(CFLAGS) $(MODULE5).c -o $@



clean:
	rm -rf *~ *.o *.out
	clear

