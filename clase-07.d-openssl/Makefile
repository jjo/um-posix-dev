CFLAGS=-Wall -g
LDLIBS=-lcrypto
TARGET=openssl-enc-test

all: $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)


clean:  
	rm -f *.o *~ $(TARGET) core archivo*txt
