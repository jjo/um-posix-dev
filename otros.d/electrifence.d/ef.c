#include <stdio.h>
#include <malloc.h>

int main(void) {
	char *ptr=malloc(10);
	printf("ptr=%p\n", ptr);
	ptr[20]='a';
	return 0;
}
