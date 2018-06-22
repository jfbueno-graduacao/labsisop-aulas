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
char* ler_mensagem_queue();

void tratador_sinal(int signum) 
{
	// Se receber SIGUSR2 quer dizer que é necessário
	// ler a msg que o server deixou na fila e interpretá-la
	if(signum == 12 || signum == 17 || signum == 31) 
	{
		char* buffer = ler_mensagem_queue();
		TMensagem* resposta = (TMensagem*) buffer;

		if(resposta->codigo == 2) {
			jogada_invalida();
		} else if(resposta->codigo == 3) {
			printf("O jogo terminou em empate!\n");
			exit(0);
		} else if(resposta->codigo == 4) {
			printf("##### Parabéns! Você venceu! #####\n");
			exit(0);
		} else if (resposta->codigo == 5) {
			printf("Você perdeu =(\n");
			exit(0);
		}
	} else { // Neste caso, o sinal recebido é um sigusr1
		fazer_jogada();
	}
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
	
	char* buffer = ler_mensagem_queue();

	TMensagem* resposta = (TMensagem*) buffer;

	if(resposta->codigo == 1) {
		iniciar_jogo();
	}

	printf("Servidor indisponível para jogar");
}

char* ler_mensagem_queue() 
{
	mqd_t queue = mq_open(FILA_SERVER_CLI, O_RDONLY | O_CREAT, 0660, NULL);
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

	printf("Meu PID = %d\nAguardando inicio do jogo \n", getpid());

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

	int jx1 = 0;
	int jy1 = 0;
	int jx2 = 0;
	int jy2 = 0;
	int ctl = 1;
	while(ctl) {
		printf("Primeira peça coordenada da jogada (X): ");
		scanf("%d", &jx1);
		if(jx1 < 1 || jx1 > 4){
		printf("Digite novamente...\n");
		}
		else{
			ctl = 0;
		}
	}
	ctl = 1;

	while(ctl) {
		printf("Primeira peça coordenada da jogada (Y): ");
		scanf("%d", &jy1);
		if(jy1 < 1 || jy1 > 4){
		printf("Digite novamente...\n");
		}
		else{
			ctl = 0;
		}
	}
	ctl = 1;

	while(ctl) {
		printf("Segunda peça coordenada da jogada (X): ");
		scanf("%d", &jx2);
		if(jx2 < 1 || jx2 > 4){
		printf("Digite novamente...\n");
		}
		else{
			ctl = 0;
		}
	}
	ctl = 1;

	while(ctl) {
		printf("Segunda peça coordenada da jogada (Y): ");
		scanf("%d", &jy2);
		if(jy2 < 1 || jy2 > 4){
		printf("Digite novamente...\n");
		}
		else{
			ctl = 0;
		}
	}
		

	printf("\n");

	jogada.x1 = jx1;
	jogada.y1 = jy1;
	jogada.x2 = jx2;
	jogada.y2 = jy2;

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