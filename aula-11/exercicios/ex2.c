#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define BUFFER_SIZE 4096

int main()
{
	int fd[2];
	char buffer[BUFFER_SIZE];

	if (pipe(fd) != 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	switch (fork())
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			close(fd[0]); 
			/*dup2(fd[1], STDOUT_FILENO); 
			close(fd[1]);*/

			execlp("date", "date", NULL);
			perror("exec date");
			break;
		default:
			close(fd[1]);		

			printf("Processo pai\nEscrevendo a saída do filho...\n");
			int n = read(fd[0], buffer, BUFFER_SIZE);
			write(STDOUT_FILENO, buffer, n);
			break;
		}

	exit(EXIT_SUCCESS);
}
