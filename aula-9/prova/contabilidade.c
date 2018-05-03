// Jéferson Bueno

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void abrir_filho();
void abrir_firefox();
void registrar_tratamento_sinais();
void tratador_usr1(int sinal);
void tratador_usr2(int sinal);

int main()
{    
    registrar_tratamento_sinais();

    float salario;

    do {
        printf("\nDigite o salário anual do colaborador: ");
        scanf("%f", &salario);
    }
    while(salario < 0 || salario > 100000);

    float salarioMensal = salario / 12;

    if(salarioMensal < 2000) {
        abrir_filho();
    }else {
        abrir_firefox();        
    }

    return 0;
}

void tratador_usr1(int sinal)
{
    printf("Sinal recebido. Aguardar a conclusão do processo filho!");
}

void tratador_usr2(int sinal)
{
    printf("Sinal recebido. Ainda em processamento...");
}

void registrar_tratamento_sinais() 
{
    struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = &tratador_usr1;

	if(sigaction(SIGUSR1, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

    struct sigaction sa2;
	memset(&sa2, 0, sizeof(sa2));
    sa2.sa_handler = &tratador_usr2;

	if(sigaction(SIGUSR2, &sa2, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	} 
}

void abrir_filho()
{
    pid_t pid_filho;
    if((pid_filho = fork()) == 0) 
    {
        printf("\nPID do processo simplificado (filho): %d - PID do processo pai: %d \n", getpid(), getppid());        
        execlp("./simplificado", "./simplificado", NULL);
    }
    else if(pid_filho < 0) 
    {
        printf("Houve um erro ao criar o processo filho para abrir o programa");
    } 
    else 
    {
        printf("Aguardando finalização do processo filho\n");
        printf("PID deste processo: %d - PID do processo pai: %d \n", getpid(), getppid());
        wait(NULL);
        printf("\n** Processo filho finalizado **\n");
    }
}

void abrir_firefox()
{
    pid_t pid_filho;
    if((pid_filho = fork()) == 0) 
    {
        printf("\nPID do processo filho: %d - PID do processo pai: %d \n", getpid(), getppid());
        execlp("firefox", "firefox", "--new-window", "www.receita.fazenda.gov.br", NULL);
    }
    else if(pid_filho < 0) 
    {
        printf("Houve um erro ao criar o processo filho para abrir o programa");
    } 
    else {
        printf("Aguardando finalização do processo filho\n");
        printf("PID deste processo: %d - PID do processo pai: %d \n", getpid(), getppid());
        wait(NULL);

        printf("\n** Processo filho finalizado **\n");
    }
}