CFLAGS=$(shell pkg-config glib-2.0 --cflags)
LDFLAGS=$(shell pkg-config glib-2.0 --libs)
CC=gcc
TARGET=tp-hash
OBJS=main.o palabra.o

all : $(TARGET)

$(TARGET):  $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean: 
	rm -f $(TARGET) *.o core*
