#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>

void hijo(int *iptr, char *id)
{
	int a;
	int i;
	for (i=0;i<100;i++) {
		a=*iptr;
		a++;
		if (rand()%50==0) {
			usleep(0);
		}
#if 0
		write(2,"\r",1);
		write(2,id,1);
#endif
		*iptr=a;
	}
}
int main(void) 
{
	int *iptr;
	key_t key=0x00240667;
	int shm_id;
	shm_id=shmget(key, 4096, IPC_CREAT|0666);
	if (shm_id ==-1) { perror ("shmget");return 1;}
	iptr=shmat(shm_id, NULL, 0);
	if (iptr ==(int *)-1) { perror ("shmat");return 2;}
	
	*iptr=0;
	srand(time(NULL));
	if (fork()==0) { hijo(iptr, "1"); return 0; }
	if (fork()==0) { hijo(iptr, "2"); return 0; }
	if (fork()==0) { hijo(iptr, "3"); return 0; }
	if (fork()==0) { hijo(iptr, "4"); return 0; }
	while(wait(NULL)>0);
	printf("\ni=%d\n", *iptr);
	return 0;
}
