#include "redblack.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/queue.h>

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
    retorno->nomeDinf = malloc(sizeof("lml24"));
    if(!retorno->nomeDinf){
        matarProgramaFaltaMemoria();
    }
	strcpy(retorno->nome, "Leonardo Martins Lopes");
    strcpy(retorno->nomeDinf, "lml24");
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
    struct nodo* y; // y será o tio de z

    // A violação principal ocorre se o pai de z também for vermelho.
    while (z->pai->cor == VERMELHO){
        // Caso A: O pai de z é um FILHO ESQUERDO.
        if (z->pai == z->pai->pai->fe){
            y = z->pai->pai->fd; // y é o tio de z

            // Caso 1: O tio de z é vermelho.
            if (y->cor == VERMELHO){
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai; // Move z para o avô para continuar a verificação.
            }
            else {
                // Caso 2: O tio de z é preto e z é um filho direito.
                if (z == z->pai->fd){
                    z = z->pai;
                    rotacaoEsquerda(T, z);
                }

                // Caso 3: O tio de z é preto e z é um filho esquerdo.
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(T, z->pai->pai);
            }
        }
        // Caso B: O pai de z é um FILHO DIREITO (código espelhado do Caso A).
        else{
            y = z->pai->pai->fe;

            if (y->cor == VERMELHO){
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            else{
                if (z == z->pai->fe){
                    z = z->pai;
                    rotacaoDireita(T, z);
                }

                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(T, z->pai->pai);
            }
        }
    }

    // O laço pode ter mudado a cor da raiz para vermelho.
    (*T)->cor = PRETO;
}

struct nodo* inserir(struct nodo** T, int chave){
    struct nodo* z = malloc(sizeof(struct nodo));
    if (!z){
        fprintf(stderr, "Falha ao alocar memoria.\n");
        exit(1);
    }
    z->chave = chave;
    z->cor = VERMELHO;
    z->fe = SENTINELA;
    z->fd = SENTINELA;
    z->pai = SENTINELA;

    // y será o pai do novo nodo.
    struct nodo* y = SENTINELA;
    // x é usado para percorrer a árvore.
    struct nodo* x = *T;

    while (x != SENTINELA){
        // Guarda o pai antes de continuar.
        y = x;
        
        if (z->chave < x->chave)
            x = x->fe;
        else if (z->chave > x->chave)
            x = x->fd;
        else{
            // Chave duplicada
            free(z);
            return SENTINELA;
        }
    }

    z->pai = y;

    // Arvore vazia
    if (y == SENTINELA)
        *T = z;

    else if (z->chave < y->chave)

        y->fe = z;
    else
        y->fd = z;
    

    inserirFixup(T, z);

    return z;
}

void transplantar(struct nodo** T, struct nodo* u, struct nodo* v){
    if (u->pai == SENTINELA)
        *T = v;
    else if (u == u->pai->fe)
        u->pai->fe = v;
    else
        u->pai->fd = v;

    v->pai = u->pai;
}

struct nodo* minimo(struct nodo* nodo){
    while (nodo->fe != SENTINELA)
        nodo = nodo->fe;
    
    return nodo;
}

int excluir(struct nodo** T, int chave){
    struct nodo* z = buscar(*T, chave);

    if (z == SENTINELA)
        return 0; 

    struct nodo *y = z;
    struct nodo *x;
    int corOriginalY = y->cor;

    if (z->fe == SENTINELA){
        x = z->fd;
        transplantar(T, z, z->fd);
    }
    else if (z->fd == SENTINELA){
        x = z->fe;
        transplantar(T, z, z->fe);
    }
    else{
        y = minimo(z->fd);
        corOriginalY = y->cor;
        x = y->fd;
        if (y->pai == z)
            x->pai = y;
        else{
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
        excluirFixup(T, x);

    free(z);
    return 1;
}

void excluirFixup(struct nodo** T, struct nodo* x){
    struct nodo* w; // w é o irmão de x

    while (x != *T && x->cor == PRETO){
        // Caso A: x é um FILHO ESQUERDO
        if (x == x->pai->fe){
            w = x->pai->fd;

            // Caso 1: O irmão de x é vermelho.
            if (w->cor == VERMELHO){
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(T, x->pai);
                w = x->pai->fd;
            }

            // Caso 2: O irmão de x é preto, e ambos os filhos de w são pretos.
            if (w->fe->cor == PRETO && w->fd->cor == PRETO){
                w->cor = VERMELHO;
                x = x->pai;
            }
            else{
                // Caso 3: O irmão de x é preto, o filho esquerdo de w é vermelho e o filho direito de w é preto.
                if (w->fd->cor == PRETO){
                    w->fe->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(T, w);
                    w = x->pai->fd;
                }

                // Caso 4: O irmão de x é preto, e o filho direito de w é vermelho.
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->fd->cor = PRETO;
                rotacaoEsquerda(T, x->pai);
                x = *T;
            }
        }
        // Caso B: x é um FILHO DIREITO (código espelhado)
        else{
            w = x->pai->fe;

            if (w->cor == VERMELHO){
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(T, x->pai);
                w = x->pai->fe;
            }

            if (w->fd->cor == PRETO && w->fe->cor == PRETO){
                w->cor = VERMELHO;
                x = x->pai;
            }
            else{
                if (w->fe->cor == PRETO){
                    w->fd->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(T, w);
                    w = x->pai->fe;
                }

                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->fe->cor = PRETO;
                rotacaoDireita(T, x->pai);
                x = *T;
            }
        }
    }
    x->cor = PRETO;
}

struct nodo* buscar(struct nodo* nodo, int chave){
    struct nodo* atual = nodo;

    while (atual != SENTINELA && chave != atual->chave){
        if (chave < atual->chave)
            atual = atual->fe;
        else
            atual = atual->fd;
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

struct FilaItem{
    struct nodo *nodo_rb;
    int nivel;
    TAILQ_ENTRY(FilaItem) pointers;
};

TAILQ_HEAD(FilaCabecalho, FilaItem);

void imprimirEmLargura(struct nodo* raiz){
    if (raiz == SENTINELA){
        return;
    }

    struct FilaCabecalho fila;
    TAILQ_INIT(&fila);

    struct FilaItem* primeiro_item = malloc(sizeof(struct FilaItem));
    if (!primeiro_item)
        matarProgramaFaltaMemoria();
    primeiro_item->nodo_rb = raiz;
    primeiro_item->nivel = 0;
    TAILQ_INSERT_TAIL(&fila, primeiro_item, pointers);

    int nivelAtual = -1;
    struct FilaItem* item_atual = NULL;

    while (!TAILQ_EMPTY(&fila)){
        item_atual = TAILQ_FIRST(&fila);
        struct nodo* nodo = item_atual->nodo_rb;

        if (item_atual->nivel != nivelAtual){
            if (nivelAtual != -1)
                printf("\n");
            nivelAtual = item_atual->nivel;
            printf("[%d] ", nivelAtual);
        }

        printf("(%c)%d", (nodo->cor == PRETO ? 'B' : 'R'), nodo->chave);
        if (nodo->pai != SENTINELA){
            printf("[%d%c]\t", nodo->pai->chave, (nodo == nodo->pai->fe ? 'e' : 'd'));
        } else {
            printf("[RAIZ]\t");
        }

        // Enfileira os filhos, se existirem
        if (nodo->fe != SENTINELA){
            struct FilaItem* novo_item = malloc(sizeof(struct FilaItem));
            if (!novo_item)
                matarProgramaFaltaMemoria();
            novo_item->nodo_rb = nodo->fe;
            novo_item->nivel = nivelAtual + 1;
            TAILQ_INSERT_TAIL(&fila, novo_item, pointers);
        }
        if (nodo->fd != SENTINELA){
            struct FilaItem* novo_item = malloc(sizeof(struct FilaItem));
            if (!novo_item)
                matarProgramaFaltaMemoria();
            novo_item->nodo_rb = nodo->fd;
            novo_item->nivel = nivelAtual + 1;
            TAILQ_INSERT_TAIL(&fila, novo_item, pointers);
        }
        
        // Remove o item que acabamos de processar e libera sua memória.
        TAILQ_REMOVE(&fila, item_atual, pointers);
        free(item_atual);
    }

    printf("\n");
}

void liberarArvore(struct nodo* nodo){
    if (nodo != SENTINELA){
        liberarArvore(nodo->fe);
        liberarArvore(nodo->fd);
        free(nodo);
    }
}