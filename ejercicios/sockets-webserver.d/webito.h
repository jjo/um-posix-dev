enum http_method {
	HTTP_GET = 1,
	HTTP_HEAD
};
struct http_parser {
	int linenum;
	enum http_method method;
	char *uri;
};
/* http.c */
void http_parser_init(struct http_parser *ht);
int http_parser_process_line(struct http_parser *ht, const char *line);
