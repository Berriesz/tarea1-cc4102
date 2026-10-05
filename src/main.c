#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "leer_grafo.h"
#include "prim.h"
#include "binomial_queue.h"
#include "fib_queue.h"

double tiempo_ms(struct timespec inicio, struct timespec fin) {
    return (fin.tv_sec - inicio.tv_sec) * 1000.0
           + (fin.tv_nsec - inicio.tv_nsec) / 1000000.0;
}

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Uso: %s <grafo>\n", argv[0]);
        return 1;
    }

    long long v, e;
    Lista *adj;

    leer_grafo(argv[1], &v, &e, &adj);

    int *parent = malloc(v * sizeof(int));

    struct timespec inicio, fin;

    /* Prim con cola binomial */
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    double total_bi = prim_binomial(adj, v, 0, parent);
    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo_bi = tiempo_ms(inicio, fin);

    /* Guardamos las mediciones de decreaseKey binomial */
    long long llamadas_bi = llamadas_decrease_bi;
    long long intercambios = intercambios_bi;
    double tiempo_decrease_bi_ms = tiempo_decrease_bi / 1000000.0;

    /* Prim con cola de Fibonacci */
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    double total_fib = prim_fibonacci(adj, v, 0, parent);
    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo_fib = tiempo_ms(inicio, fin);

    /* Guardamos las mediciones de decreaseKey Fibonacci */
    long long llamadas_fib = llamadas_decrease_fib;
    long long cortes = cortes_fib;
    double tiempo_decrease_fib_ms = tiempo_decrease_fib / 1000000.0;

    /*
     * Formato:
     *
     * v e
     * MST_binomial tiempo_total_binomial
     * llamadas_binomial tiempo_decrease_binomial intercambios
     * MST_fibonacci tiempo_total_fibonacci
     * llamadas_fibonacci tiempo_decrease_fibonacci cortes
     */

    printf("%lld %lld %.17g %.9f %lld %.9f %lld %.17g %.9f %lld %.9f %lld\n",
           v, e,
           total_bi,
           tiempo_bi,
           llamadas_bi,
           tiempo_decrease_bi_ms,
           intercambios,
           total_fib,
           tiempo_fib,
           llamadas_fib,
           tiempo_decrease_fib_ms,
           cortes);

    free(parent);
    liberar_grafo(adj, v);

    return 0;
}