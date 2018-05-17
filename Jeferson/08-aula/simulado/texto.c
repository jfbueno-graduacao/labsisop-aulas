#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>

char *glob_palavra;

void tratador(int signumber) {
	char *palavra = glob_palavra;
	int vogais = 0;
	for(int i=0; i<strlen(palavra); i++) {
		if(palavra[i] == 'A' || palavra[i] == 'E' ||
			palavra[i] == 'I' || palavra[i] == 'O' || palavra[i] == 'U' ||
			palavra[i] == 'a' || palavra[i] == 'e' ||
			palavra[i] == 'i' || palavra[i] == 'o' || palavra[i] == 'u') {
				vogais++;
			}
	}

	printf("A palavra %s tem %d vogais\n", palavra, vogais);
	exit(1);
}

void tratador2(int signumber) {
	char *palavra = glob_palavra;
	int alg = 0;
	for(int i=0; i<strlen(palavra); i++) {
		if(palavra[i] > 47 && palavra[i] < 58) {
				alg++;
			}
	}

	printf("A palavra %s tem %d algarismos\n", palavra, alg);
	exit(1);
}

int main(int argc, char **argv) {

    if(argc == 1) {
        printf("Parâmetro não informado\n\n");
        exit(1);
    }

    printf("PID = %d - Palavra = %s\nEnvie SIGUSR1 para contar as vogais e SIGUSR2 para contar os dígitos da palavra\n\n", getpid(), argv[1]);

	glob_palavra=argv[1];

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = &tratador;

    if(sigaction(SIGUSR1, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	struct sigaction sa2;
	memset(&sa2, 0, sizeof(sa2));

	sa2.sa_handler = &tratador2;

    if(sigaction(SIGUSR2, &sa2, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

	while(1);

	return 0;
}