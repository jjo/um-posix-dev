#include <sys/types.h>
struct elem {
	struct elem *next;
	void *data;
};
struct lista {
	struct elem *elem_base;
	int count;
};
struct elem *elem_new(size_t size);
void elem_destroy(struct elem *e);
struct lista *lista_new(void);
void lista_append_elem(struct lista *, struct elem *);
struct elem *lista_del_elem(struct lista *, struct elem *);
#define LISTA_FOR_EACH(l,e) \
	for(e=l->elem_base;e;e=e->next)
