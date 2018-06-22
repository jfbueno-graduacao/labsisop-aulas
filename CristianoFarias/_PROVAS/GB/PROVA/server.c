#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include "common.h"

void msg_jogador_conectado(TJogador* m);
void iniciar_server();
void iniciar_jogo();
void tabuleiro();
int verificarVitoria();
void enviar_msg_queue(const char* mensagem);
char* ler_msg_queue();
char* montarVisao(int x,int y);
void fimJogo(int vencedor);
void logando(int i_jogador, int x1, int y1,int x2,int y2, int valida);
int contParesDescobertos = 0;

TJogador* jogadores[2];
char* tabuleiroPaises[4][4] = { {"BRASIL", "EUA", "MEXICO" ,"FRANÇA"}, { "FRANÇA","PORTUGAL", "EUA", "MEXICO" }, { "BRASIL","PORTUGAL", "PERU", "PERU" },{ "MARROCOS","MARROCOS", "ALEMANHA", "ALEMANHA" } };
char* tabuleiroReferencia[4][4] = { {"X", "X", "X" ,"X"}, { "X","X", "X", "X" }, { "X","X", "X", "X" },{ "X","X", "X", "X" } };
int fd_log;

int main()
{
    iniciar_server();

    printf("Aguardando por jogadores\n-----------------------------\n");

	TJogador jogador;

	int qtdJogadores = 0;
	int mjogador = 1;

	while(qtdJogadores < 2) 
	{
		char *buffer = ler_msg_queue();

		TJogador *jogador = (TJogador*) buffer;

		msg_jogador_conectado(jogador);
		jogadores[qtdJogadores] = jogador;

		TMensagem resposta;
		resposta.codigo = 1; // 1 = Jogador logado

		// Responder cliente que está ok
		enviar_msg_queue((const char*) &resposta);

		qtdJogadores++;
	}

	printf("Jogadores logados. Iniciando jogo...\n\n");
	iniciar_jogo();
}

void iniciar_jogo()
{
	int index_jogador = 0;
	mqd_t queue;
	char* buffer = NULL;
	ssize_t tam_buffer;
	ssize_t nbytes;	

	while(1)
	{
		TJogador* jogador_atual = jogadores[index_jogador];		
		kill(jogador_atual->pid, SIGUSR1);

		printf("Vez de %s\n\n", jogador_atual->nickname);

		char* buffer = ler_msg_queue();

		TJogada *jogada = (TJogada*) buffer;

		int x1 = jogada->x1 - 1;
		int y1 = jogada->y1 - 1;
        int x2 = jogada->x2 - 1;
		int y2 = jogada->y2 - 1;

		// Se a jogada for inválida. Avisar o player via SIGUSR2 
		// e esperar uma nova jogada dele mesmo
		if(strcmp (tabuleiroReferencia[x1][y1],"OK") ==0  || strcmp (tabuleiroReferencia[x2][y2],"OK") ==0 ) {

            //escrever_log_jogada(index_jogador, jogada->x, jogada->y, 0);
			printf("\t *** Jogada inválida recebida ***\n");
            logando(index_jogador,x1,y1,x2,y2,0) ; 
			TMensagem mensagem;
			mensagem.codigo = 2; //Jogada inválida
			enviar_msg_queue((const char*) &mensagem);

			kill(jogador_atual->pid, SIGUSR2);
			continue;
		}

        if(strcmp (tabuleiroPaises[x1][y1],tabuleiroPaises[x2][y2]) ==0){
            tabuleiroReferencia[x1][y1] = "OK";
            tabuleiroReferencia[x2][y2] = "OK";
            contParesDescobertos++;	
            jogador_atual->pontuacao++;	 
             logando(index_jogador,x1,y1,x2,y2,1) ;             
        }
        else{            
             logando(index_jogador,x1,y1,x2,y2,2) ; 
             index_jogador = !index_jogador;
        }
        

		printf("Jogada recebida\n");
		tabuleiro();
		printf("\n");

		int venc = verificarVitoria();

        if(venc == -1) { // ainda nao terminou
           continue;
        }

        fimJogo(venc);
	}
}

void tabuleiro()
{
    printf("\n\ty-->\n");
    printf("\t_______________________\n");
    for(int i =0; i < 4;i++){
        if(i==0){printf("x |");}   
        if(i==1){printf("| |");}   
        if(i==2){printf("V |");}
        if(i==3){printf("  |");}    
        for(int j =0; j < 4;j++){
            printf("%s\t",montarVisao(i,j));
        }
         printf("\n");
    }
    printf("\n");
}

char* montarVisao(int x,int y){
    if(strcmp (tabuleiroReferencia[x][y],"OK") ==0){
        return tabuleiroPaises[x][y];
    }
    return "X";
}


int verificarVitoria(){
    if(contParesDescobertos == 8){
        if(jogadores[0]->pontuacao == jogadores[1]->pontuacao){
            return -2;
        }else if(jogadores[0]->pontuacao > jogadores[1]->pontuacao){
            return 0;
        }
        return 1;
    }
    return -1;
}

void fimJogo(int vencedor)
{
    if(vencedor == -2) {
        printf("Ocorreu um empate. Finalizando o jogo\n");

        // Mandar msg de empate para os dois jogadores
        for(int i = 0; i < 2; i++) {
            TMensagem msg;
            msg.codigo = 3;
            enviar_msg_queue((const char*) &msg);

            kill(jogadores[i]->pid, SIGUSR2);
            usleep(500);
        }

        exit(0);
    }

    printf("##### Parabéns, %s!!!! Você venceu! #####\n", jogadores[vencedor]->nickname);

    TMensagem msg;
    msg.codigo = 4;
    enviar_msg_queue((const char*) &msg);
    kill(jogadores[vencedor]->pid, SIGUSR2);

    int iperdedor = !vencedor;

    usleep(100);

    msg.codigo = 5;
    enviar_msg_queue((const char*) &msg);
    kill(jogadores[iperdedor]->pid, SIGUSR2);

    close(fd_log);
    exit(0);
}

void msg_jogador_conectado(TJogador *jogador) 
{
	printf("%s entrou no jogo. ID %d\n", jogador->nickname, jogador->pid);
}





void iniciar_server()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char nome_arquivo[1024];
    sprintf(nome_arquivo, "%s","Auditoria.txt");

    fd_log = open(nome_arquivo, O_CREAT | O_WRONLY, 0777);
    
    printf("Servidor iniciado\nArquivo de log criado em: %s\n", nome_arquivo);
}

void enviar_msg_queue(const char* mensagem) 
{
    mqd_t queue = mq_open(FILA_SERVER_CLI, O_WRONLY | O_CREAT, 0777, NULL);
    if (queue == (mqd_t) -1) {
        perror("mq_open");
        exit(2);
    }

    if (mq_send(queue, mensagem, sizeof(TMensagem), 29) != 0) {
        perror("erro ao enviar mensagem para o cliente");
    }

    mq_close(queue);
}

char* ler_msg_queue() 
{
    mqd_t queue = mq_open(FILA_CLI_SERVER, O_RDONLY | O_CREAT, 0777, NULL);
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

void logando(int i_jogador, int x1, int y1,int x2,int y2, int valida)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    TJogador *jogador = jogadores[i_jogador];

    char* data_hora = malloc(25);
    sprintf(data_hora, "[%d/%02d/%02d %02d:%02d:%02d]", 
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    char* buf_escrita = malloc(255);

    char* jog_valida = valida == 0 ? "Inválida" : "Válida  ";

    if(valida ==0){
        jog_valida = "Inválida";
    }else if(valida ==1){
        jog_valida = "Acertou";
    }else{
        jog_valida = "Errou";
    }

    sprintf(buf_escrita, "%s \t Jogador %s -> Jogada: (%d %d | %d %d) %s\n", 
        data_hora, jogador->nickname, x1, y1,x2,y2, jog_valida);

    if (write(fd_log, buf_escrita, strlen(buf_escrita)) != strlen(buf_escrita)) {
        perror("Erro ao escrever log");
    }
}