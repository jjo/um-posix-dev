/*
 * $Id: servicio.c,v 1.3 2003/05/02 20:34:14 jjo Exp $
 */
#include <stdio.h>
#include <unistd.h>
#include "servicio.h"

int servicio_seg(int fd, int sec)
{
	sleep(sec);
	printf("(HIJO despierto pid=%d) ", getpid());
	return write(fd, "Hola fork\n", 10);
}
int servicio(int fd)
{
	return servicio_seg(fd, 4);
}
