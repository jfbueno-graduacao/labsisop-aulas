#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
char palavra[300];

int contarVogais(){
	int i;
	int cont=0;
	for (i=0; i < strlen(palavra); i++) {
	int n = palavra[i];
		switch(n){
			case 97:			
				cont++;
			break;	
			case 101:			
				cont++;
			break;	
			case 105:			
				cont++;
			break;	
			case 111:			
				cont++;
			break;	
			case 117:			
				cont++;
			break;	
			case 65:			
				cont++;
			break;	
			case 69:			
				cont++;
			break;
			case 73:			
				cont++;
			break;	
			case 79:			
				cont++;
			break;	
			case 85:			
				cont++;
			break;
			default	:				
			break;
		}     		
	
	}
	return cont;
}
int contarDigitos(){
	int i;
	int cont=0;
	for (i=0; i < strlen(palavra); i++) {
	int n = palavra[i];
		switch(n){
			case 49:			
				cont++;
			break;	
			case 50:			
				cont++;
			break;	
			case 51:			
				cont++;
			break;	
			case 52:			
				cont++;
			break;	
			case 53:			
				cont++;
			break;	
			case 54:			
				cont++;
			break;	
			case 55:			
				cont++;
			break;
			case 56:			
				cont++;
			break;	
			case 57:			
				cont++;
			break;	
			case 48:			
				cont++;
			break;
			default	:				
			break;
		}     		
	
	}
	return cont;
}
void sinal_chegou(int signum){		
	switch(signum){				
		case 10:			
			printf("Numero de vogais: %i\n",contarVogais());
		break;
		case 12:
			printf("Numero de digitos: %i\n",contarDigitos());
		break;
		default:
			printf("Recebido sinal não catalogado: %d\n", signum);
		break;
	}
}

void run(){	
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = &sinal_chegou;

	printf("Meu PID = %d\n", getpid());
	printf("Palavra recebida = %s\n", palavra);

	
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
	printf("Envie um sinal!!! Estamos aguardando!\n");

	for(;;)	{}	//espera (busy-waiting) pelo sinal	
}

int main(int argc, char *W[]){
	if(argc == 2){
		strcat(palavra,W[1]);
	run();
		return 0;
	}else{
		printf("Quantidade de parâmetros informados é inválido, Digite 1 parametro.\n");	
	return 1;	
	}	
}




