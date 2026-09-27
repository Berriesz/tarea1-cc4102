// leer_grafo.h
#ifndef LEER_GRAFO_H
#define LEER_GRAFO_H

typedef struct {
    int vecino;
    double peso;
} Arista;

typedef struct {
    Arista *aristas;
    int cantidad;
    int capacidad;
} Lista;

void leer_grafo(const char *path, long long *v_out, long long *e_out, Lista **adj_out);

#endif