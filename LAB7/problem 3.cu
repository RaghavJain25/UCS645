#include <stdio.h>
#include <cuda.h>

#define N 1024

__global__ void vectorAdd(float *A, float *B, float *C) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    if (i < N) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    float A[N], B[N], C[N];
    float *d_A, *d_B, *d_C;

    for (int i = 0; i < N; i++) {
        A[i] = i;
        B[i] = i * 2;
    }

    cudaMalloc(&d_A, N*sizeof(float));
    cudaMalloc(&d_B, N*sizeof(float));
    cudaMalloc(&d_C, N*sizeof(float));

    cudaMemcpy(d_A, A, N*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, N*sizeof(float), cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    vectorAdd<<<(N+255)/256, 256>>>(d_A, d_B, d_C);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms;
    cudaEventElapsedTime(&ms, start, stop);

    cudaMemcpy(C, d_C, N*sizeof(float), cudaMemcpyDeviceToHost);

    printf("Kernel Time: %f ms\n", ms);

    // Device properties
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);

    double memoryClock = prop.memoryClockRate * 1e3;
    double busWidth = prop.memoryBusWidth;
    double bandwidth = 2.0 * memoryClock * (busWidth / 8) / 1e9;

    printf("Theoretical Bandwidth: %f GB/s\n", bandwidth);

    // Measured bandwidth
    double bytes = 3 * N * sizeof(float);
    double measuredBW = bytes / (ms / 1000.0) / 1e9;

    printf("Measured Bandwidth: %f GB/s\n", measuredBW);

    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);

    return 0;
}