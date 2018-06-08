//Cristiano Farias - Laboratorio de Sistemas - Lista5

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>



int load_firefox(char *url){
	pid_t pid;
	if ((pid=fork()) < 0) {
		perror("erro no fork!"); 
		exit(-1);
		return 1;
	}
	else if (pid == 0) {
		char a[500] ="firefox --new-window ";
		strcat(a,url);
		system(a);
		exit(1);
	}
	return 0;
}

int start_browser(int size, char *url[]){	
	for (int i=1;i<=size; i++) {	
		load_firefox(url[i]);
	}
	return 0;
}

int main(int argc, char *W[]){
	start_browser(argc,W);
}
