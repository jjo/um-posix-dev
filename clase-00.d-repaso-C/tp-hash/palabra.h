struct palabra {
	int cnt;
	const char *string;
};
void palabra_add(const char *str);
struct palabra *palabra_new(const char *str);
struct palabra *palabra_find(const char *str);
