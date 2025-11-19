#include <stdio.h>
#include <stdlib.h>
#include "kdtree.h"

int main() {
    int n, k;

    printf("Insira N e K.\n");
    
    if (scanf("%d %d", &n, &k) != 2) 
        return 0;

    printf("Insira os pontos.\n");

    struct Nodo* raiz = NULL; // MUDANÇA

    // Loop de leitura dos N pontos 
    for (int i = 0; i < n; i++) {
        // MUDANÇAS (struct Ponto e malloc_seguro)
        struct Ponto* p = (struct Ponto*) malloc_seguro(sizeof(struct Ponto));
        p->coords = (float*) malloc_seguro(k * sizeof(float));

        // Lê as K coordenadas
        for (int j = 0; j < k; j++) 
            scanf("%f", &p->coords[j]);
        
        // Lê a classe (inteiro no final da linha) 
        scanf("%d", &p->classe);

        raiz = insere_nodo(raiz, p, k);
    }

    printf("Árvore Construída.\n");

    char comando;
    
    while (scanf(" %c", &comando) != EOF && comando != 'f') {
        if (comando == 'b') {
            float* alvo = (float*) malloc_seguro(k * sizeof(float));
            
            for (int i = 0; i < k; i++) 
                scanf("%f", &alvo[i]);

            // MUDANÇA (struct Nodo)
            struct Nodo* resultado = busca_nodo(raiz, alvo, 0, k);

            if (resultado != NULL)
                printf("Encontrado. Classe %d.\n", resultado->ponto->classe);
            else
                printf("Não encontrado.\n");

            free(alvo);
        }
        
        else if (comando == 'l') {
            imprime_largura(raiz, k); 
        }

        else if (comando == 'z') {
            int z_qtd;
            scanf("%d", &z_qtd);

            float* alvo = (float*) malloc_seguro(k * sizeof(float));
            for (int i = 0; i < k; i++) 
                scanf("%f", &alvo[i]);

            printf("Vizinhos mais próximos:\n");
            
            z_vizinhos(raiz, alvo, z_qtd, k);
            
            free(alvo);
        }
    }

    libera_arvore(raiz);

    return 0;
}