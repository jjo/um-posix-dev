CFLAGS=-g -Wall $(COPTS)
#TARGETS=test-shmsem-server test-shmsem-client
TARGETS=hilos-0 hilos-1 hilos-2-sem hilos-2-mutex
LDLIBS=-lpthread

all: $(TARGETS) 

clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
