#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void) 
{
	signal(SIGINT, SIG_IGN);
	getchar();
	return 0;
}
