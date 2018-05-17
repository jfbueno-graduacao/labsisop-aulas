#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){

	int fd[2];

	if (pipe(fd) != 0){
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	switch (fork()){
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
		break;
	case 0:
		//close(fd[0]);
		//dup2(fd[1], STDOUT_FILENO);
		//close(fd[1]);
		//printf("PAI CAPTUROU > ");
		//perror("exec LS");
		break;
	default:
		close(fd[0]);
		dup2(fd[1], STDIN_FILENO);
		close(fd[1]);
		execlp("date", "date", NULL);
		perror("exec date");
		break;
	}
	exit(EXIT_SUCCESS);
}
