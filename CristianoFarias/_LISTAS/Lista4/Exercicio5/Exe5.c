//Cristiano Farias - Laboratorio de Sistemas - Lista4

#include <stdio.h>
#include <string.h>

void imprime_bytes_hex(int C){
	int i;
	for (i=C; i >= 0; i--) {
        	printf("%X\n", i);
	}
}
int main() {
	int str=0;
	scanf("%d", &str);
	imprime_bytes_hex(str);
    	return 0;
}
