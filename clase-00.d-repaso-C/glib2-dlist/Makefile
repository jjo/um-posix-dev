CFLAGS=$(shell pkg-config glib-2.0 --cflags)
LDFLAGS=$(shell pkg-config glib-2.0 --libs)
CC=gcc
TARGET=l1
OBJS=main.o

all : $(TARGET)

$(TARGET):  $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean: 
	rm -f $(TARGET) *.o core*
