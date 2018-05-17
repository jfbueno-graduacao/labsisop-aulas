#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>

int main(){
	//agenda SIGALRM para daqui a 5 segundos
	if (alarm(5) < 0){
		perror("Falha ao agendar alarme");
	}	

	printf("O processo é eterno, enquanto dura...\n");

	//espera ocupada (busy-waiting) pelo alarme
	while(1);
	
	printf("Linha que nunca aparece...\n");

	return 0;
}




