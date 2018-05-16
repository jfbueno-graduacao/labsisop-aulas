#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>

//variável compartilhada entre main() e tratador de sinal
volatile sig_atomic_t counter = 0;

//tratador de sinal
void tratador(int signum){
	counter++;
}

int main(){
	sigset_t mask;
	struct sigaction action;
	memset(&action, 0, sizeof(action));

	action.sa_handler = &tratador;

	if(sigaction(SIGUSR1, &action, NULL) == -1){
		perror("Falha em sigaction");
		exit(-1);
	}

	printf("Meu PID = %d\n Envie SIGUSR1 para continuar, ou SIGINT para sair \n", getpid());

	//seleciona todos os sinais, exceto SIGINT e SIGUSR1
	sigfillset(&mask);
	sigdelset(&mask,SIGUSR1);
	sigdelset(&mask,SIGINT);

	while(1){	//loop aguardando sinal
		//sigsuspend(&mask);
		sleep(1);
		printf("Contador = #%d\n",counter);
	}
	return 0;
}




