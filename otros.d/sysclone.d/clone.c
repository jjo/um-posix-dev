#include <unistd.h> 
#include <sched.h>
#include <stdlib.h>                                               
#include <string.h>                                              
#include <stdio.h>                                              
#include <fcntl.h>                                             
#include <sys/signal.h>

int variable, fd;                                        
int do_something(void *arg) {
	variable = 42;
	close(fd);
	return 0;
}             

int create_1_clonardo( int (*func)(void*))
{
	void **child_stack;
	child_stack = (void **) malloc(4096*20);
	child_stack = (void **) (4096*10 + (char *) child_stack);
	return clone(func, child_stack, 0
			|CLONE_VM
			|CLONE_FILES
			, NULL);
}
int main(int argc, char *argv[]) { 
	char tempch;
	variable = 9;
	fd = open("test.file", O_RDONLY);
	printf("The variable was %d\n", variable);
	create_1_clonardo(do_something);
	sleep(1); /* waiting for the (condition) races to comeee */

	printf("The variable is now %d\n", variable);
	if (read(fd, &tempch, 1) < 1) {                            
		perror("File Read Error");
		return 1;
	}                                                       
	printf("We could read from the file\n");
	return 0;
}                                                        
