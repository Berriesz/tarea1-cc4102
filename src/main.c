#include <stdio.h>
#include <stdlib.h>
#include "fib_queue.h"
#include "binomial_queue.h"
#include "leer_grafo.h"




int main(void) {
    FibNode *minimo = NULL;
    double costos[8] = {50.0, 20.0, 90.0, 10.0, 70.0, 30.0, 80.0, 40.0};
    for (int v = 0; v < 8; v++) {
        minimo = insertar_fib(minimo, v, costos[v]);
    }

    int v; double c;
    minimo = extraer_min_fib(minimo, &v, &c);   // saca vertice 3 (costo 10); ahora hay arboles con hijos
    printf("extraido: vertice=%d costo=%.1f\n", v, c);

    // bajamos el vertice 2 (costo 90) a 5.0
    FibNode *x = buscar(minimo, 2);
    minimo = disminuir_costo_fib(minimo, x, 5.0);

    for (int i = 0; i < 7; i++) {
        minimo = extraer_min_fib(minimo, &v, &c);
        printf("extraido: vertice=%d costo=%.1f\n", v, c);
    }
    // primero 3/10.0; luego deberia salir: 2/5.0, 1/20.0, 5/30.0, 7/40.0, 0/50.0, 4/70.0, 6/80.0
    return 0;
}