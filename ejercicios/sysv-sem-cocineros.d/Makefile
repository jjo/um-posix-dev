CFLAGS=-g -Wall $(COPTS)
TARGET=mi_cheff

all: $(TARGET) 

$(TARGET): mi_cheff.o cocinero.o proveedor.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
clean:  
	rm -f *.o *~ $(TARGET) $(LIBS) core
CFLAGS=-g -Wall
