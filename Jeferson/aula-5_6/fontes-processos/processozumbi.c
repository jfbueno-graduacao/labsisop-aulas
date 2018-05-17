#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int pid;
	printf("Antes do fork\n");
	pid = fork(); 						/* Criar processo filho */
	printf("Depois do fork\n");

	switch(pid) {
	case -1:
		perror("Erro no fork\n"); 
		exit(-1);
		break;
	case 0:
		printf("Sou o filho, e vou morrer...\n");	/* Filho: Termina com status qualquer */
		exit(43);
		break;
	default:
		printf("Sou o pai e vou entrar em loop...\n");
		while(1){
			sleep(100);				/* Pai: nunca termina */
		}
	}
	return 0; 						
}
