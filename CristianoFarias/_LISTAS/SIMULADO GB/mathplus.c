//Cristiano Farias - Laboratorio de Sistemas - Lista8

#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char buf2[1000];
void numeroFatorial(int valor);
int numeroFibo(int valor);
int numeroPrimo(int valor);
char *lerString(char *inf)
{
	char chr[1000] = "";
	printf("%s -> ", inf);
	scanf("%[^\n]", chr);
	scanf("%*[^\n]");
	scanf("%*c");
	char *ret = chr;
	return ret;
}

void run()
{
	char *t = lerString("Digite o nome de um Arquivo");
	int aux;
	aux = open(t, O_RDONLY);
	if (aux == -1)
	{
		aux = open(t, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		char *texto = lerString("Digite o texto do novo Arquivo");
		if (aux == -1)
		{
			printf("Falha para criar arquivo! \n");
			exit(0);
		}
		write(aux, texto, strlen(texto));
		close(aux);
	}
	sleep(1);	
	read(aux, buf2, 1000);	

    //validator
    int ts = atoi(buf2);	

   	if(ts >= 0 && ts<100 ){
		ts = ts/3;
		//Primeira parte(Fatorial)
		pid_t pid;
		if ((pid=fork()) < 0) {
			perror("erro no fork!"); 
			exit(-1);			
		}
		else if (pid == 0) {
			numeroFatorial(ts);
			exit(1);
		}
		//Segunda parte(Fibonacci)	
		if ((pid=fork()) < 0) {
			perror("erro no fork!"); 
			exit(-1);			
		}
		else if (pid == 0) {
			printf("\n Fibonacci é %i\n",numeroFibo(ts*2));
			exit(1);
		}
		//Terceira parte(Primos)		
		if ((pid=fork()) < 0) {
			perror("erro no fork!"); 
			exit(-1);			
		}
		else if (pid == 0) {		
			printf("\n Primos é %i\n",numeroPrimo(ts*3));
			exit(1);
		}
    }
    else{
		printf("Valor informado no arquivo é inválido para os processamentos: %i", ts);
    }
}

void numeroFatorial(int valor){
	int pont = valor;	
	for(int i = valor; i > 1;i--){
		pont = pont*(i-1);
	}
	printf("\n Fatorial é %i\n",pont);
}

int numeroFibo(int valor){		
	if(valor == 1 || valor == 2){
		return 1;
	}
	else{
		return numeroFibo(valor-1)+numeroFibo(valor-2);
	}
}

int numeroPrimo(int valor){		
	for(int i = 0; i < valor;i++){
		


	}
	return 0;
}

int main(int argc, char *W[])
{
	run();
}
