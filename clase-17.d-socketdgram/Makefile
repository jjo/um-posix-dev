CFLAGS=-g -Wall $(COPTS)
TARGETS=socket_dgram

all: $(TARGETS) 

socket_dgram: socket_dgram.o $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
