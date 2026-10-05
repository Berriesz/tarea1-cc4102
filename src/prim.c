#include <math.h>
#include <stdlib.h>
#include "prim.h"
#include "binomial_queue.h"
#include "fib_queue.h"

/*
 * prim_binomial
 * Calcula el árbol cobertor mínimo (MST) de un grafo conexo, no dirigido y
 * con pesos, usando el algoritmo de Prim con una cola binomial como cola de
 * prioridad. El MST queda descrito en el arreglo parent: para cada vértice
 * v distinto de la raíz, la arista (parent[v], v) pertenece al árbol.
 *
 * Parámetros:
 *   adj:    listas de adyacencia del grafo; adj[v] contiene las aristas
 *           incidentes a v como pares (vecino, peso).
 *   v:      cantidad de vértices del grafo.
 *   r:      vértice raíz desde donde parte el algoritmo.
 *   parent: salida; arreglo de tamaño v donde se guarda el padre de cada
 *           vértice en el MST (-1 para la raíz).
 *
 * Retorna:
 *   El peso total del MST.
 */
double prim_binomial(Lista *adj, long long v, int r, int *parent) {

    llamadas_decrease_bi = 0;
    intercambios_bi = 0;
    tiempo_decrease_bi = 0.0;

    double *costos = (double *)malloc((size_t)v * sizeof(double));
    BinomialNode **pos =
        (BinomialNode **)calloc((size_t)v, sizeof(BinomialNode *));

    // Lineas 1-3: inicializacion
    for (long long k = 0; k < v; k++) {
        costos[k] = INFINITY;
        parent[k] = -2;
    }

    costos[r] = 0.0;
    parent[r] = -1;

    // Linea 4: construir Q por inserciones sucesivas
    BinomialNode *Q = NULL;

    for (long long k = 0; k < v; k++)
        Q = insertar_bi(Q, pos, (int)k, costos[k]);

    // Lineas 6-13: ciclo principal
    double total = 0.0;

    while (Q != NULL) {
        int vert;
        double c;

        Q = extraer_min_bi(Q, pos, &vert, &c);

        if (vert != r)
            total += c;

        for (int a = 0; a < adj[vert].cantidad; a++) {

            int u = adj[vert].aristas[a].vecino;
            double w = adj[vert].aristas[a].peso;

            if (pos[u] != NULL && w < costos[u]) {

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
/*
 * prim_fibonacci
 * Calcula el árbol cobertor mínimo (MST) de un grafo conexo, no dirigido y
 * con pesos, usando el algoritmo de Prim con una cola de Fibonacci como cola
 * de prioridad. El MST queda descrito en el arreglo parent: para cada vértice
 * v distinto de la raíz, la arista (parent[v], v) pertenece al árbol.
 *
 * Parámetros:
 *   adj:    listas de adyacencia del grafo; adj[v] contiene las aristas
 *           incidentes a v como pares (vecino, peso).
 *   v:      cantidad de vértices del grafo.
 *   r:      vértice raíz desde donde parte el algoritmo.
 *   parent: salida; arreglo de tamaño v donde se guarda el padre de cada
 *           vértice en el MST (-1 para la raíz).
 *
 * Retorna:
 *   El peso total del MST.
 */
double prim_fibonacci(Lista *adj, long long v, int r, int *parent) {
    double *costos = (double *)malloc((size_t)v * sizeof(double));
    FibNode **pos =
        (FibNode **)calloc((size_t)v, sizeof(FibNode *));

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

        if (vert != r)
            total += c;

        for (int a = 0; a < adj[vert].cantidad; a++) {

            int u = adj[vert].aristas[a].vecino;
            double w = adj[vert].aristas[a].peso;

            if (pos[u] != NULL && w < costos[u]) {

                costos[u] = w;
                parent[u] = vert;

                Q = disminuir_costo_fib(Q, pos[u], w);
            }
        }
    }

    free(costos);
    free(pos);

    return total;
}