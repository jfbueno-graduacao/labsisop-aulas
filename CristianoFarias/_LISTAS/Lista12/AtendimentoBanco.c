#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>
#include "filas.h"
#define clear_console() printf("\033[H\033[J")
//gcc AtendimentoBanco.c -o AtendimentoBanco -l  pthread -l rt

void gerenciaFila(int atendente)
{
	if (atendente == 2)
	{
		TClt cliente = readFila(2);
		printf("ATENDIDO PELO ATENDENTE 2 NA FILA 2: %s\n", cliente.nome);
	}
	else
	{
	}
}

void *Caixa(void *param)
{
	char *par;
	par = (char *)param;
	int type = atoi(par);
	while (1)
	{
		sleep(7);
		gerenciaFila(type);
	}
	pthread_exit(NULL);
}

void iniciarAtendimento()
{
	//Iniciando Atendentes
	pthread_t tids[3];
	int th1 = pthread_create(&tids[0], NULL, &Caixa, (void *)"1");
	if (th1)
	{
		errno = th1;
		perror("Falha na criação da thread da Parte 1\n");
		exit(EXIT_FAILURE);
	}

	int th2 = pthread_create(&tids[1], NULL, &Caixa, (void *)"2");
	if (th2)
	{
		errno = th2;
		perror("Falha na criação da thread da Parte 1\n");
		exit(EXIT_FAILURE);
	}

	int th3 = pthread_create(&tids[2], NULL, &Caixa, (void *)"3");
	if (th3)
	{
		errno = th3;
		perror("Falha na criação da thread da Parte 1\n");
		exit(EXIT_FAILURE);
	}
	pthread_exit(NULL);
}

int main()
{
	iniciarAtendimento();
}
