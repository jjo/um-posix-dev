/*
 * $Id: servicio.c,v 1.2 2003/04/25 21:10:28 jjo Exp $
 */
#include <stdio.h>
#include <unistd.h>
#include "servicio.h"

int servicio(int fd)
{
	sleep(4);
	return write(fd, "Hola fork\n", 10);
}
