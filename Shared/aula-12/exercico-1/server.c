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

ssize_t get_msg_buffer_size(mqd_t queue);
void print_mercadoria(TMercadoria* m);

int main()
{
	mqd_t queue;
	TMercadoria mercadoria;

	char* buffer = NULL;

	ssize_t tam_buffer;
	ssize_t nbytes;	

	while(1) 
	{
		queue = mq_open(FILA_SHARED, O_RDONLY);
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

		print_mercadoria((TMercadoria*) buffer);

		mq_close(queue);
		printf("----------------------------\n");
	}
}

void print_mercadoria(TMercadoria* m) {
	printf("Despacho recebido do cliente %s", m->identificador);
	printf("\nId do produto: %d\nPeso=%f\n", m->id, m->peso);
}

ssize_t get_msg_buffer_size(mqd_t queue) {
	struct mq_attr attr;

	if (mq_getattr(queue, &attr) != -1) {
		return attr.mq_msgsize;
	}

	perror("get_msg_buffer_size");
	exit(3);
}