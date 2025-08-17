#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nodo{
    char operacao;
    float valor;
    struct nodo *fe;
    struct nodo *fd;
};

struct nodo* criar_nodo(char op, float valor){
    struct nodo *no;
    if (! (no = malloc(sizeof(struct nodo))) )
        return NULL;

    no->operacao = op;
    no->valor = valor;
    no->fe = NULL;
    no->fd = NULL;

    return no;
}

struct nodo *criar_arvore(char **tokens, int *pos, int total){
    if (*pos >= total)
        return NULL;

    char *t = tokens[*pos];
    (*pos)++;

    if (strchr("+-*/", t[0]) && strlen(t) == 1){
        struct nodo *n = criar_nodo(t[0], 0);
        n->fe = criar_arvore(tokens, pos, total);
        n->fd = criar_arvore(tokens, pos, total);
        return n;
    } else{
        float val = atof(t);
        return criar_nodo('\0', val);
    }
}

void preordem(struct nodo *nodo){
    if (nodo != NULL){
        if(nodo->operacao)
            printf("%c " ,nodo->operacao);
        else
            printf("%.2f ",nodo->valor);
        preordem(nodo->fe);
        preordem(nodo->fd);
    }
};

int main(){    
    char string[] = "/ - * 5.3 25.7 * 89 11.36 * 3 21\0";
    char *tokens[100];
    int num_tokens = 0;
    
    char *token = strtok(string, " ");
    while (token != NULL){
        tokens[num_tokens++] = token;
        token = strtok(NULL, " ");
    };
    
    int pos = 0;
    struct nodo *raiz;
    raiz = criar_arvore(tokens, &pos, num_tokens);

    preordem(raiz);
    printf("\n");

    return 0;
}