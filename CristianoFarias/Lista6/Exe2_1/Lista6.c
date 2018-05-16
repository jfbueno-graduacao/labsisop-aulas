#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
int time = 10;
void tratador(int signum){		
	switch(signum){					
		case 14:
			printf("Processo sendo executado a %i segundos\n", time);
		break;	

		default:
			printf("Recebido sinal não catalogado: %d\n", signum);
		break;
}
}
int main(){
	printf("Meu PID = %d\n", getpid());
	
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = &tratador;

	if(sigaction(SIGALRM, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	//agenda SIGALRM para daqui a 5 segundos
	if (alarm(time) < 0){
		perror("Falha ao agendar alarme");
	}	

	printf("O processo é eterno, enquanto dura...\n");

	//espera ocupada (busy-waiting) pelo alarme
	while(1);
	
	printf("Linha que nunca aparece...\n");

	return 0;
}




