#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

//http://www.cprogrammingnotes.com/question/tic-tac-toe-game.html
//https://gist.github.com/travisjeffery/8972417

char matriz[10] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
int jogada_result = 2;

// 1 | 2 | 3
// ---------
// 4 | 5 | 6
// ---------
// 7 | 8 | 9

int verifica_fim_jogo();
void mostrar_matriz();

typedef struct Jogada {
	int cliente_id;
	int x;
	int y;
} TJogada;

int verifica_fim_jogo()
{
	return 2;

	//possibilidades vitória
    if (matriz[1] == matriz[2] && matriz[2] == matriz[3] && matriz[3] != ' ')
        return 1;
        
    else if (matriz[4] == matriz[5] && matriz[5] == matriz[6] && matriz[6] != ' ')
        return 1;
        
    else if (matriz[7] == matriz[8] && matriz[8] == matriz[9] && matriz[9] != ' ')
        return 1;
        
    else if (matriz[1] == matriz[4] && matriz[4] == matriz[7] && matriz[7] != ' ')
        return 1;
        
    else if (matriz[2] == matriz[5] && matriz[5] == matriz[8] && matriz[8] != ' ')
        return 1;
        
    else if (matriz[3] == matriz[6] && matriz[6] == matriz[9] && matriz[9] != ' ')
        return 1;
        
    else if (matriz[1] == matriz[5] && matriz[5] == matriz[9] && matriz[9] != ' ')
        return 1;
        
    else if (matriz[3] == matriz[5] && matriz[5] == matriz[7] && matriz[7] != ' ')
        return 1;
	//alguma opção disponivel
    else if (matriz[1] == ' ' || matriz[2] == ' ' || matriz[3] == ' ' || matriz[4] == ' ' || matriz[5] == ' ' || matriz[6] == ' ' || matriz[7] == ' ' || matriz[8] == ' ' || matriz[9] == ' ')
        return 2;
	//todas as opções preenchidas        
    else
        return 3;
}

void mostrar_matriz()
{
    clear();
    printf("Player 1 (X)\nPlayer 2 (O)\n\n");

    printf("        1     2     3  \n");
    printf("                       \n");
    printf("           |     |     \n");
    printf("  1     %c  |  %c  |  %c \n", matriz[1], matriz[2], matriz[3]);

    printf("      _____|_____|_____\n");
    printf("           |     |     \n");

    printf("  2     %c  |  %c  |  %c \n", matriz[4], matriz[5], matriz[6]);

    printf("      _____|_____|_____\n");
    printf("           |     |     \n");

    printf("  3     %c  |  %c  |  %c \n", matriz[7], matriz[8], matriz[9]);

    printf("           |     |     \n\n");
}

int main(int argc, char* argv[]) {

	TJogada jogada;

	char temp_caractere_jogador;

	int posicao = 0;
	while(jogada_result == 2){
		mostrar_matriz();
	
		printf("aaaa %d:\n", jogada_result);
		printf("Insira a posição x:\n");
		scanf("%d", &jogada.x);
	
		printf("Insira a posição y:\n");
		scanf("%d", &jogada.y);
	
		printf("Insira o caractere do jogador(X ou O):\n");
		scanf("%s", &temp_caractere_jogador);

		posicao = (jogada.y -1) * 3 + (jogada.x);
		if(matriz[posicao] == ' '){
			matriz[0] = matriz[posicao] = temp_caractere_jogador;
			jogada_result = verifica_fim_jogo();
		}
		else{
			printf("Posição já preenchida. Utilize um campo vazio:\n");
		}
	}

	mostrar_matriz();
		printf("aaaa %d:\n", jogada_result);
	int numero_jogador = 2;
	if(matriz[0] == 'X'){
		numero_jogador = 1;
	}

	printf("Fim do jogo. O jogador %d ganhou!\n", numero_jogador);
}

