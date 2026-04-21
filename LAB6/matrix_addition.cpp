#include <stdio.h>
#define N 4

__global__ void addMatrix(int *A, int *B, int *C) {
    int i = threadIdx.x;
    int j = threadIdx.y;

    int index = i * N + j;
    C[index] = A[index] + B[index];
}

int main() {
    int A[N*N], B[N*N], C[N*N];

    int *d_A, *d_B, *d_C;

    for (int i = 0; i < N*N; i++) {
        A[i] = 1;
        B[i] = 2;
    }

    cudaMalloc(&d_A, N*N*sizeof(int));
    cudaMalloc(&d_B, N*N*sizeof(int));
    cudaMalloc(&d_C, N*N*sizeof(int));

    cudaMemcpy(d_A, A, N*N*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, N*N*sizeof(int), cudaMemcpyHostToDevice);

    dim3 threads(N, N);

    addMatrix<<<1, threads>>>(d_A, d_B, d_C);

    cudaMemcpy(C, d_C, N*N*sizeof(int), cudaMemcpyDeviceToHost);

    printf("Matrix Addition Result:\n");
    for (int i = 0; i < N*N; i++) {
        printf("%d ", C[i]);
    }

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}