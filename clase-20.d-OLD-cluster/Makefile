CFLAGS=-g -Wall $(COPTS)
TARGETS=cluster-0-pi cluster-1-pi

all: $(TARGETS) 

cluster-1-pi: cluster-1-pi.o pi_lib.o

clean:  
	rm -f *.o *~ $(TARGETS) $(TARGETS_PTH) $(LIBS) core
