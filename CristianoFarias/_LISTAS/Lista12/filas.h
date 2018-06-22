#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>
int qtdFila1 = 0;
int qtdFila2 = 0;
typedef struct ClienteBanco
{
	int tipo;
	char nome[200];
} TClt;
const struct sigevent *d;

// Nome da fila para comunicação server -> client
char *FILA_1 = "/fila_1_banco";
// Nome da fila para comunicação client -> server
char *FILA_2 = "/fila_2_banco";

ssize_t get_msg_buffer_size(mqd_t queue)
{
	struct mq_attr attr;

	if (mq_getattr(queue, &attr) != -1)
	{
		return attr.mq_msgsize;
	}

	perror("get_msg_buffer_size");
	exit(3);
}

#define clear_console() printf("\033[H\033[J")

void writeFila(char nome[], int tipoAtendimento)
{

	char *fila;

	mqd_t queue;
	TClt msg;

	msg.tipo = tipoAtendimento;
	strncpy(msg.nome, nome, 200);
	if (tipoAtendimento == 1)
	{
		fila = FILA_1;
	}
	else
	{
		fila = FILA_2;
	}

	queue = mq_open(fila, O_WRONLY | O_CREAT, 0777, NULL);
	if (queue == (mqd_t)-1)
	{
		perror("mq_open");
		exit(2);
	}

	if (mq_send(queue, (const char *)&msg, sizeof(TClt), 29) != 0)
	{
		perror("erro ao enviar mensagem");
	}
	if (tipoAtendimento == 1)
	{
		qtdFila1++;
	}
	else
	{
		qtdFila2++;
	}

	printf("\nCHEGOU %s E RETIROU SENHA TIPO %s\n", msg.nome, msg.tipo == 1 ? "NORMAL" : "PRIORITARIO");
	mq_close(queue);
}

TClt readFila(int type)
{

	char *fila;

	if (type == 1)
	{
		fila = FILA_1;
	}
	else
	{
		fila = FILA_2;
	}
	mqd_t queue;

	char *buffer = NULL;
	ssize_t tam_buffer;
	ssize_t nbytes;

	queue = mq_open(fila, O_RDONLY | O_CREAT, 0777, NULL);
	if (queue == (mqd_t)-1)
	{
		perror("mq_open");
		exit(2);
	}

	tam_buffer = get_msg_buffer_size(queue);
	buffer = calloc(tam_buffer, 1);

	nbytes = mq_receive(queue, buffer, tam_buffer, NULL);

	if (nbytes == -1)
	{
		perror("receive");
		exit(4);
	}

	TClt *jogador = (TClt *)buffer;

	if (type == 1)
	{
		if (qtdFila1 > 0)
		{
			qtdFila1--;
		}
	}
	else
	{
		if (qtdFila2 > 0)
		{
			qtdFila2--;
		}
	}

	mq_close(queue);

	return *jogador;
}
