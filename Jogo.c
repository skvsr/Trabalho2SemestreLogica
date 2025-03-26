#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

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
    scanf("%s", p->nome);
    p->forca = forca;
    p->velocidade = 1;
    p->x = 0;
    p->y = 0;
}

// posi��o no mapa
int posicao(int x, int y, Inimigo Inimigos[], num numInimigos , Item Itens[], num numItens) {
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

int main() {

    int tamanhoX = 10, tamanhoY = 10;
    
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