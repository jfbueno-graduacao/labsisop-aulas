//Cristiano Farias - Laboratorio de Sistemas - Lista5

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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
	int status;
	pid_t pid;
	if(check(argc) == 1){
		i = lerInt();
		if ((pid=fork()) < 0) {
			perror("erro no fork!"); 
			exit(-1);
			return 1;
		}
		else if (pid == 0) {
			if((i < 0) || (i > 10)){
				exit(2);
			}
			else{
				repete(W[1],i);
				exit(1);
			}	
			return 0;
		}
		else {
			pid = wait(&status);
			printf("pai Terminou-> PID:%d    PID PAI do PAI:%d\n",getpid(),getppid());
			if(WEXITSTATUS(status) == 1){
				printf("filho processou e terminou com sucesso -> STATUS:%d\n",WEXITSTATUS(status));
			}
			else{
				printf("filho não processou com sucesso -> STATUS:%d\n",WEXITSTATUS(status));
			}			
			return 0;	
		}
	}else{
		printf("Quantidade de parâmetros informados é inválido, Digite apenas 1 parametro.\n");	
		return 1;	
	}
}
