#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void repete(char *palavra, int qtdRepeticoes);

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Por favor, entre com um (e apenas um) parâmetro da linha de comando\n");
        exit(1);
    }

    pid_t pid;
    int statusFilho;

    printf("Entre com um número inteiro: ");
    int entrada;
    scanf("%d", &entrada);

    pid = fork();

    if(pid < 0) {
        printf("Houve um erro na função fork()");
    } else if(pid == 0) {
        repete(argv[1], entrada);
        exit(0);
    }else {
        pid = wait(&statusFilho);
        printf("\nStatus de terminicão processo filho: %d", WEXITSTATUS(statusFilho));
        printf("\nPai terminou - PID: %d PID Pai: %d\n\n", getpid(), getppid());
    } 
}

void repete(char *palavra, int qtdRepeticoes) 
{
    if(qtdRepeticoes < 0) {
        exit(10);
    }

    if(qtdRepeticoes > 10) {
        exit(11);
    }

    for(int i=1; i<=qtdRepeticoes; i++) {
        printf("\n#%d '%s'\n\t*PID: %d PID Pai: %d*\n", i, palavra, getpid(), getppid());
        sleep(1);
    }
}