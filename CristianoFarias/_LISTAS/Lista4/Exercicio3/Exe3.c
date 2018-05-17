//Cristiano Farias - Laboratorio de Sistemas - Lista4

#include <stdio.h>
#include <string.h>

void imprime_caracteres(char *C){
	char str[128];
	strcpy (str,C);
	int i;
	for (i=0; i < strlen(str); i++) {
        	printf("%c\n", str[i]);
	}
}
int main() {
	char str[128];
	scanf("%s", str);
	imprime_caracteres(str);
    	return 0;
}
