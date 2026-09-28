#include <stdio.h>
#include <stdlib.h>
#include "fib_queue.h"
#include "binomial_queue.h"
#include "leer_grafo.h"




int main(void) {
    FibNode **pos = (FibNode **)calloc(8, sizeof(FibNode *));
    FibNode *minimo = NULL;
    double costos[8] = {50.0, 20.0, 90.0, 10.0, 70.0, 30.0, 80.0, 40.0};
    for (int v = 0; v < 8; v++) {
        minimo = insertar_fib(minimo, pos, v, costos[v]);
    }
    minimo = disminuir_costo_fib(minimo, pos[2], 5.0);   // directo, sin buscar
    return 0;
}