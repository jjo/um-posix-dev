/*
 * $Id: holamundo.c,v 1.1 2003/03/24 13:21:50 jjo Exp $
 */

#include <unistd.h>
#include <string.h>
/*
 * El entorno UN*X _hace_ uso del valor de retorno del main,
 * típicamente con contrucciones de shell que condicionan la 
 * lógica en base al "exit status" del comando (0: Ok).
 *
 * De allí que "main" devuelva un entero.
 */
#define HOLAMUNDO "Hola mundo\n"
#define WARISSHIT "la guerra ES el standard error :/ \n"

/* 
 * 	Hacemos llamadas de bajo nivel (syscall al S.O.)
 * 	en vez de usar la biblioteca standard de I/O (stdio)
 * 
 * 	Mas info: "man 2 write" 
 */
int main(void)
{
	write(STDOUT_FILENO, HOLAMUNDO, strlen(HOLAMUNDO)); 
	write(STDERR_FILENO, WARISSHIT, strlen(WARISSHIT));
	return 0;
}
