/* $Id: convertir.c,v 1.2 2003/03/21 02:35:34 jjo Exp $ */
#include <ctype.h>
#include "convertir.h"

/* simple conversor a mayúsculars */
void convertir (char *buf, unsigned cant)
{
	while(cant--)
		*buf++=toupper(*buf);
}
