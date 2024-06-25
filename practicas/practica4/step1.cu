// g++ -o step1 step1.cpp
// ./step1
// Max error: 0.000000

/*
nvcc step1.cu -o step1
> ./step1
Max error: 0.000000

https://developer.nvidia.com/blog/even-easier-introduction-cuda/

nvprof ./step1

==12739== NVPROF is profiling process 12739, command: ./step1
Max error: 0
==12739== Profiling application: ./step1
==12739== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:  100.00%  180.44ms         1  180.44ms  180.44ms  180.44ms  add(int, float*, float*)
      API calls:   51.19%  189.91ms         2  94.956ms  43.217us  189.87ms  cudaMallocManaged
                   48.65%  180.48ms         1  180.48ms  180.48ms  180.48ms  cudaDeviceSynchronize
                    0.10%  378.76us         2  189.38us  173.49us  205.27us  cudaFree
                    0.04%  159.70us       101  1.5810us     204ns  62.573us  cuDeviceGetAttribute
                    0.01%  38.291us         1  38.291us  38.291us  38.291us  cudaLaunchKernel
                    0.00%  17.037us         1  17.037us  17.037us  17.037us  cuDeviceGetName
                    0.00%  10.609us         1  10.609us  10.609us  10.609us  cuDeviceGetPCIBusId
                    0.00%  8.2550us         1  8.2550us  8.2550us  8.2550us  cuDeviceTotalMem
                    0.00%  1.9300us         3     643ns     306ns  1.2870us  cuDeviceGetCount
                    0.00%  1.2280us         2     614ns     269ns     959ns  cuDeviceGet
                    0.00%     457ns         1     457ns     457ns     457ns  cuDeviceGetUuid

==12739== Unified Memory profiling result:
Device "NVIDIA GeForce GTX 1060 3GB (0)"
   Count  Avg Size  Min Size  Max Size  Total Size  Total Time  Name
      48  170.67KB  4.0000KB  0.9961MB  8.000000MB  737.0930us  Host To Device
      24  170.67KB  4.0000KB  0.9961MB  4.000000MB  335.4710us  Device To Host
      12         -         -         -           -  2.956630ms  Gpu page fault groups
Total CPU Page faults: 36
*/

#include <iostream>
#include <math.h>

// CUDA Kernel function to add the elements of two arrays on the GPU

// function to add the elements of two arrays
/*__global__ void add(int n, float *x, float *y)
{
  int index = threadIdx.x;
  int stride = blockDim.x;
  for (int i = 0; i < n; i++)
      y[i] = x[i] + y[i];
}*/

__global__ void add(int n, float *x, float *y)
{
  int index = threadIdx.x;
  int stride = blockDim.x;
  for (int i = index; i < n; i += stride)
    y[i] = x[i] + y[i];
}

int main(void)
{
  // 2**20
  int N = 1 << 20; // 1M elements

  // Allocate Unified Memory -- accessible from CPU or GPU
  float *x, *y;
  cudaMallocManaged(&x, N * sizeof(float));
  cudaMallocManaged(&y, N * sizeof(float));

  // float *x = new float[N];
  // float *y = new float[N];

  // initialize x and y arrays on the host
  for (int i = 0; i < N; i++)
  {
    x[i] = 1.0f;
    y[i] = 2.0f;
  }

  // Run kernel on 1M elements on the CPU
  // add(N, x, y);
  // add<<<1, 1>>>(N, x, y);
  add<<<1, 256>>>(N, x, y);

  // Wait for GPU to finish before accessing on host
  cudaDeviceSynchronize();

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (int i = 0; i < N; i++)
    maxError = fmax(maxError, fabs(y[i] - 3.0f));
  std::cout << "Max error: " << maxError << std::endl;

  // Free memory
  // delete [] x;
  // delete [] y;

  // Free memory
  cudaFree(x);
  cudaFree(y);

  return 0;
}