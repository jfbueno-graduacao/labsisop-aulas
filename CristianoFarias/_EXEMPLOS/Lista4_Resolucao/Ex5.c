#include<stdio.h>
#include<string.h>

void imprimir_hex(char letra) {
  printf("%x\n", (int) letra);
}

int main(){
	char palavra[128];
	printf("Digite a palavra:");
	scanf("%[^\n]s", palavra);
	for(int i=strlen(palavra)-1;i>=0;i--){
		imprimir_hex(palavra[i]);
	}
	return(0);
}
