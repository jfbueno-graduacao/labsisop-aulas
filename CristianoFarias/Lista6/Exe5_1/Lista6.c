#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>

void sinal_chegou(int signum){	
	char url[128];	
	printf("O sinal checou! \n");
	switch(signum){				
		case 2:						
			printf("Digite uma URL -> ");
			scanf("%s", url);
			execlp("firefox","firefox --new-window", url, NULL); 
		break;
		default:
			printf("Recebido sinal não catalogado: %d\n", signum);
		break;
}
}

int main(){
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = &sinal_chegou;

	printf("Meu PID = %d\n", getpid());


	if(sigaction(SIGINT, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	for(;;)	{}	//espera (busy-waiting) pelo sinal
	return 0;
}




