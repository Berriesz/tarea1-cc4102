/*
 * leer_grafo.c
 * ------------
 * Lee un archivo de grafo con el formato que produce generar_grafo.c 
 * y reconstruye la lista de adyacencia en memoria: un arreglo adj de v
 * listas, donde adj[x] contiene, para cada vecino de x, el nodo vecino
 * y el peso de la arista que los une. 
 */

#include <stdio.h>
#include <stdlib.h>
#include "leer_grafo.h"


// Inicializar lista
void lista_init(Lista *l) {
    l->aristas = NULL;
    l->cantidad = 0;
    l->capacidad = 0;
}

void lista_agregar(Lista *l, int vecino, double peso) {
    if (l->cantidad == l->capacidad) {
        l->capacidad = (l->capacidad == 0) ? 4 : l->capacidad * 2;
        l->aristas = (Arista *)realloc(l->aristas, l->capacidad * sizeof(Arista));
    }
    l->aristas[l->cantidad].vecino = vecino;
    l->aristas[l->cantidad].peso = peso;
    l->cantidad++;
}

/* Funcion principal: lee el archivo y deja el grafo en *adj_out (arreglo de v listas). */
void leer_grafo(const char *path, long long *v_out, long long *e_out, Lista **adj_out) {
    FILE *f = fopen(path, "r");
 
    // Leer cantidad de aristas y vertices
    long long v, e;
    fscanf(f, "%lld %lld", &v, &e);
    
    // Crear lista de adjacencias
    Lista *adj = (Lista *)malloc((size_t)v * sizeof(Lista));
    
    // Inicializar lista
    for (long long k = 0; k < v; k++) lista_init(&adj[k]);
    
    // Llenar lista
    for (long long k = 0; k < e; k++) {
        int u, x;
        double w;
        fscanf(f, "%d %d %lf", &u, &x, &w);
        
        // Cada arista se guarda en ambos sentidos
        lista_agregar(&adj[u], x, w);
        lista_agregar(&adj[x], u, w);
    }

    fclose(f);
    *v_out = v;
    *e_out = e;
    *adj_out = adj;
}

void liberar_grafo(Lista *adj, long long v) {
    for (long long k = 0; k < v; k++) free(adj[k].aristas);
    free(adj);
}

