CFLAGS=-g -Wall $(shell pkg-config --cflags glib-2.0)
TARGETS=hilo-prodcons-condvar
LDLIBS=-lpthread $(shell pkg-config --libs glib-2.0)

all: $(TARGETS) 

hilo-prodcons-condvar: hilo-prodcons.o lista-condvar.o
	$(CC) -o $(@) $^ $(LDLIBS)

clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
