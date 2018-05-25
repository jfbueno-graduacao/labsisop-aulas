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
void print_jogador(TJogador* m);
void print2(int qtdJogadores, char* str);
void jogar();

TJogador* jogadores[2];

int main()
{
	mqd_t queue;
	TJogador jogador;

	char* buffer = NULL;
	ssize_t tam_buffer;
	ssize_t nbytes;	

	int qtdJogadores = 0;

	while(qtdJogadores < 2) 
	{
		print2(qtdJogadores, "11111\n\n");
		queue = mq_open(FILA_CLI_SERVER, O_RDONLY | O_CREAT, 0660, NULL);
		if (queue == (mqd_t) -1) {
			perror("mq_open");
			exit(2);
		}

		tam_buffer = get_msg_buffer_size(queue);		
		buffer = calloc(tam_buffer, 1);

		nbytes = mq_receive(queue, buffer, tam_buffer, NULL);

		if (nbytes == -1) {
			perror("receive");
			exit(4);
		}

		TJogador *jogador = (TJogador*) buffer;
		print_jogador(jogador);
		jogadores[qtdJogadores] = jogador;

		mq_close(queue);

		// Responder cliente que está ok
		queue = mq_open(FILA_SERVER_CLI, O_WRONLY | O_CREAT, 0660, NULL);
		if (queue == (mqd_t) -1) {
			perror("mq_open");
			exit(2);
		}

		TJogada j;
		j.x = 12;
		j.y = 13;

		if (mq_send(queue, (const char*) &j, sizeof(TJogada), 29) != 0) {
			perror("erro ao enviar mensagem para o cliente");
		}

		qtdJogadores++;
		print2(qtdJogadores, "2222\n\n");

		mq_close(queue);
	}

	printf("Jogadores logados. Iniciando jogo...\n");	
}

void print2(int qtdJogadores, char* str)
{
	printf("%d -- %s", qtdJogadores, str);
}


void print_jogador(TJogador *m) {
	printf("ID %d", m->pid);
	printf("\nNickname: %s\n", m->nickname);
}

ssize_t get_msg_buffer_size(mqd_t queue) {
	struct mq_attr attr;

	if (mq_getattr(queue, &attr) != -1) {
		return attr.mq_msgsize;
	}

	perror("get_msg_buffer_size");
	exit(3);
}