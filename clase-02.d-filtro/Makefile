# $Id: Makefile,v 1.2 2003/03/21 02:35:34 jjo Exp $
CFLAGS=-g -Wall
TARGETS=filtro

all: $(TARGETS)

# Este ejecutable (filtro) depende de estos objtos (.o)
# luego de compilados, se invoca el compilador (en realidad el linker)
# para terminar de construir el ejecutable
#
# $(@) se reemplaza por el "target", $(^) por todas las dependencias
filtro: filtro.o convertir.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)

# simple test
#
# $(?) se reemplaza por la 1era dependencia
test: filtro
	@echo "*** TEST BEGIN ***"
	./$(?) < /etc/hosts
	@echo "*** TEST END ***"

clean: 
	rm -f *.o *~ $(TARGETS)
