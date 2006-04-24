/*
 * $Id: test-fork2.c,v 1.4 2006/04/24 20:49:09 jjo Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

#include "servicio.h"
int main(int argc, char *const argv[])
{
	int fd;

	fd=open("salida.txt", O_CREAT|O_TRUNC|O_RDWR, 0666);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	/* el valor de retorno depende del si es padre o hijo */
	switch( (fork()) ) {
		case -1:	/* error */
			perror("fork()");
			return 1;
		case 0:		/* hijo */
			servicio_sec(fd, 1);
			break;
	}
	puts("Saliendo...\n");
	return 0;
}
