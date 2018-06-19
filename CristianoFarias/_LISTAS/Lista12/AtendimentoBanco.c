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
