#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int glob = 6; 					/* variavel no seg. dados inicializado */

int main(){
	int var=88;				/* variavel automatica na pilha */
	pid_t pid;
	printf("antes do fork\n");
	if ((pid=fork()) < 0) {
		perror("erro no fork!"); 	/* trat. de erro */
	}
	else if (pid == 0) {
		glob++; var++; 			/* filho modifica variaveis */
	}
	else sleep(2);				/* pai dorme 2 segundos */
	
	printf("pid=%d, glob=%d, var=%d\n",getpid(), glob, var);
	exit(0);
}
