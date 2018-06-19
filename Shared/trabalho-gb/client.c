#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>
#include "common.h"

void iniciar_jogo();
void fazer_jogada();
void jogada_invalida();

void tratador_sinal(int signum) 
{
	// Se receber SIGUSR2 quer dizer que jogada foi inválida 
	// e precisa ser repetida
	if(signum == 12 || signum == 17 || signum == 31) {
		jogada_invalida();
	} 

	fazer_jogada();
}

int main() 
{
	mqd_t queue;
	TJogador msg;
	
	char nick[128];
	printf("%s", "Entre com o seu nick: ");
	scanf("%s", nick); 

	msg.pid = getpid();
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

	TResposta* resposta = (TResposta*) buffer;

	if(resposta->codigo == 1) {
		iniciar_jogo();
	}

	printf("Servidor indisponível para jogar");
}

void iniciar_jogo()
{
	sigset_t mask;
	struct sigaction action;
	memset(&action, 0, sizeof(action));

	action.sa_handler = &tratador_sinal;

	if(sigaction(SIGUSR1, &action, NULL) == -1 || sigaction(SIGUSR2, &action, NULL) == -1){
		perror("Falha ao registrar recebedor de sinais");
		exit(-1);
	}

	printf("Meu PID = %d\n aguardando inicio do jogo \n", getpid());

	//seleciona todos os sinais exceto SIGUSR1
	sigfillset(&mask);
	sigdelset(&mask, SIGUSR1);
	sigdelset(&mask, SIGUSR2);

	while(1) 
	{
		// Fica aguardando sinal do servidor
		sigsuspend(&mask);
	}
}

void fazer_jogada()
{
	TJogada jogada;

	int jogadaX = 0;
	while(jogadaX < 1 || jogadaX > 3) {
		printf("Coordenada X da jogada: ");
		scanf("%d", &jogadaX);
	}

	int jogadaY = 0;
	while(jogadaY < 1 || jogadaY > 3) {
		printf("Coordenada Y da jogada: ");
		scanf("%d", &jogadaY);
	}

	printf("\n");

	jogada.x = jogadaX;
	jogada.y = jogadaY;

	mqd_t queue = mq_open(FILA_CLI_SERVER, O_WRONLY | O_CREAT, 0777, NULL);
	if (queue == (mqd_t) -1) {
		perror("mq_open");
		exit(2);
	}

	if (mq_send(queue, (const char*) &jogada, sizeof(TJogada), 29) != 0) {
		perror("erro ao enviar mensagem para servidor");
	}

	mq_close(queue);
}

void jogada_invalida()
{
	printf("\t** A jogada anterior é inválida. Faca uma nova jogada. **\n\n");
}