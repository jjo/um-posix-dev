CFLAGS=-g -Wall
OBJETIVOS=test-cifrador tp2
LDLIBS=-lssl

all : $(OBJETIVOS)

test-cifrador: test-cifrador.o libcifrado.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
tp2: tp2.o crack.o libcifrado.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
clean : 
	rm -fv $(OBJETIVOS) *.o core*

.PRECIOUS: crackeame.dat
