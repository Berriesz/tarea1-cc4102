#include <math.h>
#include <stdlib.h>
#include "prim.h"
#include "binomial_queue.h"
#include "fib_queue.h"

double prim_binomial(Lista *adj, long long v, int r, int *parent) {
    double *costos = (double *)malloc((size_t)v * sizeof(double));
    BinomialNode **pos = (BinomialNode **)calloc((size_t)v, sizeof(BinomialNode *));

    // lineas 1-3: inicializacion
    for (long long k = 0; k < v; k++) {
        costos[k] = INFINITY;
        parent[k] = -2;                      // indefinido
    }
    costos[r] = 0.0;
    parent[r] = -1;

    // linea 4: construir Q por inserciones sucesivas
    BinomialNode *Q = NULL;
    for (long long k = 0; k < v; k++)
        Q = insertar_bi(Q, pos, (int)k, costos[k]);

    // lineas 6-13: ciclo principal
    double total = 0.0;
    while (Q != NULL) {
        int vert;
        double c;
        Q = extraer_min_bi(Q, pos, &vert, &c);
        if (vert != r) total += c;           // arista (parent[vert], vert)

        for (int a = 0; a < adj[vert].cantidad; a++) {
            int u = adj[vert].aristas[a].vecino;
            double w = adj[vert].aristas[a].peso;
            if (pos[u] != NULL && w < costos[u]) {   // u sigue en Q y mejora
                costos[u] = w;
                parent[u] = vert;
                disminuir_costo_bi(pos[u], pos, w);
            }
        }
    }

    free(costos);
    free(pos);
    return total;
}

double prim_fibonacci(Lista *adj, long long v, int r, int *parent) {
    double *costos = (double *)malloc((size_t)v * sizeof(double));
    FibNode **pos = (FibNode **)calloc((size_t)v, sizeof(FibNode *));

    for (long long k = 0; k < v; k++) {
        costos[k] = INFINITY;
        parent[k] = -2;
    }
    costos[r] = 0.0;
    parent[r] = -1;

    FibNode *Q = NULL;
    for (long long k = 0; k < v; k++)
        Q = insertar_fib(Q, pos, (int)k, costos[k]);

    double total = 0.0;
    while (Q != NULL) {
        int vert;
        double c;
        Q = extraer_min_fib(Q, pos, &vert, &c);
        if (vert != r) total += c;

        for (int a = 0; a < adj[vert].cantidad; a++) {
            int u = adj[vert].aristas[a].vecino;
            double w = adj[vert].aristas[a].peso;
            if (pos[u] != NULL && w < costos[u]) {
                costos[u] = w;
                parent[u] = vert;
                Q = disminuir_costo_fib(Q, pos[u], w);   // puede cambiar el minimo
            }
        }
    }

    free(costos);
    free(pos);
    return total;
}