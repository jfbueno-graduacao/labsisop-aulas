#include<stdio.h>
#include<string.h>

int main(){
	char palavra[128];
	printf("Digite a palavra:");
	scanf("%[^\n]s", palavra);
	for(int i=0;i<strlen(palavra);i++){
		printf("%c\n", palavra[i]);
	}
	return(0);
}
