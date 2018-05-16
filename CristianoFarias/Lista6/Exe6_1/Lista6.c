#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
char url[300];
void sinal_chegou(int signum){		
	printf("O sinal checou! \n\%s",url);
	switch(signum){				
		case 2:									
			execlp("firefox","firefox --new-window", url, NULL); 
		break;
		default:
			printf("Recebido sinal não catalogado: %d\n", signum);
		break;
}
}

int main(int argc, char *W[]){
	if(argc == 2){
	strcat(url,W[1]);
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
	}else{
	printf("Quantidade de parâmetros informados é inválido, Digite apenas 1 parametro.\n");	
	return 1;	
	}	
}




