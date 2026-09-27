#ifndef BINOMIAL_H
#define BINOMIAL_H

typedef struct BinomialNode {
    int vertice;
    int costo;
    int degree;
    struct BinomialNode *parent;
    struct BinomialNode *child;
    struct BinomialNode *sibling;
} BinomialNode;


BinomialNode* crear_nodo(int vertice, int costo);
BinomialNode* unir_arboles(BinomialNode *a, BinomialNode *b);


BinomialNode* link_trees(BinomialNode *a, BinomialNode *b);
BinomialNode* insert_binomial(BinomialNode *head, int vertice, int costo);
BinomialNode* extract_min(BinomialNode *head, int *vertice, int *costo);

#endif