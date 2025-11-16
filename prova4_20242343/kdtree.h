#ifndef KDTREE_H
#define KDTREE_H

#include <stdio.h>
#include <stdlib.h> // <-- LINHA ADICIONADA

// Sem typedef, como solicitado
struct Ponto{
    float *coords; 
    int classe;    
};

// Sem typedef, como solicitado
struct Nodo {
    struct Ponto *ponto; // Agora usa struct Ponto
    struct Nodo *esq;
    struct Nodo *dir;
};

// Protótipos agora usam "struct"
struct Nodo* cria_arvore_entrada();
struct Nodo* insere_nodo(struct Nodo* raiz, struct Ponto* p, int k);
struct Nodo* busca_nodo(struct Nodo* raiz, float* alvo, int coord, int k);
void imprime_largura(struct Nodo* raiz, int k);
void z_vizinhos(struct Nodo* raiz, float* alvo, int z, int k);
void libera_arvore(struct Nodo* raiz);

// Prototipo do malloc_seguro para ser usado pelo main.c
void* malloc_seguro(size_t tamanho); // <-- LINHA ADICIONADA

#endif