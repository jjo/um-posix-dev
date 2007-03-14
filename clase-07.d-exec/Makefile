CFLAGS=-Wall -g
TARGET=test-fork-n-exec test-fork-n-exec-fd

all: $(TARGET)

test-fork-n-exec: test-fork-n-exec.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)

test-fork-n-exec-fd: test-fork-n-exec-fd.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)


clean:  
	rm -f *.o *~ $(TARGET) core archivo*txt

-include ../doxygen/Makefile.dox
-include ../splint/Makefile
SPFLAGS+=-nullassign
