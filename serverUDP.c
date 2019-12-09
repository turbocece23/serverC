#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define SOCKET_ERROR   ((int)-1)
#define SIZEBUF 10000
#define SERVER_PORT 1313          // numero di porta del server

int main(int argc, char *argv[]){
	struct sockaddr_in locale, remoto;
	int socketfd, msglen, len, ris;
	char msg[SIZEBUF];

	/* configurazione parametri porta locale */
	locale.sin_family = AF_INET;// internet
	locale.sin_addr.s_addr = htonl(INADDR_ANY);// qualunque IP
	locale.sin_port = htons(SERVER_PORT);

	/* creazione del socket */
	socketfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketfd == SOCKET_ERROR) {
		printf ("socket() fallito err: %d \"%s\"\n", errno, strerror(errno));
		exit(1);
	}
	printf ("socket() ok\n" );
	fflush(stdout);

	ris = bind(socketfd, (struct sockaddr*) &locale, sizeof(locale));
	if (ris == SOCKET_ERROR)  {
		printf ("bind() fllita err: %d \"%s\"\n",errno, strerror(errno));
		exit(1);
	}
	printf ("bind() sul server ok, ora mi pongo in attesa:\n" );
	fflush(stdout);

	/* attesa della ricezione di un datagramma */
	len = sizeof(struct sockaddr_in);
	msglen = recvfrom(socketfd, msg, (int)SIZEBUF, 0, (struct sockaddr*)&remoto, &len);
	if (msglen<0) {
		char msgerror[1024];
		sprintf(msgerror,"recvfrom() failed [err %d] ", errno);
		perror(msgerror); 
	}

	/* lettura datagramm e parametri del mittente */ 
	printf ("ho ricevuto un messaggio: \n" );
	printf("ricevuto  msg: \"%s\" lunghezza: %d \n", msg, msglen);

	// variante 1: solo visualizzazione utilizzando inet_ntoa()
	// printf("host mittente: %s:%d\n", inet_ntoa(remoto.sin_addr), ntohs(remoto.sin_port));

	// variante 2: salvataggio utilizzando inet_ntoa()  
	// char s_IP_Remoto[15];                      // indirizzo dell'host remoto 
	// short int portaRemota;                     // porta usata dall'host remoto
	// sprintf((char*)s_IP_Remoto,"%s", inet_ntoa(remoto.sin_addr));  
	// portaRemota = ntohs(remoto.sin_port);      // trasforma in formato Big Endian 
	// printf("host mittente: %s:%d\n", s_IP_Remoto, portaRemota);

	// variante 3: salvataggio utilizzando inet_ntop()
	char s_IP_Remoto[15];                       // indirizzo dell'host remoto 
	short int portaRemota;                      // porta usata dall'host remoto
	inet_ntop(AF_INET, &remoto.sin_addr, s_IP_Remoto, sizeof(s_IP_Remoto));
	portaRemota = ntohs(remoto.sin_port);       // trasforma in formato Big Endian 
	printf("host mittente: %s:%d\n", s_IP_Remoto, portaRemota);

	fflush(stdout);

	// chiusura socket 
	close(socketfd);
	return(0);
}

/* riceveUDP.c   SERVER che riceve datagram UDP  unicast  */

