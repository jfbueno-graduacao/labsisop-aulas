//Cristiano Farias - Laboratorio de Sistemas - Lista4

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>

int main(int argc, char *argv[]) {
	if(argc <= 3){
		printf("Passagem de parametros inválida digite 3 parametros!\n");
	}
	else{
		system(argv[3]);
		sleep(5);
		system(argv[2]);
		sleep(5);
		system(argv[1]);
		sleep(5);
		printf("Obrigado por utilizar o programa! ");
		sleep(1);
		system("date");
		
	}
}
