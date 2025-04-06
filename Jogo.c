#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <locale.h>
#define LINHAS_LEGENDA 5

// ==== Structs ====
typedef struct {
    char *nome;
    int forca;
    int velocidade;
    int x, y;
} Personagem;

typedef struct {
    int forca;
    int x, y;
    int vida;
} Inimigo;

typedef struct {
    int valor;
    int x, y;
} Item;

// ==== Variáveis Globais ====
Personagem *p = NULL;
Inimigo *inimigos = NULL;
Item *itens = NULL;
int numInimigos, numItens, tamanhoX, tamanhoY;

// ==== Protótipos ====
void statusPersonagem(Personagem *p);
void salvarJogo();
void carregarJogo();
void iniciar();
void criaPersonagem(Personagem *p);
int posicao(int x, int y, Inimigo Inimigos[], int numInimigos , Item Itens[], int numItens);
void criarInimigo(Inimigo Inimigos[], int quantidade, int tamanhoX, int tamanhoY, Item Itens[], int numItens);
void criarItem(Item Itens[], int quantidade, int tamanhoX, int tamanhoY, Inimigo Inimigos[], int numInimigos);
void exibirMapa(int tamanhoX, int tamanhoY, Personagem *p, Inimigo Inimigos[], int numInimigos, Item Itens[], int numItens);
void Legendas();
void encontros(Personagem *p, Inimigo Inimigos[], int numInimigos, Item Itens[], int numItens);
void mover(Personagem *p, char direcao, int tamanhoX, int tamanhoY);
void liberarMemoria();

// ==== Implementações ====

Personagem jogador;
Inimigo inimigo;
Item item;

void delay(int milissegundos) {
    Sleep(milissegundos);
}
void statusPersonagem(Personagem *p) {
    printf("\n=== Status do Personagem ===\n");
    printf("Nome: %s\n", p->nome);
    printf("Força: %d\n", p->forca);
    printf("Velocidade: %d\n", p->velocidade);
    printf("Posição: (%d, %d)\n", p->x, p->y);
    printf("==========================\n");
    Sleep(900);
}

void salvarJogo() {
    FILE *arquivo = fopen("save.txt", "wb");
    if (!arquivo) {
        printf("Erro ao salvar o jogo!\n");
        return;
    }

    int nomeLen = strlen(p->nome) + 1;
    fwrite(&nomeLen, sizeof(int), 1, arquivo);
    fwrite(p->nome, sizeof(char), nomeLen, arquivo);
    fwrite(&p->forca, sizeof(int), 1, arquivo);
    fwrite(&p->velocidade, sizeof(int), 1, arquivo);
    fwrite(&p->x, sizeof(int), 1, arquivo);
    fwrite(&p->y, sizeof(int), 1, arquivo);

    fwrite(&tamanhoX, sizeof(int), 1, arquivo);
    fwrite(&tamanhoY, sizeof(int), 1, arquivo);
    fwrite(&numInimigos, sizeof(int), 1, arquivo);
    fwrite(inimigos, sizeof(Inimigo), numInimigos, arquivo);
    fwrite(&numItens, sizeof(int), 1, arquivo);
    fwrite(itens, sizeof(Item), numItens, arquivo);

    fclose(arquivo);
    printf("Jogo salvo com sucesso!\n");
}


void iniciar() {
    printf("Digite o tamanho do mapa (Largura Altura): ");
    scanf("%d %d", &tamanhoX, &tamanhoY);
    getchar();
    exibirMapa(tamanhoX, tamanhoY, p, NULL, 0, NULL, 0);

    p = (Personagem *)malloc(sizeof(Personagem));
    p->nome = (char *)malloc(50 * sizeof(char));

    numInimigos = (tamanhoX * tamanhoY) / 4;
    numItens = (tamanhoX * tamanhoY) / 4;

    inimigos = (Inimigo *)malloc(numInimigos * sizeof(Inimigo));
    itens = (Item *)malloc(numItens * sizeof(Item));
    criarInimigo(inimigos, numInimigos, tamanhoX, tamanhoY, itens, numItens);
    criarItem(itens, numItens, tamanhoX, tamanhoY, inimigos, numInimigos);
}

void carregarJogo() {
    FILE *arquivo = fopen("save.txt", "rb");
    if (!arquivo) {
        printf("Nenhum jogo salvo encontrado. Iniciando novo jogo...\n");
        iniciar();
        return;
    }

    p = (Personagem *)malloc(sizeof(Personagem));

    int nomeLen;
    fread(&nomeLen, sizeof(int), 1, arquivo);
    p->nome = (char *)malloc(nomeLen * sizeof(char));
    fread(p->nome, sizeof(char), nomeLen, arquivo);
    fread(&p->forca, sizeof(int), 1, arquivo);
    fread(&p->velocidade, sizeof(int), 1, arquivo);
    fread(&p->x, sizeof(int), 1, arquivo);
    fread(&p->y, sizeof(int), 1, arquivo);

    fread(&tamanhoX, sizeof(int), 1, arquivo);
    fread(&tamanhoY, sizeof(int), 1, arquivo);
    fread(&numInimigos, sizeof(int), 1, arquivo);
    inimigos = (Inimigo *)malloc(numInimigos * sizeof(Inimigo));
    fread(inimigos, sizeof(Inimigo), numInimigos, arquivo);

    fread(&numItens, sizeof(int), 1, arquivo);
    itens = (Item *)malloc(numItens * sizeof(Item));
    fread(itens, sizeof(Item), numItens, arquivo);

    fclose(arquivo);
    printf("Jogo carregado com sucesso!\n");
    Sleep(1000);
}


void criaPersonagem(Personagem *p) {
    printf("Digite o nome do personagem: ");
    fgets(p->nome, 50, stdin);
    p->nome[strcspn(p->nome, "\n")] = 0;
    p->forca = 10;
    p->velocidade = 1;
    p->x = 0;
    p->y = 0;
}

int posicao(int x, int y, Inimigo Inimigos[], int numInimigos , Item Itens[], int numItens) {
    for (int i = 0; i < numInimigos; i++) {
        if (Inimigos[i].x == x && Inimigos[i].y == y && Inimigos[i].vida) return 1;
    }
    for (int i = 0; i < numItens; i++) {
        if (Itens[i].x == x && Itens[i].y == y && Itens[i].valor > 0) return 2;
    }
    return 0;
}

void criarInimigo(Inimigo Inimigos[], int quantidade, int tamanhoX, int tamanhoY, Item Itens[], int numItens) {
    for (int i = 0; i < quantidade; i++) {
        do {
            Inimigos[i].x = rand() % tamanhoX;
            Inimigos[i].y = rand() % tamanhoY;
        } while (posicao(Inimigos[i].x, Inimigos[i].y, Inimigos, i, Itens, numItens) ||
        (p && p->x == Inimigos[i].x && p->y == Inimigos[i].y));
        Inimigos[i].forca = rand() % 10 + 1;
        Inimigos[i].vida = 1;
    }
}

void criarItem(Item Itens[], int quantidade, int tamanhoX, int tamanhoY, Inimigo Inimigos[], int numInimigos) {
    for (int i = 0; i < quantidade; i++) {
        do {
            Itens[i].x = rand() % tamanhoX;
            Itens[i].y = rand() % tamanhoY;
        } while (posicao(Itens[i].x, Itens[i].y, Inimigos, numInimigos, Itens, i) ||
        (p && p->x == Itens[i].x && p->y == Itens[i].y));
        Itens[i].valor = rand() % 10 + 1;
    }
}

void exibirMapa(int tamanhoX, int tamanhoY, Personagem *p, Inimigo Inimigos[], int numInimigos, Item Itens[], int numItens) {
    printf("  ");
    for (int i = 0; i < tamanhoX; i++) printf("--");
    printf("-\n");

    for (int i = 0; i < tamanhoY; i++) {
        printf("| ");
        for (int j = 0; j < tamanhoX; j++) {
            if (p != NULL && i == p->y && j == p->x) printf("P ");
            else if (posicao(j, i, Inimigos, numInimigos, Itens, numItens) == 1) printf("I ");
            else if (posicao(j, i, Inimigos, numInimigos, Itens, numItens) == 2) printf("T ");
            else printf(". ");
        }
        printf("|\n\n");
    }

    printf("  ");
    for (int i = 0; i < tamanhoX; i++) printf("--");
    printf("-\n");
}


void Legendas(char legenda[][40]) {
    strcpy(legenda[0], "Legenda:");
    strcpy(legenda[1], "P - Personagem");
    strcpy(legenda[2], "I - Inimigo");
    strcpy(legenda[3], "T - Item");
    strcpy(legenda[4], ". - Espaço vazio");
}

void exibirPainelLateral(Personagem *p) {
    char legenda[LINHAS_LEGENDA][40];
    Legendas(legenda);

    char status[LINHAS_LEGENDA][40];
    snprintf(status[0], 40, "Status:");
    snprintf(status[1], 40, "Nome: %s", p->nome);
    snprintf(status[2], 40, "Forca: %d", p->forca);
    snprintf(status[3], 40, "Velocidade: %d", p->velocidade);
    snprintf(status[4], 40, "Posição: (%d, %d)", p->x, p->y);

    // Imprime a linha separadora sem os títulos em caixa alta
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < LINHAS_LEGENDA; i++) {
        printf("%-25s\t%-25s\n", legenda[i], status[i]);
    }

    printf("-------------------------------------------------------------\n\n");
}



void encontros(Personagem *p, Inimigo Inimigos[], int numInimigos, Item Itens[], int numItens) {
    for (int i = 0; i < numInimigos; i++) {
        if (Inimigos[i].x == p->x && Inimigos[i].y == p->y && Inimigos[i].vida) {
            printf("Voce encontrou um inimigo!\n");
            printf("Força do inimigo: %d\n", Inimigos[i].forca);
            printf("Sua forca: %d\n", p->forca);
            Sleep(3000);
            if (p->forca > Inimigos[i].forca) {
                printf("Você venceu o inimigo!\n");
                Inimigos[i].vida = 0;
                Sleep(1200);
            } else {
                printf("Este inimigo é mais forte que você agora! Deseja fugir? (s/n) ");
                char escolha;
                scanf(" %c", &escolha);
                if (escolha == 's' || escolha == 'S') {
                    if (rand() % 10 < p->velocidade) {
                        printf("Você conseguiu fugir!\n");
                        Sleep(2000);
                        return;
                    } else {
                        printf("Você não conseguiu fugir e foi derrotado!\n");
                        Sleep(2000);
                        exit(0);
                    }
                }
            }
        }
    }

    for (int i = 0; i < numItens; i++) {
        if (Itens[i].x == p->x && Itens[i].y == p->y && Itens[i].valor > 0) {
            printf("Você encontrou um item!\n");
            printf("Valor do item: %d\n", Itens[i].valor);
            Sleep(1000);
            if (rand() % 2 == 0) {
                printf("Este item aumentou sua força!\n");
                Sleep(1000);
                p->forca += Itens[i].valor;
            } else {
                printf("Este item aumentou sua velocidade!\n");
                Sleep(1000);
                p->velocidade += Itens[i].valor;
            }
            Itens[i].valor = 0;
        }
    }
}

int inimigosRestantes(Inimigo inimigos[], int numInimigos) {
    for (int i = 0; i < numInimigos; i++) {
        if (inimigos[i].vida > 0) {
            return 1; // Ainda existem inimigos vivos
        }
    }
    return 0; // Todos foram derrotados
}


void mover(Personagem *p, char direcao, int tamanhoX, int tamanhoY) {
    int rapido = p->velocidade <= 2 ? p->velocidade : 2;
    int passos;
    printf("Quantos passos deseja andar? (1 até %d): ", rapido);
    scanf("%d", &passos);
    getchar();
    if (passos < 1) passos = 1;
    if (passos > rapido) passos = rapido;

    switch (direcao) {
        case 'w': p->y = (p->y - passos >= 0) ? p->y - passos : 0; break;
        case 's': p->y = (p->y + passos < tamanhoY) ? p->y + passos : tamanhoY - 1; break;
        case 'a': p->x = (p->x - passos >= 0) ? p->x - passos : 0; break;
        case 'd': p->x = (p->x + passos < tamanhoX) ? p->x + passos : tamanhoX - 1; break;
        default: printf("Direção inválida!\n");
    }
}

void liberarMemoria() {
    if (p) {
        free(p->nome);
        free(p);
    }
    free(inimigos);
    free(itens);
}

// ==== Função Principal ====
int main() {
    srand(time(NULL));
    char direcao;
    setlocale (LC_ALL,"Portuguese");

    printf("--------------------------------------------------------------------------------\n");
    printf("                              BEM VINDO AO                                      \n");
    printf("                                RPG GAME!                                       \n");
    printf("--------------------------------------------------------------------------------\n");

    Sleep(2000);
    printf("                                   BY                                           \n");
    printf("                              Henrique Costa                                    \n");
    Sleep(800);
    printf("                             Samuel  Pinheiro                                    \n");
    Sleep(800);
    printf("                               Samuel Vitor                                     \n");
	Sleep(800);
	
    printf("\nDeseja iniciar um novo jogo ou carregar um já existente? (N/C): ");
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
    exibirPainelLateral(p);
    printf("Digite a direção (w/a/s/d), 'q' para sair: ");
    scanf(" %c", &direcao);

    if (direcao == 'q') {
        salvarJogo();
        break;
    } else if (direcao == 'e') {
        statusPersonagem(p);
        system("pause");
    } else {
        mover(p, direcao, tamanhoX, tamanhoY);
        encontros(p, inimigos, numInimigos, itens, numItens);

        // Verifica se venceu o jogo
        if (!inimigosRestantes(inimigos, numInimigos)) {
            printf("\nParabéns! Você derrotou todos os inimigos!\n");
            salvarJogo();
            break;
        }
    }
}


    liberarMemoria();
    return 0;
}