#define _POSIX_C_SOURCE 200809
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fib_queue.h"

long long llamadas_decrease_fib = 0;
long long cortes_fib = 0;
double tiempo_decrease_fib = 0.0;

/*
 * crear_nodo_fib
 * Crea un nodo nuevo de cola de Fibonacci, sin padre ni hijos y sin marcar.
 * Sus punteros izq y der apuntan a sí mismo, formando una lista circular de
 * un solo elemento.
 *
 * Parámetros:
 *   vertice: vértice del grafo que representa el nodo.
 *   costo:   costo (clave) asociado al vértice.
 *
 * Retorna:
 *   Puntero al nodo creado.
 */
FibNode* crear_nodo_fib(int vertice, double costo) {

    FibNode *n = (FibNode *)malloc(sizeof(FibNode));

    n->vertice = vertice;
    n->costo = costo;
    n->grado = 0;
    n->marcado = 0;
    n->padre = NULL;
    n->hijo = NULL;
    n->izq = n;
    n->der = n;

    return n;
}

/*
 * insertar_fib
 * Inserta un vértice con su costo en la cola. Crea un nodo y lo agrega a la
 * lista de raíces, a la derecha del mínimo, sin unir árboles. Actualiza el
 * mínimo si corresponde y registra el nodo creado en pos[vertice].
 *
 * Parámetros:
 *   node:    raíz de menor costo de la cola (NULL si está vacía).
 *   pos:     arreglo de punteros desde cada vértice a su nodo en la cola.
 *   vertice: vértice a insertar.
 *   costo:   costo inicial del vértice.
 *
 * Retorna:
 *   Puntero a la raíz de menor costo de la cola actualizada.
 */
FibNode* insertar_fib(FibNode *node, FibNode **pos,
                      int vertice, double costo) {

    FibNode *nuevo_nodo = crear_nodo_fib(vertice, costo);

    pos[vertice] = nuevo_nodo;

    if (node == NULL) {
        return nuevo_nodo;
    }

    nuevo_nodo->der = node->der;
    nuevo_nodo->izq = node;

    node->der->izq = nuevo_nodo;
    node->der = nuevo_nodo;

    if (nuevo_nodo->costo < node->costo) {
        node = nuevo_nodo;
    }

    return node;
}

/*
 * extraer_min_fib
 * Extrae el nodo de menor costo de la cola. Sube sus hijos a la lista de
 * raíces, lo saca de la lista, lo libera y consolida la cola. Deja
 * pos[vertice] en NULL.
 *
 * Parámetros:
 *   node:    raíz de menor costo de la cola (NULL si está vacía).
 *   pos:     arreglo de punteros desde cada vértice a su nodo en la cola.
 *   vertice: salida; vértice extraído (-1 si la cola estaba vacía).
 *   costo:   salida; costo del vértice extraído (-1.0 si la cola estaba vacía).
 *
 * Retorna:
 *   Puntero a la nueva raíz de menor costo (NULL si la cola quedó vacía).
 */
FibNode* extraer_min_fib(FibNode *node, FibNode **pos,
                         int *vertice, double *costo) {

    if (node == NULL) {
        *vertice = -1;
        *costo = -1.0;
        return NULL;
    }

    FibNode *z = node;

    *vertice = z->vertice;
    *costo = z->costo;

    pos[z->vertice] = NULL;

    if (z->hijo != NULL) {

        FibNode *h = z->hijo;
        h->padre = NULL;
        FibNode *c = h->der;

        while (c != h) {
            c->padre = NULL;
            c = c->der;
        }

        FibNode *z_der = z->der;
        FibNode *h_izq = h->izq;

        z->der = h;
        h->izq = z;

        h_izq->der = z_der;
        z_der->izq = h_izq;
    }

    if (z == z->der) {

        free(z);

        return NULL;
    }

    FibNode *nuevo_inicio = z->der;
    z->izq->der = z->der;
    z->der->izq = z->izq;

    free(z);
    return consolidar_fib(nuevo_inicio);
}
/*
 * consolidar_fib
 * Une los árboles de la lista de raíces hasta que no queden dos raíces del
 * mismo grado. Primero copia las raíces a un arreglo, para no modificar la
 * lista mientras se recorre. Luego usa una tabla indexada por grado para ir
 * uniendo árboles de igual grado. Al final rearma la lista de raíces y busca
 * el nuevo mínimo.
 *
 * Parámetros:
 *   node: cualquier raíz de la lista de raíces (no puede ser NULL).
 *
 * Retorna:
 *   Puntero a la raíz de menor costo de la cola consolidada.
 */
FibNode* consolidar_fib(FibNode *node) {
    int n = 1;
    for (FibNode *r = node->der; r != node; r = r->der)
        n++;

    FibNode **raices =
        (FibNode **)malloc(n * sizeof(FibNode *));

    raices[0] = node;
    int k = 1;
    for (FibNode *r = node->der; r != node; r = r->der)
        raices[k++] = r;

    for (int i = 0; i < n; i++) {
        raices[i]->izq = raices[i];
        raices[i]->der = raices[i];
    }

    FibNode *tabla[64];
    for (int d = 0; d < 64; d++)
        tabla[d] = NULL;

    for (int i = 0; i < n; i++) {

        FibNode *x = raices[i];
        int d = x->grado;

        while (tabla[d] != NULL) {
            FibNode *y = tabla[d];

            if (y->costo < x->costo) {
                FibNode *tmp = x;
                x = y;
                y = tmp;
            }

            enlazar_fib(y, x);
            tabla[d] = NULL;
            d++;
        }
        tabla[d] = x;
    }

    free(raices);
    FibNode *min = NULL;

    for (int d = 0; d < 64; d++) {
        FibNode *t = tabla[d];

        if (t == NULL)
            continue;

        if (min == NULL) {
            min = t;

        } else {

            t->izq = min;
            t->der = min->der;

            min->der->izq = t;
            min->der = t;

            if (t->costo < min->costo)
                min = t;
        }
    }
    return min;
}


/*
 * enlazar_fib
 * Une dos árboles del mismo grado: la raíz y pasa a ser hija de la raíz x.
 * Desmarca a y y aumenta el grado de x.
 *
 * Parámetros:
 *   y: raíz que pasa a ser hija (la de mayor o igual costo).
 *   x: raíz que queda como padre (la de menor costo).
 *
 * Retorna:
 *   Nada.
 */
void enlazar_fib(FibNode *y, FibNode *x) {

    y->izq = y;
    y->der = y;

    y->padre = x;
    y->marcado = 0;

    if (x->hijo == NULL) {
        x->hijo = y;

    } else {
        FibNode *h = x->hijo;

        y->izq = h;
        y->der = h->der;

        h->der->izq = y;
        h->der = y;
    }
    x->grado++;
}
/*
 * cortar_fib
 * Corta el nodo x de su padre y: lo saca de la lista de hijos de y, disminuye
 * el grado de y y agrega x a la lista de raíces, a la derecha del mínimo.
 * Deja x sin padre y sin marcar.
 *
 * Parámetros:
 *   minimo: raíz de menor costo de la cola.
 *   x:      nodo a cortar.
 *   y:      padre de x.
 *
 * Retorna:
 *   Nada.
 */
void cortar_fib(FibNode *minimo, FibNode *x, FibNode *y) {
    cortes_fib++;
    if (x->der == x) {

        y->hijo = NULL;

    } else {

        if (y->hijo == x)
            y->hijo = x->der;

        x->izq->der = x->der;
        x->der->izq = x->izq;
    }

    y->grado--;

    x->izq = minimo;
    x->der = minimo->der;

    minimo->der->izq = x;
    minimo->der = x;

    x->padre = NULL;
    x->marcado = 0;
}

/*
 * corte_en_cascada_fib
 * Aplica la regla de cortes en cascada sobre y, que acaba de perder un hijo.
 * Si y no estaba marcado, lo marca. Si ya estaba marcado (perdió su segundo
 * hijo), lo corta de su padre y repite el proceso con ese padre. Si y es
 * raíz, no hace nada.
 *
 * Parámetros:
 *   minimo: raíz de menor costo de la cola.
 *   y:      nodo que acaba de perder un hijo.
 *
 * Retorna:
 *   Nada.
 */

void corte_en_cascada_fib(FibNode *minimo, FibNode *y) {
    FibNode *z = y->padre;
    if (z != NULL) {

        if (y->marcado == 0) {
            y->marcado = 1;

        } else {
            cortar_fib(minimo, y, z);
            corte_en_cascada_fib(minimo, z);
        }
    }
}

/*
 * disminuir_costo_fib
 * Disminuye el costo de un nodo (decreaseKey). Si el nuevo costo queda menor
 * que el de su padre, corta el nodo, lo mueve a la lista de raíces y aplica
 * los cortes en cascada. Actualiza el mínimo si corresponde. Si el nuevo
 * costo no es menor que el actual, no hace nada.
 *
 * Parámetros:
 *   minimo:      raíz de menor costo de la cola.
 *   x:           nodo cuyo costo se quiere disminuir.
 *   nuevo_costo: nuevo costo del vértice.
 *
 * Retorna:
 *   Puntero a la raíz de menor costo de la cola, que puede ser x.
 */
FibNode* disminuir_costo_fib(FibNode *minimo, FibNode *x,
                             double nuevo_costo) {

    struct timespec inicio;
    struct timespec fin;

    llamadas_decrease_fib++;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    if (nuevo_costo >= x->costo) {

        clock_gettime(CLOCK_MONOTONIC, &fin);

        tiempo_decrease_fib +=
            (fin.tv_sec - inicio.tv_sec) * 1000000000.0
            + (fin.tv_nsec - inicio.tv_nsec);

        return minimo;
    }

    x->costo = nuevo_costo;

    FibNode *y = x->padre;

    if (y != NULL && x->costo < y->costo) {

        cortar_fib(minimo, x, y);
        corte_en_cascada_fib(minimo, y);
    }

    if (x->costo < minimo->costo)
        minimo = x;

    clock_gettime(CLOCK_MONOTONIC, &fin);

    tiempo_decrease_fib +=
        (fin.tv_sec - inicio.tv_sec) * 1000000000.0
        + (fin.tv_nsec - inicio.tv_nsec);

    return minimo;
}