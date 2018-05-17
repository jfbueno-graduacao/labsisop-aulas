#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

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

	switch(fork()) 
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			close(fd[0]); 

			dup2(fd[1], STDOUT_FILENO);			
			close(fd[1]);

			printf("%s\n", "Processo auxiliar. Descompactando o arquivo" );

			//opcao -c envia o resultado para STDOUT
			execlp("gzip", "gzip", arquivo, "-d", "-c", NULL);
			perror("erro ao executar gzip");
			break;
		default:
			close(fd[1]);

			dup2(fd[0], STDIN_FILENO); // Aponta f[0] para e entrada padrão 
			close(fd[0]); // fecha fd[0]

			wait(NULL);

			int n = read(STDIN_FILENO, buffer, BUFFER_SIZE);
			write(STDOUT_FILENO, buffer, n);
			printf("%s\n", "ahoy" );
			break;
	}
}

