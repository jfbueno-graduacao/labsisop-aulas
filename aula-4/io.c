#include <stdio.h>
int main(){
    int a, b, c;
    printf("Entre o primeiro valor: ");
    scanf("%d", &a);

    printf("Entre o segundo valor: ");
    scanf("%d", &b);

    c = a + b;

    printf("%d + %d = %d\n", a, b, c);
    printf("%p\n", &a);
    return 0;
}