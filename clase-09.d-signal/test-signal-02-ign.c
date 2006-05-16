/* 
 * $Id: clase-09.d-signal/test-signal-01-catch.c git 2006-05-16 00:14:13.000000000 -0300 jjo Exp $
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void) 
{
	signal(SIGINT, SIG_IGN); /* deberiamos usar sigaction() */
	getchar();
	puts("Saliendo...");
	return 0;
}
