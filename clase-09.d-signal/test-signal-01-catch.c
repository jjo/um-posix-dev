/* 
 * $Id: test-signal-01-catch.c,v 1.5 2004/06/11 21:40:48 jjo Exp $
 */

//#define _POSIX_SOURCE 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handler(int signum) {
	fprintf(stderr, "OUch sig=%d\n", signum);
}
int main(void) 
{
	signal(SIGINT, handler); /* deberiamos usar sigaction() */
	getchar();
	return 0;
}
