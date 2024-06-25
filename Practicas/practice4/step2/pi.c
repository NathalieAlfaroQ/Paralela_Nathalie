// Compilar gcc -o pi pi.c
// Ejecuta ./pi

/*
RESULTADO AL EJECUTAR:

CPU pi calculated in 2.769034 s.
CPU estimate of PI = 3.141607 [error of 0.000014]
*/

#include <stdlib.h> 
#include <stdio.h> 
#include <cuda.h> 
#include <math.h> 
#include <time.h> 
 
#define PI 3.1415926535  // known value of pi 
#define TRIALS_PER_THREAD 4096 
#define BLOCKS 256 
#define THREADS 256 
 
float host_monte_carlo(long trials) { 
    float x, y; 
    long points_in_circle; 
    for(long i = 0; i < trials; i++) {

        x = rand() / (float) RAND_MAX; 
        y = rand() / (float) RAND_MAX; 
        points_in_circle += (x*x + y*y <= 1.0f); 
        
    } 
    return 4.0f * points_in_circle / trials; 
}

int main (int argc, char *argv[]) { 
    clock_t start, stop; 
     
    start = clock(); 
    float pi_cpu = host_monte_carlo(BLOCKS * THREADS * TRIALS_PER_THREAD); 
    stop = clock(); 
    printf("CPU pi calculated in %f s.\n", (stop-start)/(float)CLOCKS_PER_SEC); 
 
    printf("CPU estimate of PI = %f [error of %f]\n", pi_cpu, pi_cpu - PI); 
     
    return 0; 
}