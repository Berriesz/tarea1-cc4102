# Tarea 1 — Algoritmo de Prim

Implementación del algoritmo de Prim utilizando dos estructuras de cola de prioridad:

* Cola de prioridad binomial.
* Cola de prioridad de Fibonacci.

El proyecto corresponde a la Tarea 1 del curso **CC4102 — Diseño y Análisis de Algoritmos**.

## Estructura del proyecto

```text
.
├── CMakeLists.txt
├── README.md
├── include/
│   ├── binomial_queue.h
│   ├── fib_queue.h
│   ├── leer_grafo.h
│   └── prim.h
└── src/
    ├── binomial_queue.c
    ├── fib_queue.c
    ├── generar_grafo.c
    ├── leer_grafo.c
    ├── main.c
    └── prim.c
```

## Compilación

El programa puede compilarse directamente con GCC mediante:

```bash
gcc -std=c11 -Wall -Wextra -Iinclude \
    src/main.c \
    src/binomial_queue.c \
    src/fib_queue.c \
    src/leer_grafo.c \
    src/prim.c \
    -lm \
    -o main
```

Para compilar el generador de grafos:

```bash
gcc -std=c11 -Wall -Wextra -O2 \
    src/generar_grafo.c \
    -o generar_grafo
```

## Generación de grafos

El generador recibe los valores `i` y `j`, y crea un grafo con:

$$
v = 2^i
$$

vértices y

$$
e = 2^j
$$

aristas.

Uso:

```bash
./generar_grafo <i> <j> <archivo_salida> [seed]
```

Por ejemplo:

```bash
./generar_grafo 18 20 grafo.txt 1
```

## Ejecución

Una vez generado el grafo, se ejecuta:

```bash
./main grafo.txt
```

El programa ejecuta Prim utilizando ambas colas de prioridad y reporta los tiempos de ejec
