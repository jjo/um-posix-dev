CFLAGS=-g -Wall
TARGETS=tcp_ip_client tcp_ip_server

all: $(TARGETS)

tcp_ip_client: tcp_ip_client.o 
	$(CC) -o $@ $^

tcp_ip_server: tcp_ip_server.o
	$(CC) -o $@ $^

clean: 
	rm -f *.o *~ $(TARGETS) core

-include ../doxygen/Makefile.dox
