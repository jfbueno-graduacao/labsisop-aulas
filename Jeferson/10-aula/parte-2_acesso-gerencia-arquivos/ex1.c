#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	int fd = STDOUT_FILENO;
	char buf1[] = "abcdefghij";
	char buf2[] = "ABCDEFGHIJ";
	
	if (write(fd, buf1, 10) != 10) perror("escrita buf1"); /* offset=10 */
	if (lseek(fd, 40, SEEK_SET) == -1) perror("lseek"); /* offset=40 */
	if (write(fd, buf2, 10) != 10) perror("escrita buf2"); /* offset=50 */
	
	return(0);
}
