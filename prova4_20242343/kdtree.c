#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kdtree.h"

struct Vizinho{
    struct Nodo *nodo;
    float dist;
};

void* malloc_seguro(size_t tamanho) {
    void *ptr = malloc(tamanho);
    if (ptr == NULL) {
        fprintf(stderr, "Falha ao alocar memoria.\n");
        exit(1);
    }
    return ptr;
}

int pontos_iguais(struct Ponto* p, float* alvo, int k) {
    for (int i = 0; i < k; i++) 
        if (p->coords[i] != alvo[i]) return 0;
    return 1;
}

typedef struct NoFila {
    struct Nodo* info;
    struct NoFila* prox;
} NoFila;

typedef struct {
    NoFila* inicio;
    NoFila* fim;
} Fila;

Fila* cria_fila() {
    Fila* f = (Fila*) malloc_seguro(sizeof(Fila));
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

void enfileira(Fila* f, struct Nodo* n) {
    if (n == NULL) return;
    NoFila* novo = (NoFila*) malloc_seguro(sizeof(NoFila));
    novo->info = n;
    novo->prox = NULL;
    
    if (f->fim != NULL) 
        f->fim->prox = novo;
    else 
        f->inicio = novo;
    
    f->fim = novo;
}

struct Nodo* desenfileira(Fila* f) {
    if (f->inicio == NULL) return NULL;
    
    NoFila* temp = f->inicio;
    struct Nodo* ret = temp->info;
    
    f->inicio = f->inicio->prox;
    if (f->inicio == NULL) 
        f->fim = NULL;
    
    free(temp);
    return ret;
}

int fila_vazia(Fila* f) {
    return (f->inicio == NULL);
}

struct Nodo* insere_nodo(struct Nodo* raiz, struct Ponto* novo_ponto, int k) {
    // 1. Alocamos o nodo AGORA. Ele sempre será inserido em algum lugar.
    struct Nodo* novo = (struct Nodo*) malloc_seguro(sizeof(struct Nodo));
    novo->ponto = novo_ponto;
    novo->esq = NULL;
    novo->dir = NULL;

    // Caso base: árvore vazia
    if (raiz == NULL) {
        return novo;
    }

    int coord = 0;
    struct Nodo* atual = raiz;
    struct Nodo* pai = NULL;

    // Navegação iterativa até a folha (Slide 7)
    while (atual != NULL) {
        pai = atual;
        
        if (novo_ponto->coords[coord] < atual->ponto->coords[coord]) 
            atual = atual->esq;
        else 
            atual = atual->dir;

        coord = (coord + 1) % k;
    }

    // Inserção no pai correto
    // Matemática para voltar 1 nível na coordenada sem dar negativo
    int coord_pai = (coord - 1 + k) % k;

    if (novo_ponto->coords[coord_pai] < pai->ponto->coords[coord_pai]) 
        pai->esq = novo;
    else 
        pai->dir = novo;

    return raiz;
}

/* Nota: Esta função está aqui mas o main faz a leitura principal.
   Se o main já lê, esta função pode ser redundante ou usada apenas para testes isolados.
   Vou corrigir o scanf mesmo assim. */
struct Nodo* cria_arvore_entrada() {
    int n, k;
    
    // CORREÇÃO: Adicionado & comercial
    if (scanf("%d %d", &n, &k) != 2)
        return NULL; 

    struct Nodo* raiz = NULL;

    for (int i = 0; i < n; i++) {
        struct Ponto* p = (struct Ponto*) malloc_seguro(sizeof(struct Ponto));
        p->coords = (float*) malloc_seguro(k * sizeof(float));

        for (int j = 0; j < k; j++)
            scanf("%f", &p->coords[j]);

        scanf("%d", &p->classe);

        // CORREÇÃO: Chamada com 3 argumentos
        raiz = insere_nodo(raiz, p, k);
    }
    
    return raiz;
}

struct Nodo* busca_nodo(struct Nodo* r, float* vetchave, int coord, int k) {
    if (r == NULL || pontos_iguais(r->ponto, vetchave, k)) 
        return r;

    if (vetchave[coord] < r->ponto->coords[coord]) 
        return busca_nodo(r->esq, vetchave, (coord + 1) % k, k);
    
    return busca_nodo(r->dir, vetchave, (coord + 1) % k, k);
}


void imprime_largura(struct Nodo* raiz, int k) {
    if (raiz == NULL) return;

    Fila* f = cria_fila();
    enfileira(f, raiz);

    while (!fila_vazia(f)) {
        struct Nodo* atual = desenfileira(f);

        // Imprime as coordenadas do ponto
        for (int i = 0; i < k; i++)
            printf("%.1f ", atual->ponto->coords[i]);

        printf("\n");

        // Enfileira os filhos
        if (atual->esq != NULL) enfileira(f, atual->esq);
        if (atual->dir != NULL) enfileira(f, atual->dir);
    }

    free(f);
}

void libera_arvore(struct Nodo* raiz) {
    if (raiz == NULL) return;
    libera_arvore(raiz->esq);
    libera_arvore(raiz->dir);
    free(raiz->ponto->coords);
    free(raiz->ponto);
    free(raiz);
}