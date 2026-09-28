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

BinomialNode* crear_nodo_bi(int vertice, double costo);
BinomialNode* unir_arboles_bi(BinomialNode *a, BinomialNode *b);
BinomialNode* mezclar_listas_bi(BinomialNode *a, BinomialNode *b);
BinomialNode* unir_colas_bi(BinomialNode *a, BinomialNode *b);
BinomialNode* insertar_bi(BinomialNode *node, BinomialNode **pos, int vertice, double costo);
BinomialNode* extraer_min_bi(BinomialNode *head, BinomialNode **pos, int *vertice, double *costo);
void disminuir_costo_bi(BinomialNode *x, BinomialNode **pos, double nuevo_costo);

#endif