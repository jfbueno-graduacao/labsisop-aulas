/*
 Cristiano Costa Farias;
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>


pid_t op1 = -1;
pid_t op2 = -1;
pid_t op3 = -1;

int menu() {
    int op = -1;
    printf("<<<< Applications Menu >>>\n\t1) Web Browser\t\t\t(executando, pid=%d)\n"
            "\t2) Text Editor\t\t\t(executando, pid=%d)\n\t3) Terminal\t\t\t(falhou)\n"
            "\t4) Finalizar processo\t\t(concluído)\n\t5) Quit\n"
            "Opção: ", op1, op2);
    scanf("%i", &op);
    return op;
}

int webBrowser(char *url) {
    execlp("firefox", "firefox --new-window", url, NULL);
    return 1;
}

int textEditor(char *name) {
    //Iniciar editor de texto aqui.
    printf("Falta iniciar editor de texto! ");
    return 1;
}

int terminal() {
    //Iniciar terminal aqui.
    printf("Falta iniciar terminal!");
    return 1;
}

int finalizar() {
    int opScan = 0;
    while (opScan != 4) {
        printf("-> Opção 1: WebBrowser\n-> Opção 2: Text Editor\n-> Opção 3: Terminal\n-> Opção 4: Sair\n");
        printf("Digite o codigo do processo que será encerrado: ");
        scanf("%i", &opScan);
        switch (opScan) {
            case 1:
                if (op1 != -1) {
                    kill(op1, 15);
                    op1 = -1;
                } else {
                    printf("Este processo não esta em execução\n");
                }
                break;
            case 2:
                if (op2 != -1) {
                    kill(op2, 15);
                    op2 = -1;
                } else {
                    printf("Este processo não esta em execução\n");
                }
                break;
            case 3:
                if (op3 != -1) {
                    kill(op3, 15);
                    op3 = -1;
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
    while (1) {
        int op = menu();
        pid_t pid;
        //if (check(pid) == 0) {
        if (op == 1) {
            char url[300];
            printf("Digite uma URL -> ");
            scanf("%s", url);
            if ((pid = fork()) == 0) {
                webBrowser(url);
                exit(13);
            } else if (pid > 0) {
                op1 = pid;
            }
        } else if (op == 2) {
            char name[100];
            printf("Digite um nome de arquivo -> ");
            scanf("%s", name);
            if ((pid = fork()) == 0) {
                textEditor(name);
                exit(13);
            } else if (pid > 0) {
                op2 = pid;
            }
        } else if (op == 3) {
            //falta terminal
        } else if (op == 4) {
            finalizar();
        } else if (op == 5) {
            exit(13);
        } else {
            printf("\nOpção invalida!\n");
        }
        //} else {

        //}
    }
}

/*Metodo principal*/
int main(int argc, char *W[]) {


    controller();
}




