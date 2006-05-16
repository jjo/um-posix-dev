/* 
 * $Id: clase-09.d-signal/test-signal-01-catch.c git 2006-05-16 00:14:13.000000000 -0300 jjo Exp $
 */

/* 
#define _POSIX_SOURCE 
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handler(int signum) {
	fprintf(stderr, "OUch sig=%d\n(poniendo a DFL)\n", signum);
	signal (signum, SIG_DFL); /* deberiamos usar sigaction() */
}
int main(void) 
{
	signal(SIGINT, handler); /* deberiamos usar sigaction() */
	getchar();
	return 0;
}
