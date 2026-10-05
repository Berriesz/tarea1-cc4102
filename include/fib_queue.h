#ifndef FIB_QUEUE_H
#define FIB_QUEUE_H

/*
 * FibNode
 * Nodo de una cola de Fibonacci. Tanto las raíces como los hijos de cada nodo
 * forman listas circulares doblemente enlazadas (mediante izq y der). La cola
 * se maneja con un puntero a la raíz de menor costo.
 *
 * Campos:
 *   vertice: vértice del grafo que representa el nodo.
 *   costo:   costo (clave) del vértice; los árboles cumplen orden de heap.
 *   grado:   cantidad de hijos del nodo.
 *   marcado: 1 si el nodo perdió un hijo desde la última vez que pasó a ser
 *            hijo de otro nodo, 0 si no.
 *   padre:   padre del nodo (NULL si es raíz).
 *   hijo:    alguno de los hijos del nodo (NULL si no tiene hijos).
 *   izq:     hermano izquierdo en la lista circular.
 *   der:     hermano derecho en la lista circular.
 */
typedef struct FibNode {
    int vertice;
    double costo;
    int grado;
    int marcado;
    struct FibNode *padre;
    struct FibNode *hijo;
    struct FibNode *izq;
    struct FibNode *der;
} FibNode;

// Crea un nodo nuevo, sin hijos y sin marcar.
FibNode* crear_nodo_fib(int vertice, double costo);

// Inserta un vértice en la lista de raíces y registra su nodo en pos.
FibNode* insertar_fib(FibNode *node, FibNode **pos, int vertice, double costo);

// Une las raíces de igual grado y retorna el nuevo mínimo.
FibNode* consolidar_fib(FibNode *node);

// Extrae el vértice de menor costo y lo devuelve por vertice y costo.
FibNode* extraer_min_fib(FibNode *node, FibNode **pos, int *vertice, double *costo);

// Hace que la raíz y pase a ser hija de la raíz x.
void enlazar_fib(FibNode *y, FibNode *x);

// Corta x de su padre y lo mueve a la lista de raíces.
void cortar_fib(FibNode *minimo, FibNode *x, FibNode *y);

// Aplica la regla de cortes en cascada desde y hacia arriba.
void corte_en_cascada_fib(FibNode *minimo, FibNode *y);

// Disminuye el costo de un nodo y retorna el mínimo actualizado.
FibNode* disminuir_costo_fib(FibNode *minimo, FibNode *x, double nuevo_costo);

#endif // FIB_QUEUE_H