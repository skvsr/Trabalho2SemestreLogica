#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

// struct do personagem
typedef struct {
    char nome[50];
    int forca;
    int velocidade;
    int x, y;
} Personagem;

// struct do inimigo
typedef struct {
    int forca;
    int x, y;
    int vida;
} Inimigo;

// struct de item
typedef struct {
    int valor;
    int x, y;
} Item;

// Funcao do personagem
void criaPersonagem(Personagem *p, char nome[50], int forca, int velocidade) {
    printf("Digite o nome do personagem: ");
    fgets(p->nome, sizeof(p->nome), stdin);
    p->nome[strcspn(p->nome, "\n")] = 0;
    p->forca = 10;
    p->velocidade = 1;
    p->x = 0;
    p->y = 0;
}

// posicao no mapa
int posicao(int x, int y, Inimigo Inimigos[], int numInimigos , Item Itens[], int numItens) {
    for (int i = 0; i < numInimigos; i++) {
        if (Inimigos[i].x == x && Inimigos[i].y == y && Inimigos[i].vida) {
            return 1;
        }
    }
    for (int i = 0; i < numItens; i++) {
        if (Itens[i].x == x && Itens[i].y == y) {
            return 2;
        }
    }
    return 0;
}

// Funcao do inimigo
void criarInimigo(Inimigo Inimigos[], int quantidade, int tamanhoX, int tamanhoY, Item Itens[], int numItens) {
    for (int i = 0; i < quantidade; i++) {
        do {
            Inimigos[i].x = rand() % tamanhoX;
            Inimigos[i].y = rand() % tamanhoY;
        } while (posicao(Inimigos[i].x, Inimigos[i].y, Inimigos, i, Itens, numItens));
        Inimigos[i].forca = rand() % 10 + 1;
        Inimigos[i].vida = 1;
    }
}

// Funcao do item
void criarItem(Item Itens[], int quantidade, int tamanhoX, int tamanhoY, Inimigo Inimigos[], int numInimigos) {
    for (int i = 0; i < quantidade; i++) {
        do {
            Itens[i].x = rand() % tamanhoX;
            Itens[i].y = rand() % tamanhoY;
        } while (posicao(Itens[i].x, Itens[i].y, Inimigos, numInimigos, Itens, i));
        Itens[i].valor = rand() % 10 + 1;
    }
}

// Funcao do mapa
void exibirMapa(int tamanhoX, int tamanhoY, Personagem *p, Inimigo Inimigos[], int numInimigos, Item Itens[], int numItens) {
    //borda superior
    printf("  ");
    for (int i = 0; i < tamanhoX; i++) {
        printf("--");
    }
    printf("-\n");
    
    for (int i = 0; i < tamanhoY; i++) {
        printf("| "); // borda lateral
        for (int j = 0; j < tamanhoX; j++) {
            if (i == p->y && j == p->x) {
                printf("P ");
            } else if (posicao(j, i, Inimigos, numInimigos, Itens, numItens) == 1) {
                printf("I ");
            } else if (posicao(j, i, Inimigos, numInimigos, Itens, numItens) == 2) {
                printf("T ");
            } else {
                printf(". ");
            }
        }
        printf("|\n"); // borda lateral
        printf("\n");
    }
    //borda inferior
    printf("  ");
    for (int i = 0; i < tamanhoX; i++) {
        printf("--");
    }
    printf("-\n");
}

void Legendas() {
    printf("\nLegenda:\n");
    printf("P - Personagem\n");
    printf("I - Inimigo\n");
    printf("T - Item\n");
    printf(". - Espaço vazio\n");
}

// Funcao de encontros
void encontros(Personagem *p, Inimigo Inimigos[], int numInimigos, Item Itens[], int numItens) {
    for (int i = 0; i < numInimigos; i++) {
        if (Inimigos[i].x == p->x && Inimigos[i].y == p->y && Inimigos[i].vida) {
            printf("Voce encontrou um inimigo!\n");
            printf("Forca do inimigo: %d\n", Inimigos[i].forca);
            printf("Sua forca: %d\n", p->forca);
            if (p->forca > Inimigos[i].forca) {
                printf("Voce venceu o inimigo!\n");
                Inimigos[i].vida = 0;
            } else {
                printf("Voce perdeu para o inimigo!\n");
                exit(0);
            }
        }
    }
    for (int i = 0; i < numItens; i++) {
        if (Itens[i].x == p->x && Itens[i].y == p->y) {
            printf("Voce encontrou um item!\n");
            printf("Valor do item: %d\n", Itens[i].valor);
            p->forca += Itens[i].valor;
            Itens[i].valor = 0;
        }
    }
}
// Funcao de movimento
void mover(Personagem *p, char direcao, int tamanhoX, int tamanhoY) {
    switch (direcao) {
        case 'w':
            if (p->y > 0) p->y--;
            break;
        case 's':
            if (p->y < tamanhoY - 1) p->y++;
            break;
        case 'a':
            if (p->x > 0) p->x--;
            break;
        case 'd':
            if (p->x < tamanhoX - 1) p->x++;
            break;
        default:
            printf("Direcao invalida!\n");
    }
}


int main() {

    printf("--------------------------------------------------------------------------------\n");
	printf("                              BEM VINDO AO                                      \n");
	printf("                                RPG GAME!                                        \n");
	printf("--------------------------------------------------------------------------------\n");
	
	Sleep(3);
	
	printf("                                   BY                                            \n");
	printf("                              Henrique Costa                                     \n");
	
	Sleep(1);
	printf("                             Samuel Pinheiro                                     \n");
	
	Sleep(2);
	
	printf("                               Samuel Vitor                                      \n");
	
    return 0;
}