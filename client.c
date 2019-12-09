#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include <signal.h>

#define SOCKET_ERROR ((int)-1)
#define SERVER_PORT 5555
#define MAXSIZE 10

int main(int argc, char **argv)
{
	//Indirizzo del server
	char indirizzoServer[]="127.0.0.1";
	//Messaggio da inviare
	char messaggio[] = "ciao";
	//Messaggio ricevuto
	char buffer[MAXSIZE];
	//Singolo carattere inviato e ricevuto
	char ch, chMaiu;
	int ris, nwrite, len;
	
	//Identificatore del socket
	int socketfd;
	//Dati dei socket
	struct sockaddr_in locale, remoto;
	
	//Settaggio del socket locale
	locale.sin_family = AF_INET;
	locale.sin_addr.s_addr = htonl(INADDR_ANY);
	locale.sin_port = htons(0);
	
	//Assegnazione parametri del server
	remoto.sin_family = AF_INET;
	remoto.sin_addr.s_addr = inet_addr(indirizzoServer);
	remoto.sin_port = htons(SERVER_PORT);
	
	//Impostazioe del transport endpoint
	printf("Creazione del socket()");
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
}
