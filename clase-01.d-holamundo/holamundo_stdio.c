/*
 * $Id: holamundo_stdio.c,v 1.1 2003/03/21 02:19:45 jjo Exp $
 */

#include <stdio.h>

/*
 * El entorno UN*X _hace_ uso del valor de retorno del main,
 * típicamente con contrucciones de shell que condicionan la 
 * lógica en base al "exit status" del comando (0: Ok).
 *
 * De allí que "main" devuelva un entero.
 */
int main(void)
{
	printf("Hola mundo\n");
	fprintf(stderr, "la guerra ES el standard error :/ \n");
	return 0;
}
