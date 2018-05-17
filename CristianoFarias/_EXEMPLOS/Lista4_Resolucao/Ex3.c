#include<stdio.h>
#include<string.h>

void imprimir_caracter(char letra) {
  printf("%c\n", letra);
}

int main(){
	char palavra[128];
	printf("Digite a palavra:");
	scanf("%[^\n]s", palavra);
	for(int i=0;i<strlen(palavra);i++){
		imprimir_caracter(palavra[i]);
	}
	return(0);
}
