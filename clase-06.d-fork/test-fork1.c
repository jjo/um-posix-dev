/*
 * $Id: test-fork1.c,v 1.2 2003/04/25 21:10:28 jjo Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *const argv[])
{
	int ret;

	ret=fork();	/* el valor de retorno depende del si es padre o hijo */
	if (ret==0) sleep(1);
	printf("\n\rHola %s ... soy pid=%d, ret=%d\n", ret? "~:)" : "=8)",
			getpid(), ret);
	return 0;
}
