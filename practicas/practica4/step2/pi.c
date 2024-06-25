// Compilar gcc -o pi pi.c
// Ejecutar ./pi

/*
RESULTADO AL EJECUTAR:

CPU pi calculated in 2.769034 s.
CPU estimate of PI = 3.141607 [error of 0.000014]
*/

// Bibliotecas
#include <stdlib.h> 
#include <stdio.h> 
#include <cuda.h> 
#include <math.h> 
#include <time.h> 

// Constantes
#define PI 3.1415926535  
#define TRIALS_PER_THREAD 4096 
#define BLOCKS 256 
#define THREADS 256
 
/*
Cálculo de Monte Carlo en la CPU
Genera puntos aleatorios en el cuadrado unitario (0,0) a (1,1).
Cuenta cuántos puntos caen dentro del círculo unitario de radio 1.
Calcula la estimación de π basada en la proporción de puntos dentro del círculo.
*/
float host_monte_carlo(long trials) { 
    float x, y; 
    long points_in_circle; 

    for(long i = 0; i < trials; i++) {
        x = rand() / (float) RAND_MAX; 
        y = rand() / (float) RAND_MAX; 
        points_in_circle += (x*x + y*y <= 1.0f); 
    } // End for

    return 4.0f * points_in_circle / trials; 
} // End host_monte_carlo

/*
Mide el tiempo de inicio (start) y el tiempo de finalización (stop) del cálculo.
Llama a la función host_monte_carlo con el número total de intentos calculados.
Imprime el tiempo que tomó el cálculo y la estimación de π junto con el error respecto al valor conocido.
*/
int main (int argc, char *argv[]) { 
    clock_t start, stop; 
    start = clock(); 
    float pi_cpu = host_monte_carlo(BLOCKS * THREADS * TRIALS_PER_THREAD); 
    stop = clock(); 

    printf("CPU pi calculated in %f s.\n", (stop-start)/(float)CLOCKS_PER_SEC); 
    printf("CPU estimate of PI = %f [error of %f]\n", pi_cpu, pi_cpu - PI); 
    
    // End program
    return 0; 
} // End main