#include <stdio.h>

int main()
{
    int opcao;

    printf ("[1] Cadastrar cliente\n"
            "[2] Procurar cliente\n"
            "[3] Inserir pedido\n"
            "[0] Sair\n\n"
            "Digite sua escolha: ");
    
    scanf ("%d", &opcao);
    
    switch (opcao) {
        case 1:
            cadastra_cliente(); 
            break;
        case 2:
            procura_cliente(); 
            break;          
        case 3:
            insere_pedido(); 
            break;
        case 0:
            return 0;
        default:
            printf ("Opção inválida!\n");
    }
}