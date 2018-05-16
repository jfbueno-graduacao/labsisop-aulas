#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>

double adicao(double numA,double numB){
	return	numA+numB;
}
double subtracao(double numA,double numB){
	return	numA-numB;
}
double divisao(double numA,double numB){
	return	numA/numB;
}
double multiplicacao(double numA,double numB){
	return	numA*numB;
}

int intro() {
    	int op = -1;
    	double numA;
	double numB;
	printf("Digite o primeiro numero => ");
    	scanf("%lf", &numA);
	printf("Digite o segundo numero => ");
	scanf("%lf", &numB);
	printf("Digite o primeiro numero => ");
	while(op != 5){
		printf("\n\n1-soma, 2-subtração, 3-produto, 4-divisão ou 5-sair\nEscolha uma opção	=> ");
		scanf("%i", &op);
		switch(op){
		case 1:
			printf("RESULTADO => %lf",adicao(numA,numB));
		break;		
		case 2:
			printf("RESULTADO => %lf",subtracao(numA,numB));
		break;
		case 3:
			printf("RESULTADO => %lf",multiplicacao(numA,numB));
		break;
		case 4:
			printf("RESULTADO => %lf",divisao(numA,numB));
		break;	
		case 5:
			exit(13);
		break;		
		default:
			printf("Opção inválida!!!");
		break;	
		}
	}
   	return op;
}


int main(){
	intro();
	return 0;
}




