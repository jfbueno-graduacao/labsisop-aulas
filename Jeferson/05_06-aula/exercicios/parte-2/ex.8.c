#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void load_firefox(char *url);

// Quando não tem nenhum processo do Firefox o novo processo é filho do bash.
// se já existir um processo aberto, o novo processo é filho do processo que está aberto.

int main() {
    load_firefox("http://stackoverflow.com");
}

void load_firefox(char *url) {
    execlp("firefox", "firefox", "--new-window", url, NULL);
}