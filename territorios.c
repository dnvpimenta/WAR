#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da struct Territorio
// Esta struct armazena informações sobre um território no jogo
typedef struct {
    char nome[30];      // Nome do território (até 29 caracteres + '\0')
    char cor[10];       // Cor do exército que ocupa o território
    int tropas;         // Quantidade de tropas no território
} Territorio;

// Protótipos das funções
Territorio* cadastrarTerritorios(int quantidade);
void exibirTerritorios(Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

int main() {
    int quantidadeTerritorios;
    int opcao;
    int indiceAtacante, indiceDefensor;
    
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    printf("=== SISTEMA DE BATALHA DE TERRITORIOS ===\n\n");
    
    // Solicita a quantidade de territórios
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &quantidadeTerritorios);
    
    // Aloca memória dinamicamente para os territórios
    Territorio* mapa = cadastrarTerritorios(quantidadeTerritorios);
    
    if (mapa == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }
    
    // Menu principal
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Exibir territorios\n");
        printf("2. Realizar ataque\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                // Exibe todos os territórios
                exibirTerritorios(mapa, quantidadeTerritorios);
                break;
                
            case 2:
                // Realiza um ataque
                if (quantidadeTerritorios < 2) {
                    printf("E necessario pelo menos 2 territorios para atacar!\n");
                    break;
                }
                
                // Mostra os territórios disponíveis
                printf("\n=== TERRITORIOS DISPONIVEIS ===\n");
                exibirTerritorios(mapa, quantidadeTerritorios);
                
                // Seleciona território atacante
                printf("\nSelecione o territorio ATACANTE (1 a %d): ", quantidadeTerritorios);
                scanf("%d", &indiceAtacante);
                
                // Valida índice do atacante
                if (indiceAtacante < 1 || indiceAtacante > quantidadeTerritorios) {
                    printf("Indice invalido!\n");
                    break;
                }
                
                // Seleciona território defensor
                printf("Selecione o territorio DEFENSOR (1 a %d): ", quantidadeTerritorios);
                scanf("%d", &indiceDefensor);
                
                // Valida índice do defensor
                if (indiceDefensor < 1 || indiceDefensor > quantidadeTerritorios) {
                    printf("Indice invalido!\n");
                    break;
                }
                
                // Ajusta índices para base 0
                indiceAtacante--;
                indiceDefensor--;
                
                // Valida se não está atacando o próprio território
                if (indiceAtacante == indiceDefensor) {
                    printf("Nao pode atacar seu proprio territorio!\n");
                    break;
                }
                
                // Valida se não está atacando território da mesma cor
                if (strcmp(mapa[indiceAtacante].cor, mapa[indiceDefensor].cor) == 0) {
                    printf("Nao pode atacar territorio da mesma cor!\n");
                    break;
                }
                
                // Realiza o ataque usando ponteiros
                printf("\n=== SIMULACAO DE ATAQUE ===\n");
                atacar(&mapa[indiceAtacante], &mapa[indiceDefensor]);
                
                // Exibe o estado após o ataque
                printf("\n=== ESTADO APOS O ATAQUE ===\n");
                exibirTerritorios(mapa, quantidadeTerritorios);
                break;
                
            case 3:
                printf("Encerrando o programa...\n");
                break;
                
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 3);
    
    // Libera a memória alocada dinamicamente
    liberarMemoria(mapa);
    
    return 0;
}

// Função para cadastrar territórios
// Aloca memória dinamicamente e preenche os dados
Territorio* cadastrarTerritorios(int quantidade) {
    // Aloca memória usando calloc (inicializa com zeros)
    Territorio* mapa = (Territorio*)calloc(quantidade, sizeof(Territorio));
    
    if (mapa == NULL) {
        return NULL;
    }
    
    printf("\n=== CADASTRO DE %d TERRITORIOS ===\n\n", quantidade);
    
    // Preenche os dados de cada território
    for (int i = 0; i < quantidade; i++) {
        printf("Cadastro do Territorio %d:\n", i + 1);
        
        // Leitura do nome do território
        printf("Nome do territorio: ");
        scanf("%29s", mapa[i].nome);
        
        // Leitura da cor do exército
        printf("Cor do exercito: ");
        scanf("%9s", mapa[i].cor);
        
        // Leitura do número de tropas
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        
        printf("\n");
    }
    
    return mapa;
}

// Função para exibir todos os territórios
// Recebe ponteiro para o vetor de territórios
void exibirTerritorios(Territorio* mapa, int quantidade) {
    printf("\n=== LISTA DE TERRITORIOS ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%d. %s (Cor: %s, Tropas: %d)\n", 
               i + 1, 
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
}

// Função para simular um ataque entre dois territórios
// Recebe ponteiros para o atacante e defensor
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("%s (%s) esta atacando %s (%s)!\n", 
           atacante->nome, atacante->cor, 
           defensor->nome, defensor->cor);
    
    // Simula rolagem de dados (1 a 6) para ambos os lados
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);
    
    // Verifica resultado do ataque
    if (dadoAtacante > dadoDefensor) {
        // Atacante vence
        printf("\nVITORIA DO ATACANTE!\n");
        
        // O defensor muda de dono (cor)
        printf("%s agora pertence aos %s!\n", defensor->nome, atacante->cor);
        strcpy(defensor->cor, atacante->cor);
        
        // O defensor recebe metade das tropas do atacante (mínimo 1)
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas < 1) {
            tropasTransferidas = 1;
        }
        
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
        
        printf("%d tropas foram transferidas para %s.\n", 
               tropasTransferidas, defensor->nome);
        
    } else if (dadoAtacante < dadoDefensor) {
        // Defensor vence
        printf("\nVITORIA DO DEFENSOR!\n");
        
        // O atacante perde uma tropa
        if (atacante->tropas > 0) {
            atacante->tropas--;
            printf("%s perdeu 1 tropa.\n", atacante->nome);
        }
        
        // O defensor perde metade de suas tropas (mínimo 1)
        int perdaDefensor = defensor->tropas / 2;
        if (perdaDefensor < 1 && defensor->tropas > 0) {
            perdaDefensor = 1;
        }
        defensor->tropas -= perdaDefensor;
        
        printf("%s perdeu %d tropas.\n", defensor->nome, perdaDefensor);
        
    } else {
        // Empate - ambos perdem tropas
        printf("\nEMPATE! Ambos perdem tropas.\n");
        
        // Atacante perde uma tropa
        if (atacante->tropas > 0) {
            atacante->tropas--;
            printf("%s perdeu 1 tropa.\n", atacante->nome);
        }
        
        // Defensor perde uma tropa
        if (defensor->tropas > 0) {
            defensor->tropas--;
            printf("%s perdeu 1 tropa.\n", defensor->nome);
        }
    }
    
    // Garante que as tropas não ficam negativas
    if (atacante->tropas < 0) atacante->tropas = 0;
    if (defensor->tropas < 0) defensor->tropas = 0;
}

// Função para liberar a memória alocada dinamicamente
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("Memoria liberada com sucesso!\n");
    }
}