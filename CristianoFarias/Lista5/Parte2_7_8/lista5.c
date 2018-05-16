//Cristiano Farias - Laboratorio de Sistemas - Lista5

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int load_firefox(char *url){
	char a[500] ="firefox --new-window ";
	strcat(a,url);
	system(a);
	return 0;
}


int main(int argc, char *W[]){
	load_firefox(W[1]);
}
