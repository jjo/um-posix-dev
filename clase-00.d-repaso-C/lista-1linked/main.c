/* 
 * Author: JuanJo Ciarlante <jjo-noSPAM@mendoza.gov.ar>
 * License: GPLv2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista1.h"

static void lista_print(struct lista *l, FILE *f);
static struct elem *elem_new_string(char *str);
static char *palabras[] = {
	"Juan", "Jo", "2004 (C)", "--copyleft", NULL
};

int main(void)
{
	struct lista *mi_lista;
	struct elem *e;
	struct elem *e2;
	int i=0;
	char **strp;
	/* creo una nueva lista */
	mi_lista=lista_new();
	/* creo un elemento por palabra y lo appendo a lista */
	for(strp=palabras;*strp;strp++) {
		e=elem_new_string(*strp);
		if (i++==2) 	/* apunto al 3er (indice 2) elemento para luego borrarlo */
			e2=e; 
		lista_append_elem(mi_lista, e);
	}
	lista_print(mi_lista, stdout);
	lista_del_elem(mi_lista, e2);
	lista_print(mi_lista, stdout);
	return 0;
}	
static void lista_print(struct lista *l, FILE *f) {
	struct elem *e;
	/* para cada elemento de la lista, muestro e->data */
	LISTA_FOR_EACH(l, e) {
		fprintf(f, "\"%s\" ", (char*)e->data);
	}
	fputc('\n', f);
}
static struct elem *elem_new_string(char *str)
{
	struct elem *e=elem_new(strlen(str)+1);
	strcpy(e->data, str);
	return e;
}
