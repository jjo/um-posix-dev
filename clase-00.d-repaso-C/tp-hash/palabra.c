#include <glib.h>
#include "palabra.h"
#include <string.h>
#define HASH_SIZE 256
int palabra_findfunc(gconstpointer data, gconstpointer u_data);
void palabra_print(gpointer data, gpointer u_data);

static GList *TheMatrix[256];
static int hash(const char *str) {
	return str[0];
}
void palabra_add(const char *string) {
	int i;
	i=hash(string);
	TheMatrix[i]=g_list_append(TheMatrix[i], palabra_new(string));
}
struct palabra *palabra_new(const char *string) {
	struct palabra *p;
	p=palabra_find(string);
	if (!p) {
		p=g_malloc0(sizeof (*p));
		p->string=strdup(string);
	}
	p->cnt++;
	return p;
}
int palabra_findfunc(gconstpointer data, gconstpointer u_data);
struct palabra * palabra_find(const char *str) {
	struct palabra p={ .string= str};
	GList *elem;
	int i=hash(str);
	elem=g_list_find_custom(TheMatrix[i], (gconstpointer)&p, palabra_findfunc);
	if (elem)
		return elem->data;
	return NULL;
}
int palabra_findfunc(gconstpointer data, gconstpointer u_data) {
	return strcmp(((struct palabra *)data)->string, ((struct palabra *)u_data)->string);
}
void palabra_print(gpointer data, gpointer u_data) {
	printf("%s ", data);
}
