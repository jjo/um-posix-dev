CFLAGS=-g -Wall
OBJETIVOS=cifrador tp2
LDLIBS=-lssl

all : $(OBJETIVOS)

cifrador: cifrador.o libcifrado.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
tp2: tp2.o libcifrado.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
clean : 
	rm -fv $(OBJETIVOS) *.o core*
