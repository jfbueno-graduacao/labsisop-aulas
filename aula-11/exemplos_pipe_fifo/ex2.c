#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){

	int fd[2];

	if (pipe(fd) != 0){
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
			close(fd[1]); // Fecha escrita porque ele vai receber
			dup2(fd[0], STDIN_FILENO); // Aponta f[0] para e entrada padrão 
			close(fd[0]); // fecha fd[0]

			execlp("wc", "wc", NULL);
			perror("exec WC");
			break;
		default:
			close(fd[0]);			
			dup2(fd[1], STDOUT_FILENO);			
			close(fd[1]);

			execlp("ls", "ls", NULL);
			perror("exec LS");
			break;
		}
	exit(EXIT_SUCCESS);
}
