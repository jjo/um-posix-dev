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

int main(int argc, const char *argv[]) {
	int sd, conn_sd;				/* descriptores */
	struct sockaddr_in addr_local;		/* donde voy a escuchar */
	struct sockaddr_in addr_remoto;		/* el que se conecta */
	socklen_t addr_len;		/* longitud de addr_local */
	int i;				/* para setsockopt */
	pid_t hijo; 				/* pid del hijo */
	char buffer[MAX_SIZE];			/* Buffer para la lectura */
	unsigned short port;
	int nread;
	
	/* Crea el socket */	/* Verifico el nro de argumentos */
	if (argc != 2) {
		fprintf(stderr, "Uso: %s <puerto>\n",
				argv[0]);
		return 1;
	}

 
	if ((sd = socket(PF_INET, SOCK_STREAM, 0))<0) {
		perror("socket");
		return 1;		
	}

	/* Se desea reutilizar la direccion local */
	i=1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

	/* Inicializa y configura la estructura de socket del servidor */ 
	memset(&addr_local, 0, sizeof(addr_local));
	addr_local.sin_family = AF_INET;

	port = atoi(argv[1]);
	if (port <=0) {
		fprintf(stderr, "port=%d invalido\n", port);
		return 255;
	}
	addr_local.sin_port = htons(port);
		
	/* Liga el socket a una direccion */
	addr_len = sizeof(addr_local);
	if ( (bind(sd, (struct sockaddr *)&addr_local, 
		addr_len)) < 0 ) {
		perror("bind");
		return 2;		
	}
	
	/* Aguarda a q arriben conexiones */
	if ( (listen(sd, 5)) < 0 ) {
		perror("listen");
		return 3;
	}

	printf("\nSOCKET %d ESCUCHANDO ...\n", port);
	printf("\tPuerto: %d\n", ntohs(addr_local.sin_port));
	printf("\tDireccion IP: %s\n", inet_ntoa(addr_local.sin_addr));
	
	/* Ejecuta indefinidamente la aceptacion de conexiones */
	while ((conn_sd = accept(sd, (struct sockaddr *)&addr_remoto, 
		&addr_len))>=0) {
		
		switch(hijo=fork()) {
		case -1: /* Hubo un problema con la creacion del hijo */ 
			break;
		case 0: /* Codigo del hijo, futuros ZOMBIES ... */

			close(sd);
			/* Leo una sola vez del socket (supongo suficiente) */
			while ((nread=read(conn_sd, buffer, MAX_SIZE))>0) {
				write(STDOUT_FILENO, buffer, nread); 
			}
			return 0;
			break;
			
		default: /* Codigo del padre */			
			close(conn_sd);
			printf("\nNueva conexion pid=%i IP remoto=%s, puerto remoto=%d\n", 
					hijo,
					inet_ntoa(addr_remoto.sin_addr),
					ntohs(addr_remoto.sin_port));
			break;
		}
	}
	close(sd);
	return 0;
}

/*
 * $Log: tcp_ip_server.c,v $
 * Revision 1.3  2003/10/03 20:47:42  jjo
 * . cosmetic
 *
 * Revision 1.2  2003/08/22 20:51:48  jjo
 * . Renombrado struct_server -> addr_local
 * . argv[1]: port
 *
 */
