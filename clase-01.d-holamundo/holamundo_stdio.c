/*
 * $Id: holamundo_stdio.c,v 1.1 2003/03/21 02:19:45 jjo Exp $
 */

#include <stdio.h>

/*
 * El entorno UN*X _hace_ uso del valor de retorno del main,
 * t�picamente con contrucciones de shell que condicionan la 
 * l�gica en base al "exit status" del comando (0: Ok).
 *
 * De all� que "main" devuelva un entero.
 */
int main(void)
{
	printf("Hola mundo\n");
	fprintf(stderr, "la guerra ES el standard error :/ \n");
	return 0;
}
