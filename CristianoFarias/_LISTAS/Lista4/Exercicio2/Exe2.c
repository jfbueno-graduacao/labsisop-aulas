//Cristiano Farias - Laboratorio de Sistemas - Lista4

#include <stdio.h>
#include <string.h>
int main() {
	char str[128];
	int i;
	scanf("%s", str);
	for (i=0; i < strlen(str); i++) {
        printf("%c\n", str[i]);
	}
    return 0;
}
