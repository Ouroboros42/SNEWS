#include <iostream>
#include <cstdlib>
#include <chrono>


__global__ void matrixMultiply(int *a, int *b, int *c, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        float sum = 0.0f;
        for (int k = 0; k < N; ++k) {
            sum += a[row * N + k] * b[k * N + col];
        }
        c[row*N + col] = sum;
    }
}


int computeCPU(int *a, int *b, int *c, int N) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += a[i * N + k] * b[k * N + j];
            }
            c[i * N + j] = sum;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    return elapsed.count();
}



int computeGPU(int *a, int *b, int *c, int N) {

    // allocate memory for gpu
    int *dev_a, *dev_b, *dev_c;
    size_t sizeOfMatrixInBytes = N * N * sizeof(int);

    cudaMalloc((void **)&dev_a, sizeOfMatrixInBytes);
    cudaMalloc((void **)&dev_b, sizeOfMatrixInBytes);
    cudaMalloc((void **)&dev_c, sizeOfMatrixInBytes);

    cudaError_t e1 =  cudaMemcpy(dev_a, a, sizeOfMatrixInBytes, cudaMemcpyHostToDevice);
    cudaError_t e2 = cudaMemcpy(dev_b, b, sizeOfMatrixInBytes, cudaMemcpyHostToDevice);

    if (e1 != cudaSuccess) {
        std::cerr << "Error: " << cudaGetErrorString(e1) << std::endl;
    }
    if (e2 != cudaSuccess) {
        std::cerr << "Error: " << cudaGetErrorString(e2) << std::endl;
    }

    // Define grid and block dimensions
    dim3 threadsPerBlock(N, N);
    dim3 blocksPerGrid(1, 1);
    if (N*N > 512){
        threadsPerBlock.x = 512;
        threadsPerBlock.y = 512;
        blocksPerGrid.x = ceil(double(N)/double(threadsPerBlock.x));
        blocksPerGrid.y = ceil(double(N)/double(threadsPerBlock.y));
    }

    // Time GPU Kernal
    auto start = std::chrono::high_resolution_clock::now();

    matrixMultiply<<<blocksPerGrid, threadsPerBlock>>>(dev_a, dev_b, dev_c, N);
    cudaDeviceSynchronize();

    auto stop = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // Copy result back to host
    cudaError_t e3 = cudaMemcpy(c, dev_c, sizeOfMatrixInBytes, cudaMemcpyDeviceToHost);

    if (e3 != cudaSuccess) {
        std::cerr << "Error: " << cudaGetErrorString(e3) << std::endl;
    }

    // Free device memory
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    return elapsed.count();
}

bool verifyResults(int *c_cpu, int *c_gpu, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (c_cpu[i * N + j] != c_gpu[i * N + j]) {
                std::cerr << "Mismatch at index " << i << ", " << j << ": " << c_cpu[i * N + j] << " != " << c_gpu[i * N + j] << std::endl;
                return false;
            }
        }
    }
    return true;
}

void printArrays(int *a, int *b, int *c_cpu, int *c_gpu, int N) {
    std::cout << "Matrix A: " << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << a[N*i + j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Matrix B: " << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << b[N*i + j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Matrix C (CPU): " << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << c_cpu[N*i + j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Matrix C (GPU): " << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << c_gpu[N*i + j] << " ";
        }
        std::cout << std::endl;
    }
}



int main() {

    const int N = 16;
    int a[N*N], b[N*N], c_cpu[N*N], c_gpu[N*N];

    // Initialize matrices a and b
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            a[N*i + j] = i + j;
            b[N*i + j] = i - j;
        }
    }

    int cpuTime = computeCPU((int *)a, (int *)b, (int *)c_cpu, N);
    std::cout << "CPU: " << cpuTime << std::endl;

    int gpuTime = computeGPU((int *)a, (int *)b, (int *)c_gpu, N);
    std::cout << "GPU: " << gpuTime << std::endl;

    std::cout << "Verification: " << verifyResults((int *)c_cpu, (int *)c_gpu, N) << std::endl;

    //printArrays((int *)a, (int *)b, (int *)c_cpu, (int *)c_gpu, N);
}
