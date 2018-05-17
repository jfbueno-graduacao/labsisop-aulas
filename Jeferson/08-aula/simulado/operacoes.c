#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if(argc != 4) {
        printf("Quantidade de parâmetros inválida");
        exit(1);
    }

    for(int i=3; i > 0; i--) {
        system(argv[i]);
        sleep(5);
    }

    return 0;
}