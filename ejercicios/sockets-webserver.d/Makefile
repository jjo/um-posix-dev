CFLAGS=-g -Wall
TARGETS=webito

all: $(TARGETS)

webito: webito.o http.o file.o
	$(CC) -o $@ $^

clean: 
	rm -f *.o *~ $(TARGETS) core

