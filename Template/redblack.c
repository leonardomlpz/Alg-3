#include "redblack.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VERMELHO 1
#define PRETO 0

void matarProgramaFaltaMemoria(){
    fprintf(stderr, "Falha ao alocar memoria.\n");
    exit(1);
}

void excluirFixup(struct nodo** T, struct nodo* x);

struct aluno* getAluno(){
    struct aluno* retorno = malloc(sizeof(struct aluno));
    if(!retorno)
        matarProgramaFaltaMemoria();
    retorno->nome = malloc(sizeof("Leonardo Martins Lopes"));//sizeof conta o \0
	if(!retorno->nome)
		matarProgramaFaltaMemoria();
    retorno->nomeDinf = malloc(sizeof("NOME_NAS_SERVIDORAS_DINF"));
    if(!retorno->nomeDinf)
        matarProgramaFaltaMemoria();
    
	strcpy(retorno->nome, "Leonardo Martins Lopes");
    strcpy(retorno->nomeDinf, "NOME_NAS_SERVIDORAS_DINF");
    retorno->grr = 20242343;

	return retorno;
}

void imprimirDadosAluno(){
    struct aluno* aluno = getAluno();
    
    printf("Trabalho de %s\n", aluno->nome);
    printf("Login Dinf %s\n", aluno->nomeDinf);
    printf("GRR %u\n\n", aluno->grr);

    free(aluno->nome);
    free(aluno->nomeDinf);
    free(aluno);

    return;
}

struct nodo *SENTINELA = NULL;

void inicializarSentinela(){
    SENTINELA = malloc(sizeof(struct nodo));
    if (!SENTINELA){
        fprintf(stderr, "Falha ao alocar memoria.\n");
        exit(1);
    }
    SENTINELA->cor = PRETO;
    SENTINELA->pai = SENTINELA;
    SENTINELA->fe = SENTINELA;
    SENTINELA->fd = SENTINELA;
}

void rotacaoEsquerda(struct nodo **T, struct nodo *x){
    if (!T || !x)
        return ;
    // Rotacao nao eh possivel
    if (x->fd == SENTINELA)
        return;

    struct nodo *y;
    y = x->fd;
    x->fd = y->fe;

    if (y->fe != SENTINELA)
        y->fe->pai = x;

    y->pai = x->pai;
    if (x->pai == SENTINELA)
        *T = y;
    else if (x == x->pai->fe)
        x->pai->fe = y;
    else
        x->pai->fd = y;

    y->fe = x;
    x->pai = y;
}

void rotacaoDireita(struct nodo **T, struct nodo *x){
    if (!T || !x) {
        return;
    }
    
    if (x->fe == SENTINELA) {
        return;
    }

    struct nodo *y = x->fe;
    x->fe = y->fd;

    if (y->fd != SENTINELA) {
        y->fd->pai = x;
    }

    y->pai = x->pai;

    if (x->pai == SENTINELA) {
        *T = y;
    } else if (x == x->pai->fe) {
        x->pai->fe = y;
    } else {
        x->pai->fd = y;
    }

    y->fd = x;
    x->pai = y;
}

void inserirFixup(struct nodo** T, struct nodo* z){
    struct nodo* y; // 'y' será o tio de 'z'

    // A violação principal ocorre se o pai de 'z' também for vermelho.
    while (z->pai->cor == VERMELHO)
    {
        // Caso A: O pai de 'z' é um FILHO ESQUERDO.
        if (z->pai == z->pai->pai->fe)
        {
            y = z->pai->pai->fd; // 'y' é o tio de 'z'

            // Caso 1: O tio de 'z' é vermelho.
            if (y->cor == VERMELHO)
            {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai; // Move 'z' para o avô para continuar a verificação.
            }
            else 
            {
                // Caso 2: O tio de 'z' é preto e 'z' é um filho direito.
                if (z == z->pai->fd)
                {
                    z = z->pai;
                    rotacaoEsquerda(T, z);
                }

                // Caso 3: O tio de 'z' é preto e 'z' é um filho esquerdo.
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(T, z->pai->pai);
            }
        }
        // Caso B: O pai de 'z' é um FILHO DIREITO (código espelhado do Caso A).
        else
        {
            y = z->pai->pai->fe; // 'y' é o tio de 'z'

            // Caso 1 (espelhado): O tio de 'z' é vermelho.
            if (y->cor == VERMELHO)
            {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            else
            {
                // Caso 2 (espelhado): O tio de 'z' é preto e 'z' é um filho esquerdo.
                if (z == z->pai->fe)
                {
                    z = z->pai;
                    rotacaoDireita(T, z);
                }

                // Caso 3 (espelhado): O tio de 'z' é preto e 'z' é um filho direito.
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(T, z->pai->pai);
            }
        }
    }
    
    // Propriedade 2: A raiz da árvore deve ser sempre preta.
    // O laço pode ter mudado a cor da raiz para vermelho, então garantimos aqui.
    (*T)->cor = PRETO;
}

struct nodo* inserir(struct nodo** T, int chave){
    // --- Parte 1: Alocação e inicialização do novo nodo (z) ---
    // Esta parte combina a necessidade de criar o nodo (que não está no
    // pseudocódigo principal de Cormen) com as propriedades iniciais do nodo.
    struct nodo* z = malloc(sizeof(struct nodo));
    if (!z)
    {
        // Tratamento de erro de memória, conforme especificação do trabalho
        fprintf(stderr, "Falha ao alocar memoria.\n");
        exit(1);
    }
    z->chave = chave;
    z->cor = VERMELHO; // Propriedade: novos nodos são inicialmente vermelhos.
    z->fe = SENTINELA;
    z->fd = SENTINELA;
    z->pai = SENTINELA;


    // --- Parte 2: Encontrar a posição de inserção ---
    // Este laço é idêntico a uma inserção em árvore de busca binária.
    struct nodo* y = SENTINELA; // 'y' será o pai do novo nodo.
    struct nodo* x = *T;        // 'x' é usado para percorrer a árvore.

    while (x != SENTINELA)
    {
        y = x; // Guarda o pai antes de descer na árvore.
        
        if (z->chave < x->chave)
        {
            x = x->fe;
        }
        else if (z->chave > x->chave)
        {
            x = x->fd;
        }
        else
        {
            // Ponto de atenção: chave duplicada encontrada, conforme seu trabalho.
            free(z); // Libera a memória do nodo 'z' que não será inserido.
            return SENTINELA; // Retorna SENTINELA para indicar falha.
        }
    }


    // --- Parte 3: Ligar o novo nodo (z) na árvore ---
    z->pai = y; // O pai de 'z' é o último nodo 'y' visitado.

    if (y == SENTINELA)
    {
        // Caso 1: A árvore estava vazia. 'z' se torna a raiz.
        *T = z;
    }
    else if (z->chave < y->chave)
    {
        // Caso 2: 'z' é menor que seu pai, torna-se filho esquerdo.
        y->fe = z;
    }
    else
    {
        // Caso 3: 'z' é maior que seu pai, torna-se filho direito.
        y->fd = z;
    }
    

    // --- Parte 4: Corrigir violações das propriedades rubro-negras ---
    inserirFixup(T, z);

    return z; // Retorna o ponteiro para o novo nodo.
}

void transplantar(struct nodo** T, struct nodo* u, struct nodo* v){
    if (u->pai == SENTINELA)
    {
        *T = v;
    }
    else if (u == u->pai->fe)
    {
        u->pai->fe = v;
    }
    else
    {
        u->pai->fd = v;
    }
    v->pai = u->pai;
}

struct nodo* minimo(struct nodo* nodo){
    while (nodo->fe != SENTINELA)
    {
        nodo = nodo->fe;
    }
    return nodo;
}

int excluir(struct nodo** T, int chave){
    // Primeiro, precisamos encontrar o nodo 'z' a ser removido.
    // (Esta função 'buscar' será uma das próximas a ser implementada)
    struct nodo* z = buscar(*T, chave);

    if (z == SENTINELA)
    {
        // A chave não está na árvore.
        return 0; 
    }

    struct nodo *y = z;
    struct nodo *x;
    int corOriginalY = y->cor;

    if (z->fe == SENTINELA)
    {
        x = z->fd;
        transplantar(T, z, z->fd);
    }
    else if (z->fd == SENTINELA)
    {
        x = z->fe;
        transplantar(T, z, z->fe);
    }
    else
    {
        y = minimo(z->fd);
        corOriginalY = y->cor;
        x = y->fd;
        if (y->pai == z)
        {
            x->pai = y;
        }
        else
        {
            transplantar(T, y, y->fd);
            y->fd = z->fd;
            y->fd->pai = y;
        }
        transplantar(T, z, y);
        y->fe = z->fe;
        y->fe->pai = y;
        y->cor = z->cor;
    }

    // A correção só é necessária se um nodo PRETO foi removido.
    if (corOriginalY == PRETO)
    {
        excluirFixup(T, x);
    }

    free(z);
    return 1;
}

void excluirFixup(struct nodo** T, struct nodo* x){
    struct nodo* w; // 'w' é o irmão de 'x'

    while (x != *T && x->cor == PRETO)
    {
        // Caso A: 'x' é um FILHO ESQUERDO
        if (x == x->pai->fe)
        {
            w = x->pai->fd;

            // Caso 1: O irmão de 'x' ('w') é vermelho.
            if (w->cor == VERMELHO)
            {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(T, x->pai);
                w = x->pai->fd;
            }

            // Caso 2: O irmão de 'x' ('w') é preto, e ambos os filhos de 'w' são pretos.
            if (w->fe->cor == PRETO && w->fd->cor == PRETO)
            {
                w->cor = VERMELHO;
                x = x->pai;
            }
            else
            {
                // Caso 3: O irmão de 'x' ('w') é preto, o filho esquerdo de 'w' é vermelho
                // e o filho direito de 'w' é preto.
                if (w->fd->cor == PRETO)
                {
                    w->fe->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(T, w);
                    w = x->pai->fd;
                }

                // Caso 4: O irmão de 'x' ('w') é preto, e o filho direito de 'w' é vermelho.
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->fd->cor = PRETO;
                rotacaoEsquerda(T, x->pai);
                x = *T; // Termina o laço
            }
        }
        // Caso B: 'x' é um FILHO DIREITO (código espelhado)
        else
        {
            w = x->pai->fe;

            // Caso 1 (espelhado)
            if (w->cor == VERMELHO)
            {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(T, x->pai);
                w = x->pai->fe;
            }

            // Caso 2 (espelhado)
            if (w->fd->cor == PRETO && w->fe->cor == PRETO)
            {
                w->cor = VERMELHO;
                x = x->pai;
            }
            else
            {
                // Caso 3 (espelhado)
                if (w->fe->cor == PRETO)
                {
                    w->fd->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(T, w);
                    w = x->pai->fe;
                }

                // Caso 4 (espelhado)
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->fe->cor = PRETO;
                rotacaoDireita(T, x->pai);
                x = *T; // Termina o laço
            }
        }
    }
    x->cor = PRETO;
}
// Retorna o nodo encontrado ou o SENTINELA caso contrario
struct nodo* buscar(struct nodo* nodo, int chave){
    struct nodo* atual = nodo;

    // O laço continua enquanto não chegarmos a uma folha (SENTINELA)
    // e enquanto a chave do nodo atual não for a que procuramos.
    while (atual != SENTINELA && chave != atual->chave)
    {
        if (chave < atual->chave)
        {
            // A chave procurada é menor, então vamos para a subárvore esquerda.
            atual = atual->fe;
        }
        else
        {
            // A chave procurada é maior, então vamos para a subárvore direita.
            atual = atual->fd;
        }
    }

    return atual;
}

void imprimirEmOrdem(struct nodo* nodo){
    if (nodo != SENTINELA){
        imprimirEmOrdem(nodo->fe);
        printf("%d\t", nodo->chave);
        imprimirEmOrdem(nodo->fd);
    }
}




typedef struct {
    struct nodo* nodo_rb;
    int nivel;
} ItemFila;

typedef struct NodoFila {
    ItemFila item;
    struct NodoFila* proximo;
} NodoFila;

typedef struct {
    NodoFila *inicio;
    NodoFila *fim;
} Fila;

Fila* criarFila() {
    Fila* f = (Fila*)malloc(sizeof(Fila));
    if (f) f->inicio = f->fim = NULL;
    return f;
}

void enfileirar(Fila* f, struct nodo* nodo_rb, int nivel) {
    NodoFila* novoNodo = (NodoFila*)malloc(sizeof(NodoFila));
    if (novoNodo) {
        novoNodo->item.nodo_rb = nodo_rb;
        novoNodo->item.nivel = nivel;
        novoNodo->proximo = NULL;
        if (f->fim) f->fim->proximo = novoNodo;
        else f->inicio = novoNodo;
        f->fim = novoNodo;
    }
}

ItemFila desenfileirar(Fila* f) {
    NodoFila* temp = f->inicio;
    ItemFila item = temp->item;
    f->inicio = f->inicio->proximo;
    if (!f->inicio) f->fim = NULL;
    free(temp);
    return item;
}

int filaVazia(Fila* f) {
    return f->inicio == NULL;
}

void liberarFila(Fila* f) {
    while (!filaVazia(f)) desenfileirar(f);
    free(f);
}


void imprimirEmLargura(struct nodo* raiz){
    if (raiz == SENTINELA) {
        return;
    }

    Fila* fila = criarFila();
    
    // Corresponde a "enfileirar(r)" 
    enfileirar(fila, raiz, 0);

    int nivelAtual = -1;

    // Corresponde a "enquanto fila não vazia" [cite: 3768]
    while (!filaVazia(fila)) {
        // Corresponde a "n = removerCabeça()" [cite: 3769]
        ItemFila itemAtual = desenfileirar(fila);
        struct nodo* nodo = itemAtual.nodo_rb;

        // Adaptação: Verifica se mudamos de nível para imprimir o cabeçalho "[n]"
        if (itemAtual.nivel != nivelAtual) {
            if (nivelAtual != -1) printf("\n");
            nivelAtual = itemAtual.nivel;
            printf("[%d] ", nivelAtual);
        }

        // Adaptação: Corresponde a "imprimir(n)"[cite: 3770], com a formatação do trabalho
        printf("(%c)%d", (nodo->cor == PRETO ? 'B' : 'R'), nodo->chave);
        if (nodo->pai != SENTINELA) {
            printf("[%d%c]\t", nodo->pai->chave, (nodo == nodo->pai->fe ? 'e' : 'd'));
        } else {
            printf("[RAIZ]\t");
        }

        // Corresponde a "enfileirar(n.filhoEsquerdo)" [cite: 3772]
        if (nodo->fe != SENTINELA) {
            enfileirar(fila, nodo->fe, nivelAtual + 1);
        }
        // Corresponde a "enfileirar(n.filhoDireito)" [cite: 3774]
        if (nodo->fd != SENTINELA) {
            enfileirar(fila, nodo->fd, nivelAtual + 1);
        }
    }

    printf("\n");
    liberarFila(fila); // Limpa a memória usada pela fila
}

void liberarArvore(struct nodo* nodo)
{
    // A condição de parada é quando chegamos em uma folha.
    if (nodo != SENTINELA)
    {
        // Libera primeiro a subárvore esquerda.
        liberarArvore(nodo->fe);
        // Depois, libera a subárvore direita.
        liberarArvore(nodo->fd);
        // Por último, libera o próprio nodo.
        free(nodo);
    }
}