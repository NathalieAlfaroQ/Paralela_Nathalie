// Written by Barry Wilkinson, UNC-Charlotte. Pi.cu December 22, 2010.
// Derived somewhat from code developed by Patrick Rogers, UNC-C

// Para correr CUDA sin NVIDIA, seguir tutorial 
// https://colab.research.google.com/drive/1GJOfTp56OeQRdE4u2_S7pUNRcJb4ik9X?usp=sharing#scrollTo=4UgGjl7LSBeG

// Compilar nvcc -o pi pi.cu
// Ejecuta ./pi

/*
RESULTADO AL EJECUTAR:

lapz@LapzDesktop:~/nath$ nvcc -o pi pi.cu
lapz@LapzDesktop:~/nath$ ./pi

# of trials per thread = 4096, # of blocks = 256, # of threads/block = 256.

GPU pi calculated in 0.170903 s.

CPU pi calculated in 2.787307 s.

CUDA estimate of PI = 3.141582 [error of -0.000011]
CPU estimate of PI = 3.141581 [error of -0.000012]
*/

// Bibliotecas
#include <stdlib.h> 
#include <stdio.h> 
#include <cuda.h> 
#include <math.h> 
#include <time.h> 
#include <curand_kernel.h> 
 
// Constantes
#define PI 3.1415926535
#define TRIALS_PER_THREAD 4096 
#define BLOCKS 256 
#define THREADS 256 

/*
Realiza el cálculo de Monte Carlo en la GPU:
Inicializa el estado del generador de números aleatorios para cada hilo.
Genera puntos aleatorios en el cuadrado unitario (0,0) a (1,1).
Cuenta cuántos puntos caen dentro del círculo unitario de radio 1.
Calcula la estimación de π basada en la proporción de puntos dentro del círculo y
almacena el resultado en la memoria global.
*/
__global__ void gpu_monte_carlo(float *estimate, curandState *states) {

    unsigned int tid = threadIdx.x + blockDim.x * blockIdx.x; 
    int points_in_circle = 0; 
    float x, y; 
    curand_init(1234, tid, 0, &states[tid]);

    for(int i = 0; i < TRIALS_PER_THREAD; i++) { 
        x = curand_uniform(&states[tid]);
        y = curand_uniform(&states[tid]);
        points_in_circle += (x*x + y*y <= 1.0f); 
    } // End for

    estimate[tid] = 4.0f * points_in_circle / (float) TRIALS_PER_THREAD; 
} // End gpu_monte_carlo

/*
Genera puntos aleatorios en el cuadrado unitario (0,0) a (1,1).
Cuenta cuántos puntos caen dentro del círculo unitario de radio 1.
Calcula la estimación de π basada en la proporción de puntos dentro del círculo.
*/
float host_monte_carlo(long trials) { 
    float x, y; 
    long points_in_circle = 0; 

    for(long i = 0; i < trials; i++) {
        x = rand() / (float) RAND_MAX; 
        y = rand() / (float) RAND_MAX; 
        points_in_circle += (x*x + y*y <= 1.0f); 
    } // End for

    return 4.0f * points_in_circle / trials;
} // End host_monte_carlo

/*
Configura los parámetros de CUDA y asigna memoria en la GPU.
Llama a la función gpu_monte_carlo para realizar el cálculo en la GPU.
Transfiere los resultados de la GPU a la CPU y calcula la estimación final de π.
Mide y muestra el tiempo de cálculo en la GPU.
Llama a la función host_monte_carlo para realizar el cálculo en la CPU.
Mide y muestra el tiempo de cálculo en la CPU.
Compara y muestra las estimaciones de π y sus errores respecto al valor conocido.
Libera la memoria asignada en la GPU.
*/
int main (int argc, char *argv[]) {

    clock_t start, stop; 
    float host[BLOCKS * THREADS]; 
    float *dev; 
    curandState *devStates; 
 
    printf("# of trials per thread = %d, # of blocks = %d, # of threads/block = %d.\n", TRIALS_PER_THREAD, BLOCKS, THREADS); 
 
    start = clock(); 
 
    cudaMalloc((void **)&dev, BLOCKS * THREADS * sizeof(float));
    
    cudaMalloc((void **)&devStates, BLOCKS * THREADS * sizeof(curandState));
    
    gpu_monte_carlo<<<BLOCKS, THREADS>>>(dev, devStates); 

    cudaMemcpy(host, dev, BLOCKS * THREADS * sizeof(float), cudaMemcpyDeviceToHost);

    float pi_gpu = 0;

    for(int i = 0; i < BLOCKS * THREADS; i++) { 
        pi_gpu += host[i]; 
    } // End for
 
    pi_gpu /= (BLOCKS * THREADS); 
 
    stop = clock(); 
 
    printf("\nGPU pi calculated in %f s.\n", (stop-start)/(float)CLOCKS_PER_SEC); 
 
    start = clock(); 

    float pi_cpu = host_monte_carlo(BLOCKS * THREADS * TRIALS_PER_THREAD); 

    stop = clock();

    printf("\nCPU pi calculated in %f s.\n", (stop-start)/(float)CLOCKS_PER_SEC); 
 
    printf("\nCUDA estimate of PI = %f [error of %f]\n", pi_gpu, pi_gpu - PI); 

    printf("CPU estimate of PI = %f [error of %f]\n", pi_cpu, pi_cpu - PI); 
  
    cudaFree(dev);
    cudaFree(devStates);

    // End program
    return 0; 
} // End main