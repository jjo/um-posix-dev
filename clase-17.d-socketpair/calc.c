#include <stdio.h>
#include <unistd.h>
#include "calc.h"
int calc_server(int fd)
{
	struct data_packet data;
	int nread;
	while((nread=read (fd, &data, sizeof (data)))>0) {
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
			/* falta contemplar: MULTIPLICAR, DIVIDIR */
		}
		write (fd, &data, sizeof (data));
	}
	return 0;

}
int calc_client_integer(int fd, enum data_op operation, int a, int b){
	struct data_packet data = {
		.type = INTEGER,
		.op   = operation,
		.u = {
			.i = { 0, a, b }
		}
	};
	write(fd, &data, sizeof (data));
	read(fd, &data, sizeof (data));
	return data.u.i[0];
}

