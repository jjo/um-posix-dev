CFLAGS=-g -Wall
LDLIBS=-lefence

all: ef

clean:
	rm -f *.o ef

debug: ef
	export EF_PROTECT_BELOW=0 EF_ALIGNMENT=0; gdb ef
