#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

void soma(int val1, int val2){
	printf("Resultado: %d\n", val1 + val2);
}

void subtracao(int val1, int val2){
	printf("Resultado: %d\n", val1 - val2);
}

void multiplicacao(int val1, int val2){
	printf("Resultado: %d\n", val1 * val2);
}

void divisao(int val1, int val2){
	printf("Resultado: %d\n", val1 / val2);
}

int main() 
{
	int valor1, valor2, opcao;

	while(opcao!=5){
		printf("\n****** MENU ******\n");
		printf("1 - Adição\n");
		printf("2 - Subtração\n");
		printf("3 - Multiplicação\n");
		printf("4 - Divisão\n");
		printf("5 - Sair\n");
		printf("Escolha a operação:");
		scanf("%d", &opcao);

		if (opcao > 0 && opcao < 5){
			printf("Digite o primeiro valor: ");
			scanf("%d", &valor1);
			printf("Digite o segundo valor: ");
			scanf("%d", &valor2);
		}

		switch (opcao)
		{
			case 1:
				soma(valor1, valor2);
			break;

			case 2:
				subtracao(valor1, valor2);
			break;

			case 3:
				multiplicacao(valor1, valor2);
			break;

			case 4:
				divisao(valor1, valor2);
			break;

			case 5:
				continue;
			break;

			default:
				printf("Valor inválido, digite 1, 2, 3, 4 ou 5!\n");
		}
	}
	return 0;
}

