/* 
 * Author: JuanJo Ciarlante <jjo-noSPAM@mendoza.gov.ar>
 * License: GPLv2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista1.h"

static struct elem *elem_new_string(char *str);
static char *palabras[] = {
	"Juan", "Jo", "2004 (C)", "--copyleft", NULL
};

int main(void)
{
	struct lista *mi_lista;
	struct elem *e;
	char **strp;
	mi_lista=lista_new();
	for(strp=palabras;*strp;strp++) {
		e=elem_new_string(*strp);
		lista_append_elem(mi_lista, e);
	}
	LISTA_FOR_EACH(mi_lista, e) {
		puts(e->data);
	}
	return 0;
}	
static struct elem *elem_new_string(char *str)
{
	struct elem *e=elem_new(strlen(str)+1);
	strcpy(e->data, str);
	return e;
}
