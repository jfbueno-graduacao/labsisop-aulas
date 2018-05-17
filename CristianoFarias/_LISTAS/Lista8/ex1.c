#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>

#define NUM_THREADS 3
#define NUM_MSG 5
#define SLEEP_TIME 1
pthread_t tids[NUM_THREADS];
char* names[NUM_THREADS] = { "moe", "larry", "curly" };
void* PrintHello(void *param) {
	int i;
	char *who;
	who = (char*) param;
	for (i=0; i<NUM_MSG; i++) {
		sleep(SLEEP_TIME);
		printf("Hello World! It's me, %s and my id is %li!\n", who,pthread_self());
		
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int n;
	int te;
	
	for (n=0; n<NUM_THREADS; n++) {
		printf("Hello World! I am a father thread, and my id is %li!\n",pthread_self());
		te = pthread_create(&tids[n],NULL,&PrintHello,(void *) names[n]);
		if (te) {
			errno = te;
			perror("Falha na criação da thread");
			exit(EXIT_FAILURE);
		}
	}
	pthread_exit(NULL);
}
