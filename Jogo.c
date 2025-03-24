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

int main() {
    int hj;
    int nasc;
    printf("Bem vindo ao jogo de adivinhacao\n");
    return 0;
}