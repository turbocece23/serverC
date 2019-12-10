#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

//Porta del server
#define SERVER_PORT 9009
//Numero massimo di connessioni
#define MAX_CONNECTION 10
//Buffer impiegato per i messaggi inviati tra client e server
#define BUFFER_LENGTH 80

//prototipi funzione
void reverse(int);

int main () {
	//newsocket è il socket in cui il server si pone in ascolto
	//client_len è la dimensione del client in connessone con il server
	//wsocket è il socket impiegato per scambiare messaggi e stringe, esso è sempre il socket
	//impiegato dal server, diventa wsocket dopo che viene chiamata la funzione accept()
	int newsocket, client_len, wsocket,pid;
	//Indirizzo del server e del client di tipo sockaddr_in
	struct sockaddr_in servizio,cliente;
	//Variabile che conta i client che si connettono al server
	int cont=0;

	printf ("socket()\n");
	//Creazione del nuovo socket con la chiamata della funzione socket()
	//AF_INET indica la famiglia di indirizzi IPv4
	//SOCK_STREAM indica il protocollo TCP, SOCK_DGRAM indica l'UDP
	if((newsocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Creazione del socket fallita");
		return(1);
	}

	//viene settata la famiglia IPv4
	servizio.sin_family = AF_INET;
	//INADDR_ANY è una variabile che accetta qualunque indirizzo in entrata
	//dato che non possiamo settare un indirizzo del client in arrivo visto
	//che non sappiamo chi potrebbe connettersi al nostro server
	servizio.sin_addr.s_addr = htonl(INADDR_ANY);
	//La porta del nostro server aperta per l'utilizzo del nostro programma
	servizio.sin_port = htons(SERVER_PORT);

	//parametri del bind()
	//socket(scoket effettivo, il puntatore al socket creato prima, dimensione del socket)
	printf ("bind()\n");
	if (bind(newsocket, (struct sockaddr *)&servizio, sizeof servizio) == -1)
	{
		//In caso di bind fallito, stampa un messaggio di errore
		perror("Bind() fallita.\n");
		return(2);
	}

	//Mettiamo il server in ascolto per poter essere in grado di accettare eventuali connessioni
	printf ("listen()\n");
	listen(newsocket, MAX_CONNECTION);//coda massima di 10 elementi

	//Gestione delle connessioni dei client
	printf ("accept()\n");
	while (1) {
		if(cont<MAX_CONNECTION){
			cont++;
			client_len = sizeof(cliente);
			//Controlla che l'accept della funzione sia andato a buon fine
			//Ora wsocket contiene "la connessione" eseguita al socket del server, faremo rimerimento
			//a questa variabile quando vogliamo chiudere il canale di trasmissione e non la socket stesso
			if ((wsocket = accept(newsocket, (struct sockaddr *)&cliente, &client_len)) < 0)
			{
				perror("Connessione non accettata");
				return(3);
			}
			//Nel caso l'accept sia andato a buon fine
			//Creo il processo figlio
			if((pid=fork()) <0){
				perror("Errore nella creazione del nuovo processo.\n");
				exit(1);
			}
			//Se la creazione del fork è andata a buon fine e sono nel figlio
			else if(pid == 0){
				//Stampa il messaggio anche nel client tramite l'utilizzo di stderr
				fprintf(stderr, "Aperta connessione col server.\n");
				fprintf(stderr, "In esecuzione processo.\n");
				//Corpo centrale del programma che svolge l'attività preposta dell'applicazione
				reverse(wsocket);
				//Chiude il socket, o il canale di comunicazione
				close(wsocket);
				fprintf(stderr, "Chiusa connessione col server.\n");
				exit(1);
			}
		//Questo else fa riferimento a cont<MAX_CONNECTION
		//in caso ci sia un client che porva a connettersi e la codaè già piena
		//Stampa questo messaggio
		}else{
			printf("Un utente ha tentato di connettersi senza successo.\n");
		}
	}
}


void reverse(int fd)
{
	//Stringe di dimensione 80 utilizzate per lo scambio dei messaggi
	char msg[BUFFER_LENGTH];
	char parola[BUFFER_LENGTH];
	char rovescio[BUFFER_LENGTH];
	//i e j serviranno poi a invertire l'ordine della parola ricevuta
	int i=0,j=0;

	//mette la stringa nella variabile specificata
	sprintf(msg,"Benvenuto in reverse string.\n");
	//spedisce (send) all'interno di fd (chiamata anche wsocket, la connessione aperta in precedenza) il messaggio
	//specificando la sua lunghezza, il valore 0 non va cambiato
	send(fd,msg,strlen(msg),0);

	sprintf(msg,"Inserisci stringa : ");
	send(fd,msg,strlen(msg),0);

	//Riceve parola, il messaggio che riceve dal client, specificando la lungehzza del buffer
	recv(fd,parola,BUFFER_LENGTH,0);
	//Stampe di debug
	printf("Ho ricevuto la parola : %s\n",parola);
	printf("Lunghezza stringa ricevuta : %d.\n",(int)strlen(parola)-1);
	//Inversione della stringa ricevuta
	//Imposta i a 0, j alla fine della parola ricevuta
	j=strlen(parola)-3;
	printf("Valore di j : %d.\n",j);
	for(i=0;i<=strlen(parola)-3;i++){
		rovescio[i]=parola[j];
		j--;
	}
	//Stampa nel server
	printf("La stringa rovesciata e' : %s.\n",rovescio);
	//Invia lo stesso messaggio e stampalo, attraverso il socket, nel client
	sprintf(msg,"La stringa rovesciata e' : %s.\n",rovescio);
	send(fd,msg,strlen(msg),0);
	sprintf(msg,"Termine delle comunicazioni con il server.\n");
	send(fd,msg,strlen(msg),0);
	//Chiusura della connessione fra socket
	close(fd);
}
