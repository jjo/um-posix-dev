CFLAGS=-g -Wall
TARGET=test-fork-offset

all: $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) -o $@ $^

clean:  
	rm -f *.o *~ $(TARGET) core
