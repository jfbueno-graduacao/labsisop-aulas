#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <string.h>

const char* CHAVE = "/mathplus"; 	//Declaração do nome da SHMEM
const int   SIZE  = 100; 		//Declaração do tamanho a ser reservado
sem_t cSemaforo; 			//Declaração da variável de semáforo

//Declaração da estrutura de dados unitária a ser compartilhada
typedef struct InfoCompartilhada {
	int maiorprimo;
} InfoCompartilhada_t;

void validarParametro(int arg){
	if(arg < 2) {
		printf("Erro! Deve ser informado o nome do arquivo como parâmetro!\n");
		exit(1);
	}
}

int lerValorArquivo(char* nomearquivo){
	int valor;
	char buf[1024];

	printf("Nome do arquivo informado: %s\n", nomearquivo);

        int fd = open(nomearquivo, O_RDONLY);
	if (fd == -1)
	{    	
    		printf("Arquivo não localizado! Informe o valor para o arquivo a ser criado (digitar inteiro entre 0 e 100)? \n");
		while(valor < 0 || valor > 100){
    			scanf("%d", &valor);
		}
		fd = open(nomearquivo, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

		char aux[3];
		sprintf(aux, "%i", valor);

		write(fd, aux, strlen(aux));
		close(fd);
    	}
	read(fd, buf, 1024);	
	valor = atoi(buf);	

   	if(valor < 0 || valor > 100 ){
		printf("Valor inválido!");		
		exit(1);
	}
	return valor;
}

long int fatorial(int v) {
	if (v==0) return 1;

	long int resultado = (long int) v;	
	while(v > 1) resultado *= --v;

	return resultado;
}

void capicua(int v){
	int ent=v;
	int inv=0;
	do{
		inv=inv*10+(ent%10);
		ent=ent/10;
	}while(ent>0);
	
	if (v == inv) printf("É capicua\n");
	else printf("Não é capicua\n");
}

int fibonacci(int v) {
    if (v < 2 && v >= 0) return v;

    return fibonacci(v-1)+fibonacci(v-2);
}

int primalidade(int v) {
	if(v == 0 || v == 1) return 0;

	int div = 2;
	while(div < v) {
		if(v % div == 0) return 0;
		div++;
	}

	return 1;
}

int primo(int v) {
	int i = 1;
	int seq = 0;

	while(v > seq) {
		i++;
		if(primalidade(i) == 1) seq++;
	}

	return i;
}

void* ThParte1(void *param) {
	char *vParte1;
	vParte1 = (char*) param;
	int iParte1 = atoi(vParte1);

	printf("Fatorial: %ld\n", fatorial(iParte1));
	pthread_exit(NULL);
}

void* ThParte2(void *param) {
	char *vParte2;
	vParte2 = (char*) param;
	int iParte2 = atoi(vParte2);

	printf("Fibonacci: %d\n", fibonacci(iParte2));
	pthread_exit(NULL);
}

void* ThParte3(void *param) {
	char *vParte3;
	vParte3 = (char*) param;
	int iParte3 = atoi(vParte3);
	int rPrimo = primo(iParte3);

	printf("Primo: %d\n", rPrimo);

	//Criação da SHMEM
	int shfd = shm_open(CHAVE, O_RDWR | O_CREAT , 0777);
	//Validação de callback da criação da SHMEM
	if (shfd < 0) { perror("shm_open"); exit(1); }
	//Print do id da SHMEM
	printf("shfd =  %d\n", shfd);
	//Reserva da espaço para a SHMEM
	if (ftruncate(shfd, SIZE) != 0) { perror("ftruncate"); exit(1); }
	//Mapeamento da memória e link com o espaço de edereçamento do processo
	void* addr = mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, shfd, 0);
	//Validação de callback do mapeamento
	if (addr == (void*) -1) { perror("mmap"); exit(1); }
	//Declaração e endereçamento do ponteiro da área comum
	InfoCompartilhada_t* v = (InfoCompartilhada_t*) addr;
	
	//Alocação de valores no processo pai
	v->maiorprimo = 0;

	//Declaração/inicialização semáforo
	sem_init(&cSemaforo, 0, 1);
	
	//Criação do processo filho
	switch (fork()) {
	case -1: perror("fork"); exit(39);
	case 0: // no filho - escrever na memoria compartilhada
		if (sem_wait(&cSemaforo) != 0) exit(85);

		printf("Filho iniciando...\n");
		printf("Primo recebido do pai: %d \n", v->maiorprimo);
		capicua(v->maiorprimo);

		//Add no contador do semáforo
		if (sem_post(&cSemaforo) != 0) exit(85);
		printf("Filho encerrando...\n");
		exit(EXIT_SUCCESS);
	default: // no pai
		//Se callback sem_wait !=0 = erro
		//sem_wait coloca thread em suspensão se contador retornado = 0
		//SO desperta a thread quando contador > 0
		if (sem_wait(&cSemaforo) != 0) exit(85);

		printf("Pai iniciando...\n");
		//wait(NULL); // esperar o filho terminar
		// ler da memoria compartilhada
		v->maiorprimo = rPrimo;

		//Add no contador do semáforo
		if (sem_post(&cSemaforo) != 0) exit(85);

		printf("Pai finalizando...\n");

		//Aguarda finalização das threads para poder liberar o semáforo.
		sem_destroy(&cSemaforo);

	}
	//Remover SHMEM
	shm_unlink(CHAVE);

	printf("Terminou!\n");
	return EXIT_SUCCESS;

	pthread_exit(NULL);
}

int main(int argc, char** argv) {
	pthread_t tids[3];

	validarParametro(argc);
	int valor = lerValorArquivo(argv[1]);

	int parte1 = valor/3;
	int parte2 = parte1*2;
	int parte3 = parte1*3;
	printf("Parte 1: %d | Parte 2: %d | Parte 3: %d \n\n", parte1, parte2, parte3);

	char s1[3];
	sprintf(s1, "%i", parte1);

	int th1 = pthread_create(&tids[0],NULL,&ThParte1,(void *) s1);
	if (th1) {
		errno = th1;
		perror("Falha na criação da thread da Parte 1\n");
		exit(EXIT_FAILURE);
	}

	char s2[3];
	sprintf(s2, "%i", parte2);

	int th2 = pthread_create(&tids[1],NULL,&ThParte2,(void *) s2);
	if (th2) {
		errno = th2;
		perror("Falha na criação da thread da Parte 2\n");
		exit(EXIT_FAILURE);
	}

	char s3[3];
	sprintf(s3, "%i", parte3);

	int th3 = pthread_create(&tids[2],NULL,&ThParte3,(void *) s3);
	if (th3) {
		errno = th3;
		perror("Falha na criação da thread da Parte 3\n");
		exit(EXIT_FAILURE);
	}

	pthread_exit(NULL);
}

