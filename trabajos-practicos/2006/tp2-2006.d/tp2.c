#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "libcifrado.h"

//intenta_descifrado()
int main(void)
{
	char clave[16];
	char ch;
	static char buf[1024];
	static 	char buf2[1024];
	libcifrado_t *ptr;
	int n;
	int fd=open("cifrado.dat", O_RDONLY, 0);
	n=read(fd, buf, sizeof buf);

	int len;
	ptr=libcifrado_new();
	for (ch='a'; ch<'z'+1; ch++) {
		memset(clave, 0 , sizeof clave);
		strcpy(clave, "abc123");
		clave[2]=ch;
		len=n;
		libcifrado_set_clave(ptr, clave, 0);
		if (!libcifrado_bloque(ptr, buf2, &len, buf, n)) continue;
		write(STDOUT_FILENO, buf2, len);

		if (!libcifrado_bloque_final(ptr, buf2, &len)) continue;
		write(STDOUT_FILENO, buf2, len);
	}
	return 0;
}
