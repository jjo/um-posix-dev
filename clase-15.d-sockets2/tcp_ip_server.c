#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 4096

int main(void) {
	int sd, csd;				/* descriptores */
	struct sockaddr_in struct_server;	/* struct para el socket tcp */
	socklen_t struct_server_len;		/* longitud de struct_server */
	int i=1;				/* para setsockopt */
	pid_t hijo; 				/* pid del hijo */
	int status, ret;
	char buffer[MAX_SIZE];			/* Buffer para la lectura */
	
	/* Crea el socket */ 
	if ((sd = socket(PF_INET, SOCK_STREAM, 0))<0) {
		perror("socket");
		return 1;		
	}

	/* Se desea reutilizar la direccion local */ 
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

	/* Inicializa y configura la estructura de socket del servidor */ 
	memset(&struct_server, 0, sizeof(struct_server));
	struct_server.sin_family = AF_INET;
	struct_server.sin_port = htons(2411);
		
	/* Liga el socket a una direccion */
	struct_server_len = sizeof(struct_server);
	if ( (bind(sd, (struct sockaddr *)&struct_server, 
		struct_server_len)) < 0 ) {
		perror("bind");
		return 2;		
	}
	
	/* Aguarda a q arriben conexiones */
	if ( (listen(sd, 5)) < 0 ) {
		perror("listen");
		return 3;
	}

	printf("\nSOCKET 2411 ESCUCHANDO ...\n");
	printf("\tPuerto: %d\n", ntohs(struct_server.sin_port));
	printf("\tDireccion IP: %s\n", inet_ntoa(struct_server.sin_addr));
	
	/* Ejecuta indefinidamente la aceptacion de conexiones */
	while ((csd = accept(sd, (struct sockaddr *)&struct_server, 
		&struct_server_len))>=0) {
		
		switch(hijo=fork()) {
		case -1: /* Hubo un problema con la creacion del hijo */ 
			break;
		case 0: /* Codigo del hijo */

			/* Carga la estructura con todos ceros */
			memset(buffer, 0, MAX_SIZE);			
			/* Leo una sola vez del socket (supongo suficiente) */
			while (read(csd, buffer, MAX_SIZE)>0) {
				write(1, buffer, strlen(buffer)); //->-> stdout 
				memset(buffer, 0, MAX_SIZE);			
			}
			return 0; /* Muestra q hubo un error en el hijo */
			break;
			
		default: /* Codigo del padre */			
			if ((ret = waitpid(0, &status, WNOHANG))<0) {
				perror("wait");
			}
			break;
		}
		printf("\nNueva conexion %i\n", hijo);
	}
	close(sd);
	return 0;
}
