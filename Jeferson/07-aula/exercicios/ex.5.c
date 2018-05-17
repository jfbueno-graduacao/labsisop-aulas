#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sinal_chegou(int sinal)
{
    printf("\t** Recebido sinal %s **\n", strsignal(sinal));
}

int main()
{
    //sigset_t mask;
	struct sigaction sig_ac;

    memset(&sig_ac, 0, sizeof(sig_ac));

    sig_ac.sa_handler = &sinal_chegou;

    if(sigaction(SIGINT, &sig_ac, NULL) == -1){
		perror("Falha em sigaction");
		exit(-1);
	}

    while(1) {
        printf("# Meu PID=%d - Aguardando sinal SIGINT para tratá-lo\n", getpid());
        sleep(2);
    }

    return 0;
}