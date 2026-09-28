#ifndef PRIM_H
#define PRIM_H

#include "leer_grafo.h"

/* Prim con cola binomial.
 * Entrada: adj (lista de adyacencia), v (nº de vertices), r (raiz),
 *          parent (arreglo de tamaño v, lo llena la funcion)
 * Salida:  peso total del MST; el arbol queda en parent (parent[r] = -1). */
double prim_binomial(Lista *adj, long long v, int r, int *parent);

/* Igual que la anterior, pero con cola de Fibonacci. */
double prim_fibonacci(Lista *adj, long long v, int r, int *parent);

#endif // PRIM_H