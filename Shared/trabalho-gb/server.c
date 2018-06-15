#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <signal.h>

// Nome da fila para comunicação server -> client 
const char* FILA_SERVER_CLI = "/fila_server-cli3";
// Nome da fila para comunicação client -> server
const char* FILA_CLI_SERVER = "/fila_cli-server3";

typedef struct Jogador {
	pid_t pid;
	char nickname[128];
} TJogador;

typedef struct RespostaRequisicao {
	int codigo;
} TResposta;

typedef struct Jogada {
	int x;
	int y;
} TJogada;

ssize_t get_msg_buffer_size(mqd_t queue);
void print_jogador(TJogador* m);
void iniciar_jogo();
int converter_jogada(int x, int y);

TJogador* jogadores[2];
int tabuleiro[9] = { -1 };

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
		queue = mq_open(FILA_SERVER_CLI, O_WRONLY | O_CREAT, 0777, NULL);
		if (queue == (mqd_t) -1) {
			perror("mq_open");
			exit(2);
		}

		TResposta resposta;
		resposta.codigo = 1; // * Jogador logado

		if (mq_send(queue, (const char*) &resposta, sizeof(TResposta), 29) != 0) {
			perror("erro ao enviar mensagem para o cliente");
		}

		qtdJogadores++;

		mq_close(queue);
	}

	printf("Jogadores logados. Iniciando jogo...\n");
	iniciar_jogo();
}

void iniciar_jogo()
{
	int index_jogador = 0;
	mqd_t queue;
	char* buffer = NULL;
	ssize_t tam_buffer;
	ssize_t nbytes;	

	while(1)
	{
		TJogador* jogador_atual = jogadores[index_jogador];		
		kill(jogador_atual->pid, 10);

		queue = mq_open(FILA_CLI_SERVER, O_RDONLY | O_CREAT, 0777, NULL);
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

		TJogada *jogada = (TJogada*) buffer;

		int index = converter_jogada(jogada->x, jogada->y);
		tabuleiro[index] = index_jogador;
		printf("--------- %d \n ", tabuleiro[index]);

		printf("X = %d\n", jogada->x);
		printf("Y = %d\n", jogada->y);

		index_jogador = !index_jogador;
	}
}

// Converte as coordenadas da jogada para o indíce do tabuleiro
int converter_jogada(int x, int y)
{
	if(x == 1) {
		return y - 1;
	}

	if(x == 2) {
		return (y + 3) - 1;
	}
	
	return (y + 6) - 1;	
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