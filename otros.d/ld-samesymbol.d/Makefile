CFLAGS=-Wall -g
LDLIBS=-lhidden
LDFLAGS=-L.

#
# Seria mas simple hacer un objcopy  --redefine-sym ...
main: main.o hid1obj.o hid2obj.o
	$(CC) -o $@ $^ #$(LDFLAGS) $(LDLIBS)

hid1obj.o: hid1.o
	objcopy $? $@ -L misma_func

hid2obj.o: hid2.o
	objcopy $? $@ -L misma_func
clean:
	rm -f *.o *~
