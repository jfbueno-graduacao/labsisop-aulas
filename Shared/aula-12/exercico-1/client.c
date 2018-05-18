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
} TMercadoria;

int main() 
{
	/*mqd_t queue;
	TMercadoria m;*/


	int id;
	scanf("Qual o Id do produto? %d", &id);
	/*while(1)
	{		
		//scanf("Qual o Id do produto? %d", &id);

		/*queue = mq_open(FILA_SHARED, O_WRONLY | O_CREAT, 0660, NULL);
		if (queue == (mqd_t) -1) {
			perror("mq_open");
			exit(2);
		}


		m.id = id;
		m.peso = 45.2;

		if (mq_send(queue, (const char*) &m, sizeof(TMercadoria), 29) != 0) {
			perror("send #29");
		}

		mq_close(queue);

		printf("Mensagem enviada!\n");
		exit(EXIT_SUCCESS);
	}*/
}