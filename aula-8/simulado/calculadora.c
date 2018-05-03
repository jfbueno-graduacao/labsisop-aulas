#include <stdio.h>
#include <stdlib.h>

int soma(int a, int b);
int subtracao(int a, int b);
int produto(int a, int b);
int divisao(int a, int b);


int main()
{
    while(1) {
        int num1 = 0;
        int num2 = 0;
        int opcao = 0;
        int invalida;

        do {
            invalida = 0;
            printf("Digite a opcão desejada: \n1) Soma\n2) Subtracão\n3) Produto\n4) Divisao\n5) Sair\n");
            scanf("%d", &opcao);
            
            if(opcao < 1 || opcao > 5) {
                printf("Opcão inválida\n\n");
                invalida = 1;
            } else if (opcao == 5) {
                exit(0);
            }
            
        } while(invalida == 1);

        printf("\nCerto. Agora entre com o primeiro número: ");
        scanf("%d", &num1);

        printf("\ne com o segundo número: ");
        scanf("%d", &num2);

        int r;
        switch(opcao) {
            case 1:
                r = soma(num1, num2);
                break;
            case 2:
                r = subtracao(num1, num2);
                break;
            case 3:
                r = produto(num1, num2);
                break;
            case 4:
                r = divisao(num1, num2);
                break;
        }

        printf("\nO resultado foi: %d \n\n", r);
    }
}

int soma(int a, int b) {
    return a + b;
}

int subtracao(int a, int b) {
    return a - b;
}

int produto(int a, int b) {
    return a * b;
}

int divisao(int a, int b) {
    return a / b;
}