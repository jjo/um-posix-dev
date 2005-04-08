/* $Id: convertir.c,v 1.3 2005/04/08 20:59:01 jjo Exp $ */
#include <ctype.h>
#include "convertir.h"

/* simple conversor a mayúsculars */
void convertir (char *buf, unsigned cant)
{
	for(;cant--;buf++) 
		*buf=toupper(*buf);
}
