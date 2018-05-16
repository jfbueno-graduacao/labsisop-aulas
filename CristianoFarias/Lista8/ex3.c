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

void run()
{
	char *t = lerString("Digite o nome de um Arquivo");
	int aux;
	aux = open(t, O_RDONLY);
	if (aux == -1)
	{
		aux = open(t, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		char *texto = lerString("Digite o texto do novo Arquivo");
		if (aux == -1)
		{
			printf("Falha para criar arquivo! \n");
			exit(0);
		}
		write(aux, texto, strlen(texto));
		close(aux);
	}
	else
	{
		char buf2[1000];
		read(aux, buf2, 1000);
		printf("Conteudo -> \n%s", buf2);
	}
}

int main(int argc, char *W[])
{
	run();
}
