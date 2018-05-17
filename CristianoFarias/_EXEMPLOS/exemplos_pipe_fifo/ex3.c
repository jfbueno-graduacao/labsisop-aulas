#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){

	int fd;
	char buf1[] = "abcdefghij";
	char buf2[] = "";

	if(mkfifo("meuFIFO", 0777) != 0) perror("Erro FIFO");

	fd = open("meuFIFO", O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1) perror("Falha no open()");	

	if (write(fd, buf1, 10) != 10) perror("erro escrita buf1");
	if (read(fd, buf2, 10) != 10) perror("erro leitura buf2");
	
	printf("%s",buf2);

	close(fd);

	return(0);
}
