CFLAGS=-g
TARGETS=test-mem

all: $(TARGETS)

test-mem: test-mem.o
	$(CC) -o $@ $^

clean:  
	rm -f *.o *~ $(TARGETS) core
-include ../doxygen/Makefile.dox
-include ../splint/Makefile
SPFLAGS+=-nullassign
