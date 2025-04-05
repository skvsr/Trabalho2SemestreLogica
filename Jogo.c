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

void statusPersonagem(Personagem *p) {
    printf("\n===Status do Personagem===\n");
    printf("Nome: %s\n", p->nome);
    printf("Forca: %d\n", p->forca);
    printf("Velocidade: %d\n", p->velocidade);
    printf("Posicao: (%d, %d)\n", p->x, p->y);
    printf("==========================\n");
}

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
        return;
        }
        fread(p, sizeof(Personagem), 1, file); // Leitura do personagem
        char nomeTemp[50]; //- Buffer temporário para nome
        fread(nomeTemp, sizeof(char), 50, file); //- Lê o nome salvo
        p->nome = (char *)malloc(strlen(nomeTemp) + 1); //- Aloca tamanho exato
        if (!p->nome) {
            printf("Erro ao alocar memoria para o nome do personagem!\n");
            free(p); //- Libera personagem
            fclose(file);
            return;
        }
        strcpy(p->nome, nomeTemp); //- Copia nome
    fread(&tamanhoX, sizeof(int), 1, file); // Leitura do tamanho do mapa
    fread(&tamanhoY, sizeof(int), 1, file); // Leitura do tamanho do mapa
    fread(&numInimigos, sizeof(int), 1, file); // Leitura do numero de inimigos
    fread(&numItens, sizeof(int), 1, file); // Leitura do numero de itens
    inimigos = (Inimigo *)malloc(numInimigos * sizeof(Inimigo)); // Alocação dinamica para os inimigos
        if (!inimigos) {
        printf("Erro ao alocar memoria para os inimigos!\n");
        fclose(file);
        return;
        }
    itens = (Item *)malloc(numItens * sizeof(Item)); // Alocação dinamica para os itens
        if (!itens) {
        printf("Erro ao alocar memoria para os itens!\n");
        fclose(file);
        return;
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

    exibirMapa(tamanhoX, tamanhoY, p, NULL, NULL, 0, NULL, 0); // Exibe o mapa, se Deus quiser vazio

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
    fgets(p->nome, 50, stdin); // Leitura do nome do personagem
    p->nome[strcspn(p->nome, "\n")] = 0; // Remove o \n do final da string
    p->forca = 10; // Forca inicial do personagem
    p->velocidade = 1; // Velocidade inicial do personagem
    p->x = 0; // Posicao inicial do personagem
    p->y = 0; // Posicao inicial do personagem
}

// posicao no mapa
int posicao(int x, int y, Inimigo Inimigos[], int numInimigos , Item Itens[], int numItens) {
    for (int i = 0; i < numInimigos; i++) { // Verifica se a posicao ja esta ocupada por um inimigo
        if (Inimigos[i].x == x && Inimigos[i].y == y && Inimigos[i].vida) { // Verifica se o inimigo esta vivo
            return 1;
        }
    }
    for (int i = 0; i < numItens; i++) { // Verifica se a posicao ja esta ocupada por um item
        if (Itens[i].x == x && Itens[i].y == y && Itens[i].valor > 0) { // Verifica se o item esta vivo
            return 2;
        }
    }
    return 0;
}

// Funcao do inimigo
void criarInimigo(Inimigo Inimigos[], int quantidade, int tamanhoX, int tamanhoY, Item Itens[], int numItens) {
    for (int i = 0; i < quantidade; i++) {
        do {
            Inimigos[i].x = rand() % tamanhoX; // Gera uma posicao aleatoria para o inimigo
            Inimigos[i].y = rand() % tamanhoY; // Gera uma posicao aleatoria para o inimigo
        } while (posicao(Inimigos[i].x, Inimigos[i].y, Inimigos, i, Itens, numItens) ||
        (p && p->x == Inimigos[i].x && p->y == Inimigos[i].y)); // Verifica se a posicao ja esta ocupada por um inimigo ou pelo personagem
        Inimigos[i].forca = rand() % 10 + 1; // Gera uma forca aleatoria para o inimigo
        Inimigos[i].vida = 1; // Inimigo vivo
    }
}

// Funcao do item
void criarItem(Item Itens[], int quantidade, int tamanhoX, int tamanhoY, Inimigo Inimigos[], int numInimigos) {
    for (int i = 0; i < quantidade; i++) {
        do {
            Itens[i].x = rand() % tamanhoX; // Gera uma posicao aleatoria para o item
            Itens[i].y = rand() % tamanhoY; // Gera uma posicao aleatoria para o item
        } while (posicao(Itens[i].x, Itens[i].y, Inimigos, numInimigos, Itens, i) ||
        (p && p->x == Itens[i].x && p->y == Itens[i].y)); // Verifica se a posicao ja esta ocupada por um inimigo ou pelo personagem
        Itens[i].valor = rand() % 10 + 1; // Gera um valor aleatorio para o item
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
            if (p != NULL && i == p->y && j == p->x) {
                printf("P ");
                continue; // Se for a posicao do personagem, imprime P e continua
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
    int rapido = p->velocidade <= 2 ? p->velocidade : 2; // Verifica se a velocidade é menor ou igual a 1, se sim, rapido = 1, se não, rapido = velocidade
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
        free (p); // Libera a memoria do nome
    }
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

    if (direcao == 'C' || direcao == 'c') { // Carregar jogo
        carregarJogo();
    } else {
        iniciar(); // Iniciar novo jogo
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
        } else if (direcao == 'e') {
            statusPersonagem(p); // Exibe o status do personagem
            Sleep(5000); // Pausa para o jogador ver o status
            continue;
        }
        mover(p, direcao, tamanhoX, tamanhoY); 
        encontros(p, inimigos, numInimigos, itens, numItens);
        Sleep(200); // Pausa para o jogador ver o mapa
    }
    
    liberarMemoria(); // Libera a memoria alocada
    return 0;
}