#include <stdio.h>
#include <stdlib.h>
#include "binomial_queue.h"

int main(void) {
    BinomialNode *cola = NULL;   // cola vacia al empezar

    double costos[8] = {5.0, 2.0, 9.0, 1.0, 7.0, 3.0, 8.0, 4.0};

    for (int v = 0; v < 8; v++) {
        cola = insertar(cola, v, costos[v]);
    }

    // recorremos la lista de raices imprimiendo cada arbol
    printf("=== Estado final de la cola ===\n");
    int cantidad_arboles = 0;
    for (BinomialNode *r = cola; r != NULL; r = r->sibling) {
        printf("  arbol B%d -> raiz: vertice=%d costo=%.1f\n",
               r->grado, r->vertice, r->costo);
        cantidad_arboles++;
    }
    printf("Total de arboles: %d\n", cantidad_arboles);
    printf("(deberia salir: Total de arboles: 1, y ese arbol B3)\n");

    return 0;
}
