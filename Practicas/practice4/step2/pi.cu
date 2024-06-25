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

#include <stdlib.h> 
#include <stdio.h> 
#include <cuda.h> 
#include <math.h> 
#include <time.h> 
#include <curand_kernel.h> 
 
#define PI 3.1415926535
#define TRIALS_PER_THREAD 4096 
#define BLOCKS 256 
#define THREADS 256 
 
__global__ void gpu_monte_carlo(float *estimate, curandState *states) {

    unsigned int tid = threadIdx.x + blockDim.x * blockIdx.x; 
    int points_in_circle = 0; 
    float x, y; 

    curand_init(1234, tid, 0, &states[tid]);

    for(int i = 0; i < TRIALS_PER_THREAD; i++) { 
        x = curand_uniform(&states[tid]);
        y = curand_uniform(&states[tid]);
        points_in_circle += (x*x + y*y <= 1.0f); 
    }

    estimate[tid] = 4.0f * points_in_circle / (float) TRIALS_PER_THREAD; 
}
 
float host_monte_carlo(long trials) { 
    float x, y; 
    long points_in_circle = 0; 

    for(long i = 0; i < trials; i++) {
        x = rand() / (float) RAND_MAX; 
        y = rand() / (float) RAND_MAX; 
        points_in_circle += (x*x + y*y <= 1.0f); 
    } 

    return 4.0f * points_in_circle / trials;
} 
 
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
    } 
 
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

    return 0; 
}