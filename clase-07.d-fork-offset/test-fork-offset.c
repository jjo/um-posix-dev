#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define ARCH "salida.txt"
int main(void) 
{
	int status;
	int fd=open(ARCH, O_RDWR|O_CREAT, 0666);
	if (fd<0) {
		perror(ARCH);
		return 1;
	}
	if (fork()==0) {
		/* hijo */
		if (lseek(fd, 1024, SEEK_SET)<0) {
			perror("lseek");
			return 1;
		}
		return 0;
	}
	wait(&status);
	write(fd, "Donde-stoy?", 4);
	return 0;
}
