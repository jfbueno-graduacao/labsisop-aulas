#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

const char* FILA_SHARED = "/fila_msgs";

typedef struct Mercadoria {
	int id;
	float peso;
	char identificador[128];
} TMercadoria;

int main() 
{
	mqd_t queue;
	TMercadoria m;
	
	char cli_id[128];
	printf("%s", "Entre com o seu identificador de cliente: ");
	scanf("%s", cli_id);

	while(1)
	{
		int opcao;
		printf("** Menu ** \n1) Sair\n2) Novo Despacho\n");
		scanf("%d", &opcao);

		if(opcao == 1) {
			exit(0);
		}

		printf("----- Novo despacho -----\n\n");
		int id;
		printf("Qual o Id do produto? ");
		scanf("%d", &id);

		float peso;
		printf("Qual o peso do produto? ");
		scanf("%f", &peso);

		queue = mq_open(FILA_SHARED, O_WRONLY | O_CREAT, 0660, NULL);
		if (queue == (mqd_t) -1) {
			perror("mq_open");
			exit(2);
		}

		m.id = id;
		m.peso = peso;
		strncpy(m.identificador, cli_id, 127);

		if (mq_send(queue, (const char*) &m, sizeof(TMercadoria), 29) != 0) {
			perror("erro ao enviar mensagem para servidor");
		}

		mq_close(queue);

		printf("Despacho enviado!\n\n");
	}
}