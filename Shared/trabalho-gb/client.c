#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
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
void print(TResposta *m);
void iniciar_jogo();

void tratador_sinal(int signum) {
	TJogada jogada;

	int jogadaX;
	printf("Coordenada X da jogada: ");
	scanf("%d", &jogadaX);

	int jogadaY;
	printf("\nCoordenada Y da jogada: ");
	scanf("%d", &jogadaY);

	jogada.x = jogadaX;
	jogada.y = jogadaY;

	mqd_t queue = mq_open(FILA_CLI_SERVER, O_WRONLY | O_CREAT, 0660, NULL);
	if (queue == (mqd_t) -1) {
		perror("mq_open");
		exit(2);
	}

	if (mq_send(queue, (const char*) &jogada, sizeof(TJogada), 29) != 0) {
		perror("erro ao enviar mensagem para servidor");
	}

	mq_close(queue);
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

	if(sigaction(SIGUSR1, &action, NULL) == -1){
		perror("Falha ao registrar recebedor de sinais");
		exit(-1);
	}

	printf("Meu PID = %d\n ESPERANDO SINAL \n", getpid());

	//seleciona todos os sinais exceto SIGUSR1
	sigfillset(&mask);
	sigdelset(&mask, SIGUSR1);

	while(1) 
	{
		// Fica aguardando sinal do servidor
		sigsuspend(&mask);
	}
}

void print(TResposta *m) 
{
	printf("codigo resposta %d", m->codigo);
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