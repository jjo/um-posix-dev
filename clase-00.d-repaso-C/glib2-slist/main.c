/* 
 * Author: JuanJo Ciarlante <jjo-noSPAM@mendoza.gov.ar>
 * License: GPLv2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

static void lista_print(GSList *l, FILE *f);
static void print_elem(gpointer p, gpointer u_data);
static char *palabras[] = {
	"Juan", "Jo", "2004 (C)", "--copyleft", NULL
};

int main(void)
{
	GSList *mi_lista = NULL;
	struct gpointer *e;
	char **strp;
	/* creo una nueva lista */
	/* creo un elemento por palabra y lo appendo a lista */
	for(strp=palabras;*strp;strp++) {
		mi_lista=g_slist_append(mi_lista, *strp);
	}
	lista_print(mi_lista, stdout);
	e=g_slist_nth_data(mi_lista, 2);
	g_slist_remove(mi_lista, e);
	lista_print(mi_lista, stdout);
	return 0;
}	
static void lista_print(GSList *l, FILE *f) {
	struct elem *e;
	g_slist_foreach(l, print_elem, f);
	fputc('\n', f);
}
static void print_elem(gpointer p, gpointer u_data) {
	FILE *f=u_data;
	fprintf(f, "\"%s\" ", p);
}
