#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// struct do personagem
typedef struct {
    char nome[50];
    int forca;
    int velocidade;
} Personagem;

// struct do inimigo
typedef struct {
    int forca;
    int velocidade;
} Inimigo;

// struct de item
typedef struct {
    int valor;
} Item;

// Funcao do personagem
Personagem criaPersonagem(char nome[50], int forca, int velocidade) {
    Personagem p;
    printf("Digite o nome do personagem: ");
    scanf("%s", p.nome);
    p.forca = forca;
    p.velocidade = 1;
    return p;
}

// Funcao do inimigo
Inimigo criaInimigo(int forca, int velocidade) {
    Inimigo i;
    i.forca = forca;
    i.velocidade = velocidade;
    return i;
}

// Funcao do item
Item criaItem(int valor) {
    Item item;
    item.valor = valor;
    return item;
}

int main() {
    int hj;
    int nasc;
    printf("Bem vindo ao jogo de adivinhacao\n");
    return 0;
}