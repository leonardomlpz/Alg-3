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
        if (p->coords[i] != alvo[i])
            return 0;
    return 1;
}

struct NoFila {
    struct Nodo* info;
    struct NoFila* prox;
};

struct Fila {
    struct NoFila* inicio;
    struct NoFila* fim;
};

struct Fila* cria_fila() {
    struct Fila* f = (struct Fila*) malloc_seguro(sizeof(struct Fila));
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

void enfileira(struct Fila* f, struct Nodo* n) {
    if (n == NULL)
        return;
    struct NoFila* novo = (struct NoFila*) malloc_seguro(sizeof(struct NoFila));
    novo->info = n;
    novo->prox = NULL;
    
    if (f->fim != NULL) 
        f->fim->prox = novo;
    else 
        f->inicio = novo;
    
    f->fim = novo;
}

struct Nodo* desenfileira(struct Fila* f) {
    if (f->inicio == NULL)
        return NULL;
    
    struct NoFila* temp = f->inicio;
    struct Nodo* ret = temp->info;
    
    f->inicio = f->inicio->prox;
    if (f->inicio == NULL) 
        f->fim = NULL;
    
    free(temp);
    return ret;
}

int fila_vazia(struct Fila* f) {
    return (f->inicio == NULL);
}

struct Nodo* insere_nodo(struct Nodo* raiz, struct Ponto* novo_ponto, int k) {
    struct Nodo* novo = (struct Nodo*) malloc_seguro(sizeof(struct Nodo));
    novo->ponto = novo_ponto;
    novo->esq = NULL;
    novo->dir = NULL;

    // Caso base: árvore vazia
    if (raiz == NULL)
        return novo;

    int coord = 0;
    struct Nodo* atual = raiz;
    struct Nodo* pai = NULL;

    // Navegação até a folha
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

struct Nodo* cria_arvore_entrada() {
    int n, k;
    
    if (scanf("%d %d", &n, &k) != 2)
        return NULL; 

    struct Nodo* raiz = NULL;

    for (int i = 0; i < n; i++) {
        struct Ponto* p = (struct Ponto*) malloc_seguro(sizeof(struct Ponto));
        p->coords = (float*) malloc_seguro(k * sizeof(float));

        for (int j = 0; j < k; j++)
            scanf("%f", &p->coords[j]);

        scanf("%d", &p->classe);

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
    if (raiz == NULL)
        return;

    struct Fila* f = cria_fila();
    enfileira(f, raiz);

    while (!fila_vazia(f)) {
        struct Nodo* atual = desenfileira(f);

        // Imprime as coordenadas do ponto
        for (int i = 0; i < k; i++)
            printf("%.1f ", atual->ponto->coords[i]);

        printf("\n");

        // Enfileira os filhos
        if (atual->esq != NULL)
            enfileira(f, atual->esq);
        if (atual->dir != NULL)
            enfileira(f, atual->dir);
    }

    free(f);
}

float calcula_distancia(struct Ponto *p, float *alvo, int k){
    float soma = 0.0;
    float diff;

    for (int i = 0; i < k; i++) {
        diff = p->coords[i] - alvo[i];
        soma += (diff * diff);
    }
    
    return sqrt(soma);
}

void atualiza_lista_vizinhos(struct Vizinho* lista, int z, struct Nodo* no, float d) {
    int i;
    // Acha a posição de inserção
    for (i = z - 1; i >= 0; i--) {
        if (lista[i].nodo == NULL || d < lista[i].dist)
            continue;

        break;
    }
    
    int pos_insercao = i + 1;

    // Se a posição é >= z, o novo ponto é pior que todos os z,
    // então não precisamos adicioná-lo.
    if (pos_insercao >= z)
        return;

    // Desloca os piores para a direita
    for (int j = z - 1; j > pos_insercao; j--)
        lista[j] = lista[j - 1];

    // Insere o novo vizinho na posição correta
    lista[pos_insercao].nodo = no;
    lista[pos_insercao].dist = d;
}

void z_vizinhos_recursivo(struct Nodo* r, float* alvo, struct Vizinho* lista, int z, int k, int coord) {
    if (r == NULL)
        return;

    // Calcula a distância do nó atual e tenta adicionar na lista
    float dist_atual = calcula_distancia(r->ponto, alvo, k);
    atualiza_lista_vizinhos(lista, z, r, dist_atual);

    struct Nodo *prim, *sec;
    if (alvo[coord] < r->ponto->coords[coord]) {
        prim = r->esq;
        sec = r->dir; 
    } else {
        prim = r->dir;
        sec = r->esq;
    }

    // Chama recursivamente para a sub-árvore "boa"
    z_vizinhos_recursivo(prim, alvo, lista, z, k, (coord + 1) % k);
    
    // Distância do ponto alvo até o "plano de corte"
    float dist_plano = fabs(alvo[coord] - r->ponto->coords[coord]);
    
    // O pior vizinho que achamos até agora
    struct Vizinho pior_vizinho = lista[z - 1];

    // Se a lista não está cheia (pior_vizinho.nodo == NULL) OU
    // se a distância até o plano de corte é MENOR que a distância
    // do nosso pior vizinho...
    // ... então vale a pena olhar a sub-árvore "ruim"
    if (pior_vizinho.nodo == NULL || dist_plano < pior_vizinho.dist) 
        z_vizinhos_recursivo(sec, alvo, lista, z, k, (coord + 1) % k);

}

void z_vizinhos(struct Nodo* raiz, float* alvo, int z, int k) {
    // Cria a lista de vizinhos
    struct Vizinho* lista = (struct Vizinho*) malloc_seguro(z * sizeof(struct Vizinho));
    
    for (int i = 0; i < z; i++) {
        lista[i].nodo = NULL;
        lista[i].dist = 0.0;
    }

    z_vizinhos_recursivo(raiz, alvo, lista, z, k, 0);

    // Imprime os resultados
    for (int i = 0; i < z; i++) {
        if (lista[i].nodo == NULL)
            continue;

        struct Ponto* p = lista[i].nodo->ponto;
        
        // Imprime as K coordenadas
        for (int j = 0; j < k; j++) {
            printf("%.1f", p->coords[j]);
            if (j < k - 1)
                printf(", ");
        }
        
        if (i == (z - 1))
            printf(" (classe %d), dist = %.4f.\n", p->classe, lista[i].dist);
        else
            printf(" (classe %d), dist = %.4f\n", p->classe, lista[i].dist);
    }

    // Libera a lista
    free(lista);
}

void libera_arvore(struct Nodo* raiz) {
    if (raiz == NULL)
        return;
    libera_arvore(raiz->esq);
    libera_arvore(raiz->dir);
    free(raiz->ponto->coords);
    free(raiz->ponto);
    free(raiz);
}