#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void load_firefox(char *url);
void start_browser(char *url);

int main() {
    start_browser("http://stackoverflow.com");
}

void start_browser(char *url) {
    pid_t pid = fork(); 

    if(pid < 0) {
        printf("Houve um erro na criacao do processo");
        exit(1);
    }else if(pid == 0) {
        load_firefox(url);
    }else {
        wait(NULL);
    }
}

void load_firefox(char *url) {
    execlp("firefox", "firefox", "--new-window", url, NULL);
}