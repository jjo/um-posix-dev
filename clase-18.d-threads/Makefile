CFLAGS=-g -Wall $(COPTS)
#TARGETS=test-shmsem-server test-shmsem-client
TARGETS=hilos-1
LDLIBS=-lpthread

all: $(TARGETS) 

$(TARGETS): $(TARGETS).o $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
