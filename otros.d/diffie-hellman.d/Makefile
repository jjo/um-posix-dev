LDLIBS=-lssl -lcrypto
CFLAGS=-Wall -g

TARGETS=dh dhparam

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

moreclean: clean
	rm -f *.dat
