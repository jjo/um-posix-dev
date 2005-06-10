CFLAGS=-g -Wall
TARGETS=test-mem test-debug 

all: $(TARGETS)

memcheck: test-debug
	valgrind --tool=memcheck ./test-debug

clean:  
	rm -f *.o *~ $(TARGETS) core
