#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

// struct do personagem
typedef struct {
    char *nome;
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

// Ponteiros
Personagem *p = NULL; // Ponteiro para o personagem
Inimigo *inimigos = NULL; // Ponteiro para os inimigos
Item *itens = NULL; // Ponteiro para os itens
int numInimigos, numItens, tamanhoX, tamanhoY; // Variaveis globais para o tamanho do mapa e o numero de inimigos e itens

void salvarJogo() {
    FILE *file = fopen("savegame.dat", "wb");
    if (!file) {
        printf("Erro ao salvar o jogo!\n");
        return;
    }
    fwrite(p, sizeof(Personagem), 1, file); // Salva o personagem
    fwrite(p->nome, sizeof(char), strlen(p->nome) + 1, file); // Salva o nome do personagem
    fwrite(&tamanhoX, sizeof(int), 1, file); // Salva o tamanho do mapa
    fwrite(&tamanhoY, sizeof(int), 1, file); // Salva o tamanho do mapa
    fwrite(&numInimigos, sizeof(int), 1, file); // Salva o numero de inimigos
    fwrite(&numItens, sizeof(int), 1, file); // Salva o numero de itens
    fwrite(inimigos, sizeof(Inimigo), numInimigos, file); // Salva os inimigos
    fwrite(itens, sizeof(Item), numItens, file); // Salva os itens
    fclose(file);
    printf("Jogo salvo com sucesso!\n");
}

void carregarJogo() {
    FILE *file = fopen("savegame.dat", "rb");
    if (!file) {
        printf("Nenhum jogo salvo encontrado!\n");
        return;
    }
    p = (Personagem *)malloc(sizeof(Personagem)); // Alocação dinamica para o personagem
        if (!p) {
        printf("Erro ao alocar memoria para o personagem!\n");
        fclose(file);
        }
    p->nome = (char *)malloc(50 * sizeof(char)); // Alocação dinamica para o nome
        if (!p->nome) {
            printf("Erro ao alocar memoria para o nome do personagem!\n");
            fclose(file);
        }
    fread(p, sizeof(Personagem), 1, file); // Leitura do personagem
    fread(p->nome, sizeof(char), 50, file); // Leitura do nome do personagem
    fread(&tamanhoX, sizeof(int), 1, file); // Leitura do tamanho do mapa
    fread(&tamanhoY, sizeof(int), 1, file); // Leitura do tamanho do mapa
    fread(&numInimigos, sizeof(int), 1, file); // Leitura do numero de inimigos
    fread(&numItens, sizeof(int), 1, file); // Leitura do numero de itens
    inimigos = (Inimigo *)malloc(numInimigos * sizeof(Inimigo)); // Alocação dinamica para os inimigos
        if (!inimigos) {
        printf("Erro ao alocar memoria para os inimigos!\n");
        fclose(file);
        }
    itens = (Item *)malloc(numItens * sizeof(Item)); // Alocação dinamica para os itens
        if (!itens) {
        printf("Erro ao alocar memoria para os itens!\n");
        fclose(file);
        }
    fread(inimigos, sizeof(Inimigo), numInimigos, file); // Leitura dos inimigos 
    fread(itens, sizeof(Item), numItens, file); // Leitura dos itens
    fclose(file);
    printf("Jogo carregado com sucesso!\n");
}
void iniciar() {
    printf("Digite o tamanho do mapa (Largura Altura): ");
    scanf("%d %d", &tamanhoX, &tamanhoY);
    getchar(); // Limpa o buffer do teclado

    p = (Personagem *)malloc(sizeof(Personagem));
    p->nome = (char *)malloc(50 * sizeof(char));

    numInimigos = (tamanhoX * tamanhoY) / 4; // Calculo do numero de inimigos
    numItens = (tamanhoX * tamanhoY) / 4; // Calculo do numero de itens

    inimigos = (Inimigo *)malloc(numInimigos * sizeof(Inimigo));
    itens = (Item *)malloc(numItens * sizeof(Item));
    criarInimigo(inimigos, numInimigos, tamanhoX, tamanhoY, itens, numItens); // Cria os inimigos
    criarItem(itens, numItens, tamanhoX, tamanhoY, inimigos, numInimigos); // Cria os itens
}

// Funcao do personagem
void criaPersonagem(Personagem *p) {
    printf("Digite o nome do personagem: ");
    fgets(p->nome, 50, stdin);
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
    printf(". - Espaï¿½o vazio\n");
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
    int rapido = p->velocidade;
    switch (direcao) {
        case 'w':
            if (p->y - rapido >= 0) p->y-= rapido;
            else p->y = 0;
            break;
        case 's':
            if (p->y + rapido < tamanhoY) p->y+= rapido;
            else p->y = tamanhoY - 1;
            break;
        case 'a':
            if (p->x - rapido  >= 0) p->x-= rapido;
            else p->x = 0;
            break;
        case 'd':
            if (p->x + rapido < tamanhoX) p->x+= rapido;
            else p->x = tamanhoX - 1;
            break;
        default:
            printf("Direcao invalida!\n");
    }
}

void liberarMemoria() {
    if (p) {
        free(p->nome);
        free (p); // Libera a memoria do nome
    }
    free(p); // Libera a memoria do personagem
    free(inimigos); // Libera a memoria dos inimigos
    free(itens); // Libera a memoria dos itens
}


int main() {
    srand(time(NULL)); // Inicializa o gerador de numeros aleatorios
    char direcao; // Variavel para a direcao do movimento e opção

    printf("--------------------------------------------------------------------------------\n");
	printf("                              BEM VINDO AO                                      \n");
	printf("                                RPG GAME!                                        \n");
	printf("--------------------------------------------------------------------------------\n");
	
	Sleep(2000);
	
	printf("                                   BY                                            \n");
	printf("                              Henrique Costa                                     \n");
	
	Sleep(1000);
	printf("                             Samuel Pinheiro                                     \n");
	
	Sleep(1000);
	
	printf("                               Samuel Vitor                                      \n");

    printf("Deseja iniciar um novo jogo ou carregar um ja existente?(N para Novo jogo, C para Carregar existente\n");
    scanf(" %c", &direcao);
    getchar(); 

    if (direcao == 'C' || direcao == 'c') {
        carregarJogo();
    } else {
        iniciar();
        criaPersonagem(p);

    }
    
    while (1) {
        system("cls");
        exibirMapa(tamanhoX, tamanhoY, p, inimigos, numInimigos, itens, numItens);
        Legendas();
        printf("Digite a direcao (w/a/s/d) ou 'q' para sair: ");
        scanf(" %c", &direcao);
        if (direcao == 'q') {
            salvarJogo();
            break;
        }
        mover(p, direcao, tamanhoX, tamanhoY);
        encontros(p, inimigos, numInimigos, itens, numItens);
    }
    
    liberarMemoria(); // Libera a memoria alocada
    return 0;
}