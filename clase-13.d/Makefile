CFLAGS=-g -Wall $(COPTS)
#TARGETS=test-shmsem-server test-shmsem-client
TARGETS=test-pthread
LDLIBS=-lpthread

all: $(TARGETS) 

test-shmsem-var: test-shmsem-var.o $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
