#include <stdio.h>
#include <unistd.h>
#include "servicio.h"

int servicio(int fd)
{
	sleep(4);
	return write(fd, "Hola fork\n", 10);
}
