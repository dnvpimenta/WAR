#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da struct Territorio
typedef struct {
    char nome[30];      // Nome do território
    char cor[10];       // Cor do exército que ocupa o território
    int tropas;         // Quantidade de tropas no território
} Territorio;

// Definição da struct Jogador
typedef struct {
    char nome[20];      // Nome do jogador
    char* missao;       // Missão do jogador (alocada dinamicamente)
    char cor[10];       // Cor do jogador
    int territoriosConquistados; // Contador de territórios conquistados
} Jogador;

// Protótipos das funções
Territorio* cadastrarTerritorios(int quantidade);
Jogador* cadastrarJogadores(int quantidade);
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
void exibirMissao(Jogador jogador);
int verificarMissao(Jogador jogador, Territorio* mapa, int tamanhoMapa, Jogador* jogadores, int totalJogadores);
void exibirTerritorios(Territorio* mapa, int quantidade);
void exibirJogadores(Jogador* jogadores, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor, Jogador* jogadorAtacante);
int realizarTurno(Jogador* jogador, Territorio* mapa, int tamanhoMapa, Jogador* jogadores, int totalJogadores);
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int totalJogadores);

int main() {
    int quantidadeTerritorios, quantidadeJogadores;
    int vencedor = -1;
    int turno = 1;
    
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    printf("=== SISTEMA DE BATALHA COM MISSOES ===\n\n");
    
    // Solicita a quantidade de territórios
    printf("Quantos territorios deseja cadastrar? (minimo 5): ");
    scanf("%d", &quantidadeTerritorios);
    
    if (quantidadeTerritorios < 5) {
        printf("Quantidade minima nao atingida. Definindo para 5 territorios.\n");
        quantidadeTerritorios = 5;
    }
    
    // Aloca memória dinamicamente para os territórios
    Territorio* mapa = cadastrarTerritorios(quantidadeTerritorios);
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para os territorios!\n");
        return 1;
    }
    
    // Solicita a quantidade de jogadores
    printf("\nQuantos jogadores deseja cadastrar? (2 a 4): ");
    scanf("%d", &quantidadeJogadores);
    
    if (quantidadeJogadores < 2) {
        printf("Quantidade minima nao atingida. Definindo para 2 jogadores.\n");
        quantidadeJogadores = 2;
    } else if (quantidadeJogadores > 4) {
        printf("Quantidade maxima excedida. Definindo para 4 jogadores.\n");
        quantidadeJogadores = 4;
    }
    
    // Aloca memória dinamicamente para os jogadores
    Jogador* jogadores = cadastrarJogadores(quantidadeJogadores);
    if (jogadores == NULL) {
        printf("Erro ao alocar memoria para os jogadores!\n");
        liberarMemoria(mapa, NULL, 0);
        return 1;
    }
    
    printf("\n=== MISSOES ATRIBUIDAS ===\n");
    for (int i = 0; i < quantidadeJogadores; i++) {
        exibirMissao(jogadores[i]);
    }
    
    // Loop principal do jogo
    printf("\n=== INICIO DO JOGO ===\n");
    while (vencedor == -1) {
        printf("\n=== TURNO %d ===\n", turno);
        
        // Cada jogador realiza seu turno
        for (int i = 0; i < quantidadeJogadores && vencedor == -1; i++) {
            printf("\n--- Vez de %s (%s) ---\n", 
                   jogadores[i].nome, jogadores[i].cor);
            
            // Verifica se o jogador já venceu
            if (verificarMissao(jogadores[i], mapa, quantidadeTerritorios, 
                               jogadores, quantidadeJogadores)) {
                vencedor = i;
                break;
            }
            
            // Exibe situação atual
            printf("\nEstado atual do mapa:\n");
            exibirTerritorios(mapa, quantidadeTerritorios);
            
            // Realiza o turno do jogador
            if (realizarTurno(&jogadores[i], mapa, quantidadeTerritorios, 
                             jogadores, quantidadeJogadores)) {
                // Se o jogador conquistou territórios, verifica se venceu
                if (verificarMissao(jogadores[i], mapa, quantidadeTerritorios, 
                                   jogadores, quantidadeJogadores)) {
                    vencedor = i;
                    break;
                }
            }
        }
        
        turno++;
    }
    
    // Declara o vencedor
    printf("\n\n=== FIM DO JOGO ===\n");
    printf("PARABENS %s! Voce venceu o jogo!\n", jogadores[vencedor].nome);
    printf("Missao cumprida: %s\n", jogadores[vencedor].missao);
    
    // Libera a memória alocada dinamicamente
    liberarMemoria(mapa, jogadores, quantidadeJogadores);
    
    return 0;
}

// Função para cadastrar territórios
// Aloca memória dinamicamente e preenche os dados
Territorio* cadastrarTerritorios(int quantidade) {
    Territorio* mapa = (Territorio*)calloc(quantidade, sizeof(Territorio));
    
    if (mapa == NULL) {
        return NULL;
    }
    
    printf("\n=== CADASTRO DE %d TERRITORIOS ===\n\n", quantidade);
    
    // Cores disponíveis para distribuição inicial
    char* coresDisponiveis[] = {"Vermelho", "Azul", "Verde", "Amarelo"};
    int corIndex = 0;
    
    for (int i = 0; i < quantidade; i++) {
        printf("Cadastro do Territorio %d:\n", i + 1);
        
        printf("Nome do territorio: ");
        scanf("%29s", mapa[i].nome);
        
        // Distribui cores alternadamente para simular ocupação inicial
        strcpy(mapa[i].cor, coresDisponiveis[corIndex % 4]);
        corIndex++;
        
        // Define tropas iniciais aleatórias (1 a 5)
        mapa[i].tropas = (rand() % 5) + 1;
        
        printf("Cor atribuida: %s\n", mapa[i].cor);
        printf("Tropas iniciais: %d\n", mapa[i].tropas);
        
        printf("\n");
    }
    
    return mapa;
}

// Função para cadastrar jogadores
// Aloca memória dinamicamente e atribui missões
Jogador* cadastrarJogadores(int quantidade) {
    Jogador* jogadores = (Jogador*)calloc(quantidade, sizeof(Jogador));
    
    if (jogadores == NULL) {
        return NULL;
    }
    
    // Vetor de missões pré-definidas
    char* missoes[] = {
        "Conquistar 3 territorios consecutivos",
        "Eliminar todas as tropas da cor Vermelha",
        "Controlar pelo menos 50%% dos territorios",
        "Conquistar todos os territorios de uma cor especifica",
        "Ter mais de 20 tropas no total"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);
    
    // Cores disponíveis para jogadores
    char* coresJogadores[] = {"Vermelho", "Azul", "Verde", "Amarelo"};
    
    printf("\n=== CADASTRO DE %d JOGADORES ===\n\n", quantidade);
    
    for (int i = 0; i < quantidade; i++) {
        printf("Cadastro do Jogador %d:\n", i + 1);
        
        printf("Nome do jogador: ");
        scanf("%19s", jogadores[i].nome);
        
        // Atribui cor ao jogador
        strcpy(jogadores[i].cor, coresJogadores[i]);
        printf("Cor atribuida: %s\n", jogadores[i].cor);
        
        // Inicializa contador de territórios conquistados
        jogadores[i].territoriosConquistados = 0;
        
        // Atribui missão aleatória (passagem por referência)
        atribuirMissao(&jogadores[i].missao, missoes, totalMissoes);
        
        printf("\n");
    }
    
    return jogadores;
}

// Função para atribuir uma missão aleatória a um jogador
// Recebe ponteiro para ponteiro para modificar o ponteiro da missão
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    // Sorteia uma missão aleatória
    int indiceMissao = rand() % totalMissoes;
    
    // Aloca memória para armazenar a missão
    *destino = (char*)malloc(strlen(missoes[indiceMissao]) + 1);
    
    if (*destino != NULL) {
        // Copia a missão para a memória alocada
        strcpy(*destino, missoes[indiceMissao]);
    }
}

// Função para exibir a missão de um jogador
// Recebe o jogador por valor (cópia)
void exibirMissao(Jogador jogador) {
    printf("Jogador: %s (%s)\n", jogador.nome, jogador.cor);
    printf("Missao: %s\n", jogador.missao);
    printf("---\n");
}

// Função para verificar se um jogador cumpriu sua missão
// Recebe parâmetros por valor e por referência conforme necessário
int verificarMissao(Jogador jogador, Territorio* mapa, int tamanhoMapa, 
                   Jogador* jogadores, int totalJogadores) {
    // Verifica qual missão o jogador possui e avalia se foi cumprida
    if (strstr(jogador.missao, "Conquistar 3 territorios consecutivos") != NULL) {
        // Missão: Conquistar 3 territórios consecutivos
        int consecutivos = 0;
        for (int i = 0; i < tamanhoMapa - 2; i++) {
            if (strcmp(mapa[i].cor, jogador.cor) == 0 &&
                strcmp(mapa[i+1].cor, jogador.cor) == 0 &&
                strcmp(mapa[i+2].cor, jogador.cor) == 0) {
                return 1; // Missão cumprida
            }
        }
        return 0; // Missão não cumprida
        
    } else if (strstr(jogador.missao, "Eliminar todas as tropas da cor Vermelha") != NULL) {
        // Missão: Eliminar todas as tropas da cor Vermelha
        for (int i = 0; i < tamanhoMapa; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0) {
                return 0; // Ainda há tropas vermelhas
            }
        }
        return 1; // Nenhuma tropa vermelha encontrada
        
    } else if (strstr(jogador.missao, "Controlar pelo menos 50%% dos territorios") != NULL) {
        // Missão: Controlar pelo menos 50% dos territórios
        int territoriosControlados = 0;
        for (int i = 0; i < tamanhoMapa; i++) {
            if (strcmp(mapa[i].cor, jogador.cor) == 0) {
                territoriosControlados++;
            }
        }
        return (territoriosControlados * 100 / tamanhoMapa) >= 50;
        
    } else if (strstr(jogador.missao, "Conquistar todos os territorios de uma cor especifica") != NULL) {
        // Missão: Conquistar todos os territórios de uma cor específica
        // Verifica para cada cor diferente da do jogador
        for (int j = 0; j < totalJogadores; j++) {
            if (strcmp(jogadores[j].cor, jogador.cor) != 0) {
                int todosConquistados = 1;
                for (int i = 0; i < tamanhoMapa; i++) {
                    if (strcmp(mapa[i].cor, jogadores[j].cor) == 0) {
                        todosConquistados = 0;
                        break;
                    }
                }
                if (todosConquistados) return 1;
            }
        }
        return 0;
        
    } else if (strstr(jogador.missao, "Ter mais de 20 tropas no total") != NULL) {
        // Missão: Ter mais de 20 tropas no total
        int totalTropas = 0;
        for (int i = 0; i < tamanhoMapa; i++) {
            if (strcmp(mapa[i].cor, jogador.cor) == 0) {
                totalTropas += mapa[i].tropas;
            }
        }
        return totalTropas > 20;
    }
    
    return 0; // Missão não reconhecida
}

// Função para exibir todos os territórios
void exibirTerritorios(Territorio* mapa, int quantidade) {
    printf("\n=== MAPA ATUAL ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%2d. %-15s | Cor: %-10s | Tropas: %2d\n", 
               i + 1, 
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
}

// Função para exibir informações dos jogadores
void exibirJogadores(Jogador* jogadores, int quantidade) {
    printf("\n=== JOGADORES ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%d. %s (%s) | Missoes cumpridas: %d\n", 
               i + 1, 
               jogadores[i].nome, 
               jogadores[i].cor, 
               jogadores[i].territoriosConquistados);
    }
}

// Função para simular um ataque entre dois territórios
// Recebe ponteiros para os territórios e para o jogador atacante
void atacar(Territorio* atacante, Territorio* defensor, Jogador* jogadorAtacante) {
    printf("\n%s (%s) esta atacando %s (%s)!\n", 
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
        
        // Atualiza contador de territórios conquistados
        jogadorAtacante->territoriosConquistados++;
        
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

// Função para realizar o turno de um jogador
// Retorna 1 se o jogador conquistou territórios, 0 caso contrário
int realizarTurno(Jogador* jogador, Territorio* mapa, int tamanhoMapa, 
                 Jogador* jogadores, int totalJogadores) {
    int opcao;
    int conquistou = 0;
    
    do {
        printf("\nO que deseja fazer?\n");
        printf("1. Atacar um territorio\n");
        printf("2. Ver minha missao\n");
        printf("3. Ver mapa completo\n");
        printf("4. Passar a vez\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1: {
                // Realizar ataque
                int indiceAtacante, indiceDefensor;
                
                // Mostra apenas territórios do jogador que podem atacar
                printf("\nSeus territorios:\n");
                int temTerritorios = 0;
                for (int i = 0; i < tamanhoMapa; i++) {
                    if (strcmp(mapa[i].cor, jogador->cor) == 0 && mapa[i].tropas > 1) {
                        printf("%d. %s (%d tropas)\n", i + 1, mapa[i].nome, mapa[i].tropas);
                        temTerritorios = 1;
                    }
                }
                
                if (!temTerritorios) {
                    printf("Voce nao tem territorios com tropas suficientes para atacar!\n");
                    break;
                }
                
                // Seleciona território atacante
                printf("\nSelecione o territorio ATACANTE: ");
                scanf("%d", &indiceAtacante);
                indiceAtacante--;
                
                // Valida território atacante
                if (indiceAtacante < 0 || indiceAtacante >= tamanhoMapa || 
                    strcmp(mapa[indiceAtacante].cor, jogador->cor) != 0 || 
                    mapa[indiceAtacante].tropas <= 1) {
                    printf("Territorio atacante invalido!\n");
                    break;
                }
                
                // Mostra territórios que podem ser atacados
                printf("\nTerritorios que podem ser atacados:\n");
                temTerritorios = 0;
                for (int i = 0; i < tamanhoMapa; i++) {
                    if (strcmp(mapa[i].cor, jogador->cor) != 0) {
                        printf("%d. %s (%s, %d tropas)\n", 
                               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
                        temTerritorios = 1;
                    }
                }
                
                if (!temTerritorios) {
                    printf("Nao ha territorios inimigos para atacar!\n");
                    break;
                }
                
                // Seleciona território defensor
                printf("\nSelecione o territorio DEFENSOR: ");
                scanf("%d", &indiceDefensor);
                indiceDefensor--;
                
                // Valida território defensor
                if (indiceDefensor < 0 || indiceDefensor >= tamanhoMapa || 
                    strcmp(mapa[indiceDefensor].cor, jogador->cor) == 0) {
                    printf("Territorio defensor invalido!\n");
                    break;
                }
                
                // Realiza o ataque
                atacar(&mapa[indiceAtacante], &mapa[indiceDefensor], jogador);
                
                // Verifica se conquistou o território
                if (strcmp(mapa[indiceDefensor].cor, jogador->cor) == 0) {
                    conquistou = 1;
                }
                break;
            }
                
            case 2:
                // Exibe missão do jogador
                printf("\n=== SUA MISSAO ===\n");
                exibirMissao(*jogador);
                break;
                
            case 3:
                // Exibe mapa completo
                exibirTerritorios(mapa, tamanhoMapa);
                break;
                
            case 4:
                // Passa a vez
                printf("Passando a vez...\n");
                break;
                
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 4);
    
    return conquistou;
}

// Função para liberar toda a memória alocada dinamicamente
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int totalJogadores) {
    // Libera memória das missões dos jogadores
    if (jogadores != NULL) {
        for (int i = 0; i < totalJogadores; i++) {
            if (jogadores[i].missao != NULL) {
                free(jogadores[i].missao);
            }
        }
        free(jogadores);
    }
    
    // Libera memória do mapa
    if (mapa != NULL) {
        free(mapa);
    }
    
    printf("\nMemoria liberada com sucesso!\n");
}