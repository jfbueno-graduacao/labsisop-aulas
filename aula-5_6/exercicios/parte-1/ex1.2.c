#include <stdio.h>
#include <stdlib.h>

void repete(char *palavra, int qtdRepeticoes);

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

    repete(argv[1], entrada);
}

void repete(char *palavra, int qtdRepeticoes) 
{
    for(int i=1; i<=qtdRepeticoes; i++) {
        printf("#%d '%s'\n", i, palavra);
    }
}