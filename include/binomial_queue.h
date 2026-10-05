#ifndef BINOMIAL_H
#define BINOMIAL_H

/*
 * BinomialNode
 * Nodo de una cola binomial. Cada árbol se representa con punteros al padre,
 * al primer hijo y al siguiente hermano. Las raíces de la cola forman una
 * lista enlazada (mediante sibling) ordenada por grado creciente, y la cola
 * se maneja con un puntero a la primera raíz.
 *
 * Campos:
 *   vertice: vértice del grafo que representa el nodo.
 *   costo:   costo (clave) del vértice; los árboles cumplen orden de heap.
 *   grado:   cantidad de hijos del nodo.
 *   parent:  padre del nodo (NULL si es raíz).
 *   child:   primer hijo del nodo (NULL si no tiene hijos).
 *   sibling: siguiente hermano, o siguiente raíz si el nodo es raíz.
 */
typedef struct BinomialNode {
    int vertice;
    double costo;
    int grado;
    struct BinomialNode *parent;
    struct BinomialNode *child;
    struct BinomialNode *sibling;
} BinomialNode;

// Crea un nodo nuevo de grado 0.
BinomialNode* crear_nodo_bi(int vertice, double costo);

// Une dos árboles del mismo grado k en uno de grado k+1.
BinomialNode* unir_arboles_bi(BinomialNode *a, BinomialNode *b);

// Mezcla dos listas de raíces ordenadas por grado en una sola.
BinomialNode* mezclar_listas_bi(BinomialNode *a, BinomialNode *b);

// Une dos colas binomiales completas en una sola.
BinomialNode* unir_colas_bi(BinomialNode *a, BinomialNode *b);

// Inserta un vértice con su costo y registra su nodo en pos.
BinomialNode* insertar_bi(BinomialNode *node, BinomialNode **pos, int vertice, double costo);

// Extrae el vértice de menor costo y lo devuelve por vertice y costo.
BinomialNode* extraer_min_bi(BinomialNode *head, BinomialNode **pos, int *vertice, double *costo);

// Disminuye el costo de un nodo intercambiando contenido con sus ancestros.
void disminuir_costo_bi(BinomialNode *x, BinomialNode **pos, double nuevo_costo);

extern long long llamadas_decrease_bi;
extern long long intercambios_bi;
extern double tiempo_decrease_bi;


#endif