#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

void descompactar(char* arquivo);

int main(int argc, char **argv)
{
	if(argc != 2) {
		printf("Entre com um parâmetro da linha de comando\n");
		return 1;
	}

	char* ext = strrchr(argv[1], '.');

	if(strcmp(ext, ".gz") == 0) {
		descompactar(argv[1]);
	}	
}

void descompactar(char* arquivo) {
	int fd[2];
	char buffer[BUFFER_SIZE];

	printf("%s\n", arquivo);

	switch(fork()) 
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			close(fd[0]); //Fecha leitura

			dup2(fd[1], STDIN_FILENO);			
			close(fd[1]);

			//opcao -c envia o resultado para STDIN
			execlp("gzip", "gzip", arquivo, "-d", "-c", NULL);
			perror("erro ao executar gzip");
			break;
		default:
			dup2(fd[0], STDOUT_FILENO);

			printf("%s\n", "Processo pai. Lendo arquivo:");

			execlp("cat", "cat", NULL);
			perror("erro ao executar cat");
			break;
	}
}

