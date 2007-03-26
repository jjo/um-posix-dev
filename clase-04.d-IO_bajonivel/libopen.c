#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>	/* open() */
#include <sys/stat.h>	/* umask() */
#include "libopen.h"

/**
 * Funcion de entrada para invocar varios "sabores" de open()
 *
 * @param num numero de funcion open a invocar
 * @param  fname nombre del archivo a abrir
 * @return el retorno de open(...)
 */
int open_test(int num, const char *fname)
{
	int fd;
	switch(num) {
		case 1: fprintf(stderr, "-> open(\"%s\", 0, 0)\n", fname);
			fd=open (fname, 0, 0);
			break;

		case 2: fprintf(stderr, "-> open(\"%s\", O_CREAT|O_RDWR, 0)\n", fname);
			fd=open (fname, O_CREAT|O_RDWR, 0);
			break;

		case 3: fprintf(stderr, "-> open(\"%s\", O_CREAT|O_RDWR, 0666)\n", fname);
			fd=open (fname, O_CREAT|O_RDWR, 0666);
			break;

		case 4: fprintf(stderr, "-> umask(0000);open(\"%s\", O_CREAT|O_RDWR, 0666)\n", fname);
			umask(0000);
			fd=open (fname, O_CREAT|O_RDWR, 0666);
			break;

		default: return -1;
	}
	if (fd>=0) {
		char buf[64]; int len;
		buf[sizeof(buf)-1]=0;
		len=snprintf(buf, sizeof (buf)-1, "Hola (num=%d)\n", num);
		write(fd, buf, len);
	}
	return fd;
}

