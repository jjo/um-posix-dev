CFLAGS=-g -Wall $(COPTS)
TARGETS=hilo-prodcons-sem hilo-prodcons-condvar
LDLIBS=-lpthread

all: $(TARGETS) 

hilo-prodcons-sem: hilo-prodcons.o lista-sem.o
	$(CC) -o $(@) $^ $(LDLIBS)

hilo-prodcons-condvar: hilo-prodcons.o lista-condvar.o
	$(CC) -o $(@) $^ $(LDLIBS)

clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
