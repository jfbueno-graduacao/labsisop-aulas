//Cristiano Farias - Laboratorio de Sistemas - Lista4

#include <stdio.h>
#include <string.h>

void imprime_bytes_bin(int C){
	printf("\n");
	int num;
	int cont;
	for (cont=C; cont >= 0; cont--) {
		num = cont;
        	int vet_bin[50];
		int i = 0, j;
		while(num > 0) {
			vet_bin[i] = num % 2;
			i++;
			num = num / 2;
		}
		for(j = i - 1; j >= 0; j--)
		printf("%d", vet_bin[j]);
		printf("\n");
	}

}

int main() {
	int str=0;
	scanf("%d", &str);
	 imprime_bytes_bin(str);
    	return 0;
}
