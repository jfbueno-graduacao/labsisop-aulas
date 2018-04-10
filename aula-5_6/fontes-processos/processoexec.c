#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){   
	pid_t p = fork();					/* cria novo processo */
	if (p < 0) { 						/* ocorrencia de erro */
		fprintf(stderr, "Criação de novo processo falhou! \n");
		exit(-1);					/* terminacao de erro */
	} else if (p == 0) {	 				/* processo filho */
		printf("Iniciando a execução do filho... \n"); 
		execlp("/bin/ls", "ls", NULL);			/* atribui novo programa ao filho */
		printf("Filho com parada de execução forçada! \n"); 
		exit(-1);
	} else { 						/* processo pai */
		printf("Pai aguardando o filho terminar... \n"); 
		wait(NULL);					/* pai vai aguardar até o filho completar */
		printf("Filho completou a execução! \n");	
		exit(0);					/* terminacao de sucesso */
	}	
}
