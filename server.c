#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>

#include <string.h>
#include <signal.h>

#define MAX 8192 //In bytes, 8KB
#define PORTA 5555 //Porta del servizio
void quitprogramma(int sock);

int main()
{
	signal(SIGINT,quitprogramma);
	//Dati di invio e ricezione
	char buff[MAX];
	char messaggi[MAX]="Messaggio inviato: ";
	
	//Indirizzo del server
	struct sockaddr_in server_addr;
	
	//Indirizzo del client
	struct sockaddr_in client_addr;
	
	//I socket descriptor usati per identificare server e client
	int sd_server, sd_client;

	//Creazione socket descriptor per il server.
	//AF_INET + SOCK_STREAM --> TCP, utilizzo del protocollo TCP (IPPROTO_TCP)
	printf("socket()\n");
	if((sd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Errore nella creazione del server\n");
		return 1;
	}

	//Inseriamo nella struttura alcune informazioni
	//La famiglia dei protocolli
	server_addr.sin_family = AF_INET;
	//La porta in ascolto
	server_addr.sin_port = htons(PORTA);
	//Dato che è un server bisogna associargli l'indirizzo della macchina su cui sta girando
	server_addr.sin_addr.s_addr = INADDR_ANY;

	//Assegnazione del processo alla socket tramite la funzione BIND
	if(bind(sd_server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Errore di binding\n");
		return(2);
	}

	//Si mette in ascolto con un massimo di 5 connessioni
	listen (sd_server, 5);

	//Essendo un server monothreading, accetterà una sola connessione per volta
	//Dimensione della struttura client_addr
	int address_size = sizeof(client_addr);
	
	if((sd_client = accept(sd_server, (struct sockaddr *)&client_addr, &address_size)) < 0)
	{
		printf("Errore nella chiamata accept\n");
	}
	
	//Si ricevono i dati dal client
	recv(sd_client, buff, sizeof(buff), 0);
	printf("Messaggio ricevuto: %s", buff);
	
	//Si spedisce il messaggio appena ricevuto per la conferma
	send(sd_client, messaggi, strlen(messaggi), 0);
	send(sd_client, buff, strlen(buff), 0);
	
	//Chiusura del socket descriptor
	shutdown(sd_server,2);
	shutdown(sd_client,2);
	close(sd_client);
	close(sd_server);

	printf("Programma terminato");
	return EXIT_SUCCESS;
}

void quitprogramma(int sock)
{
	printf("Programma interrotto");
	close(sock);
}
