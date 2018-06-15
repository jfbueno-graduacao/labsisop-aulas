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

const char* CHAVE_SHMEM = "/shared";

int fib(int x);
long int fat(int x);
int prime(int x);
int isprime(int x);
void start(int entrada);
void* thread1(void *param);
void* thread2(void *param);
void* thread3(void *param);
void novo_proc();

typedef struct InfoCompartilhada {
	int numero;
} InfoCompartilhada_t;

sem_t tela_lock;
InfoCompartilhada_t* info;

int main(int argc, char** argv) {
	// gravar o arquivo e os caralho
	printf("%d\n", argc);

	if(argc < 2) {
		printf("É preciso apssar o nome do arquivo como parametro\n");
		exit(1);
	}
	
	int entrada;

	char* nomearq = argv[1];
    int fd = open(nomearq, O_RDONLY);
    if (fd == -1)
    {    	
    	printf("Arquivo inexistente, criando arquivo. Qual o valor a escrever? \n");
    	scanf("%d", &entrada);

    	if(entrada < 0 || entrada > 100) {
    		printf("Valor inválido, o valor deve ser positivo e menor que 100\n");
    		exit(1);
    	}

        int nfd = open(nomearq, O_CREAT | O_RDWR,S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR);

        //todo: usar sprintf para converter para char*
        if(write(nfd, (void *) entrada, 2) != 10) {
        	perror("erro ao escrever no arquivo");
        }
        exit(1);
    }
    else
    {
        size_t size = 2;
        char buf2[2];
        size_t bt = read(fd, buf2, size);

        //todo: usar atoi p/ converter valor lido para int
        printf("Arquivo %s", buf2);

        exit(1);
    }

	sem_init(&tela_lock, 1, 0);

	start(entrada);

	sem_destroy(&tela_lock);
	close(fd);
}

void start(int entrada) {
	int divido = entrada / 3;

	printf("%d Valor lido \n", entrada);
	printf("%d Valor divido \n\n", divido);

	pthread_t thread_id;
	int thread_status = pthread_create(&thread_id, NULL, &thread1, (void *) divido);

	pthread_t thread2_id;
	int thread2_status = pthread_create(&thread2_id, NULL, &thread2, (void *) (divido* 2));

	pthread_t thread3_id;
	int thread3_status = pthread_create(&thread3_id, NULL, &thread3, (void *) (divido * 3));

	novo_proc();

	pthread_exit(NULL);
}

void novo_proc() {
	pid_t pid = fork();

	if(pid == 0) {
		printf("Novo processo iniciado com PID %d\n", getpid());

		if (sem_wait(&tela_lock) != 0) 
			exit(85);

		int i = 80;
		while(i < 100) {
			printf("%d\n", i);
			i++;
		}

		exit(1);
	}else {
		wait(NULL);
	}
}

void* thread1(void *param) {
	int num = (int) param;	
	long int res = fat(num);

	printf("Fat(%d) = %ld --> %d! = %ld\n\n", num, res, num, res);

	pthread_exit(NULL);
}

void* thread2(void *param) {
	int num = (int) param;	
	int res = fib(num);

	printf("Fib(%d) = %d\n\n", num, res);
	pthread_exit(NULL);
}

void* thread3(void *param) {
	int num = (int) param;	
	int res = prime(num);

	printf("Prime(%d) = %d\n\n", num, res);

	int shfd = shm_open(CHAVE_SHMEM, O_RDWR | O_CREAT , 0600);
	
	if (shfd < 0) { 
		perror("shm_open"); 
		exit(1); 
	}
		
	if (ftruncate(shfd, sizeof(int)) != 0) { 
		perror("ftruncate"); exit(1); 
	}

	void* addr = mmap(NULL, sizeof(int), PROT_WRITE, MAP_SHARED, shfd, 0);
	
	if (addr == (void*) -1) { 
		perror("mmap"); 
		exit(1); 
	}
	
	info = (InfoCompartilhada_t*) addr;
	info->numero = res;

	if (sem_post(&tela_lock) != 0) 
		exit(85);

	pthread_exit(NULL);
}

long int fat(int x) {
	long int res = (long int)x;	
	while(x > 1)
	{
		res *= --x;
	}

	return res;
}

int fib(int x) {
    if (x == 0) {
        return 0;
    }

    if (x == 1) {
        return 1;
    }

    return fib(x - 1) + fib(x - 2);
}

int prime(int pos) {
	int num = 1;
	int found = 0;

	while(pos > found) {
		num++;
		if(isprime(num) == 1) {
			found++;
		}
	}

	return num;
}

int isprime(int num) {
	if(num == 0 || num == 1)
		return 0;

	int divisor = 2;
	while(divisor < num) {
		if(num % divisor == 0) {
			return 0;
		}

		divisor++;
	}

	return 1;
}