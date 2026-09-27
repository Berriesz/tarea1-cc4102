#include <stdio.h>
#include <stdlib.h>
#include "binomial_queue.h"

BinomialNode* crear_nodo(int vertice, int costo){
    BinomialNode  *new = (BinomialNode *)malloc(sizeof(BinomialNode));
    new->vertice = vertice;
    new->costo = costo;
    new->degree = 0;
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
    winner->degree++;

    return winner;
}

