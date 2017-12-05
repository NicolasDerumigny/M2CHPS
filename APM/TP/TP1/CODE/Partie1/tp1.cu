#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void kernel(double *a, double *b, double *c, int N)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x; 
    
    if (i < N)
    {
	c[i] = a[i] + b[i];
    }
}

int main(int argc, char **argv)
{
    int N = 1000;
    int sz_in_bytes = N*sizeof(double);

    double *h_a, *h_b, *h_c;
    double *d_a, *d_b, *d_c;

    h_a = (double*)malloc(sz_in_bytes);
    h_b = (double*)malloc(sz_in_bytes);
    h_c = (double*)malloc(sz_in_bytes);

    // Initiate values on h_a and h_b
    for(int i = 0 ; i < N ; i++)
    {
	h_a[i] = 1./(1.+i);
	h_b[i] = (i-1.)/(i+1.);
    }

    // 3-arrays allocation on device 
    cudaMalloc((void**)&d_a, sz_in_bytes);
    cudaMalloc((void**)&d_b, sz_in_bytes);
    cudaMalloc((void**)&d_c, sz_in_bytes);

    // copy on device values pointed on host by h_a and h_b
    // (the new values are pointed by d_a et d_b on device)
    cudaMemcpy(d_a, h_a, sz_in_bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, sz_in_bytes, cudaMemcpyHostToDevice);

    dim3  dimBlock(64, 1, 1);
    dim3  dimGrid((N + dimBlock.x - 1)/dimBlock.x, 1, 1);
    kernel<<<dimGrid , dimBlock>>>(d_a, d_b, d_c, N);

    // Result is pointed by d_c on device
    // Copy this result on host (result pointed by h_c on host)
    cudaMemcpy(h_c, d_c, sz_in_bytes, cudaMemcpyDeviceToHost);

    // freeing on device 
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    free(h_a);
    free(h_b);
    free(h_c);

    return 0;
}
