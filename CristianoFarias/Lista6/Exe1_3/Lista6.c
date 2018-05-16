#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>

void tratador(int signum){		
	switch(signum){		
		case 10:
			printf("Recebido sinal de USR1 (%d)\n", signum);
		break;
		case 12:
			printf("Recebido sinal de USR2 (%d)\n", signum);
		break;
		case 13:
			printf("Recebido sinal de PIPE (%d)\n", signum);
		break;
		case 8:
			printf("Recebido sinal de FPE (%d)\n", signum);
		break;
		case 14:
			printf("Recebido sinal de ALRM (%d)\n", signum);
		break;
		case 17:
			printf("Recebido sinal de CHLD (%d)\n", signum);
		break;
		case 6:
			printf("Recebido sinal de ABRT (%d)\n", signum);
		break;
		case 18:
			printf("Recebido sinal de CONT (%d)\n", signum);
		break;
		case 15:
			printf("Recebido sinal de TERM (%d)\n", signum);
		break;
		case 2:
			printf("Recebido sinal de INT (%d)\n", signum);
		break;
		case 3:
			printf("Recebido sinal de QUIT (%d)\n", signum);
		break;

		default:
			printf("Recebido sinal não catalogado: %d\n", signum);
		break;
}

}

int main(){
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = &tratador;

	printf("Meu PID = %d\n", getpid());

	if(sigaction(SIGUSR1, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	if(sigaction(SIGUSR2, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	if(sigaction(SIGPIPE, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}


	if(sigaction(SIGFPE, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	if(sigaction(SIGALRM, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	if(sigaction(SIGCHLD, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	if(sigaction(SIGABRT, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	if(sigaction(SIGCONT, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	if(sigaction(SIGTERM, &sa, NULL) != 0){
		perror("Falha ao instealar tratador de sinal");
		exit(-1);
	}
	if(sigaction(SIGINT, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}
	if(sigaction(SIGQUIT, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}
	
	//O comando Stop é de total controle do sistema, não é possivel utilizar-lo.
	//if(sigaction(SIGSTOP, &sa, NULL) != 0){
	//	perror("Falha ao instalar tratador de sinal");
	//	exit(-1);
	//}

	for(;;)	{}	//espera (busy-waiting) pelo sinal
	return 0;
}




