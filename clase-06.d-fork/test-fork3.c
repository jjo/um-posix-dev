/*
 * $Id: test-fork3.c,v 1.2 2004/08/20 21:22:13 jjo Exp $
 *
 * Como test-fork2.c pero mas prolijo: 
 * 	- el hijo no muestra "Saliendo" (return 0 al main)
 * 	- esperamos al que el hijo termine
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

#include "servicio.h"
int main(int argc, char *const argv[])
{
	int ret;
	int fd;
	int status;

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
			servicio_seg(fd, 1); /* 1 segundo */
			return 0;
	}
	/* deberi'amos ver si wait() retorna error (-1) ... */
	ret=wait (&status);

	printf("Saliendo... ret=%d, status=%d\n", ret, status);
	return 0;
}
