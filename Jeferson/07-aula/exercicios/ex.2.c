#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>

void tratador_sinal(int signumber) {
	printf("SIGALRM foi interceptado e o processo não será finalizado\n");
}

int main() {

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = &tratador_sinal;

	if(sigaction(SIGALRM, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	if (alarm(5) < 0){
		perror("Falha ao agendar alarme");
	}

	printf("O processo é eterno...\n");

	while(1);
	
	printf("Linha que nunca aparece...\n");

	return 0;
}