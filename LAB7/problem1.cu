#include <stdio.h>

#define N 1024

__global__ void computeSum(int *result_iter, int *result_formula) {
    int tid = threadIdx.x;

    if (tid == 0) {
        // Iterative sum
        int sum = 0;
        for (int i = 1; i <= N; i++) {
            sum += i;
        }
        *result_iter = sum;
    }

    if (tid == 1) {
        // Formula sum
        *result_formula = (N * (N + 1)) / 2;
    }
}

int main() {
    int h_iter, h_formula;
    int *d_iter, *d_formula;

    cudaMalloc((void**)&d_iter, sizeof(int));
    cudaMalloc((void**)&d_formula, sizeof(int));

    computeSum<<<1, 2>>>(d_iter, d_formula);

    cudaMemcpy(&h_iter, d_iter, sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(&h_formula, d_formula, sizeof(int), cudaMemcpyDeviceToHost);

    printf("Iterative Sum: %d\n", h_iter);
    printf("Formula Sum: %d\n", h_formula);

    cudaFree(d_iter);
    cudaFree(d_formula);

    return 0;
}