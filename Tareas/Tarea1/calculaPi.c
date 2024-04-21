// Nathalie Alfaro Quesada, B90221
// Compilar  gcc -o calculaPi calculaPi.c -lm
// Ejecutar  time ./calculaPi 100000000 4

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <stdatomic.h>

#define MAX_THREADS 50000

int total_threads;
int total_points;

// Hacerla tipo atomica
_Atomic int points_inside_circle = 0;

void *throw_darts(void *arg)
{
    int childID = (uintptr_t)arg;
    int points_per_thread = total_points / total_threads;
    unsigned int seed = time(NULL) * (childID + 1);

    // Crear una local
    int local_points_inside_circle = 0;
    for (int i = 0; i < points_per_thread; i++)
    {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (sqrt(x * x + y * y) <= 1)
        {
            // Incrementar la local dentro del ciclo
            local_points_inside_circle++;
        }
    }
    // Retornar la cantidad de puntos dentro del círculo
    // (usando un falso puntero) y hagan que el hilo principal
    // incremente la variable global
    // puntero de tipo void* generico que apunta cualquier tipo
    // tipo int grande como para guardar un puntero
    return (void*)(intptr_t)local_points_inside_circle;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Uso: %s <total_points> <total_threads>\n", argv[0]);
        return 1;
    }
    total_points = atoi(argv[1]);
    total_threads = atoi(argv[2]);
    if (total_threads > MAX_THREADS)
    {
        printf("El número de hilos no puede ser mayor a %d\n", MAX_THREADS);
        return 1;
    }
    if (total_points % total_threads != 0)
    {
        printf("El número de puntos debe ser divisible entre el número de hilos\n");
        return 1;
    }

    // Acá deberán introducir el paralelismo:
    // arreglo de tipo pthread_t y de tamano MAX_THREADS
    pthread_t threads[MAX_THREADS];

    // Este ciclo es para crear hilos 
    // uintptr_t tipo de dato que contiene un int sin signo puntero
    for (uintptr_t i = 0; i < total_threads; i++)
    {
        // Llamamos a la funcion para crear un nuevo hilo
        // Hilo i en el arreglo threads - Sin atributos - Funcion para el hilo - Argumento para la funcion
        pthread_create(&threads[i], NULL, throw_darts, (void *)i);
    }

    // Para cada uno de los hilos creados
    for (uintptr_t i = 0; i < total_threads; i++)
    {
        void *result;
        // Esperamos que el hilo i termine y se una al principal - Guardar valor de retorno del hilo
        pthread_join(threads[i], &result);
        // La cantidad de puntos en el circulo generados por el hilo i se guardan en
        // result al retornar cada hilo i
        int local_points_inside_circle = (intptr_t)result;
        // Sumar atomicamente ya que hay varios hilos y asi tener el resultado correcto
        atomic_fetch_add(&points_inside_circle, local_points_inside_circle);
    }

    double pi = 4.0 * points_inside_circle / total_points;
    printf("Valor de pi usando operaciones atómicas: %f\n", pi);

    return 0;
}
