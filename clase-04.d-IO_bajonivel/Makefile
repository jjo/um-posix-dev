CFLAGS=-Wall -g
TARGET=test-open

all: $(TARGET)

test1: test-open
	./test-open 1 archivo1.txt

test2: test-open
	./test-open 2 archivo2.txt

test3: test-open
	./test-open 3 archivo3.txt

test4: test-open
	./test-open 4 archivo4.txt


clean:  
	rm -f *.o *~ $(TARGET) core archivo*txt
