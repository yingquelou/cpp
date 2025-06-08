#include <stdio.h>
#include <cuda_runtime.h>
__global__ void kernel() {
    printf("Hello from GPU!\n");
}

int main() {
    kernel<<<1, 1>>>();
    cudaDeviceSynchronize();
    return 0;
}