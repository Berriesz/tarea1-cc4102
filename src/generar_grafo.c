/*
 * generar_grafo.c
 * ----------------
 * Genera un grafo no dirigido, conexo y simple con:
 *      v = 2^i nodos
 *      e = 2^j aristas
 * y pesos w(e) aleatorios en el rango (0, 1].
 *
 * Formato de salida (texto plano, lista de aristas):
 *      v e
 *      u_1 v_1 w_1
 *      u_2 v_2 w_2
 *      ...
 *      u_e v_e w_e
 *
 * Compilacion:
 *      gcc -O2 -std=c11 -o generar_grafo generar_grafo.c
 *
 * Uso:
 *      ./generar_grafo <i> <j> <archivo_salida> [seed]
 *
 * Ejemplo: v=2^20, e=2^20, seed 123
 *      ./generar_grafo 20 20 grafo_i20_j20.txt 123
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --- Estructura para una arista --- */

typedef struct {
    int u;
    int v;
    double w;
} Edge;

/* --- Lista de adyacencia simple (un arreglo dinamico por nodo) --- */

typedef struct {
    int *vecinos;
    int cantidad;
    int capacidad;
} Lista;

static void lista_init(Lista *l) {
    l->vecinos = NULL;
    l->cantidad = 0;
    l->capacidad = 0;
}

/* --- Funciones auxiliares para listas de adyacencia de nodos --- */

// Buscar si un nodo ya esta en la lista de otro nodo
static int lista_contiene(const Lista *l, int x) {
    for (int k = 0; k < l->cantidad; k++) {
        if (l->vecinos[k] == x) return 1;
    }
    return 0;
}

// Agregar un nodo a la lista de otro nodo, estas listas son arreglos dinamicos (se duplica su tamaño si requiere mas memoria)
static void lista_agregar(Lista *l, int x) {
    if (l->cantidad == l->capacidad) {
        l->capacidad = (l->capacidad == 0) ? 4 : l->capacidad * 2;
        l->vecinos = (int *)realloc(l->vecinos, l->capacidad * sizeof(int));
    }
    l->vecinos[l->cantidad++] = x;
}
// Agregar la artista (a,b) en ambos sentidos, si no existia previamente
static int agregar_si_no_existe(Lista *adj, int a, int b) {
    if (lista_contiene(&adj[a], b)) return 0;  // Ya existia
    lista_agregar(&adj[a], b);
    lista_agregar(&adj[b], a);
    return 1;
}

/* --- Utilidades de aleatoriedad --- */

// Entero uniforme en [0,n) para las primeras conexiones de los nodos
static long long rand_below(long long n) {
    double frac = (double)rand() / ((double)RAND_MAX + 1.0);
    long long r = (long long)(frac * (double)n);
    if (r >= n) r = n - 1; 
    if (r < 0) r = 0;
    return r;
}

// Double uniforme en (0,1] para los pesos de los nodos
static double rand_weight(void) {
    double frac = (double)rand() / ((double)RAND_MAX + 1.0);
    return 1.0 - frac;
}

/* --- Programa principal --- */

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Uso: %s <i> <j> <archivo_salida> [seed]\n", argv[0]);
        fprintf(stderr, "  genera un grafo conexo con v=2^i nodos y e=2^j aristas\n");
        fprintf(stderr, "  pesos uniformes en (0,1]\n");
        return 1;
    }

    int i = atoi(argv[1]);
    int j = atoi(argv[2]);
    const char *outpath = argv[3];
    unsigned long seed = strtoul(argv[4], NULL, 10);
    
    // Definir v y e                     
    long long v = 1LL << i;
    long long e = 1LL << j;

    // Semilla de rand
    srand((unsigned int)seed);

    // Arreglo de listas de adyacencia, una por nodo
    Lista *adj = (Lista *)malloc((size_t)v * sizeof(Lista));

    for (long long k = 0; k < v; k++) lista_init(&adj[k]);

    Edge *edges = (Edge *)malloc(sizeof(Edge) * (size_t)e);

    // Contador para la cantidad de aristas ya conectadas
    long long count = 0;

    /* --- Paso 1: arbol cobertor aleatorio --- 
     * Cada nodo k (desde 1 hasta v-1) se conecta a un padre elegido
     * uniformemente entre los nodos [0, k-1] ya "existentes". 
     */
    for (long long k = 1; k < v; k++) {
        long long padre = rand_below(k);   /* uniforme en [0, k-1] */
        double w = rand_weight();

        edges[count].u = (int)k;
        edges[count].v = (int)padre;
        edges[count].w = w;
        lista_agregar(&adj[k], (int)padre);
        lista_agregar(&adj[padre], (int)k);
        count++;
    }

    /* --- Paso 2: aristas restantes al azar ---
     * Se sortean pares (a,b) uniformes en [0,v), si son el mismo nodo (a==b)
     * o ya existen (se revisa con lista_contiene), se descartan
     * y se vuelve a sortear.
     */
    while (count < e) {
        long long a = rand_below(v);
        long long b = rand_below(v);
        if (a == b) continue;
        if (!agregar_si_no_existe(adj, (int)a, (int)b)) continue;
        edges[count].u = (int)a;
        edges[count].v = (int)b;
        edges[count].w = rand_weight();
        count++;
    }

    /* --- Paso 3: escribir a archivo de texto --- */
    FILE *f = fopen(outpath, "w");

    fprintf(f, "%lld %lld\n", v, e);
    for (long long k = 0; k < e; k++) {
        fprintf(f, "%d %d %.17g\n", edges[k].u, edges[k].v, edges[k].w);
    }
    fclose(f);

    fprintf(stderr,
        "Grafo generado: v=%lld nodos, e=%lld aristas -> '%s' (seed=%lu)\n",
        v, e, outpath, seed);

    // Liberar memoria
    for (long long k = 0; k < v; k++) free(adj[k].vecinos);
    free(adj);
    free(edges);
    return 0;
}
