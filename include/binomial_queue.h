#ifndef BINOMIAL_H
#define BINOMIAL_H

typedef struct BinomialNode {
    int vertice;
    double costo;
    int grado;
    struct BinomialNode *parent;
    struct BinomialNode *child;
    struct BinomialNode *sibling;
} BinomialNode;


BinomialNode* crear_nodo(int vertice, double costo);
BinomialNode* unir_arboles(BinomialNode *a, BinomialNode *b);
BinomialNode* insertar(BinomialNode *node, int vertice, double costo);

BinomialNode* extraer_min(BinomialNode *head, int *vertice, double *costo);
BinomialNode* disminuir_costo(BinomialNode *head, int vertice, double nuevo_costo);

#endif