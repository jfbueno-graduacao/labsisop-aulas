#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void start_firefox(char *url) {
    pid_t pid = fork(); 
    int status;

    if(pid < 0) {
        printf("Houve um erro na criacao do processo\n");
        exit(1);
    }else if(pid == 0) {
        execlp("firefox", "firefox", "--new-window", url, NULL);
    }else {
        printf("\nPID Pai: %d PID Filho: %d\n\n", getpid(), pid);
        pid = wait(&status);

        if(status != 0) {
            printf("Processo filho foi finalizado com status de não sucesso. Status retornado foi %d\n", WEXITSTATUS(status));
        }else {
            printf("Processo filho foi finalizado com sucesso\n");
        }
    }
}

int main(int argc, char **argv)
{
    if(argc == 1) {
        printf("É necessário informar a URL como parâmetro do programa\n");
        exit(1);
    }

    start_firefox(argv[1]);

    return 0;
}