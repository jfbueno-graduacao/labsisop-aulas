#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void load_firefox(char *url);

int main() {
    
}

void load_firefox(char *url) {
    execlp("firefox", "firefox", "--new-window", url, NULL);
}