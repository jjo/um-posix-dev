CFLAGS=-Wall -g
TARGET=test-fork1 test-fork2

all: $(TARGET)

test-fork1: test-fork1.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)

test-fork2: test-fork2.o servicio.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)


clean:  
	rm -f *.o *~ $(TARGET) core 
