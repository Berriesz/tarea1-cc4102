cmake_minimum_required(VERSION 3.20)
project(Tarea1_cc4102 C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(Tarea1_cc4102
        src/main.c
        src/binomial_queue.c
        include/leer_grafo.h
)