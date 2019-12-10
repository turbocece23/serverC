#include <stdio.h>

char* letturaStdin()
{
	char *input = NULL;
	int c = 0;
	int pos = 0;
	
	//fino a quando il flusso di stdin non termina
	while((c = fgetc(stdin)) != EOF)
	{
		if(input == NULL)
		{
			//alloco un char per il primo carattere
			input = (char*)malloc(sizeof(char));
		}
		else
		{
			//alloco altri char per i successivi caratteri
			input = (char*)realloc(input, sizeof(char)*(pos+1));
		}
		
		//salva il carttere letto in input
		input[pos] = c;
		
		//tengo conto della dimensione della mia stringa
		pos++;
	}
	
	return input;
}
