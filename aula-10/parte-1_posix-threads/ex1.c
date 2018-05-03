#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>

#define NUM_THREADS 3
#define NUM_MSG 5
#define SLEEP_TIME 1

void* PrintHello(void *param) {
	int i;
	char *who;
	who = (char*) param;
	for (i=0; i<NUM_MSG; i++) {
		sleep(SLEEP_TIME);
		printf("Hello World! It's me, %s!\n", who);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t tids[NUM_THREADS];
	int n;
	int te;
	char* names[NUM_THREADS] = { "moe", "larry", "curly" };
	for (n=0; n<NUM_THREADS; n++) {
		te = pthread_create(&tids[n],NULL,&PrintHello,(void *) names[n]);
		if (te) {
			errno = te;
			perror("Falha na criação da thread");
			exit(EXIT_FAILURE);
		}
	}
	pthread_exit(NULL);
}
