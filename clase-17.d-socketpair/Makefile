CFLAGS=-g -Wall $(COPTS)
TARGETS=socketpair

all: $(TARGETS) 

socketpair: socketpair.o $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
