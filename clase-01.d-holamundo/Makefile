# $Id: Makefile,v 1.2 2003/03/24 13:21:50 jjo Exp $
#
# Indicarle al compilador que agrege s�mbolos 
# para debugging (-g) y todos los warning (-Wall)
CFLAGS=-g -Wall

TARGETS=holamundo holamundo_stdio

# La 1era regla es la que se intenta "construir" por default
# (al tipear "make" a secas)
#
# make construir� $(TARGETS) usando las reglas impl�citas; en este
# caso al encontrar <archivo>.c crear� un ejecutable invocando el 
# compilador (el cual a su vez llama al linker).
all: $(TARGETS)

clean:
	@echo Limpiando ...
	rm -fv $(TARGETS) *.o core

-include ../doxygen/Makefile.dox
-include ../splint/Makefile
