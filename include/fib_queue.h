#ifndef FIB_QUEUE_H
#define FIB_QUEUE_H


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

FibNode* crear_nodo_fib(int vertice, double costo);
FibNode* insertar_fib(FibNode *node, int vertice, double costo);
FibNode* consolidar_fib(FibNode *node);
FibNode* extraer_min_fib(FibNode *node, int *vertice, double *costo);
void enlazar_fib(FibNode *y, FibNode *x);

void cortar_fib(FibNode *minimo, FibNode *x, FibNode *y);
void corte_en_cascada_fib(FibNode *minimo, FibNode *y);
FibNode* disminuir_costo_fib(FibNode *minimo, FibNode *x, double nuevo_costo);

#endif // FIB_QUEUE_H