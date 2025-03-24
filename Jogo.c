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
    char nome[50];
    int forca;
    int velocidade;
} Inimigo;

// struct de item
typedef struct {
    char nome[50];
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

int main() {
    int hj;
    int nasc;
    printf("Bem vindo ao jogo de adivinhacao\n");
    return 0;
}