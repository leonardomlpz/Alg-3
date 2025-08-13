#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nodo{
    char operacao;
    float valor;
    struct nodo *fe;
    struct nodo *fd;
};

//void preordem(struct nodo *nodo){
//    if (nodo != NULL){
//        printf("%f", nodo->valor);
//        preordem(nodo->fe);
//        preordem(nodo->fd);
//    }
//};

int main(){
//    struct nodo *raiz;
//    if (! (raiz = malloc(sizeof(struct nodo))) )
//        return 0;
//
//    void *item;
//    item  = raiz->operacao;

    char *string = "/ - * 5.3 25.7 * 89 11.36 * 3 21\0 after null terminator";
    char *token;

    token = strtok(string, " ");
    do{
        printf("token: %s\n", token);
    } while (token = strtok(NULL, " "));
}