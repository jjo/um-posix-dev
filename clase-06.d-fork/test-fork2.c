/*
 * $Id: test-fork2.c,v 1.3 2004/08/20 21:22:13 jjo Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

#include "servicio.h"
int main(int argc, char *const argv[])
{
	int ret;
	int fd;

	fd=open("salida.txt", O_CREAT|O_TRUNC|O_RDWR, 0666);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	/* el valor de retorno depende del si es padre o hijo */
	switch( (ret=fork()) ) {
		case -1:	/* error */
			perror("fork()");
			return 1;
		case 0:		/* hijo */
			servicio(fd);
			break;
	}
	printf("Saliendo...");
	return 0;
}
