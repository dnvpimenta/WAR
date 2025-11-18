#include <stdio.h>
#include <string.h>

struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    struct Territorio territorios[5];
    int i;
    
    printf("=== SISTEMA DE CADASTRO DE TERRITÓRIOS ===\n\n");
    printf("--- CADASTRO DE TERRITÓRIOS ---\n");
    
    for (i = 0; i < 5; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        
        printf("Digite o nome do territorio: ");
        scanf("%29s", territorios[i].nome);
        
        printf("Digite a cor do exercito: ");
        scanf("%9s", territorios[i].cor);
        
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        
        // Limpar buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    printf("\n\n--- DADOS DOS TERRITORIOS CADASTRADOS ---\n");
    printf("==========================================\n");
    
    for (i = 0; i < 5; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do exercito: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("  --------------------\n");
    }
    
    printf("\nPressione Enter para sair...");
    getchar();
    
    return 0;
}