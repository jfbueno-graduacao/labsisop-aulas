#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("%s", "Por favor, entre com um (e apenas um) parâmetro da linha de comando\n");
        exit(1);
    }

    printf("%s", "Entre com um número inteiro: ");
    int entrada;
    scanf("%d", &entrada);

    for(int i=1; i<=entrada; i++) {
        printf("#%d '%s'\n", i, argv[1]);
    }
}