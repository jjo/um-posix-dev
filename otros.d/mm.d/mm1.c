#include <mm.h>
#include <stdio.h>
#include <signal.h>

int main(void) {
	char *ptr;
	printf ("MM_maxsize()=%d\n", MM_maxsize());
	printf ("=%d\n", MM_create(MM_maxsize(), "/tmp/kkkk"));
	ptr=MM_malloc(200000);
	switch(fork()) {
		case 0:
			strcpy(ptr,"Holaa");
		default:
			wait(NULL);
	}
	printf("=%p %s\n", ptr, ptr);
	/* kill(getpid(), SIGSTOP); */
	MM_destroy();
	return 0;
}
