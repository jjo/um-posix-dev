#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void) 
{
	signal(SIGINT, SIG_IGN); /* deberiamos usar sigaction() */
	getchar();
	return 0;
}
