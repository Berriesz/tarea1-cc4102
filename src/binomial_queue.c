#include <stdio.h>
#include <stdlib.h>
#include "binomial_queue.h"

/*
 * crear_nodo_bi
 * Crea un nodo nuevo de cola binomial, sin padre, hijos ni hermanos.
 *
 * Parámetros:
 *   vertice: vértice del grafo que representa el nodo.
 *   costo:   costo (clave) asociado al vértice.
 *
 * Retorna:
 *   Puntero al nodo creado (árbol de grado 0).
 */
BinomialNode* crear_nodo_bi(int vertice, double costo) {
    BinomialNode *nuevo = (BinomialNode *)malloc(sizeof(BinomialNode));
    nuevo->vertice = vertice;
    nuevo->costo = costo;
    nuevo->grado = 0;
    nuevo->parent = NULL;
    nuevo->child = NULL;
    nuevo->sibling = NULL;
    return nuevo;
}

/*
 * unir_arboles_bi
 * Une dos árboles binomiales del mismo grado k en uno de grado k+1.
 * La raíz de menor costo queda como raíz, y la otra pasa a ser su primer hijo.
 *
 * Parámetros:
 *   a, b: raíces de los dos árboles a unir (deben tener el mismo grado).
 *
 * Retorna:
 *   Puntero a la raíz del árbol resultante.
 */
BinomialNode* unir_arboles_bi(BinomialNode *a, BinomialNode *b) {
    BinomialNode *winner;
    BinomialNode *loser;
    BinomialNode *hijoViejo;

    if (a->costo >= b->costo) {
        winner = b;
        hijoViejo = b->child;
        loser = a;
    } else {
        winner = a;
        hijoViejo = a->child;
        loser = b;
    }

    loser->parent  = winner;
    loser->sibling = hijoViejo;
    winner->child  = loser;
    winner->grado++;

    return winner;
}

/*
 * mezclar_listas_bi
 * Mezcla dos listas de raíces, cada una ordenada por grado creciente, en una
 * sola lista ordenada por grado. No une árboles: puede dejar dos raíces del
 * mismo grado seguidas.
 *
 * Parámetros:
 *   a, b: primeras raíces de cada lista (pueden ser NULL).
 *
 * Retorna:
 *   Puntero a la primera raíz de la lista mezclada.
 */
BinomialNode* mezclar_listas_bi(BinomialNode *a, BinomialNode *b) {
    BinomialNode dummy;
    dummy.sibling = NULL;
    BinomialNode *cola = &dummy;
    while (a != NULL && b != NULL) {
        if (a->grado <= b->grado) {
            cola->sibling = a;
            a = a->sibling;
        } else {
            cola->sibling = b;
            b = b->sibling;
        }
        cola = cola->sibling;
    }
    cola->sibling = (a != NULL) ? a : b;
    return dummy.sibling;
}

/*
 * unir_colas_bi
 * Une dos colas binomiales en una sola. Primero mezcla sus listas de raíces y
 * luego une los árboles de igual grado, de modo que no quede ningún grado
 * repetido.
 *
 * Parámetros:
 *   a, b: primeras raíces de cada cola (pueden ser NULL).
 *
 * Retorna:
 *   Puntero a la primera raíz de la cola resultante, o NULL si ambas estaban
 *   vacías.
 */
BinomialNode* unir_colas_bi(BinomialNode *a, BinomialNode *b) {
    BinomialNode *head = mezclar_listas_bi(a, b);
    if (head == NULL) return NULL;

    BinomialNode *prev = NULL, *x = head, *next = x->sibling;
    while (next != NULL) {
        if (x->grado != next->grado ||
            (next->sibling != NULL && next->sibling->grado == x->grado)) {
            prev = x;
            x = next;
        } else {
            BinomialNode *despues = next->sibling;
            BinomialNode *w = unir_arboles_bi(x, next);
            w->sibling = despues;
            if (prev == NULL) head = w; else prev->sibling = w;
            x = w;
        }
        next = x->sibling;
    }
    return head;
}

/*
 * insertar_bi
 * Inserta un vértice con su costo en la cola. Crea un árbol de grado 0 y lo
 * une con las primeras raíces mientras tengan su mismo grado, como al sumar 1
 * a un contador binario. Registra el nodo creado en pos[vertice].
 *
 * Parámetros:
 *   node:    primera raíz de la cola (NULL si está vacía).
 *   pos:     arreglo de punteros desde cada vértice a su nodo en la cola.
 *   vertice: vértice a insertar.
 *   costo:   costo inicial del vértice.
 *
 * Retorna:
 *   Puntero a la primera raíz de la cola actualizada.
 */
BinomialNode* insertar_bi(BinomialNode *node, BinomialNode **pos, int vertice, double costo) {
    BinomialNode *actual = crear_nodo_bi(vertice, costo);
    BinomialNode *hermano;
    pos[vertice] = actual;

    if (node == NULL) return actual;

    while (node != NULL && node->grado == actual->grado) {
        hermano = node->sibling;
        actual = unir_arboles_bi(node, actual);
        node = hermano;
    }
    actual->sibling = node;
    return actual;
}

/*
 * extraer_min_bi
 * Extrae el nodo de menor costo de la cola. Busca la raíz mínima, la saca de
 * la lista de raíces, invierte la lista de sus hijos (para dejarla en grado
 * creciente) y la une con el resto de la cola. Libera el nodo extraído y deja
 * pos[vertice] en NULL.
 *
 * Parámetros:
 *   head:    primera raíz de la cola (NULL si está vacía).
 *   pos:     arreglo de punteros desde cada vértice a su nodo en la cola.
 *   vertice: salida; vértice extraído (-1 si la cola estaba vacía).
 *   costo:   salida; costo del vértice extraído (-1.0 si la cola estaba vacía).
 *
 * Retorna:
 *   Puntero a la primera raíz de la cola actualizada (NULL si quedó vacía).
 */
BinomialNode* extraer_min_bi(BinomialNode *head, BinomialNode **pos, int *vertice, double *costo) {
    if (head == NULL) {
        *vertice = -1;
        *costo = -1.0;
        return NULL;
    }

    // buscar la raiz minima
    BinomialNode *min_node = head;
    BinomialNode *prev = NULL;
    BinomialNode *current = head;
    for (BinomialNode *r = head->sibling; r != NULL; r = r->sibling) {
        if (r->costo < min_node->costo) {
            min_node = r;
            prev = current;
        }
        current = r;
    }

    // sacarla de la lista de raices
    if (prev == NULL) head = min_node->sibling;
    else prev->sibling = min_node->sibling;

    // dar vuelta la lista de hijos (quedan en grado creciente)
    BinomialNode *rev = NULL;
    BinomialNode *c = min_node->child;
    while (c != NULL) {
        BinomialNode *sig = c->sibling;
        c->parent  = NULL;
        c->sibling = rev;
        rev = c;
        c = sig;
    }
    head = unir_colas_bi(head, rev);

    *vertice = min_node->vertice;
    *costo = min_node->costo;
    pos[min_node->vertice] = NULL;
    free(min_node);
    return head;
}

/*
 * disminuir_costo_bi
 * Disminuye el costo de un nodo (decreaseKey). Mientras el nodo tenga menor
 * costo que su padre, intercambia el contenido (vértice y costo) de ambos y
 * actualiza sus entradas en pos. Si el nuevo costo no es menor que el actual,
 * no hace nada.
 *
 * Parámetros:
 *   node:        nodo cuyo costo se quiere disminuir.
 *   pos:         arreglo de punteros desde cada vértice a su nodo en la cola.
 *   nuevo_costo: nuevo costo del vértice.
 *
 * Retorna:
 *   Nada. La lista de raíces no cambia, así que no se retorna una nueva cabeza.
 */
void disminuir_costo_bi(BinomialNode *node, BinomialNode **pos, double nuevo_costo) {
    if (nuevo_costo >= node->costo) return;

    node->costo = nuevo_costo;
    BinomialNode *x = node;
    BinomialNode *y = x->parent;

    while (y != NULL && x->costo < y->costo) {
        int temp_vertice = y->vertice;
        double temp_costo = y->costo;

        y->vertice = x->vertice;
        y->costo   = x->costo;
        x->vertice = temp_vertice;
        x->costo   = temp_costo;

        pos[y->vertice] = y;
        pos[x->vertice] = x;

        x = y;
        y = x->parent;
    }
}