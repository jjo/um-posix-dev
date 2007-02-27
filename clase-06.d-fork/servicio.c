/*
 * $Id: servicio.c,v 1.4 2006/04/24 20:49:09 jjo Exp $
 */
#include <stdio.h>
#include <unistd.h>
#include "servicio.h"

int servicio_sec(int fd, unsigned sec)
{
	sleep(sec);
	printf("(HIJO despierto pid=%d) ", getpid());
	return write(fd, "Hola fork\n", 10);
}
