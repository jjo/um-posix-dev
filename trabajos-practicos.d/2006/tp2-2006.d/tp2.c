#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "libcifrado.h"

//intenta_descifrado()
int main(void)
{
	char clave[16];
	char buf[1024];
	libcifrado_t *ptr;
	int n;
	int fd, fd_w;
	FILE *f;
	char word[256];
	fd=open("cifrado.dat", O_RDONLY, 0);
	if (fd<0) { perror("cifrado.dat"); return 1;}
	fd_w=open("/usr/share/dict/words", O_RDONLY, 0);
	if (fd_w<0) { perror("words"); return 1;}
	n=read(fd, buf, sizeof buf);
	f=fdopen(fd_w, "r");

	int len;
	ptr=libcifrado_new();
	while(fgets(word, sizeof word,f)!=NULL) {
		memset(clave, 0 , sizeof clave);
		word[strlen(word)-1]=0;
		strncpy(clave, word, sizeof clave);
		//printf("clave=%s\n", clave);
		libcifrado_set_clave(ptr, clave, 0);
		if (!(len=libcifrado_bloque(ptr, buf, n))) continue;
		if (strstr(buf, "Ghandhi")) {
			write(STDOUT_FILENO, buf, len);
			printf("clave=%s\n", word);
		}
	}
	return 0;
}
