#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>

struct msg {
	int sender_id;
	int op;
	int val;
};
#define USE_LOCKF 0
static int sem_down(int semid)
{
	struct sembuf op;
	op.sem_num =  0;         // member number (we only have number 0)
	op.sem_op  = -1;         // dec by one
	op.sem_flg =  0;         // no flags

	if( semop(semid, &op, 1) == -1)
		return -1;
	return 0;
}
static int sem_up(int semid)
{
	struct sembuf op;
	op.sem_num =  0;         // member number (we only have number 0)
	op.sem_op  =  1;         // up by one
	op.sem_flg =  0;         // no flags

	if( semop(semid, &op, 1) == -1)
		return -1;
	return 0;
}
static int data_get(int fd) {
	int data;
	lseek(fd, 0, SEEK_SET);
	read(fd, &data, sizeof(data));
	return data;
}
static int data_set(int fd, int data) {
	lseek(fd, 0, SEEK_SET);
	write(fd, &data, sizeof(data));
	return data;
}

static int data_lock_n_add(int fd, int n, int mutexid) {
	int data;
#if USE_LOCKF
	lseek(fd, 0, SEEK_SET);
	lockf(fd, F_LOCK, sizeof(data));	
#else
	sem_down(mutexid);
#endif
	data=data_get(fd)+n;
	data_set(fd, data);
#if USE_LOCKF
	lseek(fd, 0, SEEK_SET);
	lockf(fd, F_ULOCK, sizeof(data));
#else
	sem_up(mutexid);
#endif
}

static int use_cpu(int p) {
	while (p--) sqrt((double)p);
}
#define TOTAL_ITER 10000000
#define CHILD_ITER 100
#define CPU_ITER TOTAL_ITER/CHILD_ITER
static int hijo(int fd, int send_fd, int mutexid) {
	int i;
	struct msg msg_hijo;
	char *ptr;
	//ptr=mmap(NULL,  sizeof(int), PROT_WRITE ,  MAP_SHARED, fd, 0);
	printf("ptr=%p\n", ptr);
	msg_hijo.sender_id=getpid();
	msg_hijo.op=0;
	msg_hijo.val=1;
	i=CHILD_ITER;  
	while(i--) {
		use_cpu(CPU_ITER);
#if USE_SOCKETPAIR
		write(send_fd, &msg_hijo, sizeof(msg_hijo));
#else
		data_lock_n_add(fd, 1, mutexid);
#endif
	}
	return 0;
}
int main(int argc, const char *argv[]) {
	const char *fname;
	struct msg msg;
	int mutexid;
	int fds[2];
	int fd, i;
	if (argc<2) {
		fprintf(stderr, "uso: %s arch.bin\n", argv[0]);
		return 255;
	}
	fname=argv[1];
	fd=open(fname, O_RDWR|O_CREAT, 0644);
	if (fd < 0) {
		perror(fname);
		return 1;
	}
#if USE_SOCKETPAIR
	socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
#endif
	mutexid = semget(IPC_PRIVATE, 1, 0600);
	if (mutexid<0) {
		perror("semget()");
		return 2;
	}
	sem_up(mutexid);
	data_set(fd, 0);
	for (i=0;i<2;i++) {
		if (fork()==0) { 
#if USE_SOCKETPAIR
			close(fds[0]);
#endif
			return hijo (fd,fds[1],mutexid); 
		}
	}
#if USE_SOCKETPAIR
	close (fds[1]);
	while (read(fds[0], &msg, sizeof(msg))>0) {
		data_lock_n_add(fd, msg.val);
	}
#endif
	wait(NULL);
	wait(NULL);
	printf("data=%d\n", data_get(fd));
 	return 0;
}
