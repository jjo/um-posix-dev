/*
 * $Id: holamundo.c,v 1.2 2004/03/12 17:42:25 jjo Exp $
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
#define ERRHOLA "ERROR: el mundo no responde\n"

/* 
 * 	Hacemos llamadas de bajo nivel (syscall al S.O.)
 * 	en vez de usar la biblioteca standard de I/O (stdio)
 * 
 * 	Mas info: "man 2 write" 
 */
int main(void)
{
	write(1, HOLAMUNDO, strlen(HOLAMUNDO)); 
	write(2, ERRHOLA, strlen(ERRHOLA));
	return 0;
}
