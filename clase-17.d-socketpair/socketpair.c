#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#define tostr(x) #x
#define ERRSYS(call) do { if ( (call) < 0) { perror(tostr(call));exit(1); }} while (0)

enum data_type { INTEGER, DOUBLE };
enum data_op { SUMAR, RESTAR, MULTIPLICAR, DIVIDIR };

struct data {
	int id;
	enum data_type type;
	enum data_op   op;
	union {
		double d[3];
		int    i[3];
	}u;
};
int servicio(int fd)
{
	struct data data;
	read (fd, &data, sizeof (struct data));
	switch (data.op) {
		case SUMAR:
			switch(data.type) {
				case INTEGER: data.u.i[0]=data.u.i[1]+data.u.i[2];
					      break;
				case DOUBLE:  data.u.d[0]=data.u.d[1]+data.u.d[2];
					      break;
			}
			break;
		case RESTAR:
			switch(data.type) {
				case INTEGER: data.u.i[0]=data.u.i[1]-data.u.i[2];
					      break;
				case DOUBLE:  data.u.d[0]=data.u.d[1]-data.u.d[2];
					      break;
			}
			break;
	}
	write (fd, &data, sizeof (struct data));
	return 0;

}
int pide_suma_int(int fd, int a, int b){
	struct data data = {
		.type = INTEGER,
		.op   = SUMAR,
		.u = {
			.i = { 0, a, b }
		}
	};
	write(fd, &data, sizeof (struct data));
	read(fd, &data, sizeof (struct data));
	return data.u.i[0];
}
int main(void)
{
	int fds[2];
	int ret;
	ERRSYS(socketpair(PF_UNIX, SOCK_DGRAM, 0, fds));
	switch(fork()) {
		case 0: /* hijo */
			close(fds[0]);
			return servicio(fds[1]);
		case -1: /* error*/
			perror("fork()");
			return 1;
	}
	/* padre */
	close(fds[1]);
	ret=pide_suma_int(fds[0], 10, 22);
	printf("resultado=%d\n", ret);
	return 0;
}
