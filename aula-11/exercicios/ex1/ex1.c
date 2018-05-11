#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define MAXLINE 4096

int main(){
	int n, fd[2];
	char line[MAXLINE];

	if (pipe(fd) != 0){
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	printf("ID do processo Pai %d\n\n", getpid());

	switch (fork())
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:
			printf("ID do processo filho %d\n\n", getpid());
			close(fd[0]);

			printf("Processo filho (%d) escrevendo no PIPE\n\n", getpid());
			write(fd[1], "Hello World\n" ,12);
			break;
		default:
			close(fd[1]);

			printf("Processo Pai (%d) lendo do PIPE\n\n", getpid());
			n = read(fd[0], line, MAXLINE);
			
			write(STDOUT_FILENO, line, n);
			break;
	}
	exit(EXIT_SUCCESS);
}
