CFLAGS=-Wall -g
TARGETS=opcion

all: $(TARGETS)

opcion: opcion.o
	$(CC) -o $@ $^

clean:  
	rm -f *.o *~ $(TARGETS) core
