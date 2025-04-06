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

// Ponteiros
Personagem *p = NULL; // Ponteiro para o personagem
Inimigo *inimigos = NULL; // Ponteiro para os inimigos
Item *itens = NULL; // Ponteiro para os itens
int numInimigos, numItens, tamanhoX, tamanhoY; // Variaveis globais para o tamanho do mapa e o numero de inimigos e itens

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
            Sleep(3000);
            if (p->forca > Inimigos[i].forca) {
                printf("Voce venceu o inimigo!\n");
                Inimigos[i].vida = 0;
                Sleep(1200);
            } else {
                printf("Este inimigo é mais forte que voce agora!");
                printf("Deseja fugir? (s/n)");
                char escolha;
                scanf(" %c", &escolha);
                if (escolha == 's' || escolha == 'S') {
                    int chanceFugir = rand() % 10; // Chance de fugir
                    if (chanceFugir < p->velocidade) {
                        printf("Voce conseguiu fugir!\n");
                        return;
                    } else {
                        printf("Voce nao conseguiu fugir e foi derrotado!\n");
                        exit(0); // Encerra o jogo se o personagem não conseguir fugir
                    } 
                }
            }
        }
    }

    for (int i = 0; i < numItens; i++) {
        if (Itens[i].x == p->x && Itens[i].y == p->y && Itens[i].valor > 0) {
            printf("Voce encontrou um item!\n");
            printf("Valor do item: %d\n", Itens[i].valor);
            int tipo = rand() % 2; // Tipo do item (0 = forca, 1 = velocidade)
            if (tipo == 0) {
                printf("Este item aumentou sua forca!\n");
                p->forca += Itens[i].valor; // Aumenta a forca do personagem
            } else { printf("Este item aumentou sua velocidade!\n");
                p->velocidade += Itens[i].valor; // Aumenta a velocidade do personagem
            } Itens[i].valor = 0; // Remove o item do mapa
        }
    }
}
// Funcao de movimento
void mover(Personagem *p, char direcao, int tamanhoX, int tamanhoY) {
    int rapido = p->velocidade <= 2 ? p->velocidade : 2;
    int passos;
    printf("Quantos passos deseja andar? (1 até %d): ", rapido);
    scanf("%d", &passos); // Leitura do numero de passos
    if (passos < 1 ) passos = 1; // Verifica se o numero de passos é menor que 1, se sim, passos = 1
    if (passos > rapido) passos = rapido; // Verifica se o numero de passos é maior que a velocidade do personagem, se sim, rapido = velocidade
    switch (direcao) {
        case 'w':
            if (p->y - passos >= 0) p->y-= passos;
            else p->y = 0;
            break;
        case 's':
            if (p->y + passos < tamanhoY) p->y+= passos;
            else p->y = tamanhoY - 1;
            break;
        case 'a':
            if (p->x - passos  >= 0) p->x-= passos;
            else p->x = 0;
            break;
        case 'd':
            if (p->x + passos < tamanhoX) p->x+= passos;
            else p->x = tamanhoX - 1;
            break;
        default:
            printf("Direcao invalida!\n");
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


int main() {
    srand(time(NULL));
    char direcao;
    setlocale (LC_ALL,"Portuguese");

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
        printf("Digite a direcao (w/a/s/d), 'e' para ver status e 'q' para sair: ");
        scanf(" %c", &direcao);
        if (direcao == 'q') {
            salvarJogo();
            break;
        }
    }
}


    liberarMemoria();
    return 0;
}