#ifndef KDTREE_H
#define KDTREE_H

#include <stdio.h>

struct Ponto{
    float *coords; 
    int classe;    
};

struct Nodo {
    struct Ponto *ponto;
    struct Nodo *esq;
    struct Nodo *dir;
};

// Cria a árvore lendo N e K da entrada padrão (stdin)
struct Nodo* cria_arvore_entrada();

struct Nodo* insere_nodo(struct Nodo* raiz, struct Ponto* p, int k);

struct Nodo* busca_nodo(struct Nodo* raiz, float* alvo, int coord, int k);

void imprime_largura(struct Nodo* raiz, int k);

void z_vizinhos(struct Nodo* raiz, float* alvo, int z, int k);

void libera_arvore(struct Nodo* raiz);

#endif