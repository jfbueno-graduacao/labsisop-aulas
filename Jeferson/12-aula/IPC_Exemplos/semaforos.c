#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

//Declaração das funções das threads
void* thr1(void* p);
void* thr2(void* p);

//Declaração da variável de semáforo em área de memório compart.
sem_t tela_lock;

int main(void) {
	//Declaração thread
	pthread_t t[2];

	//Declaração/inicialização semáforo
	sem_init(&tela_lock, 0, 1);

	//Disparo das threads
	pthread_create(&t[0], NULL, &thr1, NULL);
	pthread_create(&t[1], NULL, &thr2, NULL);

	//Aguarda finalização das threads para poder liberar o semáforo.
	sem_destroy(&tela_lock);

	//Encerramento e liberação recursos das threads
	pthread_exit(NULL);
}

void* thr1(void* p) {
	//Declaração do array de teste
	char* m[] = { "(1) Ola", "(1) eu", "(1) sou", "(1) a", "(1) thread", "1.", "\n" };

	//Laço para simulação
	for (int i = 0; i < 5; i++) {
		//Se callback sem_wait !=0 = erro
		//sem_wait coloca thread em suspensão se contador retornado = 0
		//SO desperta a thread quando contador > 0
		if (sem_wait(&tela_lock) != 0) exit(85);
		//Laço para simulação interna
		for (int j = 0; j < 7; j++) {
			//Grava na saida padrão STDOUT
			write(1, m[j], strlen(m[j]));
			write(1, " ", 1);
			//Sleep em micro segundos programado
			usleep(500000);
		}
		//Add no contador do semáforo
		if (sem_post(&tela_lock) != 0) exit(85);
	}
	//Encerramento função
	return NULL;
}
void* thr2(void* p) {
	//Declaração do array de teste
	char* m[] = { "(2) Ola", "(2) eu", "(2) sou", "(2) a", "(2) thread", "2.", "\n" };

	//Laço para simulação
	for (int i = 0; i < 5; i++) {
		//Se callback sem_wait !=0 = erro
		//sem_wait coloca thread em suspensão se contador retornado = 0
		//SO desperta a thread quando contador > 0
		if (sem_wait(&tela_lock) != 0) exit(85);
		//Laço para simulação interna
		for (int j = 0; j < 7; j++) {
			//Grava na saida padrão STDOUT
			write(1, m[j], strlen(m[j]));
			write(1, " ", 1);
			//Sleep em micro segundos programado
			usleep(500000);
		}
		//Add no contador do semáforo
		if (sem_post(&tela_lock) != 0) exit(85);
	}
	//Encerramento função
	return NULL;
}









