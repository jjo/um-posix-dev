/* 
 * Author: JuanJo Ciarlante <jjo-noSPAM@mendoza.gov.ar>
 * License: GPLv2
 */
#include <stdlib.h>
#include <string.h>
#include "lista1.h"

/* 
 * confiamos en que los punteros quedaran en NULL al llenar de
 * "bits" en cero la struct
 */
 /*
           /   +---------------------+
          |    |  struct elem* next  |
          |    +---------------------|
malloc() <     +  void*        data  |----.
          |    +---------------------+ <--'    _
          |    |                     |         ^
	  |    |                     |         | size
          :    :                     :         v 
          \    +---------------------+         -
  */
struct elem *elem_new(size_t size)
{
	struct elem *e;
	e=malloc(sizeof (*e)+size);
	if (!e) return NULL;
	memset(e, 0, sizeof (*e));
	e->data=(char*)e+sizeof(*e);
	return e;
}
void elem_destroy(struct elem *e) { free(e); }
struct lista *lista_new(void)
{
	struct lista *l;
	l=malloc(sizeof (*l));
	if (!l) return NULL;
	memset(l, 0, sizeof (*l));
	return l;
}
/* agrega un elem al final de la lista */
void lista_append_elem(struct lista *l, struct elem *e)
{
	struct elem *e_tmp;
	e->next=NULL; /* aseguro el fin de lista */
	if (!l->elem_base) {
		l->elem_base=e; return;
	}
	LISTA_FOR_EACH(l, e_tmp) {
		if (!e_tmp->next) {
			e_tmp->next=e;
			l->count++;
			break;
		}
	}
}
/* 
 * delete-a de la lista  el elem. que se le pasa, o el 1ero
 * si e==NULL
 */
struct elem *lista_del_elem(struct lista *l, struct elem *e)
{
	struct elem **epp; /* elem pointerpointer */
	if (!e) e=l->elem_base; /* si e==NULL, extrae el 1ero */

	for(epp=&l->elem_base;*epp;epp=&(*epp)->next) {
		if (*epp==e) { 
			*epp=e->next;
			l->count--;
			return e;
		}
	}
	return NULL;
}
