CFLAGS=-Wall -g
TARGET=test-open

all: $(TARGET)

test-open: test-open.o libopen.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)

test: test1 test2 test3 test4 
test1: test-open
	@echo -e "\n*** $(@) ***"
	-./test-open 1 archivo1.txt

test2: test-open
	@echo -e "\n*** $(@) ***"
	-./test-open 2 archivo2.txt

test3: test-open
	@echo -e "\n*** $(@) ***"
	-./test-open 3 archivo3.txt

test4: test-open
	@echo -e "\n*** $(@) ***"
	-./test-open 4 archivo4.txt


clean:  
	rm -f *.o *~ $(TARGET) core archivo*txt

-include ../doxygen/Makefile.dox
-include ../splint/Makefile
SPFLAGS+=-nullassign
