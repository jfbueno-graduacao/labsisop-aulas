#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>

int main() {
	int cont=1;
	while(1) {
		cont++;
		printf("(%d)\n", cont);
		sleep(1);
	}
}