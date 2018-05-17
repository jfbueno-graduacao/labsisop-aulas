#include<stdio.h>
#include<string.h>

void imprimir_decimal(char letra) {
  printf("%d\n", (int) letra);
}

int main(){
	char palavra[128];
	printf("Digite a palavra:");
	scanf("%[^\n]s", palavra);
	for(int i=0;i<strlen(palavra);i++){
		imprimir_decimal(palavra[i]);
	}
	return(0);
}
