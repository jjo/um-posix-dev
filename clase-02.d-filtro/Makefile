# $Id: Makefile,v 1.3 2003/03/28 18:28:45 jjo Exp $
CFLAGS=-g -Wall
TARGETS=filtro filtro_stdio

all: $(TARGETS)

# Este ejecutable (filtro) depende de estos objtos (.o)
# luego de compilados, se invoca el compilador (en realidad el linker)
# para terminar de construir el ejecutable
#
# $(@) se reemplaza por el "target", $(^) por todas las dependencias
filtro: filtro.o convertir.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)

filtro_stdio: filtro_stdio.o convertir.o
	$(CC) $(LDFLAGS) -o $(@) $(^) $(LDLIBS)

# simple test
#
# $(?) se reemplaza por la 1era dependencia
test: filtro
	@echo "*** TEST BEGIN ***"
	ls -al | ./$(?) 
	@echo "*** TEST END ***"

clean: 
	rm -f *.o *~ $(TARGETS)
