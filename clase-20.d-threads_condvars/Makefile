CFLAGS=-g -Wall $(COPTS)
TARGETS=hilo-prodcons-sem simple-trigger-condvar
LDLIBS=-lpthread

all: $(TARGETS) 

hilo-prodcons-sem: hilo-prodcons-sem.o lista.o
	$(CC) -o $(@) $^ $(LDLIBS)

hilo-sync-sem: hilo-main.o libjj_sem.o
	$(CC) -o $(@) $^ $(LDLIBS)

hilo-sync-rwlock: hilo-main.o libjj_rwlock.o
	$(CC) -o $(@) $^ $(LDLIBS)

clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
