#include "webito.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
void http_parser_init(struct http_parser *ht)
{
	memset(ht, 0, sizeof *ht);
}
int http_parser_process_line(struct http_parser *ht, const char *line)
{
	int urilen;
	if (ht->linenum==0) {
		/* 1era linea */
		if (strncasecmp(line, "GET ", 4)==0) {
			ht->method=HTTP_GET;
			line+=4;
			urilen=strcspn(line, " \n\r");
			ht->uri=malloc(urilen+1);
			memcpy(ht->uri, line, urilen);
			ht->uri[urilen]=0;
			fprintf(stderr, "uri=%s.\n", ht->uri);
		} else {
			fprintf(stderr, "request not supported: \"%s\"\n", line);
		}
	}
	return 0;
}
