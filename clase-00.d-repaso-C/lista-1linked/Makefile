CC=gcc
CFLAGS=-g -Wall
TARGET=l1
OBJS=lista1.o main.o

all : $(TARGET)

$(TARGET):  $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean: 
	rm -f $(TARGET) *.o core*
