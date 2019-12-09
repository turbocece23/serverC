/* Brunello Cesare 5AI
 * 
 * Generare due processi padre/figlio in cui:
 * ➢ Il processo padre scrive la sequenza di caratteri [A-Z][a-z] in
 * un file di testo (aperto prima di eseguire la fork) e
 * successivamente chiude e rimuove il file creato
 * ➢ Il processo figlio attende 5 secondi (tramite la funzione “sleep
 * (num_secs)”) e stampa il contenuto del file precedentemente
 * gestito dal processo padre
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Variabile di tipo FILE che verrà utilizzata per aprire il file su cui intendiamo lavorare
FILE *f;

int main()
{
	//Il file viene aperto dal processo padre per inserire all'interno i caratteri
	f=fopen("alfabeto.txt","w");
	printf("\n\nIL MIO PID (Processo padre): %d\n\n",getpid());
	
	pid_t pidpadre = getpid();
	
	int i;
	//Creazione della sequenza di caratteri in minuscolo
	char c='A';
	printf("Stampa dei caratteri in maiuscolo\n");
	for(i=0;i<26;i++)
	{
		fprintf(f,"%c",c);
		c++;
	}
	
	//Creazione della sequenza di caratteri in maiuscolo
	c='a';
	printf("Stampa dei caratteri in minuscolo\n");
	for(i=0;i<26;i++)
	{
		fprintf(f,"%c",c);
		c++;
	}
	
	printf("Sono il padre e ho finito di usare il file, lo chiudo.\n\n");
	fclose(f);
	
	pid_t pid1 = fork();
	
	if(pid1<0)
	{
		printf("Qualcosa è andato storto, la creazione del fork non è andata a buon fine\n");
		printf("Termino il programma\n");
		return 1;
	}else if(getpid() != pidpadre )
	{
		sleep(2);
		printf("\n\nIL MIO PID (Processo figlio): %d\n\n",getpid());
		f=fopen("alfabeto.txt","r");
		
		printf("Stampo il contenuto del file:\n");
		char d = fgetc(f);
		while (d != EOF)
		{
			printf ("%c", d);
			d = fgetc(f);
		}
		fclose(f);
	}
	
	wait(NULL);
	return 0;
}
