/* 
 * $Id: test-signal-02-ign.c,v 1.4 2003/05/16 21:13:50 jjo Exp $
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void) 
{
	signal(SIGINT, SIG_IGN); /* deberiamos usar sigaction() */
	getchar();
	return 0;
}
