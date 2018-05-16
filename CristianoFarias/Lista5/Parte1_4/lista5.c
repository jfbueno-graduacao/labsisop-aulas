//Cristiano Farias - Laboratorio de Sistemas - Lista5

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int lerInt(){
	int i;
	printf("Digite um valor int-> ");
	scanf("%d",&i);
	return i;
}

void repete(char *W, int N){
	for (int i=0;i<N; i++) {
		printf("#%i '%s' MEU_PID:%d PID_PAI:%d\n",i+1,W,getpid(),getppid());
		sleep(1);
	}
}

int check (int qtd){
	if(qtd == 2){
		return 1;
	}
	return 0;
}

int main(int argc, char *W[]){
	int i;
	pid_t pid;
	if(check(argc) == 1){
		i = lerInt();
		if ((pid=fork()) < 0) {
			perror("erro no fork!"); 
			return 1;
		}
		else if (pid == 0) {
			repete(W[1],i);
			return 0;	
		}
		else {
			printf("pai Terminou-> PID:%d    PID PAI do PAI:%d\n",getpid(),getppid());
			return 0;	
		}
	}else{
		printf("Quantidade de parâmetros informados é inválido, Digite apenas 1 parametro.\n");	
		return 1;	
	}
}
