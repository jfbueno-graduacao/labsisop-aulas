typedef struct Cliente {
  int tipo;
} TClt;

// Nome da fila para comunicação server -> client 
const char* FILA_1 = "/fila1";
// Nome da fila para comunicação client -> server
const char* FILA_2 = "/fila2";

ssize_t get_msg_buffer_size(mqd_t queue) {
	struct mq_attr attr;

	if (mq_getattr(queue, &attr) != -1) {
		return attr.mq_msgsize;
	}

	perror("get_msg_buffer_size");
	exit(3);
}

#define clear_console() printf("\033[H\033[J")