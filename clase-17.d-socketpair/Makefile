CFLAGS=-g -Wall $(COPTS)
TARGETS=socketcalc
OBJS=main.o calc.o

all: $(TARGETS) 

socketcalc: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
