

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include <sys/wait.h>

//Cristiano Farias, Jéferson Bueno e Mario Coin

pid_t pids_filhos[3];
int status_filhos[3];
char * status[3];
int time = 5;
int menuAtivo = 0;


int menu();
int menuText();

void tratador(int signal) {
    for (int i = 0; i < 3; i++) {
        int status = 0;
        pid_t return_pid = waitpid(pids_filhos[i], &status, WNOHANG);

        if (return_pid == -1) {
            //perror("Erro ao checar status de processo filho\n");
        } else if (return_pid == 0) {
            // Processo continua executando
        } else if (return_pid == pids_filhos[i]) {
            // O processo foi abortado por fora do sistema
            pids_filhos[i] = 0;
            status_filhos[i] = -1;
        }
    }
    switch (signal) {
        case 14:
            if (menuAtivo == 0) {
                menuText();
            }
            break;
    }

}

void checkStatus() {
    int i;
    for (i = 0; i < 3; i++) {
        switch (status_filhos[i]) {
            case -2:
                status[i] = "Falha ao finalizar";
                break;
            case -1:
                status[i] = "Abortado";
                break;
            case 0:
                status[i] = "Concluido";
                break;
            case 1:
                status[i] = "Executando";
                break;
            case 100:
                status[i] = "";
                break;
        }
    }

}

int menuText() {
    checkStatus();
    printf("\n<<<< Applications Menu >>>\n\t"
            "1) Web Brswser\t\t\t(%s, pid=%d)\n\t"
            "2) Text Editor\t\t\t(%s, pid=%d)\n\t"
            "3) Terminal\t\t\t(%s, pid=%d)\n\t"
            "4) Finalizar processo\t\t(concluído)\n\t"
            "5) Quit\n"
            "Opção: ", status[0], pids_filhos[0], status[1], pids_filhos[1], status[2], pids_filhos[2]);
    return 1;
}

int menu() {
    int op = -1;
    menuText();

    //desativado, não foi possivel ajustar a tempo.
    //    //agenda SIGALRM para daqui a 5 segundos
    //    if (alarm(time) < 0) {
    //        perror("Falha ao agendar alarme");
    //    }
    //Lê do teclado a opção
    scanf("%i", &op);
    return op;
}

int webBrowser(char *url) {
    execlp("firefox", "firefox --new-window", url, NULL);
    return 1;
}

int textEditor(char *name) {
    //Iniciar editor de texto aqui.
    execlp("gedit", "gedit", name, NULL);
    return 1;
}

int terminal() {
    //Iniciar terminal aqui.
    execlp("gnome-terminal", "gnome-terminal", NULL, NULL);
    return 1;
}

int finalizar() {
    int opScan = 0;
    while (opScan != 4) {
        //Opções dinâmicas de término
        if (pids_filhos[0] != -1) {
            printf("-> Opção 1: WebBrowser\n");
        }
        if (pids_filhos[1] != -1) {
            printf("-> Opção 2: Text Editor\n");
        }
        if (pids_filhos[2] != -1) {
            printf("-> Opção 3: Terminal\n");
        }
        printf("-> Opção 4: Sair\n");

        printf("Digite o codigo do processo que será encerrado: ");
        scanf("%i", &opScan);
        switch (opScan) {
            case 1:
                if (pids_filhos[0] != -1) {
                    kill(pids_filhos[0], 15);
                    pids_filhos[0] = 0;
                    opScan = 4;
                } else {
                    printf("Este processo não esta em execução\n");
                }
                break;
            case 2:
                if (pids_filhos[1] != -1) {
                    kill(pids_filhos[1], 15);
                    pids_filhos[1] = 0;
                    opScan = 4;
                } else {
                    printf("Este processo não esta em execução\n");
                }
                break;
            case 3:
                if (pids_filhos[2] != -1) {
                    kill(pids_filhos[2], 15);
                    pids_filhos[2] = 0;
                    opScan = 4;
                } else {
                    printf("Este processo não esta em execução\n");
                }
                break;
            case 4:
                break;
            default:
                printf("\nOpção inválida\n");
                break;
        }

    }
    return 0;

}

/*Inicia os chamados do menu*/
int controller() {

    struct sigaction sa;
    memset(&sa, 0, sizeof (sa));

    sa.sa_handler = &tratador;

    if (sigaction(SIGCHLD, &sa, NULL) != 0) {
        perror("Falha ao instalar tratador de sinal");
        exit(-1);
    }

    if (sigaction(SIGINT, &sa, NULL) != 0) {
        perror("Falha ao instalar tratador de sinal");
        exit(-1);
    }
    if (sigaction(SIGALRM, &sa, NULL) != 0) {
        perror("Falha ao instalar tratador de sinal");
        exit(-1);
    }



    while (1) {
        int op = menu();
        pid_t pid;

        //Web Browser
        if (op == 1 && pids_filhos[0] == 0) {
            char url[300];
            printf("Digite uma URL -> ");
            scanf("%s", url);
            if ((pid = fork()) == 0) {
                status_filhos[0] = webBrowser(url);
                exit(13);
            } else if (pid > 0) {
                pids_filhos[0] = pid;
            }
            status_filhos[0] = 1;
        }//Text Editor
        else if (op == 2 && pids_filhos[1] == 0) {
            char name[100];
            printf("Digite um nome de arquivo -> ");
            scanf("%s", name);
            if ((pid = fork()) == 0) {
                textEditor(name);
                exit(13);
            } else if (pid > 0) {
                pids_filhos[2] = pid;
            }
            status_filhos[1] = 1;
        }//Terminal
        else if (op == 3 && pids_filhos[2] == 0) {
            if ((pid = fork()) == 0) {
                terminal();
                exit(13);
            } else if (pid > 0) {
                pids_filhos[2] = pid;
            }
            status_filhos[2] = 1;
        } else if (op == 4) {
            finalizar();
        } else if (op == 5) {
            exit(13);
        } else {
            printf("\nOpção invalida ou não é possivel ser executada no momento!\n");
        }
    }
}

/*Metodo principal*/
int main(int argc, char *W[]) {
    status_filhos[0] = 100;
    status_filhos[1] = 100;
    status_filhos[2] = 100;
    controller();
}




