#include "redblack.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VERMELHO 1
#define PRETO 0

struct aluno* getAluno(){
    struct aluno* retorno = malloc(sizeof(struct aluno));
    if(!retorno)
        matarProgramaFaltaMemoria();
    retorno->nome = malloc(sizeof("SEU_NOME"));//sizeof conta o \0
	if(!retorno->nome)
		matarProgramaFaltaMemoria();
    retorno->nomeDinf = malloc(sizeof("NOME_NAS_SERVIDORAS_DINF"));
    if(!retorno->nomeDinf)
        matarProgramaFaltaMemoria();
    
	strcpy(retorno->nome, "SEU_NOME");
    strcpy(retorno->nomeDinf, "NOME_NAS_SERVIDORAS_DINF");
    retorno->grr = 1234;

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
        return NULL;
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
//IMPLEMENTE AS DEMAIS FUNÇÕES AQUI