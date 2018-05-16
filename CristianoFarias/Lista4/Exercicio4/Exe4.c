//Cristiano Farias - Laboratorio de Sistemas - Lista4

#include <stdio.h>
#include <string.h>

void imprime_bytes_dec(char *C){
	char str[128];
	strcpy (str,C);
	int i;
	for (i=0; i < strlen(str); i++) {
        	printf("%d\n", (str[i]));
	}
}
int main() {
	char str[128];
	scanf("%s", str);
	imprime_bytes_dec(str);
    	return 0;
}
