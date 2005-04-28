/*
 * $Id: holamundo_stdio.c,v 1.4 2005/04/28 13:43:27 jjo Exp $
 */

#include <stdio.h>

/*
 * El entorno UN*X _hace_ uso del valor de retorno del main,
 * tipicamente con contrucciones de shell que condicionan la 
 * logica en base al "exit status" del comando (0: Ok).
 *
 * De alli que "main" devuelva un entero.
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
