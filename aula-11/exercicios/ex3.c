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

	switch(fork()) 
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			close(fd[0]); 

			execlp("gzip", "gzip", arquivo, "-d -c", NULL);
			perror("erro ao executar gzip");
			break;
		default:
			close(fd[1]);

			int n = read(fd[0], buffer, BUFFER_SIZE);
			write(STDOUT_FILENO, buffer, n);
			break;
	}
}
