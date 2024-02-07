#include <iostream>
#include "vectorAdd.cuh"
#include <chrono>
#include <cmath>

__global__ void vectorAdd(int *a, int *b, int *c) {
    int i = threadIdx.x;
    c[i] = expf(a[i]) * expf(b[i]);
}

int computeCPU(int *a, int *b, int *c, int N) {
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < N; i++) {
        c[i] = exp(a[i]) * exp(b[i]);
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

    // std::cout << "array a: ";
    // for(int i = 0; i < N; i++) {
    //     std::cout << a[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "array b: ";
    // for(int i = 0; i < N; i++) {
    //     std::cout << b[i] << " ";
    // }
    // std::cout << std::endl;

    cudaError_t e1 = cudaMemcpy(dev_a, a, sizeOfArrayInBytes, cudaMemcpyHostToDevice);
    cudaError_t e2 = cudaMemcpy(dev_b, b, sizeOfArrayInBytes, cudaMemcpyHostToDevice);

    // std::cout << "copy to device: " << cudaGetErrorString(e1) << std::endl;
    // std::cout << "copy to device: " << cudaGetErrorString(e2) << std::endl;

    // Time GPU Kernal
    auto start = std::chrono::high_resolution_clock::now();

    const int NUM_BLOCKS = 1;
    const int NUM_THREADS = N;

    vectorAdd<<<NUM_BLOCKS, NUM_THREADS>>>(dev_a, dev_b, dev_c);
    cudaDeviceSynchronize();

    auto stop = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    cudaError_t e3 = cudaMemcpy(c, dev_c, sizeOfArrayInBytes, cudaMemcpyDeviceToHost);
    // std::cout << "copy to host: " << cudaGetErrorString(e3) << std::endl;

    // std::cout << "array c: ";
    // for(int i = 0; i < N; i++) {
    //     std::cout << c[i] << " ";
    // }
    // std::cout << std::endl;

    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    return elapsed.count();
}

int verify(int *a, int *b, int N) {
    for(int i = 0; i < N; i++) {
        if (a[i] != b[i]) {
            std::cout << "Mismatch at " << i << ": " << a[i] << " != " << b[i] << std::endl;
            return 0;
        }
    }
    return 1;
}


void singleRun() {
    const int N = 1000;
    int a[N], b[N], c_cpu[N], c_gpu[N];

    for(int i = 0; i < N; i++) {
        a[i] = i % 10;
        b[i] = i % 10;
    }

    int cpuTime = computeCPU(a, b, c_cpu, N);
    int gpuTime = computeGPU(a, b, c_gpu, N);

    std::cout << "Verification: " << verify(c_cpu, c_gpu, N) << std::endl;

    std::cout << "CPU: " << cpuTime << std::endl;
    std::cout << "GPU: " << gpuTime << std::endl;
}

int main() {
    singleRun();
    return 0;
}

