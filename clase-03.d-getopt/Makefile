CFLAGS=-Wall -g
TARGETS=opcion

all: $(TARGETS)

opcion: opcion.o
	$(CC) -o $@ $^

clean:  
	rm -f *.o *~ $(TARGETS) core

-include ../doxygen/Makefile.dox
-include ../splint/Makefile
SPFLAGS+=-nullassign
