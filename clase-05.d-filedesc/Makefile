CFLAGS=-Wall -g
TARGET=test-filedes

all: $(TARGET)

test-filedes: test-filedes.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)


clean:  
	rm -f *.o *~ $(TARGET) core archivo*txt

-include ../doxygen/Makefile.dox
-include ../splint/Makefile
SPFLAGS+=-nullassign
