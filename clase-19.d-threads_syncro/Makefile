
CFLAGS=-g -Wall $(COPTS)
TARGETS=hilo-sync-mutex hilo-sync-sem hilo-sync-rwlock
LDLIBS=-lpthread

all: $(TARGETS) 

hilo-sync-mutex: hilo-main.o libjj_mutex.o
	$(CC) -o $(@) $^ $(LDLIBS)

hilo-sync-sem: hilo-main.o libjj_sem.o
	$(CC) -o $(@) $^ $(LDLIBS)

hilo-sync-rwlock: hilo-main.o libjj_rwlock.o
	$(CC) -o $(@) $^ $(LDLIBS)

clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
