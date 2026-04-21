#include <stdio.h>

__global__ void mergeKernel(int *arr, int *temp, int width, int n) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int start = tid * (2 * width);

    if (start >= n) return;

    int mid = min(start + width, n);
    int end = min(start + 2 * width, n);

    int i = start, j = mid, k = start;

    while (i < mid && j < end) {
        if (arr[i] < arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }

    while (i < mid) temp[k++] = arr[i++];
    while (j < end) temp[k++] = arr[j++];
}

void parallelMergeSort(int *arr, int n) {
    int *d_arr, *d_temp;

    cudaMalloc(&d_arr, n * sizeof(int));
    cudaMalloc(&d_temp, n * sizeof(int));

    cudaMemcpy(d_arr, arr, n * sizeof(int), cudaMemcpyHostToDevice);

    for (int width = 1; width < n; width *= 2) {
        mergeKernel<<<(n/(2*width)+1), 256>>>(d_arr, d_temp, width, n);
        cudaMemcpy(d_arr, d_temp, n * sizeof(int), cudaMemcpyDeviceToDevice);
    }

    cudaMemcpy(arr, d_arr, n * sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(d_arr);
    cudaFree(d_temp);
}