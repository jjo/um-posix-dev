CFLAGS=-g -Wall $(COPTS)
TARGETS=tcp-server

all: $(TARGETS) 

tcp-server: tcp-server.o $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
