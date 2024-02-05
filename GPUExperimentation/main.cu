#include <iostream>
#include "main.cuh"
#include <chrono>

__global__ void vectorAdd(int *a, int *b, int *c) {
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}

int computeCPU(int *a, int *b, int *c, int N) {
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    return elapsed.count();
}

int computeGPU(int *a, int *b, int *c, int N) {

    int *dev_a, *dev_b, *dev_c;

    size_t sizeOfArrayInBytes = N * sizeof(int);

    cudaMalloc((void** )&dev_a, sizeOfArrayInBytes);
    cudaMalloc((void** )&dev_b, sizeOfArrayInBytes);
    cudaMalloc((void** )&dev_c, sizeOfArrayInBytes);

    cudaMemcpy(dev_a, a, sizeOfArrayInBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, sizeOfArrayInBytes, cudaMemcpyHostToDevice);

    // Time GPU Kernal
    auto start = std::chrono::high_resolution_clock::now();

    const int NUM_BLOCKS = 1;
    const int NUM_THREADS = N;
    vectorAdd<<<NUM_BLOCKS, NUM_THREADS>>>(dev_a, dev_b, dev_c);
    cudaDeviceSynchronize();

    auto stop = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    cudaMemcpy(c, dev_c, sizeOfArrayInBytes, cudaMemcpyDeviceToHost);
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    return elapsed.count();
}


int main() {
    const int N = 100;
    int a[N], b[N], c_cpu[N], c_gpu[N];

    for(int i = 0; i < N; i++) {
        a[i] = 1;
        b[i] = 1;
    }

    int cpuTime = computeCPU(a, b, c_cpu, N);
    int gpuTime = computeGPU(a, b, c_gpu, N);

    std::cout << "CPU: " << cpuTime << std::endl;
    std::cout << "GPU: " << gpuTime << std::endl;
}



