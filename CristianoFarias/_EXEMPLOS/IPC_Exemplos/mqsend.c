#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

//Nome da fila
const char* NOME_FILA = "/fila1";

//Estrutura de dados para a mensagem
typedef struct Mensagem {
	int id;
	char nome[30];
} TMensagem;

int main(int argc, char* argv[]) {
	//Declaração da fila
	mqd_t queue;
	//Declaração da mensagem
	TMensagem m;
	//Texto a ser enviado na mensagem - entrada por linha de comando
	char* texto = argv[1];

	//Obter descritor (mq_open+O_WRONLY+O_CREAT)
	queue = mq_open(NOME_FILA, O_WRONLY | O_CREAT, 0660, NULL);
	if (queue == (mqd_t) -1) {
		perror("mq_open");
		exit(2);
	}

	//Montar a mensagem
	m.id = 1;
	strncpy(m.nome, texto, 29);

	//Enviar (mq_send)
	if (mq_send(queue, (const char*) &m, sizeof(TMensagem), 29) != 0) {
		perror("send #29");
	}

	//Liberar descritor (mq_close)
	mq_close(queue);

	printf("Mensagem enviada!\n");
	exit(EXIT_SUCCESS);
}
