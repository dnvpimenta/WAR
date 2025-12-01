#include <stdio.h>
#include <string.h>

// Definição da struct Territorio
// Esta struct armazena informações sobre um território no jogo
struct Territorio {
    char nome[30];      // Nome do território (até 29 caracteres + '\0')
    char cor[10];       // Cor do exército que ocupa o território
    int tropas;         // Quantidade de tropas no território
};

int main() {
    // Declaração do vetor de structs para armazenar 5 territórios
    struct Territorio territorios[5];
    
    printf("=== CADASTRO DE TERRITORIOS ===\n\n");
    
    // Entrada dos dados dos 5 territórios
    // Utilizamos um loop for para cadastrar cada território
    for (int i = 0; i < 5; i++) {
        printf("Cadastro do Territorio %d:\n", i + 1);
        
        // Leitura do nome do território
        printf("Nome do territorio: ");
        // Usamos scanf com especificador de largura para evitar overflow
        scanf("%29s", territorios[i].nome);
        
        // Leitura da cor do exército
        printf("Cor do exercito: ");
        scanf("%9s", territorios[i].cor);
        
        // Leitura do número de tropas
        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        
        printf("\n"); // Linha em branco para separar os cadastros
    }
    
    // Exibição dos dados cadastrados
    printf("\n=== TERRITORIOS CADASTRADOS ===\n\n");
    
    // Percorremos o vetor para exibir todas as informações
    for (int i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  Nome:   %s\n", territorios[i].nome);
        printf("  Cor:    %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("\n");
    }
    
    return 0; // Indica que o programa foi executado com sucesso
}