CFLAGS=-g -Wall $(COPTS)
TARGETS=tcp-server tcp-server2

all: $(TARGETS) 

%: %.o $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
