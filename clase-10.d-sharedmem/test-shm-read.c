#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>

#define BUFFERSIZE   4096
int main(void)
{
	int shm_id;
	key_t key;
	char *ptr=NULL;
	key=ftok(".", 0);
	if (key<0) {
		perror("ftok()");
		return 1;
	}
	shm_id=shmget(key, BUFFERSIZE, IPC_CREAT|0666);
	printf("shm_id=%d\n", shm_id);
	if (shm_id < 0) {
		perror("shmget()");
		return 2;
	}
	ptr=shmat(shm_id, NULL, 0);
	if (!ptr) {
		perror("shmat()");
		return 3;
	}
	printf("%s\n", ptr);
	/* system("ipcs -m"); */

	return 0;
}

