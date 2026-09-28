#ifndef LEER_GRAFO_H
#define LEER_GRAFO_H

// Estructura de vecino con su peso asociado
typedef struct {
    int vecino;
    double peso;
} Arista;

// Lista dinamica de aristas salientes de un nodo
typedef struct {
    Arista *aristas;
    int cantidad;
    int capacidad;
} Lista;

void lista_init(Lista *l);
void lista_agregar(Lista *l, int vecino, double peso);
void leer_grafo(const char *path, long long *v_out, long long *e_out, Lista **adj_out);
void liberar_grafo(Lista *adj, long long v);

#endif // LEER_GRAFO_H