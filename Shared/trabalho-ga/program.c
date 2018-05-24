#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pids_filhos[3];
int status_filhos[3];
/* Status de terminacao dos filhos
   -2 - Falha ao finalizar
   -1 - Abortado
    0 - Concluído
    1 - Executando 
*/


void processar_entrada(int opcao);
int mostrar_menu();
void iniciar_processo(int chave);
void tratador(int signal);

void tratador(int signal)
{
    for(int i = 0; i < 3; i++) {
        int status = 0;
        pid_t return_pid = waitpid(pids_filhos[i], &status, WNOHANG);
        
        if (return_pid == -1) {
            perror("Erro ao checar status de processo filho\n"); 
        } else if (return_pid == 0) {
            // Processo continua executando
        } else if (return_pid == pids_filhos[i]) {
            // O processo foi abortado por fora do sistema
            pids_filhos[i] = 0;
            status_filhos[i] = -1;
        }
    }
}

int main() 
{
    struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = &tratador;

    if(sigaction(SIGCHLD, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

    if(sigaction(SIGINT, &sa, NULL) != 0){
		perror("Falha ao instalar tratador de sinal");
		exit(-1);
	}

    while(1) 
    {
        int entrada = mostrar_menu();

        if (entrada < 1 || entrada > 5) {
            printf("Opcao inválida\n");
        }
        else {
            processar_entrada(entrada);
            sleep(5);
        }
    }
}

int mostrar_menu() 
{
    printf("<<< Applications Menu >>> \n \
            \n\t 1) Web Browser \t (STATUS, pid=%d) \
            \n\t 2) Text Editor \t (STATUS, pid=%d) \
            \n\t 3) Terminal \t\t (STATUS, pid=%d) \
            \n\t 4) Finalizar Processo \
            \n\t 5) Quit\n\n",
            pids_filhos[0], pids_filhos[1], pids_filhos[2]);
    
    int opcao = 0;
    fflush(stdin);
    scanf("%d", &opcao);
    return opcao;
}

void processar_entrada(int entrada)
{
    if(entrada > 0 && entrada < 4) {
        iniciar_processo(--entrada);
        return;
    }
    
    switch(entrada)
    {
        case 4: ;
            int opcao_encerrar;

            do {
                printf("Qual aplicacao deve ser encerrada?\n\n1) Web Browser\n2) Text editor\n3) Terminal\n");
                scanf("%d", &opcao_encerrar);
            }
            while(opcao_encerrar < 1 || opcao_encerrar > 3);

            pid_t pid_enc = pids_filhos[--opcao_encerrar];
            
            if(pid_enc == 0) {
                printf("A aplicacão não está sendo executada\n\n");
                return;
            }

            int kill_ret = kill(pid_enc, 15); //15 -> SIGTERM
            if(kill_ret < 0){
                status_filhos[opcao_encerrar] = -2; //Falha ao finalizar
            }

            break;
        case 5:
            printf("Finalizando!!!\n\n");
            exit(0);
            break;
    }
}

void iniciar_processo(int chave)
{
    char url[256];
    if(chave == 0) {
        printf("Digite a URL: ");
        scanf("%s", url);
    }

    pid_t pid_filho;

    switch(chave)
    {
        case 0:
            if((pid_filho = fork()) == 0) {
                execlp("firefox", "firefox --new-window", url, NULL);
            }else {
                pids_filhos[0] = pid_filho;
            }
            break;
        case 1:
            if((pid_filho = fork()) == 0) {
                execlp("gedit", "gedit", "novo-arquivo", NULL);
            }else {
                pids_filhos[1] = pid_filho;
            }
            
            break;
        case 2:
            if((pid_filho = fork()) == 0) {
                execlp("gnome-terminal", "gnome-terminal", NULL, NULL);
            }else {
                pids_filhos[2] = pid_filho;
            }
            
            break;
    }
}
