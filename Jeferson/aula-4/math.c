#include <stdio.h>

int main(){
    int a, b;
    a = b = 5;
    printf("%d\n", ++a + 5);
    printf("%d\n", a);
    printf("%d\n", b++ + 5);
    printf("%d\n", b);
    return 0;
}