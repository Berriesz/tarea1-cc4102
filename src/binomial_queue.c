#include <stdio.h>
#include <stdlib.h>
#include "binomial_queue.h"

BinomialNode* crear_nodo_bi(int vertice, double costo) {
    BinomialNode *nuevo = (BinomialNode *)malloc(sizeof(BinomialNode));
    nuevo->vertice = vertice;
    nuevo->costo = costo;
    nuevo->grado = 0;
    nuevo->parent = NULL;
    nuevo->child = NULL;
    nuevo->sibling = NULL;
    return nuevo;
}

BinomialNode* unir_arboles_bi(BinomialNode *a, BinomialNode *b) {
    BinomialNode *winner;
    BinomialNode *loser;
    BinomialNode *hijoViejo;

    if (a->costo >= b->costo) {
        winner = b;
        hijoViejo = b->child;
        loser = a;
    } else {
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

// Mezcla dos listas de raices (ya ordenadas por grado) en una sola ordenada
BinomialNode* mezclar_listas_bi(BinomialNode *a, BinomialNode *b) {
    BinomialNode dummy;
    dummy.sibling = NULL;
    BinomialNode *cola = &dummy;
    while (a != NULL && b != NULL) {
        if (a->grado <= b->grado) {
            cola->sibling = a;
            a = a->sibling;
        } else {
            cola->sibling = b;
            b = b->sibling;
        }
        cola = cola->sibling;
    }
    cola->sibling = (a != NULL) ? a : b;
    return dummy.sibling;
}

// Une dos colas binomiales completas
BinomialNode* unir_colas_bi(BinomialNode *a, BinomialNode *b) {
    BinomialNode *head = mezclar_listas_bi(a, b);
    if (head == NULL) return NULL;

    BinomialNode *prev = NULL, *x = head, *next = x->sibling;
    while (next != NULL) {
        if (x->grado != next->grado ||
            (next->sibling != NULL && next->sibling->grado == x->grado)) {
            prev = x;
            x = next;
        } else {
            BinomialNode *despues = next->sibling;
            BinomialNode *w = unir_arboles_bi(x, next);
            w->sibling = despues;
            if (prev == NULL) head = w; else prev->sibling = w;
            x = w;
        }
        next = x->sibling;
    }
    return head;
}

BinomialNode* insertar_bi(BinomialNode *node, BinomialNode **pos, int vertice, double costo) {
    BinomialNode *actual = crear_nodo_bi(vertice, costo);
    BinomialNode *hermano;
    pos[vertice] = actual;

    if (node == NULL) return actual;

    while (node != NULL && node->grado == actual->grado) {
        hermano = node->sibling;
        actual = unir_arboles_bi(node, actual);
        node = hermano;
    }
    actual->sibling = node;
    return actual;
}

BinomialNode* extraer_min_bi(BinomialNode *head, BinomialNode **pos, int *vertice, double *costo) {
    if (head == NULL) {
        *vertice = -1;
        *costo = -1.0;
        return NULL;
    }

    // buscar la raiz minima
    BinomialNode *min_node = head;
    BinomialNode *prev = NULL;
    BinomialNode *current = head;
    for (BinomialNode *r = head->sibling; r != NULL; r = r->sibling) {
        if (r->costo < min_node->costo) {
            min_node = r;
            prev = current;
        }
        current = r;
    }

    // sacarla de la lista de raices
    if (prev == NULL) head = min_node->sibling;
    else prev->sibling = min_node->sibling;

    // dar vuelta la lista de hijos (quedan en grado creciente)
    BinomialNode *rev = NULL;
    BinomialNode *c = min_node->child;
    while (c != NULL) {
        BinomialNode *sig = c->sibling;
        c->parent  = NULL;
        c->sibling = rev;
        rev = c;
        c = sig;
    }
    head = unir_colas_bi(head, rev);

    *vertice = min_node->vertice;
    *costo = min_node->costo;
    pos[min_node->vertice] = NULL;
    free(min_node);
    return head;
}

void disminuir_costo_bi(BinomialNode *node, BinomialNode **pos, double nuevo_costo) {
    if (nuevo_costo >= node->costo) return;

    node->costo = nuevo_costo;
    BinomialNode *x = node;
    BinomialNode *y = x->parent;

    while (y != NULL && x->costo < y->costo) {
        int temp_vertice = y->vertice;
        double temp_costo = y->costo;

        y->vertice = x->vertice;
        y->costo   = x->costo;
        x->vertice = temp_vertice;
        x->costo   = temp_costo;

        pos[y->vertice] = y;
        pos[x->vertice] = x;

        x = y;
        y = x->parent;
    }
}