#include <ctype.h>
#include "convertir.h"

void convertir (char *buf, int cant)
{
	for(;cant--;buf++) *buf=toupper(*buf);
}
