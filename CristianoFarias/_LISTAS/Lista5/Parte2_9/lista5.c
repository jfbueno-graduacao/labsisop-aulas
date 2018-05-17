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

int start_browser(char *url){
	load_firefox(url);
	return 0;
}

int main(int argc, char *W[]){
	pid_t pid;
	if ((pid=fork()) < 0) {
		perror("erro no fork!"); 
		exit(-1);
		return 1;
	}
	else if (pid == 0) {
		start_browser(W[1]);
	}
}
