#ifndef KDTREE_H
#define KDTREE_H

#include <stdio.h>

typedef struct {
    float *coords; 
    int classe;    
} Ponto;

typedef struct Nodo {
    Ponto *ponto;
    struct Nodo *esq;
    struct Nodo *dir;
} Nodo;

// Cria a árvore lendo N e K da entrada padrão (stdin)
Nodo* cria_arvore_entrada();

Nodo* insere_nodo(Nodo* raiz, Ponto* p, int k);

Nodo* busca_nodo(Nodo* raiz, float* alvo, int coord, int k);

void imprime_largura(Nodo* raiz, int k);

void z_vizinhos(Nodo* raiz, float* alvo, int z, int k);

void libera_arvore(Nodo* raiz);

#endif