CFLAGS=-g -Wall $(COPTS)
TARGETS=tcp_cli_select tcp_cli_select_hname

all: $(TARGETS) 

do_select: do_select.o $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
clean:  
	rm -f *.o *~ $(TARGETS) $(LIBS) core
