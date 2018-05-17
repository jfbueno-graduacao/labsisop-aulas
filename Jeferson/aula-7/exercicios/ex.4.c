#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void start_firefox(char *url) {
    pid_t pid = fork(); 

    if(pid < 0) {
        printf("Houve um erro na criacao do processo");
        exit(1);
    }else if(pid == 0) {
        execlp("firefox", "firefox", "--new-window", url, NULL);
    }
}

void sig_handler(int signal)
{
    char url[256];
    printf("Entre com a URL desejada: ");
    scanf("%s", url);

    printf("Navegue a vontade em %s usando o Firefox.\nContinuarei aguardando por novos sinais =)\n", url);

    start_firefox(url);
}

int main()
{
    sigset_t mask;
	struct sigaction action;

    memset(&action, 0, sizeof(action));

    action.sa_handler = &sig_handler;

    if(sigaction(SIGUSR2, &action, NULL) == -1){
		perror("Falha em sigaction");
		exit(-1);
	}

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    while(1) {
        printf("# Meu PID=%d - Aguardando sinal SIGUSR2\n", getpid());
        sleep(2);
    }

    return 0;
}