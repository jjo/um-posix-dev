CFLAGS=-g -Wall $(COPTS)
TARGETS=hilos-3-condvars hilos-4-socketcli hilos-5-socketcli-pth
LDLIBS=-lpthread -lpth

all: $(TARGETS) 

clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
