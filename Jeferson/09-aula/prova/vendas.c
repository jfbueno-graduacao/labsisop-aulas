// Jéferson Bueno

#include <stdio.h>

int main()
{
    int qtd;
    float custo;

    printf("Digite a quantidade: ");
    scanf("%d", &qtd);

    printf("Digite o custo unitário: ");
    scanf("%f", &custo);

    float custo_total = qtd * custo;

    printf("O valor de custo total do produto é R$ %f (%d x R$ %f)\n", custo_total, qtd, custo);

    float contrib = custo_total / 2;
    printf("O valor de margem de contribuição é %f\n", contrib);

    float icms = (custo_total * 17) / 100;
    float margem_icms = icms / 2;
    float icms_total = icms + margem_icms;

    printf("O valor do ICMS para o produto é %f\n", icms_total);

    printf("O valor de venda do produto é, %f\n", custo_total + contrib + icms_total);
}