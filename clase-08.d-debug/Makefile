CFLAGS=-g -Wall
TARGETS=test-mem test-debug test-debug-efence

all: $(TARGETS)

test-debug-efence: test-debug.o
	$(CC) -o $(@) $(^) -lefence

clean:  
	rm -f *.o *~ $(TARGETS) core
