CFLAGS=-g -Wall
TARGETS=webito webito-mt

all: $(TARGETS)

webito: webito.o http.o file.o
	$(CC) -o $@ $^

webito-mt: webito-mt.o http.o file.o
	$(CC) -o $@ $^ -lpthread

clean: 
	rm -f *.o *~ $(TARGETS) core

