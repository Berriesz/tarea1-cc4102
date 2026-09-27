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

BinomialNode* fusionar(BinomialNode *node, BinomialNode *arbol){

    if (node == NULL){
        return arbol;
    }

    BinomialNode *hermano;
    while (node != NULL && node->grado == arbol->grado) {
        hermano = node->sibling;
        arbol = unir_arboles(node, arbol);
        node = hermano;
    }
    arbol->sibling = node;
    return arbol;

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


BinomialNode* extraer_min(BinomialNode *head, int *vertice, double *costo){

    if(head == NULL){
        *vertice = -1;
        *costo = -1.0;
        return NULL;
    }

    BinomialNode *min_node = head;
    BinomialNode *prev = NULL;
    BinomialNode *current = head;
    for (BinomialNode *r = head -> sibling; r != NULL; r = r->sibling) {
        
        if(r->costo < min_node->costo){
            min_node = r;
            prev = current;
        }
        current = r;
    }

    if(prev == NULL){
        head = min_node->sibling;
    } else{
        prev->sibling = min_node->sibling;
    }

    BinomialNode *child = min_node->child;
    while(child != NULL){
        BinomialNode *next_child = child->sibling;
        child->parent = NULL;
        child->sibling = NULL;
        head = fusionar(head, child);
        child = next_child;
    }

    *vertice = min_node->vertice;
    *costo = min_node->costo;
    free(min_node);
    return head;
}
void disminuir_costo(BinomialNode *node, double nuevo_costo){

    if (nuevo_costo >= node->costo) {
        return; // No se puede aumentar el costo
    }

    node->costo = nuevo_costo;
    BinomialNode *x = node;
    BinomialNode *y = x->parent;

    while (y != NULL && x->costo < y->costo) {
        int temp_vertice = x->vertice;
        double temp_costo = x->costo;

        y->vertice = x->vertice;
        y->costo = x->costo;

        x->vertice = temp_vertice;
        x->costo = temp_costo;

        y = x;
        x = y->parent;
    }

}

