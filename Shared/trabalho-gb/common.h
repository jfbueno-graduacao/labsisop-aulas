typedef struct Jogador {
	pid_t pid;
	char nickname[128];
	int multiplicador;
} TJogador;

typedef struct MensagemRequisicao {
	int codigo;
} TMensagem;

typedef struct Jogada {
	int x;
	int y;
} TJogada;

// Nome da fila para comunicação server -> client 
const char* FILA_SERVER_CLI = "/fila_server-cli_";
// Nome da fila para comunicação client -> server
const char* FILA_CLI_SERVER = "/fila_cli-server_";

ssize_t get_msg_buffer_size(mqd_t queue) {
	struct mq_attr attr;

	if (mq_getattr(queue, &attr) != -1) {
		return attr.mq_msgsize;
	}

	perror("get_msg_buffer_size");
	exit(3);
}

#define clear_console() printf("\033[H\033[J")