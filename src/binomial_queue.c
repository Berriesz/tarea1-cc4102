#include <stdio.h>
#include <stdlib.h>
#include "binomial_queue.h"

BinomialNode* crear_nodo(int vertice, double costo){
    BinomialNode  *new = (BinomialNode *)malloc(sizeof(BinomialNode));
    new->vertice = vertice;
    new->costo = costo;
    new->grado = 0;
    new->parent = NULL;
    new->child = NULL;
    new->sibling = NULL;
    return new;
}

BinomialNode* unir_arboles(BinomialNode *a, BinomialNode *b){

    BinomialNode *winner;
    BinomialNode *loser;
    BinomialNode *hijoViejo;

    if(a->costo >= b->costo){

        winner = b;
        hijoViejo = b->child;
        loser = a;

    } else{
        winner = a;
        hijoViejo = a->child;
        loser = b;
    }

    loser->parent  = winner;
    loser->sibling = hijoViejo;
    winner->child  = loser;
    winner->grado++;

    return winner;
}

BinomialNode* insertar( BinomialNode *node, int vertice, double costo){
    if (node == NULL) {
        return crear_nodo(vertice, costo);
    }
    BinomialNode *actual = crear_nodo(vertice, costo);
    BinomialNode *hermano;
    while (node != NULL && node->grado == actual->grado) {
        hermano = node->sibling;
        actual = unir_arboles(node, actual);
        node = hermano;
    }
    actual->sibling = node;
    return actual;
}


BinomialNode* extraer_min(BinomialNode *head, int *vertice, double *costo);
BinomialNode* disminuir_costo(BinomialNode *head, int vertice, double nuevo_costo);

