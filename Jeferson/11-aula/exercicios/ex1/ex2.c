#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main() {

	int fd[2];

	if (pipe(fd) != 0) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	printf("ID do processo pai %d\n\n", getpid());

	switch (fork())
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		case 0:

			printf("ID do processo filho %d\n\n", getpid());
			close(fd[1]);

			printf("Processo filho copiando PIPE para StdIn\n\n");
			dup2(fd[0], STDIN_FILENO);

			printf("Processo filho fechando PIPE\n\n");
			close(fd[0]); 

			execlp("wc", "wc", NULL);
			perror("exec WC");
			break;
		default:
			printf("ID do processo filho %d\n\n", getpid());
			close(fd[0]);			

			printf("Processo filho copiando pipe para StdOut \n\n");
			dup2(fd[1], STDOUT_FILENO);

			printf("Processo filho fechando pipe\n\n");
			close(fd[1]);

			execlp("ls", "ls", NULL);
			perror("exec LS");
			break;
		}
	exit(EXIT_SUCCESS);
}
