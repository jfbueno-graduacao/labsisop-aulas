#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void load_firefox(char *url);
void start_browser(char *url);

int main() {
    char url[128];

    while(1) {
        printf("Entre com uma URL (ou Ctrl + C para encerrar): ");
        scanf("%s", url);

        start_browser(url);
    }    
}

void start_browser(char *url) {
    pid_t pid = fork(); 

    if(pid < 0) {
        printf("Houve um erro na criacao do processo");
        exit(1);
    }else if(pid == 0) {
        load_firefox(url);
        printf("Fim?");
    }
}

void load_firefox(char *url) {
    execlp("firefox", "firefox", "--new-window", url, NULL);
}