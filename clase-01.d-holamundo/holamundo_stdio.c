/*
 * $Id: holamundo_stdio.c,v 1.3 2004/03/12 17:42:25 jjo Exp $
 */

#include <stdio.h>

/*
 * El entorno UN*X _hace_ uso del valor de retorno del main,
 * t�picamente con contrucciones de shell que condicionan la 
 * l�gica en base al "exit status" del comando (0: Ok).
 *
 * De all� que "main" devuelva un entero.
 */
#define HOLAMUNDO "Hola mundo\n"
#define ERRHOLA "ERROR: el mundo no responde\n"

/* 
 * 	Usamos la biblioteca standard de I/O (stdio)
 * 
 * 	Mas info: "man 3 printf"
 */
int main(void)
{
	printf(HOLAMUNDO);
	fprintf(stderr, ERRHOLA);
	return 0;
}
