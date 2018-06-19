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
void desenhar_tabuleiro();
char mapear_jogada(int item);
int verificar_vencedor();

TJogador* jogadores[2];
int tabuleiro2[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

int main()
{
	mqd_t queue;
	TJogador jogador;

	char* buffer = NULL;
	ssize_t tam_buffer;
	ssize_t nbytes;	

	int qtdJogadores = 0;
	int mjogador = 1;

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
		jogador->multiplicador = (mjogador += 2);
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
		kill(jogador_atual->pid, SIGUSR1);

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

		int x = jogada->x - 1;
		int y = jogada->y - 1;

		// Se a jogada for inválida. Avisar o player via SIGUSR2 
		// e esperar uma nova jogada dele mesmo
		if(tabuleiro2[x][y] != 0) {
			printf("\t *** Jogada inválida recebida ***\n");
			kill(jogador_atual->pid, SIGUSR2);
			continue;
		}

		tabuleiro2[x][y] = jogador_atual->multiplicador;
		index_jogador = !index_jogador;

		printf("Jogada recebida\n");
		desenhar_tabuleiro();
		printf("\n");

		int venc = verificar_vencedor();

		if(venc == -2) {
			printf("Ocorreu um empate. Finalizando o jogo\n");
		} else if (venc == -1) {
			continue;
		}

		printf("##### Parabéns, %s!!!! Você venceu! #####\n", jogadores[venc]->nickname);
	}
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
    printf("  1     %c  |  %c  |  %c \n", mapear_jogada(tabuleiro2[0][0]), mapear_jogada(tabuleiro2[0][1]), mapear_jogada(tabuleiro2[0][2]));

    printf("      _____|_____|_____\n");
    printf("           |     |     \n");

    printf("  2     %c  |  %c  |  %c \n", mapear_jogada(tabuleiro2[1][0]), mapear_jogada(tabuleiro2[1][1]), mapear_jogada(tabuleiro2[1][2]));

    printf("      _____|_____|_____\n");
    printf("           |     |     \n");

    printf("  3     %c  |  %c  |  %c \n", mapear_jogada(tabuleiro2[2][0]), mapear_jogada(tabuleiro2[2][1]), mapear_jogada(tabuleiro2[2][2]));

    printf("           |     |     \n\n");
}

// Mapeia um item da matriz que controla as jogadas para 
// o símbolo referente àquela jogada
char mapear_jogada(int item)
{
	if(item == jogadores[0]->multiplicador) {
		return 'X';
	}

	if(item == jogadores[1]->multiplicador) {
		return 'O';
	}

	return ' ';
}

// Verifica se há um vencedor e, se houver, retorna o indíce do mesmo no array de jogadores
// Caso contrário retorn -1 se o jogo ainda não terminou ou -2 se for um empate
int verificar_vencedor()
{
	int somas_colunas[3] = { 0, 0, 0 };
	int somas_diagonais[2] = { 0, 0 };
	// Estas variáveis servem para guardar os valores das somas das colunas
	// e das duas diagonais. Assim, só é preciso passar uma vez pela matriz para
	// verificar se há um ganhador

	int m0 = jogadores[0]->multiplicador * 3;
	int m1 = jogadores[1]->multiplicador * 3;
	int qtd_cadas_disponiveis = 0;

	printf("M0 == %d\n", m0);
	printf("M1 == %d\n", m1);

	for(int linha = 0; linha < 3; linha++) {
		int soma_linha = 0;

		for(int coluna = 0; coluna < 3; coluna++) {
			int valor = tabuleiro2[linha][coluna];

			soma_linha += valor;
			somas_colunas[coluna] += valor;

			if(valor == 0) {
				qtd_cadas_disponiveis++;
			}

			if(soma_linha == m0 || somas_colunas[coluna] == m0) {
				printf("JOGADOR UM GANHOU ESSA MERDA\n");
				return 0;
			}else if(soma_linha == m1 || somas_colunas[coluna] == m1) {
				printf("JOGADOR DOIS GANHOU ESSA PORRA\n");
				return 1;
			}
		}
	}

	if(qtd_cadas_disponiveis == 0) {
		return -2;
	}

	return -1;
}