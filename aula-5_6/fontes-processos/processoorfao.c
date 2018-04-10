#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int pid;
	printf("Antes do fork\n");
	pid = fork(); 			/* Criar processo filho */
	printf("Depois do fork\n");

	switch(pid) {
	case -1:
		perror("Erro no fork\n"); 
		exit(-1);
		break;
	case 0:
		printf("Sou o filho, estou entrando em loop... Meu PID eh: %i \n", getpid());
		while(1){ 
			sleep(100);	/* Filho: nunca termina */
		}
		//printf("Sou o filho, nao vou exibir este texto porque estou em loop!");
		break;
	default:
		printf("Sou o pai e vou finalizar com erro 42\n");
		exit(42); 		/* Pai: Termina com status qualquer */
	}
	return 0; 			/* nunca será executado */
}
