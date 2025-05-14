// nvcc -arch=native test.cu -o test.exe

#include <cstdio>
#include <cuda_runtime.h>

__global__ void hello_from_gpu() {
    printf("Hello from GPU thread %d!\n", threadIdx.x);
}

int main() {
    int nDev = 0;
    cudaGetDeviceCount(&nDev);
    if (nDev == 0) { puts("No CUDA device!"); return 1; }

    cudaDeviceProp p;
    cudaGetDeviceProperties(&p, 0);
    printf("Device 0: %s\n", p.name);

    hello_from_gpu<<<1, 4>>>();   // один блок, 4 потока
    cudaDeviceSynchronize();      // дождаться вывода

    return 0;
}

// Device 0: NVIDIA GTX 1650
// Hello from GPU thread 0!
// Hello from GPU thread 1!
// Hello from GPU thread 2!
// Hello from GPU thread 3!
