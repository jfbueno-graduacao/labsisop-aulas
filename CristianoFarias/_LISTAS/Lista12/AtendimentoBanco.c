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

//NOMES: CRISTIANO FARIAS, JEFFERSON BUENO, MARIO COIN

int getRand(int start, int end)
{
	return start + (rand() % end);
}

int gerenciaFila(int atendente)
{
	if (qtdFila2 > 0)
	{
		TClt cliente = readFila(2);
		printf("CAIXA %i ATENDENDO FILA 2: %s\n", atendente, cliente.nome);
		sleep(getRand(5, 60));
		printf("\nCAIXA %i LIVRE\n", atendente);
		return 0;
	}
	else if (qtdFila1 > 0)
	{
		TClt cliente = readFila(1);
		printf("CAIXA %i ATENDENDO FILA 1: %s\n", atendente, cliente.nome);
		sleep(getRand(5, 60));
		printf("\nCAIXA %i LIVRE\n", atendente);
	}
	return 0;
}

void *Caixa(void *param)
{
	char *par;
	par = (char *)param;
	int type = atoi(par);
	while (1)
	{
		sleep(getRand(0, 3));
		gerenciaFila(type);
	}
	pthread_exit(NULL);
}

void *ClienteFila(void *param)
{
	char *par;
	par = (char *)param;
	int type = atoi(par);
	int contCliente = 1;
	while (1)
	{
		sleep(getRand(0, 13));
		char novoId[100];
		snprintf(novoId, 100, "%s %d", "Cliente ", contCliente++);
		writeFila(novoId, getRand(1, 2));
	}
	pthread_exit(NULL);
}

void iniciarClientes()
{
	pthread_t tids[1];
	int th1 = pthread_create(&tids[0], NULL, &ClienteFila, (void *)"1");
	if (th1)
	{
		errno = th1;
		perror("Falha na criação da thread\n");
		exit(EXIT_FAILURE);
	}
}

void iniciarAtendimento()
{
	//Iniciando Atendentes
	pthread_t tids[3];
	int th1 = pthread_create(&tids[0], NULL, &Caixa, (void *)"1");
	if (th1)
	{
		errno = th1;
		perror("Falha na criação da thread1\n");
		exit(EXIT_FAILURE);
	}

	int th2 = pthread_create(&tids[1], NULL, &Caixa, (void *)"2");
	if (th2)
	{
		errno = th2;
		perror("Falha na criação da thread\n");
		exit(EXIT_FAILURE);
	}

	int th3 = pthread_create(&tids[2], NULL, &Caixa, (void *)"3");
	if (th3)
	{
		errno = th3;
		perror("Falha na criação da thread\n");
		exit(EXIT_FAILURE);
	}
	pthread_exit(NULL);
}

int main()
{
	mq_unlink(FILA_1);
	mq_unlink(FILA_2);
	//Cadastrando fila dinamicamente e aleatoriamente:
	iniciarClientes();
	iniciarAtendimento();
}
