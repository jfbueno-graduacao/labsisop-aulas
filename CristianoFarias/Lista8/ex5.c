//Cristiano Farias - Laboratorio de Sistemas - Lista8

#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char *lerString(char *inf)
{
	char chr[1000] = "";
	printf("%s -> ", inf);
	scanf("%[^\n]", chr);
	scanf("%*[^\n]");
	scanf("%*c");
	char *ret = chr;
	return ret;
}

void run(char *t)
{
	//char *t = lerString("Digite o nome de um Arquivo");
	int aux;
	aux = open(t, O_RDONLY);
	if (aux == -1)
	{
		printf("%s Falha para ler o arquivo! \n", t);	
	}
	else
	{
		char buf2[1000];
		read(aux, buf2, 1000);
		//printf("Conteudo -> \n%s", buf2);
		int contLinha = 0;
		for (size_t i = 0; i < strlen(buf2); i++)
		{
			if (buf2[i] == '\n')
			{
				contLinha++;
			}
		}
		printf("%s : %i\n", t, contLinha);
	}
}

int main(int argc, char *W[])
{

	for (int i = 1; i <= argc-1; i++)
	{
		run(W[i]);	
	}
}
