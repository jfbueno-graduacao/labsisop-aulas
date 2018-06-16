#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <signal.h>
#include "common.h"

void msg_jogador_conectado(TJogador* m);
void iniciar_jogo();
int converter_jogada(int x, int y);
void desenhar_tabuleiro();
char mapear_jogada(int item);

TJogador* jogadores[2];
int tabuleiro[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };

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
		msg_jogador_conectado(jogador);
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

	printf("Jogadores logados. Iniciando jogo...\n\n");
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

		printf("Vez de %s\n\n", jogador_atual->nickname);

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

		// Se a jogada for inválida. Avisar o player via SIGUSR2 
		// e esperar uma nova jogada dele mesmo
		if(tabuleiro[index] != -1) {
			printf("\t *** Jogada inválida recebida ***\n");
			kill(jogador_atual->pid, SIGUSR2);
			continue;
		}

		tabuleiro[index] = index_jogador;
		index_jogador = !index_jogador;

		printf("Jogada recebida\n");
		desenhar_tabuleiro();
		printf("\n\n");
	}
}

// Converte as coordenadas da jogada para o indíce do tabuleiro
int converter_jogada(int x, int y)
{
	if(x == 1) {
		return y - 1;
	}

	if(x == 2) {
		return (3 + y) - 1;
	}
	
	return (6 + y) - 1;	
}

void msg_jogador_conectado(TJogador *jogador) 
{
	printf("%s entrou no jogo. ID %d\n", jogador->nickname, jogador->pid);
}

void desenhar_tabuleiro()
{
    printf("[X] %s\n[O] %s\n\n", jogadores[0]->nickname, jogadores[1]->nickname);

    printf("        1     2     3  \n");
    printf("                       \n");
    printf("           |     |     \n");
    printf("  1     %c  |  %c  |  %c \n", mapear_jogada(tabuleiro[0]), mapear_jogada(tabuleiro[1]), mapear_jogada(tabuleiro[2]));

    printf("      _____|_____|_____\n");
    printf("           |     |     \n");

    printf("  2     %c  |  %c  |  %c \n", mapear_jogada(tabuleiro[3]), mapear_jogada(tabuleiro[4]), mapear_jogada(tabuleiro[5]));

    printf("      _____|_____|_____\n");
    printf("           |     |     \n");

    printf("  3     %c  |  %c  |  %c \n", mapear_jogada(tabuleiro[6]), mapear_jogada(tabuleiro[7]), mapear_jogada(tabuleiro[8]));

    printf("           |     |     \n\n");
}

// Mapeia um item da matriz que controla as jogadas para 
// o símbolo referente àquela jogada
char mapear_jogada(int item)
{
	switch(item) {
		case -1:
			return ' ';
		case 0:
			return 'X';
		case 1:
			return 'O';
	}
}