#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

// Nome da fila para comunicação server -> client 
const char* FILA_SERVER_CLI = "/fila_server-cli3";
// Nome da fila para comunicação client -> server
const char* FILA_CLI_SERVER = "/fila_cli-server3";

typedef struct Jogador {
	int pid;
	char nickname[128];
} TJogador;

typedef struct Jogada {
	int x;
	int y;
} TJogada;

ssize_t get_msg_buffer_size(mqd_t queue);
void print(TJogada *m);

int main() 
{
	mqd_t queue;
	TJogador msg;
	
	char nick[128];
	printf("%s", "Entre com o seu nick: ");
	scanf("%s", nick);

	msg.pid = 123;
	strncpy(msg.nickname, nick, 127);

	queue = mq_open(FILA_CLI_SERVER, O_WRONLY | O_CREAT, 0660, NULL);
	if (queue == (mqd_t) -1) {
		perror("mq_open");
		exit(2);
	}

	if (mq_send(queue, (const char*) &msg, sizeof(TJogador), 29) != 0) {
		perror("erro ao enviar mensagem para servidor");
	}

	mq_close(queue);
	printf("Pedido para entrar no jogo enviado!\n");
	
	queue = mq_open(FILA_SERVER_CLI, O_RDONLY | O_CREAT, 0660, NULL);
	if (queue == (mqd_t) -1) {
		perror("mq_open");
		exit(2);
	}

	ssize_t tam_buffer = get_msg_buffer_size(queue);		
	char* buffer = calloc(tam_buffer, 1);

	ssize_t nbytes = mq_receive(queue, buffer, tam_buffer, NULL);
	
	if (nbytes == -1) {
		perror("receive");
		exit(4);
	}

	TJogada* resposta = (TJogada*) buffer;
	print(resposta);
	
}

void print(TJogada *m) {
	printf("x %d", m->x);
	printf("y %d", m->y);
}

ssize_t get_msg_buffer_size(mqd_t queue) 
{
	struct mq_attr attr;

	if (mq_getattr(queue, &attr) != -1) {
		return attr.mq_msgsize;
	}

	perror("get_msg_buffer_size");
	exit(3);
}

void jogar()
{

}