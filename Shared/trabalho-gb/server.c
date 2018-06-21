#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <signal.h>
#include <time.h>
#include "common.h"

void msg_jogador_conectado(TJogador* m);
void iniciar_server();
void iniciar_jogo();
void desenhar_tabuleiro();
char mapear_jogada(int item);
int verificar_vencedor();
void enviar_msg_queue(const char* mensagem);
char* ler_msg_queue();
void finalizar_jogo(int vencedor);
void escrever_log_jogada(int i_jogador, int x, int y, int valida);

TJogador* jogadores[2];
int tabuleiro2[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
int fd_log;

int main()
{
    iniciar_server();

    printf("Aguardando por jogadores\n-----------------------------\n");

	TJogador jogador;

	int qtdJogadores = 0;
	int mjogador = 1;

	while(qtdJogadores < 2) 
	{
		char *buffer = ler_msg_queue();

		TJogador *jogador = (TJogador*) buffer;
		jogador->multiplicador = (mjogador += 2);

		msg_jogador_conectado(jogador);
		jogadores[qtdJogadores] = jogador;

		TMensagem resposta;
		resposta.codigo = 1; // 1 = Jogador logado

		// Responder cliente que está ok
		enviar_msg_queue((const char*) &resposta);

		qtdJogadores++;
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

		char* buffer = ler_msg_queue();

		TJogada *jogada = (TJogada*) buffer;

		int x = jogada->x - 1;
		int y = jogada->y - 1;

		// Se a jogada for inválida. Avisar o player via SIGUSR2 
		// e esperar uma nova jogada dele mesmo
		if(tabuleiro2[x][y] != 0) {

            escrever_log_jogada(index_jogador, jogada->x, jogada->y, 0);
			printf("\t *** Jogada inválida recebida ***\n");

			TMensagem mensagem;
			mensagem.codigo = 2; //Jogada inválida
			enviar_msg_queue((const char*) &mensagem);

			kill(jogador_atual->pid, SIGUSR2);
			continue;
		}

        escrever_log_jogada(index_jogador, jogada->x, jogada->y, 1);

		tabuleiro2[x][y] = jogador_atual->multiplicador;
		index_jogador = !index_jogador;

		printf("Jogada recebida\n");
		desenhar_tabuleiro();
		printf("\n");

		int venc = verificar_vencedor();

        if(venc == -1) { // ainda nao terminou
            continue;
        }

        finalizar_jogo(venc);
	}
}

void finalizar_jogo(int vencedor)
{
    if(vencedor == -2) {
        printf("Ocorreu um empate. Finalizando o jogo\n");

        // Mandar msg de empate para os dois jogadores
        for(int i = 0; i < 2; i++) {
            TMensagem msg;
            msg.codigo = 3;
            enviar_msg_queue((const char*) &msg);

            kill(jogadores[i]->pid, SIGUSR2);
            usleep(500);
        }

        exit(0);
    }

    printf("##### Parabéns, %s!!!! Você venceu! #####\n", jogadores[vencedor]->nickname);

    TMensagem msg;
    msg.codigo = 4;
    enviar_msg_queue((const char*) &msg);
    kill(jogadores[vencedor]->pid, SIGUSR2);

    int iperdedor = !vencedor;

    usleep(100);

    msg.codigo = 5;
    enviar_msg_queue((const char*) &msg);
    kill(jogadores[iperdedor]->pid, SIGUSR2);

    close(fd_log);
    exit(0);
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

	for(int linha = 0; linha < 3; linha++) {
		int soma_linha = 0;

		for(int coluna = 0; coluna < 3; coluna++) {
			int valor = tabuleiro2[linha][coluna];

			soma_linha += valor;
			somas_colunas[coluna] += valor;

			if(linha == coluna) {
				somas_diagonais[0] += valor; 
			}

            if((linha + 1) + (coluna + 1) == 4) {
                somas_diagonais[1] += valor;
            }

			if(valor == 0) {
				qtd_cadas_disponiveis++;
			}

			if( soma_linha == m0 || somas_colunas[coluna] == m0 || 
				somas_diagonais[0] == m0 || somas_diagonais[1] == m0) {
				return 0;
			}else if(soma_linha == m1 || somas_colunas[coluna] == m1 || 
					 somas_diagonais[0] == m1 || somas_diagonais[1] == m1) {
				return 1;
			}
		}
	}

	if(qtd_cadas_disponiveis == 0) {
		return -2;
	}

	return -1;
}

void iniciar_server()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char nome_arquivo[1024];
    sprintf(nome_arquivo, "log__%d-%02d-%02d_%02d-%02d-%02d", 
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fd_log = open(nome_arquivo, O_CREAT | O_WRONLY, 0777);
    
    printf("Servidor iniciado\nArquivo de log criado em: %s\n", nome_arquivo);
}

void enviar_msg_queue(const char* mensagem) 
{
    mqd_t queue = mq_open(FILA_SERVER_CLI, O_WRONLY | O_CREAT, 0777, NULL);
    if (queue == (mqd_t) -1) {
        perror("mq_open");
        exit(2);
    }

    if (mq_send(queue, mensagem, sizeof(TMensagem), 29) != 0) {
        perror("erro ao enviar mensagem para o cliente");
    }

    mq_close(queue);
}

char* ler_msg_queue() 
{
    mqd_t queue = mq_open(FILA_CLI_SERVER, O_RDONLY | O_CREAT, 0777, NULL);
    if (queue == (mqd_t) -1) {
        perror("mq_open");
        exit(2);
    }

    ssize_t tam_buffer = get_msg_buffer_size(queue);        
    char* buffer = calloc(tam_buffer, 1);

    ssize_t nbytes = mq_receive(queue, buffer, tam_buffer, NULL);
    
    if (nbytes == -1) {
        perror("Erro ao receber mensagem do servidor");
        exit(4);
    }

    mq_close(queue);

    return buffer;
}

void escrever_log_jogada(int i_jogador, int x, int y, int valida)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    TJogador *jogador = jogadores[i_jogador];

    char* data_hora = malloc(25);
    sprintf(data_hora, "[%d/%02d/%02d %02d:%02d:%02d]", 
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    char* buf_escrita = malloc(255);

    char* jog_valida = valida == 0 ? "Inválida" : "Válida  ";
    char simbolo = mapear_jogada(jogador->multiplicador);

    sprintf(buf_escrita, "%s \t Jogada: (%d, %d) %s \t [%c] %s\n", 
        data_hora, x, y, jog_valida, simbolo, jogador->nickname);

    if (write(fd_log, buf_escrita, strlen(buf_escrita)) != strlen(buf_escrita)) {
        perror("Erro ao escrever log");
    }
}