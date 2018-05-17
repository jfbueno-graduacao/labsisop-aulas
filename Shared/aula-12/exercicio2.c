//Cristiano Farias / Jéferson Bueno - Laboratorio de Sistemas - Lista10

#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int contarOcorrencia(char *texto, char *key);

void run(char *arquivo, char *chave)
{
    int aux;
    aux = open(arquivo, O_RDONLY);
    if (aux == -1)
    {
        printf("Arquivo '%s' não encontrado!, Finalizando.\n", arquivo);
        exit(0);
    }
    else
    {
        size_t size = 1000;
        char buf2[1000];
        size_t bt = read(aux, buf2, size);
        printf("Arquivo '%s' carregado , total de bytes -> %ld Bytes\n", arquivo, bt);
        printf("Numero de ocorrências da palavra '%s' encontradas no texto -> %i\n\n", chave, contarOcorrencia(buf2, chave));
    }
}

int contarOcorrencia(char *texto, char *key)
{
    int i, j, found, count;
    int tamanhoTexto = strlen(texto);
    int tamanhoChave = strlen(key);
    count = 0;
    for (i = 0; i <= tamanhoTexto - tamanhoChave; i++)
    {
        found = 1;
        for (j = 0; j < tamanhoChave; j++)
        {
            if (texto[i + j] != key[j])
            {
                found = 0;
                break;
            }
        }
        if (found == 1)
        {
            count++;
        }
    }

    return count;
}

int main(int argc, char *args[])
{
    if (argc <= 2)
    {
        printf("Passagem de parametros inválida informe 2 parametros ex: 'nomeArquivo.txt contarPalavra'!\n");
    }
    else
    {
        run(args[1], args[2]);
    }
}
