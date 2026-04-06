#include <stdio.h>
#define N 1024

__global__ void sumKernel(float *input, float *result) {
    int i = threadIdx.x;
    atomicAdd(result, input[i]);
}

int main() {
    float h_input[N], h_result = 0;
    float *d_input, *d_result;

    for (int i = 0; i < N; i++)
        h_input[i] = 1.0f;

    cudaMalloc((void**)&d_input, N * sizeof(float));
    cudaMalloc((void**)&d_result, sizeof(float));

    cudaMemcpy(d_input, h_input, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_result, &h_result, sizeof(float), cudaMemcpyHostToDevice);

    sumKernel<<<1, N>>>(d_input, d_result);

    cudaMemcpy(&h_result, d_result, sizeof(float), cudaMemcpyDeviceToHost);

    printf("Sum = %f\n", h_result);

    cudaFree(d_input);
    cudaFree(d_result);

    return 0;
}