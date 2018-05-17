#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	int fd;
	fd= open("arquivo1.txt", O_RDONLY);
	if (fd == -1) perror("Falha no open()");	
	return(0);
}

