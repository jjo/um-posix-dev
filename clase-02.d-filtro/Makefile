CFLAGS=-Wall
TARGETS=filtro2

all: $(TARGETS)

filtro2: filtro.o convertir.o
	$(CC) -o $@ $^

test: filtro2
	@echo "*** TEST BEGIN ***"
	./$? < /etc/hosts
	@echo "*** TEST END ***"

clean: 
	rm -f *.o *~ $(TARGETS)
