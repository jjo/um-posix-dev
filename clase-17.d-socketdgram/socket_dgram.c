 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <netdb.h>

     /*
      * descripteurs de socket
      *
      * sdr : pour la reception
      * sdw : pour l'emission
      */
     int sdr, sdw;

     /*
      * sockets
      *
      * sock_r : pour la reception
      * sock_w : pour l'emission
      */
     struct sockaddr_in sock_r, sock_w;

     /*
      * creation des sockets
      */
     sdr = socket(PF_INET, SOCK_DGRAM, 0);
     if (sdr < 0) {
         perror("socket");
         exit(1);
     }
     sdw = socket(PF_INET, SOCK_DGRAM, 0);
     if (sdw < 0) {
         perror("socket");
         exit(1);
     }

     /*
      * initialisation de la socket de reception
      */
     memset(&sock_r, 0, sizeof(sock_r));
     sock_r.sin_family = AF_INET;
     sock_r.sin_port = htons(PORT);
     sock_r.sin_addr.s_addr = htonl(INADDR_ANY);
     /*
      * initialisation de la socket d'emission
      */
     memset(&sock_w, 0, sizeof(sock_w));
     sock_w.sin_family = AF_INET;
     sock_w.sin_port = htons(PORT);
     sock_w.sin_addr.s_addr = inet_addr(GROUP);

     len_r = sizeof(sock_r);
     len_w = sizeof(sock_w);
     struct ip_mreq {
         struct in_addr imr_multiaddr;   /* multicast group to join */
         struct in_addr imr_interface;   /* interface to join on    */
     }

     /*
      * allocation de la structure imr
      */
     struct ip_mreq imr;

     /*
      * initialisation de la structure imr
      */
     imr.imr_multiaddr.s_addr = inet_addr(group);
     imr.imr_interface.s_addr = htonl(inaddr_any);


     if (setsockopt(sdr, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &imr, sizeof(struct ip_mreq)) < 0) {
         perror("setsockopt - IP_ADD_MEMBERSHIP");
         exit(1);
     }

     /*
      * operation bind
      */
     if (bind(sdr, (struct sockaddr *)&sock_r, sizeof(sock_r)) < 0) (
         perror("bind");
         exit(1);
     }

   La reception proprement dite des donnees se realisera par un appel `a
   recvfrom() `a travers l'interface socket.

     /*
      * reception des datagrammes
      */
     while (1) {
         cnt = recvfrom(sdr, buf, sizeof(buf), 0, (struct sockaddr *)&sock_r, &len_r);
         if (cnt < 0) {
             perror("recvfrom");
             exit(1);
         }
         else if (cnt == 0) { /* fin de transmission */
             break;
         }
         printf("%s\n", buf); /* affichage du message */
     }

     /*
      * emission des datagrammes
      */
     cnt = sendto(sdw, buf, strlen(buf), 0, (struct sockaddr *)&sock_w, len_w);
     if (cnt < 0) {
         perror("sendto");
         exit(1);
     }

