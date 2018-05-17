//Cristiano Farias - Laboratorio de Sistemas - Lista5

#include <stdio.h>
#include <string.h>

int lerInt(){
	int i;
	printf("Digite um valor int-> ");
	scanf("%d",&i);
	return i;
}

void repete(char *W, int N){
	for (int i=0;i<N; i++) {
	printf("#%i '%s'\n",i+1,W);
	}
}

int check (int qtd){
if(qtd == 2){
return 1;
}
return 0;
}

int main(int argc, char *W[]){
	int i;
	if(check(argc) == 1){
	i = lerInt();
	repete(W[1],i);
	return 0;
	}else{
	printf("Quantidade de parâmetros informados é inválido, Digite apenas 1 parametro.\n");	
	return 1;	
	}
}
