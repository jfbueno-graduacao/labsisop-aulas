#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//PARA COMPILAR
// gcc shm.c -o shm -l rt

//Declaração do nome da SHMEM
const char* CHAVE = "/compart1";
//Declaração do tamanho a ser reservado
const int   SIZE  = 5*4096;
//Declaração da estrutura de dados unitária a ser compartilhada
typedef struct InfoCompartilhada {
	int a;
	double d;
	long n[200];
} InfoCompartilhada_t;

int main(void) {

	//Criação da SHMEM
	int shfd = shm_open(CHAVE, O_RDWR | O_CREAT , 0600);
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
	v->a = 0;
	v->d = 0;
	v->n[35] = 0x0;

	//Criação do processo filho
	switch (fork()) {
	case -1: perror("fork"); exit(39);
	case 0: // no filho - escrever na memoria compartilhada
		printf("Filho iniciando...\n");
		sleep(2);
		v->a = 35;
		v->d = 3.14158192;
		v->n[35] = 0xFFFF;
		exit(EXIT_SUCCESS);
	default: // no pai
		printf("Pai aguardando filho terminar...\n");
		wait(NULL); // esperar o filho terminar
		// ler da memoria compartilhada
		printf("\nv: a=%d d=%f n[35]=%lx\n", v->a, v->d, v->n[35]);
	}
	//Remover SHMEM
	shm_unlink(CHAVE);

	printf("Terminou!\n");
	return EXIT_SUCCESS;
}
